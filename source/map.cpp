//=============================================================================
// Spellcross MAP related routines: Loading, rendering.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#include "map.h"
#include "spellcross.h"
#include "fs_archive.h"
#include "fsu_archive.h"
#include "spell_units.h"
#include "other.h"

#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <regex>
#include <tuple>
#include <algorithm>
#include <random>
#include <chrono>

#include "wx/dcgraph.h"
#include "wx/dcbuffer.h"
#include <wx/rawbmp.h>

using namespace std;






//=============================================================================
// Map scroller class
//=============================================================================
// map scroller
TScroll::TScroll()
{
	Reset();
}
// reset scroller to default
void TScroll::Reset()
{
	xref = 0;
	yref = 0;
	// scroll position
	dx = 0;
	dy = 0;
	// selection size
	size = 1;
	// scroll state
	state = 0;
	// no change
	modified = 0;
}
// set absolute position
void TScroll::SetPos(int x,int y)
{
	dx = x;
	dy = y;
	xref = x;
	yref = y;
	state = 0;
	modified = true;
}
// set cursor ref. position (callwed on mouse down event)
void TScroll::SetRef(int x,int y)
{
	xref = x;
	yref = y;
	// shift active
	state = 1;
}
// cursor moved to new position
void TScroll::Move(int x,int y)
{
	if(state)
	{
		// calculate scroll shift
		dx -= (x - xref);
		dy -= (y - yref);
	}
	// set new ref position
	xref = x;
	yref = y;
	// something changed
	modified = 1;
}
// cursor to idle state (mouse leave or up)
void TScroll::Idle()
{
	state = 0;
	modified = 0;
}
// check if there was any change in cursor 
bool TScroll::wasModified()
{
	return(modified);
}
// check if there was any change in cursor and commit the change will be processed
bool TScroll::Commit()
{
	bool mod = modified;
	modified = 0;
	return(mod);
}
// limit scroll to valid range
tuple<int,int> TScroll::CheckScroll(int x_limit,int y_limit)
{
	if(dx >= x_limit)
		dx = x_limit/* - 1*/;
	if(dx < 0)
		dx = 0;
	if(dy >= y_limit)
		dy = y_limit/* - 1*/;
	if(dy < 0)
		dy = 0;
	return {dx, dy};
}
// limit scroll to valid range (min,max)
tuple<int,int> TScroll::CheckScroll(int x_min, int y_min, int x_limit,int y_limit)
{
	if(dx >= x_limit)
		dx = x_limit/* - 1*/;
	if(dx < x_min)
		dx = x_min;
	if(dy >= y_limit)
		dy = y_limit/* - 1*/;
	if(dy < y_min)
		dy = y_min;
	return {dx, dy};
}
// return scroll position
tuple<int,int> TScroll::GetScroll()
{
	return {dx,dy};
}
// return cursor position relative to selection
tuple<int,int> TScroll::GetCursor()
{
	return {xref,yref};
}
// get selction region size
int TScroll::GetSize()
{
	return(size);
}
// resize selection size by delta
int TScroll::ResizeSelection(int delta)
{
	// change selection size
	size += delta;
	if(size < 1)
		size = 1;
	if(size > MAX_SEL_SIZE)
		size = MAX_SEL_SIZE;
	// something changed
	modified = 1;
	// return new size
	return(size);
}





//=============================================================================
// class MapLayer3 - Map layer 3 record (ANM animations)
//=============================================================================
MapLayer3::MapLayer3(AnimL1* anm, int x_pos, int y_pos, int frame_ofs, int frame_limit)
{
	this->anim = anm;
	this->x_pos = x_pos;
	this->y_pos = y_pos;
	this->frame_ofs = min(frame_ofs,anm->frames.size()-1);
	this->frame_limit = min(frame_limit,anm->frames.size()-1);
}
MapLayer3::~MapLayer3()
{
	anim = NULL;
}

//=============================================================================
// class MapLayer4 - Map layer 4 record (PNM animations)
//=============================================================================
MapLayer4::MapLayer4(AnimPNM* pnm, int x_pos, int y_pos, int x_ofs, int y_ofs, int frame_ofs, int frame_limit)
{
	this->anim = pnm;
	this->x_pos = x_pos;
	this->y_pos = y_pos;
	this->x_ofs = x_ofs;
	this->y_ofs = y_ofs;
	this->frame_ofs = frame_ofs;
	this->frame_limit = (frame_limit<0)?(pnm->frames.size()):(frame_limit);
}
MapLayer4::~MapLayer4()
{
	anim = NULL;
}




//=============================================================================
// class Map
//=============================================================================

SpellMap::SpellMap()
{
	game_mode = false;

	L1 = NULL;
	L2 = NULL;
	elev = NULL;
	flags = NULL;
	Lunit = NULL;
	select = NULL;
	pic = NULL;
	L1_flags = NULL;

	last_gamma = 0.0;
	gamma = 1.0;
	wL1 = true;
	wL2 = true;
	wL3 = true;
	wL4 = true;
	wSTCI = true;
	wUnits = true;

	map_path = L"";
	def_path = L"";	

	unit_selection = NULL;
	hud_enabled = true;
}

SpellMap::~SpellMap()
{
	// cleanup heap allocated stuff
	Close();
}

// switch game mode
int SpellMap::SetGameMode(int new_mode)
{
	int old_state = game_mode;
	game_mode = new_mode;

	// force recalculation of units view map
	if(game_mode)
		InvalidateUnitsView();

	return(old_state);
}

// returns path to DEF file or DTA file if DEF was not used
wstring SpellMap::GetTopPath()
{
	if (def_path.empty())
		return(map_path);
	return(def_path);
}

// cleanup MAP data
void SpellMap::Close()
{
	int k;
	// loose terrain
	if(L1)
		delete[] L1;
	L1 = NULL;
	// loose objects
	if(L2)
		delete[] L2;
	L2 = NULL;
	// loose elevation map
	if(elev)
		delete[] this->elev;
	elev = NULL;
	// loose flags
	if(flags)
		delete[] flags;
	flags = NULL;
	// loose ANM list
	for(k = 0; k < L3.size(); k++)
		delete L3[k];
	L3.clear();
	// loose PNM list
	for(k = 0; k < L4.size(); k++)
		delete L4[k];
	L4.clear();
	// loose start/ciel
	start.clear();
	escape.clear();
	// loose units layer
	if(Lunit)
		delete[] Lunit;
	Lunit = NULL;
	// loose units list
	for(k = 0; k < units.size(); k++)
		delete units[k];
	units.clear();
	unit_selection = NULL;
	// loose selection array
	if(select)
		delete[] select;
	select = NULL;
	if(L1_flags)
		delete[] L1_flags;
	L1_flags = NULL;

	if(pic)
		delete[] pic;
	pic = NULL;

	ClearHUDbuttons();

	unit_sel_land_preference = true;
	w_unit_hud = true;
	units_view_mask.clear();
	units_view_mask_0.clear();
	units_view_map.clear();

	msel.clear();

	filter.clear();
	
	// reset gamma to make correctio recalculation
	last_gamma = 0.0;
}


// create blank map
int SpellMap::Create(SpellData* spelldata, const char *terr_name, int x, int y)
{
	// loose old map data
	Close();

	// set terrain
	terrain = spelldata->GetTerrain(terr_name);
	if(!terrain)
	{
		Close();
		return(1);
	}
	
	// set map size
	x_size = x;
	y_size = y;

	// load L1 sprite indices
	L1 = new Sprite *[x_size*y_size];
	L2 = new Sprite *[x_size*y_size];
	flags = new uint8_t[x_size * y_size];
	L1_flags = new uint32_t[x_size*y_size];
	elev = new int[x_size * y_size];
	
	// filter mask
	filter.resize(x_size*y_size);
	

	// generate plain map
	for(int k = 0; k < (x_size * y_size); k++)
	{		
		L1[k] = terrain->GetSpriteWild("PLA00_??",Terrain::RANDOM);
		L2[k] = NULL;
		elev[k] = 2;
		flags[k] = 0;
	}
	
	// get tile flags from context data
	SyncL1flags();
	
	// create selections maps
	select = new uint8_t[x_size*y_size];
	std::memset((void*)select,0x00,x_size*y_size);

	// prefetch pointers to common stuff needed fast when rendering
	start_sprite = terrain->GetSprite("START");
	escape_sprite = terrain->GetSprite("CIEL");

	return(0);
}

// Load MAP from file
int SpellMap::Load(wstring &path, SpellData *spelldata)
{		
	// loose old map data
	Close();
	
	// default map file
	wstring map_path = path;

	// DEF file
	SpellDEF *def = NULL;

	if (path.rfind(L".DEF") != wstring::npos)
	{
		// --- this is DEF file: parse it ---

		// load DEF file
		def = new SpellDEF(path);

		// parse mission data
		SpellDefSection *mission_data = def->GetSection("MissionData");
		if (!mission_data->Size())
		{
			// likely not a valid DEF file
			return(1);
		}

		for (int k = 0; k < mission_data->Size(); k++)
		{
			if ((*mission_data)[k]->name.compare("MissionMap") == 0)
			{
				// --- *.MAP file definition ---
				size_t pos = map_path.rfind(L"\\", 260);
				if (pos != wstring::npos)
					map_path.resize(pos);
				else
					map_path = L"";
				string map_name = (*(*mission_data)[k]->parameters)[0];
				map_path = map_path + L"\\" + wstring(map_name.begin(), map_name.end()) + L".DTA";
				break;
			}			
		}
		delete mission_data;

		// store DEF file path
		def_path = path;
	}
	else
	{
		// only *.map file
		def_path = L"";		
	}

	// store map path
	this->map_path = map_path;
	
	
	// --- Load MAP file to buffer
	// try open file
	ifstream fr(map_path, ios::in | ios::binary | ios::ate);
	if (!fr.is_open())
		return(1);

	// read to local buffer and close
	streampos flen = fr.tellg();
	fr.seekg(0);
	uint8_t* map_buffer = new uint8_t[flen];
	fr.read((char*)map_buffer, flen);
	fr.close();
	unsigned char* data = map_buffer;
			

	// get L1 data offset
	//int L1_offset = *(__int32*)data;
	data += 4;

	// get L1 sprites list size
	int L1_count = *(__int32*)data;
	data += 4;
	if (L1_count > 4095)
	{
		delete[] map_buffer;
		return(1);
	}

	// version check (actually dunno what is that but it's always the same and looks like version code)
	if (*data++ != 0x12)
	{
		delete[] map_buffer;
		return(1);
	}

	// get map size
	this->x_size = *(__int16*)data;
	data += 2;
	this->y_size = *(__int16*)data;
	data += 2;

	// get map terrain name
	std::memset((void*)this->terrain_name, '\0', sizeof(this->terrain_name));
	std::memcpy((void*)this->terrain_name, data, 13);
	data += 13;

	// try to get terrain data pointer
	this->terrain = spelldata->GetTerrain(this->terrain_name);
	if (!this->terrain)
	{
		delete[] map_buffer;
		return(1);
	}

	// store spelldata
	// ###todo: rework in more sensible way? This should not be there...
	this->spelldata = spelldata;
	
	// load list of used L1 sprites
	Sprite** sprites = new Sprite*[L1_count];
	int *sprite_ids = new int[L1_count];
	for (int k = 0; k < L1_count; k++)
	{
		// read sprite name		
		char name[9];
		std::memset((void*)name, '\0', sizeof(name));
		std::memcpy((void*)name, (void*)data, 8);
		data += 8;

		// try to get sprite's data
		sprites[k] = this->terrain->GetSprite(name);
		if (!sprites[k])
		{
			// not found!
			delete[] sprites;
			delete[] sprite_ids;
			delete[] map_buffer;
			return(1);
		}
		sprite_ids[k] = this->terrain->GetSpriteID(sprites[k]);
	}

	// load L1 sprite indices
	this->L1 = new Sprite * [x_size*y_size];
	this->L1_flags = new uint32_t[x_size*y_size];
	this->elev = new int[x_size * y_size];
	for (int k = 0; k < (x_size * y_size); k++)
	{
		// get cell code
		WORD code = *(WORD*)data;
		data += 2;

		// extract sprite index
		int sid = code & 0x0FFF;

		// elevation code
		int elev = code >> 12;
		
		// put sprite ref to map array
		if (sid > L1_count)
		{
			// out of available range
			delete[] sprites;
			delete[] sprite_ids;
			delete[] map_buffer;
			return(1);
		}
		this->L1[k] = sprites[sid];
		this->elev[k] = elev;

		// get tile flags from context data
		//this->L1_flags[k] = this->terrain->sprites[sprite_ids[sid]]->GetFlags();
	}
	// get tile flags from context data
	SyncL1flags();
	// get rid of L1 sprites list
	delete[] sprites;
	delete[] sprite_ids;
	
	



	//////////////
	///// L2 /////
	//////////////

	// read count of L2 sprites
	int L2_count = *(__int32*)data;
	data += 4;
	if (L2_count > 255)
	{
		return(1);
		delete[] map_buffer;
	}
	
	

	// load list of used L2 sprites
	sprites = new Sprite * [L2_count];
	for (int k = 0; k < L2_count; k++)
	{
		// read sprite name		
		char name[9];
		std::memset((void*)name, '\0', sizeof(name));
		std::memcpy((void*)name, (void*)data, 8);
		data += 8;

		// try to get sprite's data
		sprites[k] = this->terrain->GetSprite(name);
		if (!sprites[k])
		{
			// not found!
			delete[] sprites;
			delete[] map_buffer;
			return(1);
		}
	}

	// load L2 sprite indices
	this->L2 = new Sprite * [x_size * y_size];
	this->flags = new uint8_t[x_size * y_size];
	for (int k = 0; k < (x_size * y_size); k++)
	{
		// get sprite index
		int sid = *(uint8_t*)data++;
		// get sprite flags
		int code = *(uint8_t*)data++;
		
		// put sprite ref to map array
		if (sid > L2_count)
		{
			// out of available range
			delete[] sprites;
			delete[] map_buffer;
			return(1);
		}
		this->L2[k] = (sid)?sprites[sid-1]:NULL;		
		this->flags[k] = code;
	}
	// get rid of L2 sprites list
	delete[] sprites;





	//////////////////////////////
	///// L3 - ANM animation /////
	//////////////////////////////

	// read count of L3 animations
	int L3_count = *(__int32*)data; data += 4;

	// load list of used L3 sprites
	if(L3_count)
	{
		AnimL1 **anims = new AnimL1*[L3_count];
		for (int k = 0; k < L3_count; k++)
		{
			// read anim name
			char name[9];
			std::memset((void*)name, '\0', sizeof(name));
			std::memcpy((void*)name, (void*)data, 8);
			data += 8;

			// try to get sprite's data
			anims[k] = this->terrain->GetANM(name);
			if (!anims[k])
			{
				// not found!
				delete[] anims;
				delete[] map_buffer;
				return(1);
			}
		}

		// decode animation locations
		if (L3_count)
		{
			// get total ANM items count
			int L3_items = *(__int32*)data; data += 4;
		
			// for each item:
			for (int k = 0; k < L3_items; k++)
			{
				// initial frame offset
				int frame_ofs = *data++;
				// frames count limit, ###note: this may be actually frames count from offset???
				// ###todo: findout why frame offset is byte and frames count word??? Maybe it is byte limit, and another byte of whatever?
				int frame_limit = *(__int16*)data; data += 2;
				// x,y position in map
				int x_pos = *(__int16*)data; data += 2;
				int y_pos = *(__int16*)data; data += 2;
				// animation ID
				int aid = *(__int16*)data; data += 2;
						
				// put new animation entry to list
				MapLayer3* anim = new MapLayer3(anims[aid], x_pos, y_pos, frame_ofs, frame_limit);
				L3.push_back(anim);
						
			}

		}

		// clear used animation list
		delete[] anims;
	}



	//////////////////////////////
	///// L4 - PNM animation /////
	//////////////////////////////

	// read count of L4 animations
	int L4_count = *(__int32*)data; data += 4;

	// load list of used L3 sprites
	AnimPNM** pnims = new AnimPNM * [L4_count];
	for (int k = 0; k < L4_count; k++)
	{
		// read anim name
		char name[9];
		std::memset((void*)name, '\0', sizeof(name));
		std::memcpy((void*)name, (void*)data, 8);
		data += 8;

		// try to get sprite's data
		pnims[k] = this->terrain->GetPNM(name);
		if (!pnims[k])
		{
			// not found!
			delete[] pnims;
			delete[] map_buffer;
			return(1);
		}
	}

	// decode animation locations
	if (L4_count)
	{
		// get total PNM items count
		int L4_items = *(__int32*)data; data += 4;

		// for each item:
		for (int k = 0; k < L4_items; k++)
		{
			// 17 bytes per record
			
			// initial frame offset
			int frame_ofs = *data++;
			// frames count limit, ###note: this may be actually frames count from offset???
			// ###todo: findout why frame offset is byte and frames count word??? Maybe it is byte limit, and another byte of whatever?
			int frame_limit = *(WORD*)data; data += 2;
			// x,y position in map
			int x_pos = *(WORD*)data; data += 2;
			int y_pos = *(WORD*)data; data += 2;
			// animation ID
			int pid = *(WORD*)data; data += 2;
			// origin shift in pixels, ###todo: really 4-byte integers??? Seems strange waste of memory
			// ###note: the coordinates seem to be in y,x order for whatever reason
			int y_ofs = *(__int32*)data; data += 4;
			int x_ofs = *(__int32*)data; data += 4; 

			// put new animation entry to list
			MapLayer4* pnim = new MapLayer4(pnims[pid], x_pos, y_pos, x_ofs, y_ofs, frame_ofs, frame_limit);
			L4.push_back(pnim);
		}

	}
	// clear used animation list
	delete[] pnims;



	//////////////////////////////////////
	///// L5 - Mysterious numbers #1 /////
	//////////////////////////////////////

	//////////////////////////////////////
	///// L6 - Mysterious numbers #2 /////
	//////////////////////////////////////

	//////////////////////////
	///// L7 - Sounds #1 /////
	//////////////////////////

	//////////////////////////
	///// L8 - Sounds #2 /////
	//////////////////////////

	// loose map data buffer
	delete[] map_buffer;



	//////////////////////////////////////
	///// Other map arrays and stuff /////
	//////////////////////////////////////
	
	// create selections maps
	select = new uint8_t[x_size*y_size];
	std::memset((void*)select, 0x00, x_size*y_size);

	// filter mask
	filter.resize(x_size*y_size, NULL);

	// init unit pointer PNM
	pnm_sipka = MapLayer4(spelldata->pnm_sipka);


	//////////////////////////
	///// Load DEF stuff /////
	//////////////////////////
	int last_unit_index = 0;
	if (def)
	{
		// parse mission data
		SpellDefSection *mission_data = def->GetSection("MissionData");

		for (int k = 0; k < mission_data->Size(); k++)
		{
			SpellDefCmd* cmd = (*mission_data)[k];
			if (cmd->name.compare("AddStartSquare") == 0)
			{
				// --- AddStartSquare(s) ---				
				for (int p = 0; p < cmd->parameters->size(); p++)
				{					
					int xy = atoi((*cmd->parameters)[p].c_str());
					MapXY coor;				
					coor.y = (xy / x_size);
					coor.x = xy - coor.y * x_size;
					start.push_back(coor);
				}				
			}
			else if (cmd->name.compare("AddEscapeSquare") == 0)
			{
				// --- AddStartSquare(s) ---				
				for (int p = 0; p < cmd->parameters->size(); p++)
				{
					int xy = atoi((*cmd->parameters)[p].c_str());
					MapXY coor;
					coor.y = (xy / x_size);
					coor.x = xy - coor.y * x_size;
					escape.push_back(coor);
				}
			}
			else if (cmd->name.compare("AddUnit") == 0)
			{
				// --- AddUnit(unit_order, unit_id, position, experience, man_count, unit_mode, name) ---				
				MapUnit *unit = new MapUnit();

				// always os
				unit->is_enemy = 1;

				// unit index within map (identifier)
				unit->id = atoi((*cmd->parameters)[0].c_str());

				// unit type index
				unit->type_id = atoi((*cmd->parameters)[1].c_str());
								
				// position
				int xy = atoi((*cmd->parameters)[2].c_str());				
				unit->coor.y = (xy / x_size);
				unit->coor.x = xy - unit->coor.y * x_size;

				// experience
				unit->experience = atoi((*cmd->parameters)[3].c_str());

				// man count
				unit->man = atoi((*cmd->parameters)[4].c_str());

				// unit behaviour type
				unit->type = (*cmd->parameters)[5].c_str();				

				// unit active (to change in game mode)
				unit->is_active = 1;

				// try fetch unit record from spelldata
				unit->unit = spelldata->units->GetUnit(unit->type_id);				
				if (!unit->unit)
				{
					delete mission_data;
					delete unit;
					return(1);
				}

				// copy unit name
				strcpy_s(unit->name,sizeof(unit->name),unit->unit->name);
				auto & custom_name = cmd->parameters->at(6);
				if(custom_name.size() && custom_name.compare("-")!=0)
					strcpy_s(unit->name,sizeof(unit->name),custom_name.c_str());

				// initial action points
				unit->ResetAP();

				unit->dig_level = rand()%7;
				unit->commander_id = rand()%9;
				unit->is_commander = rand()%2;
				unit->morale = 1 + rand()%100;


				// add unit to list
				units.push_back(unit);
			}
			else if (cmd->name.compare("AddSpecialEvent") == 0)
			{
				// --- Event definitions: AddSpecialEvent(type, position, index, probability) ---
				
				int index;
				int probab;
				if(cmd->parameters->at(0).compare("SeePlace") == 0)
				{				
					// event position xy
					MapXY coor;					
					int xy = atoi(cmd->parameters->at(1).c_str());
					coor.y = (xy / x_size);
					coor.x = xy - coor.y * x_size;
				
					// event index
					index = atoi(cmd->parameters->at(2).c_str());

					// probability
					probab = atoi(cmd->parameters->at(3).c_str());
				}
				else if(cmd->parameters->at(0).compare("MissionStart") == 0)
				{
					// event index
					index = atoi(cmd->parameters->at(1).c_str());

					// probability
					probab = atoi(cmd->parameters->at(2).c_str());
				}
				else
					continue;
				
				// parse event data
				string event_data_header = "EventData(" + std::to_string(index) + ")";
				SpellDefSection *event_data = def->GetSection(event_data_header);

				// for each item in event:
				for (int evid = 0; evid < event_data->Size(); evid++)
				{
					SpellDefCmd* evcmd = (*event_data)[evid];

					if (evcmd->name.compare("AddSpecialUnit") == 0)
					{
						// --- AddSpecialUnit(unit_order, unit_id, position, experience, man_count, name) ---				
						MapUnit* unit = new MapUnit();

						// unit index within map (identifier)
						unit->id = last_unit_index++;

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
							unit->type = MapUnitType::SpecUnit1;
						}
						
						// unit type index
						unit->type_id = atoi((*evcmd->parameters)[1].c_str());

						// position
						int xy = atoi((*evcmd->parameters)[2].c_str());
						unit->coor.y = (xy / x_size);
						unit->coor.x = xy - unit->coor.y * x_size;

						// experience
						unit->experience = atoi((*evcmd->parameters)[3].c_str());

						// man count
						unit->man = atoi((*evcmd->parameters)[4].c_str());						

						// unit active (to change in game mode)
						unit->is_active = 1;

						// try fetch unit record from spelldata
						unit->unit = spelldata->units->GetUnit(unit->type_id);
						if (!unit->unit)
						{
							delete event_data;
							delete unit;
							return(1);
						}

						// copy unit name
						strcpy_s(unit->name, sizeof(unit->name),unit->unit->name);
						auto& custom_name = evcmd->parameters->at(5);
						if(custom_name.size() && custom_name.compare("-")!=0)
							strcpy_s(unit->name,sizeof(unit->name),custom_name.c_str());

						// default action points
						unit->ResetAP();

						unit->dig_level = rand()%7;
						unit->commander_id = rand()%9;
						unit->is_commander = rand()%2;
						unit->morale = 1 + rand()%100;


						// add unit to list
						units.push_back(unit);
					}
				}
				// clear parsed event data
				delete event_data;

			}
		}
		delete mission_data;
		delete def;

		// initialize units state variables:
		for (int k = 0; k < units.size(); k++)
		{
			int azn_stat = units[k]->unit->gr_base->stat.azimuths;
			int azn_anim = units[k]->unit->gr_base->anim.azimuths;
			int azn_step = 1;
			if (azn_anim)
			{
				azn_step = azn_stat / azn_anim;
				units[k]->azimuth_anim = rand() % azn_anim;
				units[k]->azimuth = units[k]->azimuth_anim * azn_step;
			}
			else
			{
				units[k]->azimuth_anim = 0;
				units[k]->azimuth = rand() % azn_stat;				
			}			
			units[k]->frame = 0;
		}
		
	}

	// select some unit
	if(units.size())
		unit_selection = units[0];

	// clear units view range map
	PrepareUnitsViewMask();
	ClearUnitsView(true);

	// init range maps
	InitUnitRangeStuff();
	
	// prefetch pointers to common stuff needed fast when rendering
	start_sprite = terrain->GetSprite("START");
	escape_sprite = terrain->GetSprite("CIEL");

	
	// done
	return(0);
}

