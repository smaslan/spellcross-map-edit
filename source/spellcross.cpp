//=============================================================================
// Unsorted Spellcross data handling routines.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#include "spellcross.h"
#include "sprites.h"
#include "fs_archive.h"
#include "fsu_archive.h"
#include "spell_units.h"
#include "LZ_spell.h"
#include "spell_texts.h"
#include "other.h"
#include "map.h"

#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <regex>
#include <filesystem>

//#include <format>

using namespace std;


//=============================================================================
// class SpellDEF - DEF files parser
//=============================================================================
SpellDefCmd::SpellDefCmd(std::string command, std::string params, std::string comment)
{
	name = command;
	comment = comment;

	std::regex regexz(",");
	parameters = new vector<std::string>(std::sregex_token_iterator(params.begin(), params.end(), regexz, -1), std::sregex_token_iterator());
}
SpellDefCmd::~SpellDefCmd()
{	
	parameters->clear();
	delete parameters;
}

// --- section of DEF file
SpellDefSection::SpellDefSection()
{
}
SpellDefSection::~SpellDefSection()
{
	for(unsigned k = 0; k < list.size(); k++)
		delete list[k];
	list.clear();
}
// get section items count
int SpellDefSection::Size()
{
	return(list.size());
}
// add item
void SpellDefSection::Add(SpellDefCmd *cmd)
{
	list.push_back(cmd);
}
// get item
SpellDefCmd* SpellDefSection::operator[](int index)
{
	if(index < 0 || index > list.size())
		return(NULL);
	return(list[index]);
}



// construct from file
SpellDEF::SpellDEF(wstring &path)
{
	data = NULL;

	// try open file
	ifstream fr(path.c_str(), ios::in | ios::binary | ios::ate);
	if (!fr.is_open())
	{
		throw runtime_error("Cannot open DEF file!");
	}

	// read to local buffer and close
	streampos flen = fr.tellg();
	fr.seekg(0);
	data = new char[(int)flen];
	fr.read(data, flen);
	fr.close();
}
// construct from data buffer
SpellDEF::SpellDEF(uint8_t* data, int size)
{
	this->data = new char[size];
	if (this->data == NULL)
	{
		// allocation failed
		throw std::runtime_error("DEF file memory allocation failed!");
	}
	std::memcpy((void*)this->data, (void*)data, size);
}
// destroy
SpellDEF::~SpellDEF()
{
	if (data)
		delete[] data;
	data = NULL;
}

// returns vector of commands in given section
SpellDefSection *SpellDEF::GetSection(std::string section)
{
	//vector<SpellDefCmd*> cmd_list;
	SpellDefSection *sec_data = new SpellDefSection();
	
	std::string data = std::string(this->data);
	
	// get first occurence of key
	data = data.substr(data.find(section.c_str()));
	
	if(section.find("(") != string::npos)
		section = section.replace(section.find("("), 1, "\\(");
	if (section.find(")") != string::npos)
		section = section.replace(section.find(")"), 1, "\\)");
	
	// look for entire section
	regex secexp(".*" + section + " +\\{([^\\}]+)");
	smatch match;
	std::regex_search(data, match, secexp);

	if (match.size() < 2)
	{
		// failed
		return(sec_data);
	}

	// found
	std::string sectxt = match[1];
		
	while (1)
	{
		regex cmdexp("(.*\\r*\\n*) *([a-zA-Z]+)\\(([^\\)]+)\\)\\r*?\\n*?");
		std::regex_search(sectxt, match, cmdexp);

		if (match.size() != 4)
		{
			return(sec_data);
		}
		
		// parse command data
		SpellDefCmd* cmd = new SpellDefCmd(match[2], match[3], match[1]);		
		// store to list		
		sec_data->Add(cmd);

		// continue
		sectxt = match.suffix();
	}

	return(sec_data);
}














//=============================================================================
// class SpellData
//=============================================================================

