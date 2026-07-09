#include "spell_map_event.h"
#include "spell_units.h"
#include "map.h"
#include "other.h"
#include <stdexcept>
#include <tuple>
#include <algorithm>

using namespace std;

SpellMapEventRec::SpellMapEventRec(SpellMap* parent_map)
{
	map = parent_map;
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
	was_listed = false;
}
SpellMapEventRec::SpellMapEventRec(SpellMapEventRec* rec)
{
	*this = *rec;
	
	// duplicate unit objects
	for(auto& unit : units)
	{
		unit.unit = new MapUnit(*unit.unit);
		unit.unit->creator_event = this;
	}
	// unlink triggering unit
	trig_unit = NULL;
}
SpellMapEventRec::~SpellMapEventRec()
{
	for(auto & unit : units)
		delete unit.unit;
	units.clear();
	
	// unlink event links to unit
	if(trig_unit)
	{
		trig_unit->RemoveTrigEvent(this);		
		trig_unit = NULL;
	}
}
// add unit to event spawned units list
int SpellMapEventRec::AddUnit(MapUnit* unit)
{	
	map->LockMap();
	map->HaltUnitRanging(true);
	
	units.push_back(unit);
	// make unit link to this event
	unit->creator_event = this;
	unit->is_event = true;
	if(unit->is_enemy)
		unit->spec_type = MapUnitType::Values::EnemyUnit;
	else if(unit->spec_type == MapUnitType::Values::EnemyUnit)
		unit->spec_type = MapUnitType::Values::MissionUnit;

	map->events->RelinkUnits();
	map->ResumeUnitRanging(false);
	map->ReleaseMap();
	return(0);
}
// try extract unit from the spawned units list
MapUnit *SpellMapEventRec::ExtractUnit(MapUnit* unit)
{	
	for(auto it = begin(units); it != end(units); it++)
		if(it->unit == unit)
		{
			// remove from list, but not delete
			map->LockMap();
			map->HaltUnitRanging(true);
			units.erase(it);
			// unlink from this event
			unit->creator_event = NULL;
			unit->is_event = false;
			// static units can be only enemies
			unit->is_enemy = true;
			unit->spec_type = MapUnitType::Values::EnemyUnit;
			// return unit
			map->ResumeUnitRanging(false);
			map->ReleaseMap();
			return(unit);
		}
	// not found
	return(NULL);
}
// remove all spawned units from event's list, optional move them to map
void SpellMapEventRec::ClearUnits(bool to_static)
{
	map->LockMap();
	map->HaltUnitRanging(true);
	bool was_changed = !units.empty();
	for(auto& unit: units)
	{		
		auto ext_unit = ExtractUnit(unit.unit);
		if(!ext_unit)
			continue;
		if(to_static)
			map->AddUnit(ext_unit);
	}
	units.clear();
	if(was_changed)
		map->SortUnits();
	map->ResumeUnitRanging(false);
	map->ReleaseMap();
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
int SpellMapEventRec::isTransportSave()
{
	return(evt_type == EVT_TRANSPORT_UNIT || evt_type == EVT_SAVE_UNIT);
}
int SpellMapEventRec::isTransportSaveDestroy()
{
	return(evt_type == EVT_TRANSPORT_UNIT || evt_type == EVT_SAVE_UNIT || evt_type == EVT_DESTROY_UNIT);
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
	if(!units.empty() || !texts.empty())
		is_done = true;
	return(is_done);
}
int SpellMapEventRec::isDestroyObject()
{
	return(evt_type == EVT_DESTROY_OBJ);
}
int SpellMapEventRec::hasTargetUnit()
{
	return(evt_type == EVT_SEE_UNIT || evt_type == EVT_SAVE_UNIT || evt_type == EVT_TRANSPORT_UNIT || evt_type == EVT_DESTROY_UNIT);
}
int SpellMapEventRec::hasPosition()
{
	return(evt_type == EVT_SEE_PLACE || evt_type == EVT_DESTROY_OBJ);
}
// can event be MissionObjective()?
int SpellMapEventRec::isObjectiveType()
{
	return(evt_type != EvtTypes::EVT_VOID && evt_type != EvtTypes::EVT_MISSION_START);
}
// can event record be AddSpecEvent()?
int SpellMapEventRec::isEventType()
{
	return(evt_type == EvtTypes::EVT_MISSION_START || evt_type == EvtTypes::EVT_SEE_UNIT || evt_type == EvtTypes::EVT_SEE_PLACE);
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
// check if trigger-source unit is in target position (TransportUnit or SaveUnit)
int SpellMapEventRec::CheckUnitInPos(bool clear)
{
	if(!isTransportSave())
		return(false);
	
	if(!trig_unit)
		return(false);
		
	vector<MapXY> &list = (evt_type == EVT_SAVE_UNIT)?(map->start):(map->escape);
	for(auto & pos : list)
		if(trig_unit->coor == pos)
		{
			if(clear)
				is_done = true;
			return(true);
		}
	
	// not in target position
	if(clear)
		is_done = false;
	return(false);
}
// get position of event either from its position or trigger unit position or return noselection
MapXY SpellMapEventRec::GetPosition()
{
	if(hasPosition())
		return(position);
	if(hasTargetUnit() && trig_unit)
		return(trig_unit->coor);
	return(MapXY());
}





// create events list
SpellMapEvents::SpellMapEvents(SpellMap *parent)	
{
	map = parent;
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
    MapXY pos(mxy%map->x_size, mxy/map->x_size);
    if(pos.y >= map->y_size || pos.x >= map->x_size)
        pos = {-1,-1};
    return(pos);
}
int SpellMapEvents::ConvXY(MapXY pos)
{
	return(pos.x + pos.y*map->x_size);
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
		if(type == SpellMapEventRec::EVT_SEE_UNIT && type == evt->evt_type && probab && probab == evt->probability && trig_unit == evt->trig_unit_id)
			return(evt);
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
	map->LockMap();
	map->HaltUnitRanging(true);
	events.push_back(event);	
	map->ResumeUnitRanging(false);
	map->ReleaseMap();

	//###todo: check duplicates?

	return(event);
}

// remove event (returns original object, user deletes it!)
SpellMapEventRec* SpellMapEvents::ExtractEvent(SpellMapEventRec* event)
{	
	if(!event)
		return(NULL);
	auto id = find(events.begin(), events.end(), event);
	if(id == events.end())
		return(NULL);

	// remove from list, but not delete object (user must do it)
	map->HaltUnitRanging(true);
	map->LockMap();
	if(map->GetSelectEvent() == event)
		map->SelectEvent(NULL);
	events.erase(id);
	map->ReleaseMap();
	map->ResumeUnitRanging(false);
	return(event);
}
// erase event (deletes it, places linked units directly to map units)
int SpellMapEvents::EraseEvent(SpellMapEventRec* event)
{
	// try remove event from the list
	auto evt = ExtractEvent(event);
	if(!evt)
		return(1);

	map->HaltUnitRanging(true);
	map->LockMap();

	// place all linked units to the map
	for(auto & rec : evt->units)
	{
		// extract unit from event units list
		auto unit = evt->ExtractUnit(rec.unit);
		if(unit->is_enemy)
		{
			// enemy units can go directly to the map
			map->units.push_back(unit);
		}
		else
		{
			// alinace units can be only MissionStart()
			AddMissionStartUnit(unit);
		}		
	}

	map->SortUnits();
	map->ReleaseMap();
	map->ResumeUnitRanging(false);

	delete evt;
	return(0);
}

// cleanup invalid events (e.g. unit triggered events after unit removal)
void SpellMapEvents::CleanupEvents()
{
	// make list to remove
	std::vector<SpellMapEventRec*> list;
	for(auto &evt: events)
	{
		if(evt->hasTargetUnit() && !evt->trig_unit)
			list.push_back(evt); // trigger unit no longer exist?
		else if(evt->evt_type == SpellMapEventRec::EvtTypes::EVT_VOID)
			list.push_back(evt); // void event type
		else if(evt->isMissionStart() && evt->units.empty() && evt->texts.empty() && evt->video.empty())
			list.push_back(evt); // mission start with nothing attached
		else if(evt->isDestroyObject() && evt->position.IsSelected() && !map->CheckObj(&evt->position))
			list.push_back(evt); // target object not exist on coordinates
	}
	// remove'em	
	for(auto &evt: list)
		EraseEvent(evt);
}

// clear all events
void SpellMapEvents::ClearEvents()
{
	map->HaltUnitRanging(true);
	map->LockMap();
	events_map.assign(map->x_size*map->y_size,NULL);
	for(auto& evt : events)
		delete evt;
	events.clear();
	map->ReleaseMap();
	map->ResumeUnitRanging(false);	
}


// get last DEF command parser error
std::string SpellMapEvents::GetLastError()
{
	return last_error;
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
	last_error = "";
	if(cmd->parameters.size() < 1)
	{
		// failed - not enough parameters
		last_error = string_format("Wrong params count in command '%s'!",cmd->full_command.c_str());
		return(1);
	}

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
		if(strcmp(ptype->name,cmd->parameters.at(0).c_str()) == 0)
		{
			event_type = ptype->type;
			break;
		}
		else
			ptype++;
	if(event_type == SpellMapEventRec::EvtTypes::EVT_VOID)
	{
		// type unknown
		last_error = string_format("Unknown MissionObjective type '%s' in command '%s'!",cmd->parameters.at(0).c_str(),cmd->full_command.c_str());
		return(1);
	}		

	// decode parameters
	wstring label = L"";
	MapXY target_position;
	int unit_index = -1;
	if(event_type == SpellMapEventRec::EvtTypes::EVT_TRANSPORT_UNIT || event_type == SpellMapEventRec::EvtTypes::EVT_SAVE_UNIT || event_type == SpellMapEventRec::EvtTypes::EVT_DESTROY_UNIT)
	{
		if(cmd->parameters.size() != 3)
		{
			// failed - not enough parameters
			last_error = string_format("Wrong params count in command '%s'!",cmd->full_command.c_str());
			return(1);
		}

		// target unit index
		unit_index = stoi(cmd->parameters.at(1));

		// objective label
		label = char2wstringCP895(cmd->parameters.at(2).c_str());
	}
	else if(event_type == SpellMapEventRec::EvtTypes::EVT_DESTROY_ALL)
	{
		if(cmd->parameters.size() != 2)
		{
			// failed - not enough parameters
			last_error = string_format("Wrong params count in command '%s'!",cmd->full_command.c_str());
			return(1);
		}

		// objective label
		label = char2wstringCP895(cmd->parameters.at(1).c_str());
	}
	else if(event_type == SpellMapEventRec::EvtTypes::EVT_DESTROY_OBJ || event_type == SpellMapEventRec::EvtTypes::EVT_SEE_PLACE)
	{
		if(cmd->parameters.size() != 3)
		{
			// failed - not enough parameters
			last_error = string_format("Wrong params count in command '%s'!",cmd->full_command.c_str());
			return(1);
		}

		// event position		
		int xy = stoi(cmd->parameters.at(1));
		target_position = ConvXY(xy);
		if(!target_position.IsSelected())
		{
			last_error = string_format("Position %d out of valid range in command '%s'!",xy,cmd->full_command.c_str());
			return(1);
		}
		
		// objective label
		label = char2wstringCP895(cmd->parameters.at(2).c_str());
	}

	// make new object-event
	SpellMapEventRec *evt = new SpellMapEventRec(map);
	evt->evt_type = event_type;
	evt->type_name = cmd->parameters.at(0);
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
	last_error = "";
	if(cmd->parameters.size() < 1)
	{
		last_error = string_format("Wrong params count in command '%s'!",cmd->full_command.c_str());
		return(1);
	}
	if(cmd->parameters.at(0).compare("SeePlace") == 0)
	{
		// AddSpecialEvent(SeePlace, target_position, event_data_index, event_probability)
		event_type = SpellMapEventRec::EVT_SEE_PLACE;
		if(cmd->parameters.size() != 4)
		{
			// failed - not enough parameters
			last_error = string_format("Wrong params count in command '%s'!",cmd->full_command.c_str());
			return(1);
		}

		// event position		
		int xy = stoi(cmd->parameters.at(1));
		target_position = ConvXY(xy);
		if(!target_position.IsSelected())
		{
			last_error = string_format("Position %d out of valid range in command '%s'!",xy, cmd->full_command.c_str());
			return(1);
		}
		
		// event index
		event_data_index = stoi(cmd->parameters.at(2));

		// probability
		event_probability = stoi(cmd->parameters.at(3));

		// try find another matching event
		evt = FindEvent(SpellMapEventRec::EVT_SEE_PLACE, event_probability, target_position);
	}
	else if(cmd->parameters.at(0).compare("MissionStart") == 0)
	{
		// AddSpecialEvent(MissionStart, event_data_index, event_probability)
		event_type = SpellMapEventRec::EVT_MISSION_START;
		if(cmd->parameters.size() != 3)
		{
			// failed - not enough parameters
			last_error = string_format("Wrong params count in command '%s'!",cmd->full_command.c_str());
			return(1);
		}

		// event index
		event_data_index = stoi(cmd->parameters.at(1));

		// probability
		event_probability = stoi(cmd->parameters.at(2));

		// try find another matching event
		evt = FindEvent(SpellMapEventRec::EVT_MISSION_START,event_probability);
	}
	else if(cmd->parameters.at(0).compare("SeeUnit") == 0)
	{
		// AddSpecialEvent(SeeUnit, unit_index, event_data_index, event_probability)
		event_type = SpellMapEventRec::EVT_SEE_UNIT;
		if(cmd->parameters.size() != 4)
		{
			// failed - not enough parameters
			last_error = string_format("Wrong params count in command '%s'!",cmd->full_command.c_str());
			return(1);
		}

		// unit to see index
		unit_index = stoi(cmd->parameters.at(1));

		// event index
		event_data_index = stoi(cmd->parameters.at(2));

		// probability
		event_probability = stoi(cmd->parameters.at(3));

		// try find another matching event
		evt = FindEvent(SpellMapEventRec::EVT_SEE_UNIT,event_probability,unit_index);
	}
	else
	{
		// unknown or not implemented
		last_error = string_format("Unknown event type '%s' in command '%s'!",cmd->parameters.at(0).c_str(),cmd->full_command.c_str());
		return(1);
	}	
	if(event_probability > 100)
	{
		last_error = string_format("Probability %d too high in command '%s'!",event_probability,cmd->full_command.c_str());
		return(1);
	}
	if(!evt)
	{
		// make new event if not existing match found
		is_new_event = true;
		evt = new SpellMapEventRec(map);
		evt->evt_type = event_type;
		evt->position = target_position;
		evt->probability = event_probability;
		evt->trig_unit_id = unit_index;
		events.push_back(evt);
	}
	
	
	// event type name
	evt->type_name = cmd->parameters.at(0);
	

	// try parse event data
	string event_data_header = "EventData(" + std::to_string(event_data_index) + ")";
	std::unique_ptr<SpellDefSection> event_data(def->GetSection(event_data_header));
	if(!event_data)
	{
		// not found
		last_error = string_format("Cannot found '%s' section in map DEF file!",event_data_header.c_str());
		if(is_new_event)
		{
			events.pop_back();
			delete evt;
		}		
		return(1);
	}

	// for each item in event data (there can be only one in original Spellcross):
	for(int evid = 0; evid < event_data->Size(); evid++)
	{
		SpellDefCmd* evcmd = (*event_data)[evid];

		if(evcmd->name.compare("AddSpecialUnit") == 0)
		{
			// --- AddSpecialUnit(unit_order, unit_id, position, experience, man_count, name) ---				
			if(evcmd->parameters.size() != 6)
			{
				// failed - not enough parameters
				last_error = string_format("Wrong params count in command '%s'!",evcmd->full_command.c_str());
				if(is_new_event)
				{
					events.pop_back();
					delete evt;
				}				
				return(1);
			}
			
			// make unit
			MapUnit* unit = new MapUnit(map);

			// unit index within map (identifier)
			unit->id = -1;

			// decode unit type
			if(evcmd->parameters.at(0).compare("EnemyUnit") == 0)
			{
				unit->spec_type = MapUnitType::EnemyUnit;
				unit->is_enemy = 1;
			}
			else if(evcmd->parameters.at(0).compare("MissionUnit") == 0)
			{
				unit->spec_type = MapUnitType::MissionUnit;
			}
			else if(evcmd->parameters.at(0).compare("ArmyUnit") == 0)
			{
				unit->spec_type = MapUnitType::ArmyUnit;
			}
			else if(evcmd->parameters.at(0).compare("SpecUnit1") == 0)
			{
				unit->spec_type = MapUnitType::SpecUnit;
				unit->id = 48; // this idiocy is original Spellcross hard defined designation for SpecUnit1
			}
			else if(evcmd->parameters.at(0).compare("SpecUnit2") == 0)
			{
				unit->spec_type = MapUnitType::SpecUnit;
				unit->id = 49; // this idiocy is original Spellcross hard defined designation for SpecUnit2
			}
			else if(evcmd->parameters.at(0).compare("VoluntUnit") == 0)
			{
				unit->spec_type = MapUnitType::VoluntUnit;
			}
			else
			{
				// unknown unit type				
				last_error = string_format("Unknown special unit type '%s' in command '%s'!",evcmd->parameters.at(0).c_str(),evcmd->full_command.c_str());
				delete unit;
				if(is_new_event)
				{
					events.pop_back();
					delete evt;
				}				
				return(1);
			}
			
			// generate new unit ID
			if(unit->id < 0)
				unit->id = GetNextID();

			// unit type index			
			int unit_type_id = stoi(evcmd->parameters.at(1));

			// try fetch unit record from spelldata
			unit->unit = data->units->GetUnit(unit_type_id);
			if(!unit->unit)
			{
				last_error = string_format("Unknown unit type is '%d' in command '%s'!",unit_type_id,evcmd->full_command.c_str());
				delete unit;
				if(is_new_event)
				{
					events.pop_back();
					delete evt;
				}
				return(1);
			}

			// desired unit position
			int xy = stoi(evcmd->parameters.at(2));
			unit->coor = ConvXY(xy);
			if(!unit->coor.IsSelected())
			{
				last_error = string_format("Unit position %d out of valid range in command '%s'!",xy,evcmd->full_command.c_str());
				delete unit;
				if(is_new_event)
				{
					events.pop_back();
					delete evt;
				}
				return(1);
			}
			
			// experience level
			unit->InitExperience(stoi(evcmd->parameters.at(3)));

			// man count (health)
			unit->man = min(stoi(evcmd->parameters.at(4)),unit->unit->cnt);

			// unit active (to change in game mode)
			unit->is_active = 0;

			// store back link to event to the unit
			unit->creator_event = evt;
			
			// copy unit name
			unit->name = L"";
			auto custom_name = char2wstringCP895(evcmd->parameters.at(5).c_str());
			if(custom_name.size() && custom_name != L"-")
				unit->name = custom_name;

			// default action points
			unit->ResetAP();
			unit->ResetTurnsCounter();
			unit->ClearDigLevel();
			unit->commander_id = 0;
			unit->is_commander = false;
			unit->morale = 100;
			unit->is_event = true;			
			unit->is_active = evt->isMissionStart();

			// try to parse randomizer
			unit->randomize_mode = MapUnit::RandomizeMode::OFF;
			unit->randomize_units.clear();
			if(evcmd->sub_name == "Randomize")
			{
				if(evcmd->sub_params.size() == 1 && iequals(evcmd->sub_params[0],"OFF"))
				{
					// disabled
					unit->randomize_mode = MapUnit::RandomizeMode::OFF;
				}
				else if(evcmd->sub_params.size() == 1 && iequals(evcmd->sub_params[0],"AUTO"))
				{
					// auto mode (defined globally for the map)
					unit->randomize_mode = MapUnit::RandomizeMode::AUTO;
				}
				else if(!evcmd->sub_params.empty())
				{
					// explicit list of unit codes
					unit->randomize_mode = MapUnit::RandomizeMode::EXPLICIT;
					for(auto& unit_id_str: evcmd->sub_params)
					{
						char* send;
						auto unit_type_id = std::strtol(unit_id_str.c_str(),&send,10);
						auto rand_unit = data->units->GetUnit(unit_type_id);
						if(!rand_unit || send == unit_id_str.c_str())
						{
							last_error = string_format("Unknown unit type '%s' parameter in command '%s'!",unit_id_str.c_str(),evcmd->sub_full_command.c_str());
							delete unit;
							if(is_new_event)
							{
								events.pop_back();
								delete evt;
							}
							return(1);
						}
						unit->randomize_units.push_back(unit_type_id);
					}
				}
				else
				{
					// invalid parameters
					last_error = string_format("Unit randomizer command '%s' parameters not recognized for main command '%s'!",evcmd->sub_full_command.c_str(),evcmd->full_command.c_str());
					delete unit;
					if(is_new_event)
					{
						events.pop_back();
						delete evt;
					}
					return(1);
				}
			}


			// add unit to list
			evt->units.emplace_back(unit);
		}
		else if(evcmd->name.compare("EventText") == 0)
		{
			// --- EventText(text_fs_name) ---
			if(evcmd->parameters.size() != 1)
			{
				last_error = string_format("Wrong params count in command '%s'!",evcmd->full_command.c_str());
				if(is_new_event)
				{
					events.pop_back();
					delete evt;
				}
				return(1);
			}
			
			// try to find text in string list
			auto text_name = evcmd->parameters.at(0);
			auto text = data->texts->GetText(text_name);
			if(!text)
			{
				// make fake text resource
				text = data->texts->AddText(text_name,"Text resouce placeholder generated by map loader when actual text resource not found!",SpellLang::ENG,SpellTextRec::TextPanel::DEFAULT,NULL,true);
				if(!last_error.empty())
					last_error += "\n";
				last_error += string_format("Text '%s' in command '%s' not found in loaded resources!",text_name.c_str(),evcmd->full_command.c_str());
				if(!text)
				{
					if(is_new_event)
					{
						events.pop_back();
						delete evt;
					}
					return(1);
				}
			}
			evt->texts.emplace_back(text);
		}
		else if(evcmd->name.compare("PlayCANAnimation") == 0)
		{
			// --- PlayCANAnimation(video_name) ---
			if(evcmd->parameters.size() != 1)
			{
				last_error = string_format("Wrong params count in command '%s'!",evcmd->full_command.c_str());
				if(is_new_event)
				{
					events.pop_back();
					delete evt;
				}
				return(1);
			}

			// try to find video
			auto vid_name = evcmd->parameters.at(0);
			auto vid_list = data->videos->GetNames(vid_name);
			if(vid_list.empty())
			{
				last_error = string_format("Animation '%s' in command '%s' not found in loaded resources!",vid_name.c_str(),evcmd->full_command.c_str());
				/*delete event_data;
				if(is_new_event)
				{
					events.pop_back();
					delete evt;
				}
				return(1);*/
			}
			evt->video = vid_name;
		}
	}

	return(0);
}

// generate event DEF data <header, event_data>
std::tuple<std::string, std::string> SpellMapEventRec::FormatDEFrecord(int *initial_id,bool save_event,bool save_obj)
{
	std::string head = "";
	std::string data = "";

	if(is_objective && save_obj)
	{
		// --- AddMissionObjective():

		switch(evt_type)
		{
			case EvtTypes::EVT_TRANSPORT_UNIT:
				head += string_format("    AddMissionObjective(TransportUnit,%d,%s)\n",trig_unit_id,wstring2stringCP895(label).c_str());
				break;
			case EvtTypes::EVT_SAVE_UNIT:
				head += string_format("    AddMissionObjective(SaveUnit,%d,%s)\n",trig_unit_id,wstring2stringCP895(label).c_str());
				break;
			case EvtTypes::EVT_SEE_PLACE:
				head += string_format("    AddMissionObjective(SeePlace,%d,%s)\n",map->ConvXY(position),wstring2stringCP895(label).c_str());
				break;
			case EvtTypes::EVT_DESTROY_OBJ:
				head += string_format("    AddMissionObjective(DestroyObject,%d,%s)\n",map->ConvXY(position),wstring2stringCP895(label).c_str());
				break;
			case EvtTypes::EVT_DESTROY_UNIT:
				head += string_format("    AddMissionObjective(DestroyUnit,%d,%s)\n",trig_unit_id,wstring2stringCP895(label).c_str());
				break;
			case EvtTypes::EVT_DESTROY_ALL:
				head += string_format("    AddMissionObjective(DestroyAllUnits,%s)\n",wstring2stringCP895(label).c_str());
				break;
			default:
				break;
		}

	}

	if(save_event)
	{
		// --- AddSpecialEvent():

		// total data count for event
		int evt_count = texts.size() + !video.empty() + !units.empty();

		// duplicate event header for each data (spellcross seems to not be able to aggregate different actions for one event...)
		std::string event_cmd = "";
		bool used = evt_count > 0;
		for(int k = *initial_id; k < *initial_id + evt_count; k++)
		{			
			switch(evt_type)
			{
				case EvtTypes::EVT_MISSION_START:					
					event_cmd = string_format("AddSpecialEvent(MissionStart,%d,%d%%)", k, probability);
					break;
				case EvtTypes::EVT_SEE_PLACE:
					event_cmd = string_format("AddSpecialEvent(SeePlace,%d,%d,%d%%)",map->ConvXY(position),k,probability);
					break;
				case EvtTypes::EVT_SEE_UNIT:
					event_cmd = string_format("AddSpecialEvent(SeeUnit,%d,%d,%d%%)",trig_unit_id,k,probability);
					break;
				default:
					used = false;
					break;
			}
			if(!event_cmd.empty())
				head += string_format("    %s\n",event_cmd.c_str());
		}

		if(used)
		{
			// place text data
			for(auto text: texts)
			{
				data += string_format(";%s:\n",event_cmd.c_str());
				data += string_format("EventData(%d) {\n",(*initial_id)++);
				data += string_format("    EventText(%s)\n}\n\n",text.text->name.c_str());
			}

			// place video data
			if(!video.empty())
			{
				data += string_format(";%s:\n",event_cmd.c_str());
				data += string_format("EventData(%d) {\n",(*initial_id)++);
				data += string_format("    PlayCANAnimation(%s)\n}\n\n",video.c_str());
			}

			// place unit data
			if(!units.empty())
			{				
				data += string_format(";%s:\n",event_cmd.c_str());
				data += string_format("EventData(%d) {\n",(*initial_id)++);
				for(auto& unit: units)
				{
					std::string name = wstring2stringCP895(unit.unit->name);
					if(name.empty())
						name = "-";
					std::string spec_type_str = unit.unit->spec_type.GetString();
					if(unit.unit->spec_type == MapUnitType::Values::SpecUnit && unit.unit->id == 48)
						spec_type_str = "SpecUnit1";
					else if(unit.unit->spec_type == MapUnitType::Values::SpecUnit && unit.unit->id == 49)
						spec_type_str = "SpecUnit2";
					data += string_format("    ;%ls\n",unit.unit->unit->name.c_str());
					data += string_format("    AddSpecialUnit(%s,%d,%d,%02d,%d,%s)",spec_type_str.c_str(),unit.unit->unit->type_id,map->ConvXY(unit.unit->coor),unit.unit->experience_init,unit.unit->man,name.c_str());

					// append randomizer setup
					if(unit.unit->randomize_mode == MapUnit::RandomizeMode::OFF)
						data += ",Randomize(off)";
					else if(unit.unit->randomize_mode == MapUnit::RandomizeMode::AUTO)
						data += ",Randomize(auto)";
					else if(unit.unit->randomize_mode == MapUnit::RandomizeMode::EXPLICIT)
					{
						data += ",Randomize(";
						for(int k = 0; k < unit.unit->randomize_units.size(); k++)
						{
							data += string_format("%d",unit.unit->randomize_units[k]);
							if(k + 1 < unit.unit->randomize_units.size())
								data += ",";
						}
						data += ")";
					}
					data += "\n";

				}
				data += string_format("}\n\n");
			}
		}
	}
	return std::tuple(head,data);
}






SpellMapEventRec** SpellMapEvents::EventMap(MapXY pos)
{
	return(&events_map[pos.x + pos.y*map->x_size]);
}

// reset event triggers and build events map for fast search (call at least once at mission start)
void SpellMapEvents::ResetEvents()
{
	map->LockMap();
	map->HaltUnitRanging(true);

	// reset all events
	for(auto & evt : events)
	{
		evt->is_done = false;
		evt->next = NULL;
		evt->hide = evt->probability < rand()%101;
		for(auto & text : evt->texts)
			text.is_done = false;
		for(auto& unit : evt->units)
		{
			unit.is_placed = false;
			unit.unit->was_seen = false;
		}
		if(evt->trig_unit)
		{
			evt->trig_unit->RemoveTrigEvent(evt);
			evt->trig_unit = NULL;
		}
	}

	// make link to trigger units
	RelinkUnits();

	map->ResumeUnitRanging(false);
	map->ReleaseMap();
}

// build links to particular units
int SpellMapEvents::RelinkUnits(vector<MapUnit*> *map_units)
{
	map->LockMap();
	map->HaltUnitRanging(true);
		
	// take map units either from parent map (default) or from explicit list
	vector<MapUnit*> units_list;
	if(map_units)
		units_list = *map_units;
	else
		units_list = map->units;
		
	// make list of all existing units (map units and events' units)	
	for(auto& evt : events)
		for(auto& unit : evt->units)
			units_list.push_back(unit.unit);

	// unlink all events from all units
	for(auto &unit: units_list)
	{
		unit->trig_events.clear();
		//unit->creator_event = NULL;
	}

	// make map of events
	events_map.assign(map->x_size*map->y_size,NULL);
	for(auto& evt : events)
	{
		evt->next = NULL;
		evt->trig_unit = NULL;
		if(evt->hasPosition())
		{
			// place event to map (or at the end of chain of events for the position)
			auto tile = EventMap(evt->position);
			if(!*tile)
			{
				// first event at this position
				*tile = evt;
			}
			else
			{
				// next event(s) at the position - link it to previous event
				SpellMapEventRec* prev = *tile;
				while(prev->next)
					prev = prev->next;
				prev->next = evt;
			}
		}
		else if(evt->hasTargetUnit())
		{
			// look for target unit and assign ref
			for(auto& unit : units_list)
				if(unit->id == evt->trig_unit_id)
				{
					// target unit found
					evt->trig_unit = unit;
					unit->trig_events.push_back(evt);					
				}
		}
	}

	map->ResumeUnitRanging(false);
	map->ReleaseMap();
	
	return(0);
}



// get first event for given position
SpellMapEventRec* SpellMapEvents::GetEvent(MapXY pos)
{
	for(auto &evt: events)
		if(evt->GetPosition() == pos)
			return(evt);
	return(NULL);
}
// get events count for given position
int SpellMapEvents::GetEventsCount(MapXY pos)
{
	int count = 0;
	for(auto& evt: events)
		if(evt->GetPosition() == pos)
			count++;
	return(count);
}
// get another event at the position (cycle through)
SpellMapEventRec* SpellMapEvents::GetAnotherEvent(SpellMapEventRec *evt)
{
	if(!evt)
		return(NULL);

	// try find this event in the list
	auto pevt = std::find(events.begin(),events.end(),evt);	
	if(pevt == events.end())
		return(GetEvent(evt->GetPosition()));
	int ev_id = pevt - events.begin();

	// look for next event at position
	for(int k = ev_id+1; k < events.size(); k++)
		if(events[k]->GetPosition() == evt->GetPosition())
			return(events[k]);

	// not found, look from start again
	for(int k = 0; k <= ev_id; k++)
		if(events[k]->GetPosition() == evt->GetPosition())
			return(events[k]);
	
	return(NULL);
}


// get event by list order index
SpellMapEventRec* SpellMapEvents::GetEvent(int index)
{
	if(index < 0 || index >= events.size())
		return(NULL);
	return(events[index]);
}

// check/return event of given type at position or at map cursor
SpellMapEventRec* SpellMapEvents::CheckEvent(SpellMapEventRec::EvtTypes type, MapXY *pos)
{
	MapXY posxy = map->GetSelection();
	if(pos)
		posxy = *pos;
	if(!posxy.IsSelected())
		return(NULL);

	for(auto &evt: events)
		if(evt->evt_type == type && evt->hasPosition() && evt->position == posxy)
			return(evt);
	return(NULL);
}


// fast check if there is some undone event at the position (using event map)
int SpellMapEvents::CheckEventMap(MapXY pos)
{
	return(CheckEventMap(ConvXY(pos)));
}
int SpellMapEvents::CheckEventMap(int pos)
{
	auto evt = events_map[pos];
	if(!evt)
		return(false);
	if(evt->is_done)
		return(false);
	return(true);
}

// get list of events for given position (SeePlace), only not executed
// in editor mode show all position related events
SpellMapEventsList SpellMapEvents::GetEvents(MapXY pos, bool clear)
{
	return(GetEvents(ConvXY(pos),clear));
}
SpellMapEventsList SpellMapEvents::GetEvents(int pos,bool clear)
{	
	// make list of events
	vector<SpellMapEventRec*> list;
	auto evt = events_map[pos];
	while(evt)
	{
		if(!map->isGameMode())
			list.push_back(evt);
		else if(!evt->is_done && evt->isSeePlace() && !evt->hide)
		{
			list.push_back(evt);
			if(clear)
				evt->is_done = true;
		}
		evt = evt->next;
	}		
	return(list);
}
// check and eventually return non-done DestroyObject events linked to position
SpellMapEventsList SpellMapEvents::GetDestroyObjectEvents(MapXY pos,bool clear)
{
	// make list of events
	vector<SpellMapEventRec*> list;
	for(auto & evt : events)
	{
		if(evt->isDone())
			continue;
		if(!evt->isDestroyObject())
			continue;
		if(evt->position != pos)
			continue;
		list.push_back(evt);
		if(clear)
			evt->is_done = true;
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
		if(evt->is_done && map->isGameMode())
			continue;
		if(clear)
			evt->is_done = true;
		if(evt->hide && map->isGameMode())
			continue;
		list.push_back(evt);
	}
	return(list);
}

// insert unit to first possible MissionStart() event matching the probability or a one
int SpellMapEvents::AddMissionStartUnit(MapUnit* unit,int probab)
{
	if(unit->creator_event && unit->creator_event->isMissionStart())
		return(1);

	// remove unit from other event eventually
	if(unit->creator_event)
	{
		unit->creator_event->ExtractUnit(unit);
		unit->creator_event = NULL;
	}

	// try to find some suitable MissionStart() event
	SpellMapEventRec *target_evt = NULL;
	for(auto & evt : events)
		if(evt->isMissionStart() && evt->probability == probab)
			target_evt = evt;
	if(!target_evt)
	{
		// not found: make one
		target_evt = new SpellMapEventRec(map);
		target_evt->SetType(SpellMapEventRec::EvtTypes::EVT_MISSION_START);
		target_evt->probability = probab;
		AddEvent(target_evt);
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
	auto evt = new SpellMapEventRec(map);
	evt->SetType(SpellMapEventRec::EvtTypes::EVT_SEE_UNIT);
	evt->trig_unit_id = unit->id;
	evt->probability = probab;
	AddEvent(evt);
	ResetEvents();
	return(evt);
}

// add new SeePlace event at position if not there yet, otherwise return existing
SpellMapEventRec* SpellMapEvents::AddSeePlaceEvent(MapXY pos,int probab)
{
	auto evt = CheckEvent(SpellMapEventRec::EvtTypes::EVT_SEE_PLACE,&pos);
	if(!evt)
	{
		evt = new SpellMapEventRec(map);
		evt->SetType(SpellMapEventRec::EvtTypes::EVT_SEE_PLACE);
		evt->probability = probab;
		evt->position = pos;
		AddEvent(evt);
		ResetEvents();
	}	
	return(evt);
}

// try add unit-related objective (Save, Transport, Destroy) if not exist yet
SpellMapEventRec* SpellMapEvents::AddUnitObjective(MapUnit* unit,SpellMapEventRec::EvtTypes type)
{
	if(!unit)
		return(NULL);

	// do not allow conflicting events/objectives
	if(unit->GetTrigEvent({SpellMapEventRec::EvtTypes::EVT_SAVE_UNIT,SpellMapEventRec::EvtTypes::EVT_TRANSPORT_UNIT,SpellMapEventRec::EvtTypes::EVT_DESTROY_UNIT}))
		return(NULL);

	auto evt = new SpellMapEventRec(map);
	evt->SetType(type);
	evt->trig_unit_id = unit->id;
	evt->probability = 100;
	evt->is_objective = true;
	evt->label = char2wstring(evt->type_name.c_str());
	AddEvent(evt);
	ResetEvents();

	return(evt);
}

// are all objectives done?
int SpellMapEvents::ObjectivesDone()
{	
	for(auto & evt : events)
		if(evt->is_objective && !evt->isDone())
			return(false);
	return(true);
}

// get event order ID by pointer
int SpellMapEvents::GetEventID(SpellMapEventRec* target)
{
	for(int k = 0; k < events.size(); k++)
		if(events[k] == target)
			return(k);
	return(-1);
}




// --- Events listing stuff:

// clear event listed flags to restart listing process
void SpellMapEvents::ListerClear()
{
	for(auto &evt: events)
	{
		evt->was_listed = false;
		for(auto &unit: evt->units)
			unit.was_listed = false;
	}
}

// call repeatedly to get event(s) list at the same position, returns empty list of all events were listed
SpellMapEventsList SpellMapEvents::ListerGet()
{
	SpellMapEventsList list;	
	MapXY pos;
	for(auto& evt: events)
	{
		// skip already listed events
		if(evt->was_listed || evt->isMissionStart())
			continue;

		// get its position		
		MapXY pos2 = evt->GetPosition();
		if(!pos2.IsSelected())
			continue;
		if(pos.IsSelected() && pos != pos2)
			continue;
		pos = pos2;
		
		// place to list
		evt->was_listed = true;
		list.push_back(evt);
	}
	if(list.empty())
		pos.Clear();

	// append mission start events?
	/*if(with_mission_start)
	{
		for(auto& evt: events)
		{
			// skip already listed events
			if(evt->was_listed || !evt->isMissionStart())
				continue;
			// mission start event found (should be only one)

			// for each spawned unit
			for(auto &unit: evt->units)
			{
				if(unit.was_listed)
					continue;
				if(pos.IsSelected() && unit.unit->coor != pos)
					continue;
				pos = unit.unit->coor;
				unit.was_listed = true;
				list.push_back(evt);
			}
			break;						
		}
	}*/
	return(list);
}

// call repeatedly to get list of units with mission start event, optional pos filters units to certain spot, returns empty when no more units (reset by ListerClear())
std::vector<SpellMapEventUnitRec*> SpellMapEvents::ListerGetMissionStart(MapXY pos)
{	
	std::vector<SpellMapEventUnitRec*> list;
	bool filter_pos = pos.IsSelected();
	for(auto& evt: events)
	{
		// skip already listed events
		if(evt->was_listed || !evt->isMissionStart())
			continue;
		// mission start event found

		// for each spawned unit
		for(auto& unit: evt->units)
		{
			if(unit.was_listed)
				continue;
			if(filter_pos && unit.unit->coor != pos)
				continue;
			unit.was_listed = true;
			list.push_back(&unit);
		}
		//break;
	}
	return(list);
}