// convert x,y to combined tile position
int SpellMap::ConvXY(int x, int y)
{
	if(x >= 0 && y >= 0)
		return(x + y * x_size);
	return(-1);
}
int SpellMap::ConvXY(MapXY *mxy)
{
	if(mxy->IsSelected())
		return(mxy->x + mxy->y * x_size);
	return(-1);
}
int SpellMap::ConvXY(MapXY &mxy)
{
	return(ConvXY(&mxy));
}


// check units were moved and eventually clear moved flags
int SpellMap::UnitsMoved(int clear)
{
	// scan units for placement changes and clear the move flags
	int moved = units_moved;
	for(auto& unit : units)
	{
		if(unit->was_moved)
			moved = true;
		if(clear)
			unit->was_moved = false;
	}
	if(clear)
		units_moved = false;
	return(moved);
}

// invalidate current units view map to force recalculation (actual calculation is done while rendering for now)
void SpellMap::InvalidateUnitsView()
{
	units_moved = true;
}

// this sorts units list for proper render order, call after load or units changes
void SpellMap::SortUnits()
{
	int k;
		
	// allocate units layer array
	if (!Lunit)
		Lunit = new MapUnit*[x_size * y_size];
	
	// clear units layer array
	std::memset((void*)Lunit, NULL, sizeof(MapUnit*)*x_size*y_size);

	// clear render chain pointers
	for (k = 0; k < units.size(); k++)
	{
		units[k]->next = NULL;
		units[k]->assigned = 0;
	}

	// --- sort render order, ie. first unit pointer to layer array, next unit pointer to unit record themselves	
	for (int ut = 0; ut < 2; ut++)
	{		
		for (k = 0; k < units.size(); k++)
		{
			MapUnit* new_unit = units[k];

			// int first pass go for air units only because they have to go first
			int allowed = 1;
			if (ut == 0)
				allowed = new_unit->unit->isAir();

			if (allowed && !new_unit->assigned)
			{
				// not assigned air unit:
				MapUnit** lay = &Lunit[ConvXY(new_unit->coor)];
				if (!*lay)
				{
					// first unit at this position, so place it to layer array
					*lay = new_unit;
				}
				else
				{
					// not first unit at position, so search last unit at this position and append it
					MapUnit* next_unit = *lay;
					while (next_unit->next)
					{
						// used, go to next
						next_unit = next_unit->next;
					}
					// got first free unit it, append new one
					next_unit->next = new_unit;
				}
				new_unit->assigned = 1;
			}
		}
	}
}

// is some valid map data loaded?
int SpellMap::IsLoaded()
{
	if (!L1 || !L2 || !elev || !flags)
		return(0);

	if (!x_size || !y_size)
		return(0);

	// ###todo: should be checked more rigorously...
	return(1);
}



// get total size (in pixels) of map
tuple<int,int> SpellMap::GetMapSurfaceSize()
{
	return tuple(pic_x_size, pic_y_size);
}

// call whenever scroll or screen changes
// it allocates render buffer and fixes scroller limits
int SpellMap::RenderPrepare(wxBitmap& bmp, TScroll* scroll)
{
	if(!IsLoaded())
		return(1);

	// get surface size
	surf_x = bmp.GetWidth();
	surf_y = bmp.GetHeight();

	// reinit of surface changed (this is only needed when map is smaller than screen)
	if(pic && (surf_x != last_surf_x || surf_y != last_surf_y))
	{
		delete[] pic;	
		pic = NULL;		
	}

	// remember last surface size
	last_surf_x = surf_x;
	last_surf_y = surf_y;
	
	// allocate maximum indexed image memory for entire map	
	if(!pic)
	{
		// new render buffer size
		pic_x_size = max((x_size)*80 + 40,surf_x);
		pic_y_size = max(MSYOFS + 60 + 47 + (y_size) * 24 + MSYOFS,surf_y);
		// allocate it
		pic = new uint8_t[pic_x_size * pic_y_size];		
		// buffer end for range checking
		pic_end = &pic[pic_x_size * pic_y_size];
		// surface modified flag (used mainly for rearranging HUD buttons)
		surf_modified = true;
	}
	if(!pic)
		return(1);	

	// limit scroller to valid range
	int scroll_x_min = 0;
	int scroll_y_min = 0;
	int scroll_x_max = pic_x_size - surf_x;
	int scroll_y_max = pic_y_size - surf_y;
	if(game_mode)
	{
		scroll_x_min = 120;
		scroll_y_min = 250;
		scroll_x_max -= 240;
		scroll_y_max -= 400;
		if(!hud_enabled)
			scroll_y_max -= 90;
	}
	scroll->CheckScroll(scroll_x_min,scroll_y_min,scroll_x_max,scroll_y_max);

	// top-left visible portion of buffer (surface origin)	
	auto [x_pos,y_pos] = scroll->GetScroll();
	surf_x_origin = (x_pos >= 80 ? (80) : (0)) + (x_pos % 80);
	surf_y_origin = (y_pos / 48 <= MSYOFST ? (y_pos / 48) * 48 : MSYOFST * 48) + (y_pos % 48);
			
	// tiles to draw
	xs_size = surf_x / 80 + 3;
	ys_size = surf_y / 24 + 12;
	// scroll offset in tiles
	auto [dx,dy] = scroll->GetScroll();
	xs_ofs = dx / 80 - 1;
	ys_ofs = dy / 48 - MSYOFST;
	if(xs_ofs < 0)
		xs_ofs = 0;
	if(ys_ofs < 0)
		ys_ofs = 0;	

	return(0);
}
// return true if surface was modified in last RenderPrepare()
int SpellMap::isRenderSurfModified()
{
	return(surf_modified);
}
// call to commit surface modified flag
int SpellMap::CommitRenderSurfModified()
{
	int temp = surf_modified;
	surf_modified = false;
	return(temp);
}


// update map persistent selection by list of selected tiles
void SpellMap::SelectTiles(vector<MapXY> tiles, int mode)
{
	// modify selection array
	for(int k = 0;k < tiles.size();k++)
	{
		if(mode == SELECT_ADD)
			select[ConvXY(tiles[k])] = 1;
		else if(mode == SELECT_CLEAR)
			select[ConvXY(tiles[k])] = 0;
		else if(mode == SELECT_XOR)
			select[ConvXY(tiles[k])] ^= 1;
	}
}
// (de)select all tiles in visible map range (ignores borders where are mostly some texture mismatches)
void SpellMap::SelectTiles(int mode)
{
	for(int y = 0; y < y_size; y++)
		for(int x = 0; x < x_size; x++)
			if(mode == SELECT_ADD && TileIsVisible(x,y))
				select[ConvXY(x,y)] = 1;
			else if(mode == SELECT_CLEAR)
				select[ConvXY(x,y)] = 0;
}
// return list of persistent selections
vector<MapXY> SpellMap::GetPersistSelections()
{
	vector<MapXY> list;
	for(int y = 0; y < y_size; y++)
		for(int x = 0; x < x_size; x++)
		{
			if(select[ConvXY(x,y)])
			{
				MapXY pxy(x,y);
				list.push_back(pxy);
			}
		}
	return(list);
}




// analyze map selection
vector<MapXY> &SpellMap::GetSelections(wxBitmap& bmp, TScroll *scroll)
{
	int m,n,i,j;
		
	// check & fix surface and scroll ranges
	if(!RenderPrepare(bmp, scroll))
	{	
		// default no selection
		MapXY selxy = sel;
	
		if(scroll->Commit())
		{	
			// no select found yet
			selxy.x = -1;
			selxy.y = -1;

			// get scroll position
			auto [dx,dy] = scroll->GetScroll();
			// get cursor position relative to scroll
			auto [xref,yref] = scroll->GetCursor();
		
			const int smooth = 1;

			// ###todo: this whole thing is just awful and should be reworked! Here should not be direct access to sprite data - move to Sprite class.
			//          also it is ineffective. Should be more analytical to reduce search range.
			// find selection tiles
			for(m = 0; m < ys_size; m++)
			{
				if(m + ys_ofs * 2 >= y_size)
					break;
				for(n = 0; n < xs_size; n++)
				{
					if(n + xs_ofs >= x_size)
						break;
					// --- for every valid tile ---

					// get map tile
					Sprite* sid = L1[(m + ys_ofs * 2) * x_size + n + xs_ofs];
					int sof = elev[(m + ys_ofs * 2) * x_size + n + xs_ofs];

					// get dimensions for current tile slope
					char sn = sid->name[2]; /* tile slope */
					int so = sid->y_ofs; /* vertical offset */
					int sx = spelldata->special.solid[sn - 'A'].x_size;
					int sy = spelldata->special.solid[sn - 'A'].y_size;


					// pixel coordinates of search rect
					int mxx = n * 80 + (((m & 1) != 0) ? 0 : 40);
					int ymn = -((dy / 48 <= MSYOFST ? (dy / 48) * 48 : MSYOFST * 48) + (dy % 48) * smooth);
					int xmn = mxx - ((dx >= 80 ? (80) : (0)) + (dx % 80) * smooth);
					if(sn == 'C' || sn == 'D' || sn == 'M' || sn == 'B')
						ymn += (m * 24 - sy + 47 - sof * 18 + MSYOFS + 50);
					else if(sn == 'L' || sn == 'F')
						ymn += (m * 24 - sy + 29 - sof * 18 + MSYOFS + 50);
					else
						ymn += (m * 24 - sof * 18 + MSYOFS + 50);

					// now decide which tile is selected in search rect
					if(yref != -1 && yref > ymn && yref < ymn + sy &&
						xref != -1 && xref > xmn && xref < xmn + sx)
					{
						// we are somewhere in the expected tile area - check if we are in tile polygons
						// tile data
						unsigned char* seld = spelldata->special.solid[sn - 'A'].data;
						int sla = 0;
						for(i = 0; i < sy; i++)
						{
							unsigned char oo = *((unsigned int*)seld); seld += 4;
							unsigned char nn = *((unsigned int*)seld); seld += 4;
							if(nn == 0)
								continue;
							if((yref - ymn) == i && (xref - xmn) >= oo && (xref - xmn) < (oo + nn) &&
								seld[xref - xmn - oo] != 0)
							{
								sla = 1;
								break;
							}
							seld += nn;
						}

						// some valid?
						if(sla)
						{
							// yaha, remember which one and leave
							selxy.x = n + xs_ofs;
							selxy.y = m + ys_ofs*2;
							goto seldone;
						}
					}
				}
			}
		seldone:

			// store selection
			sel = selxy;

			// first multiselect item is always central
			msel.clear();
			msel.push_back(selxy);

			// find multiselect tiles:
			if(selxy.IsSelected())
			{		
				// selection size
				int size = scroll->GetSize();
				int minref = -(size - 1)/2;

				for(j = 0; j < size; j++)
				{
					for(i = 0; i < size; i++)
					{
						// this point
						MapXY pxy;
						pxy.x = (selxy.x*2 + ((selxy.y%2)?0:1) + minref+i+minref+j)>>1;
						pxy.y = selxy.y + minref+j-(minref+i);

						// skip point if center point
						if(pxy.x == selxy.x && pxy.y == selxy.y)
							continue;
				
						// put new point to list
						if(pxy.x < x_size && pxy.x >= 0 && pxy.y < y_size && pxy.y >= 0)
							msel.push_back(pxy);
					}
				}
			}

		}
	}

	return(msel);
}
MapXY SpellMap::GetSelection(wxBitmap& bmp,TScroll* scroll)
{
	vector<MapXY>& sel = GetSelections(bmp, scroll);
	if(sel.size())
		return(sel[0]);
	else
		return(MapXY());
}
// get selection elevation
int SpellMap::GetElevation(wxBitmap& bmp,TScroll* scroll)
{
	// fix selection
	MapXY sel = GetSelection(bmp,scroll);
	if(sel.IsSelected())
	{
		// return tile name
		return((elev[ConvXY(sel)]));
	}
	else
		return(0);	
}
// get L1 terrain tile name
char *SpellMap::GetL1tileName(wxBitmap& bmp, TScroll* scroll)
{
	// fix selection
	MapXY sel = GetSelection(bmp, scroll);

	if(sel.IsSelected())
	{
		// return tile name
		return(L1[ConvXY(sel)]->name);
	}
	else
		return(NULL);
}

// get L2 object tile name
char* SpellMap::GetL2tileName(wxBitmap& bmp,TScroll* scroll)
{
	// fix selection
	MapXY sel = GetSelection(bmp,scroll);

	if(sel.IsSelected())
	{
		// return tile name
		return(L2[ConvXY(sel)]->name);
	}
	else
		return(NULL);
}

// get map flag array's object height
int SpellMap::GetFlagHeight(MapXY *sel)
{
	return((flags[ConvXY(sel)] >> 4)&0x0F);
}
// get map flag array's flag part
int SpellMap::GetFlagFlag(MapXY* sel)
{
	return((flags[ConvXY(sel)])&0x0F);
}
// get flags for given tile (###todo: decode what those flags actually means?)
tuple<int,int,int> SpellMap::GetTileFlags(wxBitmap& bmp,TScroll* scroll)
{
	// fix selection
	MapXY sel = GetSelection(bmp,scroll);
	
	if(sel.IsSelected())
	{
		// return tile name
		return {GetFlagFlag(&sel), GetFlagHeight(&sel), flags[ConvXY(sel)]};
	}
	else
		return {0,0,0};
}
// return vector of sprites matching the given selection
vector<Sprite*> SpellMap::GetL1sprites(vector<MapXY>& selection)
{
	vector<Sprite*> list;
	for(int k = 0; k < selection.size(); k++)
	{
		Sprite *spr = NULL;
		if(selection[k].IsSelected())
			spr = L1[ConvXY(selection[k])];
		list.push_back(spr);
	}
	return(list);
}
// return vector of sprites matching the given selection
vector<Sprite*> SpellMap::GetL2sprites(vector<MapXY>& selection)
{
	vector<Sprite*> list;
	for(int k = 0; k < selection.size(); k++)
	{
		Sprite* spr = NULL;
		if(selection[k].IsSelected())
			spr = L2[ConvXY(selection[k])];
		list.push_back(spr);
	}
	return(list);
}
// return vector of L1/2 flags matching the given selection
vector<uint8_t> SpellMap::GetFlags(vector<MapXY>& selection)
{
	vector<uint8_t> list;
	for(int k = 0; k < selection.size(); k++)
	{
		uint8_t spr = 0x00;
		if(selection[k].IsSelected())
			spr = flags[ConvXY(selection[k])];
		list.push_back(spr);
	}
	return(list);
}

