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

#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <regex>
#include <tuple>

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
		dx = x_limit - 1;
	else if(dx < 0)
		dx = 0;
	if(dy >= y_limit)
		dy = y_limit - 1;
	else if(dy < 0)
		dy = 0;
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
	this->frame_ofs = frame_ofs;
	this->frame_limit = frame_limit;
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
	this->frame_limit = frame_limit;
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
	L1 = NULL;
	L2 = NULL;
	elev = NULL;
	flags = NULL;
	Lunit = NULL;

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
}

SpellMap::~SpellMap()
{
	// cleanup heap allocated stuff
	Close();
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

	if(pic)
		delete[] pic;
	pic = NULL;
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
			delete[] map_buffer;
			return(1);
		}
	}

	// load L1 sprite indices
	this->L1 = new Sprite * [x_size*y_size];
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
			delete[] map_buffer;
			return(1);
		}
		this->L1[k] = sprites[sid];
		this->elev[k] = elev;
	}
	// get rid of L1 sprites list
	delete[] sprites;
	




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


	//////////////////////////
	///// Load DEF stuff /////
	//////////////////////////
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

				// try fetch unit record from spelldata
				unit->unit = spelldata->units->GetUnit(unit->type_id);				
				if (!unit->unit)
				{
					delete mission_data;
					delete unit;
					return(1);
				}

				// add unit to list
				units.push_back(unit);
			}
			else if (cmd->name.compare("AddSpecialEvent") == 0)
			{
				// --- Event definitions: AddSpecialEvent(type, position, index, probability) ---
				
				if ((*cmd->parameters)[0].compare("SeePlace"))
					continue;

				// event position xy
				MapXY coor;
				int xy = atoi((*cmd->parameters)[1].c_str());
				coor.y = (xy / x_size);
				coor.x = xy - coor.y * x_size;
				
				// event index
				int index = atoi((*cmd->parameters)[2].c_str());

				// probability
				int probab = atoi((*cmd->parameters)[3].c_str());
				
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
						unit->id = atoi((*evcmd->parameters)[0].c_str());

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

						// try fetch unit record from spelldata
						unit->unit = spelldata->units->GetUnit(unit->type_id);
						if (!unit->unit)
						{
							delete event_data;
							delete unit;
							return(1);
						}

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

	// assign the units to map array
	SortUnits();

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
	if (!L1 || !L2 || !elev || !flags || !Lunit)
		return(0);

	if (!x_size || !y_size)
		return(0);

	// ###todo: should be checked more rigorously...
	return(1);
}





// selection is valid?
bool MapXY::IsSelected()
{
	return(x >= 0 && y >= 0);
}