SpellData::SpellData(wstring &data_path,wstring& cd_data_path,wstring& spec_path)
{
	font = NULL;
	font7 = NULL;
	units = NULL;
	units_fsu = NULL;
	info = NULL;
	sounds = NULL;
	texts = NULL;
	
	// store path
	spell_data_root = data_path;

	// load terrains
	const wchar_t* terrain_list[] ={L"\\T11.FS",
									 L"\\PUST.FS",
									 L"\\DEVAST.FS"};
	for(unsigned k = 0; k < sizeof(terrain_list)/sizeof(const wchar_t*); k++)
	{
		// terrain archive path
		wstring path = data_path + terrain_list[k];

		// load terrain
		Terrain* new_terrain = new Terrain();
		new_terrain->Load(path);

		// store to list
		terrain.push_back(new_terrain);
	}
		
	// load FSU data
	wstring path = data_path + L"\\UNITS.FSU";
	units_fsu = new FSUarchive(path);
	
	// load COMMON.FS
	FSarchive *common;
	path = data_path + L"\\COMMON.FS";
	common = new FSarchive(path);
	uint8_t* data;
	int size;	
	
	// load UNITS.PAL palette chunk and merge with terrain palette chunk
	if (common->GetFile("UNITS.PAL", &data, &size))
	{
		delete common;
		throw runtime_error("UNITS.PAL not found in COMMON.FS!");
	}
	for (unsigned k = 0; k < this->terrain.size(); k++)
		std::memcpy((void*)&this->terrain[k]->pal[128][0], (void*)data, 96 * 3);
	
	// load SYSTEM.PAL palette chunk and merge with terrain palette chunk
	if (common->GetFile("SYSTEM.PAL", &data, &size))
	{
		delete common;
		throw runtime_error("SYSTEM.PAL not found in COMMON.FS!");
	}
	for (unsigned k = 0; k < this->terrain.size(); k++)
	{
		uint8_t temp[10][3];
		std::memcpy((void*)temp, (void*)&this->terrain[k]->pal[240][0], 10 * 3);
		std::memcpy((void*)&this->terrain[k]->pal[224][0], (void*)data, 32 * 3);
		std::memcpy((void*)&this->terrain[k]->pal[240][0], (void*)temp, 10 * 3);
	}

	// load generic graphic resources
	LoadAuxGraphics(common);

	// processing all other common files
	for(int fid = 0; fid < common->Count(); fid++)
	{
		// get file data
		char* name;
		uint8_t* data;
		int flen;
		common->GetFile(fid,&data,&flen,&name);
		uint8_t* data_end = &data[flen];

		if(wildcmp("*.PNM",name))
		{
			// PNM animations
			AnimPNM *pnm = new AnimPNM();
			char temp_name[13];
			strcpy_noext(temp_name,name);
			pnm->Decode(data,temp_name);
			pnms.push_back(pnm);			
		}
	}

	// hard assign some PNM links
	pnm_sipka = GetPNM("SIPKA");

	// load sound stuff
	sounds = new SpellSounds(data_path);

	// load JEDNOTKY.DEF units definition file
	if (common->GetFile("JEDNOTKY.DEF", &data, &size))
	{
		delete common;
		throw runtime_error("JEDNOTKY.DEF not found in COMMON.FS!");
	}
	units = new SpellUnits(data, size, units_fsu, &gres, sounds);

	// load font file
	if(common->GetFile("FONT_001.FNT",&data,&size))
	{
		// failed
		delete common;
		throw std::exception("Font file FONT_001.FNT not found in FS archive!");
	}
	font = new SpellFont(data, size);

	// close common.fs
	delete common;


	// load TEXTS.FS
	wstring texts_path = (filesystem::path(data_path) / filesystem::path(L"TEXTS.FS")).wstring();
	FSarchive* texts_fs = new FSarchive(texts_path);
	texts = new SpellTexts(texts_fs, SpellLang::CZE, sounds); // ###todo: decode language somehow?
	delete texts_fs;

	
	// load INFO.FS (units art)
	path = cd_data_path + L"\\INFO.FS";
	info = new FSarchive(path);
	

	// load special tiles
	LoadSpecialLand(spec_path);

	// load aux 7pix font
	std::filesystem::path font7_path = spec_path;
	font7_path.append("font_spellcross_7pix.fnt");
	font7 = new SpellFont(font7_path.wstring());

	// load aux 14pix font and merge it to spellcross font
	std::filesystem::path font14_path = spec_path;
	font14_path.append("font_spellcross_14pix.fnt");
	SpellFont font14_aux(font14_path.wstring());
	font->Merge(font14_aux);
		

	// copy some global stuff to terrains
	for(const auto & terr : terrain)
	{
		terr->font = font;
		terr->font7 = font7;
	}
	
}