// render frame
int SpellMap::Render(wxBitmap &bmp, TScroll* scroll, SpellTool *tool,std::function<void(void)> hud_buttons_cb)
{
	int i;
	int m, n;

	// skip if invalid pointer
	if (!IsLoaded())
		return(1);

	// prepare scroll & buffer
	if(RenderPrepare(bmp, scroll))
		return(1);

	// find selection tiles
	auto msel = GetSelections(bmp,scroll);
	MapXY cursor;
	if(msel.size() && msel[0].IsSelected())
		cursor = msel[0];

	// clear used surface range
	for(int y = 0; y < min(surf_y,pic_y_size-surf_y_origin); y++)
	{
		uint8_t* src = &pic[surf_x_origin + (surf_y_origin + y)*pic_x_size];		
		std::memset((void*)src,MAP_BACK_COLOR,min(surf_x,pic_x_size - surf_x_origin));
		/*int rend_x = surf_x;
		if(&src[surf_x] > pic_end)
			rend_x = pic_end - src;		
		std::memset((void*)src,MAP_BACK_COLOR,rend_x);*/
	}

	// edit tool pre-processing:
	Sprite **tool_L1 = NULL;
	if(msel.size() && tool && tool->isObject())
	{
		// object tool selected (house, etc.)
		auto obj = tool->GetObject();

		// make temp tool object array for L1
		tool_L1 = new Sprite*[x_size*y_size];
		std::memset((void*)tool_L1, 0, sizeof(Sprite*)*x_size*y_size);

		// place object's tiles to L1 overlay
		obj->PlaceMapTiles(tool_L1, x_size, y_size, msel[0]);

	}

	// check if units position moved since last test (and clear movement flags)
	int units_moved = UnitsMoved();

	if(units_moved && game_mode)
	{
		// update view mask (this should be probably somewhere else?)
		ClearUnitsView(false);
		AddUnitsView();
	}

	// sort units positions for rendering
	if(units_moved)
		SortUnits();

	
	// --- Render Layer 1 - ground sprites ---
	for (m = 0; m < ys_size; m++)
	{
		if (m + ys_ofs * 2 >= y_size)
			break;
		for (n = 0; n < xs_size; n++)
		{
			if (n + xs_ofs >= x_size)
				break;
			int mxy = ConvXY(n + xs_ofs,m + ys_ofs*2);

			// select view area
			uint8_t *fil = NULL;
			if(!TileIsVisible(n + xs_ofs,m + ys_ofs*2))
				fil = terrain->filter.darkpal;

			// game mode view range
			if(game_mode && units_view[mxy] == 0)
				continue;
			else if(game_mode && units_view[mxy] == 1)
				fil = terrain->filter.darkpal;

			// apply optional filter mask
			if(filter[mxy])
				fil = filter[mxy];

			// get sprite parameters
			
			Sprite* sid = L1[mxy];
			char sn = sid->name[2];
			int sof = elev[mxy];

			// override sprite by plain one?
			if (!wL1)
			{
				// try get class glyph
				sid = terrain->GetTileGlyph(sid);
				// use blank one of not found
				if(!sid)				
					sid = &spelldata->special.grid[sn - 'A'];
			}

			// override layer by tool?
			if(tool_L1 && tool_L1[mxy])
			{
				sid = tool_L1[mxy];
			}

			// render sprite
			int mxx = n * 80 + (((m & 1) != 0) ? 0 : 40);
			int myy = m * 24 - sof * 18 + MSYOFS + 50;
			sid->Render(pic, pic_end, mxx, myy, pic_x_size, fil);
		}
	}

	if(tool_L1)
		delete[] tool_L1;

	// render 3D terrain mesh overlay (for debug only)
	/*for (m = 0; m < ys_size; m++)
	{
		if (m + ys_ofs * 2 >= y_size)
			break;
		for (n = 0; n < xs_size; n++)
		{
			if (n + xs_ofs >= x_size)
				break;
			// get sprite parameters
			Sprite* sid = L1[(m + ys_ofs * 2) * x_size + n + xs_ofs];
			int sof = elev[(m + ys_ofs * 2) * x_size + n + xs_ofs];

			// render sprite
			int mxx = n * 80 + ((m & 1 != 0) ? 0 : 40);
			int myy = m * 24 - sof * 18 + MSYOFS + 50;

			sid->RenderTileWires(pic, pic_end, mxx, myy, pic_x_size, 252);
		}
	}*/

	
	// --- Redner Layer 3 - ANM animations ---
	if (wL3)
	{
		for (i = 0; i < L3.size(); i++)
		{
			MapLayer3* anm = L3[i];

			// skip if not in visible area
			if (anm->x_pos < xs_ofs || anm->x_pos >= (xs_ofs + xs_size) || anm->y_pos < ys_ofs * 2 || anm->y_pos >= (ys_ofs * 2 + ys_size))
				continue;
			int mxy = ConvXY(anm->x_pos, anm->y_pos);

			// game mode view range
			uint8_t* fil = NULL;
			if(game_mode && units_view[mxy] == 0)
				continue;
			else if(game_mode && units_view[mxy] == 1)
				fil = terrain->filter.darkpal;

			// L1 elevation
			int sof = elev[mxy];

			// get sprite
			Sprite* frame = anm->anim->frames[anm->frame_ofs];

			// render sprite
			int mxx = (anm->x_pos - xs_ofs) * 80 + ((((anm->y_pos - ys_ofs * 2) & 1) != 0) ? 0 : 40);
			int myy = (anm->y_pos - ys_ofs * 2) * 24 - sof * 18 + MSYOFS + 50;
			frame->Render(pic, pic_end, mxx, myy, pic_x_size, fil);
		}
	}

	// --- Redner special tiles - START/CIEL ---
	if (wSTCI)
	{
		// for each special sprite type
		vector<MapXY> *spec[] = { &start, &escape };
		Sprite* spec_sprite[] = { start_sprite, escape_sprite };
		for (int sid = 0; sid < 2; sid++)
		{
			for (i = 0; i < spec[sid]->size(); i++)
			{
				MapXY* pos = &(*spec[sid])[i];

				// skip if not in visible area
				if (pos->x < xs_ofs || pos->x >= (xs_ofs + xs_size) || pos->y < ys_ofs * 2 || pos->y >= (ys_ofs * 2 + ys_size))
					continue;
				int mxy = ConvXY(pos->x,pos->y);

				// game mode view range
				uint8_t* fil = NULL;
				if(game_mode && units_view[mxy] == 0)
					continue;
				else if(game_mode && units_view[mxy] == 1)
					fil = terrain->filter.darkpal;

				// L1 elevation
				int y_elev = elev[mxy];

				// render sprite
				int mxx = (pos->x - xs_ofs) * 80 + ((((pos->y - ys_ofs * 2) & 1) != 0) ? 0 : 40);
				int myy = (pos->y - ys_ofs * 2) * 24 - y_elev * 18 + MSYOFS + 50;
				spec_sprite[sid]->Render(pic, pic_end, mxx, myy, pic_x_size, fil);

			}
		}
	}


	// --- Render selection tiles (persistent ones) ---
	for(m = 0; m < ys_size; m++)
	{
		if(m + ys_ofs * 2 >= y_size)
			break;
		for(n = 0; n < xs_size; n++)
		{
			if(n + xs_ofs >= x_size)
				break;
			int x_pos = n + xs_ofs;
			int y_pos = m + ys_ofs*2;
			int mxy = ConvXY(x_pos, y_pos);			

			// skip if not selected
			if(!select[mxy])
				continue;			

			// remap tile colors
			uint8_t fil[256];
			fil[193] = 230;
			
			// get tile
			Sprite *spr = L1[mxy];							
			int sof = elev[mxy];
			// override by selector
			spr = &spelldata->special.select[spr->GetSlope() - 'A'];

			// render sprite
			int mxx = n * 80 + (((m & 1) != 0) ? 0 : 40);
			int myy = m * 24 - sof * 18 + MSYOFS + 50;
			spr->Render(pic,pic_end,mxx,myy,pic_x_size,fil);
		}
	}



	// --- Render cursor selection tiles ---
	if (msel.size() && msel[0].IsSelected())
	{		
		for (i = 0; i < msel.size(); i++)
		{
			n = msel[i].x - xs_ofs;
			m = msel[i].y - ys_ofs * 2;

			if (n < 0 || m < 0 || (m + ys_ofs * 2) >= y_size || (n + xs_ofs) >= x_size)
				continue;
			int mxy = ConvXY(n + xs_ofs, m + ys_ofs*2);

			// game mode view range
			uint8_t* fil = NULL;
			if(game_mode && units_view[mxy] == 0)
				continue;
			else if(game_mode && units_view[mxy] == 1)
				fil = terrain->filter.darkpal;

			// get map tile
			Sprite *sid = L1[mxy];
			int sof = elev[mxy];

			// get sprite
			char sn = sid->name[2];

			// override sprite by selector
			sid = &spelldata->special.select[sn - 'A'];

			// draw selection sprite
			int mxx = n * 80 + (((m & 1) != 0) ? 0 : 40);
			int myy = m * 24 - sof * 18 + MSYOFS + 50;
			sid->Render(pic, pic_end, mxx, myy, pic_x_size, fil);
		}
	}

	
	
	// --- Redner Layer 4 - PNM animations ---
	if (wL4)
	{			
		for (i = 0; i < L4.size(); i++)
		{
			MapLayer4* pnm = L4[i];

			// skip if not in visible area
			if (pnm->x_pos < xs_ofs || pnm->x_pos >= (xs_ofs + xs_size) || pnm->y_pos < ys_ofs * 2 || pnm->y_pos >= (ys_ofs * 2 + ys_size))
				continue;
			int mxy = ConvXY(pnm->x_pos, pnm->y_pos);

			// game mode view range
			uint8_t* fil = NULL;
			if(game_mode && units_view[mxy] == 0)
				continue;
			else if(game_mode && units_view[mxy] == 1)
				fil = terrain->filter.darkpal;

			// L1 elevation
			int y_elev = elev[mxy];
			
			// get sprite
			Sprite* frame = pnm->anim->frames[pnm->frame_ofs];

			// render sprite
			int mxx = (pnm->x_pos - xs_ofs)*80 + pnm->x_ofs + ((((pnm->y_pos - ys_ofs*2) & 1) != 0) ? 0 : 40);
			int myy = (pnm->y_pos - ys_ofs * 2) * 24 + pnm->y_ofs - y_elev*18 + MSYOFS + 50;
			frame->Render(pic, pic_end, mxx, myy, pic_x_size, fil);
		}
	}



	// --- Render Layer 2 - Objects ---
	// --- Redner Units ---
	for (m = 0; m < ys_size; m++)
	{
		if (m + ys_ofs * 2 >= y_size)
			break;
		for (n = 0; n < xs_size; n++)
		{
			if (n + xs_ofs >= x_size)
				break;
			int mxy = ConvXY(n + xs_ofs, m + ys_ofs * 2);

			// select view area
			uint8_t* filter = NULL;
			if(!TileIsVisible(n + xs_ofs,m + ys_ofs*2))
				filter = terrain->filter.darkpal;			

			// tile selected
			int is_selected = (msel.size() && msel[0].IsSelected() && msel[0].x - xs_ofs == n && msel[0].y - ys_ofs*2 == m);

			// first unit to render
			MapUnit* unit = Lunit[mxy];			
			// L1 tile
			Sprite* sid = L1[mxy];
			int sof = elev[mxy];
			char slope = sid->name[2];
			// L2 tile
			Sprite* sid2 = L2[mxy];

			// game mode view range
			int hide_units = game_mode && units_view[mxy] == 1;
			if(game_mode && units_view[mxy] == 0)
				continue;
			else if(hide_units)
				filter = terrain->filter.darkpal;
			

			// render origin
			int mxx = n * 80 + (((m & 1) != 0) ? 0 : 40);
			int myy = m * 24 - sof * 18 + MSYOFS + 50/* + sid->y_ofs*/;

			// 3 pass render: 1) air units, 2) objects, 3) rest of units
			for (int pass = 0; pass < 3; pass++)
			{										
				// units render:
				while (wUnits && unit && !hide_units)
				{
					// unit animation frame selection
					int frame, azimuth;
					if (is_selected && unit->unit->gr_base->anim.frames)
					{
						frame = unit->frame;
						azimuth = unit->azimuth_anim;
					}
					else
					{
						frame = -1;
						azimuth = unit->azimuth;
					}

					if ((pass == 0 && unit->unit->isAir()) || pass == 2)
					{
						int top_y_ofs = unit->Render(terrain, pic, pic_end, mxx, myy, pic_x_size, filter, sid, w_unit_hud,azimuth, frame);

						// render selection pointer for selected unit
						if(unit_selection && unit_selection->coor.IsEqual(n + xs_ofs,m + ys_ofs * 2))
						{
							Sprite* frame = pnm_sipka.anim->frames[pnm_sipka.frame_ofs];
							frame->Render(pic, pic_end, mxx, myy + top_y_ofs - pnm_sipka.anim->y_max - 8, pic_x_size, filter);
						}
					}
					else
						break;
					// go to next unit at this position
					unit = unit->next;
				}							
					
				// object render:
				if (wL2 && pass == 1 && sid2)
					sid2->Render(pic, pic_end, mxx, myy + sid->y_ofs, pic_x_size, filter);
			}
		}
	}


	MapUnit *unit = GetSelectedUnit();
	MapXY t_pos = GetSelection(bmp, scroll);
	/*if(unit && t_pos.IsSelected())
		FindUnitPath(unit, t_pos);*/
	
	if(unit && t_pos.IsSelected())
		FindUnitRange(unit);

	
	// show debug order lines
	for(int k = 0; k < (int)dbg_ord.size()-1; k++)
	{
		MapXY mxy = dbg_ord[k];
		MapXY nxy = dbg_ord[k+1];
		
		// skip if not in visible area
		if(mxy.x < xs_ofs || mxy.x >= (xs_ofs + xs_size) || mxy.y < ys_ofs * 2 || mxy.y >= (ys_ofs * 2 + ys_size))
			continue;
		if(nxy.x < xs_ofs || nxy.x >= (xs_ofs + xs_size) || nxy.y < ys_ofs * 2 || nxy.y >= (ys_ofs * 2 + ys_size))
			continue;

		// L1 elevation
		Sprite* mspr = L1[ConvXY(mxy)];
		int m_y_elev = elev[ConvXY(mxy)];
		Sprite* nspr = L1[ConvXY(nxy)];
		int n_y_elev = elev[ConvXY(nxy)];
		
		// line ends
		int mxx = (mxy.x - xs_ofs)*80 + ((((mxy.y - ys_ofs*2) & 1) != 0) ? 0 : 40) + mspr->x_size/2;
		int myy = (mxy.y - ys_ofs * 2) * 24 - m_y_elev*18 + MSYOFS + 50 + mspr->y_ofs + mspr->y_size/2;
		int nxx = (nxy.x - xs_ofs)*80 + ((((nxy.y - ys_ofs*2) & 1) != 0) ? 0 : 40) + nspr->x_size/2;
		int nyy = (nxy.y - ys_ofs * 2) * 24 - n_y_elev*18 + MSYOFS + 50 + nspr->y_ofs + nspr->y_size/2;

		plot_line(pic, pic_end, 0, 0, pic_x_size, 252, mxx,myy, nxx,nyy);
	}

	// DEBUG: render mean elevations (for units view range)
	/*for(m = 0; m < ys_size; m++)
	{
		if(m + ys_ofs * 2 >= y_size)
			break;
		for(n = 0; n < xs_size; n++)
		{
			if(n + xs_ofs >= x_size)
				break;
			int x_pos = n + xs_ofs;
			int y_pos = m + ys_ofs*2;
			int mxy = ConvXY(x_pos,y_pos);

			// get view height map elevation
			auto [vx, vy, vz] = GetUnitsViewTileCenter(x_pos, y_pos);
			auto [tile_id, vh] = GetUnitsViewMask(vx, vy);
			string hstr = string_format("%d",vh);

			// get tile
			Sprite* spr = L1[mxy];
			int sof = elev[mxy];

			// render sprite
			int mxx = n * 80 + (((m & 1) != 0) ? 0 : 40);
			int myy = m * 24 - sof * 18 + MSYOFS + 50;

			terrain->font7->Render(pic, pic_end, pic_x_size, mxx, myy + spr->y_ofs, 80, spr->y_size, hstr, 252, 254, SpellFont::DIAG);
		}
	}*/

	// Debug: render remaining ap/fire
	for(m = 0; m < ys_size; m++)
	{
		if(m + ys_ofs * 2 >= y_size)
			break;
		for(n = 0; n < xs_size; n++)
		{
			if(n + xs_ofs >= x_size)
				break;
			int x_pos = n + xs_ofs;
			int y_pos = m + ys_ofs*2;
			int mxy = ConvXY(x_pos,y_pos);

			// get view height map elevation
			int ap_left = unit_ap_left[mxy];
			if(ap_left < 0)
				continue;
			string hstr = string_format("%d",ap_left);

			// get tile
			Sprite* spr = L1[mxy];
			int sof = elev[mxy];

			// render sprite
			int mxx = n * 80 + (((m & 1) != 0) ? 0 : 40);
			int myy = m * 24 - sof * 18 + MSYOFS + 50;

			terrain->font7->Render(pic,pic_end,pic_x_size,mxx,myy + spr->y_ofs,80,spr->y_size,hstr,252,254,SpellFont::DIAG);
		}
	}


	
	
	// check unit on cursor
	MapUnit* cursor_unit = GetCursorUnit(bmp, scroll);
		
	// render HUD
	RenderHUD(pic, pic_end, pic_x_size, &cursor, cursor_unit, hud_buttons_cb);

	
	
	// --- translate indexed image buffer to RGB bitmap scanline by scanline:

	// apply gamma correction to palette:
	if (gamma != last_gamma)
	{
		// store last gamma to prevent recalculatiom in each frame
		last_gamma = gamma;

		// male local copy of palette	
		std::memcpy((void*)pal, (void*)terrain->pal, 3 * 256);

		// apply gamma correction (this should be maybe optimized out of here?
		for (int k = 0; k < 256; k++)
			for (int c = 0; c < 3; c++)
				pal[k][c] = (uint8_t)(pow((double)pal[k][c] / 255.0, 1.0 / gamma) * 255.0);
	}
	
	
	
	// render 24bit RGB data to raw bmp buffer
	wxNativePixelData data(bmp);
	wxNativePixelData::Iterator p(data);
	for(int y = 0; y < surf_y; ++y)
	{
		uint8_t* scan = p.m_ptr;
		uint8_t* src = &pic[surf_x_origin + (surf_y_origin + y)*pic_x_size];
		int rend_x = surf_x;
		if(&src[surf_x] > pic_end)
			rend_x = pic_end - src;
		for(int x = 0; x < rend_x; x++)
		{
			*scan++ = pal[*src][2];
			*scan++ = pal[*src][1];
			*scan++ = pal[*src][0];
			src++;
		}
		p.OffsetY(data,1);
	}

	// render surface changes processed
	CommitRenderSurfModified();

	return(0);
}



AStarNode::AStarNode()
{
	g_cost = INIT_COST;
	h_cost = INIT_COST;
	f_cost = INIT_COST;
	closed = false;
	pos ={0,0};
	parent_pos ={-1,-1};
}


