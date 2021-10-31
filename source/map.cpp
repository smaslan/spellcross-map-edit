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

using namespace std;



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
	int k;
	if (L1)
		delete[] L1;
	L1 = NULL;
	if (L2)
		delete[] L2;
	L2 = NULL;
	if (elev)
		delete[] this->elev;
	elev = NULL;
	if (flags)
		delete[] flags;
	flags = NULL;	
	L3.clear();
	L4.clear();
	start.clear();
	escape.clear();
	if (Lunit)
		delete[] Lunit;
	for (k = 0; k < units.size(); k++)
		delete units[k];
	units.clear();

	if (pic)
		delete[] pic;
	pic = NULL;

}

// returns path to DEF file or DTA file if DEF was not used
wstring SpellMap::GetTopPath()
{
	if (def_path.empty())
		return(map_path);
	return(def_path);
}

// Load MAP from file
int SpellMap::Load(wstring &path, SpellData *spelldata)
{		
	// default map file
	wstring map_path = path;

	// DEF file
	SpellDEF *def;

	if (path.rfind(L".DEF") != wstring::npos)
	{
		// --- this is DEF file: parse it ---

		// load DEF file
		def = new SpellDEF(path);

		// parse mission data
		vector<SpellDefCmd*> mission_data = def->GetSection("MissionData");
		if (!mission_data.size())
		{
			// likely not a valid DEF file
			return(1);
		}

		for (int k = 0; k < mission_data.size(); k++)
		{
			if (mission_data[k]->name.compare("MissionMap") == 0)
			{
				// --- *.MAP file definition ---
				size_t pos = map_path.rfind(L"\\", 260);
				if (pos != wstring::npos)
					map_path.resize(pos);
				else
					map_path = L"";
				string map_name = (*mission_data[k]->parameters)[0];
				map_path = map_path + L"\\" + wstring(map_name.begin(), map_name.end()) + L".DTA";
			}			
		}
		mission_data.clear();

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
		if (!anims[k])
		{
			// not found!
			delete[] anims;
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
		vector<SpellDefCmd*> mission_data = def->GetSection("MissionData");

		for (int k = 0; k < mission_data.size(); k++)
		{
			SpellDefCmd* cmd = mission_data[k];
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
				vector<SpellDefCmd*> event_data = def->GetSection(event_data_header);

				// for each item in event:
				for (int evid = 0; evid < event_data.size(); evid++)
				{
					SpellDefCmd* evcmd = event_data[evid];

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
							delete unit;
							return(1);
						}

						// add unit to list
						units.push_back(unit);
					}
				}
				// clear parsed event data
				event_data.clear();

			}
		}
		mission_data.clear();

		// initialize units state variables:
		for (int k = 0; k < units.size(); k++)
		{
			units[k]->azimuth = rand() % units[k]->unit->gr_base->stat.azimuths;
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
	return(x + y * x_size);
}
int SpellMap::ConvXY(MapXY &mxy)
{
	return(mxy.x + mxy.y * x_size);
}