SpellData::~SpellData()
{
	// destroy terrain data
	for (unsigned k = 0; k < terrain.size(); k++)
		delete terrain[k];
	terrain.clear();	
	// clear units list
	if(units)
		delete units;
	// delete FSU unit data
	if(units_fsu)
		delete units_fsu;
	// delete pnm animations
	for(auto & pnm : pnms)
		delete pnm;
	// delete font
	if(font)
		delete font;
	if(font7)
		delete font7;
	if(info)
		delete info;
	if(sounds)
		delete sounds;
	if(texts)
		delete texts;
}

// find loaded PNM animation
AnimPNM* SpellData::GetPNM(const char* name)
{
	for(auto & pnm : pnms)
		if(strcmp(pnm->name, name) == 0)
			return(pnm);
	return(NULL);
}

// auto build sprite context from all available spellcross maps
int SpellData::BuildSpriteContextOfMaps(wstring folder, string terrain_name,std::function<void(std::string)> status_cb)
{
	// for each file in given folder:
	int count = 0;
	for(const auto& entry : std::filesystem::directory_iterator(folder))
	{		
		if(wildcmp("*.DTA",wstring2string(entry.path().filename()).c_str()))
		{
			// this seems to be a map data file
			wstring map_path = entry.path();
			
			// try to load map
			SpellMap map;				
			if(map.Load(map_path, this))
				continue;

			// check if this is correct terrain type
			if(terrain_name.compare(map.terrain_name))
			{
				map.Close();
				continue;
			}

			string name = wstring2string(entry.path().filename());
			string status = string_format("Processing map #%d: %s",++count, name.c_str());
			if(status_cb)
				status_cb(status);

			// build context from entire map
			map.SelectTiles(SpellMap::SELECT_ADD);
			map.BuildSpriteContext();
			map.Close();			
		}
	}


	string status = string_format("Done (%d files processed)",count);
	if(status_cb)
		status_cb(status);

	return(0);
}


