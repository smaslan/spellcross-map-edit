#include "spell_map_event.h"
#include <stdexcept>
#include <tuple>
#include <algorithm>

using namespace std;

SpellMapEventRec::SpellMapEventRec()
{
	evt_type = EVT_VOID;
	is_objective = false;
	trig_unit = NULL;
	trig_unit_id = -1;
	probability = 100;
	type_name = EvtNames[evt_type];
	is_done = false;
	next = NULL;
	hide = false;
	in_placement = false;
}
SpellMapEventRec::SpellMapEventRec(SpellMapEventRec* rec)
{
	*this = *rec;
	
	// duplicate unit objects
	for(auto& unit : units)
	{
		unit.unit = new MapUnit(*unit.unit);
		unit.unit->map_event = this;
	}
}
SpellMapEventRec::~SpellMapEventRec()
{
	for(auto & unit : units)
		delete unit.unit;
	units.clear();
	
	// unlink SeeUnit() event link to unit
	if(trig_unit)
	{
		trig_unit->trig_event = NULL;
		trig_unit = NULL;
	}
}
int SpellMapEventRec::AddUnit(MapUnit* unit)
{	
	units.push_back(unit);
	// make unit link to this event
	unit->map_event = this;
	unit->is_event = true;
	return(0);
}
MapUnit *SpellMapEventRec::ExtractUnit(MapUnit* unit)
{	
	for(auto it = begin(units); it != end(units); it++)
		if(it->unit == unit)
		{
			// remove from list, but not delete
			units.erase(it);
			// unlink from this event
			unit->map_event = NULL;
			unit->is_event = false;
			// return unit
			return(unit);
		}
	// not found
	return(NULL);
}
void SpellMapEventRec::ClearUnits()
{
	for(auto& unit : units)
		delete unit.unit;
	units.clear();	
}
void SpellMapEventRec::ClearTexts()
{
	texts.clear();
}
int SpellMapEventRec::isMissionStart()
{
	return(evt_type == EVT_MISSION_START);
}
int SpellMapEventRec::isSeePlace()
{
	return(evt_type == EVT_SEE_PLACE);
}
int SpellMapEventRec::isSeeUnit()
{
	return(evt_type == EVT_SEE_UNIT);
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
int SpellMapEventRec::hasTargetUnit()
{
	return(evt_type == EVT_SEE_UNIT || evt_type == EVT_SAVE_UNIT || evt_type == EVT_TRANSPORT_UNIT);
}
int SpellMapEventRec::hasPosition()
{
	return(evt_type == EVT_SEE_PLACE || evt_type == EVT_DESTROY_OBJ);
}

// return list of text names of event types
vector<string> SpellMapEventRec::GetEventTypes()
{
	vector<string> list;
	for(auto & type : EvtNames)
		list.push_back(type);
	return(list);
}

// set type (and modify type string)
int SpellMapEventRec::SetType(int type_id)
{
	if(type_id < 0 || type_id >= sizeof(EvtNames))
		return(1);
	evt_type = type_id;
	type_name = EvtNames[type_id];
	return(0);
}



// create events list
SpellMapEvents::SpellMapEvents(int x_size, int y_size,vector<MapUnit*>& map_units, int &game_mode)
	: is_game_mode(game_mode), map_units(map_units)
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
SpellMapEventRec* SpellMapEvents::FindEvent(int type,int probab,int trig_unit)
{
	for(auto& evt : events)
	{
		if(type == SpellMapEventRec::EVT_SEE_UNIT && type == evt->evt_type && probab && probab == evt->probability && trig_unit == evt->trig_unit_id)
			return(evt);
	}
	return(NULL);
}
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

// inset new event to list
SpellMapEventRec* SpellMapEvents::AddEvent(SpellMapEventRec* event)
{
	events.push_back(event);	

	//###todo: check duplcites?

	return(event);
}

// remove event (returns original object, user deletes it!)
SpellMapEventRec* SpellMapEvents::RemoveEvent(SpellMapEventRec* event)
{	
	auto id = find(events.begin(), events.end(), event);
	if(id == events.end())
		return(NULL);

	// remove from list, but not delete object (user must do it)
	events.erase(id);
	return(event);
}
// erase event (deletes it, places linked units directly to map units)
int SpellMapEvents::EraseEvent(SpellMapEventRec* event)
{
	// try remove event from the list
	auto evt = RemoveEvent(event);
	if(!evt)
		return(1);

	// place all linked units to the map
	for(auto & rec : evt->units)
	{
		// extract unit from event units list
		auto unit = evt->ExtractUnit(rec.unit);
		if(unit->is_enemy)
		{
			// enemy units can go directly to the map
			map_units.push_back(unit);
		}
		else
		{
			// alinace can be only MissionStart()
			AddMissionStartUnit(unit);
		}		
	}

	delete evt;
	return(0);
}

// clear all events
void SpellMapEvents::ClearEvents()
{
	events_map.assign(x_size*y_size,NULL);
	for(auto& evt : events)
		delete evt;
	events.clear();
}


// parse and add mission objectives (they are treated as events for simplicity)
//AddMissionObjective(DestroyAllUnits,Zni‡en¡ v¨ech jednotek)
//AddMissionObjective(TransportUnit,48,Probojovat se na z kladnu)
//AddMissionObjective(SaveUnit,48,Zachr nit mjr.Davidsona)
//AddMissionObjective(DestroyUnit,50,Zabít Daertora)
//AddMissionObjective(SeePlace,648,Lokalizovat sklad ‡.1)
//AddMissionObjective(DestroyObject,1966,Zni‡it vchod do jeskynˆ)
int SpellMapEvents::AddMissionObjective(SpellData* data,SpellDEF* def,SpellDefCmd* cmd)
{
	
	// identify type of objective
	const struct{
		const char *name;
		int type;
	} types[] = {
		{"DestroyAllUnits",SpellMapEventRec::EvtTypes::EVT_DESTROY_ALL},
		{"DestroyUnit",SpellMapEventRec::EvtTypes::EVT_DESTROY_UNIT},
		{"DestroyObject",SpellMapEventRec::EvtTypes::EVT_DESTROY_OBJ},
		{"SeePlace",SpellMapEventRec::EvtTypes::EVT_SEE_PLACE},
		{"SaveUnit",SpellMapEventRec::EvtTypes::EVT_SAVE_UNIT},
		{"TransportUnit",SpellMapEventRec::EvtTypes::EVT_TRANSPORT_UNIT},
		{NULL,SpellMapEventRec::EvtTypes::EVT_VOID}};	
	int event_type = SpellMapEventRec::EvtTypes::EVT_VOID;
	auto *ptype = &types[0];
	while(ptype->name)
		if(strcmp(ptype->name,cmd->parameters->at(0).c_str()) == 0)
		{
			event_type = ptype->type;
			break;
		}
		else
			ptype++;
	if(event_type == SpellMapEventRec::EvtTypes::EVT_VOID)
	{
		// type unknown
		return(1);
	}		

	// decode parameters
	wstring label = L"";
	MapXY target_position;
	int unit_index = -1;
	if(event_type == SpellMapEventRec::EvtTypes::EVT_TRANSPORT_UNIT || event_type == SpellMapEventRec::EvtTypes::EVT_SAVE_UNIT || event_type == SpellMapEventRec::EvtTypes::EVT_DESTROY_UNIT)
	{
		// target unit index
		unit_index = atoi(cmd->parameters->at(1).c_str());

		// objective label
		label = char2wstringCP895(cmd->parameters->at(2).c_str());
	}
	else if(event_type == SpellMapEventRec::EvtTypes::EVT_DESTROY_ALL)
	{
		// objective label
		label = char2wstringCP895(cmd->parameters->at(1).c_str());
	}
	else if(event_type == SpellMapEventRec::EvtTypes::EVT_DESTROY_OBJ || event_type == SpellMapEventRec::EvtTypes::EVT_SEE_PLACE)
	{
		// event position		
		target_position = ConvXY(atoi(cmd->parameters->at(1).c_str()));
		
		// objective label
		label = char2wstringCP895(cmd->parameters->at(2).c_str());
	}

	// make new object-event
	SpellMapEventRec *evt = new SpellMapEventRec();
	evt->evt_type = event_type;
	evt->type_name = cmd->parameters->at(0);
	evt->label = label;
	evt->position = target_position;
	evt->probability = 100;
	evt->trig_unit_id = unit_index;
	evt->is_objective = true;	
	events.push_back(evt);

	return(0);
}

// parse and add given event
int SpellMapEvents::AddSpecialEvent(SpellData *data, SpellDEF* def, SpellDefCmd* cmd)
{
	SpellMapEventRec *evt = NULL;
	MapXY target_position;
	int event_data_index;
	int event_probability;
	int event_type;
	int unit_index = -1;
	int is_new_event = false;
	if(cmd->parameters->at(0).compare("SeePlace") == 0)
	{
		// AddSpecialEvent(SeePlace, target_position, event_data_index, event_probability)
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
		// AddSpecialEvent(MissionStart, event_data_index, event_probability)
		event_type = SpellMapEventRec::EVT_MISSION_START;

		// event index
		event_data_index = atoi(cmd->parameters->at(1).c_str());

		// probability
		event_probability = atoi(cmd->parameters->at(2).c_str());

		// try find another matching event
		evt = FindEvent(SpellMapEventRec::EVT_MISSION_START,event_probability);
	}
	else if(cmd->parameters->at(0).compare("SeeUnit") == 0)
	{
		// AddSpecialEvent(SeeUnit, unit_index, event_data_index, event_probability)
		event_type = SpellMapEventRec::EVT_SEE_UNIT;

		// unit to see index
		unit_index = atoi(cmd->parameters->at(1).c_str());

		// event index
		event_data_index = atoi(cmd->parameters->at(2).c_str());

		// probability
		event_probability = atoi(cmd->parameters->at(3).c_str());

		// try find another matching event
		evt = FindEvent(SpellMapEventRec::EVT_SEE_UNIT,event_probability,unit_index);
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
		evt->trig_unit_id = unit_index;
		events.push_back(evt);
	}
	
	// event type name
	evt->type_name = cmd->parameters->at(0);
	

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

			// store back link to event to the unit
			unit->map_event = evt;

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
		if(evt->isSeeUnit() && evt->trig_unit)
		{
			evt->trig_unit->trig_event = NULL;
			evt->trig_unit = NULL;
		}
	}

	// make list of all existing units (map units and events' units)
	vector<MapUnit*> units_list = map_units;
	for(auto& evt : events)
		for(auto & unit : evt->units)
			units_list.push_back(unit.unit);
	
	// make map of events
	events_map.assign(x_size*y_size, NULL);
	for(auto& evt : events)
	{		
		if(evt->isSeePlace())
		{
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
		else if(evt->isSeeUnit())
		{
			// look for target unit and assign ref
			for(auto & unit : units_list)
				if(unit->id == evt->trig_unit_id)
				{
					// target unit found
					unit->trig_event = evt;
					evt->trig_unit = unit;
				}			
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
		if(!evt->is_done && evt->isSeePlace() && (!evt->hide || !is_game_mode))
			list.push_back(evt);
		if(clear)
			evt->is_done = true;
		evt = evt->next;
	}		
	return(list);
}
// get all events list
vector<SpellMapEventRec*>& SpellMapEvents::GetEvents()
{
	return(events);
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

// insert unit to first possible MissionStart() event or create new MissionStart() event
int SpellMapEvents::AddMissionStartUnit(MapUnit* unit,int probab)
{
	if(unit->map_event && unit->map_event->isMissionStart())
		return(1);

	// remove unit from other event eventually
	if(unit->map_event)
	{
		unit->map_event->ExtractUnit(unit);
		unit->map_event = NULL;
	}

	// try to find some suitable MissionStart() event
	SpellMapEventRec *target_evt = NULL;
	for(auto & evt : events)
		if(evt->isMissionStart() && evt->probability == 100)
			target_evt = evt;
	if(!target_evt)
	{
		// not found: make one
		target_evt = new SpellMapEventRec();
		target_evt->SetType(SpellMapEventRec::EvtTypes::EVT_MISSION_START);
		target_evt->probability = probab;
		events.push_back(target_evt);
		// resort events
		ResetEvents();
	}

	// add unit to MissionStart() event
	target_evt->AddUnit(unit);

	return(0);
}

// add new SeeUnit() event
SpellMapEventRec* SpellMapEvents::AddSeeUnitEvent(MapUnit* unit,int probab)
{
	auto evt = new SpellMapEventRec();
	evt->SetType(SpellMapEventRec::EvtTypes::EVT_SEE_UNIT);
	evt->trig_unit_id = unit->id;
	evt->probability = probab;
	AddEvent(evt);
	ResetEvents();
	return(evt);
}
