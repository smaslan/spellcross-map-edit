#include "spell_map_event.h"
#include <stdexcept>
#include <tuple>

using namespace std;

SpellMapEventRec::SpellMapEventRec()
{
	evt_type = EVT_VOID;
	is_done = false;
	next = NULL;
	hide = false;
}
SpellMapEventRec::~SpellMapEventRec()
{
	for(auto & unit : units)
		delete unit.unit;
	units.clear();
}
int SpellMapEventRec::isMissionStart()
{
	return(evt_type == EVT_MISSION_START);
}
int SpellMapEventRec::isSeePlace()
{
	return(evt_type == EVT_SEE_PLACE);
}
int SpellMapEventRec::isDone()
{
	if(is_done)
		return(is_done);
	for(auto & unit : units)
		if(!unit.is_placed)
			return(false);
	for(auto& text : texts)
		if(!text.is_done)
			return(false);
	is_done = true;
	return(is_done);
}


// create events list
SpellMapEvents::SpellMapEvents(int x_size, int y_size)
{
    this->x_size = x_size;
    this->y_size = y_size;
	next_index = 0;
}
// cleanup
SpellMapEvents::~SpellMapEvents()
{
	for(auto & evt : events)
		delete evt;
	events.clear();
}

// convert map coordinates
MapXY SpellMapEvents::ConvXY(int mxy)
{
    MapXY pos(mxy%x_size, mxy/x_size);
    if(pos.y >= y_size)
        pos = {-1,-1};
    return(pos);
}
int SpellMapEvents::ConvXY(MapXY pos)
{
	return(pos.x + pos.y*x_size);
}


// get next free unit ID and update value
int SpellMapEvents::GetNextID()
{
	int index = next_index;
	next_index++;
	if(next_index >= 48 && next_index <= 99)
		next_index = 100; // skip reserved indices
	return(index);
}

// tries to find existing event with identical parameters
SpellMapEventRec* SpellMapEvents::FindEvent(int type,int probab,MapXY pos)
{
	for(auto & evt : events)
	{
		if(type == SpellMapEventRec::EVT_MISSION_START && type == evt->evt_type && probab == 100 && probab == evt->probability)
			return(evt);
		if(type == SpellMapEventRec::EVT_SEE_PLACE && type == evt->evt_type && probab && probab == evt->probability && pos == evt->position)
			return(evt);
	}
	return(NULL);
}

// parse and add given event
int SpellMapEvents::AddEvent(SpellData *data, SpellDEF* def, SpellDefCmd* cmd)
{
	SpellMapEventRec *evt = NULL;
	MapXY target_position;
	int event_data_index;
	int event_probability;
	int event_type;
	int is_new_event = false;
	if(cmd->parameters->at(0).compare("SeePlace") == 0)
	{
		// SeePlace(target_position, event_data_index, event_probability)
		event_type = SpellMapEventRec::EVT_SEE_PLACE;

		// event position		
		int xy = atoi(cmd->parameters->at(1).c_str());
		target_position = ConvXY(xy);
		
		// event index
		event_data_index = atoi(cmd->parameters->at(2).c_str());

		// probability
		event_probability = atoi(cmd->parameters->at(3).c_str());

		// try find another matching event
		evt = FindEvent(SpellMapEventRec::EVT_SEE_PLACE, event_probability, target_position);
	}
	else if(cmd->parameters->at(0).compare("MissionStart") == 0)
	{
		// MissionStart(event_data_index, event_probability)
		event_type = SpellMapEventRec::EVT_MISSION_START;

		// event index
		event_data_index = atoi(cmd->parameters->at(1).c_str());

		// probability
		event_probability = atoi(cmd->parameters->at(2).c_str());

		// try find another matching event
		evt = FindEvent(SpellMapEventRec::EVT_MISSION_START,event_probability);
	}
	else
	{
		// unknown or not implemented
		return(1);
	}	
	if(!evt)
	{
		// make new event if not existing match found
		is_new_event = true;
		evt = new SpellMapEventRec();
		evt->evt_type = event_type;
		evt->position = target_position;
		evt->probability = event_probability;
		events.push_back(evt);
	}
	

	// try parse event data
	string event_data_header = "EventData(" + std::to_string(event_data_index) + ")";
	SpellDefSection* event_data = def->GetSection(event_data_header);

	// for each item in event data (there can be only one in original Spellcross):
	for(int evid = 0; evid < event_data->Size(); evid++)
	{
		SpellDefCmd* evcmd = (*event_data)[evid];

		if(evcmd->name.compare("AddSpecialUnit") == 0)
		{
			// --- AddSpecialUnit(unit_order, unit_id, position, experience, man_count, name) ---				
			MapUnit* unit = new MapUnit();

			// unit index within map (identifier)
			unit->id = -1;
			
			// decode unit type
			if(evcmd->parameters->at(0).compare("EnemyUnit") == 0)
			{
				unit->is_enemy = 1;
			}
			else if(evcmd->parameters->at(0).compare("MissionUnit") == 0)
			{
				unit->type = MapUnitType::MissionUnit;
			}
			else if(evcmd->parameters->at(0).compare("SpecUnit1") == 0)
			{
				unit->type = MapUnitType::SpecUnit;
				unit->id = 48; // this idiocy is original Spellcross hard defined designation for SpecUnit1
			}
			else if(evcmd->parameters->at(0).compare("SpecUnit2") == 0)
			{
				unit->type = MapUnitType::SpecUnit;
				unit->id = 49; // this idiocy is original Spellcross hard defined designation for SpecUnit2
			}
			
			// generate new unit ID
			if(unit->id < 0)
				unit->id = GetNextID();

			// unit type index			
			unit->type_id = atoi(evcmd->parameters->at(1).c_str());

			// desired unit position
			int xy = atoi(evcmd->parameters->at(2).c_str());
			unit->coor = ConvXY(xy);
			
			// experience level
			unit->experience = atoi(evcmd->parameters->at(3).c_str());

			// man count (health)
			unit->man = atoi(evcmd->parameters->at(4).c_str());

			// unit active (to change in game mode)
			unit->is_active = 0;

			// try fetch unit record from spelldata
			unit->unit = data->units->GetUnit(unit->type_id);
			if(!unit->unit)
			{
				delete event_data;
				delete unit;
				if(is_new_event)
				{
					events.pop_back();
					delete evt;
				}
				return(1);
			}

			// copy unit name
			strcpy_s(unit->name,sizeof(unit->name),unit->unit->name);
			auto& custom_name = evcmd->parameters->at(5);
			if(custom_name.size() && custom_name.compare("-")!=0)
				strcpy_s(unit->name,sizeof(unit->name),custom_name.c_str());

			// default action points
			unit->ResetAP();
			unit->ResetTurnsCounter();
			unit->ClearDigLevel();
			unit->commander_id = 0;
			unit->is_commander = false;
			unit->morale = 100;
			unit->is_event = true;			
			unit->is_active = evt->isMissionStart();

			// add unit to list
			evt->units.emplace_back(unit);
		}
		else if(evcmd->name.compare("EventText") == 0)
		{
			// --- EventText(text_fs_name) ---
			if(evcmd->parameters->size() < 1)
			{
				delete event_data;
				if(is_new_event)
				{
					events.pop_back();
					delete evt;
				}
				return(1);
			}
			
			// try to find text in string list
			auto text_name = evcmd->parameters->at(0);
			auto text = data->texts->GetText(text_name);
			if(!text)
			{
				delete event_data;
				if(is_new_event)
				{
					events.pop_back();
					delete evt;
				}
				return(1);
			}
			evt->texts.emplace_back(text);
		}
	}
	delete event_data;

	return(0);
}