// load generic graphics resources
int SpellData::LoadAuxGraphics(FSarchive *fs)
{
	// init LZW decoder
	LZWexpand *lzw = new LZWexpand(1000000);

	// for each file:
	for(int k = 0; k < fs->Count(); k++)
	{
		// get file data
		char *name;
		uint8_t *data;
		int flen;
		fs->GetFile(k, &data, &flen, &name);
		uint8_t* data_end = &data[flen];

		int is_lzw = false;
		if(wildcmp("I_*.LZ", name))
		{
			// units icons, fized width 60
			is_lzw = true;
			lzw->Decode(data, data_end, &data, &flen);			
			gres.AddRaw(data, flen, 60, flen/60, name, (uint8_t*)terrain[0]->pal);
		}
		else if(strcmp(name, "LISTA_0.LZ") == 0 || strcmp(name,"LISTA_1.LZ") == 0)
		{
			// war map bottom panel
			is_lzw = true;
			lzw->Decode(data,data_end,&data,&flen);
			gres.AddRaw(data,flen,640,flen/640,name,(uint8_t*)terrain[0]->pal);
		}
		else if(strcmp(name,"LISTA_0B.LZ0") == 0)
		{
			// war map right panel overlay
			is_lzw = true;
			lzw->Decode(data,data_end,&data,&flen);
			gres.AddRaw(data,flen,160,flen/160,name,(uint8_t*)terrain[0]->pal);
		}
		else if(strcmp(name,"LISTAPAT.LZ") == 0)
		{
			// war map bottom panel side filling
			is_lzw = true;
			lzw->Decode(data,data_end,&data,&flen);
			gres.AddRaw(data,flen,32,flen/32,name,(uint8_t*)terrain[0]->pal);
		}
		else if(strcmp(name,"LEV_GFK.LZ") == 0)
		{
			// experience mark
			is_lzw = true;
			lzw->Decode(data,data_end,&data,&flen);
			gres.AddRaw(data,flen,9,flen/9,name,(uint8_t*)terrain[0]->pal);
		}
		else if(strcmp(name,"M_ACCOMP.LZ") == 0 || strcmp(name,"M_FAILED.LZ") == 0)
		{
			// war map end title
			is_lzw = true;
			lzw->Decode(data,data_end,&data,&flen);
			gres.AddRaw(data,flen,340,flen/340,name,(uint8_t*)terrain[0]->pal);
		}
		else if(strcmp(name,"OPT_BAR.LZ") == 0)
		{
			// window frame
			is_lzw = true;
			lzw->Decode(data,data_end,&data,&flen);
			gres.AddRaw(data,flen,10,flen/10,name,(uint8_t*)terrain[0]->pal);
		}
		else if(wildcmp("*.ICO",name) || wildcmp("*.BTN",name))
		{
			// ICO files (compression like in PNM files)			
			gres.AddICO(data, flen, name,(uint8_t*)terrain[0]->pal);
		}
		else if(wildcmp("*.CUR",name))
		{
			// CUR files (simple bitmaps with dimensions and transparencies)
			gres.AddCUR(data,flen,name,(uint8_t*)terrain[0]->pal);
		}
		else if(wildcmp("*.GFK",name))
		{
			// GFK projection files: fixed 21x21 pixel with transparencies
			gres.AddRaw(data,flen,21,21,name,(uint8_t*)terrain[0]->pal,true);
		}
		else if(strcmp(name,"I_ATTACK") == 0 || strcmp(name,"I_LOWER") == 0 || strcmp(name,"I_MOVE") == 0 || strcmp(name,"I_UPPER") == 0 || strcmp(name,"I_SELECT") == 0)
		{
			// raw icon files
			gres.AddRaw(data,flen,20,20,name,(uint8_t*)terrain[0]->pal,false,true);
		}
		else if(strcmp(name,"I_TAB") == 0)
		{
			// raw icon files
			gres.AddRaw(data,flen,60,45,name,(uint8_t*)terrain[0]->pal,false,true);
		}
		else if(wildcmp("RAM?HORZ.DTA",name))
		{
			// frame part
			gres.AddRaw(data,flen,76,flen/76,name,(uint8_t*)terrain[0]->pal,true);
		}
		else if(wildcmp("RAM*.DTA",name))
		{
			// frame part
			gres.AddRaw(data,flen,10,flen/10,name,(uint8_t*)terrain[0]->pal,true);
		}
		else if(wildcmp("*.PNM",name))
		{
			// PNM animations:
			gres.AddPNM(data,flen,name);
		}

		if(is_lzw)
			delete[] data;
	}

	// make round LED indicators for mission HUD
	gres.AddLED(204,"RLED_OFF",(uint8_t*)terrain[0]->pal);
	gres.AddLED(253,"RLED_ON",(uint8_t*)terrain[0]->pal);
	gres.AddLED(229,"YLED_ON",(uint8_t*)terrain[0]->pal);
	
	// --- DO NOT ADD ANYTHING TO LIST FROM HERE!!! it would change memory locations!

	// make direct (fast) links to some resoruces
	gres.red_led_off = gres.GetResource("RLED_OFF");
	gres.red_led_on = gres.GetResource("RLED_ON");
	gres.yellow_led_on = gres.GetResource("YLED_ON");
	gres.wm_hud = gres.GetResource("LISTA_0");
	gres.wm_hud_enemy = gres.GetResource("LISTA_1");
	gres.wm_hud_sides = gres.GetResource("LISTAPAT");
	gres.wm_hud_overlay = gres.GetResource("LISTA_0B");
	gres.wm_form[0] = gres.GetResource("WM_FORM0");
	gres.wm_form[1] = gres.GetResource("WM_FORM1");
	gres.wm_form[2] = gres.GetResource("WM_FORM2");
	gres.wm_exp_mark = gres.GetResource("LEV_GFK");
	gres.wm_freeze = gres.GetResource("MRAZIK");
	gres.wm_paralyze = gres.GetResource("PARALIZ");
	gres.wm_btn_idle = gres.GetResource("MAINB__D");
	gres.wm_btn_hover = gres.GetResource("MAINB__A");
	gres.wm_btn_press = gres.GetResource("MAINB__P");
	gres.wm_glyph_air = gres.GetResource("AIRUNIT");
	gres.wm_glyph_center_unit = gres.GetResource("CENTRUNIT");
	gres.wm_glyph_down = gres.GetResource("DOWN");
	gres.wm_glyph_up = gres.GetResource("UP");
	gres.wm_glyph_radar_down = gres.GetResource("RADAROFF");
	gres.wm_glyph_radar_up = gres.GetResource("RADARON");
	gres.wm_glyph_end_turn = gres.GetResource("ENDTURN");
	gres.wm_glyph_goto_unit = gres.GetResource("GOTOUNIT");
	gres.wm_glyph_ground = gres.GetResource("GRNDUNIT");
	gres.wm_glyph_map = gres.GetResource("MAP");
	gres.wm_glyph_heal = gres.GetResource("HEAL");
	gres.wm_glyph_unit_info = gres.GetResource("INFO");
	gres.wm_glyph_next = gres.GetResource("NEXT");
	gres.wm_glyph_options = gres.GetResource("OPTIONS");
	gres.wm_glyph_next_unused = gres.GetResource("PREVIOUS");
	gres.wm_glyph_retreat = gres.GetResource("RETREAT");
	gres.wm_glyph_end_placement = gres.GetResource("UKONCEN");
	gres.wm_glyph_info = gres.GetResource("UNITINFO");
	gres.wm_glyph_place_unit = gres.GetResource("UKONCEN");
	gres.wm_sel_tab = gres.GetResource("I_TAB");
	gres.wm_sel_attack = gres.GetResource("I_ATTACK");
	gres.wm_sel_move = gres.GetResource("I_MOVE");
	gres.wm_sel_upper = gres.GetResource("I_UPPER");
	gres.wm_sel_lower = gres.GetResource("I_LOWER");
	gres.wm_sel_select = gres.GetResource("I_SELECT");
	gres.wm_frame_horz = gres.GetResource("RAM2HORZ.DTA");
	gres.wm_frame_vert = gres.GetResource("RAM2VERT.DTA");
	gres.wm_frame_corner = gres.GetResource("RAM2ROH.DTA");

	// render cursors
	gres.cur_pointer = gres.RenderCUR("SIPKA.CUR");
	gres.cur_wait = gres.RenderCUR("WAIT.CUR");
	gres.cur_select = gres.RenderCUR("SELECT.CUR");
	gres.cur_question = gres.RenderCUR("OTAZNIK.CUR");
	gres.cur_move = gres.RenderCUR("DOJAZD.CUR");
	gres.cur_attack_down = gres.RenderCUR("TARGT_D.CUR");
	gres.cur_attack_up = gres.RenderCUR("TARGT_U.CUR");
	gres.cur_attack_up_down = gres.RenderCUR("OTAZNIK.CUR");
	
	// order projectiles
	gres.SortProjectiles();
	
	// loose LZW decoder
	delete lzw;

	return(0);
}


