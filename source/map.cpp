//=============================================================================
// Spellcross MAP related routines: Loading, rendering.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
/*#undef _HAS_STD_BYTE
#define _HAS_STD_BYTE 0*/

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
// update surface size
void TScroll::SetSurface(int xx,int yy)
{
	xsize = xx;
	ysize = yy;
}
tuple<int,int> TScroll::GetSurface()
{
	return tuple(xsize,ysize);
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
	moved = false;
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
	moved = false;
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
	moved = true;
}
// cursor to idle state (mouse leave or up)
int TScroll::Idle()
{
	state = 0;
	modified = 0;
	
	int was_moved = moved;
	moved = false;
	return(was_moved);
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
// class MapSprite - Map layer 1/2 records
//=============================================================================
MapSprite::~MapSprite()
{
}

MapSprite::MapSprite()
{
	L1 = NULL;
	L2 = NULL;
	elev = 0;
	hp = 0;
	flags = 0;
	is_target = false;
	hit_pnm = NULL;
	hit_pnm_frame = 0;
}

/*MapSprite::MapSprite(Sprite* sprite)
{
	Set(sprite);
}*/

void MapSprite::SetL1(Sprite* sprite,int elev)
{
	L1 = sprite;
	if(elev >= 0)
		this->elev = elev;
	if(L1 && L1->destructible)
	{		
		// init hit points
		int max_hp = L1->destructible->hp;
		if(L1->two_stage_desctruct && L1->is_destructed)
			hp = rand()%(max_hp/2);
		else if(L1->is_destructed)
			hp = 0;
		else
			hp = max_hp;
	}
}
void MapSprite::SetL2(Sprite* sprite,int flags)
{
	L2 = sprite;
	if(flags >= 0)
		this->flags = flags;
	if(L2 && L2->destructible)
	{
		// init hit points
		int max_hp = L2->destructible->hp;
		if(L2->two_stage_desctruct && L2->is_destructed)
			hp = max((10*max_hp)/100,rand()%(max_hp/2));
		else if(L2->is_destructed)
			hp = 0;
		else
			hp = max_hp;
	}
}

int MapSprite::UpdateDestructible()
{
	Sprite** tile = NULL;
	if(L1 && L1->destructible)
		tile = &L1;
	else if(L2 && L2->destructible)
		tile = &L2;
	if(!tile)
		return(false);
		
	if((*tile)->two_stage_desctruct && 2*hp < (*tile)->destructible->hp && !(*tile)->is_destructed && (*tile)->destructible_alt)
	{
		// half destruct tile
		*tile = (*tile)->destructible_alt;
	}
	
	if(!hp && (*tile)->two_stage_desctruct)
	{
		// total destroy of wall
		*tile = NULL;
		flags = 0x00;
		return(true);
	}
	
	if(!hp && (*tile)->destructible_alt)
	{
		// destroy of single destruct level object (bridge, non-walls)
		*tile = (*tile)->destructible_alt;		
		
		// convert bridge class to water
		if(flags == 0x70)
			flags = 0x60;
		// ###todo: destroy stuff around?
		
		return(true);
	}

	return(false);
}

Sprite* MapSprite::GetDestructible()
{
	Sprite* destr = NULL;	
	if(L1 && L1->destructible)
		destr = L1;
	else if(L2 && L2->destructible)
		destr = L2;
	return(destr);
}

int MapSprite::isDestructible()
{
	auto destr = GetDestructible();
	if(!destr)
		return(false);		
	return(true);
}

int MapSprite::isTarget()
{
	auto destr = GetDestructible();
	if(!destr)
		return(false);
	return(destr->is_target);
}

int MapSprite::GetMaxHP()
{
	auto *destr = GetDestructible();
	if(!destr)
		return(0);
	return(destr->destructible->hp);
}

int MapSprite::PlayHit()
{
	SpellSound *snd = NULL;
	if(L1 && L1->destructible && L1->destructible->sound_hit)
		snd = new SpellSound(*L1->destructible->sound_hit);
	else if(L2 && L2->destructible && L2->destructible->sound_hit)
		snd = new SpellSound(*L2->destructible->sound_hit);
	if(snd)
		snd->Play(true);
	return(0);
}

int MapSprite::PlayDestruct()
{
	SpellSound* snd = NULL;
	if(L1 && L1->destructible && L1->destructible->sound_destruct)
		snd = new SpellSound(*L1->destructible->sound_destruct);
	else if(L2 && L2->destructible && L2->destructible->sound_destruct)
		snd = new SpellSound(*L2->destructible->sound_destruct);
	if(snd)
		snd->Play(true);
	return(0);
}

//=============================================================================
// class MapLayer3 - Map layer 3 record (ANM animations)
//=============================================================================
MapLayer3::MapLayer3(AnimL1* anm, int x_pos, int y_pos, int frame_ofs, int frame_limit)
{
	this->anim = anm;
	this->x_pos = x_pos;
	this->y_pos = y_pos;
	this->frame_ofs = min(frame_ofs,(int)anm->frames.size()-1);
	this->frame_limit = min(frame_limit,(int)anm->frames.size()-1);
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
	
	pic = NULL;
	
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
	selected_event = NULL;

	unit_range_th = NULL;
	unit_range_th_control = UNIT_RANGE_TH_IDLE;

	unit_range_view_mode = UNIT_RANGE_NONE;
	unit_range_view_mode_lock = false;

	sound_loops = NULL;

	events = NULL;

	saves = NULL;
}

SpellMap::~SpellMap()
{
	// cleanup heap allocated stuff
	Close();
}

// lock map access
int SpellMap::LockMap()
{
	map_lock.lock();
	return(0);
}

// release map access
int SpellMap::ReleaseMap()
{
	map_lock.unlock();
	return(0);
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
// game mode
int SpellMap::isGameMode()
{
	return(game_mode);
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

	// cleanup unit range calculator
	unit_range_nodes.clear();
	unit_range_nodes_buffer.clear();
	unit_ap_left.clear();
	unit_fire_left.clear();
	if(unit_range_th)
	{
		unit_range_th_control = UNIT_RANGE_TH_EXIT;
		unit_range_th->join();
		delete unit_range_th;
		unit_range_th = NULL;
	}

	// loose layer data
	tiles.clear();
	L3.clear();
	L4.clear();
	// loose start/ciel
	start.clear();
	escape.clear();
	// loose units layer
	Lunit.clear();
	// loose units list
	for(k = 0; k < units.size(); k++)
		delete units[k];
	units.clear();
	unit_selection = NULL;
	// loose selection array
	select.clear();
	L1_flags.clear();

	if(pic)
		delete[] pic;
	pic = NULL;
	pic_y_buffer.clear();

	// delete events 
	if(events)
		delete events;
	events = NULL;

	ClearHUDbuttons();

	selected_event = NULL;
	unit_sel_land_preference = true;
	w_unit_hud = true;
	units_view_mask.clear();
	units_view_mask_0.clear();
	units_view_map.clear();
	
	SetUnitRangeViewMode(UNIT_RANGE_NONE);
	
	msel.clear();

	filter.clear();

	if(sound_loops)
		delete sound_loops;
	sound_loops = NULL;
	sounds.clear();

	if(saves)
		delete saves;
	saves = NULL;
	
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
	tiles.resize(x_size*y_size);
	L1_flags.resize(x_size*y_size);
	
	// filter mask
	filter.resize(x_size*y_size);

	// create events
	events = new SpellMapEvents(this);

	// make save slots
	saves = new Saves(this);

	// generate plain map
	for(int k = 0; k < (x_size * y_size); k++)
	{		
		tiles[k].SetL1(terrain->GetSpriteWild("PLA00_??",Terrain::RANDOM),2);
		tiles[k].SetL2(NULL,0);
	}
	
	// get tile flags from context data
	SyncL1flags();
	
	// create selections maps
	select.assign(x_size*y_size,0x00);

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
	vector<uint8_t> map_buffer(flen);
	fr.read((char*)map_buffer.data(), flen);
	fr.close();
	unsigned char* data = map_buffer.data();
			

	// get L1 data offset
	//int L1_offset = *(int32_t*)data;
	data += 4;

	// get L1 sprites list size
	int L1_count = *(int32_t*)data;
	data += 4;
	if (L1_count > 4095)
	{
		Close();
		return(1);
	}

	// version check (actually dunno what is that but it's always the same and looks like version code)
	if (*data++ != 0x12)
		return(1);

	// get map size
	this->x_size = *(int16_t*)data;
	data += 2;
	this->y_size = *(int16_t*)data;
	data += 2;

	// get map terrain name
	std::memset((void*)this->terrain_name, '\0', sizeof(this->terrain_name));
	std::memcpy((void*)this->terrain_name, data, 13);
	data += 13;

	// try to get terrain data pointer
	this->terrain = spelldata->GetTerrain(this->terrain_name);
	if (!this->terrain)
	{
		Close();
		return(1);
	}

	// store spelldata
	// ###todo: rework in more sensible way? This should not be there...
	this->spelldata = spelldata;
	
	// load list of used L1 sprites
	vector<Sprite*> sprites;
	sprites.assign(L1_count,NULL);
	for (auto & sprite : sprites)
	{
		// read sprite name		
		char name[9];
		std::memset((void*)name, '\0', sizeof(name));
		std::memcpy((void*)name, (void*)data, 8);
		data += 8;

		// try to get sprite's data
		sprite = terrain->GetSprite(name);
		if (!sprite)
		{
			// not found!
			Close();
			return(1);
		}
	}

	// load L1 sprite indices
	tiles.resize(x_size*y_size);
	L1_flags.resize(x_size*y_size);
	for (auto & tile : tiles)
	{
		// get cell code
		int16_t code = *(int16_t*)data;
		data += 2;

		// extract sprite index
		int sid = code & 0x0FFF;

		// elevation code
		int elev = code >> 12;
		
		// put sprite ref to map array
		if (sid > L1_count)
		{
			// out of available range
			Close();
			return(1);
		}
		tile.SetL1(sprites[sid],elev);
	}
	// get tile flags from context data
	SyncL1flags();
	
	



	//////////////
	///// L2 /////
	//////////////

	// read count of L2 sprites
	int L2_count = *(int32_t*)data;
	data += 4;
	if (L2_count > 255)
	{
		Close();
		return(1);		
	}
	
	// load list of used L2 sprites
	sprites.assign(L2_count,NULL);
	for (auto & sprite : sprites)
	{
		// read sprite name
		char name[9];
		std::memset((void*)name, '\0', sizeof(name));
		std::memcpy((void*)name, (void*)data, 8);
		data += 8;

		// try to get sprite's data
		sprite = terrain->GetSprite(name);
		if (!sprite)
		{
			// not found!
			Close();
			return(1);
		}
	}

	// load L2 sprite indices
	for (auto & tile : tiles)
	{
		// get sprite index
		int sid = *(uint8_t*)data++;
		// get sprite flags
		int code = *(uint8_t*)data++;
		
		// put sprite ref to map array
		if (sid > L2_count)
		{
			// out of available range
			Close();
			return(1);
		}
		tile.SetL2((sid)?sprites[sid-1]:NULL,code);
	}




	//////////////////////////////
	///// L3 - ANM animation /////
	//////////////////////////////

	// read count of L3 animations
	int L3_count = *(int32_t*)data; data += 4;

	// load list of used L3 sprites
	if(L3_count)
	{
		vector<AnimL1*> anims(L3_count);
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
				Close();
				return(1);
			}
		}

		// decode animation locations
		if (L3_count)
		{
			// get total ANM items count
			int L3_items = *(int32_t*)data; data += 4;
		
			// for each item:
			L3.reserve(L3_items);
			for (int k = 0; k < L3_items; k++)
			{
				// initial frame offset
				int frame_ofs = *data++;
				// frames count limit, ###note: this may be actually frames count from offset???
				// ###todo: findout why frame offset is byte and frames count word??? Maybe it is byte limit, and another byte of whatever?
				int frame_limit = *(int16_t*)data; data += 2;
				// x,y position in map
				int x_pos = *(int16_t*)data; data += 2;
				int y_pos = *(int16_t*)data; data += 2;
				// animation ID
				int aid = *(int16_t*)data; data += 2;
						
				// put new animation entry to list
				//MapLayer3* anim = new MapLayer3(anims[aid], x_pos, y_pos, frame_ofs, frame_limit);
				L3.emplace_back(anims[aid],x_pos,y_pos,frame_ofs,frame_limit);
						
			}

		}

	}



	//////////////////////////////
	///// L4 - PNM animation /////
	//////////////////////////////

	// read count of L4 animations
	int L4_count = *(int32_t*)data; data += 4;

	// load list of used L3 sprites
	vector<AnimPNM*> pnims(L4_count);
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
			Close();
			return(1);
		}
	}

	// decode animation locations
	if (L4_count)
	{
		// get total PNM items count
		int L4_items = *(int32_t*)data; data += 4;

		// for each item:
		for (int k = 0; k < L4_items; k++)
		{
			// 17 bytes per record
			
			// initial frame offset
			int frame_ofs = *data++;
			// frames count limit, ###note: this may be actually frames count from offset???
			// ###todo: findout why frame offset is byte and frames count word??? Maybe it is byte limit, and another byte of whatever?
			int frame_limit = *(uint16_t*)data; data += 2;
			// x,y position in map
			int x_pos = *(uint16_t*)data; data += 2;
			int y_pos = *(uint16_t*)data; data += 2;
			// animation ID
			int pid = *(uint16_t*)data; data += 2;
			// origin shift in pixels, ###todo: really 4-byte integers??? Seems strange waste of memory
			// ###note: the coordinates seem to be in y,x order for whatever reason
			int y_ofs = *(int32_t*)data; data += 4;
			int x_ofs = *(int32_t*)data; data += 4; 

			// put new animation entry to list
			//MapLayer4* pnim = new MapLayer4(pnims[pid], x_pos, y_pos, x_ofs, y_ofs, frame_ofs, frame_limit);
			L4.emplace_back(pnims[pid],x_pos,y_pos,x_ofs,y_ofs,frame_ofs,frame_limit);
		}

	}



	//////////////////////////////////////
	///// L5 - Mysterious numbers #1 /////
	//////////////////////////////////////
	
	// read count of L5 stuff
	int L5_count = *(int32_t*)data; data += 4;
	data += L5_count*2;

	//////////////////////////////////////
	///// L6 - Mysterious numbers #2 /////
	//////////////////////////////////////

	// read count of L6 stuff
	int L6_count = *(int32_t*)data; data += 4;
	data += L6_count*2;

	//////////////////////////
	///// L7 - Sounds #1 /////
	//////////////////////////

	sound_loops = new MapLoopSounds(x_size,y_size);
	int L7_names_count = *(int32_t*)data; data += 4;
	for(int k = 0; k < L7_names_count; k++)
	{
		char name[9];
		char* pstr = name;
		while(*data)
			*pstr++ = *data++;
		data++;
		*pstr = '\0';
		auto sound = new SpellSound(spelldata->sounds->channels,spelldata->sounds->GetSample(name));
		sound_loops->list.push_back(sound);
	}
	int L7_count = *(int32_t*)data; data += 4;
	for(int k = 0; k < L7_count; k++)
	{
		int pxy = *(uint16_t*)data; data += 2;
		int sid = (int)*data; data++;
		sound_loops->sounds.emplace_back(MapXY(pxy% x_size,pxy / x_size),sound_loops->list[sid]->GetSample());
	}
	sound_loops->UpdateMaps();

	//////////////////////////
	///// L8 - Sounds #2 /////
	//////////////////////////

	int L8_names_count = *(int32_t*)data; data += 4;
	vector<SpellSample*> L8_sounds;
	for(int k = 0; k < L8_names_count; k++)
	{
		char name[9];
		char* pstr = name;
		while(*data)
			*pstr++ = *data++;
		data++;
		*pstr = '\0';
		L8_sounds.push_back(spelldata->sounds->GetSample(name));
	}
	int L8_count = *(int32_t*)data; data += 4;
	for(int k = 0; k < L8_count; k++)
	{
		int pxy = *(uint16_t*)data; data += 2;
		int sid = (int)*data; data++;		
		sounds.emplace_back(MapXY(pxy% x_size,pxy / x_size), L8_sounds[sid]);
	}


	


	//////////////////////////////////////
	///// Other map arrays and stuff /////
	//////////////////////////////////////
	
	// create selections maps
	select.assign(x_size*y_size,0x00);

	// filter mask
	filter.resize(x_size*y_size, NULL);

	// init unit pointer PNM
	pnm_sipka = MapLayer4(spelldata->pnm_sipka);

	// create events list
	events = new SpellMapEvents(this);

	// make save slots
	saves = new Saves(this);

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
					int xy = stoi(cmd->parameters->at(p));
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
					int xy = stoi(cmd->parameters->at(p));
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
				unit->id = stoi(cmd->parameters->at(0));

				// unit type index
				unit->type_id = stoi(cmd->parameters->at(1));
				
				// try fetch unit record from spelldata
				unit->unit = spelldata->units->GetUnit(unit->type_id);
				if(!unit->unit)
				{
					delete mission_data;
					delete unit;
					return(1);
				}

				// position
				int xy = stoi(cmd->parameters->at(2));
				unit->coor.y = (xy / x_size);
				unit->coor.x = xy - unit->coor.y * x_size;

				// experience
				unit->InitExperience(stoi(cmd->parameters->at(3)));

				// man count
				unit->man = stoi(cmd->parameters->at(4));

				// unit behaviour type
				unit->type = cmd->parameters->at(5).c_str();

				// unit active (to change in game mode)
				unit->is_active = 1;
				
				// copy unit name
				strcpy_s(unit->name,sizeof(unit->name),unit->unit->name);
				auto & custom_name = cmd->parameters->at(6);
				if(custom_name.size() && custom_name.compare("-")!=0)
					strcpy_s(unit->name,sizeof(unit->name),custom_name.c_str());

				// initial action points
				unit->ResetAP();

				unit->dig_level = 0;
				unit->commander_id = rand()%9;
				unit->is_commander = rand()%2;
				unit->morale = 50 + rand()%51;

				// add unit to list
				units.push_back(unit);
			}
			else if(cmd->name.compare("AddSpecialEvent") == 0)
			{
				// --- Event definitions: AddSpecialEvent(type, position, index, probability) ---
				events->AddSpecialEvent(spelldata, def, cmd);
			}
			else if(cmd->name.compare("AddMissionObjective") == 0)
			{
				// --- Mission objectives: treating them as events too				
				events->AddMissionObjective(spelldata, def, cmd);
			}
		}
		delete mission_data;
		delete def;
		
	}

	// initialize events
	events->ResetEvents();

	// select some unit
	if(units.size())
		unit_selection = units[0];

	// initial sorting of units to map
	SortUnits();

	// clear units view range map
	PrepareUnitsViewMask();
	ClearUnitsView(true);

	// clear unit attack range
	UnitAttackRangeInit();

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


// check any unit was moved and eventually clear moved flags
int SpellMap::UnitsMoved(int clear)
{
	// scan units for placement changes and clear the move flags
	int moved = false;
	for(auto& unit : units)
	{
		if(unit->was_moved)
			moved = true;
		if(clear)
			unit->was_moved = false;
	}
	if(!isGameMode())
	{
		for(auto & evt : events->GetEvents())
		{			
			for(auto& unit : evt->units)
			{
				if(unit.unit->was_moved)
					moved = true;
				if(clear)
					unit.unit->was_moved = false;
			}
		}
	}	
	return(moved);
}