SpellMapEventRec** SpellMapEvents::EventMap(MapXY pos)
{
	return(&events_map[pos.x + pos.y*x_size]);
}

// reset event triggers and build events map for fast search (call at least once at mission start)
void SpellMapEvents::ResetEvents()
{
	// reset all events
	for(auto & evt : events)
	{
		evt->is_done = false;
		evt->next = NULL;
		evt->hide = evt->probability < rand()%101;
		for(auto & text : evt->texts)
			text.is_done = false;
		for(auto& unit : evt->units)
			unit.is_placed = false;
	}
	
	// make map of events
	events_map.assign(x_size*y_size, NULL);
	for(auto& evt : events)
	{
		// only see place events
		if(!evt->isSeePlace())
			continue;

		// place event to map (or at the end of chain of events for the position)
		auto tile = EventMap(evt->position);
		if(!*tile)
		{
			*tile = evt;
		}
		else
		{
			SpellMapEventRec *prev = *tile;
			while(prev)
				prev = prev->next;
			prev = evt;
		}
	}
}

// get first event for given position
SpellMapEventRec* SpellMapEvents::GetEvent(MapXY pos)
{
	return(*EventMap(pos));
}

// fast check if there is some undone event at the position
int SpellMapEvents::CheckEvent(MapXY pos)
{
	return(CheckEvent(ConvXY(pos)));
}
int SpellMapEvents::CheckEvent(int pos)
{
	auto evt = events_map[pos];
	if(!evt)
		return(false);
	if(evt->is_done)
		return(false);
	return(true);
}

// get list of events for given position, only not executed
SpellMapEventsList SpellMapEvents::GetEvents(MapXY pos, bool clear)
{
	return(GetEvents(ConvXY(pos)));
}
SpellMapEventsList SpellMapEvents::GetEvents(int pos,bool clear)
{	
	// make list of events
	vector<SpellMapEventRec*> list;
	auto evt = events_map[pos];
	while(evt)
	{
		if(!evt->is_done && evt->isSeePlace() && !evt->hide)
			list.push_back(evt);
		if(clear)
			evt->is_done = true;
		evt = evt->next;
	}		
	return(list);
}

// returns list of events associated with mission start, eventually clears them, is skips executed ones or below probability ones
SpellMapEventsList SpellMapEvents::GetMissionStartEvent(bool clear)
{
	vector<SpellMapEventRec*> list;
	for(auto & evt : events)
	{
		if(!evt->isMissionStart())
			continue;
		if(evt->is_done)
			continue;
		if(clear)
			evt->is_done = true;
		if(evt->hide)
			continue;
		list.push_back(evt);
	}
	return(list);
}