// call before unit range finder is used when map is loaded
int SpellMap::InitUnitRangeStuff()
{
	// make default nodes
	AStarNode node_init;
	unit_range_nodes_buffer.assign(x_size*y_size,node_init);

	// fill with tile addresses
	auto* p_node = &unit_range_nodes_buffer.at(0);
	for(int y = 0; y < y_size; y++)
		for(int x = 0; x < x_size; x++)
		{
			p_node->pos = MapXY(x,y);
			p_node++;
		}	

	// init unit range map
	unit_ap_left.assign(x_size*y_size,-1);
	unit_fire_left.assign(x_size*y_size,-1);

	return(0);
}

// find walkable range of unit
int SpellMap::FindUnitRange(MapUnit* unit)
{	
	// processed tiles flags
	vector<int> done(x_size*y_size,false);

	// recoursive tiles search buffer
	vector<int> dirz;
	vector<MapXY> posz;
	dirz.reserve(500);
	posz.reserve(500);

	// initial tile
	dirz.push_back(-1);
	posz.push_back(unit->coor);
	done[ConvXY(unit->coor)] = true;

	// clear range filters
	filter.assign(x_size*y_size, NULL);
	unit_ap_left.assign(x_size*y_size,-1);
	unit_fire_left.assign(x_size*y_size,-1);

	int count = 0;
	vector<int> times;

	// recoursively expand move range untill no path is found
	while(true)
	{		
		dirz.back()++;
		if(dirz.back() >= 4)
		{
			// this tile is done
			dirz.pop_back();
			posz.pop_back();
			if(dirz.empty())
			{
				// all done				
				return(0);
			}			
		}

		// look for next neighbor
		MapXY neig_pos = GetNeighborTile(posz.back(), dirz.back());
		if(!neig_pos.IsSelected())
			continue;

		// skip done tiles
		if(done[ConvXY(neig_pos)])
			continue;
		done[ConvXY(neig_pos)] = true;

		count++;

		auto start = std::chrono::high_resolution_clock::now();

		// try to get path to target tile
		auto path = FindUnitPath(unit, neig_pos);

		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

		times.push_back(duration.count());
		
		if(path.empty())
			continue;

		// check AP limit
		/*if(path.back().g_cost > 50)
			continue;*/
		
		// mark tiles accessible
		int ap_left = unit->action_points - path.back().g_cost;
		int fire_count = unit->GetFireCount(ap_left);
		unit_ap_left[ConvXY(neig_pos)] = ap_left;
		unit_fire_left[ConvXY(neig_pos)] = fire_count;
		if(fire_count)
			filter[ConvXY(neig_pos)] = terrain->filter.bluepal;
		else
			filter[ConvXY(neig_pos)] = terrain->filter.dbluepal;


		// path found and all tests ok: step forward in recursion
		dirz.push_back(-1);
		posz.push_back(neig_pos);
	}

	

	return(1);
}

// try to find path for unit to target position
vector<AStarNode> SpellMap::FindUnitPath(MapUnit* unit, MapXY target)
{
	// local nodes map
	vector<AStarNode> *nodes = &unit_range_nodes;
	
	auto start = std::chrono::high_resolution_clock::now();

	// init local nodes map
	nodes->resize(x_size*y_size);
	memcpy(&nodes->at(0), &unit_range_nodes_buffer[0],x_size*y_size*sizeof(AStarNode));
			
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
			
	// path start position
	MapXY start_pos = unit->coor;
	
	// list of active nodes
	vector<AStarNode*> open_set;
	open_set.reserve(500);

	// initial tile
	open_set.push_back(&nodes->at(ConvXY(start_pos)));
	open_set.back()->f_cost = 0;
	open_set.back()->h_cost = 0;
	open_set.back()->g_cost = 0;

	// unit maximum AP
	int max_ap = unit->GetMaxAP();

	int count = 0;

	while(true)
	{
		// look for lowest cost node
		int init_node = 0;
		while(init_node < open_set.size() && !open_set[init_node])
			init_node++;
		if(init_node >= open_set.size())
			break; // done with fail

		AStarNode* current_node = open_set[init_node];
		int min_cost_id = init_node;
		for(int k = init_node+1; k < open_set.size(); k++)
		{
			if(!open_set[k])
				continue;
			if(open_set[k]->f_cost < current_node->f_cost ||
				open_set[k]->g_cost == current_node->g_cost && open_set[k]->h_cost < current_node->h_cost)
			{
				current_node = open_set[k];
				min_cost_id = k;
			}
			count++;
		}	
		// remove it from open nodes
		open_set.erase(open_set.begin() + min_cost_id);
		//open_set[min_cost_id] = NULL;
		// close it
		current_node->closed = true;

		if(current_node->pos == target)
		{
			// found path: retrace
			vector<AStarNode> path;
			
			//path.push_back(*current_node);

			path.reserve(30);
			while(true)
			{
				path.push_back(*current_node);
				if(!current_node->parent_pos.IsSelected())
					break;
				current_node = &nodes->at(ConvXY(current_node->parent_pos));
			};
			reverse(path.begin(),path.end());

			return(path);
		}

		// current sprite
		Sprite *current_spr = L1[ConvXY(current_node->pos)];
		int current_slope = current_spr->GetSlope();

		// for each neighbor:
		for(int nid = 0; nid < 8; nid++)
		{
			MapXY n_pos = GetTileNeihgborXY(current_node->pos, nid);
			if(!n_pos.IsSelected())
				continue;
			AStarNode *neighbor_node = &nodes->at(ConvXY(n_pos));

			// skip if closed
			if(neighbor_node->closed)
				continue;

			// basic AP/step
			double ap_step_w = (double)unit->GetWalkAP();
			// finite AP/step
			double ap_step = ap_step_w;

			// tile flags
			int flag = flags[ConvXY(n_pos)];

			// get L1 sprite
			Sprite* spr = L1[ConvXY(n_pos)];
			int class_flags = spr->GetFlags();
			int slope = spr->GetSlope();
			
			
			if(!unit->unit->isAir())
			{
				if(flag == 0x60 || flag == 0x40  || flag == 0x30 || flag == 0x20 || flag == 0x10)
					continue;
				
				if(unit->unit->isLight() && flag == 0x90)
					ap_step = (ap_step_w*10.0/9.0);
				else if(flag == 0x90)
					continue;
				else if(slope != 'A' && current_slope != 'A')
					ap_step = (ap_step_w*11.0/9.0);

				// modify step cost for various terrains	
				double cost = 1.0;
				if(class_flags & Sprite::IS_DIRT_ROAD)
					cost = 0.9;
				else if(class_flags & Sprite::IS_ROAD)
					cost = 0.75;
				ap_step *= cost;
			}						
												
			// potential neighbor costs
			int next_g = current_node->g_cost + (int)round(ap_step*current_node->pos.Distance(n_pos));
			int next_h = ap_step_w*neighbor_node->pos.Distance(target);
			int next_f = next_g + next_h;

			// leave if exceeded AP count
			if(next_g > max_ap)
				continue;

			if(neighbor_node->f_cost == AStarNode::INIT_COST || neighbor_node->f_cost > next_f)
			{
				// new node or better node: replace
				neighbor_node->g_cost = next_g;
				neighbor_node->h_cost = next_h;
				neighbor_node->f_cost = next_f;
				neighbor_node->parent_pos = current_node->pos;
				open_set.push_back(neighbor_node);
			}
		}		
	}

	// fail: return empty path
	vector<AStarNode> path;
	return(path);
}


// select unit, if currently selected at the same position, switch between air-land
MapUnit* SpellMap::SelectUnit(MapUnit* new_unit)
{
	if(unit_selection && unit_selection == new_unit)
	{
		// selection of already selected unit: detect if there is air-land pair and swap selection
		for(auto & unit : units)
		{
			if(unit != new_unit && unit->coor == new_unit->coor)
			{
				// found: swap air-land selection
				unit_selection = unit;
			}
		}
	}
	else if(new_unit)
		unit_selection = new_unit;
	// return new selection
	return(unit_selection);
}

// get unit at cursor position
MapUnit *SpellMap::GetCursorUnit(wxBitmap& bmp,TScroll* scroll)
{
	// find selection tiles
	auto msel = GetSelections(bmp,scroll);

	// no selection?
	if(!msel.size() || !msel[0].IsSelected())
		return(NULL);
	auto & pos = msel[0];

	for(auto * unit : units)
	{
		if(unit->coor == pos)
			return(unit); // found match
	}
	return(NULL);
}

// get currently selected unit
MapUnit* SpellMap::GetSelectedUnit()
{	
	return(unit_selection);
}


// set mission HUD enable state
int SpellMap::SetHUDstate(int state)
{
	int old_state = hud_enabled;
	hud_enabled = state;
	return(old_state);
}
// get mission HUD state
int SpellMap::GetHUDstate()
{	
	return(hud_enabled);
}

// render game mode HUD
int SpellMap::RenderHUD(uint8_t *buf,uint8_t* buf_end,int buf_x_size,MapXY *cursor, MapUnit *cursor_unit,std::function<void(void)> hud_buttons_cb)
{
	static int last_hud_state = -1;
	static MapUnit* last_cursor_unit = NULL;
	static MapUnit* last_selected_unit = NULL;

	if(!hud_enabled)
	{
		// is disabled: just clear HUD buttons		
		ClearHUDbuttons();
		if(hud_buttons_cb)
			hud_buttons_cb();
		last_hud_state = hud_enabled;
		return(0);
	}
			
	auto& gres = spelldata->gres;
	auto& font = spelldata->font;
	SpellGraphicItem *img;
	int x_ofs, y_ofs;

	// right panel x-offset (with exception of unit icon and experience indicators)
	const int x_rpan = 336;
	// right panel x-offset for icon and experience
	const int x_rpan_icon = 579;

	// render center panel
	img = gres.wm_hud;
	x_ofs = surf_x_origin + (surf_x - img->x_size)/2;
	y_ofs = surf_y_origin + surf_y - img->y_size;
	img->Render(buf,buf_end,buf_x_size,x_ofs,y_ofs);
	int hud_left = x_ofs;
	int hud_right = x_ofs + img->x_size;
	int hud_top = y_ofs;

	// render side fillings
	img = gres.wm_hud_sides;
	y_ofs = surf_y_origin + surf_y - img->y_size;
	x_ofs = hud_right;
	for(; x_ofs < surf_x_origin+surf_x; x_ofs += img->x_size)
		img->Render(buf,buf_end,buf_x_size,x_ofs,y_ofs);
	if(x_ofs < surf_x)
		img->Render(buf,buf_end,buf_x_size,x_ofs,y_ofs);
	x_ofs = hud_left-img->x_size;
	for(; x_ofs >= surf_x_origin; x_ofs -= img->x_size)
		img->Render(buf,buf_end,buf_x_size,x_ofs,y_ofs);
	img->Render(buf,buf_end,buf_x_size,x_ofs,y_ofs);

	// for left and right panels
	for(int pid = 0; pid < 2; pid++)
	{	
		MapUnit *unit;
		int px_ref = 0;
		int ix_ref = 0;
		if(pid == 0 && unit_selection)
			unit = unit_selection;
		else if(pid == 1 && cursor_unit)
		{
			unit = cursor_unit;
			px_ref = x_rpan;
			ix_ref = x_rpan_icon;
		}
		else if(pid == 1)
		{
			// no cursor unit: overlay right panel (when buttons active)
			img = gres.wm_hud_overlay;
			img->Render(buf,buf_end,buf_x_size,hud_left+409,hud_top+26);
			continue;
		}
		else
			continue;
		
		// render health bar
		//pos A: 118,54  size: 112,7
		//pos B: 454,54  size: 112,7
		int men_max = unit->unit->cnt;
		int men_active = unit->man;
		int men_wound = 0;
		int health_pix = men_active*112/men_max;
		int wound_pix = (men_wound + men_active)*112/men_max;
		for(int y = 0; y < 7; y++)
		{
			memset(&buf[hud_left+px_ref+118 + (hud_top+y+54)*buf_x_size], 216, wound_pix);
			memset(&buf[hud_left+px_ref+118 + (hud_top+y+54)*buf_x_size], 235, health_pix);
		}
		// pos a: 133,61
		font->Render(buf,buf_end,buf_x_size,hud_left+px_ref+133,hud_top+61,string_format("%02d",men_max),216,254,SpellFont::RIGHT_DOWN);
		// pos a: 168,61
		font->Render(buf,buf_end,buf_x_size,hud_left+px_ref+168,hud_top+61,string_format("%02d",men_active),235,254,SpellFont::RIGHT_DOWN);
		// pos a: 204,61
		font->Render(buf,buf_end,buf_x_size,hud_left+px_ref+204,hud_top+61,string_format("%02d",men_wound),216,254,SpellFont::RIGHT_DOWN);

		// pos a: 109,83
		int attack_light = unit->unit->alig;
		font->Render(buf,buf_end,buf_x_size,hud_left+px_ref+109,hud_top+83,string_format("%02d",attack_light),232,254,SpellFont::RIGHT_DOWN);
		// pos a: 145,83
		int attack_heavy = unit->unit->aarm;
		font->Render(buf,buf_end,buf_x_size,hud_left+px_ref+145,hud_top+83,string_format("%02d",attack_heavy),232,254,SpellFont::RIGHT_DOWN);
		// pos a: 181,83
		int attack_air = unit->unit->aair;
		font->Render(buf,buf_end,buf_x_size,hud_left+px_ref+181,hud_top+83,string_format("%02d",attack_air),232,254,SpellFont::RIGHT_DOWN);
		// pos a: 216,83
		int defense = unit->unit->def;
		font->Render(buf,buf_end,buf_x_size,hud_left+px_ref+216,hud_top+83,string_format("%02d",defense),232,254,SpellFont::RIGHT_DOWN);
				

		// render dig level
		const struct { int x; int y; } dig_level[6] ={{97,79},{102,77},{106,73},{108,68},{109,63},{109,58}};
		int dig_count = unit->dig_level;
		for(int k = 0; k < dig_count; k++)
		{
			auto pos = &dig_level[k];
			gres.yellow_led_on->Render(buf,buf_end,buf_x_size,hud_left+px_ref+pos->x,hud_top+pos->y);
		}

		// render unit experience
		// pos A: 5,77  size: 51,2
		int skill_fraction_pix = rand()%52;
		for(int y = 0; y < 2; y++)
			memset(&buf[hud_left+ix_ref+5 + (hud_top+y+77)*buf_x_size],235,skill_fraction_pix);
		
		const struct { int x; int y; } exp_mark[2][12] = {{	{1,81},{11,81},{21,81},{31,81},{41,81},{51,81},
															{1,90},{11,90},{21,90},{31,90},{41,90},{51,90}},
														 {	{580,81},{590,81},{600,81},{610,81},{620,81},{630,81},
															{580,90},{590,90},{600,90},{610,90},{620,90},{630,90}}};
		int skill_level = rand()%13;
		for(int k = 0; k < skill_level; k++)
			gres.wm_exp_mark->Render(buf,buf_end,buf_x_size,hud_left+exp_mark[pid][k].x,hud_top+exp_mark[pid][k].y);

		// render morale
		// pos A: 73,82  size: 6,48
		// pos B: 402,82  size: 6,48
		int morale = unit->morale;
		int morale_pix = morale*48/100;
		for(int y = 0; y < morale_pix; y++)
			memset(&buf[hud_left+px_ref+73 + (hud_top+82-y)*buf_x_size],199,6);
		// pos a: 75,70
		font->Render(buf,buf_end,buf_x_size,hud_left+px_ref+75,hud_top+70,string_format("%02d",morale),252,254,SpellFont::RIGHT_DOWN);

		// render action points
		// pos A: 81,70  size: 6,44
		// pos B: 417,70  size: 6,44
		int ap_max = unit->GetMaxAP();
		int ap = unit->action_points;
		int ap_pix = ap*44/ap_max;
		for(int y = 0; y < ap_pix; y++)
			memset(&buf[hud_left+px_ref+81 + (hud_top+70-y)*buf_x_size],228,6);
		// pos a: 83,56
		font->Render(buf,buf_end,buf_x_size,hud_left+px_ref+83,hud_top+56,string_format("%02d",ap),230,254,SpellFont::RIGHT_DOWN);

		// fire count bar
		const struct { int x; int y; } fire_pos[6] ={{88,52},{88,47},{88,42},{88,37},{88,32},{88,27}};
		int ap_left = 0;
		if(cursor->IsSelected() && unit_ap_left[ConvXY(cursor)] >= 0)
			ap_left = unit_ap_left[ConvXY(cursor)];
		int fire_count = unit->GetFireCount(ap_left);
		int max_fire_count = unit->GetMaxFireCount();
		for(int k = 0; k < max_fire_count; k++)
		{
			auto pos = &fire_pos[k];
			if(k <= fire_count)
				gres.red_led_on->Render(buf,buf_end,buf_x_size,hud_left+px_ref+pos->x,hud_top+pos->y);
			else
				gres.red_led_off->Render(buf,buf_end,buf_x_size,hud_left+px_ref+pos->x,hud_top+pos->y);
		}

		// render state text
		// pos: 244,77  size: 150,16
		if((pid == 0 && !cursor_unit) || (pid == 1 && unit_selection))
			font->Render(buf,buf_end,buf_x_size,hud_left+244,hud_top+77,150,16,string_format("ab: %d / attack: %d",ap_left,fire_count),232,254,SpellFont::DIAG2);

		// render unit icon
		// pos a: 1,23  size: 60x50
		// pos b: 579,23  size: 60x50
		unit->unit->icon_glyph->Render(buf,buf_end,buf_x_size,hud_left+((pid==0)?(1):(579)),hud_top+23);

		// render command level mark
		// pos a: 48,25
		// pos b: 626,25
		int command_level = rand()%4;
		if(command_level)
			gres.wm_form[command_level-1]->Render(buf,buf_end,buf_x_size,hud_left+ix_ref+48,hud_top+25);

		// render freeze mark
		// pos a: 13,5
		// pos b: 593,5
		int freeze_mark = rand()%2;
		if(freeze_mark)
			gres.wm_freeze->Render(buf,buf_end,buf_x_size,hud_left+ix_ref+(pid==1)+13,hud_top+5);
		
		// render paralysed mark
		// pos a: 36,5
		// pos b: 616,5
		int paralyze_mark = rand()%2;
		if(paralyze_mark)
			gres.wm_paralyze->Render(buf,buf_end,buf_x_size,hud_left+ix_ref+(pid==1)+36,hud_top+5);
	
		// render unit name
		// pos a: 95,25   size: 137,16
		string name = unit->name;		
		font->Render(buf,buf_end,buf_x_size,hud_left+px_ref+95,hud_top+28,137,16,name,232,254,SpellFont::DIAG2);

	}	

	// modify HUD buttons?
	int state_chaned = isRenderSurfModified() || cursor_unit != last_cursor_unit || unit_selection != last_selected_unit || hud_enabled != last_hud_state;

	if(state_chaned && hud_buttons_cb)
	{		
		t_xypos btn_top[4] = {{246,26},{246+36,26},{246+36*2,26},{246+36*3,26}};
		t_xypos btn_right[8] = {{413,29},{449,29},{485,29},{534,29}, {413,63},{449,63},{485,63},{534,63}};
		t_xypos hud_origin = {hud_left, hud_top};
				
		ClearHUDbuttons();						
		CreateHUDbutton(gres.wm_glyph_next,hud_origin, btn_top[0], buf, buf_end, buf_x_size, 0,bind(&SpellMap::OnHUDnextUnit,this), NULL);
		CreateHUDbutton(gres.wm_glyph_next_unused,hud_origin,btn_top[1],buf,buf_end,buf_x_size, 0,bind(&SpellMap::OnHUDnextUnfinishedUnit,this),NULL);
		
		if(!cursor_unit)
		{
			CreateHUDbutton(gres.wm_glyph_map,hud_origin,btn_right[0],buf,buf_end,buf_x_size,HUD_ACTION_MINIMAP,NULL,NULL);
			CreateHUDbutton((unit_sel_land_preference)?(gres.wm_glyph_ground):(gres.wm_glyph_air),hud_origin,btn_right[1],buf,buf_end,buf_x_size,0,bind(&SpellMap::OnHUDswitchAirLand,this),NULL);
			CreateHUDbutton(gres.wm_glyph_goto_unit,hud_origin,btn_right[2],buf,buf_end,buf_x_size,0,NULL,NULL);
			CreateHUDbutton(gres.wm_glyph_end_turn,hud_origin,btn_right[3],buf,buf_end,buf_x_size,0,NULL,NULL);
			
			CreateHUDbutton(gres.wm_glyph_unit_info,hud_origin,btn_right[4],buf,buf_end,buf_x_size,0,bind(&SpellMap::OnHUDswitchUnitHUD,this),NULL);
			CreateHUDbutton(gres.wm_glyph_info,hud_origin,btn_right[5],buf,buf_end,buf_x_size,0,NULL,NULL);
			CreateHUDbutton(gres.wm_glyph_options,hud_origin,btn_right[6],buf,buf_end,buf_x_size,0,NULL,NULL);
			CreateHUDbutton(gres.wm_glyph_retreat,hud_origin,btn_right[7],buf,buf_end,buf_x_size,0,NULL,NULL);
		}
		
		hud_buttons_cb();
	}

	// store last units for change detection
	last_hud_state = hud_enabled;
	last_cursor_unit = cursor_unit;
	last_selected_unit = unit_selection;

	return(0);
}