// invalidate current units view map to force recalculation (actual calculation is done while rendering for now)
void SpellMap::InvalidateUnitsView()
{
	for(auto& unit : units)
		unit->was_moved = true;
	if(!isGameMode())
	{
		for(auto& evt : events->GetEvents())
			for(auto& unit : evt->units)
				unit.unit->was_moved = true;
	}
}

// this sorts units list for proper render order, call after load or units changes
void SpellMap::SortUnits()
{	
	// clear units layer array
	Lunit.assign(x_size*y_size, NULL);

	// make full list of visible units
	auto units_list = units;
	units_list.reserve(100);
	if(!isGameMode())
	{
		// in editor mode show event untis too!
		for(auto & evt : events->GetEvents())
			for(auto & unit : evt->units)
				units_list.push_back(unit.unit);
	}

	// clear render chain pointers
	for(auto & unit : units_list)
	{
		unit->next = NULL;
		unit->assigned = 0;
	}

	// --- sort render order, ie. first unit pointer to layer array, next unit pointer to unit record themselves	
	for (int ut = 0; ut < 2; ut++)
	{		
		for (auto & new_unit : units_list)
		{
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
	if (tiles.empty())
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
int SpellMap::RenderPrepare(TScroll* scroll)
{
	if(!IsLoaded())
		return(1);

	// get surface size
	tie(surf_x, surf_y) = scroll->GetSurface();
	/*surf_x = bmp.GetWidth();
	surf_y = bmp.GetHeight();*/

	// reinit of surface changed (this is only needed when map is smaller than screen)
	if(pic && (surf_x != last_surf_x || surf_y != last_surf_y))
	{
		delete[] pic;	
		pic = NULL;
		pic_y_buffer.clear();
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
		// make y buffer
		pic_y_buffer.assign(pic_x_size,-1);
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
	if(xs_ofs + xs_size > x_size)
		xs_size = x_size - xs_ofs;
	if(ys_ofs + ys_size > y_size)
		ys_size = y_size - ys_ofs;

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

// returns currently visible tiles positions
vector<MapXY> SpellMap::GetVisibleTiles()
{
	vector<MapXY> list;
	list.reserve(x_size*y_size);
	for(int y = ys_ofs; y < ys_ofs + ys_size; y++)
		for(int x = xs_ofs; x < xs_ofs + xs_size; x++)
			list.emplace_back(x,y);
	list.shrink_to_fit();
	return(list);
}


// get random sound of visible map portion, return left-right volume
MapSound* SpellMap::GetRandomSound(double *left, double *right)
{
	// make list of candidates
	vector<MapSound*> list;
	list.reserve(sounds.size());
	for(auto & sound : sounds)
	{
		auto pos = sound.GetPosition();
		if(pos.x >= xs_ofs && pos.x < xs_ofs + xs_size && pos.y >= ys_ofs && pos.y < ys_ofs + ys_size)
			list.push_back(&sound);
	}
	if(list.empty())
		return(NULL);

	// get random sound
	auto sound = list[rand()%list.size()];
	auto pos = sound->GetPosition();
	
	// relative sound position normalized to +-1.0
	double pos_x = (double)(pos.x - (xs_ofs + xs_size/2))/(double)xs_size*2.0;
	double pos_y = (double)(pos.y - (ys_ofs + ys_size/2))/(double)ys_size*2.0;
	double dist = sqrt(pos_x*pos_x + pos_y*pos_y);

	// calc panning
	double volume = max(min(2.0 - 2.0*dist*0.7071,1.0),0.0);
	volume = volume*volume;
	double l_vol = (pos_x < 0.0)?(1.0):(1.0 - pos_x);
	double r_vol = (pos_x > 0.0)?(1.0):(1.0 + pos_x);
	l_vol = min(max(l_vol*l_vol*volume,0.0),1.0);
	r_vol = min(max(r_vol*r_vol*volume,0.0),1.0);

	if(left)
		*left = l_vol;
	if(right)
		*right = r_vol;

	return(sound);
}

// make sound loops
MapLoopSounds::MapLoopSounds(int x_size,int y_size)
{
	this->x_size = x_size;
	this->y_size = y_size;
}
MapLoopSounds::~MapLoopSounds()
{
	for(auto& snd : list)
		snd->Stop();
	for(auto& snd : list)
	{
		while(!snd->isDone())
			this_thread::sleep_for(1ms);
		delete snd;
	}
	list.clear();
}

// makes map of volumes per sound for ech tile of map, call whenever sounds are repositioned
int MapLoopSounds::UpdateMaps()
{
	// loose old maps
	for(auto& snd : maps)
		snd.clear();
	maps.clear();

	// make new maps
	for(auto& snd : list)
		maps.emplace_back(x_size*y_size,0);

	// build volume maps for each sound
	for(int sid = 0; sid < list.size(); sid++)
	{
		auto &map = maps[sid];
		auto name = list[sid]->GetSample()->name;
		for(auto& snd : sounds)
		{
			if(strcmp(snd.GetName(),name) != 0)
				continue;
			auto snd_pos = snd.GetPosition();
			for(int y = 0; y < y_size; y++)
			{
				for(int x = 0; x < x_size; x++)
				{
					double dist = MapXY(x,y).Distance(snd_pos);
					double w = (dist < 7.0)?(1.0):max(1.0 - 0.1*(dist - 7.0),0.0);
					int16_t w_int = (int16_t)(100.0*w*w);
					map[x + y*x_size] = max((int16_t)map[x + y*x_size],w_int);
				}
			}
		}
	}

	return(0);
}

// calculate volume <left,right> of given loop sound for given position in map
tuple<double,double> MapLoopSounds::GetViewVolume(int index,int xs_ofs,int ys_ofs,int xs_size,int ys_size)
{
	if(index >= maps.size())
		return tuple(0.0, 0.0);
	auto map = maps[index];

	// get mean sound position (w. average by volume)
	MapXY pos(0,0);
	int count = 0;
	for(int y = ys_ofs; y < ys_ofs + ys_size; y++)
		for(int x = xs_ofs; x < xs_ofs + xs_size; x++)
		{					
			int w = (int)map[x + y*x_size];
			pos.x += w*x;
			pos.y += w*y;
			count += w;
		}
	if(!count)
		return tuple(0.0,0.0);
	pos.x /= count;
	pos.y /= count;

	// relative sound position normalized to +-1.0
	double pos_x = (double)(pos.x - (xs_ofs + xs_size/2))/(double)xs_size*2.0;
	double pos_y = (double)(pos.y - (ys_ofs + ys_size/2))/(double)ys_size*2.0;
	double dist = sqrt(pos_x*pos_x + pos_y*pos_y);

	// calc panning
	double volume = max(min(2.0 - 2.0*dist*0.7071,1.0),0.0);
	volume = volume*volume;
	double l_vol = (pos_x < 0.0)?(1.0):(1.0 - pos_x);
	double r_vol = (pos_x > 0.0)?(1.0):(1.0 + pos_x);
	l_vol = min(max(l_vol*l_vol*volume,0.0),1.0);
	r_vol = min(max(r_vol*r_vol*volume,0.0),1.0);
	
	// cutoff limit
	if(l_vol < 0.001 && r_vol < 0.001)
		return tuple(0.0, 0.0);

	return tuple(l_vol,r_vol);		
}

// start/stop/update volumes of loop ambient sounds
int MapLoopSounds::UpdateVolumes(int xs_ofs,int ys_ofs,int xs_size,int ys_size)
{
	for(int k = 0; k < list.size(); k++)
	{
		auto snd = list[k];
		
		auto [left_vol, right_vol] = GetViewVolume(k, xs_ofs,ys_ofs,xs_size,ys_size);
		snd->SetPanning(left_vol,right_vol);

		if(left_vol > 0.0 && right_vol > 0.0)
		{
			if(snd->isDone())
				snd->Play(false, true);
		}
		else
		{
			if(!snd->isDone())
				snd->Stop();
		}		
	}
	return(0);
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
vector<MapXY> &SpellMap::GetSelections(TScroll *scroll)
{
	int m,n,i,j;
		
	// check & fix surface and scroll ranges
	if(!RenderPrepare(scroll))
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
					auto *tile = &tiles[(m + ys_ofs * 2) * x_size + n + xs_ofs];
					Sprite* sid = tile->L1;
					int sof = tile->elev;

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
MapXY SpellMap::GetSelection(TScroll* scroll)
{
	vector<MapXY>& sel = GetSelections(scroll);
	if(sel.size())
		return(sel[0]);
	else
		return(MapXY());
}
MapXY SpellMap::GetSelection()
{
	if(msel.empty() || !msel[0].IsSelected())
		return(MapXY());
	else
		return(msel[0]);
}

// get selection elevation
int SpellMap::GetElevation(TScroll* scroll)
{
	// fix selection
	MapXY sel = GetSelection(scroll);
	if(sel.IsSelected())
	{
		return((tiles[ConvXY(sel)].elev));
	}
	else
		return(0);	
}
// get L1 terrain tile name
char *SpellMap::GetL1tileName(TScroll* scroll)
{
	// fix selection
	MapXY sel = GetSelection(scroll);

	if(sel.IsSelected())
	{
		// return tile name
		return(tiles[ConvXY(sel)].L1->name);
	}
	else
		return(NULL);
}

// get L2 object tile name
char* SpellMap::GetL2tileName(TScroll* scroll)
{
	// fix selection
	MapXY sel = GetSelection(scroll);

	if(sel.IsSelected())
	{
		// return tile name
		return(tiles[ConvXY(sel)].L2->name);
	}
	else
		return(NULL);
}

// get map flag array's object height
int SpellMap::GetFlagHeight(MapXY *sel)
{
	return((tiles[ConvXY(sel)].flags >> 4)&0x0F);
}
// get map flag array's flag part
int SpellMap::GetFlagFlag(MapXY* sel)
{
	return((tiles[ConvXY(sel)].flags)&0x0F);
}
// get flags for given tile (###todo: decode what those flags actually means?)
tuple<int,int,int> SpellMap::GetTileFlags(TScroll* scroll)
{
	// fix selection
	MapXY sel = GetSelection(scroll);
	
	if(sel.IsSelected())
	{
		// return tile name
		return {GetFlagFlag(&sel), GetFlagHeight(&sel), tiles[ConvXY(sel)].flags};
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
			spr = tiles[ConvXY(selection[k])].L1;
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
			spr = tiles[ConvXY(selection[k])].L2;
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
			spr = tiles[ConvXY(selection[k])].flags;
		list.push_back(spr);
	}
	return(list);
}

// get rect portion of last rendered buffer (indexed map color)
int SpellMap::GetRender(uint8_t* buf, int x_size, int y_size, int x_pos, int y_pos)
{
	// clear buffer
	memset(buf, 0x00, x_size*y_size);

	// copy visible part of surface only
	for(int y = 0; y < y_size; y++)
	{
		int ys = y + y_pos;		
		if(ys < 0 || ys >= surf_y)
			continue;
		
		int x = 0;
		if(x_pos < 0)
			x = -x_pos;
		int xn = x_size - x;

		int xs = 0;
		if(x_pos > 0)
			xs = x_pos;
		xn = min(surf_x - xs, xn);
				
		uint8_t* src = &pic[surf_x_origin + xs + (surf_y_origin + ys)*pic_x_size];
		uint8_t* dst = &buf[x + y*x_size];
		memcpy(dst, src, xn);
	}
	
	return(0);
}

// get gamma corrected palette
uint8_t* SpellMap::GetPalette()
{
	return(&pal[0][0]);
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
	if(RenderPrepare(scroll))
		return(1);

	// find selection tiles
	auto msel = GetSelections(scroll);
	MapXY cursor;
	if(msel.size() && msel[0].IsSelected())
		cursor = msel[0];

	// clear used surface range
	for(int y = 0; y < min(surf_y,pic_y_size-surf_y_origin); y++)
	{
		uint8_t* src = &pic[surf_x_origin + (surf_y_origin + y)*pic_x_size];		
		std::memset((void*)src,MAP_BACK_COLOR,min(surf_x,pic_x_size - surf_x_origin));
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
	int units_moved = UnitsMoved(true);

	// check if unit selection changed
	int unit_changed = UnitChanged(true);

	// sort units positions for rendering
	if(units_moved)
		SortUnits();

	if(units_moved && game_mode)
	{
		// update view mask (this should be probably somewhere else?)
		ClearUnitsView(false);
		AddUnitsView();				
	}	
	
	MapUnit* unit = GetSelectedUnit();	
	if((units_moved || unit_changed || !unit) && !unit->isMoving())
	{
		// initialize unit view recalculation
		FindUnitRange(unit);

		// calculate attack range
		CalcUnitAttackRange(unit);
	}

	// check unit on cursor
	MapUnit* cursor_unit = GetCursorUnit(scroll);

	// lock stuff while rendering
	FindUnitRangeLock(true);
	LockMap();
	
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
			uint8_t *fil = GetUnitRangeFilter(mxy);
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
			Sprite* sid = tiles[mxy].L1;
			char sn = sid->name[2];
			int sof = tiles[mxy].elev;

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
			MapLayer3* anm = &L3[i];

			// skip if not in visible area
			if (anm->x_pos < xs_ofs || anm->x_pos >= (xs_ofs + xs_size) || anm->y_pos < ys_ofs * 2 || anm->y_pos >= (ys_ofs * 2 + ys_size))
				continue;
			int mxy = ConvXY(anm->x_pos, anm->y_pos);

			// game mode view range
			uint8_t* fil = GetUnitRangeFilter(mxy);
			if(game_mode && units_view[mxy] == 0)
				continue;
			else if(game_mode && units_view[mxy] == 1)
				fil = terrain->filter.darkpal;

			// L1 elevation
			int sof = tiles[mxy].elev;

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
				uint8_t* fil = GetUnitRangeFilter(mxy);
				if(game_mode && units_view[mxy] == 0)
					continue;
				else if(game_mode && units_view[mxy] == 1)
					fil = terrain->filter.darkpal;

				// L1 elevation
				int y_elev = tiles[mxy].elev;

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
			Sprite *spr = tiles[mxy].L1;
			int sof = tiles[mxy].elev;
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
			uint8_t* fil = GetUnitRangeFilter(mxy);
			if(game_mode && units_view[mxy] == 0)
				continue;
			else if(game_mode && units_view[mxy] == 1)
				fil = terrain->filter.darkpal;

			// get map tile
			Sprite *sid = tiles[mxy].L1;
			int sof = tiles[mxy].elev;

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
			MapLayer4* pnm = &L4[i];

			// skip if not in visible area
			if (pnm->x_pos < xs_ofs || pnm->x_pos >= (xs_ofs + xs_size) || pnm->y_pos < ys_ofs * 2 || pnm->y_pos >= (ys_ofs * 2 + ys_size))
				continue;
			int mxy = ConvXY(pnm->x_pos, pnm->y_pos);

			// game mode view range
			uint8_t* fil = GetUnitRangeFilter(mxy);
			if(game_mode && units_view[mxy] == 0)
				continue;
			else if(game_mode && units_view[mxy] == 1)
				fil = terrain->filter.darkpal;

			// L1 elevation
			int y_elev = tiles[mxy].elev;
			
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
	MapXY attack_pos;
	MapXY target_pos;
	MapUnit *attacker = NULL;
	SpellUnitRec *target = NULL;
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
			uint8_t* filter = GetUnitRangeFilter(mxy);
			if(!TileIsVisible(n + xs_ofs,m + ys_ofs*2))
				filter = terrain->filter.darkpal;			

			// tile selected
			int is_selected = (msel.size() && msel[0].IsSelected() && msel[0].x - xs_ofs == n && msel[0].y - ys_ofs*2 == m);

			// first unit to render
			MapUnit* unit = Lunit[mxy];			
			// L1 tile
			MapSprite *tile = &tiles[mxy];
			Sprite* sid = tile->L1;
			int sof = tile->elev;
			char slope = sid->name[2];
			// L2 tile
			Sprite* sid2 = tile->L2;

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
					if ((pass == 0 && unit->unit->isAir()) || pass == 2)
					{
						// render unit
						uint8_t* filter_unit = NULL;
						if(cursor_unit == unit && !game_mode)
							filter_unit = terrain->filter.goldpal;
						int top_y_ofs = unit->Render(terrain, pic, pic_end, mxx, myy, pic_x_size,filter_unit, filter, sid, w_unit_hud);
						
						// store attack-target positions in buffer (for projectile trajectory calculation)
						if(unit->isAttacker())
						{
							attacker = unit;
							attack_pos = MapXY(mxx + unit->attack_fire_x_org, myy + unit->attack_fire_y_org + sid->y_ofs);
						}
						if(unit->isTarget())
						{
							target_pos = MapXY(mxx + 40, myy + sid->y_ofs + sid->y_size/2);
							target = unit->unit;
						}

						// render fire PNM animation
						if(unit->attack_state == MapUnit::ATTACK_STATE::FLIGHT && unit->attack_fire_pnm)
						{
							auto *pnm = unit->attack_fire_pnm;
							auto *frame = pnm->frames[unit->attack_fire_frame];
							int org_x = mxx + unit->attack_fire_x_org - (pnm->x_max + pnm->x_min)/2;
							int org_y = myy + unit->attack_fire_y_org - (pnm->y_max + pnm->y_min)/2;
							frame->Render(pic,pic_end,org_x,org_y,pic_x_size,filter);
						}

						// render hit PNM animation
						if(unit->attack_state == MapUnit::ATTACK_STATE::HIT && unit->attack_hit_pnm)
						{
							auto *frame = unit->attack_hit_pnm->frames[unit->attack_hit_frame];
							frame->Render(pic,pic_end,mxx,myy,pic_x_size,filter);
						}						

						// render selection pointer for selected unit
						if(unit_selection && unit_selection == unit)
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

				// render tile/object hit PNM animation
				if(wL2 && pass == 2 && tile->hit_pnm)
				{
					auto* frame = tile->hit_pnm->frames[tile->hit_pnm_frame];
					frame->Render(pic,pic_end,mxx,myy,pic_x_size,filter);
				}

				// mark target tile position
				if(tile->is_target)
					target_pos = MapXY(mxx + 40,myy + sid->y_ofs + sid->y_size/2);
			}
		}
	}

	// render projectile
	if(attacker && attacker->attack_state == MapUnit::ATTACK_STATE::FLIGHT && attack_pos.IsSelected() && target_pos.IsSelected() && attacker->unit->hasProjectile(target))
	{
		double angle =  unit->unit_angle;

		auto glyph = attacker->unit->projectile->GetGlyph(angle);
		
		int dx = target_pos.x - attack_pos.x;
		int dy = target_pos.y - attack_pos.y;
		int mxx = attack_pos.x + dx*attacker->attack_proj_step/attacker->attack_proj_delay - glyph->x_size/2;
		int myy = attack_pos.y + dy*attacker->attack_proj_step/attacker->attack_proj_delay - glyph->y_size/2;
			
		glyph->Render(pic, pic_end, pic_x_size, mxx, myy);
	}
		

	
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
		Sprite* mspr = tiles[ConvXY(mxy)].L1;
		int m_y_elev = tiles[ConvXY(mxy)].elev;
		Sprite* nspr = tiles[ConvXY(nxy)].L1;
		int n_y_elev = tiles[ConvXY(nxy)].elev;
		
		// line ends
		int mxx = (mxy.x - xs_ofs)*80 + ((((mxy.y - ys_ofs*2) & 1) != 0) ? 0 : 40) + mspr->x_size/2;
		int myy = (mxy.y - ys_ofs * 2) * 24 - m_y_elev*18 + MSYOFS + 50 + mspr->y_ofs + mspr->y_size/2;
		int nxx = (nxy.x - xs_ofs)*80 + ((((nxy.y - ys_ofs*2) & 1) != 0) ? 0 : 40) + nspr->x_size/2;
		int nyy = (nxy.y - ys_ofs * 2) * 24 - n_y_elev*18 + MSYOFS + 50 + nspr->y_ofs + nspr->y_size/2;

		plot_line(pic, pic_end, 0, 0, pic_x_size, 252, mxx,myy, nxx,nyy);
	}


	// --- Redner Layer 7+8 sound marks ---
	vector<MapSound> *sound_groups[] = {&sounds, &sound_loops->sounds};
	bool sound_enabled[] = {wSound, wSoundLoop};
	for(int sid = 0; sid < 2; sid++)
	{
		terrain->font->SetFilter(terrain->filter.darker);

		if(sound_enabled[sid])
		{
			auto sound_list = sound_groups[sid];
			for(auto& sound : *sound_list)
			{
				auto pos = sound.GetPosition();

				// skip if not in visible area
				if(pos.x < xs_ofs || pos.x >= (xs_ofs + xs_size) || pos.y < ys_ofs * 2 || pos.y >= (ys_ofs * 2 + ys_size))
					continue;
				int mxy = ConvXY(pos);

				// get tile
				Sprite* spr = tiles[mxy].L1;
				int sof = tiles[mxy].elev;

				// render sprite
				n = pos.x - xs_ofs;
				m = pos.y - ys_ofs*2;
				int mxx = n * 80 + (((m & 1) != 0) ? 0 : 40);
				int myy = m * 24 - sof * 18 + MSYOFS + 50;

				int color = 252;
				string glyph = "\x1F";
				if(sid)
				{
					glyph += " \x1E";
					color = 214;//217;
				}
				terrain->font->Render(pic,pic_end,pic_x_size,mxx,myy + spr->y_ofs - 7,80,spr->y_size,glyph,color,254,SpellFont::SOLID);
				string hstr = string(sound.GetName());
				terrain->font->Render(pic,pic_end,pic_x_size,mxx,myy + spr->y_ofs + 7,80,spr->y_size,hstr,color,254,SpellFont::SOLID);
			}
		}
	}
	

	// --- Events:
	if(wEvents)
	{
		terrain->font->SetFilter(terrain->filter.darker);
		terrain->font7->SetFilter(terrain->filter.darker);
		
		// render event-unit connection lines:
		for(auto & evt : events->GetEvents())
		{
			MapXY ev_pos;
			if(evt->isSeePlace())
				ev_pos = evt->position;
			else if(evt->isSeeUnit() && evt->trig_unit)
				ev_pos = evt->trig_unit->coor;
			else 
				continue;

			// event position in surface
			Sprite* spr = tiles[ConvXY(ev_pos)].L1;
			auto [mxx,myy] = GetSurfPos(ev_pos);
			mxx += 40;
			myy += spr->y_ofs + spr->y_size/2;

			for(auto & unit : evt->units)
			{
				// unit position in surface
				Sprite* spr = tiles[ConvXY(unit.unit->coor)].L1;
				auto [mxx2,myy2] = GetSurfPos(unit.unit->coor);
				mxx2 += 40;
				myy2 += spr->y_ofs + spr->y_size/2;
				if(unit.unit->unit->isAir())
					myy2 -= SpellUnitRec::AIR_UNIT_FLY_HEIGHT;
				
				int is_selected = (cursor_unit == unit.unit || evt == GetSelectEvent());

				const struct {int x;int y;int color;} ofs[] = {{0,0,252},{0,1,253},{0,-1,253},{1,0,253},{-1,0,253}};
				for(int k = (is_selected*4); k >= 0 ; k--)
					plot_line(pic,pic_end,0,0,pic_x_size, ofs[k].color, mxx+ofs[k].x,myy+ofs[k].y, mxx2+ofs[k].x,myy2+ofs[k].y);
			}
		}		
		
		auto start_evts = events->GetMissionStartEvent();

		// render event marks:
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

				// get tile
				Sprite* spr = tiles[mxy].L1;
				int sof = tiles[mxy].elev;
				int mxx = n * 80 + (((m & 1) != 0) ? 0 : 40);
				int myy = m * 24 - sof * 18 + MSYOFS + 50 + spr->y_ofs;

				if(events->CheckEvent(mxy))
				{	
					vector<string> labels;
					auto list = events->GetEvents(mxy);
					int is_selected = false;
					for(auto & evt : list)
					{										
						labels.push_back("\x1C" + evt->type_name);
						if(evt->probability != 100)
							labels.back() += string_format("(%d%%)",evt->probability);
						if(!evt->units.empty())
							labels.back() += "\x1A";
						if(!evt->texts.empty())
							labels.back() += "\x1B";
						is_selected |= (GetSelectEvent() == evt && !evt->in_placement);
					}

					int color = (is_selected && sel_blink_state)?214:252;
					terrain->font->Render(pic,pic_end,pic_x_size,mxx,myy,80,spr->y_size,labels,color,254,SpellFont::SOLID);
				}
				else
				{
					// show events associated to units (MissionStart and SeeUnit):

					auto unit = Lunit[mxy];
					while(unit)
					{
						if(unit->map_event && unit->map_event->isMissionStart())
						{
							// found matching MissionStart() event:
							auto evt = unit->map_event;
							int is_selected = (evt == GetSelectEvent());

							// make label on top of unit
							string label = "?" + evt->type_name;
							if(evt->probability != 100)
								label += string_format("(%d%%)",evt->probability);
							//label += "\x1A";											

							// plot it
							int y_ofs = (unit->unit->isAir())?(-SpellUnitRec::AIR_UNIT_FLY_HEIGHT):0;
							int color = (is_selected && sel_blink_state)?214:252;
							terrain->font7->Render(pic,pic_end,pic_x_size,mxx,myy - y_ofs,80,spr->y_size,label,color,254,SpellFont::SOLID);
						}
						else if(unit->trig_event && unit->trig_event->isSeeUnit())
						{
							// SeeUnit() event:
							auto evt = unit->trig_event;
							int is_selected = (evt == GetSelectEvent());

							// make label on top of unit
							string label = "\x1C" + evt->type_name;
							if(evt->probability != 100)
								label += string_format("(%d%%)",evt->probability);
							if(!evt->units.empty())
								label += "\x1A";
							if(!evt->texts.empty())
								label += "\x1B";

							// plot it
							int y_ofs = (unit->unit->isAir())?(-SpellUnitRec::AIR_UNIT_FLY_HEIGHT):0;
							int color = (is_selected && sel_blink_state)?214:252;
							terrain->font->Render(pic,pic_end,pic_x_size,mxx,myy - y_ofs,80,spr->y_size,label,color,254,SpellFont::SOLID);
						}

						unit = unit->next;
					}
				}


				
			}
		}
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

			if(!GetUnitRangeFilter(mxy))
				continue;

			// get view height map elevation
			int ap_left = unit_ap_left[mxy];
			int fire_left = unit_fire_left[mxy];
			if(fire_left < 1)
				continue;
			string hstr = string_format("%c %d",29,fire_left);

			// get tile
			Sprite* spr = L1[mxy];
			int sof = elev[mxy];

			// render sprite
			int mxx = n * 80 + (((m & 1) != 0) ? 0 : 40);
			int myy = m * 24 - sof * 18 + MSYOFS + 50;

			//terrain->font7->Render(pic,pic_end,pic_x_size,mxx,myy + spr->y_ofs,80,spr->y_size,hstr,252,254,SpellFont::DIAG);
			terrain->font7->Render(pic,pic_end,pic_x_size,mxx,myy + spr->y_ofs,80,spr->y_size,hstr,252,254,SpellFont::RIGHT_DOWN);
		}
	}*/

	// put unit range view mode string (game mode)
	string unit_view_string;
	if(viewingUnitMoveRange())
		unit_view_string = "MOVEMENT MODE";
	else if(viewingUnitAttackRange())
		unit_view_string = "ATTACK MODE";
	if(!unit_view_string.empty())
		terrain->font->Render(pic,pic_end,pic_x_size,surf_x_origin+surf_x-150,surf_y_origin+0,150,50,unit_view_string,255,254,SpellFont::RIGHT_DOWN);
	
	
	
		
	// render HUD
	RenderHUD(pic, pic_end, pic_x_size, &cursor, cursor_unit, hud_buttons_cb);


	// unlock stuff while rendering
	FindUnitRangeLock(false);
	ReleaseMap();
	
	
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

tuple<int,int> SpellMap::GetSurfPos(MapXY& pos)
{
	// relative pos in visibel part of surface
	int x_pos = pos.x - xs_ofs;
	int y_pos = pos.y - ys_ofs*2;
	auto mxy = ConvXY(pos);
	Sprite* spr = tiles[mxy].L1;
	int sof = tiles[mxy].elev;
	int mxx = x_pos * 80 + (((y_pos & 1) != 0) ? 0 : 40);
	int myy = y_pos * 24 - sof * 18 + MSYOFS + 50;
	return tuple(mxx,myy);
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

	if(!unit_range_th)
	{
		unit_range_th_control = UNIT_RANGE_TH_IDLE;
		unit_range_th = new thread(&SpellMap::FindUnitRange_th, this);
	}

	return(0);
}


// locks/unlocks data related to unit range
int SpellMap::FindUnitRangeLock(bool state)
{
	if(state)
		unit_range_th_lock.lock();
	else
		unit_range_th_lock.unlock();
	return(0);
}

// find walkable range of unit
int SpellMap::FindUnitRange(MapUnit* unit)
{
	if(unit_range_th_control != UNIT_RANGE_TH_IDLE)
	{
		// still running: terminate task before new task
		unit_range_th_control = UNIT_RANGE_TH_STOP;
		while(unit_range_th_control != UNIT_RANGE_TH_IDLE)
			this_thread::sleep_for(1ms);
	}
	
	// set new unit
	if(unit)
		unit_range_th_unit = MapUnit(*unit);
	else
		unit_range_th_unit.coor = {-1,-1};
	
	// start calculation
	unit_range_th_control = UNIT_RANGE_TH_RUN;

	return(1);
}



// find walkable range of unit thread
int SpellMap::FindUnitRange_th()
{		
	// target unit
	MapUnit* unit = &unit_range_th_unit;

	// clear range filters
	vector<int> unit_ap_left;
	vector<int> unit_fire_left;
	
	while(unit_range_th_control != UNIT_RANGE_TH_EXIT)
	{		
		if(unit_range_th_control != UNIT_RANGE_TH_RUN)
		{
			// chill out till caller wants to do stuff..
			unit_range_th_control = UNIT_RANGE_TH_IDLE;
			this_thread::sleep_for(1ms);
			continue;
		}

		// processed tiles flags
		vector<int> done(x_size*y_size,false);

		// clear range filters
		unit_ap_left.assign(x_size*y_size,-1);
		unit_fire_left.assign(x_size*y_size,-1);

		if(!unit->coor.IsSelected() || unit->radar_up || !unit->isActive())
		{
			FindUnitRangeLock(true);
			this->unit_ap_left = unit_ap_left;
			this->unit_fire_left = unit_fire_left;
			FindUnitRangeLock(false);
			unit_range_th_control = UNIT_RANGE_TH_IDLE;
			continue;
		}

		// recoursive tiles search buffer
		vector<int> dirz;
		vector<MapXY> posz;
		dirz.reserve(500);
		posz.reserve(500);

		// initial tile
		dirz.push_back(-1);
		posz.push_back(unit->coor);
		done[ConvXY(unit->coor)] = true;

		// recoursively expand move range untill no path is found
		while(true)
		{		
			// leave if run no longer needed
			if(unit_range_th_control != UNIT_RANGE_TH_RUN)
				break;
			
			dirz.back()++;
			if(dirz.back() >= 4)
			{
				// this tile is done
				dirz.pop_back();
				posz.pop_back();
				if(dirz.empty())
				{
					// all done: store range data atomically
					
					FindUnitRangeLock(true);
					this->unit_ap_left = unit_ap_left;
					this->unit_fire_left = unit_fire_left;
					FindUnitRangeLock(false);
					unit_range_th_control = UNIT_RANGE_TH_IDLE;

					break;
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

			// skip if unit in path
			auto tile_unit = Lunit[ConvXY(neig_pos)];
			int no_pasaran = false;
			while(tile_unit)
			{				
				if(unit->unit->isAir() && tile_unit->unit->isAir() || !unit->unit->isAir() && !tile_unit->unit->isAir())
				{
					no_pasaran = true;
					break;
				}
				tile_unit = tile_unit->next;
			}
			if(no_pasaran)
				continue;


			// get tile infoz
			Sprite* spr = tiles[ConvXY(neig_pos)].L1;
			int flag = tiles[ConvXY(neig_pos)].flags;
			//int class_flags = spr->GetFlags();
			//int slope = spr->GetSlope();
			
			int is_bridge = (flag == 0x70);
			int is_forest = (flag == 0x90);
			int is_object = flag && !is_bridge && !is_forest; // any obstacle but trees
			int is_obstacle = flag && !is_bridge; // any obstacle
			int is_hoverable = !is_obstacle || (flag == 0x60); // can hover over

			// skip invalid targets to save time
			if(unit->unit->isHover())
			{
				if(!is_hoverable)
					continue;
			}
			else if(!unit->unit->isAir() && !unit->unit->usingTeleportMove())
			{
				if(is_object)
					continue;
				if(is_forest && !unit->unit->isWalk())
					continue;
			}

			// try to get path to target tile
			auto path = FindUnitPath(unit, neig_pos);

			if(path.empty())
				continue;

			// mark tiles accessible			
			if(!unit->unit->usingTeleportMove() || !is_obstacle)
			{
				int ap_left = unit->action_points - path.back().g_cost;
				int fire_count = unit->GetFireCount(ap_left);
				unit_ap_left[ConvXY(neig_pos)] = ap_left;
				unit_fire_left[ConvXY(neig_pos)] = fire_count;
			}

			// path found and all tests ok: step forward in recursion
			dirz.push_back(-1);
			posz.push_back(neig_pos);
		}
	}

	return(0);
}

struct FindUnitPathCompareHeap
{
	bool operator()(AStarNode*& b,AStarNode*& a) const
	{		
		return((a->f_cost < b->f_cost || a->g_cost == b->g_cost && a->h_cost < b->h_cost));
	}
};

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
	push_heap(open_set.begin(),open_set.end(),FindUnitPathCompareHeap());

	// unit maximum AP
	int max_ap = unit->action_points;

	int count = 0;

	while(open_set.size())
	{
		// look for lowest cost node	
		pop_heap(open_set.begin(),open_set.end(),FindUnitPathCompareHeap());
		AStarNode* current_node = open_set.back();
		open_set.pop_back();		

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
		Sprite *current_spr = tiles[ConvXY(current_node->pos)].L1;
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

			// skip if unit in path
			auto tile_unit = Lunit[ConvXY(n_pos)];
			int no_pasaran = false;
			while(tile_unit)
			{
				if(unit->unit->isAir() && tile_unit->unit->isAir() || !unit->unit->isAir() && !tile_unit->unit->isAir())
				{
					no_pasaran = true;
					break;
				}
				tile_unit = tile_unit->next;
			}
			if(no_pasaran)
				continue;

			// basic AP/step
			double ap_step_w = (double)unit->GetWalkAP();
			// finite AP/step
			double ap_step = ap_step_w;

			// tile flags
			int flag = tiles[ConvXY(n_pos)].flags;

			int is_bridge = (flag == 0x70);
			int is_forest = (flag == 0x90);
			int is_object = flag && !is_bridge && !is_forest; // any obstacle but trees
			int is_obstacle = flag && !is_bridge; // any obstacle
			int is_hoverable = !is_obstacle || (flag == 0x60); // can hover over			

			// get L1 sprite
			Sprite* spr = tiles[ConvXY(n_pos)].L1;
			int class_flags = spr->GetFlags();
			int slope = spr->GetSlope();
			
			if(unit->unit->isHover())
			{
				// hover unit
				if(!is_hoverable)
					continue;
				
				// apply slope penalty
				if(slope != 'A' && current_slope != 'A')
					ap_step = (ap_step_w*11.0/9.0);
			}
			else if(!unit->unit->isAir() && !unit->unit->usingTeleportMove())
			{
				if(is_object)
					continue;
				
				if(unit->unit->isWalk() && is_forest)
					ap_step = (ap_step_w*10.0/9.0);
				else if(is_forest)
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
				push_heap(open_set.begin(),open_set.end(),FindUnitPathCompareHeap());
			}
		}		
	}

	// fail: return empty path
	vector<AStarNode> path;
	return(path);
}

// get filter pointer or NULL based for the tile
uint8_t* SpellMap::GetUnitRangeFilter(int mxy)
{
	if(viewingUnitMoveRange() && unit_fire_left[mxy] > 0)
		return(terrain->filter.bluepal);
	if(viewingUnitMoveRange() && unit_ap_left[mxy] >= 0)
		return(terrain->filter.dbluepal);
	if(viewingUnitAttackRange() && unit_attack_map[mxy] > 0)
		return(terrain->filter.redpal);
	if(viewingUnitMoveRange() || viewingUnitAttackRange())
		return(terrain->filter.darkpal);
	return(NULL);
}

// set map view mode (none, move range, fire range)
int SpellMap::SetUnitRangeViewMode(int mode)
{
	int old_state = unit_range_view_mode;

	if(mode == UNIT_RANGE_INCREMENT)
	{
		// cycle through mode only
		unit_range_view_mode++;
		if(unit_range_view_mode > UNIT_RANGE_ATTACK)
			unit_range_view_mode = UNIT_RANGE_NONE;
		
		// explicit selection lock if not NONE state
		unit_range_view_mode_lock = (unit_range_view_mode != UNIT_RANGE_NONE);
	}
	else if(!unit_range_view_mode_lock)
	{
		// explicit setup
		unit_range_view_mode = mode;
		
	}
	return(old_state);
}


// check if unit can move to target position
int SpellMap::CanUnitMove(MapXY target)
{
	if(!target.IsSelected())
		return(false);
	int npos = ConvXY(target);
	
	// selected unit
	auto* unit = GetSelectedUnit();
	if(!unit)
		return(false);

	// runtime created unit cannot move until activated (next turn)
	if(!unit->isActive())
		return(false);
	
	// deployed radar cannot move
	if(unit->radar_up)
		return(false);

	// check calculated move range
	if(unit_ap_left[npos] < 0)
		return(false);

	// check if target position is not blocked?
	auto other = Lunit[npos];	
	while(other)
	{
		if(unit->unit->isAir() == other->unit->isAir() || unit->unit->isLand() == other->unit->isLand())
			return(false);
		other = other->next;
	}

	// ok, it seems target tile is available	
	return(true);
}

// move unit (in game mode)
int SpellMap::MoveUnit(MapXY target)
{	
	// selected unit
	auto* unit = GetSelectedUnit();
	if(!unit)
		return(1);

	if(unit->radar_up)
		return(1);

	// clear last move
	LockMap();
	unit->move_state = MapUnit::MOVE_STATE::IDLE;
	unit->move_nodes.clear();
	ReleaseMap();
	
	//  check target
	if(!target.IsSelected())
		return(1);
	int t_mxy = ConvXY(target);
	
	// skip if outside move range
	if(unit_ap_left[t_mxy] < 0)
		return(1);
	
	// find path
	auto path = FindUnitPath(unit, target);
	
	LockMap();

	// build path
	for(int nid = 1; nid < path.size(); nid++)
		unit->move_nodes.push_back(path[nid]);

	// init path move
	if(!unit->move_nodes.empty())
	{
		unit->move_state = MapUnit::MOVE_STATE::TURRET;
		unit->move_step = 0;
		
		unit->ClearDigLevel();
		unit->ResetTurnsCounter();
	}

	ReleaseMap();
		
	return(0);
}


// get unit action options for cursor position (game mode)
int SpellMap::GetUnitOptions(TScroll* scroll)
{	
	auto select_pos =GetSelection(scroll);
	int options = 0;

	if(unit_selection && CanUnitAttackLand(select_pos))
	{
		// can attack land unit
		options |= UNIT_OPT_LOWER;
	}
	if(unit_selection && CanUnitAttackAir(select_pos))
	{
		// can attakc air unit
		options |= UNIT_OPT_UPPER;
	}
	if(unit_selection && CanUnitAttackObject(select_pos))
	{
		// can attack object
		options |= UNIT_OPT_LOWER;
	}
	if(select_pos.IsSelected() && CanSelectUnit(select_pos))
	{
		// try select unit (if on cursor)
		options |= UNIT_OPT_SELECT;
	}
	if(select_pos.IsSelected() && CanUnitMove(select_pos))
	{
		// game mode: move unit
		options |= UNIT_OPT_MOVE;
	}

	return(options);
}




// reset AP of all units
int SpellMap::ResetUnitsAP()
{
	for(auto & unit : units)
		unit->ResetAP();
	return(0);
}

// attack target (unit or object)
int SpellMap::Attack(MapXY pos, int prefer_air)
{
	if(!pos.IsSelected())
		return(1);
	auto pxy = ConvXY(pos);
	
	// try select unit target
	MapUnit *target = NULL;
	if(prefer_air && CanUnitAttackAir(pos))
		target = CanUnitAttackAir(pos);		
	else if(!prefer_air && CanUnitAttackLand(pos))
		target = CanUnitAttackLand(pos);
	else if(CanUnitAttackLand(pos))
		target = CanUnitAttackLand(pos);
	else if(CanUnitAttackAir(pos))
		target = CanUnitAttackAir(pos);
	else if(!CanUnitAttackObject(pos))
		return(1);
		
	// get attacking unit
	auto* unit = GetSelectedUnit();
	if(!unit)
		return(1);

	unit->attack_target = NULL;
	unit->attack_target_obj.Clear();

	if(target && unit->unit->isActionKamikaze())
	{
		// --- kamikaze atack is processed via special actions		
		unit->attack_target = target;
		unit->altitude = 100;
		unit->action_state = MapUnit::ACTION_STATE::KAMIKAZE;

		// play attack sound (if exist)
		unit->PlayFire(target);

		unit->ResetTurnsCounter();
		return(0);
	}

	// --- attack unit or object
	if(target)
		unit->attack_target = target;
	else
		unit->attack_target_obj = pos;
	int frame_stop;
	FSU_resource* fsu_anim = unit->GetShotAnim(target,&frame_stop);		
	unit->in_animation = fsu_anim;
	unit->frame = 0;
	unit->frame_stop = frame_stop;
	unit->attack_state = MapUnit::ATTACK_STATE::DIR;

	unit->ResetTurnsCounter();
	return(0);
}
/*int SpellMap::AttackUnit(MapUnit *target)
{
	// get target position
	if(!target)
		return(1);
	auto target_pos = target->coor;
	int target_mxy = ConvXY(target_pos);	
	
	// skip if out of range
	if(!unit_attack_map[target_mxy])
		return(1);

	// get attacking unit
	auto* unit = GetSelectedUnit();
	if(!unit)
		return(1);

	// check fires count
	int fires = unit->GetFireCount();
	if(!fires)
		return(1);
	
	if(unit->unit->isActionKamikaze())
	{
		// kamikaze atack is processed via special actions
		unit->attack_target = target;
		unit->altitude = 100;
		unit->action_state = MapUnit::ACTION_STATE::KAMIKAZE;

		// play attack sound (if exist)
		unit->PlayFire(target);

		unit->ResetTurnsCounter();
		return(0);
	}
			
	// play FSU animation	
	int frame_stop;
	FSU_resource* fsu_anim = unit->GetShotAnim(target,&frame_stop);
	unit->attack_target = target;
	unit->in_animation = fsu_anim;
	unit->frame = 0;		
	unit->frame_stop = frame_stop;
	unit->attack_state = MapUnit::ATTACK_STATE_DIR;

	unit->ResetTurnsCounter();
	return(0);
}*/


// check if some selectable unit is at postion (game mode only!)
MapUnit* SpellMap::CanSelectUnit(MapXY pos)
{
	if(!isGameMode())
		return(NULL);
	if(!pos.IsSelected())
		return(NULL);
	auto pxy = ConvXY(pos);

	// get candidates (alinace only)
	MapUnit *land_unit = NULL;
	MapUnit *air_unit = NULL;
	auto unit = Lunit[pxy];
	while(unit)
	{
		if(unit->unit->isLand() && !unit->is_enemy && unit != unit_selection)
			land_unit = unit;
		if(unit->unit->isAir() && !unit->is_enemy && unit != unit_selection)
			air_unit = unit;
		unit = unit->next;
	}
	
	// pick some with selected preference
	if(unit_sel_land_preference && land_unit)
		return(land_unit);
	if(!unit_sel_land_preference && air_unit)
		return(air_unit);
	if(land_unit)
		return(land_unit);
	if(air_unit)
		return(air_unit);
	return(NULL);
}

// select unit, if currently selected at the same position, switch between air-land
MapUnit* SpellMap::SelectUnit(MapUnit* new_unit)
{		
	if(unit_selection && unit_selection == new_unit)
	{
		// selection of already selected unit: detect if there is air-land pair and swap selection

		// make full list of visible units
		auto units_list = units;
		units_list.reserve(100);
		if(!isGameMode())
		{
			// in editor mode show event untis too!
			for(auto& evt : events->GetEvents())
				for(auto& unit : evt->units)
					units_list.push_back(unit.unit);
		}

		for(auto & unit : units_list)
		{
			if(unit != new_unit && unit->coor == new_unit->coor)
			{
				// found: swap air-land selection
				if(!isGameMode() || !unit->is_enemy)
					unit_selection = unit;
			}
		}
	}
	unit_selection = new_unit;
	// unit selected
	unit_selection_mod = true;

	if(new_unit && new_unit->unit->sound_report)
		new_unit->PlayReport();

	// return new selection
	return(unit_selection);
}

// check if unit was selected and eventually clear flag
int SpellMap::UnitChanged(int clear)
{
	int was_selected = unit_selection_mod;
	if(clear)
		unit_selection_mod = false;
	return(was_selected);
}

// get unit at cursor position
MapUnit *SpellMap::GetCursorUnit(TScroll* scroll)
{
	// find selection tiles
	auto msel = GetSelections(scroll);

	// no selection?
	if(!msel.size() || !msel[0].IsSelected())
		return(NULL);
	auto & pos = msel[0];

	for(auto * unit : units)
	{
		if(unit->coor == pos)
			return(unit); // found match
	}
	if(!isGameMode())
	{
		for(auto & evt : events->GetEvents())
			for(auto & unit : evt->units)
			{
				if(unit.unit->coor == pos)
					return(unit.unit); // found match
			}
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


// return top most HUD pixel y-coordinate in surface coordinates system
int SpellMap::GetHUDtop(int surf_x_pos)
{
	surf_x_pos += surf_x_origin;
	if(!hud_enabled || surf_x_pos >= pic_y_buffer.size())
		return(surf_y);
	int y_top = pic_y_buffer[surf_x_pos];
	if(y_top < 0)
		return(surf_y);
	y_top -= surf_y_origin;
	return(y_top);
}

// render game mode HUD
int SpellMap::RenderHUD(uint8_t *buf,uint8_t* buf_end,int buf_x_size,MapXY *cursor, MapUnit *cursor_unit,std::function<void(void)> hud_buttons_cb)
{
	static int last_hud_state = -1;
	static MapUnit* last_cursor_unit = NULL;
	static MapUnit* last_selected_unit = NULL;
	static int last_can_heal = -1;
	static MapSprite *last_destructible = NULL;

	if(!hud_enabled)
	{
		// is disabled: just clear HUD buttons		
		ClearHUDbuttons();
		if(hud_buttons_cb)
			hud_buttons_cb();
		last_hud_state = hud_enabled;
		return(0);
	}
	
	// destructible object selected?
	MapSprite *destructible = NULL;
	if(cursor->IsSelected() && tiles[ConvXY(cursor)].isTarget())
		destructible = &tiles[ConvXY(cursor)];
			
	auto& gres = spelldata->gres;
	auto& font = spelldata->font;
	SpellGraphicItem *img;
	int x_ofs, y_ofs;

	// right panel x-offset (with exception of unit icon and experience indicators)
	const int x_rpan = 336;
	// right panel x-offset for icon and experience
	const int x_rpan_icon = 579;

	// clear y-buffer (for finding top most pixel of HUD (for mouse area cutoff))
	pic_y_buffer.assign(pic_x_size,-1);

	// render center panel
	img = gres.wm_hud;
	x_ofs = surf_x_origin + (surf_x - img->x_size)/2;
	y_ofs = surf_y_origin + surf_y - img->y_size;
	img->Render(buf,buf_end,buf_x_size,x_ofs,y_ofs,false,pic_y_buffer.data());
	int hud_left = x_ofs;
	int hud_right = x_ofs + img->x_size;
	int hud_top = y_ofs;

	// render side fillings
	img = gres.wm_hud_sides;
	y_ofs = surf_y_origin + surf_y - img->y_size;
	x_ofs = hud_right;
	for(; x_ofs < surf_x_origin+surf_x; x_ofs += img->x_size)
		img->Render(buf,buf_end,buf_x_size,x_ofs,y_ofs,false,pic_y_buffer.data());
	if(x_ofs < surf_x)
		img->Render(buf,buf_end,buf_x_size,x_ofs,y_ofs,false,pic_y_buffer.data());
	x_ofs = hud_left-img->x_size;
	for(; x_ofs >= surf_x_origin; x_ofs -= img->x_size)
		img->Render(buf,buf_end,buf_x_size,x_ofs,y_ofs,false,pic_y_buffer.data());
	img->Render(buf,buf_end,buf_x_size,x_ofs,y_ofs,false,pic_y_buffer.data());

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
		int men_wound = unit->wounded;
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
		int attack_light = unit->GetAttack(MapUnit::TARGET_TYPE::LIGHT);
		font->Render(buf,buf_end,buf_x_size,hud_left+px_ref+109,hud_top+83,string_format("%02d",attack_light),232,254,SpellFont::RIGHT_DOWN);
		if(pid == 0 && cursor_unit && cursor_unit->unit->isLight())
			RenderHUDrect(buf, buf_end, buf_x_size,hud_left+px_ref+109-11,hud_top+83,27,15,253);

		// pos a: 145,83
		int attack_heavy = unit->GetAttack(MapUnit::TARGET_TYPE::ARMOR);
		font->Render(buf,buf_end,buf_x_size,hud_left+px_ref+145,hud_top+83,string_format("%02d",attack_heavy),232,254,SpellFont::RIGHT_DOWN);
		if(pid == 0 && cursor_unit && cursor_unit->unit->isArmored())
			RenderHUDrect(buf,buf_end,buf_x_size,hud_left+px_ref+145-20,hud_top+83,37,15,253);

		// pos a: 181,83
		int attack_air = unit->GetAttack(MapUnit::TARGET_TYPE::AIR);
		font->Render(buf,buf_end,buf_x_size,hud_left+px_ref+181,hud_top+83,string_format("%02d",attack_air),232,254,SpellFont::RIGHT_DOWN);
		if(pid == 0 && cursor_unit && cursor_unit->unit->isAir())
			RenderHUDrect(buf,buf_end,buf_x_size,hud_left+px_ref+181-20,hud_top+83,36,15,253);

		// pos a: 216,83
		int defense = unit->GetDefence();
		font->Render(buf,buf_end,buf_x_size,hud_left+px_ref+216,hud_top+83,string_format("%02d",defense),232,254,SpellFont::RIGHT_DOWN);
		if(pid == 1 && cursor_unit)
			RenderHUDrect(buf,buf_end,buf_x_size,hud_left+px_ref+216-14,hud_top+83,31,15,253);


				

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
		int skill_fraction_pix = 0;
		if(unit->experience_level > 0 && unit->experience_level < 12)
		{
			int skill_frac_base = unit->unit->GetExperiencePts(unit->experience_level);
			int skill_frac_next = unit->unit->GetNextExperiencePts(unit->experience_level);
			skill_fraction_pix = 51*(unit->experience - skill_frac_base)/(skill_frac_next - skill_frac_base);
		}
		for(int y = 0; y < 2; y++)
			memset(&buf[hud_left+ix_ref+5 + (hud_top+y+77)*buf_x_size],235,min(max(skill_fraction_pix,1),51));
		
		const struct { int x; int y; } exp_mark[2][12] = {{	{1,81},{11,81},{21,81},{31,81},{41,81},{51,81},
															{1,90},{11,90},{21,90},{31,90},{41,90},{51,90}},
														 {	{580,81},{590,81},{600,81},{610,81},{620,81},{630,81},
															{580,90},{590,90},{600,90},{610,90},{620,90},{630,90}}};
		int skill_level = unit->experience_level;
		for(int k = 0; k < skill_level; k++)
			gres.wm_exp_mark->Render(buf,buf_end,buf_x_size,hud_left+exp_mark[pid][k].x,hud_top+exp_mark[pid][k].y);

		// render morale
		// pos A: 73,82  size: 6,48
		// pos B: 402,82  size: 6,48
		double morale = unit->morale;
		int morale_pix = (int)(morale*48/100);
		for(int y = 0; y < morale_pix; y++)
			memset(&buf[hud_left+px_ref+73 + (hud_top+82-y)*buf_x_size],199,6);
		// pos a: 75,70
		font->Render(buf,buf_end,buf_x_size,hud_left+px_ref+75,hud_top+70,string_format("%02.0f",morale),252,254,SpellFont::RIGHT_DOWN);

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
		int fire_count = unit->GetFireCount();
		int max_fire_count = unit->GetMaxFireCount();
		for(int k = 0; k < max_fire_count; k++)
		{
			auto pos = &fire_pos[k];
			if(k < fire_count)
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

	// destructible object selected?
	if(destructible)
	{
		// yaha: render object info
		auto spec_obj = destructible->GetDestructible()->destructible;
		string name = spec_obj->name;
		font->Render(buf,buf_end,buf_x_size,hud_left+415,hud_top+26,name,232,254,SpellFont::DIAG2);
				
		int hp =  100*destructible->hp/spec_obj->hp;
		int stat_pos = font->Render(buf,buf_end,buf_x_size,hud_left+415+5,hud_top+26+16,"Object state: ",234,254,SpellFont::DIAG2);
		font->Render(buf,buf_end,buf_x_size,stat_pos,hud_top+26+16,string_format("%d%%",hp),232,254,SpellFont::DIAG2);

		int def = spec_obj->defence;
		int def_pos  = font->Render(buf,buf_end,buf_x_size,hud_left+415+5,hud_top+26+2*16,"Defence: ",234,254,SpellFont::DIAG2);
		font->Render(buf,buf_end,buf_x_size,def_pos,hud_top+26+2*16,string_format("%d",def),232,254,SpellFont::DIAG2);		
	}


	// can unit heal?
	int can_heal = 0;
	if(unit_selection)
		can_heal = unit_selection->HasMaxAP() && unit_selection->HasWounded();

	// modify HUD buttons?
	int state_changed = isRenderSurfModified() || cursor_unit != last_cursor_unit || unit_selection != last_selected_unit || hud_enabled != last_hud_state ||
		last_can_heal != can_heal || last_destructible != destructible;

	if(state_changed && hud_buttons_cb)
	{		
		t_xypos btn_top[4] = {{246,26},{246+36,26},{246+36*2,26},{246+36*3,26}};
		t_xypos btn_right[8] = {{413,29},{449,29},{485,29},{534,29}, {413,63},{449,63},{485,63},{534,63}};
		t_xypos hud_origin = {hud_left, hud_top};
		
		// invalidate but keep old buttons
		ClearHUDbuttons(true);

		CreateHUDbutton(gres.wm_glyph_next_unused,hud_origin,btn_top[0],buf,buf_end,buf_x_size,0,bind(&SpellMap::OnHUDnextUnfinishedUnit,this),NULL);
		CreateHUDbutton(gres.wm_glyph_next,hud_origin, btn_top[1], buf, buf_end, buf_x_size, 0,bind(&SpellMap::OnHUDnextUnit,this), NULL);

		if(unit_selection && unit_selection->unit->hasSpecAction())
		{
			// render middle buttons (unit actions)
			int can_do_action = unit_selection->CanSpecAction();
			if(unit_selection->unit->isActionTurretDown() || unit_selection->unit->isActionTurretUp())
				CreateHUDbutton(unit_selection->unit->GetActionBtnGlyph(),hud_origin,btn_top[2],buf,buf_end,buf_x_size,0,bind(&SpellMap::OnHUDturretToggle,this),NULL,!can_do_action);
			else if(unit_selection->unit->isActionToggleRadar())
				CreateHUDbutton(unit_selection->unit->GetActionBtnGlyph(unit_selection->radar_up),hud_origin,btn_top[2],buf,buf_end,buf_x_size,0,bind(&SpellMap::OnHUDradarToggle,this),NULL,!can_do_action);
			else if(unit_selection->unit->isActionLand() || unit_selection->unit->isActionTakeOff())
				CreateHUDbutton(unit_selection->unit->GetActionBtnGlyph(),hud_origin,btn_top[2],buf,buf_end,buf_x_size,0,bind(&SpellMap::OnHUDairLandTakeOff,this),NULL,!can_do_action);
			else if(unit_selection->unit->isActionToFortres() || unit_selection->unit->isActionFromFortres())
				CreateHUDbutton(unit_selection->unit->GetActionBtnGlyph(),hud_origin,btn_top[2],buf,buf_end,buf_x_size,0,bind(&SpellMap::OnHUDfortresToggle,this),NULL,!can_do_action);
			else if(unit_selection->unit->isActionCreateUnit())
				CreateHUDbutton(unit_selection->unit->GetActionBtnGlyph(),hud_origin,btn_top[2],buf,buf_end,buf_x_size,0,bind(&SpellMap::OnHUDcreateUnit,this),NULL,!can_do_action);
		}

		if(unit_selection)
			CreateHUDbutton(gres.wm_glyph_heal,hud_origin,btn_top[3],buf,buf_end,buf_x_size,0,bind(&SpellMap::OnHUDhealUnit,this),NULL,!can_heal);
			
		if(!cursor_unit && !destructible)
		{
			// no unit under cursor: render 8 buttons in the right panel
			CreateHUDbutton(gres.wm_glyph_map,hud_origin,btn_right[0],buf,buf_end,buf_x_size,HUD_ACTION_MINIMAP,NULL,NULL);
			CreateHUDbutton((unit_sel_land_preference)?(gres.wm_glyph_ground):(gres.wm_glyph_air),hud_origin,btn_right[1],buf,buf_end,buf_x_size,0,bind(&SpellMap::OnHUDswitchAirLand,this),NULL);
			CreateHUDbutton(gres.wm_glyph_goto_unit,hud_origin,btn_right[2],buf,buf_end,buf_x_size,0,NULL,NULL);
			CreateHUDbutton(gres.wm_glyph_end_turn,hud_origin,btn_right[3],buf,buf_end,buf_x_size,0,bind(&SpellMap::OnHUDswitchEndTurn,this),NULL);
			
			CreateHUDbutton(gres.wm_glyph_unit_info,hud_origin,btn_right[4],buf,buf_end,buf_x_size,0,bind(&SpellMap::OnHUDswitchUnitHUD,this),NULL);
			CreateHUDbutton(gres.wm_glyph_info,hud_origin,btn_right[5],buf,buf_end,buf_x_size,0,NULL,NULL);
			CreateHUDbutton(gres.wm_glyph_options,hud_origin,btn_right[6],buf,buf_end,buf_x_size,HUD_ACTION_MAP_OPTIONS,NULL,NULL);
			CreateHUDbutton(gres.wm_glyph_retreat,hud_origin,btn_right[7],buf,buf_end,buf_x_size,0,NULL,NULL);
		}
		
		// cleanup leftover buttons
		CleanupInvalidHUDbuttons();
		
		// update GUI buttons
		hud_buttons_cb();
	}

	// store last units for change detection
	last_hud_state = hud_enabled;
	last_cursor_unit = cursor_unit;
	last_selected_unit = unit_selection;
	last_can_heal = can_heal;
	last_destructible = destructible;

	

	return(0);
}

// render simple rectangle frame
int SpellMap::RenderHUDrect(uint8_t* buf,uint8_t* buf_end,int buf_x_size,int x1,int y1,int w, int h, uint8_t color)
{
	if(&buf[x1 + y1*buf_x_size] >= buf_end || &buf[x1+w-1 + (y1+h-1)*buf_x_size] >= buf_end)
		return(1);
	memset(&buf[x1 + y1*buf_x_size],color,w);
	memset(&buf[x1 + (y1+h-1)*buf_x_size],color,w);
	for(int y = y1+1; y < y1+h-1;y++)
	{
		buf[x1+0 + y*buf_x_size] = color;
		buf[x1+w-1 + y*buf_x_size] = color;
	}
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
	InvalidateHUDbuttons();
}
void SpellMap::OnHUDswitchAirLand()
{
	unit_sel_land_preference = !unit_sel_land_preference;
	InvalidateHUDbuttons();
}
void SpellMap::OnHUDswitchUnitHUD()
{
	w_unit_hud = !w_unit_hud;
	InvalidateHUDbuttons();
}
void SpellMap::OnHUDswitchEndTurn()
{	
	// play button sound
	auto sound = new SpellSound(*spelldata->sounds->aux_samples.btn_end_turn);
	sound->Play(true);
	
	FinishUnits();
	ResetUnitsAP();
	
	// todo: replace by something that will trigger range recalculation
	unit_selection_mod = true;
	InvalidateHUDbuttons();
}
// heal unit HUD button pressed
void SpellMap::OnHUDhealUnit()
{
	auto unit = GetSelectedUnit();
	if(!unit)
		return;
	unit->Heal();
	InvalidateHUDbuttons();
}
// turret toggle HUD button pressed
void SpellMap::OnHUDturretToggle()
{
	auto unit = GetSelectedUnit();
	if(!unit)
		return;

	// reduce AP
	unit->action_points -= unit->unit->action_ap;

	// play action sound
	unit->PlayAction();

	if(unit->unit->isActionTurretDown())
	{
		// hide turret:
		unit->action_state = MapUnit::ACTION_STATE::TURRET_DOWN;
	}
	else
	{
		// show turret:
		unit->action_state = MapUnit::ACTION_STATE::TURRET_FINISH;
	}
	unit->ResetTurnsCounter();
}

// radar toggle HUD button pressed
void SpellMap::OnHUDradarToggle()
{
	auto unit = GetSelectedUnit();
	if(!unit)
		return;

	// reduce AP
	unit->action_points -= unit->unit->action_ap;

	// play action sound
	unit->PlayAction();

	// go to animation state	
	unit->in_animation = unit->unit->gr_action;
	unit->frame_stop = unit->unit->action_fsu_frames;	
	unit->azimuth = unit->in_animation->GetAnimAzim(unit->unit->gr_base->GetStaticAngle(unit->azimuth));

	// initiate animation
	if(unit->radar_up)
	{
		// hide radar:
		unit->frame = unit->frame_stop-1;
		unit->action_state = MapUnit::ACTION_STATE::RADAR_DOWN;
	}
	else
	{
		// show radar:
		unit->frame = 0;
		unit->action_state = MapUnit::ACTION_STATE::RADAR_UP;
	}
	unit->ResetTurnsCounter();
}

// aircraft land/takeoff HUD button pressed
void SpellMap::OnHUDairLandTakeOff()
{
	auto unit = GetSelectedUnit();
	if(!unit)
		return;

	// reduce AP
	unit->action_points -= unit->unit->action_ap;	
	
	// play action sound
	unit->PlayAction();

	// initiate animation
	if(unit->unit->isAir())
	{
		// land:
		unit->altitude = 100;
		unit->action_state = MapUnit::ACTION_STATE::AIR_LAND;		
	}
	else
	{
		// take off:
		auto target = spelldata->units->GetUnit(unit->unit->action_params[2]);
		unit->MorphUnit(target);
		unit->altitude = 0;
		unit->action_state = MapUnit::ACTION_STATE::AIR_TAKEOFF;
	}

	// go to animation state	
	unit->in_animation = unit->unit->gr_base;
	unit->frame_stop = unit->in_animation->anim.frames;	
	unit->frame = 0;
	unit->azimuth = unit->in_animation->GetAnimAzim(unit->in_animation->GetStaticAngle(unit->azimuth));

	unit->ClearDigLevel();
	unit->ResetTurnsCounter();
}

// switch to/from fortres HUD button callback
void SpellMap::OnHUDfortresToggle()
{
	auto unit = GetSelectedUnit();
	if(!unit)
		return;

	// reduce AP
	unit->action_points -= unit->unit->action_ap;

	// play action sound
	unit->PlayAction();
	
	// initiate animation
	if(unit->unit->isActionToFortres())
	{
		// morph to fortress:
		unit->in_animation = unit->unit->gr_action;
		unit->frame_stop = unit->unit->action_fsu_frames;
		unit->frame = 0;
		unit->azimuth = unit->in_animation->GetAnimAzim(unit->unit->gr_base->GetStaticAngle(unit->azimuth));
		unit->action_state = MapUnit::ACTION_STATE::TO_FORT;
	}
	else
	{
		// morph from fortress:
		unit->frame = unit->frame_stop - 1;
		unit->action_state = MapUnit::ACTION_STATE::FROM_FORT_TURRET;
	}

	unit->ResetTurnsCounter();
}

// create new unit HUD button pressed
void SpellMap::OnHUDcreateUnit()
{
	auto unit = GetSelectedUnit();
	if(!unit)
		return;

	// unly one child unit allowed
	if(unit->child)
		return;

	// reduce AP
	unit->action_points -= unit->unit->action_ap;

	// play action sound
	unit->PlayAction();

	// init animation
	unit->in_animation = unit->unit->gr_action;
	unit->frame_stop = unit->unit->action_fsu_frames;
	unit->frame = 0;
	unit->azimuth = unit->in_animation->GetAnimAzim(unit->unit->gr_base->GetStaticAngle(unit->azimuth));
	unit->action_state = MapUnit::ACTION_STATE::CREATE_UNIT;

	unit->ResetTurnsCounter();
}




// clear all HUD buttons (or invalidate only)
void SpellMap::ClearHUDbuttons(bool only_invalidate)
{
	if(only_invalidate)
	{
		for(auto& btn : hud_buttons)
			btn->Invalidate();
	}
	else
	{
		for(auto & btn : hud_buttons)
			delete btn;
		hud_buttons.clear();
	}
}
// cleanup invalidated HUD buttons
void SpellMap::CleanupInvalidHUDbuttons()
{
	for(int pid = hud_buttons.size()-1; pid>=0; pid--)
	{
		auto btn = hud_buttons[pid];
		if(btn->IsValid())
			continue;
		delete btn;
		hud_buttons.erase(hud_buttons.begin() + pid);
	}
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
	int action_id,std::function<void(void)> cb_press,std::function<void(void)> cb_hover,int disabled_glyph)
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
			glyph->Render(surf[sid],&surf[sid][x_size*y_size],x_size,0, (sid==2)*2,disabled_glyph);
	}
	
	// try to find old button with matching position so we can override it
	SpellBtnHUD *btn = NULL;
	for(auto & cbtn : hud_buttons)
		if(cbtn->PositionMatch(x_pos - surf_x_origin,y_pos - surf_y_origin,x_size,y_size))
		{
			btn = cbtn;
			btn->Make(x_pos - surf_x_origin,y_pos - surf_y_origin,x_size,y_size,(uint8_t*)terrain->pal,surf[0],surf[1],surf[2],action_id,cb_press,cb_hover);
			break;
		}
	if(!btn)
	{
		// create new button if not existed before
		btn = new SpellBtnHUD(x_pos - surf_x_origin, y_pos - surf_y_origin, x_size, y_size, (uint8_t*)terrain->pal, surf[0],surf[1],surf[2], action_id, cb_press, cb_hover);
		hud_buttons.push_back(btn);
	}

	// store enabled/disabled state to block button press
	btn->is_disabled = disabled_glyph;

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
			Sprite *spr = tiles[mxy].L1;
			int flag = tiles[mxy].flags;

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
						h += tiles[mxy].elev*Sprite::TILE_ELEVATION;

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
		max_h = max(max_h, (int)tile_h);
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
		// all tiles to never seen state
		units_view.assign(x_size*y_size, 0);

		// no units seen yet
		for(auto& unit : units)
		{
			unit->was_seen = false;
			unit->is_visible = 0;
		}
	}
	else
	{
		// all visible to seen, but not visible
		if(units_view.size() != x_size*y_size)
			units_view.assign(x_size*y_size,0);
		for(auto & pos : units_view)
			if(pos == 2)
				pos = 1;

		// no units seen yet
		for(auto& unit : units)
			unit->is_visible = max(unit->is_visible - 1,0);
	}

	

	return(0);
}

// calculate unit view range, returns true if new contact detected and returns activated events list
int SpellMap::AddUnitView(MapUnit *unit, vector<SpellMapEventRec*>* event_list)
{
	if(!unit)
		return(0);

	// no new contact
	int new_contact = false;

	// get unit view range in tiles
	int is_radar = false;
	int ref_view = unit->unit->sdir;
	if(unit->radar_up && unit->unit->isActionToggleRadar())
	{
		ref_view = unit->unit->action_params[2];
		is_radar = true;
	}	

	// reference position
	MapXY ref_pos = unit->coor;
	int ref_mxy = ConvXY(ref_pos);
	int ref_alt = tiles[ConvXY(ref_pos)].elev;

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
		int next_alt = tiles[next_mxy].elev;
		int view = ref_view + max(ref_alt-next_alt,0); // expand if terget lower than ref

		// visible?
		if((next_pos.Distance(ref_pos)-0.5) > view)
			continue; // nope: goto next tile
		// mark this tile as potentially visible (to stop recursion)
		units_view[next_mxy] += 3;				
		
		// -- send ray to target tile to find out if it is visible:				
		if(is_radar)
		{
			// in case of radar do not bother sending rays: all tiles in range visible
			units_view[next_mxy] = 5;
		}
		else if(!is_radar && units_view[next_mxy] < 5) // only if was not visible before
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

						// check new unit contact
						if(!Lunit.empty())
						{
							MapUnit* unit = Lunit[next_mxy];
							while(unit)
							{
								// new contact?
								if(!unit->is_visible && unit->is_enemy)
									new_contact = true;
								// check linked SeeUnit() event?
								if(event_list && unit->trig_event && unit->trig_event->isSeeUnit())
									event_list->push_back(unit->trig_event);
								// set unit seen flag
								unit->is_visible = 2;
								unit->was_seen = true;								
								unit = unit->next;
							}
						}
						if(event_list && events->CheckEvent(next_mxy))
						{
							// possibly some event here: get all undone events							
							auto list = events->GetEvents(next_mxy, true);
							event_list->insert(event_list->end(), list.begin(), list.end());
						}
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

		// -- expand view to entire objects (houses)
		vector<int> hdir;
		vector<MapXY> hpos;
		hdir.push_back(0);
		hpos.push_back(next_pos);
		int mxy = ConvXY(next_pos);
		if(tiles[mxy].flags == 0x10 || tiles[mxy].flags == 0x20 || tiles[mxy].flags == 0x30)
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
				if(tiles[hnext_mxy].flags != 0x10 && tiles[hnext_mxy].flags != 0x20 && tiles[hnext_mxy].flags != 0x30)
					continue;

				// mark as seen
				units_view[hnext_mxy] = 5;
				// check new enemy contact
				// check new unit contact
				if(!Lunit.empty())
				{
					MapUnit* unit = Lunit[next_mxy];
					while(unit)
					{
						// new contact?
						if(!unit->is_visible && unit->is_enemy)
							new_contact = true;
						// check linked SeeUnit() event?
						if(event_list && unit->trig_event && unit->trig_event->isSeeUnit())
							event_list->push_back(unit->trig_event);
						// mark unit as seen
						unit->is_visible = 2;
						unit->was_seen = true;
						unit = unit->next;
					}
				}
				if(event_list && events->CheckEvent(next_mxy))
				{
					// possibly some event here: get all undone events
					auto list = events->GetEvents(next_mxy,true);
					event_list->insert(event_list->end(),list.begin(),list.end());
				}

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

	return(new_contact);
}

// add view range of all moved units of given type, clear moved flags
int SpellMap::AddUnitsView(int unit_type, int clear,MapUnit* except_unit)
{
	SpellMapEventsList events;
	int new_contact = false;

	for(auto & unit : units)
	{
		// filter unit types to view
		if(unit->is_enemy && !(unit_type & UNIT_TYPE_OS) || !unit->is_enemy && !(unit_type & UNIT_TYPE_ALIANCE))
			continue;
		if(except_unit == unit)
			continue;
		if(clear)
			unit->was_moved = false;
		
		// add view mask
		SpellMapEventsList events_batch;
		new_contact |= AddUnitView(unit, &events_batch);
		
		// collect events
		events.insert(events.end(), events_batch.begin(), events_batch.end());
	}

	if(new_contact)
	{
		// play contact sound
		auto unit = GetSelectedUnit();
		if(unit && !unit->is_enemy)
			unit->PlayContact();
	}

	// process events
	ProcEventsList(events);

	return(0);
}

// store/restore units view to mem
int SpellMap::StoreUnitsView()
{
	units_view_mem = units_view;
	return(0);
}
int SpellMap::RestoreUnitsView()
{
	if(units_view_mem.size() == units_view.size())
	{
		for(int tid = 0; tid < units_view_mem.size(); tid++)
		{
			if(units_view[tid] >= 1 && !units_view_mem[tid])
				units_view_mem[tid] = 1;			
		}
		units_view = units_view_mem;
	}
	return(0);
}



// check if target can be attacked by current unit
int SpellMap::CanUnitAttack(MapUnit *target)
{
	if(!target)
		return(false);

	// only enemy targets
	if(!target->is_enemy)
		return(false);
	
	// attacking unit
	auto unit = GetSelectedUnit();
	if(!unit)
		return(false);
	auto pos = ConvXY(target->coor);

	// within range?
	if(!unit_attack_map[pos])
		return(false);

	// has some attacks left?
	if(!unit->GetFireCount())
		return(false);

	// allowed attack target?
	if(!unit->unit->canAttack(target->unit))
		return(false);

	return(true);
}
// can selected unit attack land unit (only if there is some)?
MapUnit* SpellMap::CanUnitAttackLand(MapXY pos)
{
	if(!pos.IsSelected())
		return(NULL);

	auto pxy = ConvXY(pos);	
	auto unit = Lunit[pxy];
	while(unit)
	{
		if(unit->unit->isLand() && CanUnitAttack(unit))
			return(unit);
		unit = unit->next;
	}
	return(NULL);
}
// can selected unit attack air unit (only if there is some)?
MapUnit *SpellMap::CanUnitAttackAir(MapXY pos)
{
	if(!pos.IsSelected())
		return(NULL);

	auto pxy = ConvXY(pos);
	auto unit = Lunit[pxy];
	while(unit)
	{
		if(unit->unit->isAir() && CanUnitAttack(unit))
			return(unit);
		unit = unit->next;
	}
	return(NULL);
}
// can selected unit attack object (only if there is some)?
int SpellMap::CanUnitAttackObject(MapXY pos)
{
	if(!pos.IsSelected())
		return(false);

	auto pxy = ConvXY(pos);	
	
	// attackable target?
	if(!tiles[pxy].isTarget())
		return(false);

	// already destroyed?
	if(!tiles[pxy].hp)
		return(false);
	
	// attacking unit
	auto unit = GetSelectedUnit();
	if(!unit)
		return(false);

	// within range?
	if(!unit_attack_map[pxy])
		return(false);

	// has some attacks left?
	if(!unit->GetFireCount())
		return(false);

	// allowed attack target?
	if(!unit->unit->canAttackObject())
		return(false);

	return(true);
}

// initialize unit attack range calculator
int SpellMap::UnitAttackRangeInit()
{
	// clear current mask
	unit_attack_map.assign(x_size*y_size,0);

	return(0);
}

// calculates attack range for given unit
int SpellMap::CalcUnitAttackRange(MapUnit *unit)
{
	// clear current view
	UnitAttackRangeInit();

	// leave if no unit selected
	if(!unit)
		return(0);

	// get initial unit attack range
	int ref_view = unit->unit->fire_range;

	// reference unit position
	MapXY ref_pos = unit->coor;
	int ref_mxy = ConvXY(ref_pos);
	int ref_alt = tiles[ConvXY(ref_pos)].elev;
	int ref_slope = tiles[ref_mxy].L1->GetSlope();

	if(ref_slope != 'A' && unit->unit->fire_flags & SpellUnitRec::FIRE_NOT_SLOPES)
	{
		// cannot fire from slopes, leave
		return(0);
	}

	if(unit->unit->isActionKamikaze())
	{
		// kamikaze can attack only to target directly below
		unit_attack_map[ref_mxy] = 2;
		return(0);
	}


	// recursion buffer
	vector<int> dirz;
	vector<MapXY> posz;
	dirz.reserve(500);
	posz.reserve(500);

	// place first tile to recoursive buffer (center tile)
	dirz.push_back(0);
	posz.push_back(ref_pos);


	// proceed recoursively till max visibility
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
		MapXY next_pos = GetNeighborTile(this_pos,dirz.back()-1);
		if(!next_pos.IsSelected())
			continue;
		int next_mxy = ConvXY(next_pos);

		// skip if already done
		if(unit_attack_map[next_mxy])
			continue;

		// next tile elevation
		int next_alt = tiles[next_mxy].elev;
		int view = ref_view + max(ref_alt-next_alt,0); // expand if terget lower than ref

		// visible?
		if((next_pos.Distance(ref_pos)-0.5) > view)
			continue; // nope: goto next tile
		// mark this tile as potentially visible (to stop recursion)
		unit_attack_map[next_mxy] = 1;

		if(unit->unit->isIndirectFire())
		{
			// indirect fire: no need to check direct sight, only min distance
			if((next_pos.Distance(ref_pos)-0.5) > 2.0)
			{
				// min range ok
				unit_attack_map[next_mxy] = 2;
			}
		}
		else
		{
			// -- send ray to target tile to find out if it is visible:				
			 
			// multisampling mode
			const int msx_ofs[4] ={1,-1,0,0};
			const int msy_ofs[4] ={0,0,1,-1};
			const int ms_count = 4;

			// get list of all points belonging target tile (in view mask coordinate system)
			auto target_pix = GetUnitsViewTilePixels(next_pos);
			// for each target point: send ray and check if the tartget point is visible, if at least one ray is, mark tile as visible
			for(auto& target_pos : target_pix)
			{
				// get view ray initial coordinates (unit)
				auto [x0,y0,z0b] = GetUnitsViewTileCenter(ref_pos);
				int z0 = (int)z0b;
				if(unit->unit->isAir())
					z0 += 100; // air-unit height estimate
				else
					z0 += 15; // unit height estimate

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
						unit_attack_map[next_mxy] = 2;
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
				if(unit_attack_map[next_mxy] == 2)
					break;
			}
		}

		// proceed to next position allowed
		dirz.push_back(0);
		posz.push_back(next_pos);
	}

	// clear potentially visible tiles (temps)
	for(auto& tile : unit_attack_map)
		if(tile < 2)
			tile = 0;

	return(0);
}

// finish units action before end of turn (dig levels, morale, ...)
int SpellMap::FinishUnits()
{
	for(auto & unit : units)
	{
		// update dig level
		unit->UpdateDigLevel();

		// activate runtime crated units
		unit->ActivateUnit();

		// todo: update morale (long inactivity)

		// update idle counters for next round
		unit->IncrementTurnsCounter();
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
void SpellMap::SetRender(bool wL1, bool wL2, bool wL3, bool wL4, bool wSTCI, bool wUnits,bool wSound,bool wSoundLoop,bool wEvents)
{
	this->wL1 = wL1;
	this->wL2 = wL2;
	this->wL3 = wL3;
	this->wL4 = wL4;
	this->wSTCI = wSTCI;
	this->wUnits = wUnits;
	this->wSound = wSound;
	this->wSoundLoop = wSoundLoop;
	this->wEvents = wEvents;
}

// set gamma correction for rendering
void SpellMap::SetGamma(double gamma)
{
	this->gamma = gamma;
	// forces recalculation of gamma table
	last_gamma = 0.0;
}
double SpellMap::GetGamma()
{
	return(gamma);
}

// timer tick - update of palettes and animations
int SpellMap::Tick()
{
	if (!this->IsLoaded())
		return(0);

	int update = false;

	static int tick_250ms_div;
	tick_250ms_div++;
	if(tick_250ms_div >= 25)
		tick_250ms_div = 0;
	int tick_250ms = (tick_250ms_div == 0);

	static int tick_100ms_div;
	tick_100ms_div++;
	if(tick_100ms_div >= 10)
		tick_100ms_div = 0;
	int tick_100ms = (tick_100ms_div == 0);

	static int tick_40ms_div;
	tick_40ms_div++;
	if(tick_40ms_div >= 4)
		tick_40ms_div = 0;
	int tick_40ms = (tick_40ms_div == 0);

	static int tick_30ms_div;
	tick_30ms_div++;
	if(tick_30ms_div >= 3)
		tick_30ms_div = 0;
	int tick_30ms = (tick_30ms_div == 0);

	if(tick_250ms)
	{
		// selection blinking
		sel_blink_state = !sel_blink_state;
	}

	if(tick_100ms)
	{
	
		// cycle water palette
		uint8_t water[10][3];
		std::memcpy((void*)water, (void*)&pal[240][0], 10 * 3);
		std::memcpy((void*)&pal[240][0], (void*)&water[1][0], 9 * 3);
		std::memcpy((void*)&pal[249][0], (void*)&water[0][0], 1 * 3);

		// animate L3 animations
		for (int k = 0; k < L3.size(); k++)
		{
			MapLayer3 *anm = &L3[k];
			// cycle through frames
			anm->frame_ofs++;
			if (anm->frame_ofs >= anm->frame_limit)
				anm->frame_ofs = 0;
		}

		// animate L4 animations
		for (int k = 0; k < L4.size(); k++)
		{
			MapLayer4* pnm = &L4[k];
			// cycle through frames
			pnm->frame_ofs++;
			if (pnm->frame_ofs >= pnm->frame_limit)
				pnm->frame_ofs = 0;
		}

		// animate unit pointer
		pnm_sipka.frame_ofs++;
		if(pnm_sipka.frame_ofs >= pnm_sipka.frame_limit)
			pnm_sipka.frame_ofs = 0;

		update = true;
	}
	
	
	// === Random map sounds ===
	static int rnd_sound_delay = 0;
	static chrono::steady_clock::time_point ref_time = std::chrono::high_resolution_clock::now();	
	auto now_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(now_time - ref_time).count();
	if(duration >= rnd_sound_delay)
	{
		if(rnd_sound_delay && sounds.size())
		{
			// play some sound			
			double left_vol;
			double right_vol;
			auto sound = GetRandomSound(&left_vol,&right_vol);
			if(sound)
			{
				auto sound_obj = new SpellSound(spelldata->sounds->channels, sound->GetSample());
				sound_obj->SetPanning(left_vol, right_vol);
				sound_obj->Play(true);
			}
		}
		rnd_sound_delay = rand()%20;
		ref_time = std::chrono::high_resolution_clock::now();
	}

	if(tick_100ms)
	{
		sound_loops->UpdateVolumes(xs_ofs, ys_ofs, xs_size, ys_size);
	}




	
	// get ref. unit
	auto* unit = GetSelectedUnit();
	if(!unit)
		return(0);

	
	LockMap();

	if(unit->move_state != MapUnit::MOVE_STATE::IDLE)
	{			
		// === UNIT MOVEMENT ===

		//unit_action_lock.lock();

		if(unit->unit->usingTeleportMove())
		{
			// --- teleport move mode:

			if(unit->move_state == MapUnit::MOVE_STATE::TURRET)
			{
				// turn unit to match teleport animation
				int azim_count = unit->unit->gr_base->stat.azimuths;
				int delta_azim = mod(0x0A - unit->azimuth + azim_count/2, azim_count) - azim_count/2;
				if(delta_azim == 0)
				{
					// aligned: done										
					unit->frame = -1;
					unit->move_state = MapUnit::MOVE_STATE::TELEPORT_IN;
				}
				else if(delta_azim > 0)
					unit->azimuth++;
				else
					unit->azimuth--;
				if(unit->azimuth < 0)
					unit->azimuth += azim_count;
				else if(unit->azimuth >= azim_count)
					unit->azimuth -= azim_count;

				update = true;
			}
			else if(unit->move_state == MapUnit::MOVE_STATE::TELEPORT_IN && tick_30ms)
			{
				// play teleport animation

				if(unit->frame < 0)
				{
					// start animation				
					unit->in_animation = unit->unit->gr_action;
					unit->azimuth = 0; // animation has no azimuth!

					// play move sound (async)
					unit->PlayMove();

					// update view of all but this unit:
					ClearUnitsView();
					AddUnitsView(UNIT_TYPE_ALIANCE,true,unit);
					StoreUnitsView();
					AddUnitView(unit);

					// go directly to last step
					unit->move_step = unit->move_nodes.size()-1;
				}

				unit->frame++;
				if(unit->frame >= unit->in_animation->anim.frames)
				{
					// unit disappeared:

					// move unit
					auto next_pos = unit->move_nodes[unit->move_step];
					unit->action_points -= (next_pos.g_cost - 0);
					unit->coor = next_pos.pos;

					// stop move sound (async - this is just flag to shut down in next sound frame)
					unit->PlayStop();

					// resort units in map
					SortUnits();

					// play the animation backwards
					unit->move_state = MapUnit::MOVE_STATE::TELEPORT_OUT;
				}

				update = true;
			}
			if(unit->move_state == MapUnit::MOVE_STATE::TELEPORT_OUT && tick_30ms)
			{
				// teleport out

				unit->frame--;
				if(unit->frame < 0)
				{
					// unit again visible:					

					// update this unit view
					RestoreUnitsView();
					SpellMapEventsList events;
					int new_contact = AddUnitView(unit, &events);
					if(new_contact)
					{
						unit->PlayContact();
					}
					// append events
					event_list.insert(event_list.end(), events.begin(), events.end());
					
					// check and append TransportUnit/SaveUnit events
					if(unit->trig_event && unit->trig_event->CheckUnitInPos(true))
						event_list.push_back(unit->trig_event);

					// done					
					unit->in_animation = NULL;
					unit->azimuth = 0x0A; // azimuth matching teleport animation which has only one azimumth
					unit->move_nodes.clear();
					unit->was_moved = true; // this will force range recalculation
					unit->move_state = MapUnit::MOVE_STATE::IDLE;					
				}

				update = true;
			}

		}
		else
		{
			if(unit->move_state == MapUnit::MOVE_STATE::TURRET)
			{
				// turret allign:
				if(unit->unit->hasTurret())
				{
					// has turret: align it first with rest of vehicle
			
					unit->move_state = MapUnit::MOVE_STATE::MOVE;
					unit->move_step = 0;
				}
				else
				{
					// no turret: skip step
					unit->move_state = MapUnit::MOVE_STATE::MOVE;
					unit->move_step = 0;
				}

			}
			else if(unit->move_state == MapUnit::MOVE_STATE::MOVE)
			{
				// movement:

				if(unit->move_step == 0)
				{
					// first step:
				
					// play move sound (async)
					unit->PlayMove();

					// start animation
					unit->frame = -1;
					if(unit->unit->gr_base->anim.frames)
						unit->in_animation = unit->unit->gr_base;

					// update view of all but this unit:
					ClearUnitsView();
					AddUnitsView(UNIT_TYPE_ALIANCE, true, unit);
					StoreUnitsView();
					AddUnitView(unit);

				}
			
				auto this_pos = unit->coor;

				int ap_prev = 0;
				if(unit->move_step)
					ap_prev = unit->move_nodes[unit->move_step-1].g_cost;

				auto next_pos = unit->move_nodes[unit->move_step++];

				unit->action_points -= (next_pos.g_cost - ap_prev);
				unit->coor = next_pos.pos;
				//unit->was_moved = true; // not needed, this would trigger full view recalc in renderer
			
				double azimuth = this_pos.Angle(next_pos.pos);

				// resort units in map
				SortUnits();
			
				// update this unit view
				RestoreUnitsView();
				SpellMapEventsList events;
				int new_contact = AddUnitView(unit, &events);
				if(new_contact)
				{
					// enemy contact event:
					unit->PlayContact();
				}
				// append events
				event_list.insert(event_list.end(),events.begin(),events.end());

				// check and append TransportUnit/SaveUnit events
				if(unit->trig_event && unit->trig_event->CheckUnitInPos(true))
					event_list.push_back(unit->trig_event);

			
				if(unit->in_animation)
				{
					// animated move
					unit->azimuth = unit->unit->gr_base->GetAnimAzim(azimuth);
					unit->frame++;
					if(unit->frame >= unit->in_animation->anim.frames)
						unit->frame = 0;
				}
				else
				{
					// static
					unit->azimuth = unit->unit->gr_base->GetStaticAzim(azimuth);
				}
				unit->azimuth_turret = unit->azimuth;

			

				if(new_contact || !events.empty() || unit->move_step >= unit->move_nodes.size())
				{
					// movement done:
					unit->move_state = MapUnit::MOVE_STATE::IDLE;
					unit->move_nodes.clear();
					unit->was_moved = true; // this will force range recalculation

					// stop move sound (async - this is just flag to shut donw in next sound frame)
					unit->PlayStop();

					// stop animation (switch to static)
					unit->in_animation = NULL;
					unit->azimuth = unit->unit->gr_base->GetStaticAzim(azimuth);
					unit->frame = 0;
				}

			}

			update = true;
		}

		//unit_action_lock.unlock();

		
	}
	else if(unit->attack_state != MapUnit::ATTACK_STATE::IDLE)
	{
		// === UNIT ATTACK ===

		// target unit/position
		MapUnit* target = unit->attack_target;
		MapXY target_pos = (target)?(target->coor):(unit->attack_target_obj);
		auto *target_obj = &tiles[ConvXY(target_pos)];

		auto *sprite = tiles[ConvXY(unit->coor)].L1;
		int slope_id = sprite->GetSlope() - 'A';

		if(unit->attack_state == MapUnit::ATTACK_STATE::DIR)
		{
			// determining fire azimuth (determined during render)
			
			//unit->projectile_angle = GetUnitsAngle(unit,target);
			//unit->unit_angle = GetUnitsAngle(unit,target);//unit->coor.Angle(target->coor);
			unit->unit_angle = unit->coor.Angle(target_pos);
			unit->attack_angle = unit->unit_angle;
			//unit->attack_angle = GetUnitsAngle(unit,target);
			if(unit->unit->isMLRS())
			{
				unit->unit_angle -= 90.0;
				unit->attack_angle -= 90.0;
			}
			unit->attack_dist = unit->coor.Distance(target_pos);
			unit->attack_state = MapUnit::ATTACK_STATE::TURN;

			// play move sound when rotating unit (async)
			if(!unit->unit->hasTurret())
				unit->PlayMove();

		}
		else if(unit->attack_state == MapUnit::ATTACK_STATE::TURN && tick_40ms_div)
		{
			// turning unit or turret to direction:

			// turret allign:
			if(unit->unit->hasTurret())
			{
				// has turret:

				// rotate turret to the target				
				//int target_azimuth = unit->unit->gr_aux->GetStaticFireAzim(slope_id, unit->unit_angle);
				int target_azimuth = unit->unit->gr_aux->GetStaticAzim(unit->unit_angle);
				int azim_count = unit->unit->gr_base->stat.azimuths;
				int delta_azim = mod(target_azimuth - unit->azimuth_turret + azim_count/2,azim_count) - azim_count/2;
				if(delta_azim == 0)
				{
					// aligned: done
					unit->attack_state = MapUnit::ATTACK_STATE::SHOT;
				}
				else if(delta_azim > 0)
					unit->azimuth_turret++;
				else
					unit->azimuth_turret--;			
				if(unit->azimuth_turret < 0)
					unit->azimuth_turret += azim_count;
				else if(unit->azimuth_turret >= azim_count)
					unit->azimuth_turret -= azim_count;

			}
			else
			{
				// no turret: rotate whole unit

				// rotate turrent to the target				
				//int target_azimuth = unit->unit->gr_base->GetStaticFireAzim(slope_id,unit->unit_angle);
				int target_azimuth = unit->unit->gr_base->GetStaticAzim(unit->unit_angle);
				auto *shot_anim = unit->GetShotAnim(target,&unit->frame_stop);
				if(shot_anim)
					target_azimuth = shot_anim->GetAnimAzim(unit->unit_angle);
				int azim_count = unit->unit->gr_base->stat.azimuths;
				int delta_azim = mod(target_azimuth - unit->azimuth + azim_count/2,azim_count) - azim_count/2;
				if(delta_azim == 0)
				{
					// aligned: done
					unit->attack_state = MapUnit::ATTACK_STATE::SHOT;

					// stop rotation sound
					unit->PlayStop();
				}
				else if(delta_azim > 0)
					unit->azimuth++;
				else
					unit->azimuth--;
				if(unit->azimuth < 0)
					unit->azimuth += azim_count;
				else if(unit->azimuth >= azim_count)
					unit->azimuth -= azim_count;
				
			}

			update = true;
		}
		else if(unit->attack_state == MapUnit::ATTACK_STATE::SHOT && tick_30ms)
		{
			// shot anim running:
							
			if(unit->frame == 0)
			{
				// start shot animation:
				
				unit->in_animation = unit->GetShotAnim(target,&unit->frame_stop);
				if(unit->in_animation)
					unit->azimuth = unit->in_animation->GetAnimAzim(unit->unit_angle);

				// play shot sound (async)
				unit->PlayFire(target);

				// mark target unit
				if(target)
					target->is_target = true;
				else
					target_obj->is_target = true;

				// reduce AP by single shot
				unit->UpdateFireAP();
				unit->was_moved = true; // this will force range recalculation
			}

			// next frame
			unit->frame++;
			if(unit->frame >= unit->frame_stop || !unit->in_animation)
			{				
				// optional fire animation (cannon fireball)
				unit->attack_fire_pnm = unit->GetFirePNM(target);
				unit->attack_fire_frame = 0;
				
				// get fire origin
				FSU_resource::Txy fire_org;
				if(unit->in_animation)
					fire_org = unit->in_animation->anim.fire_origin[unit->azimuth];
				else if(unit->unit->hasTurret())
					fire_org = unit->unit->gr_aux->stat.fire_origin[slope_id][unit->azimuth_turret];
				else
					fire_org = unit->unit->gr_base->stat.fire_origin[slope_id][unit->azimuth];
				unit->attack_fire_x_org = fire_org.x;
				unit->attack_fire_y_org = fire_org.y;

				// stop anim
				unit->in_animation = NULL;
				unit->frame = 0;
								
				// start projectile flight
				unit->attack_proj_step = 0;
				if((target && unit->unit->hasProjectile(target->unit)) || unit->unit->hasProjectile())
					unit->attack_proj_delay = (int)(0.04*unit->attack_dist/0.02);
				else
					unit->attack_proj_delay = (int)(0.01*unit->attack_dist/0.02);
				unit->attack_state = MapUnit::ATTACK_STATE::FLIGHT;
			}			

			update = true;
		}
		else if(unit->attack_state == MapUnit::ATTACK_STATE::FLIGHT)
		{
			// projectile flight and fire PNM:

			// progress fire PNM
			unit->attack_fire_frame++;
			if(unit->attack_fire_pnm && unit->attack_fire_frame >= unit->attack_fire_pnm->frames.size())
			{
				// fire PNM done:
				unit->attack_fire_pnm = NULL;
				unit->attack_fire_frame = 0;
			}

			// progress projectile trace
			unit->attack_proj_step++;
			if(unit->attack_proj_step >= unit->attack_proj_delay)
			{
				// projectile flight done:
				unit->attack_state = MapUnit::ATTACK_STATE::HIT;

				// stop fire anim if still goes
				unit->attack_fire_pnm = NULL;
				unit->attack_fire_frame = 0;
				
				// apply damage to target
				MapUnit::AttackResult hit = MapUnit::AttackResult::Hit;
				if(target)
					hit = unit->DamageTarget(target);
				else
					hit = unit->DamageTarget(target_obj);

				// play hit sound
				unit->PlayHit(target, hit != MapUnit::AttackResult::Missed);

				// play target hit sound if applicable
				if(target && hit == MapUnit::AttackResult::Hit)
					target->PlayBeingHit();
				else if(target && hit == MapUnit::AttackResult::Kill)
					target->PlayDie();
				else if(!target && hit == MapUnit::AttackResult::Hit)
					target_obj->PlayHit();
				else if(!target && hit == MapUnit::AttackResult::Kill)
					target_obj->PlayDestruct();
				
				// default hit PNM
				auto hit_pnm = unit->GetTargetHitPNM(target);
				
				// override by object destruction PNM?
				if(!target && hit == MapUnit::AttackResult::Kill && target_obj->GetDestructible()->destroy_pnm)
				{
					hit_pnm = target_obj->GetDestructible()->destroy_pnm;
					attack_explosion_list = GetDestructibleList(target_pos);
				}

				// init hit PNM
				if(target && hit_pnm)
				{
					// being hit PNM for unit target
					target->attack_hit_pnm = hit_pnm;
					target->attack_hit_frame = 0;
					target->attack_state = MapUnit::ATTACK_STATE::HIT;
				}
				else if(!target && hit_pnm)
				{
					// being hit PNM for map tile target
					target_obj->hit_pnm = hit_pnm;
					target_obj->hit_pnm_frame = 0;
				}
				else
				{
					// not PNM needed - stop here					
					unit->attack_state = MapUnit::ATTACK_STATE::DIE_INIT;									

					// unmark target unit
					if(target)
						target->is_target = false;
					else
						target_obj->is_target = false;
				}

				// expand explosion to linked tiles
				for(auto & kaboom_tile : attack_explosion_list)
				{
					kaboom_tile->hit_pnm = target_obj->hit_pnm;
					kaboom_tile->hit_pnm_frame = target_obj->hit_pnm_frame;
				}
			}

			update = true;
		}
		else if(unit->attack_state == MapUnit::ATTACK_STATE::HIT && tick_40ms)
		{
			// hit animation:
			if(target)
			{
				// for unit target
				target->attack_hit_frame++;
				if(target->attack_hit_frame >= target->attack_hit_pnm->frames.size())
				{
					// hit anim done:
					unit->attack_state = MapUnit::ATTACK_STATE::DIE_INIT;
					target->attack_state = MapUnit::ATTACK_STATE::IDLE;
					target->attack_hit_pnm = NULL;					
					target->is_target = false;					
				}
			}
			else
			{
				// for map tile target(s)
				target_obj->hit_pnm_frame++;				
				if(target_obj->hit_pnm_frame >= target_obj->hit_pnm->frames.size())
				{
					// hit anim done:
					unit->attack_state = MapUnit::ATTACK_STATE::UPDATE;
					target_obj->hit_pnm = NULL;					
					target_obj->is_target = false;
				}
				// duplicate PNM state to linked tiles
				for(auto& kaboom_tile : attack_explosion_list)
				{
					kaboom_tile->hit_pnm_frame = target_obj->hit_pnm_frame;
					kaboom_tile->hit_pnm = target_obj->hit_pnm;
				}
				if(!target_obj->hit_pnm)
					attack_explosion_list.clear();

			}

			update = true;
		}
		else if(unit->attack_state == MapUnit::ATTACK_STATE::DIE_INIT)
		{
			// initialize die animation if needed
			if(target->isDead() && target->unit->gr_die)
			{
				// play die action
				target->in_animation = target->unit->gr_die;
				target->frame_stop = target->unit->die_anim_frames;
				target->frame = 0;
				unit->attack_state = MapUnit::ATTACK_STATE::DIE;
			}
			else
			{
				// no die
				unit->attack_state = MapUnit::ATTACK_STATE::UPDATE;
			}

		}
		else if(unit->attack_state == MapUnit::ATTACK_STATE::DIE && tick_40ms)
		{
			// play die animation
			unit->frame++;
			if(unit->frame >= unit->frame_stop || !unit->in_animation)
			{				
				// stop anim
				unit->in_animation = NULL;
				unit->frame = 0;			
				unit->attack_state = MapUnit::ATTACK_STATE::UPDATE;
			}

		}
		else if(unit->attack_state == MapUnit::ATTACK_STATE::UPDATE)
		{
			// update target unit/object after attack:
			if(target && target->isDead())
			{
				// update unit after attack
				
				if(target->unit->die_action_id == SpellUnitRec::DIE_ACTION::MORPH)
				{
					// morph unit
					auto new_type = spelldata->units->GetUnit(target->unit->die_action_params[2]);
					if(new_type)
						target->MorphUnit(new_type,50);
				}
				else
				{
					// kill target unit
					auto dead = ExtractUnit(target);
					if(dead)
					{
						// kill unit
						auto kill_evt = dead->Kill();
						
						// equeue linked event
						if(kill_evt)
							event_list.push_back(kill_evt);
					}
					SortUnits();
				}								
			}
			else if(!target)
			{
				// update hit object and map
				auto destroyed_list = UpdateDestructible(target_pos);				
				if(!destroyed_list.empty())
				{
					// update view mask
					PrepareUnitsViewMask();
				}
				unit->was_moved = true; // this will force range recalculation

				// try get eventual DestroyObject events
				for(auto & destroyed : destroyed_list)
				{										
					auto destroy_events = events->GetDestroyObjectEvents(destroyed,true);
					event_list.insert(event_list.end(),destroy_events.begin(),destroy_events.end());
				}

			}

			// idle
			unit->attack_state = MapUnit::ATTACK_STATE::IDLE;
						
			update = true;
		}
		
	}
	else if(unit->action_state != MapUnit::ACTION_STATE::IDLE && tick_30ms_div)
	{				
		// === Special action ===
		
		bool air_done = false;
		if(unit->action_state == MapUnit::ACTION_STATE::AIR_LAND)
		{
			// landing
			unit->altitude -= 3;
			if(unit->altitude <= 0)
			{
				// landed: morph unit and stop				
				auto morph_type = spelldata->units->GetUnit(unit->unit->action_params[2]);
				unit->MorphUnit(morph_type);				
				air_done = true;
			}
			unit->frame++;
			if(unit->frame >= unit->frame_stop)
				unit->frame = 0;
			update = true;
		}
		else if(unit->action_state == MapUnit::ACTION_STATE::AIR_TAKEOFF)
		{
			unit->altitude += 3;
			if(unit->altitude >= 100)
			{
				// took off: stop
				air_done = true;
			}
			unit->frame++;
			if(unit->frame >= unit->frame_stop)
				unit->frame = 0;
			update = true;
		}
		else if(unit->action_state == MapUnit::ACTION_STATE::AIR_FINISH)
		{
			// waiting to finish sound, so the unit morph can be finished
			if(unit->AreSoundsDone())
			{
				// reload sounds for morphed unit
				unit->ClearSounds();
				unit->action_state = MapUnit::ACTION_STATE::IDLE;
			}
		}
		else if(unit->action_state == MapUnit::ACTION_STATE::TURRET_DOWN)
		{
			// align turret:
			int azim_count = unit->unit->gr_base->stat.azimuths;
			int delta_azim = mod(unit->azimuth - unit->azimuth_turret + azim_count/2,azim_count) - azim_count/2;
			if(delta_azim == 0)
			{
				// aligned: done
				unit->action_state = MapUnit::ACTION_STATE::TURRET_FINISH;				
			}
			else if(delta_azim > 0)
				unit->azimuth_turret++;
			else
				unit->azimuth_turret--;
			if(unit->azimuth_turret < 0)
				unit->azimuth_turret += azim_count;
			else if(unit->azimuth_turret >= azim_count)
				unit->azimuth_turret -= azim_count;

			update = true;
		}
		else if(unit->action_state == MapUnit::ACTION_STATE::TURRET_FINISH)
		{
			// morph unit
			auto morph_type = spelldata->units->GetUnit(unit->unit->action_params[2]);
			unit->MorphUnit(morph_type);
			air_done = true;
			update = true;
		}
		else if(unit->action_state == MapUnit::ACTION_STATE::RADAR_UP)
		{			
			// radar tower up:
			unit->frame++;
			if(unit->frame >= unit->frame_stop)
			{
				unit->frame = unit->frame_stop-1;
				unit->radar_up = true;
				air_done = true;
			}
			update = true;
		}
		else if(unit->action_state == MapUnit::ACTION_STATE::RADAR_DOWN)
		{
			// radar tower down:
			unit->frame--;
			if(unit->frame <= 0)
			{
				unit->frame = 0;
				unit->radar_up = false;
				unit->azimuth = unit->unit->gr_base->GetStaticAzim(unit->in_animation->GetAnimAngle(unit->azimuth));
				unit->in_animation = NULL;
				air_done = true;
			}
			update = true;
		}
		else if(unit->action_state == MapUnit::ACTION_STATE::TO_FORT)
		{
			// morhp to fortress:

			unit->frame++;
			if(unit->frame >= unit->frame_stop)
			{
				// morph unit to fortress
				auto morph_type = spelldata->units->GetUnit(unit->unit->action_params[2]);
				unit->MorphUnit(morph_type);
				double angle = unit->in_animation->GetAnimAngle(unit->azimuth);
				unit->azimuth = unit->unit->gr_base->GetStaticAzim(angle);
				unit->azimuth_turret = unit->unit->gr_aux->GetStaticAzim(angle);
				unit->in_animation = NULL;
				air_done = true;
			}
			update = true;
		}
		else if(unit->action_state == MapUnit::ACTION_STATE::FROM_FORT_TURRET)
		{
			// morph from fortress:

			// align turret:
			int azim_count = unit->unit->gr_base->stat.azimuths;
			int delta_azim = mod(unit->azimuth - unit->azimuth_turret + azim_count/2,azim_count) - azim_count/2;
			if(delta_azim == 0)
			{
				// aligned: go to animation state	
				unit->in_animation = unit->unit->gr_action;
				unit->frame_stop = unit->unit->action_fsu_frames;
				unit->frame = unit->frame_stop - 1;
				unit->azimuth = unit->in_animation->GetAnimAzim(unit->unit->gr_base->GetStaticAngle(unit->azimuth));
				unit->action_state = MapUnit::ACTION_STATE::FROM_FORT;
			}
			else if(delta_azim > 0)
				unit->azimuth_turret++;
			else
				unit->azimuth_turret--;
			if(unit->azimuth_turret < 0)
				unit->azimuth_turret += azim_count;
			else if(unit->azimuth_turret >= azim_count)
				unit->azimuth_turret -= azim_count;

			update = true;			
		}
		else if(unit->action_state == MapUnit::ACTION_STATE::FROM_FORT)
		{
			// morph from fortress:			
			unit->frame--;
			if(unit->frame <= 0)
			{
				// morph unit to fortress
				auto morph_type = spelldata->units->GetUnit(unit->unit->action_params[2]);
				unit->MorphUnit(morph_type);
				unit->azimuth = unit->unit->gr_base->GetStaticAzim(unit->in_animation->GetAnimAngle(unit->azimuth));
				unit->in_animation = NULL;
				air_done = true;
			}
			update = true;
		}
		else if(unit->action_state == MapUnit::ACTION_STATE::CREATE_UNIT)
		{
			// create new unit
			unit->frame++;
			if(unit->frame >= unit->frame_stop)
			{
				// stop anim
				unit->azimuth = unit->unit->gr_base->GetStaticAzim(unit->in_animation->GetAnimAngle(unit->azimuth));
				unit->in_animation = NULL;
				
				// create new unit
				auto new_type = spelldata->units->GetUnit(unit->unit->action_params[2]);
				CreateUnit(unit, new_type);
				unit->action_points = 0;
								
				air_done = true;
			}
			update = true;
		}
		else if(unit->action_state == MapUnit::ACTION_STATE::KAMIKAZE)
		{
			// kamikaze attack: drop
			
			// fast drop
			unit->altitude -= 10;
			if(unit->altitude <= 20)
			{
				// reached ground: kaboom!
				
				// hide attacker unit till actually destroyed
				unit->hide = true;

				// start explosion PNM
				unit->attack_target->attack_hit_pnm = unit->GetTargetHitPNM(unit->attack_target);
				unit->attack_target->attack_hit_frame = 0;
				unit->attack_target->attack_state = MapUnit::ATTACK_STATE::HIT;
				unit->action_state = MapUnit::ACTION_STATE::KAMIKAZE_EXPLOSION;

				// play explosion sound
				unit->PlayHit(unit->attack_target);
			}			
			update = true;
		}
		else if(unit->action_state == MapUnit::ACTION_STATE::KAMIKAZE_EXPLOSION)
		{
			// kamikaze attack: explosion

			if(unit->attack_target->attack_hit_pnm)
				unit->attack_target->attack_hit_frame++;
			if(unit->attack_target->attack_hit_pnm && unit->attack_target->attack_hit_frame >= unit->attack_target->attack_hit_pnm->frames.size())
			{
				// hit anim done:
				unit->attack_target->attack_hit_pnm = NULL;
				unit->attack_target->attack_state = MapUnit::ATTACK_STATE::IDLE;

				// unmark target unit
				unit->attack_target->is_target = false;					

				// apply damage to target
				MapUnit::AttackResult hit = MapUnit::AttackResult::Hit;
				hit = unit->DamageTarget(unit->attack_target);

				// play hit sound
				unit->PlayHit(unit->attack_target,hit != MapUnit::AttackResult::Missed);

				// play target hit sound if applicable
				if(hit == MapUnit::AttackResult::Hit)
					unit->attack_target->PlayBeingHit();
				else if(hit == MapUnit::AttackResult::Kill)
					unit->attack_target->PlayDie();
				
				// process dammage to target
				unit->attack_state = MapUnit::ATTACK_STATE::DIE_INIT;
			}

			if(!unit->attack_target->attack_hit_pnm && unit->AreSoundsDone())
			{
				// waiting to finish sound
				unit->action_state = MapUnit::ACTION_STATE::IDLE;

				// destroy attacker unit
				if(unit->parent)
					SelectUnit(unit->parent); // siwth to parent
				else
					OnHUDnextUnit(); // or switch to next unit
				RemoveUnit(unit);

				//
				RestoreUnitsView();
				SortUnits();
				unit->was_moved = true; // this will force range recalculation				

				InvalidateHUDbuttons();
			}

			update = true;
		}
		
		if(air_done)
		{
			// animation finished
			unit->altitude = 100;
			if(unit->in_animation && !unit->radar_up)
				unit->azimuth = unit->in_animation->GetStaticAzim(unit->in_animation->GetAnimAngle(unit->azimuth));
			if(!unit->radar_up)
				unit->in_animation = NULL;
			unit->action_state = MapUnit::ACTION_STATE::AIR_FINISH;
			unit->was_moved = true; // this will force range recalculation			
			// resort units in map
			SortUnits();
			// update this unit view
			RestoreUnitsView();
			SpellMapEventsList events;
			int new_contact = AddUnitView(unit, &events);
			if(new_contact)
			{
				// enemy contact event:
				unit->PlayContact();
			}
			// append events
			event_list.insert(event_list.end(),events.begin(),events.end());
			
			// check and append TransportUnit/SaveUnit events
			if(unit->trig_event && unit->trig_event->CheckUnitInPos(true))
				event_list.push_back(unit->trig_event);

			InvalidateHUDbuttons();
		}		

	}

	// try process pending events
	ProcEventsList(event_list);

	ReleaseMap();

	// repaint
	return(update);
}

// call this to set function inteface for creating message and polling state
void SpellMap::SetMessageInterface(SpellMessageCraeteFunPtr create_msg,SpellMessagePollingFunPtr msg_checker)
{
	m_msg_creator = create_msg;
	m_msg_checker = msg_checker;
}


// get unit by its id
MapUnit* SpellMap::GetUnit(int id)
{
	for(auto& unit : units)
		if(unit->id == id)
			return(unit);
	return(NULL);
}

// remove all units
int SpellMap::RemoveAllUnits()
{
	for(auto & unit : units)
		delete unit;
	units.clear();
	unit_selection = NULL;
	return(0);
}

// removes unit from map list (used e.g. for kamikaze attacks)
int SpellMap::RemoveUnit(MapUnit* unit)
{
	// try find unit in the list
	auto uid = find(units.begin(),units.end(),unit);
	if(uid == units.end())
		return(1); // not found

	// delete unit
	delete unit;
	units.erase(uid);

	// resort units
	SortUnits();

	return(0);
}

// extracts unit from map units list, but not deletes it (used to move unit elsewhere)
MapUnit* SpellMap::ExtractUnit(MapUnit* unit)
{
	// try find unit in the list
	auto uid = find(units.begin(),units.end(),unit);
	if(uid == units.end())
		return(NULL); // not found
	
	// remove from list
	units.erase(uid);
	
	// resort units
	SortUnits();

	return(unit);
}

// create new map unit (by parent unit)
int SpellMap::CreateUnit(MapUnit *parent, SpellUnitRec *new_type)
{
	// copy creator
	MapUnit *unit = new MapUnit(*parent);	

	// morhp to new type
	unit->MorphUnit(new_type,100);

	// set some initial parameters
	unit->type = MapUnitType::MissionUnit;
	unit->ResetAP();
	unit->ClearDigLevel();
	unit->ResetHealth();

	// try place unit to map
	if(PlaceUnit(unit))
	{
		delete unit;
		return(1);
	}

	// make link between parent and child
	unit->parent = parent;
	parent->child = unit;		
	
	return(0);
}

// add unit to map list (assuming there is no conflict)
int SpellMap::AddUnit(MapUnit* unit)
{
	units.push_back(unit);
	return(0);
}

// try to place new unit to map (looks for nearest suitable place)
int SpellMap::PlaceUnit(MapUnit* unit)
{
	// resort units map for fast colision checks
	SortUnits();

	vector<int> used(x_size*y_size,0);
	vector<MapXY> cand_list;
	cand_list.push_back(unit->coor);
	while(true)
	{
		int placed = false;
		for(auto& cand : cand_list)
		{
			auto pos_mxy = ConvXY(cand);
			// mark as tested position
			used[pos_mxy] = 1;

			// check colision with other untis
			MapUnit* obst = Lunit[pos_mxy];
			int fail = false;
			while(obst)
			{
				if(unit->unit->isAir() == obst->unit->isAir() || !unit->unit->isAir() == !obst->unit->isAir())
					fail = true;
				obst = obst->next;
			}
			if(fail)
				continue;

			if(!unit->unit->isAir())
			{
				// land unit: check obstacles
				if(!unit->unit->isWalk() && tiles[pos_mxy].flags == 0x90)
					continue;
				if(!unit->unit->isHover() && tiles[pos_mxy].flags == 0x60)
					continue;
				if(tiles[pos_mxy].flags && tiles[pos_mxy].flags != 0x90 && tiles[pos_mxy].flags != 0x60)
					continue;
			}

			// found viable position: place new unit and leave
			unit->coor = cand;
			placed = true;
			break;
		}
		if(placed)
			break;

		// not found yet: look around
		auto cand_temp = cand_list;
		cand_list.clear();
		for(auto& cand : cand_temp)
		{
			for(int k = 0; k < 4; k++)
			{
				auto n_pos = GetNeighborTile(cand,k);
				if(used[ConvXY(n_pos)])
					continue;
				used[ConvXY(n_pos)] = 1;
				cand_list.push_back(n_pos);
			}
		}
		if(cand_list.empty())
		{
			// not suitable place found in whole map
			return(1);
		}
	}

	// store new unit
	units.push_back(unit);
	
	// resort units map
	SortUnits();
	return(0);
}

// get pixel position of position origin in rendered map
tuple<int,int> SpellMap::GetPosOrigin(MapXY pos)
{
	if(!pos.IsSelected())
		return tuple(-1,-1);

	// this tile
	Sprite *spr = tiles[ConvXY(pos)].L1;
	int sof = tiles[ConvXY(pos)].elev;
	
	// tile center (in 2D)
	int x_org = spr->x_ofs + spr->x_size/2;
	int y_org = spr->y_ofs + spr->y_size/2;

	// calculate origin withing the map surface
	int mxx = pos.x * 80 + (((pos.y & 1) != 0) ? 0 : 40) + x_org;
	int myy = pos.y * 24 - sof * 18 + MSYOFS + 50 + y_org;

	return tuple(mxx,myy);
}

// Calculates angle from reference unit to taret unit in 2D rendered map
double SpellMap::GetUnitsAngle(MapUnit *ref, MapUnit *target)
{
	if(!ref || !target)
		return(0.0);

	auto [x_ref,y_ref] = GetPosOrigin(ref->coor);
	auto [x_targ,y_targ] = GetPosOrigin(target->coor);

	if(ref->unit->isAir())
		y_ref -= SpellUnitRec::AIR_UNIT_FLY_HEIGHT;
	if(target->unit->isAir())
		y_targ -= SpellUnitRec::AIR_UNIT_FLY_HEIGHT;

	return atan2(y_ref - y_targ,x_targ - x_ref)*180.0/M_PI;
}


// ----------------------------------------------------------------------------
// Events handling
// ----------------------------------------------------------------------------

// remove event created units from list
int SpellMap::ResetUnitEvents()
{
	// remove event units
	auto sel_unit = GetSelectedUnit();
	for(int k = units.size()-1; k > 0; k--)
	{
		auto unit = units[k];
		if(unit->is_event)
		{
			if(sel_unit == unit)
				SelectUnit(NULL);
			delete unit;
			units.erase(units.begin() + k);
		}
	}

	// resort untis map
	SortUnits();
		
	// try select some unit if not selected
	if(GetSelectedUnit())
		return(0);
	for(auto& unit : units)
		if(!unit->is_enemy)
		{
			SelectUnit(unit);
			return(0);
		}
	if(!units.empty())
		SelectUnit(units[0]);

	return(0);
}

// place mission start event units (if not done yet)
int SpellMap::MissionStartEvent()
{
	if(!isGameMode())
		return(1);
	
	// get list of events
	auto list = events->GetMissionStartEvent(true);
	if(list.empty())
		return(0);

	// process all events:
	int is_selected = false;
	for(auto & evt : list)
	{
		if(evt->units.empty())
			continue;

		// for each unit to place:
		for(auto & unit : evt->units)
		{
			// make new unit
			MapUnit *new_unit = new MapUnit(*unit.unit,true);
			unit.is_placed = true;
			
			if(PlaceUnit(new_unit))
			{
				// failed
				delete new_unit;
				continue;
			}
			if(!is_selected)
			{
				// select first unit, because Aliance unints in map can be only placed via events, so no valid initial selection is possible before this point
				is_selected = true;
				SelectUnit(new_unit);
			}
		}

		// check if everything is done and mark it as done eventually
		evt->isDone();
	}
	return(0);
}

// execute list of given events, remove finished from the list
int SpellMap::ProcEventsList(SpellMapEventsList &list)
{
	if(!isGameMode())
		return(1);

	while(!list.empty())
	{		
		auto evt = list.back();
		
		// place units:
		for(auto & unit : evt->units)
		{
			// skip if already placed
			if(unit.is_placed)
				continue;
			
			// make new unit						
			MapUnit* new_unit = new MapUnit(*unit.unit);
			unit.is_placed = true;
			if(PlaceUnit(new_unit))
			{
				delete new_unit;
				continue;
			}
		}

		// show messages
		for(auto & text : evt->texts)
		{
			// skip processed
			if(text.is_done)
				continue;
			
			// skip if message interface is still busy (previous message)
			if(m_msg_checker())
				continue;

			// show info message (no user interaction)
			m_msg_creator(text.text, false, NULL);
			text.is_done = true;
		}


		if(evt->isDone())
		{
			// entire event is done: remove it from pending list
			list.pop_back();
		}
		else
			break;
	}

	return(0);
}

// get first event at cursor position or NULL if not found
SpellMapEventRec* SpellMap::GetCursorEvent(TScroll* scroll)
{
	// find selection tiles
	auto msel = GetSelections(scroll);

	// no selection?
	if(!msel.size() || !msel[0].IsSelected())
		return(NULL);
	auto& pos = msel[0];

	// try to get event
	auto evt = events->GetEvent(pos);
	if(!evt)
	{
		// not found: check if there is MissionStart event unit		
		auto unit = Lunit[ConvXY(pos)];
		while(unit)
		{
			if(unit->map_event && unit->map_event->isMissionStart())
			{
				// found some
				evt = unit->map_event;
				break;
			}
			unit = unit->next;
		}
	}

	return(evt);
}

// select/deselect event
int SpellMap::SelectEvent(SpellMapEventRec* evt)
{
	selected_event = evt;
	return(0);
}
SpellMapEventRec* SpellMap::GetSelectEvent()
{
	return(selected_event);
}

// moves unit from map list to event or back
int SpellMap::UpdateEventUnit(SpellMapEventRec* evt,MapUnit* unit)
{
	// it the unit in map?
	auto free_unit = ExtractUnit(unit);
	if(free_unit)
	{
		// yaha: move unit from map to event
		evt->AddUnit(unit);
		SortUnits();
		events->ResetEvents();
		return(0);
	}
	
	// nope: look for it in event	
	free_unit = evt->ExtractUnit(unit);
	if(free_unit)
	{
		AddUnit(free_unit);
		SortUnits();
		events->ResetEvents();
		return(0);
	}

	
	
	
	// unit is not found, most likely belongs to other event, do nothing
	return(1);
}


// update destructible tiles after HP modification (changes graphics, eventually alters surrounding tiles (bridges))
vector<MapXY> SpellMap::UpdateDestructible(MapXY target_pos)
{	
	// target tile
	int mxy = ConvXY(target_pos);
	auto *tile = &tiles[mxy];

	// list of destroyed positions
	vector<MapXY> destroyed_list;

	if(tile->flags != 0x70)
	{
		// non bridge: just destruct object
		if(tile->UpdateDestructible())
			destroyed_list = {target_pos};
		return(destroyed_list);
	}
	
	vector<MapXY> list = {target_pos};
	vector<int> proc(x_size*y_size,0);
	proc[mxy] = true;
	vector<MapXY> update_list = {target_pos};

	// bridge object: copy HP to surorunding tiles, destruct all	
	do{
		vector<MapXY> next;
		for(auto & pos : list)
		{		
			for(int nid = 0; nid < 4; nid++)
			{
				auto n_pos = GetNeighborTile(pos, nid);
				int nxy = ConvXY(n_pos);
				if(!n_pos.IsSelected())
					continue;

				// skip if already processed
				if(proc[nxy])
					continue;
				proc[nxy] = true;
		
				if(tiles[nxy].flags == 0x70)
				{
					// other desctructible part of bridge: copy HP and list for update
					tiles[nxy].hp = tile->hp;
					update_list.push_back(n_pos);
					next.push_back(n_pos);
				}
				else if(tiles[nxy].flags == 0x60)
				{
					// non-destructible part of bridge: just list for update
					tiles[nxy].hp = tile->hp;
					update_list.push_back(n_pos);
				}
			}		
		}
		list = next;
	}while(!list.empty());

	// update tiles
	for(auto & u_tile : update_list)
	{
		auto *mod_tile = &tiles[ConvXY(u_tile)];
		if(mod_tile->UpdateDestructible())
			destroyed_list.push_back(u_tile);
	}

	return(destroyed_list);
}

// get destructible list matching target position (mainly used for exploding bridges)
vector<MapSprite*> SpellMap::GetDestructibleList(MapXY target_pos)
{
	// target tile
	int mxy = ConvXY(target_pos);
	auto* tile = &tiles[mxy];

	// affected tiles
	vector<MapSprite*> update_list;

	if(tile->flags != 0x70)
	{
		// non bridge: just destruct object
		return(update_list);
	}

	vector<MapXY> list = {target_pos};
	vector<int> proc(x_size*y_size,0);
	proc[mxy] = true;

	// bridge object: copy HP to surorunding tiles, destruct all	
	do {
		vector<MapXY> next;
		for(auto& pos : list)
		{
			for(int nid = 0; nid < 4; nid++)
			{
				auto n_pos = GetNeighborTile(pos,nid);
				int nxy = ConvXY(n_pos);
				if(!n_pos.IsSelected())
					continue;

				// skip if already processed
				if(proc[nxy])
					continue;
				proc[nxy] = true;

				if(tiles[nxy].flags == 0x70)
				{
					// other desctructible part of bridge: copy HP and list for update
					update_list.push_back(&tiles[nxy]);
					next.push_back(n_pos);
				}
			}
		}
		list = next;
	} while(!list.empty());

	return(update_list);
}




// ----------------------------------------------------------------------------
// Map State Save
// ----------------------------------------------------------------------------
SpellMap::SavedState::SavedState()
{	
	events = NULL;
	sel_unit = NULL;
}
SpellMap::SavedState::~SavedState()
{
	Clear();
}
// clear save position
void SpellMap::SavedState::Clear()
{
	for(auto& unit : units)
		delete unit;
	units.clear();
	units_view.clear();
	sel_unit = NULL;
	tiles.clear();
	if(events)
	{
		events->ClearEvents();
		delete events;
		events = NULL;
	}
}
// get unit by its id
MapUnit* SpellMap::SavedState::GetUnit(int id)
{
	for(auto& unit : units)
		if(unit->id == id)
			return(unit);
	return(NULL);
}

SpellMap::Saves::Saves(SpellMap* parent)
{
	map = parent;
	max_saves = 0;
}
SpellMap::Saves::~Saves()
{
	for(auto& save : saves)
		delete save;
	saves.clear();
}
// clear saves, set saves count
int SpellMap::Saves::Clear(int count)
{	
	for(auto & save : saves)
		delete save;
	saves.clear();
	max_saves = count;	
	return(0);
}
// how many times can we save?
int SpellMap::Saves::canSave()
{
	return(max_saves - saves.size());
}
// can load state?
int SpellMap::Saves::canLoad()
{
	return(!saves.empty());
}

// save state to list
int SpellMap::Saves::Save(SpellMap::SavedState *slot)
{	
	SpellMap::SavedState *save = slot;	
	if(!save)
	{
		// add save slot if not provided explicitely
		saves.push_back(new SpellMap::SavedState());
		save = saves.back();
	}

	map->LockMap();

	// copy destructible tiles
	save->tiles = map->tiles;

	// make blank copy of units (no linking to events)
	save->sel_unit = NULL;
	for(auto & unit : map->units)
	{		
		MapUnit *copy = new MapUnit(*unit, false);		
		save->units.push_back(copy);
		// store selected unit
		if(map->GetSelectedUnit() == unit)
			save->sel_unit = copy;
	}
			
	// make blank copy of events
	save->events = new SpellMapEvents(map);
	for(auto& evt : map->events->GetEvents())
		save->events->AddEvent(new SpellMapEventRec(evt));

	// relink inter-units links and links units-events
	for(int uid = 0; uid < map->units.size(); uid++)
	{
		MapUnit *orig = map->units[uid];
		MapUnit *copy = save->units[uid];

		// relink parent/child unit to new list
		if(orig->parent)
			copy->parent = save->GetUnit(orig->parent->id);
		if(orig->child)
			copy->child = save->GetUnit(orig->child->id);
		
		// relink events links to new events list
		if(orig->map_event)
			copy->map_event = save->events->GetEvent(map->events->GetEventID(orig->map_event));
	}

	// save units view
	save->units_view = map->units_view;

	map->ReleaseMap();

	return(0);
}

// load save position (by default last one)
int SpellMap::Saves::LoadByID(int index)
{
	// select save slot
	int save_id = saves.size()-1;
	if(index >= 0)
		save_id = index;
	if(save_id < 0 || save_id >= saves.size())
		return(1); // out of range
	SpellMap::SavedState *save = saves[save_id];
	
	// load slot 
	return(Load(save));
}
// load saved position from explicit slot (by default last one)
int SpellMap::Saves::Load(SpellMap::SavedState* save)
{
	if(!save)
	{
		// get last save slot of not provided
		if(canLoad())
			save = saves.back();
		else
			return(1);
	}

	map->LockMap();
		
	// replace tiles
	map->tiles = save->tiles;
	
	// replace events	
	map->events->ClearEvents();
	for(auto& evt : save->events->GetEvents())
		map->events->AddEvent(new SpellMapEventRec(evt));

	// replace units
	map->RemoveAllUnits();
	for(auto& unit : save->units)
	{
		MapUnit* copy = new MapUnit(*unit,false);
		map->AddUnit(copy);
		if(unit == save->sel_unit)
			map->SelectUnit(copy);
	}

	// relink inter-units links and links units-events
	for(int uid = 0; uid < save->units.size(); uid++)
	{
		MapUnit* copy = map->units[uid];
		MapUnit* orig = save->units[uid];

		// relink parent/child unit to new list
		if(orig->parent)
			copy->parent = map->GetUnit(orig->parent->id);
		if(orig->child)
			copy->child = map->GetUnit(orig->child->id);

		// relink events links to new events list
		if(orig->map_event)
			copy->map_event = map->events->GetEvent(save->events->GetEventID(orig->map_event));
	}

	// deselect event
	map->SelectEvent(NULL);

	// relink event units
	map->events->RelinkUnits();

	// resort units to tiles
	map->SortUnits();

	// update units view
	map->units_view = save->units_view;
	map->units_view_mem = save->units_view;
	map->PrepareUnitsViewMask();
	
	map->ReleaseMap();
	
	return(0);
}

// save initial state (used when entering game mode)
int SpellMap::Saves::SaveInitial()
{
	initial.Clear();
	return(Save(&initial));
}

// restore initial state (used when leaving game mode)
int SpellMap::Saves::LoadInitial()
{
	return(Load(&initial));
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
			int nelv = tiles[ConvXY(nxy)].elev;
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
			int nelv = tiles[ConvXY(nxy)].elev;
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
		tiles[mxy].elev = elv;
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
		int nelv = tiles[ConvXY(nxy)].elev;
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
			tiles[ConvXY(nxy)].elev = nelv;
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
			int nelv = tiles[ConvXY(nxy)].elev;

			// align if neighbour higher
			tiles[ConvXY(nxy)].elev = elv;
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
			int nelv = tiles[ConvXY(nxy)].elev;

			if(nelv > elv)
			{
				// align if neighbour higher
				tiles[ConvXY(nxy)].elev = elv;
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
		int nelv = tiles[ConvXY(nxy)].elev;

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
				int alt = tiles[ConvXY(i,j)].elev;

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
						int nalt = tiles[ConvXY(nxy)].elev;
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
				tiles[ConvXY(i,j)].SetL1(sprite);
				
				// remove objects?
				if(slp != 'A')
				{
					tiles[ConvXY(i,j)].SetL2(NULL,0);
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
				int alt = tiles[ConvXY(i,j)].elev;

				Sprite *sprite = tiles[ConvXY(i,j)].L1;
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
						int nalt = tiles[ConvXY(nxy)].elev;

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

					if(tiles[ConvXY(dxy)].elev == alt)
						msk |= (1<<sid);
										
					Sprite *sprite = tiles[ConvXY(uxy)].L1;
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
					
					if(tiles[ConvXY(dnaxy)].elev == alt)
						msk |= (1<<((sid+3)%4));
					if(tiles[ConvXY(dnbxy)].elev == alt)
						msk |= (1<<((sid+0)%4));

					Sprite* sprite = tiles[ConvXY(uxy)].L1;
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
					Sprite* spr1 = tiles[ConvXY(axy)].L1;
					MapXY bxy = GetTileNeihgborXY(i,j,unb);								
					Sprite* spr2 = tiles[ConvXY(bxy)].L1;
					if(spr1->GetSlope() == 'A' && spr2->GetSlope() == 'A')
					{
						msk |= (1<<((sid+0)%4));
						msk |= (1<<((sid+3)%4));
					}

					MapXY dxy = GetTileNeihgborXY(i,j,did);
					if(tiles[ConvXY(dxy)].elev == alt)
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
					tiles[ConvXY(i,j)].SetL1(spr);
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
int SpellMap::EditElev(TScroll* scroll, int step)
{
	int i,j;

	// leave if map not loaded
	if(!IsLoaded())
		return(1);

	// update map L1 flags
	SyncL1flags();

	// get selection tiles
	auto msel = GetSelections(scroll);
	if(!msel[0].IsSelected())
		return(2);

	// === for each selected ===
	int min = ELEV_MAX;
	int max = ELEV_MIN;
	for(i = 0; i < msel.size(); i++)
	{
		// selected tile
		int mxy = ConvXY(msel[i]);
		Sprite *sprite = tiles[mxy].L1;

		// tile slope
		char sn = sprite->name[2];

		// normal tile - use its alt
		int elv = tiles[mxy].elev;
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
				int nalt = tiles[ConvXY(nxy)].elev;

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
		Sprite* sprite = tiles[mxy].L1;

		// tile slope
		char sn = sprite->name[2];

		if(step > 0 || sn == 'A')
		{
			// update alt
			int elv = tiles[mxy].elev;
			int elv_o = elv;
			if((step > 0 && elv == min) || (step < 0 && elv == max))
			{
				elv += step;
				if(elv < ELEV_MIN)
					elv = ELEV_MIN;
				if(elv > ELEV_MAX-1)
					elv = ELEV_MAX-1;
				tiles[mxy].elev = elv;

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
				int alt = tiles[ConvXY(nxy)].elev;
				int alt_o = alt;

				if(alt == max)
				{
					alt += step;
					if(alt < ELEV_MIN)
						alt = ELEV_MIN;
					if(alt > ELEV_MAX-1)
						alt = ELEV_MAX-1;
					tiles[ConvXY(nxy)].elev = alt;
					
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
		int elv = tiles[ConvXY(x,y)].elev;

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

				Sprite *spr = tiles[mxy].L1;
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
						Sprite* neig = tiles[nxy].L1;
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
					tiles[mxy].SetL1(cand_map[mxy][0]);
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
					auto neig_slope = tiles[nxy].L1->GetSlope();

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
					tiles[mxy].SetL1(spr);
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
				Sprite *spr = tiles[ConvXY(mxy)].L1;
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
					Sprite *neig = tiles[ConvXY(nxy)].L1;

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

						int neig_slope = tiles[ConvXY(nxy)].L1->GetSlope();
					
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
				tiles[ConvXY(mxy)].SetL1(spr);
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
			L1_flags[ConvXY(x,y)] = tiles[ConvXY(x,y)].L1->GetFlags();
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
					ref.name[2] = tiles[mxy].L1->name[2];					
					neig.name[2] = tiles[nxy].L1->name[2];

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
			Sprite *tile = tiles[mxy].L1;
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
					Sprite* sprite = tiles[ConvXY(nxy)].L1;
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
			tiles[pxy].SetL1(terrain->GetTileGlyph(tspr, flag));
			if(!tiles[pxy].L1)
				tiles[pxy].SetL1(terrain->GetSprite("START"));

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