// this sorts units list for proper render order, call after load or units changes
void SpellMap::SortUnits()
{
	int k;
	// allocate units layer array
	if (!Lunit)
		Lunit = new MapUnit*[x_size * y_size];
	
	// clear units layer array
	memset((void*)Lunit, NULL, sizeof(MapUnit*)*x_size*y_size);

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

int SpellMap::Render(System::Drawing::Bitmap ^bmp, int *x_pos, int *y_pos, int x_cursor, int y_cursor)
{
	int i, j;
	int m, n;

	// ###todo: always smooth scroll?
	int smooth = 1;

	// skip if invalid pointer
	if (!IsLoaded())
		return(1);

	// get surface size
	surf_x = bmp->Width;
	surf_y = bmp->Height;
	if (!last_surf_x || last_surf_x != surf_x || !last_surf_y || last_surf_y != surf_y)
	{
		// surface size changed
	}


#define MSYOFS 150
#define MSYOFST 6

	// allocate maximum indexed image memory
	pic_x_size = (x_size) * 80 + 40;
	pic_y_size = MSYOFS + 60 + 47 + (y_size) * 24 + 150;
	if (!pic)
		pic = new uint8_t[pic_x_size * pic_y_size];
	if (!pic)
	{
		return(1);
	}
	uint8_t* pice = &pic[pic_x_size * pic_y_size];

	// limit scroller to valid range
	if (*x_pos >= pic_x_size - surf_x)
		*x_pos = pic_x_size - surf_x - 1;
	else if (*x_pos < 0)
		*x_pos = 0;
	if (*y_pos >= pic_y_size - surf_y)
		*y_pos = pic_y_size - surf_y - 1;
	else if (*y_pos < 0)
		*y_pos = 0;
	


	// tiles to draw
	int xsz = surf_x / 80 + 3;
	int ysz = surf_y / 24 + 12;

	// scroll offset in tiles
	int xss = *x_pos / 80 - 1;
	int yss = *y_pos / 48 - MSYOFST;
	if (yss < 0)
		yss = 0;
	if (xss < 0)
		xss = 0;

	// visible tiles range to clear
	m = surf_x + 80 * 2;
	if (m + 80 > pic_x_size)
		m = surf_x - 80;
	n = surf_y + MSYOFST * 48;
	if (n > +48 > pic_y_size)
		n = pic_y_size - 48;

#define MAP_BACK_COLOR 230 /* map background color index */

	// clear visible tiles range
	for (i = 0; i < n; i++)
		std::memset((void*)(pic + pic_x_size * (i + ((yss >= 1) ? 48 : 0)) + ((xss * 80 >= 1) ? 80 : 0)), MAP_BACK_COLOR, m);


	// find selection tiles
	int selx = -1;
	int sely = -1;
	for (m = 0; m < ysz; m++)
	{
		if (m + yss * 2 >= y_size)
			break;
		for (n = 0; n < xsz; n++)
		{
			if (n + xss >= x_size - 1)
				break;
			// --- for every valid tile ---

			// get map tile
			Sprite* sid = L1[(m + yss * 2) * x_size + n + xss];
			int sof = elev[(m + yss * 2) * x_size + n + xss];

			// get dimensions for current tile slope
			char sn = sid->name[2]; /* tile slope */
			int so = sid->y_ofs; /* vertical offset */
			int sx = spelldata->special.solid[sn - 'A'].x_size;
			int sy = spelldata->special.solid[sn - 'A'].y_size;


			// pixel coordinates of search rect
			int mxx = n * 80 + ((m & 1 != 0) ? 0 : 40);
			int ymn = -((*y_pos / 48 <= MSYOFST ? (*y_pos / 48) * 48 : MSYOFST * 48) + (*y_pos % 48) * smooth);
			int xmn = mxx - ((*x_pos >= 80 ? (80) : (0)) + (*x_pos % 80) * smooth);
			if (sn == 'C' || sn == 'D' || sn == 'M' || sn == 'B')
				ymn += (m * 24 - sy + 47 - sof * 18 + MSYOFS + 50);
			else if (sn == 'L' || sn == 'F')
				ymn += (m * 24 - sy + 29 - sof * 18 + MSYOFS + 50);
			else
				ymn += (m * 24 - sof * 18 + MSYOFS + 50);

			// now decide which tile is selected in search rect
			if (y_cursor != -1 && y_cursor > ymn && y_cursor<ymn + sy &&
				x_cursor != -1 && x_cursor>xmn && x_cursor < xmn + sx)
			{
				// tile data				
				unsigned char* seld = spelldata->special.solid[sn - 'A'].data;
				int sla = 0;
				for (i = 0; i < sy; i++)
				{
					unsigned char oo = *((unsigned int*)seld); seld += 4;
					unsigned char nn = *((unsigned int*)seld); seld += 4;
					if (nn == 0)
						continue;
					if ((y_cursor - ymn) == i && (x_cursor - xmn) >= oo && (x_cursor - xmn) < (oo + nn) &&
						seld[x_cursor - xmn - oo] != 0)
					{
						sla = 1;
						break;
					}
					seld += nn;
				}

				// some valid?
				if (sla)
				{
					// yaha, remember which one and leave
					selx = n;
					sely = m;
					goto seldone;
				}
			}
		}
	}
seldone:

	// refresh statusbar
	/*if (selx == -1)
	{
		msel.x = -1;
		msel.y = -1;
		MForm->SB->Panels->Items[0]->Text = "x=";
		MForm->SB->Panels->Items[1]->Text = "y=";
		MForm->SB->Panels->Items[2]->Text = "xy=";
		MForm->SB->Panels->Items[3]->Text = "L1=";
		MForm->SB->Panels->Items[4]->Text = "L2=";
	}
	else
	{
		// central select
		msel.x = xss + selx;
		msel.y = yss * 2 + sely;

		// extend to multiselect
		msel.seln = MAP_SEL_MAX * MAP_SEL_MAX;
		MapSelectList(map, msel.sel, &msel.seln, msel.x, msel.y, -(tscr.size - 1) / 2, -(tscr.size - 1) / 2, tscr.size, tscr.size);


		sid = map->L1[msel.y * map->x + msel.x];
		sof = map->L1h[msel.y * map->x + msel.x];
		sid2 = map->L2[msel.y * map->x + msel.x];
		sof2 = map->L2h[msel.y * map->x + msel.x];

		sprintf(str, "x=%d", msel.x);
		MForm->SB->Panels->Items[0]->Text = str;
		sprintf(str, "y=%d", msel.y);
		MForm->SB->Panels->Items[1]->Text = str;
		sprintf(str, "xy=%d", msel.y * map->x + msel.x);
		MForm->SB->Panels->Items[2]->Text = str;
		sprintf(str, "L1=%s (%d)", TT->spr[sid].name, sof);
		MForm->SB->Panels->Items[3]->Text = str;
		if (sid2 == 0)
		{
			sprintf(str, "L2= (0x%02X)", sof2);
			MForm->SB->Panels->Items[4]->Text = str;
		}
		else
		{
			sprintf(str, "L2=%s (0x%02X)", TT->spr[sid2 - 1].name, sof2);
			MForm->SB->Panels->Items[4]->Text = str;
		}
	}
	MForm->SB->Repaint();*/

	// leave if no change
	/*if (selx == lselx && sely == lsely &&
		xsz == lxsz && ysz == lysz &&
		xss == lxss && yss == lyss &&
		!map->dforce && !(smooth && (lxps != x_pos || lyps != y_pos)))
	{
		return(0);
	}
	// remember last selection
	map->dforce = 0;
	lxps = tscr.xpos;
	lyps = tscr.ypos;
	lselx = selx;
	lsely = sely;
	lxsz = xsz;
	lysz = ysz;
	lxss = xss;
	lyss = yss;*/



	// --- Render Layer 1 - ground sprites ---
	for (m = 0; m < ysz; m++)
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

			// override sprite by plain one?
			if (!wL1)
			{
				char sn = sid->name[2];
				sid = &spelldata->special.grid[sn - 'A'];
			}

			// render sprite
			int mxx = n * 80 + ((m & 1 != 0) ? 0 : 40);
			int myy = m * 24 - sof * 18 + MSYOFS + 50;
			sid->Render(pic, pice, mxx, myy, pic_x_size);
		}
	}

	
	// --- Redner Layer 3 - ANM animations ---
	if (wL3)
	{
		for (i = 0; i < L3.size(); i++)
		{
			MapLayer3* anm = L3[i];

			// skip if not in visible area
			if (anm->x_pos < xss || anm->x_pos >= (xss + xsz) || anm->y_pos < yss * 2 || anm->y_pos >= (yss * 2 + ysz))
				continue;

			// L1 elevation
			int sof = elev[(anm->y_pos) * x_size + anm->x_pos];

			// get sprite
			Sprite* frame = anm->anim->frames[anm->frame_ofs];

			// render sprite
			int mxx = (anm->x_pos - xss) * 80 + (((anm->y_pos - yss * 2) & 1 != 0) ? 0 : 40);
			int myy = (anm->y_pos - yss * 2) * 24 - sof * 18 + MSYOFS + 50;
			frame->Render(pic, pice, mxx, myy, pic_x_size);
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
				if (pos->x < xss || pos->x >= (xss + xsz) || pos->y < yss * 2 || pos->y >= (yss * 2 + ysz))
					continue;

				// L1 elevation
				int y_elev = elev[(pos->y) * x_size + pos->x];

				// render sprite
				int mxx = (pos->x - xss) * 80 + (((pos->y - yss * 2) & 1 != 0) ? 0 : 40);
				int myy = (pos->y - yss * 2) * 24 - y_elev * 18 + MSYOFS + 50;
				spec_sprite[sid]->Render(pic, pice, mxx, myy, pic_x_size);

			}
		}
	}



	// --- Render selection tiles ---
	if (selx != -1 && sely != -1)
	{
		struct {
			int seln;
			struct {
				int x, y;
			}sel[1];
		} msel;
		msel.seln = 1;
		msel.sel[0].x = selx;
		msel.sel[0].y = sely;
		
		for (i = 0; i < msel.seln; i++)
		{
			n = msel.sel[i].x /*- xss*/;
			m = msel.sel[i].y /*- yss * 2*/;

			if (n < 0 || m < 0 || (m + yss * 2) >= y_size || (n + xss) >= x_size - 1)
				continue;

			// get map tile
			Sprite *sid = L1[(m + yss * 2) * x_size + n + xss];
			int sof = elev[(m + yss * 2) * x_size + n + xss];

			// get sprite
			char sn = sid->name[2];

			// override sprite by selector
			sid = &spelldata->special.select[sn - 'A'];

			// draw selection sprite
			int mxx = n * 80 + ((m & 1 != 0) ? 0 : 40);
			int myy = m * 24 - sof * 18 + MSYOFS + 50;
			sid->Render(pic, pice, mxx, myy, pic_x_size);
		}
	}

	
	
	// --- Redner Layer 4 - PNM animations ---
	if (wL4)
	{			
		for (i = 0; i < L4.size(); i++)
		{
			MapLayer4* pnm = L4[i];

			// skip if not in visible area
			if (pnm->x_pos < xss || pnm->x_pos >= (xss + xsz) || pnm->y_pos < yss * 2 || pnm->y_pos >= (yss * 2 + ysz))
				continue;

			// L1 elevation
			int y_elev = elev[(pnm->y_pos) * x_size + pnm->x_pos];
			
			// get sprite
			Sprite* frame = pnm->anim->frames[pnm->frame_ofs];

			// render sprite
			int mxx = (pnm->x_pos - xss)*80 + pnm->x_ofs + (((pnm->y_pos - yss*2) & 1 != 0) ? 0 : 40);
			int myy = (pnm->y_pos - yss * 2) * 24 + pnm->y_ofs - y_elev*18 + MSYOFS + 50;
			frame->Render(pic, pice, mxx, myy, pic_x_size);
		}
	}



	// --- Render Layer 2 - Objects ---
	// --- Redner Units ---
	for (m = 0; m < ysz; m++)
	{
		if (m + yss * 2 >= y_size)
			break;
		for (n = 0; n < xsz; n++)
		{
			if (n + xss >= x_size - 1)
				break;
			int mxy = ConvXY(n + xss, m + yss * 2);

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
					if ((pass == 0 && unit->unit->isAir()) || pass == 2)
						unit->unit->Render(pic, pice, mxx, myy, pic_x_size, terrain->filter.darkpal2, slope, unit->azimuth);
					else
						break;
					// go to next unit at this position
					unit = unit->next;
				}							
					
				// object render:
				if (wL2 && pass == 1 && sid2)
					sid2->Render(pic, pice, mxx, myy, pic_x_size);
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

	// Lock the bitmap's bits.  
	System::Drawing::Rectangle rect = System::Drawing::Rectangle(0, 0, surf_x, surf_y);
	System::Drawing::Imaging::BitmapData^ bmpData = bmp->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, bmp->PixelFormat);

	// first scanline address
	uint8_t*scan = (uint8_t*)((void*)bmpData->Scan0);
	// actual scanline width in target memory
	int stride = bmpData->Stride;

	// for each row:
	for (int y = 0; y < surf_y; y++)
	{
		// source scanline
		uint8_t* src = &pic[(y + (*y_pos / 48 <= MSYOFST ? (*y_pos / 48) * 48 : MSYOFST * 48) + (*y_pos % 48) * smooth)*pic_x_size + ((*x_pos >= 80 ? (80) : (0)) + (*x_pos % 80) * smooth)];
		uint8_t* dest = scan;
		for (int x = 0; x < surf_x; x++)
		{
			*dest++ = pal[*src][2];
			*dest++ = pal[*src][1];
			*dest++ = pal[*src][0];
			src++;
		}
		// goto next destination scanline
		scan += stride;
	}

	// Unlock bmp databits
	bmp->UnlockBits(bmpData);

	return(0);
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
	

	// repaint
	return(1);
}