// HUD button event handlers
void SpellMap::OnHUDnextUnit()
{
	for(int uid = 0; uid < units.size(); uid++)
	{
		if(units[uid] == unit_selection)
		{
			// current unit found in the list: go to next
			uid++;
			if(uid >= units.size())
				uid = 0;
			unit_selection = units[uid];
		}
	}
}
void SpellMap::OnHUDnextUnfinishedUnit()
{

}
void SpellMap::OnHUDswitchAirLand()
{
	unit_sel_land_preference = !unit_sel_land_preference;
	InvalidateHUDbuttons();
}
void SpellMap::OnHUDswitchUnitHUD()
{
	w_unit_hud = !w_unit_hud;	
}




// clear all HUD buttons
void SpellMap::ClearHUDbuttons()
{
	for(auto & btn : hud_buttons)
		delete btn;
	hud_buttons.clear();
}
// get list of HUD button
vector<SpellBtnHUD*> *SpellMap::GetHUDbuttons()
{
	return(&hud_buttons);
}
// get HUD button by id (assigned by GUI)
SpellBtnHUD* SpellMap::GetHUDbutton(int id)
{
	for(auto& btn : hud_buttons)
		if(btn->wx_id == id)
			return(btn);
	return(NULL);
}
// sets flag for refresh of HUD button in next render
void SpellMap::InvalidateHUDbuttons()
{
	surf_modified = true;
}
// create new HUD button
SpellBtnHUD* SpellMap::CreateHUDbutton(SpellGraphicItem* glyph,t_xypos &hud_pos,t_xypos &pos,uint8_t* buf,uint8_t* buf_end,int buf_x_size,
	int action_id,std::function<void(void)> cb_press,std::function<void(void)> cb_hover)
{
	int x_pos = hud_pos.x + pos.x;
	int y_pos = hud_pos.y + pos.y;

	// button base graphics
	SpellGraphicItem *btn_gr_states[3] = {spelldata->gres.wm_btn_idle, spelldata->gres.wm_btn_hover, spelldata->gres.wm_btn_press};	

	// for each button state:
	int x_size;
	int y_size;
	uint8_t* surf[3];
	for(int sid = 0; sid < 3; sid++)
	{
		auto *grb = btn_gr_states[sid];
		x_size = grb->x_size + grb->x_ofs;
		y_size = grb->y_size + grb->y_ofs;
		
		// make button background from current surface (wxWidgets cannot render semitransparents)
		surf[sid] = new uint8_t[x_size*y_size];
		for(int y = 0; y < y_size; y++)
			memcpy(&surf[sid][y*x_size], &buf[x_pos + (y_pos + y)*buf_x_size], x_size);

		// render button base
		grb->Render(surf[sid], &surf[sid][x_size*y_size], x_size, 0, 0);

		// render overlay button glyph
		if(glyph)
		{
			glyph->Render(surf[sid],&surf[sid][x_size*y_size],x_size,0, (sid==2)*2);
		}
	}
	
	// create button
	SpellBtnHUD* btn = new SpellBtnHUD(x_pos - surf_x_origin, y_pos - surf_y_origin, x_size, y_size, (uint8_t*)terrain->pal, surf[0],surf[1],surf[2], action_id, cb_press, cb_hover);
	hud_buttons.push_back(btn);

	// loose buffers
	for(int sid = 0; sid < 3; sid++)
		delete[] surf[sid];

	return(btn);
}





// creates height map for unit view calculation, call this whenever map static content changes (destroyed walls)
int SpellMap::PrepareUnitsViewMask()
{
	// allocate mask
	const int tile_size = units_view_mask_size;
	units_view_mask_x_size = (x_size)*tile_size + tile_size/2 + 2;
	units_view_mask_y_size = (y_size + 1) * (tile_size/2) + 2;
	if(!units_view_mask.size())
	{
		units_view_map.assign(units_view_mask_x_size*units_view_mask_y_size,0);
		units_view_mask.assign(units_view_mask_x_size*units_view_mask_y_size,0);
		units_view_mask_0.assign(units_view_mask_x_size*units_view_mask_y_size,0);
	}
	
	// render mask
	for(int y = 0; y < y_size; y++)
	{
		for(int x = 0; x < x_size; x++)
		{
			int mxy = ConvXY(x,y);	

			// tile origin (local coordinate system)
			int mxx = 1 + x*tile_size + (((y & 1) != 0) ? 0 : tile_size/2);
			int myy = 1 + y*(tile_size/2);

			// this tile L1 sprite
			Sprite *spr = L1[mxy];
			int flag = flags[mxy];

			// sprite origin (sprite coordinate system)
			double sxx = -40.0;
			double syy = -40.0;
			double ssx = 80.0/(tile_size-2);
			double ssy = 80.0/(tile_size-1);
			
			for(int n = 0; n < tile_size; n++)
			{
				for(int m = 0; m < tile_size-1; m++)
				{
					if((n < tile_size/2 && m >= (tile_size/2-1 - n) && m <= (tile_size/2-1 + n)) || (n >= tile_size/2 && m >= (n-tile_size/2) && m <= (tile_size-1+tile_size/2-1 - n)))
					{
						// get elevation of tile at this relative position
						double h = spr->GetTileZ(sxx + ssx*m, -(syy + ssy*n));
						// add terrain elevation
						h += elev[mxy]*Sprite::TILE_ELEVATION;

						// store basic elevation (without objects)
						units_view_mask_0[mxx+m + (myy+n)*units_view_mask_x_size] = (unsigned)max(h,0.0);
						units_view_map[mxx+m + (myy+n)*units_view_mask_x_size] = mxy;

						// include L2 objects to mask
						if(flag == 0x90)
						{
							// tree and stuff: elevate center part of tile
							const int rfrm = 1;
							if((n >= rfrm+1 && n < tile_size/2 && m >= tile_size/2-1+rfrm-n && m <= tile_size/2-1-rfrm+n) ||
								(n >= tile_size/2 && n <= tile_size-rfrm-2 && m >= n-tile_size/2+rfrm-0 && m <= tile_size-1+tile_size/2-1-n-(rfrm-0)))
								h += 20.0;
						}
						else if(flag == 0x10 || flag == 0x20 || flag == 0x30)
						{
							// houses: elevete by hopefully height of object? this is unlear, but makes sense flag code is actual sprite z-size
							h += flag;
						}
						else if(flag == 0xA0 || flag == 0xB0 || flag == 0xC0 || flag == 0xD0 || flag == 0xE0 || flag == 0xF0)
						{
							// walls: full blocking
							if(n > 0 && n < tile_size-1 && m > 0 && m < tile_size-2)
								h += Sprite::TILE_ELEVATION;
						}						
						units_view_mask[mxx+m + (myy+n)*units_view_mask_x_size] = (unsigned)max(h,0.0);						
					}
				}
			}
		}
	}

	return(1);
}

// render units view elevation map (mask) to bitmap
// do not forget destroy returned object!
wxBitmap *SpellMap::ExportUnitsViewZmap()
{
	// make raster for entire map
	wxBitmap *bmp = new wxBitmap(wxSize(units_view_mask_x_size, units_view_mask_y_size), 24);

	// get max Z
	int max_h = 0;
	for(auto & tile_h : units_view_mask)
		max_h = max(max_h, tile_h);
	int gain = 65535/max_h;
	
	// render 24bit RGB data to raw bmp buffer
	uint8_t* src = &units_view_mask[0];
	wxNativePixelData data(*bmp);
	wxNativePixelData::Iterator p(data);
	for(int y = 0; y < units_view_mask_y_size; y++)
	{
		uint8_t* scan = p.m_ptr;
		for(int x = 0; x < units_view_mask_x_size; x++)
		{
			uint8_t pix = (((uint32_t)*src*gain)>>8);
			*scan++ = pix;
			*scan++ = pix;
			*scan++ = pix;
			src++;
		}
		p.OffsetY(data,1);
	}
	return(bmp);
}

// get unit view map and mask <map tile ID, height>, where <x,y> are units_view...[] related coordinates (not equal to main render coordinates!)
tuple<int,int> SpellMap::GetUnitsViewMask(int x, int y,int plain_tile_id)
{
	int tile_id = units_view_map[x + y*units_view_mask_x_size];
	if(plain_tile_id == tile_id)
		return tuple(tile_id, units_view_mask_0[x + y*units_view_mask_x_size]);
	else
		return tuple(tile_id, units_view_mask[x + y*units_view_mask_x_size]);

}

// get unit view map/mask center coordinates <x,y,z> of tile (these are related to local units_vies... coordinates), the mxy or <x,y> are tile coordinates
tuple<int,int,int> SpellMap::GetUnitsViewTileCenter(MapXY mxy)
{
	return GetUnitsViewTileCenter(mxy.x, mxy.y);
}
tuple<int,int,int> SpellMap::GetUnitsViewTileCenter(int x,int y)
{
	// tile origin
	int mxx = 1 + x*units_view_mask_size + (((y & 1) != 0) ? 0 : units_view_mask_size/2) + units_view_mask_size/2;
	int myy = 1 + y*(units_view_mask_size/2) + units_view_mask_size/2;
	return tuple(mxx,myy,units_view_mask_0[mxx + myy*units_view_mask_x_size]);
}

// get unit view map/mask pixel coordinates <x,y,z> of tile (these are related to local units_vies... coordinates), the mxy or <x,y> are tile coordinates
vector<Txyz> SpellMap::GetUnitsViewTilePixels(MapXY mxy,int all)
{
	return(GetUnitsViewTilePixels(mxy.x,mxy.y,all));
}
vector<Txyz> SpellMap::GetUnitsViewTilePixels(int x,int y,int all)
{
	const int tile_size = units_view_mask_size;
	// tile origin
	int mxx = 1 + x*tile_size + (((y & 1) != 0) ? 0 : tile_size/2);
	int myy = 1 + y*(tile_size/2);
	
	// make list of all belonging pixels
	vector<Txyz> list;
	if(all)
	{
		list.reserve(tile_size*tile_size);
		for(int n = 0; n < tile_size; n++)
		{
			for(int m = 0; m < tile_size-1; m++)
			{
				if((n < tile_size/2 && m >= (tile_size/2-1 - n) && m <= (tile_size/2-1 + n)) || (n >= tile_size/2 && m >= (n-tile_size/2) && m <= (tile_size-1+tile_size/2-1 - n)))
				{
					list.emplace_back(mxx+m,myy+n,(int)(unsigned)units_view_mask_0[mxx+m + (myy+n)*units_view_mask_x_size]);
				}
			}
		}
	}
	else
	{
		// only selection of points (center + corners)
		list.reserve(5);
		list.emplace_back(mxx+tile_size/2,myy+tile_size/2,(int)(unsigned)units_view_mask_0[mxx+tile_size/2 + (myy+tile_size/2)*units_view_mask_x_size]);
		list.emplace_back(mxx+tile_size/2,myy+0,(int)(unsigned)units_view_mask_0[mxx+tile_size/2 + (myy+0)*units_view_mask_x_size]);
		list.emplace_back(mxx+tile_size/2,myy+tile_size-1,(int)(unsigned)units_view_mask_0[mxx+tile_size/2 + (myy+tile_size-1)*units_view_mask_x_size]);
		list.emplace_back(mxx+0,myy+tile_size/2,(int)(unsigned)units_view_mask_0[mxx+0 + (myy+tile_size/2)*units_view_mask_x_size]);
		list.emplace_back(mxx+tile_size-2,myy+tile_size/2,(int)(unsigned)units_view_mask_0[mxx+tile_size-2 + (myy+tile_size/2)*units_view_mask_x_size]);
	}
	return(list);
}




// reset units view state to desired state
int SpellMap::ClearUnitsView(int to_unseen)
{
	if(to_unseen)
	{
		// all to never seen state
		units_view.assign(x_size*y_size, 0);
	}
	else
	{
		// all seen to currently invisible
		if(units_view.size() != x_size*y_size)
			units_view.resize(x_size*y_size,0);
		else
			for(auto & pos : units_view)
				if(pos == 2)
					pos = 1;
	}		

	return(0);
}

// calculate unit view range
int SpellMap::AddUnitView(MapUnit *unit)
{
	if(!unit)
		return(1);

	// get unit view range in tiles
	int ref_view = unit->unit->sdir;

	// reference position
	MapXY ref_pos = unit->coor;
	int ref_mxy = ConvXY(ref_pos);
	int ref_alt = elev[ConvXY(ref_pos)];

	// recursion buffer
	vector<int> dirz;
	vector<MapXY> posz;
	dirz.reserve(400);
	posz.reserve(400);

	// first tile
	if(!dirz.size())
	{		
		dirz.push_back(0);
		posz.push_back(ref_pos);
	}
	
	// mark my position as visible
	units_view[ConvXY(ref_pos)] = 2;

	// proceed recoursively till max visibility
	int count = 0;
	while(true)
	{
		// go to next direction
		dirz.back()++;
		if(dirz.back() > 4)
		{
			// this recursion level is done: revert back
			dirz.erase(dirz.end()-1);
			posz.erase(posz.end()-1);
			if(!dirz.size())
				break; // all done
			continue;
		}
		MapXY this_pos = posz.back();

		// try look for next position
		MapXY next_pos = GetNeighborTile(this_pos, dirz.back()-1);
		if(!next_pos.IsSelected())
			continue;
		int next_mxy = ConvXY(next_pos);

		// skip if already done
		if(units_view[next_mxy] > 2)
			continue;
		
		// next tile elevation
		int next_alt = elev[next_mxy];
		int view = ref_view + max(ref_alt-next_alt,0); // expand if terget lower than ref

		// visible?
		if((next_pos.Distance(ref_pos)-0.5) > view)
			continue; // nope: goto next tile
		// mark this tile as potentially visible (to stop recursion)
		units_view[next_mxy] += 3;				
		
		// -- send ray to target tile to find out if it is visible:				
		if(units_view[next_mxy] < 5) // only if was not visible before
		{
			// multisampling mode
			const int msx_ofs[4] ={1,-1,0,0};
			const int msy_ofs[4] ={0,0,1,-1};
			const int ms_count = 4;
		
			// get list of all points belonging target tile (in view mask coordinate system)
			auto target_pix = GetUnitsViewTilePixels(next_pos);
			// for each target point: send ray and check if the tartget point is visible, if at least one ray is, mark tile as visible
			for(auto & target_pos : target_pix)
			{
				// get view ray initial coordinates (unit)
				auto [x0,y0,z0b] = GetUnitsViewTileCenter(ref_pos);				
				int z0 = (int)z0b;
				if(unit->unit->isAir())
					z0 += 100; // air-unit height estimate
				else
					z0 += 15; // unit height estimate

				count++;

				// get target coordinates
				int x1 = target_pos.x;
				int y1 = target_pos.y;
				int z1 = target_pos.z;
				z1 += 4; // target height estimate
		
				// based on: http://members.chello.at/easyfilter/bresenham.html
				int dx = abs(x1-x0),sx = x0<x1 ? 1 : -1;
				int dy = abs(y1-y0),sy = y0<y1 ? 1 : -1;
				int dz = abs(z1-z0),sz = z0<z1 ? 1 : -1;
				int dm = max(max(dx,dy),dz),i = dm; // maximum difference
				x1 = y1 = z1 = dm/2; // error offset

				int last_tile_id = -1;
				for(;;) {  // loop
				
					auto [tile_id,hx] = GetUnitsViewMask(x0,y0,ref_mxy);
					// multisampling test (tries to send ray with +-1pixel offset)
					int pass = (ms_count == 0);
					for(int k = 0; k < ms_count; k++)
					{
						auto [ms_tile_id,ms_hx] = GetUnitsViewMask(x0 + msx_ofs[k],y0 + msy_ofs[k],ref_mxy);
						if(z0 >= ms_hx/* && ms_tile_id == tile_id*/)
						{
							pass = true;
							break;
						}
					}
					if(tile_id == next_mxy)
					{
						// target reached: mark tile as visible and done
						units_view[next_mxy] = 5;
						break;
					}
					// at least one pixel of crossed tile must be visible
					if(!pass)
						break;
			
					last_tile_id = tile_id;

					if(i-- == 0) break;
					x1 -= dx; if(x1 < 0) { x1 += dm; x0 += sx; }
					y1 -= dy; if(y1 < 0) { y1 += dm; y0 += sy; }
					z1 -= dz; if(z1 < 0) { z1 += dm; z0 += sz; }
				}
				// done if ray passed to target
				if(units_view[next_mxy] == 5)
					break;
			}
		}

		// -- expand viewto entire objects (houses)
		vector<int> hdir;
		vector<MapXY> hpos;
		hdir.push_back(0);
		hpos.push_back(next_pos);
		int mxy = ConvXY(next_pos);
		if(flags[mxy] == 0x10 || flags[mxy] == 0x20 || flags[mxy] == 0x30)
		{
			while(1)
			{			
				// go to next direction
				hdir.back()++;
				if(hdir.back() > 4)
				{
					// this recursion level is done: revert back
					hdir.erase(hdir.end()-1);
					hpos.erase(hpos.end()-1);
					if(!hdir.size())
						break; // all done
					continue;
				}
				MapXY hthis_pos = hpos.back();

				// try look for next position
				MapXY hnext_pos = GetNeighborTile(hthis_pos,hdir.back()-1);
				if(!hnext_pos.IsSelected())
					continue;
				int hnext_mxy = ConvXY(hnext_pos);

				// skip if already done
				if(units_view[hnext_mxy] > 2)
					continue;

				// skip if not house
				if(flags[hnext_mxy] != 0x10 && flags[hnext_mxy] != 0x20 && flags[hnext_mxy] != 0x30)
					continue;

				// mark as seen
				units_view[hnext_mxy] = 5;

				// proceed to next position
				hdir.push_back(0);
				hpos.push_back(hnext_pos);
			}
		}


		// proceed to next position allowed
		dirz.push_back(0);
		posz.push_back(next_pos);		
	}

	// clear potentially visible tiles (temps)
	for(auto & tile : units_view)
		if(tile > 2)
			tile -= 3;

	return(count);
}