// Load special land images (solid A-M, edge A-M, selection A-M)
int SpellData::LoadSpecialLand(wstring &spec_folder)
{

	// groups to load
	const wchar_t* files[] = { L"\\spec\\solid.fs", L"\\spec\\edge.fs", L"\\spec\\grid.fs", L"\\spec\\select.fs" };
	Sprite* lists[] = { special.solid, special.edge, special.grid, special.select };
	
	// for each group:
	for (int g = 0; g < sizeof(files)/sizeof(wchar_t*); g++)
	{
		// try to load special archive
		wstring path = spec_folder + files[g];
		FSarchive *fs = new FSarchive(path);

		// for each slope:
		char name[6];
		strcpy_s(name, sizeof(name), "_.BMP");
		for (char sid = 'A'; sid <= 'M'; sid++)
		{
			name[0] = sid;
			uint8_t* pic;
			int size;
			if (fs->GetFile(name, &pic, &size))
			{
				// not found?
				delete[] fs;
				return(1);
			}

			// target sprite object
			Sprite* spr = &lists[g][sid - 'A'];

			// store some name (because of sprite type letter)
			std::memset(spr->name, '\0', sizeof(spr->name));
			spr->name[0] = '_';
			spr->name[1] = '_';
			spr->name[2] = sid;


			// --- very crude BMP decoder:
			// get width
			int x = *((DWORD*)&pic[0x12]);

			// get heigth
			int y = *((DWORD*)&pic[0x16]);

			// get graphics data start
			int fofs = *((DWORD*)&pic[0x0A]);

			// store x, y
			spr->x_size = x;
			spr->y_size = y;
			
			// vertical offset (derived from original sprites)
			if (sid == 'B' || sid == 'D' || sid == 'F' || sid == 'L' || sid == 'M')
				spr->y_ofs = -18;
			else if (sid == 'C')
				spr->y_ofs = -17;
			else
				spr->y_ofs = 0;

			// no transparencies
			spr->has_transp = 0;

			// allocate image buffer
			spr->data = new uint8_t[(80 + sizeof(int) * 2) * 200];

			// recode bmp->Sprite
			int pp1 = 0;
			for (int j = 0; j < y; j++)
			{
				// get line
				int pp2 = 0;
				uint8_t ss = 0;
				int oo = 0;
				uint8_t line[80];
				for (int k = 0; k < x; k++)
				{
					if (pic[fofs + (y - 1) * x - j * x + k] == 0x07u && ss == 0)
						oo++;
					else
					{
						ss = 1;
						line[pp2++] = pic[fofs + (y - 1) * x - j * x + k];
					}
				}

				// check end transparency
				int m;
				for (m = pp2 - 1; m >= 0; m--)
				{
					if (line[m] != 0x07)
						break;
				}				

				// store offset
				std::memset((void*)&spr->data[pp1], 0x00, sizeof(int) * 2);
				spr->data[pp1] = oo;
				pp1 += sizeof(int);

				// store line len
				spr->data[pp1] = m + 1;
				pp1 += sizeof(int);

				// check for transparencies
				if (memchr(line, 0x07, m + 1) != NULL && !spr->has_transp)
					spr->has_transp = 1;

				// convert colors
				for (int n = 0; n < m + 1; n++)
				{
					ss = 0x00;
					switch (line[n])
					{
						case 0xFFu: ss = 0xFEu; break;
						case 0x00u: ss = 0xFFu; break;
						case 0xF9u: ss = 0xC1u; break;
					}
					line[n] = ss;
				}

				// store line data
				std::memcpy((void*)&spr->data[pp1], (void*)line, m + 1);
				pp1 += m + 1;				
			}

			// ###todo: convert this whole stuff to load alrady converted sprite data! 
			// ###todo: add destructor for these sprites somewhere?
		}

		// loose archive
		delete fs;
	}


	return(0);
}

// get number of loaded terraings
int SpellData::GetTerrainCount()
{
	return(terrain.size());
}
// get terrain pointer by terrain name or return NULL
Terrain* SpellData::GetTerrain(const char* name)
{
	for (unsigned k = 0; k < this->terrain.size(); k++)
		if (_strcmpi(this->terrain[k]->name, name) == 0)
			return(this->terrain[k]);
	return(NULL);
}
// get terrain pointer by index or NULL
Terrain* SpellData::GetTerrain(int index)
{
	if(index >= terrain.size())
		return(NULL);
	return(terrain[index]);
}