// call whenever scroll or screen changes
// it allocates render buffer and fixes scroller limits
int SpellMap::RenderPrepare(wxBitmap& bmp, TScroll* scroll)
{
	// get surface size
	surf_x = bmp.GetWidth();
	surf_y = bmp.GetHeight();

	// allocate maximum indexed image memory for entire map
	pic_x_size = (x_size) * 80 + 40;
	pic_y_size = MSYOFS + 60 + 47 + (y_size) * 24 + MSYOFS;
	if(!pic)
		pic = new uint8_t[pic_x_size * pic_y_size];
	if(!pic)
		return(1);
	// buffer end for range checking
	pic_end = &pic[pic_x_size * pic_y_size];

	// limit scroller to valid range
	scroll->CheckScroll(pic_x_size - surf_x, pic_y_size - surf_y);
		
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






// analyze map selection
vector<MapXY> &SpellMap::GetSelections(wxBitmap& bmp, TScroll *scroll)
{
	int m,n,i,j;

	// check & fix surface and scroll ranges
	RenderPrepare(bmp, scroll);
	
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
				if(n + xs_ofs >= x_size - 1)
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
				int mxx = n * 80 + ((m & 1 != 0) ? 0 : 40);
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

	return(msel);
}
MapXY SpellMap::GetSelection(wxBitmap& bmp,TScroll* scroll)
{
	vector<MapXY>& sel = GetSelections(bmp, scroll);
	return(sel[0]);
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

// render frame
int SpellMap::Render(wxBitmap &bmp, TScroll* scroll)
{
	int i, j;
	int m, n;

	// ###todo: always smooth scroll?
	int smooth = 1;

	// skip if invalid pointer
	if (!IsLoaded())
		return(1);

	// prepare scroll & buffer
	if(RenderPrepare(bmp, scroll))
		return(1);

	// find selection tiles
	auto msel = GetSelections(bmp,scroll);
		
	// visible tiles range to clear
	m = surf_x + 80 * 2;
	if (m + 80 > pic_x_size)
		m = surf_x - 80;
	n = surf_y + MSYOFST * 48;
	if (n > +48 > pic_y_size)
		n = pic_y_size - 48;
	// clear visible tiles range
	for (i = 0; i < n; i++)
		std::memset((void*)(pic + pic_x_size * (i + ((ys_ofs >= 1) ? 48 : 0)) + ((xs_ofs * 80 >= 1) ? 80 : 0)), MAP_BACK_COLOR, m);
	

	// --- Render Layer 1 - ground sprites ---
	for (m = 0; m < ys_size; m++)
	{
		if (m + ys_ofs * 2 >= y_size)
			break;
		for (n = 0; n < xs_size; n++)
		{
			if (n + xs_ofs >= x_size - 1)
				break;

			// select view area
			uint8_t *filter = NULL;
			if(!TileIsVisible(n + xs_ofs,m + ys_ofs*2))
				filter = terrain->filter.darkpal2;

			// get sprite parameters
			int mxy = ConvXY(n + xs_ofs,m + ys_ofs*2);
			Sprite* sid = L1[mxy];
			int sof = elev[mxy];

			// override sprite by plain one?
			if (!wL1)
			{
				char sn = sid->name[2];
				sid = &spelldata->special.grid[sn - 'A'];
			}

			// render sprite
			int mxx = n * 80 + ((m & 1 != 0) ? 0 : 40);
			int myy = m * 24 - sof * 18 + MSYOFS + 50;
			sid->Render(pic, pic_end, mxx, myy, pic_x_size, filter);
		}
	}

	// render 3D terrain mesh overlay (for debug only)
	/*for (m = 0; m < ysz; m++)
	{
		if (m + yss * 2 >= y_size)
			break;
		for (n = 0; n < xsz; n++)
		{
			if (n + xss >= x_size - 1)
				break;
			// get sprite parameters
			Sprite* sid = L1[(m + yss * 2) * x_size + n + xss];
			int sof = elev[(m + yss * 2) * x_size + n + xss];

			// render sprite
			int mxx = n * 80 + ((m & 1 != 0) ? 0 : 40);
			int myy = m * 24 - sof * 18 + MSYOFS + 50;

			sid->RenderTileWires(pic, pice, mxx, myy, pic_x_size, 252);
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

			// L1 elevation
			int sof = elev[(anm->y_pos) * x_size + anm->x_pos];

			// get sprite
			Sprite* frame = anm->anim->frames[anm->frame_ofs];

			// render sprite
			int mxx = (anm->x_pos - xs_ofs) * 80 + (((anm->y_pos - ys_ofs * 2) & 1 != 0) ? 0 : 40);
			int myy = (anm->y_pos - ys_ofs * 2) * 24 - sof * 18 + MSYOFS + 50;
			frame->Render(pic, pic_end, mxx, myy, pic_x_size);
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

				// L1 elevation
				int y_elev = elev[(pos->y) * x_size + pos->x];

				// render sprite
				int mxx = (pos->x - xs_ofs) * 80 + (((pos->y - ys_ofs * 2) & 1 != 0) ? 0 : 40);
				int myy = (pos->y - ys_ofs * 2) * 24 - y_elev * 18 + MSYOFS + 50;
				spec_sprite[sid]->Render(pic, pic_end, mxx, myy, pic_x_size);

			}
		}
	}



	// --- Render selection tiles ---
	if (msel[0].IsSelected())
	{
		/*struct {
			int seln;
			struct {
				int x, y;
			}sel[1];
		} msel;
		msel.seln = 1;
		msel.sel[0].x = sel.x;
		msel.sel[0].y = sel.y;*/
		
		for (i = 0; i < msel.size(); i++)
		{
			n = msel[i].x - xs_ofs;
			m = msel[i].y - ys_ofs * 2;

			if (n < 0 || m < 0 || (m + ys_ofs * 2) >= y_size || (n + xs_ofs) >= x_size - 1)
				continue;
			int mxy = ConvXY(n + xs_ofs, m + ys_ofs*2);

			// get map tile
			Sprite *sid = L1[mxy];
			int sof = elev[mxy];

			// get sprite
			char sn = sid->name[2];

			// override sprite by selector
			sid = &spelldata->special.select[sn - 'A'];

			// draw selection sprite
			int mxx = n * 80 + ((m & 1 != 0) ? 0 : 40);
			int myy = m * 24 - sof * 18 + MSYOFS + 50;
			sid->Render(pic, pic_end, mxx, myy, pic_x_size);

			
			/*SpellUnitRec* unit = spelldata->units->GetUnit(68);
			unit->Render(pic, pice, mxx, myy, pic_x_size, terrain->filter.darkpal2, sid, 0);*/
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

			// L1 elevation
			int y_elev = elev[(pnm->y_pos) * x_size + pnm->x_pos];
			
			// get sprite
			Sprite* frame = pnm->anim->frames[pnm->frame_ofs];

			// render sprite
			int mxx = (pnm->x_pos - xs_ofs)*80 + pnm->x_ofs + (((pnm->y_pos - ys_ofs*2) & 1 != 0) ? 0 : 40);
			int myy = (pnm->y_pos - ys_ofs * 2) * 24 + pnm->y_ofs - y_elev*18 + MSYOFS + 50;
			frame->Render(pic, pic_end, mxx, myy, pic_x_size);
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
			if (n + xs_ofs >= x_size - 1)
				break;
			int mxy = ConvXY(n + xs_ofs, m + ys_ofs * 2);

			// select view area
			uint8_t* filter = NULL;
			if(!TileIsVisible(n + xs_ofs,m + ys_ofs*2))
				filter = terrain->filter.darkpal2;

			// tile selected
			int is_selected = (msel[0].IsSelected() && msel[0].x - xs_ofs == n && msel[0].y - ys_ofs*2 == m);

			// first unit to render
			MapUnit* unit = Lunit[mxy];			
			// L1 tile
			Sprite* sid = L1[mxy];
			int sof = elev[mxy];
			char slope = sid->name[2];
			// L2 tile
			Sprite* sid2 = L2[mxy];

			// render origin
			int mxx = n * 80 + ((m & 1 != 0) ? 0 : 40);
			int myy = m * 24 - sof * 18 + MSYOFS + 50/* + sid->y_ofs*/;

			// 3 pass render: 1) air units, 2) objects, 3) rest of units
			for (int pass = 0; pass < 3; pass++)
			{										
				// units render:
				while (wUnits && unit)
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
						unit->unit->Render(pic, pic_end, mxx, myy, pic_x_size, terrain->filter.darkpal2, sid, azimuth, frame);
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

	
	// get scroll position
	auto [x_pos,y_pos] = scroll->GetScroll();
	
	// render 24bit RGB data to raw bmp buffer
	wxNativePixelData data(bmp);
	wxNativePixelData::Iterator p(data);
	for(unsigned int y = 0; y < surf_y; ++y)
	{
		uint8_t* scan = p.m_ptr;
		uint8_t* src = &pic[(y + (y_pos / 48 <= MSYOFST ? (y_pos / 48) * 48 : MSYOFST * 48) + (y_pos % 48) * smooth)*pic_x_size + ((x_pos >= 80 ? (80) : (0)) + (x_pos % 80) * smooth)];		
		for(int x = 0; x < surf_x; x++)
		{
			*scan++ = pal[*src][2];
			*scan++ = pal[*src][1];
			*scan++ = pal[*src][0];
			src++;
		}
		p.OffsetY(data,1);
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













typedef struct {
	short x,y;
	short elv;
	short edir;
}TMEEN;

#define ALTEDBG 0

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
	if(angle < 0 || angle > 7)
		return(nxy);
	nxy.x = x + ((y&1)?xlstb[angle]:xlsta[angle]);
	nxy.y = y + ylst[angle];

	return(nxy);
}

// change elevation of terrain by step for all selected tiles
int SpellMap::EditElev(wxBitmap& bmp, TScroll* scroll, int step)
{
	//TMap* map=&cmap;
	//int x,y;
	int i,j;

	// leave if map not loaded
	if(!IsLoaded())
		return(1);

	// get selection tiles
	auto msel = GetSelections(bmp, scroll);
	if(!msel[0].IsSelected())
		return(2);

	// select terrain
	/*TTerr* TT;
	MapGetTerr((void**)&TT);*/

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
		//int sid=MapL1getId(map,x,y);
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
	std::memset((void*)flag, 0x00,x_size* y_size*sizeof(uint8_t));

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
	for(i = 0; i < elist.size(); i++)
	{
		if(elist[i].edir != 0)
		{
			// update land textures
			EditElevText(flag);
		}
	}


	// loose memory
	delete[] flag;

	//DrawCMapSectorForce();

	return(0);
}