// add view range of all units of given type
int SpellMap::AddUnitsView(int unit_type)
{
	for(auto & unit : units)
	{
		// filter unit types to view
		if(unit->is_enemy && !(unit_type & UNIT_TYPE_OS) || !unit->is_enemy && !(unit_type & UNIT_TYPE_ALIANCE))
			continue;
		
		// add view mask
		AddUnitView(unit);
	}
	return(0);
}



// returns true when tile is in the normally visible area of map, false for the dark map bevel
bool SpellMap::TileIsVisible(int x, int y)
{
	int is_even = !!(y&1);
	int x_ok = (x >= (1 + is_even)) && (x < x_size-3);	
	return(x_ok && y >= 8 && y < (y_size-14));
}

// configure map elements visibility
void SpellMap::SetRender(bool wL1, bool wL2, bool wL3, bool wL4, bool wSTCI, bool wUnits)
{
	this->wL1 = wL1;
	this->wL2 = wL2;
	this->wL3 = wL3;
	this->wL4 = wL4;
	this->wSTCI = wSTCI;
	this->wUnits = wUnits;
}

// set gamma correction for rendering
void SpellMap::SetGamma(double gamma)
{
	this->gamma = gamma;
	// forces recalculation of gamma table
	last_gamma = 0.0;
}

// timer tick - update of palettes and animations
int SpellMap::Tick()
{
	if (!this->IsLoaded())
		return(0);
	
	// cycle water palette
	uint8_t water[10][3];
	std::memcpy((void*)water, (void*)&pal[240][0], 10 * 3);
	std::memcpy((void*)&pal[240][0], (void*)&water[1][0], 9 * 3);
	std::memcpy((void*)&pal[249][0], (void*)&water[0][0], 1 * 3);

	// animate L3 animations
	for (int k = 0; k < L3.size(); k++)
	{
		MapLayer3 *anm = L3[k];
		// cycle through frames
		anm->frame_ofs++;
		if (anm->frame_ofs >= anm->frame_limit)
			anm->frame_ofs = 0;
	}

	// animate L4 animations
	for (int k = 0; k < L4.size(); k++)
	{
		MapLayer4* pnm = L4[k];
		// cycle through frames
		pnm->frame_ofs++;
		if (pnm->frame_ofs >= pnm->frame_limit)
			pnm->frame_ofs = 0;
	}

	// animate unit pointer
	pnm_sipka.frame_ofs++;
	if(pnm_sipka.frame_ofs >= pnm_sipka.frame_limit)
		pnm_sipka.frame_ofs = 0;

	// animate units
	for (int k = 0; k < units.size(); k++)
	{
		MapUnit* unit = units[k];

		// cycle frames when animation avilable
		FSU_resource* fsu = unit->unit->gr_base;
		if (fsu->anim.frames)
		{
			unit->frame++;
			if (unit->frame >= fsu->anim.frames)
				unit->frame = 0;
		}
	}	

	// repaint
	return(1);
}








int SpellMap::EditElevNbrQuad(int x, int y, int elv, int* min)
{
	int i,j;

	// no quad yet
	int hquad[4];

	// --- for each possible quad ---
	for(j = 0; j < 4; j++)
	{
		// total neighbours higher than center tile
		int hcnt = 0;

		for(i = 0; i < 3; i++)
		{
			// for every tile in quad
			int tile = (7 + i + j*2)%8;

			// neighbour position
			MapXY nxy = GetTileNeihgborXY(x,y,tile);

			// skip if outside map
			if(nxy.x < 0 || nxy.x >= x_size || nxy.y < 0 || nxy.y >= y_size)
				continue;

			// neighbour higher?
			int nelv = elev[ConvXY(nxy)];
			if(nelv > elv)
				hcnt++;
		}

		// store count
		hquad[j] = hcnt;
	}

	// --- decide which quad needs to be aligned ---
	// find minimum
	int qmin = 4;
	int qminid;
	for(i = 0; i < 4; i++)
	{
		if(hquad[i] < qmin)
		{
			qmin = hquad[i];
			qminid = i;
		}
	}

	// return min count if requested
	if(min)
		*min = qmin;

	// and its id
	return(qminid);
}

int SpellMap::EditElevNbrDbl(int x,int y,int elv)
{
	int j, k;
	int hdbl[4] = {0,0,0,0};

	// --- for each corner ---
	for(j = 0; j < 4; j++)
	{
		for(k = 0; k < 3; k++)
		{
			// neighbour tile id
			int tile = (7 + k + j*2)%8;

			// neighbour position
			MapXY nxy = GetTileNeihgborXY(x,y,tile);

			// done if outside map
			if(nxy.x < 0 || nxy.x >= x_size || nxy.y < 0 || nxy.y >= y_size)
				continue;

			// higher?
			int nelv = elev[ConvXY(nxy)];
			hdbl[j] |= (nelv > elv);
		}
	}

	return((hdbl[0]&hdbl[2]&!hdbl[1]&!hdbl[3]) || (hdbl[1]&hdbl[3]&!hdbl[0]&!hdbl[2]));
}

void SpellMap::EditElevNbrRule(uint8_t* flag,TTileElevMod* mod,int x,int y,int elv)
{
	int mxy = ConvXY(x,y);

	// check neigbouring quads for valid one
	int qmin;
	EditElevNbrQuad(x, y, elv, &qmin);
	// neighbor pair?
	int dbl = EditElevNbrDbl(x, y, elv);
	
	// is there some valid?
	if(qmin > 0 || dbl)
	{		 		
		// nope, increase center tile
		elv++;

		// store it
		elev[mxy] = elv;
		// tile modified flag
		flag[mxy] = 1;

		// and mark as modified
		mod->x = x;
		mod->y = y;
		mod->elev = elv;
		mod->edir =+1;
	}
}

void SpellMap::EditElevNbr(uint8_t* flag,int elv,int edir,int x,int y)
{
	int i;

	if(edir != 0)
		flag[ConvXY(x,y)] = 1;

	// no modified neighbour yet
	TTileElevMod mod[8];
	for(i = 0; i < 8; i++)
		mod[i].elev = -1;

	// === check slopes for each neighbour ===
	for(i = 0; i < 8 && edir != 0; i++)
	{
		// neighbour position
		MapXY nxy = GetTileNeihgborXY(x,y,i);

		// done if outside map
		if(nxy.x < 0 || nxy.x >= x_size || nxy.y < 0 || nxy.y >= y_size)
			continue;

		// get neighbour elev
		int nelv = elev[ConvXY(nxy)];
		int nelv_o = nelv;

		// limit diference to +/-1
		if(nelv - elv > 1)
			nelv = elv + 1;
		else if(nelv - elv < -1)
			nelv = elv - 1;

		// modified?
		if(nelv != nelv_o)
		{
			// yaha
			elev[ConvXY(nxy)] = nelv;
			flag[ConvXY(nxy)] = 1;

			// set neighbour modified flag
			mod[i].x = nxy.x;
			mod[i].y = nxy.y;
			mod[i].elev = nelv;
			mod[i].edir = nelv - nelv_o;
		}
	}


	// === check quads for center tile ===
	int qmin;
	int qminid = EditElevNbrQuad(x,y,elv,&qmin);

	// terrain lowered and quad invalid?
	if(qmin > 0 && edir < 0)
	{
		// yaha, fix quad

		// for every tile in quad
		for(i = 0; i < 3; i++)
		{
			// tile
			int tile = (7 + i + qminid*2)%8;

			// neighbour position
			MapXY nxy = GetTileNeihgborXY(x,y,tile);

			// done if outside map
			if(nxy.x < 0 || nxy.x >= x_size || nxy.y < 0 || nxy.y >= y_size)
				continue;

			// neighbour elevation
			int nelv = elev[ConvXY(nxy)];

			// align if neighbour higher
			elev[ConvXY(nxy)] = elv;
			flag[ConvXY(nxy)] = 1;

			// set neighbour modified flag
			mod[tile].x = nxy.x;
			mod[tile].y = nxy.y;
			if(nelv > elv)
				mod[tile].edir = -1;
			else
				mod[tile].edir = 0;
			mod[tile].elev = elv;
		}
	}

	// === check invalid pattern ===
	int dbl = EditElevNbrDbl(x,y,elv);
	if(dbl && edir <= 0)
	{
		for(i = 0; i < 2; i++)
		{
			int tile = 4 + i*2;

			// neighbour position
			MapXY nxy = GetTileNeihgborXY(x,y,tile);

			// done if outside map
			if(nxy.x < 0 || nxy.x >= x_size || nxy.y < 0 || nxy.y >= y_size)
				continue;

			// neighbour elevation
			int nelv = elev[ConvXY(nxy)];

			if(nelv > elv)
			{
				// align if neighbour higher
				elev[ConvXY(nxy)] = elv;
				flag[ConvXY(nxy)] = 1;
				
				// set neighbour modified flag
				mod[tile].x = nxy.x;
				mod[tile].y = nxy.y;
				mod[tile].edir = -1;
				mod[tile].elev = elv;
			}
		}
	}


	// === check quads for every neighbouring tile ===
	for(i = 0; i < 8; i++)
	{
		// neighbour position
		MapXY nxy = GetTileNeihgborXY(x,y,i);

		// done if outside map
		if(nxy.x < 0 || nxy.x >= x_size || nxy.y < 0 || nxy.y >= y_size)
			continue;

		// get neighbour elev
		int nelv = elev[ConvXY(nxy)];

		// and check/fix
		if(edir > 0)
		{
			EditElevNbrRule(flag,&mod[i],nxy.x,nxy.y,nelv);
		}
		else if(edir < 0)
		{
			int dbl = EditElevNbrDbl(nxy.x,nxy.y,nelv);
			if(dbl)
			{
				mod[i].x = nxy.x;
				mod[i].y = nxy.y;
				mod[i].elev = nelv;
				mod[i].edir = 0;
			}
		}
	}


	// === now repeat for all modified tiles ===
	for(i = 0; i < 8; i++)
	{
		if(mod[i].elev >= 0)
		{
			// modified
			EditElevNbr(flag,mod[i].elev,mod[i].edir,mod[i].x,mod[i].y);
		}
	}
}

void SpellMap::EditElevSlope(uint8_t* flag)
{
	int i,j,k,m;

	const char slopes[16] = {'A','K','C','M','E','A','D','B','H','J','A','L','G','I','F','A'};

	// === mark modified area ===
	for(j = 0; j < y_size; j++)
	{
		for(i = 0; i < x_size; i++)
		{
			// for each neighbour
			for(k = 0; k < 8 && flag[ConvXY(i,j)] == 1; k++)
			{
				// neighbour position				
				MapXY nxy = GetTileNeihgborXY(i,j,k);

				// done if outside map
				if(nxy.x < 0 || nxy.x >= x_size || nxy.y < 0 || nxy.y >= y_size)
					continue;

				// mark neigbours
				if(flag[ConvXY(nxy)] != 1)
					flag[ConvXY(nxy)] = 2;
			}
		}
	}

	// === find land slopes for each modified tile ===
	for(j = 0; j < y_size; j++)
	{
		for(i = 0; i < x_size; i++)
		{
			if(flag[ConvXY(i,j)] > 0)
			{
				// center tile alt
				int alt = elev[ConvXY(i,j)];

				// corners
				int corn = 0x00;

				// check corner alts
				for(k = 0; k < 4; k++)
				{
					for(m = 0; m < 3; m++)
					{
						// neighbour tile id
						int tile = (7 + m + k*2)%8;

						// neighbour position
						MapXY nxy = GetTileNeihgborXY(i,j,tile);

						// done if outside map
						if(nxy.x < 0 || nxy.x >= x_size || nxy.y < 0 || nxy.y >= y_size)
							continue;

						// current alt
						int nalt = elev[ConvXY(nxy)];
						if(nalt > alt)
							break;
					}

					// store corner alt
					corn |= (((m!=3)?1:0)<<k);
				}
				char slp = slopes[corn];
				
				// try to get some tile of requested slope
				char sprname[9] = "PLx00_00"; // this should exist in each terrain
				sprname[2] = slp;
				Sprite *sprite = terrain->GetSprite(sprname);
				if(!sprite)
					continue; // skip if not found, but this should not happen

				// assign new sprite to L1
				L1[ConvXY(i,j)] = sprite;
				
				// remove objects?
				if(slp != 'A')
				{
					L2[ConvXY(i,j)] = NULL;
					flags[ConvXY(i,j)] = 0;
				}
			}
		}
	}
}

// fix textures after elevation change (only basic, need to implement full map retexture)
void SpellMap::EditElevText(uint8_t* flag)
{
	int i,j,k;

	// === mark modified area ===
	for(j = 0; j < y_size; j++)
	{
		for(i = 0; i < x_size; i++)
		{
			// for each neighbour
			for(k = 0; k < 8 && flag[ConvXY(i,j)] == 2; k++)
			{
				// neighbour position
				MapXY nxy = GetTileNeihgborXY(i,j,k);

				// done if outside map
				if(nxy.x < 0 || nxy.x >= x_size || nxy.y < 0 || nxy.y >= y_size)
					continue;

				// mark neigbours
				if(flag[ConvXY(nxy)] == 0)
					flag[ConvXY(nxy)] = 3;
			}
		}
	}

	// === find land slopes for each modified tile ===
	for(j = 0; j < y_size; j++)
	{
		for(i = 0; i < x_size; i++)
		{
			if(flag[ConvXY(i,j)] > 0)
			{
				// center tile alt
				int alt = elev[ConvXY(i,j)];

				Sprite *sprite = L1[ConvXY(i,j)];
				char slp = sprite->GetSlope();

				int msk = 0;
				int mskh = 0;
				if(slp == 'A')
				{
					// --- A slope ---
					for(k = 0; k < 4; k++)
					{
						// neighbour tile
						int tile = (7+k*2)%8;
						MapXY nxy = GetTileNeihgborXY(i,j,tile);

						// center tile alt
						int nalt = elev[ConvXY(nxy)];

						// build mask
						msk |= (nalt<alt?1:0)<<k;
					}
				}
				else if(slp == 'D' || slp == 'G' || slp == 'J' || slp == 'M')
				{
					int sid = (slp-'D')/3;
					int did = (7+2*sid)%8;
					int uid = (7+2*(sid+2))%8;

					MapXY dxy = GetTileNeihgborXY(i,j,did);
					MapXY uxy = GetTileNeihgborXY(i,j,uid);
					//MapXY sxy = GetTileNeihgborXY(i,j,uid);

					if(elev[ConvXY(dxy)] == alt)
						msk |= (1<<sid);
										
					Sprite *sprite = L1[ConvXY(uxy)];
					if(sprite->GetSlope() == 'A')
						msk |= (1<<((sid+2)%4));

				}
				else if(slp == 'C' || slp == 'E' || slp == 'H' || slp == 'K')
				{
					int sid = (slp=='C')?0:((slp-'E')/3+1);
					int dna = (5+sid*2)%8;
					int dnb = (7+sid*2)%8;
					int uid = (2+sid*2)%8;

					MapXY dnaxy = GetTileNeihgborXY(i,j,dna);
					MapXY dnbxy = GetTileNeihgborXY(i,j,dnb);
					MapXY uxy = GetTileNeihgborXY(i,j,uid);
					
					if(elev[ConvXY(dnaxy)] == alt)
						msk |= (1<<((sid+3)%4));
					if(elev[ConvXY(dnbxy)] == alt)
						msk |= (1<<((sid+0)%4));

					Sprite* sprite = L1[ConvXY(uxy)];
					if(sprite->GetSlope() == 'A')
						mskh |= (1<<((sid+2)%4));
				}
				else
				{
					int sid = (slp=='B')?2:((slp=='F')?3:((slp-'I')/3));

					int una = (5+sid*2)%8;
					int unb = (7+sid*2)%8;
					int did = (2+sid*2)%8;

					MapXY axy = GetTileNeihgborXY(i,j,una);
					Sprite* spr1 = L1[ConvXY(axy)];
					MapXY bxy = GetTileNeihgborXY(i,j,unb);								
					Sprite* spr2 = L1[ConvXY(bxy)];
					if(spr1->GetSlope() == 'A' && spr2->GetSlope() == 'A')
					{
						msk |= (1<<((sid+0)%4));
						msk |= (1<<((sid+3)%4));
					}

					MapXY dxy = GetTileNeihgborXY(i,j,did);
					if(elev[ConvXY(dxy)] == alt)
						mskh |= (1<<((sid+2)%4));
				}

				// search suitable sprite
				char name[9] = "PLxdd_??";
				name[2] = slp;
				name[3] = msk + '0' + ((msk>9)?('A'-'0'-10):0);
				name[4] = mskh + '0' + ((mskh>9)?('A'-'0'-10):0);
				Sprite *spr = terrain->GetSpriteWild(name, Terrain::RANDOM);
				
				if(spr)
				{
					// update sprite
					L1[ConvXY(i,j)] = spr;
				}
				
				
			}
		}
	}


}





// returns XY of tiles neighboring the XY tile at angle = <0;7>
MapXY SpellMap::GetTileNeihgborXY(MapXY mxy,int angle)
{
	return(GetTileNeihgborXY(mxy.x, mxy.y, angle));
}
MapXY SpellMap::GetTileNeihgborXY(int x, int y, int angle)
{
	const int xlsta[8]={+1,+1, 0, 0,-1, 0, 0,+1};
	const int xlstb[8]={+1, 0, 0,-1,-1,-1, 0, 0};
	const int ylst[8] ={0,-1,-2,-1, 0,+1,+2,+1};
	MapXY nxy;	
	nxy.x = x + ((y&1)?xlstb[angle]:xlsta[angle]);
	nxy.y = y + ylst[angle];
	if(nxy.x < 0 || nxy.x >= x_size || nxy.y < 0 || nxy.y >= y_size)
		nxy = {-1,-1};
	return(nxy);
}

// change elevation of terrain by step for all selected tiles
int SpellMap::EditElev(wxBitmap& bmp, TScroll* scroll, int step)
{
	int i,j;

	// leave if map not loaded
	if(!IsLoaded())
		return(1);

	// update map L1 flags
	SyncL1flags();

	// get selection tiles
	auto msel = GetSelections(bmp, scroll);
	if(!msel[0].IsSelected())
		return(2);

	// === for each selected ===
	int min = ELEV_MAX;
	int max = ELEV_MIN;
	for(i = 0; i < msel.size(); i++)
	{
		// selected tile
		int mxy = ConvXY(msel[i]);
		Sprite *sprite = L1[mxy];

		// tile slope
		char sn = sprite->name[2];

		// normal tile - use its alt
		int elv = elev[mxy];
		if(elv > max)
			max = elv;
		if(elv < min)
			min = elv;

		// find min/max
		if(sn!='A' && step<0)
		{
			// shaped tile - check neighbours
			for(j=0;j<8;j++)
			{
				MapXY nxy = GetTileNeihgborXY(msel[i],j);
				
				// done if outside map
				if(nxy.x < 0 || nxy.x >= x_size || nxy.y < 0 || nxy.y >= y_size)
					continue;

				// tile alt
				int nalt = elev[ConvXY(nxy)];

				if(nalt > max)
					max = nalt;
				if(nalt < min)
					min = nalt;
			}
		}
	}


	vector<TTileElevMod> elist;
	for(i = 0; i < msel.size(); i++)
	{
		// selected tile
		int mxy = ConvXY(msel[i]);
		Sprite* sprite = L1[mxy];

		// tile slope
		char sn = sprite->name[2];

		if(step > 0 || sn == 'A')
		{
			// update alt
			int elv = elev[mxy];
			int elv_o = elv;
			if((step > 0 && elv == min) || (step < 0 && elv == max))
			{
				elv += step;
				if(elv < ELEV_MIN)
					elv = ELEV_MIN;
				if(elv > ELEV_MAX-1)
					elv = ELEV_MAX-1;
				elev[mxy] = elv;

				// add tile to list of modified
				TTileElevMod pel;
				pel.x = msel[i].x;
				pel.y = msel[i].y;
				pel.edir = elv - elv_o; // delta elevation
				elist.push_back(pel);
			}
		}
		else
		{
			// shaped tile
			for(j = 0; j < 8; j++)
			{				
				// skip if outside map
				MapXY nxy = GetTileNeihgborXY(msel[i],j);
				if(nxy.x < 0 || nxy.x >= x_size || nxy.y < 0 || nxy.y >= y_size)
					continue;
				
				// tile alt down
				int alt = elev[ConvXY(nxy)];
				int alt_o = alt;

				if(alt == max)
				{
					alt += step;
					if(alt < ELEV_MIN)
						alt = ELEV_MIN;
					if(alt > ELEV_MAX-1)
						alt = ELEV_MAX-1;
					elev[ConvXY(nxy)] = alt;
					
					// add tile to list of modified
					TTileElevMod pel;
					pel.x = nxy.x;
					pel.y = nxy.y;
					pel.edir = alt - alt_o; // delta elevation
					elist.push_back(pel);
				}
			}
		}
	}

	// allocate modification flag array
	uint8_t *flag = new uint8_t[x_size*y_size];
	std::memset((void*)flag, 0x00,x_size*y_size*sizeof(uint8_t));

	for(i = 0; i < elist.size(); i++)
	{
		// selected tile
		int x = elist[i].x;
		int y = elist[i].y;

		// modify heigth
		int elv = elev[ConvXY(x,y)];

		if(elist[i].edir != 0)
		{
			// update elevations
			EditElevNbr(flag,elv,elist[i].edir,x,y);

			// update land slopes
			EditElevSlope(flag);
		}
	}
	for(i = 0; i < elist.size(); i++)
	{
		if(elist[i].edir != 0)
		{
			// update land slopes
			EditElevSlope(flag);
		}
	}
	/*for(i = 0; i < elist.size(); i++)
	{
		if(elist[i].edir != 0)
		{
			// update land textures
			EditElevText(flag);
		}
	}*/

	// apply textures
	ReTexture(flag);


	// loose memory
	delete[] flag;

	//DrawCMapSectorForce();

	return(0);
}

// get neighboring map tile pos
MapXY SpellMap::GetNeighborTile(MapXY xy,int quad)
{
	return(GetNeighborTile(xy.x, xy.y, quad));
}
MapXY SpellMap::GetNeighborTile(int x, int y, int quad)
{
	MapXY tile;
	if(quad > 4 || quad < 0)
		return(tile);

	// get neighbor
	const int y_ofs[4] ={-1,-1,+1,+1};
	if(y&1)
	{
		// odd row
		const int x_ofs[4] ={0,-1,-1,0};		
		tile.x = x + x_ofs[quad];
	}
	else
	{
		// even row
		const int x_ofs[4] = {1,0,0,1};
		tile.x = x + x_ofs[quad];
	}
	tile.y = y + y_ofs[quad];

	// check range
	if(tile.y < 0 || tile.y >= y_size || tile.x < 0 || tile.x >= x_size)
	{
		tile.x = -1;
		tile.y = -1;
	}
	return(tile);
}

// this routine tries to fix textures of modified tiles
int SpellMap::ReTexture(uint8_t *modz,std::function<void(std::string)> status_cb)
{	

	if(status_cb)
		status_cb("Texturing...");

	// shuffle context alternatives before texturing
	//terrain->ShuffleSpriteContext();

	SyncShading();
	
	dbg_ord.clear();

	// clear filter mask (using it to show failed textures)
	filter.assign(x_size*y_size, NULL);

	int mode = 0;

	if(mode == 0)
	{
		bool fail = false;

		vector<vector<Sprite*>> cand_map(x_size*y_size);
		vector<MapXY> tlist;
		vector<vector<MapXY>> tile_lists;

		while(!fail)
		{
			// all to non-assigned
			for(int k = 0; k < x_size*y_size; k++)
			{
				if(modz[k])
					modz[k] = 1;
				cand_map[k].clear();
			}
		
			// build ordered list of tiles to assign
			// using spiral order from outside from tile with most assigned neighbors			
			tlist.clear();
			tile_lists.clear();
			while(true)
			{			
				// count fixed neghbors count for each unassigned tile
				vector<MapXY> mlist;
				vector<int> mcnt;
				for(int k = 0; k < x_size*y_size; k++)
				{
					if(modz[k] == 1)
					{
						MapXY mxy(k%x_size,k/x_size);
						mlist.push_back(mxy);
						int count = 0;
						for(int eid = 0; eid < 4; eid++)
						{
							MapXY nxy = GetNeighborTile(mxy,eid);
							if(nxy.IsSelected() && modz[ConvXY(nxy)] == 0)
								count++;
						}
						mcnt.push_back(count);
					}
				}
				if(!mlist.size())
					break; // done

				// add tile group to list of tile lists
				tile_lists.emplace_back();

				// max number of neghbors - start with this tile
				//int max_neig_cnt = *max_element(tncnt.begin(), tncnt.end());
				int max_neig = max_element(mcnt.begin(),mcnt.end()) - mcnt.begin();
				MapXY mxy = mlist[max_neig];

				// now spiral from initial tile to center ccw
				while(true)
				{
					// search first invalid edge (needed to follow inner edge of unassigned area)
					int edge_init = 0;
					for(int eid = 0; eid < 4; eid++)
					{
						MapXY nxy = GetNeighborTile(mxy,eid);
						if(nxy.IsSelected() && modz[ConvXY(nxy)] != 1)
						{
							edge_init = eid;
							break;
						}
					}
					// tile assigned
					modz[ConvXY(mxy)] = 2;
					tlist.push_back(mxy);					
					tile_lists.back().push_back(mxy);

					// not find next tile of spiral
					int found = false;
					for(int eid = 0; eid < 4; eid++)
					{					
						MapXY nxy = GetNeighborTile(mxy,(eid + edge_init)&3);
						if(!nxy.IsSelected())
							continue;
						if(modz[ConvXY(nxy)] != 1)
							continue;
						mxy = nxy;										
						found = true;
						break;
					}
					if(!found)
						break;
				}
				// repeat just in case there are several isolated islands of tiles to assign
			}
			// all to non-assigned
			for(int k = 0; k < x_size*y_size; k++)
				if(modz[k])
					modz[k] = 1;

			dbg_ord = tlist;
		
		
			// make map of possible candidates			
			for(auto & tile_xy : tlist)		
			{
				int mxy = ConvXY(tile_xy);
				if(modz[mxy] == 0)
					continue;

				Sprite *spr = L1[mxy];
				auto ref_class = L1_flags[mxy];
				int ref_slope = spr->GetSlope();
				auto ref_obj_class = spr->GetToolClass();
				auto ref_obj_grp = spr->GetToolClassGroup();

				vector<Sprite*> cand_list;
				vector<Sprite*> cand_list2;
				cand_list.reserve(terrain->GetSpriteCount());
				cand_list2.reserve(terrain->GetSpriteCount());
				cand_list.clear();

				// -- for each search phase (1. har assigned reference, 2. candidates reference, 3. lookahead filter)
				int first_neighbor;
				int next_first_neighbor = true;
				for(int phase = 0; phase < 3; phase++)
				{
					// -- for each tile edge (neighbor):
					for(int eid = 0; eid < 4; eid++)
					{
						// neighboring tile
						MapXY o_nxy = GetNeighborTile(tile_xy,eid);
						if(!o_nxy.IsSelected())
							continue; // outside map					
						int nxy = ConvXY(o_nxy);

						// neighbor tile
						Sprite* neig = L1[nxy];
						auto neig_class = L1_flags[nxy];
						auto neig_obj_class = neig->GetToolClass();
						auto neig_obj_grp = neig->GetToolClassGroup();
					
						// neighbor edge index
						int eid180 = (eid + 2)&3;
												
						cand_list2.clear();

						first_neighbor = next_first_neighbor;

						if(phase == 0)
						{
							// -- 1. phase: make candidate list based on hard assigned neihbors only						
							if(modz[nxy] == 0)
							{							
								// for each context sprite:
								for(int k = 0; k < neig->GetContextCount(eid180,ref_slope); k++)
								{
									Sprite* cspr = neig->GetContext(eid180,ref_slope,k);
									int is_object = cspr->GetFlags() & Sprite::IS_OBJECT;
									//int is_object = true;
									// skip if class not match
									if(cspr->GetFlags() != ref_class)
										continue;
									if(is_object && cspr->GetToolClass() != ref_obj_class)
										continue;
									if(is_object && ref_obj_class && cspr->GetToolClassGroup() != ref_obj_grp)
										continue;
									if(!CheckTileShading(tile_xy, cspr))
										continue;

									if(first_neighbor)
									{
										// checking first neighbor: just add candidate to list
										next_first_neighbor = false;
										cand_list.push_back(cspr);
									}
									else
									{
										// checking next neighbor: select only previous candidate that matches the new one (if there is some)
										if(find(cand_list.begin(), cand_list.end(), cspr) != cand_list.end())
											cand_list2.push_back(cspr);
									}
								}
								if(!first_neighbor)
									cand_list = cand_list2; // move temp candidate list to working one for next edge test
							}
						}
						else if(phase == 1)
						{
							// -- 2. phase: filter candidate list by previously assigned candidates
							if(cand_map[nxy].size())
							{
								// process each candidate:
								for(auto& cand : cand_map[nxy])
								{
									// for each context sprite of neighbor:
									for(int k = 0; k < cand->GetContextCount(eid180,ref_slope); k++)
									{
										Sprite* cspr = cand->GetContext(eid180,ref_slope,k);
										int is_object = cspr->GetFlags() & Sprite::IS_OBJECT;
										//int is_object = true;
										// skip if class not match
										if(cspr->GetFlags() != ref_class)
											continue;
										if(is_object && cspr->GetToolClass() != ref_obj_class)
											continue;
										if(is_object && ref_obj_class && cspr->GetToolClassGroup() != ref_obj_grp)
											continue;
										if(!CheckTileShading(tile_xy,cspr))
											continue;

										if(first_neighbor)
										{
											next_first_neighbor = false;
											// checking first neighbor: just add candidate to list
											if(find(cand_list.begin(),cand_list.end(),cspr) == cand_list.end())
												cand_list.push_back(cspr); // add only if no exist in list
										}
										else
										{
											// checking next neighbor: select only previous candidate that matches the new one (if there is some)
											if(find(cand_list.begin(),cand_list.end(),cspr) != cand_list.end())
												if(find(cand_list2.begin(),cand_list2.end(),cspr) == cand_list2.end())
													cand_list2.push_back(cspr);
										}
									}
								}
								if(!first_neighbor)
									cand_list = cand_list2; // move temp candidate list to working one for next edge test
							}
						}
						else if(phase == 2)
						{
							// 3. phase: look ahead to yet not assigned tiles and filter by their classes and stuff...
						
							/*for(const auto & cspr : terrain->sprites)
							{
								// skip nor matching types
								bool is_object = cspr->GetFlags() & Sprite::IS_OBJECT;
								if(cspr->GetSlope() != ref_slope)
									continue;
								if(cspr->GetFlags() != ref_class)
									continue;
								if(is_object && cspr->GetToolClass() != ref_obj_class)
									continue;
								if(is_object && ref_obj_class && cspr->GetToolClassGroup() != ref_obj_grp)
									continue;

								// for each context of this candidate in current direction (edge):
								// check if at least one context has required classes
								bool pass = false;
								for(int k = 0; k < cspr->GetContextCount(eid,ref_slope); k++)
								{
									Sprite* pspr = cspr->GetContext(eid,ref_slope,k);
									bool is_object = pspr->GetFlags() & Sprite::IS_OBJECT;
									// skip if class not match
									if(pspr->GetFlags() != neig_class)
										continue;
									if(is_object && pspr->GetToolClass() != neig_obj_class)
										continue;
									if(is_object && ref_obj_class && pspr->GetToolClassGroup() != neig_obj_grp)
										continue;
									pass = true;
									break;
								}
								if(!pass)
									continue;

							
								if(first_neighbor)
								{
									// checking first neighbor: just add candidate to list
									cand_list.push_back(cspr);
								}
								else
								{
									// checking next neighbor: select only previous candidate that matches the new one (if there is some)
									for(const auto& cc : cand_list)
									{									
										if(cc == cspr)
										{
											cand_list2.push_back(cspr);
											break;
										}
									}
								}
							}*/
						}
					}
				}

				if(!cand_list.size())
				{
					// FAILED: no candidate found for this tile
					// this is likely caused by invalid surounding
					
					// mask red failed tile
					filter[mxy] = terrain->filter.redpal;

					fail = true;					
					continue;					
				}

				// store candidate list to candidate map
				cand_map[mxy] = cand_list;
			
			}
			if(fail)
			{
				if(status_cb)
					status_cb("Texturing ... failed initial search of tile candidates! Try invalidate region.");
				return(1);
			}

			// remove alternative candiates (pick one)
			for(auto& tile_xy : tlist)
			{
				int mxy = ConvXY(tile_xy);
			
				auto cand_list = cand_map[mxy];
				cand_map[mxy].clear();
				int count = cand_list.size();

				vector<int> done(count, false);
				vector<Sprite*> cand_list2;
				cand_list2.reserve(count);

				for(int cid = 0; cid < count; cid++)
				{
					auto & cand = cand_list[cid];
					if(done[cid])
						continue;
					// mark this candidate done
					done[cid] = true;
					cand_list2.push_back(cand);
				
					// mark all alternatives as done
					for(int aid = cid + 1; aid < count; aid++)
					{
						auto& alt = cand_list[aid];
						if(!done[aid] && cand->CompareSpriteContextAlt(alt))
						{
							done[aid] = true;
							cand_list2.push_back(alt);
						}
					}
					// select random altarnative and store to candidates map
					int count2 = cand_list2.size();
					if(count2 > 1)
						cand_map[mxy].push_back(cand_list2[rand() % count2]);
					else
						cand_map[mxy].push_back(cand_list2[0]);
				}			
			}

			/*for(auto& tile_xy : tlist)
			{
				int mxy = ConvXY(tile_xy);
				if(modz[mxy] == 0)
					continue;

				Sprite* spr = L1[mxy];
				auto ref_class = L1_flags[mxy];
				int ref_slope = spr->GetSlope();
				auto ref_obj_class = spr->GetToolClass();
				auto ref_obj_grp = spr->GetToolClassGroup();
						
				// initial list of candidates
				vector<Sprite*> cand_list;
				cand_list = cand_map[mxy];
				cand_map[mxy].clear();

				// process each candidate:
				for(auto & cand : cand_list)
				{
					// for each tile edge (neighbor):
					int pass = true;
					for(int eid = 0; eid < 4; eid++)
					{									
						// neighboring tile
						MapXY o_nxy = GetNeighborTile(tile_xy.x,tile_xy.y,eid);
						if(!o_nxy.IsSelected())
							continue; // outside map					
						int nxy = ConvXY(o_nxy);
						if(modz[nxy] != 1)
							continue; // not unassined tile
						
						// neighbor edge index
						int eid180 = (eid + 2)&3;
					
						// for each neighbor candidate:
						int found = false;
						for(auto & neig : cand_map[nxy])
						{
							// for each context sprite of neighbor:
							for(int k = 0; k < neig->GetContextCount(eid180,ref_slope); k++)
							{
								Sprite* cspr = neig->GetContext(eid180,ref_slope,k);
								if(cspr == cand)
								{
									// at least one potential neighbor allowed at this edge
									found = true;
									break;
								}
							}
							if(found)
								break;
						}
						if(!found)
						{
							// this edge's nighbor does not allow this tile candidate
							pass = false;
							break;
						}					
					}
					if(pass)
					{
						// place only allowed candidates to filtered list
						cand_map[mxy].push_back(cand);
					}
				}
			
				if(!cand_map[mxy].size())
				{
					// FAILED: no candidate found for this tile
					// this is likely caused by invalid surounding

					// mask red failed tile
					filter[mxy] = terrain->filter.redpal;

					fail = true;				
				}
			}*/
			if(fail)
			{
				if(status_cb)
					status_cb("Texturing ... failed filtering of tile candidates! Try invalidate region.");
				return(1);
			}

		
			// -- hard assign tiles where only one candidate exists:
			int repeat = false;
			for(auto& tile_xy : tlist)
			{
				int mxy = ConvXY(tile_xy);
				if(modz[mxy] == 0)
					continue;						
				if(cand_map[mxy].size() <= 1)
				{
					// single candidate: hard assign, mark as assigned
					L1[mxy] = cand_map[mxy][0];
					modz[mxy] = 0;
					repeat = true;
				}			
			}
			if(!repeat)
				break;
		}
		
		// -- build minimalist list of possible neighbors for each possible candidate of each map tile to assing:
		vector<vector<vector<vector<Sprite*>>>> cand_map_neig(x_size*y_size);
		for(auto & tile_xy : tlist)
		{
			int mxy = ConvXY(tile_xy);
			auto& cand_list = cand_map[mxy];

			// allocate list for each candidate
			cand_map_neig[mxy].resize(cand_list.size());

			// for each candidate in list:			
			for(int cid = 0; cid < cand_list.size(); cid++)
			{
				Sprite *cand = cand_list[cid];

				// allocate list for each edge
				cand_map_neig[mxy][cid].resize(4);

				// for each edge:
				for(int eid = 0; eid < 4; eid++)
				{
					MapXY nxy_pos = GetNeighborTile(tile_xy.x,tile_xy.y,eid);
					if(!nxy_pos.IsSelected())
						continue;
					int nxy = ConvXY(nxy_pos);

					// skip hard assigned tile edges
					if(modz[nxy] == 0)
						continue;

					/*Sprite* spr = L1[nxy];
					auto ref_class = L1_flags[nxy];
					int ref_slope = spr->GetSlope();
					auto ref_obj_class = spr->GetToolClass();
					auto ref_obj_grp = spr->GetToolClassGroup();*/

					// neighbor slope
					auto neig_slope = L1[nxy]->GetSlope();

					// for each neighbor candidate of this candidate:
					for(int nid = 0; nid < cand->GetContextCount(eid, neig_slope); nid++)
					{						
						Sprite *neig = cand->GetContext(eid, nid);

						// check match with local map neghbors candidates:
						for(const auto & nspr : cand_map[nxy])
						{
							if(neig == nspr)
							{
								// add candidate to list
								cand_map_neig[mxy][cid][eid].push_back(neig);
								break;
							}
						}
					}																	
				}
			}
		}

		string str = "";
		for(auto & cmap : cand_map)
		{
			if(cmap.size())
			{
				str = str + string_format("%d ",cmap.size());
			}
		}
		if(status_cb)
			status_cb("Texturing ... candidate list: " + str);

		//return(0);

		// randomizer seed
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		
		// -- texturing start: for each isolated tiles group
		for(const auto & tile_list : tile_lists)
		{			

			// reset all to non-assigned again
			for(int k = 0; k < x_size*y_size; k++)
			{
				if(modz[k])
					modz[k] = 1;
			}

			// candidate index map (points to candidate in cand_map[][index])
			vector<int> index_map(x_size*y_size, -1);			
			vector<vector<int>> cand_lists(x_size*y_size);

			

			int level = 0;
			while(true)
			{	
				// this tile
				auto tile_xy = tile_list[level];
				int mxy = ConvXY(tile_xy);
				
				// this tile candidate index
				int& index = index_map[mxy];

				// this tile
				if(index < 0)
				{
					// this level is still empty: fill in all possible tiles based on known tiles and known classes around

					// local candidate buffers (temp data, two swappable buffers)
					vector<Sprite*> cand_buf[2];
					auto *cand1 = &cand_buf[0];
					auto *cand2 = &cand_buf[1];

					// start with entire candidate list
					*cand1 = cand_map[mxy];					
					cand2->reserve(cand1->size());
					cand2->clear();

					// for each edge (neighbor)
					for(int eid = 0; eid < 4; eid++)
					{
						int eid180 = (eid + 2)&3;
						// neighbor xy
						MapXY nxy_pos = GetNeighborTile(tile_xy, eid);
						if(!nxy_pos.IsSelected())
							continue;
						int nxy = ConvXY(nxy_pos);
						// test only assigned tiles, but not hard assigned
						if(modz[nxy] != 2)
							continue;

						// filter candidate list by neighbor list						
						int neig_index = cand_lists[nxy][index_map[nxy]];
						auto & neig_list = cand_map_neig[nxy][neig_index][eid180];
						cand2->assign(cand1->size(),NULL);
						for(int k = 0; k < cand1->size(); k++)
						{
							if(cand1->at(k) && find(neig_list.begin(),neig_list.end(),cand1->at(k)) != neig_list.end())
								cand2->at(k) = cand1->at(k);
						}
						auto temp = cand1;
						cand1 = cand2;
						cand2 = temp;
					}
					
					// store candidate list (storing indexes of candidates selected from cand_map[][])
					cand_lists[mxy].reserve(cand1->size());
					cand_lists[mxy].clear();
					for(int k = 0; k < cand1->size();k++)
					{
						if(cand1->at(k))
							cand_lists[mxy].push_back(k);
					}

					// shuffle candidates					
					std::shuffle(cand_lists[mxy].begin(),cand_lists[mxy].end(),std::default_random_engine(seed));

					// reset candidate index
					index = 0;
				}


				if(index >= cand_lists[mxy].size())
				{
					// all candidates tested (unsuccessfully) 

					// mark current position back as not assigned
					modz[mxy] = 1;

					// invalidate this level
					index = -1;

					// move level back
					level--;
					
					// move previous level index
					if(level >= 0)
					{
						auto tile_xy = tile_list[level];
						int mxy = ConvXY(tile_xy);
						index_map[mxy]++;
					}



				}
				else
				{
					// some candidate found
					Sprite *spr = cand_map[mxy][cand_lists[mxy][index]];

					// put new sprite to current position
					L1[mxy] = spr;
					// mark it as temporarily assigned
					modz[mxy] = 2;

					// move to next level					
					level++;
				}				

				if(level < 0)
				{
					// search failed:

					if(status_cb)
						status_cb("Texturing ... failed!");

					break;
				}
				if(level >= tile_list.size())
				{
					// texturing done with success:

					if(status_cb)
						status_cb("Texturing ... done!");

					break;
				}
			}
			
		}


	}
	else if(mode == 1)
	{
		// make list of modified tiles (zig-zag order)
		vector<MapXY> tlist;
		MapXY mxy;
		/*mxy.x = 0;
		mxy.y = 0;
		while(mxy.y < y_size)
		{
			while(mxy.x < x_size)
			{
				if(modz[ConvXY(mxy)])
				{
					tlist.push_back(mxy);
					modz[ConvXY(mxy)] = 1;
				}
				mxy.x++;
			}
			mxy.y++;
			if(mxy.y >= y_size)
				break;
			while(mxy.x > 0)
			{
				mxy.x--;
				if(modz[ConvXY(mxy)])
				{
					tlist.push_back(mxy);
					modz[ConvXY(mxy)] = 1;
				}
			}
			mxy.y++;
		}*/
		// all to non-assigned
		for(int k = 0; k < x_size*y_size; k++)
		{
			if(modz[k])
				modz[k] = 1;
		}
		// search initial tile
		for(int k = 0; k < x_size*y_size; k++)
		{
			if(modz[k])
			{
				mxy.x = k%x_size;
				mxy.y = k/x_size;
				tlist.push_back(mxy);
				modz[k] = 2; // assigned
				break;
			}
		}
		// now spiral from initial tile to center ccw
		while(true)
		{
			bool found = false;
			for(int eid = 0; eid < 4; eid++)
			{
				MapXY nxy = GetNeighborTile(mxy,eid);
				if(modz[ConvXY(nxy)] != 1)
					continue;
				mxy = nxy;
				tlist.push_back(mxy);
				modz[ConvXY(nxy)] = 2; // assigned
				found = true;
				break;
			}
			if(!found)
				break;
		}
		// reset all to non-assigned again
		for(int k = 0; k < x_size*y_size; k++)
		{
			if(modz[k])
				modz[k] = 1;
		}


	
		// make local candidate flags array
		Sprite **cand = new Sprite*[terrain->GetSpriteCount()];
		Sprite **cand2 = new Sprite*[terrain->GetSpriteCount()];
	

		vector<SpellMapTxt> seq;

		int level = 0;
		while(1)
		{
			if(level >= seq.size())
			{
				// new level
				SpellMapTxt rec;
				seq.push_back(rec);
			}

			// this level
			SpellMapTxt *rec = &seq[level];
		
			// this tile
			MapXY mxy = tlist[level];
			if(!rec->TilesCount())
			{
				// this level is still empty: fill in all possible tiles based on known tiles and known classes around
			
				// ref tile
				Sprite *spr = L1[ConvXY(mxy)];
				uint32_t ref_flags = L1_flags[ConvXY(mxy)];
				uint32_t ref_obj_class = spr->GetToolClass();
				uint32_t ref_obj_grp = spr->GetToolClassGroup();

				// step 1: make list based on already assigned tiles
				int cand_N = 0;		
				for(int eid = 0; eid < 4; eid++)
				{
					// checking first neghbor?
					bool first_neighbor = (cand_N == 0);
					// neighbor tile
					MapXY nxy = GetNeighborTile(mxy,eid);
					if(!nxy.IsSelected())
						continue;
					// skip if not yet assigned
					if(modz[ConvXY(nxy)] == 1)
						continue;
					// neighbor edge index
					int eid180 = (eid + 2)&3;
					// neighbor tile
					Sprite *neig = L1[ConvXY(nxy)];

					int neig_slope = spr->GetSlope();
				
					// for each context sprite:
					std::memset((void*)cand2,0,cand_N*sizeof(Sprite*));
					for(int k = 0; k < neig->GetContextCount(eid180,neig_slope); k++)
					{					
						Sprite* cspr = neig->GetContext(eid180,neig_slope,k);
						// skip if class not match
						if(cspr->GetFlags() != ref_flags)
							continue;
						if(cspr->GetToolClass() != ref_obj_class)
							continue;
						if(ref_obj_class && cspr->GetToolClassGroup() != ref_obj_grp)
							continue;

					
						if(first_neighbor)
						{
							// checking first neighbor: just add candidate to list
							cand[cand_N++] = cspr;
						}
						else
						{
							// checking next neighbor: need to check overlap with previous neighbor						
							for(int m = 0; m < cand_N; m++)
							{
								if(cand[m] == cspr)
									cand2[m] = cspr;
							}
						}
					}
					if(!first_neighbor)
					{
						Sprite **temp = cand;
						cand = cand2;
						cand2 = temp;
					}			
				}
			
				// step 2: reduce list using remaining unassigned neighbors based on their classes
				//         this is kind of look ahead to get rid of candidates that at not assignable anyways
				//         so each of the candidates from step 1 must have at least one own candidate neighbor
				//   for each candidate:
				for(int cid = 0; cid < cand_N; cid++)
				{
					// skip unassigned candidate items
					Sprite* cspr = cand[cid];
					if(!cspr)
						continue;

					// for each neighbor:
					for(int eid = 0; eid < 4; eid++)
					{
						// neighbor tile
						MapXY nxy = GetNeighborTile(mxy,eid);
						if(!nxy.IsSelected())
							continue;
						// skip if assigned
						if(modz[ConvXY(nxy)] != 1)
							continue;

						int neig_slope = L1[ConvXY(nxy)]->GetSlope();
					
						// for each context of candidate:
						bool allow = false;
						for(int k = 0; k < cspr->GetContextCount(eid,neig_slope); k++)
						{
							Sprite* nspr = cspr->GetContext(eid,neig_slope,k);
							// skip if neighbor class not match what should be in the position
							if(nspr->GetFlags() != L1_flags[ConvXY(nxy)])
								continue;
							allow = true;
							break;
						}
						if(!allow)
						{
							// not possible candidate: remove it from list
							cand[cid] = NULL;
							break;
						}
					}
				}					
				// make condensed candidate list
				for(int m = 0; m < cand_N; m++)
				{
					if(cand[m])
						rec->AddTile(cand[m]);
				}			
				// reset candidate index
				rec->Shuffle();
				rec->SetIndex();			
			}

			// get candidate
			Sprite * spr = rec->GetTile();
			// move to next tile
			rec->NextIndex();
			if(!spr)
			{
				// all candidates tested (unsuccessfully) 
				 
				// mark current position back as not assigned
				modz[ConvXY(mxy)] = 1;
				 
				// move level back
				level--;
				// remove last level from stack
				seq.pop_back();
			}
			else
			{
				// some match found
				
				// put new sprite to current position
				L1[ConvXY(mxy)] = spr;
				// mark it as temporarily assigned
				modz[ConvXY(mxy)] = 2;
								
				// move to next level
				level++;

				// reset search index
				/*if(level < seq.size())
					seq[level].SetIndex();*/
			}

			if(level < 0)
			{
				// search failed:

				if(status_cb)
					status_cb("Texturing ... failed!");

				break;
			}
			if(level >= tlist.size())
			{
				// texturing done with success:

				if(status_cb)
					status_cb("Texturing ... done!");

				break;
			}
		}

		delete[] cand;
		delete[] cand2;

	}

	

	return(0);
}


// invalidate region of map (retexture)
int SpellMap::IvalidateTiles(vector<MapXY> tiles,std::function<void(std::string)> status_cb)
{
	// leave if map not loaded
	if(!IsLoaded())
		return(1);

	// get selection tiles
	if(!tiles.size() || !tiles[0].IsSelected())
		return(2);

	// update map L1 flags
	SyncL1flags();

	// allocate modification flag array
	vector<uint8_t> flag(x_size*y_size, 0);

	// === for each selected ===
	for(auto & xy : tiles)
	{
		// selected tile
		int mxy = ConvXY(xy);
		// set modified flag
		flag[mxy] = 1;
	}
	
	// apply textures
	ReTexture(flag.data(),status_cb);

	return(0);
}







int SpellMapTxt::TilesCount()
{
	return(tile_list.size());
}
// add tile to list
void SpellMapTxt::AddTile(Sprite *tile)
{
	tile_list.push_back(tile);
}
// clear tile list
void SpellMapTxt::ClearTiles()
{
	tile_list.clear();
}
// set tile index in the list
int SpellMapTxt::SetIndex(int id)
{
	if(id >= tile_list.size())
		return(1);
	index = id;
	return(0);
}
// shuffle candidates order
void SpellMapTxt::Shuffle()
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(tile_list.begin(), tile_list.end(),std::default_random_engine(seed));
}
// move tile index forward, return 1 if outside
int SpellMapTxt::NextIndex()
{	
	index++;
	if(index >= tile_list.size())
		return(1);
	return(0);
}
// get current tile from list or -1 if empty
Sprite *SpellMapTxt::GetTile(int id)
{
	if(!tile_list.size() || index >= tile_list.size())
		return(NULL);
	if(id >= 0)
		return(tile_list[id]);
	else
		return(tile_list[index]);
}


// update L1 class flags using current L1 sprite array (should be called whenever L1 was modified)
void SpellMap::SyncL1flags()
{
	// for each tile:
	for(int y=0;y<y_size;y++)
	{
		for(int x=0;x<x_size;x++)
		{
			L1_flags[ConvXY(x,y)] = L1[ConvXY(x,y)]->GetFlags();
		}
	}
}

// update map shading flags
void SpellMap::SyncShading()
{
	shading.assign(x_size*y_size, 0);
	shading_mask.assign(x_size*y_size,0);

	Sprite ref;
	Sprite neig;

	for(int y=0; y<y_size; y++)
	{
		for(int x=0; x<x_size; x++)
		{
			int mxy = ConvXY(x,y);
			for(int eid = 0; eid <4; eid++)
			{
				int eid180 = (eid + 2)&3;
				auto nxy_pos = GetNeighborTile(x,y,eid);
				int nxy = ConvXY(nxy_pos);				

				if(nxy_pos.IsSelected())
				{
					// make two fake sprites					
					ref.name[2] = L1[mxy]->name[2];					
					neig.name[2] = L1[nxy]->name[2];

					if(ref.name[2] == 'M')
						ref.name[2] *= 1;

					// try to find valid edge shading combination
					ref.SetShadingFlags(0xFF00 | 0);
					neig.SetShadingFlags(0xFF00 | 0);
					if(ref.CheckShadingValid(&neig,eid))
					{
						shading[mxy] |= 0<<eid;
						continue;
					}
					ref.SetShadingFlags(0xFF00 | 1<<eid);
					if(ref.CheckShadingValid(&neig,eid))
					{
						shading[mxy] |= 1<<eid;
						continue;
					}
					neig.SetShadingFlags(0xFF00 | 1<<eid180);
					if(ref.CheckShadingValid(&neig,eid))
					{
						shading[mxy] |= 1<<eid;
						continue;
					}
					ref.SetShadingFlags(0xFF00 | 0);
					if(ref.CheckShadingValid(&neig,eid))
					{
						shading[mxy] |= 0<<eid;
						continue;
					}
				}
				else
				{
					// undecided
					shading_mask[mxy] |= 1<<eid;
				}
			}
		}
	}

}
// check tile candidate has allowed shading
int SpellMap::CheckTileShading(MapXY &pos, Sprite *spr)
{
	int mxy = ConvXY(pos);
	int ref_flag = shading[mxy] & 0x0F;
	int ref_mask = shading[mxy] & 0x0F;
	int spr_flag = spr->GetShadingFlags() & 0x0F;
	int spr_mask = spr->GetShadingMask() & 0x0F;

	// ignore edges that are masked by reference
	ref_flag &= ~ref_mask;
	spr_flag &= ~ref_mask;
	// ignore edge that are masked by sprite
	ref_flag &= ~spr_mask;
	spr_flag &= ~spr_mask;
	return(ref_flag == spr_flag);
}

// update sprite context based on selected map tiles
int SpellMap::BuildSpriteContext()
{
	for(int y=0;y<y_size;y++)
	{
		for(int x=0;x<x_size;x++)
		{
			// for each selection
			int mxy = ConvXY(x,y);
			if(!select[mxy])
				continue;

			// get center tile
			Sprite *tile = L1[mxy];
			int tid = terrain->GetSpriteID(tile);
			if(tid < 0)
				return(1);

			// for each neighbor
			for(int qid = 0; qid < 4; qid++)
			{
				// get neighbor
				auto nxy = GetNeighborTile(x, y, qid);
				if(nxy.IsSelected())
				{
					// add to list
					Sprite* sprite = L1[ConvXY(nxy)];
					terrain->sprites[tid]->AddContext(qid, sprite);
				}
			}
		}
	}
	
	return(0);
}




// Edit map class
int SpellMap::EditClass(vector<MapXY>& selection, SpellTool *tool, std::function<void(std::string)> status_cb)
{
	// leave if map not loaded
	if(!IsLoaded())
		return(1);


	if(status_cb)
		status_cb("Changing terrain class...");


	// update map L1 flags
	SyncL1flags();

	if(tool->isObject())
		return(1);

	// get tool selection
	auto [tool_id, item_id] = tool->GetTool();

	Sprite *tspr = NULL;
	for(auto const& sid : terrain->sprites)
	{
		if(sid->GetToolClass() == tool_id + 1 &&
			sid->GetToolClassGroup() == item_id + 1 &&
			(sid->GetGlyphFlags() & Sprite::IS_TOOL_ITEM_GLYPH))
		{
			// mathing sprite found: 
			tspr = sid;
			break;
		}
	}

	if(!tspr)
		return(1);
	
	
	// make modified flags array
	uint8_t *modz = new uint8_t[x_size*y_size];
	std::memset((void*)modz, 0,x_size*y_size);

	
	typedef struct{
		MapXY pos;
		uint32_t flags;
	}Task;
	vector<Task> task_list;

	// make initial list of tasks to do
	for(auto& tsel : selection)
	{
		Task task = {tsel, tspr->GetFlags()};
		task_list.push_back(task);
	}
		
	for(int tid = 0; tid < task_list.size(); tid++)
	{
		Task task = task_list[tid];

		uint32_t tool_flags = task.flags;

		uint32_t terr_base = (Sprite::CLASS_DARK_GRASS | Sprite::IS_SAND | Sprite::IS_MUD | Sprite::IS_SWAPM | Sprite::IS_BLOOD | Sprite::IS_WATER);
		uint32_t mask_others = terr_base & ~tool_flags;		
		
		if(task.pos.IsSelected())
		{
			int pxy = ConvXY(task.pos);

			if(modz[pxy])
				continue;

			auto flag = L1_flags[pxy];
						
			if(tool_flags & Sprite::IS_GRASS)
			{
				if(flag & Sprite::IS_GRASS)
				{
					flag = tool_flags;
				}
				else
				{
					flag |= tool_flags;
					flag &= ~Sprite::IS_RIDGE;
				}
			}
			else if(tool_flags & terr_base)
			{
				if(flag & tool_flags)
				{
					flag &= ~Sprite::IS_GRASS;
				}
				else
				{
					if(flag & mask_others)
						flag |= Sprite::IS_GRASS;
					flag |= tool_flags;
					flag &= ~mask_others;
				}
			}
			
			//L1[pxy] = tspr;
			L1_flags[pxy] = flag;
			L1[pxy] = terrain->GetTileGlyph(tspr, flag);
			if(!L1[pxy])
				L1[pxy] = terrain->GetSprite("START");

			// tile definitive
			modz[pxy] = 1;


			
			// --- fix neighbors

			
			uint32_t ref_flags = flag;

			// for all 8 neighbors check:
			for(int nid = 0; nid<8; nid++)
			{
				// neighbor tile agnle (45, 135, 225, 315, 0, 90, 180, 270)
				int n_angle = (nid&3)*2 + !(nid>>2);
				MapXY n_pos = GetTileNeihgborXY(task.pos, n_angle);
				if(!n_pos.IsSelected())
					continue;
				int nxy = ConvXY(n_pos);

				// skip if already assigned
				if(modz[nxy])
					continue;

				uint32_t n_flags = L1_flags[ConvXY(n_pos)];
				uint32_t new_flags = n_flags;

				if(!(ref_flags & Sprite::IS_GRASS))
				{
					// ref tile is single class: need to make transition to neighbors
					
					if((ref_flags & terr_base) != (n_flags & terr_base))
					{
						new_flags = ref_flags & terr_base;
						Task task = {n_pos, new_flags};
						task_list.push_back(task);
					}
				}
				else if((n_flags & terr_base) != (ref_flags & terr_base) && !(n_flags & Sprite::IS_GRASS))
				{
					// neighbor is single class, but different than desired: base

					new_flags = Sprite::IS_GRASS;

					Task task = {n_pos, new_flags};
					task_list.push_back(task);
				}

			}

		}

	}


	// expand modified area by 1 (possibly not enough for many terrains?)
	for(int y = 0; y < y_size; y++)
	{
		for(int x = 0; x < x_size; x++)
		{
			for(int nid = 0; nid<8; nid++)
			{
				MapXY n_pos = GetTileNeihgborXY(x,y,nid);
				int nxy = ConvXY(n_pos);

				if(!n_pos.IsSelected())
					continue;

				if(modz[ConvXY(x,y)] != 1)
					continue;

				if(modz[nxy] != 0)
					continue;
				
				modz[nxy] = 2;
			}
		}
	}	

	// try retexture
	ReTexture(modz, status_cb);

	delete[] modz;

	return(0);
}




