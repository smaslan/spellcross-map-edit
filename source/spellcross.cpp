//=============================================================================
// Unsorted Spellcross data handling routines.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#include "spellcross.h"
#include "fs_archive.h"
#include "fsu_archive.h"
#include "spell_units.h"

#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <regex>

#include <format>

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
	delete parameters;
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
vector<SpellDefCmd*> SpellDEF::GetSection(std::string section)
{
	vector<SpellDefCmd*> cmd_list;
	
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
		return(cmd_list);
	}

	// found
	std::string sectxt = match[1];
		
	while (1)
	{
		regex cmdexp("(.*\\r*\\n*) *([a-zA-Z]+)\\(([^\\)]+)\\)\\r*?\\n*?");
		std::regex_search(sectxt, match, cmdexp);

		if (match.size() != 4)
		{
			return(cmd_list);
		}
		
		// parse command data
		SpellDefCmd* cmd = new SpellDefCmd(match[2], match[3], match[1]);		
		// store to list
		cmd_list.push_back(cmd);

		// continue
		sectxt = match.suffix();
	}

	return(cmd_list);

}














//=============================================================================
// class SpellData
//=============================================================================

SpellData::SpellData(wstring &data_path)
{
	// load terrains
	vector<wchar_t*> terrain_list({L"\\T11.FS",
		                           L"\\PUST.FS",
		                           L"\\DEVAST.FS"});
	for (unsigned k = 0; k < terrain_list.size(); k++)
	{		
		// terrain archive path
		wstring path = data_path + terrain_list[k];
						
		// load terrain
		Terrain* new_terrain = new Terrain;
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

	// load JEDNOTKY.DEF units definition file
	if (common->GetFile("JEDNOTKY.DEF", &data, &size))
	{
		delete common;
		throw runtime_error("JEDNOTKY.DEF not found in COMMON.FS!");
	}
	units = new SpellUnits(data, size, units_fsu);

	// close common.fs
	delete common;
	
}

SpellData::~SpellData()
{
	// destroy terrain data
	for (unsigned k = 0; k < terrain.size(); k++)
		delete terrain[k];
	terrain.clear();
	// clear units list
	delete units;
}


// Load special land images (solid A-M, edge A-M, selection A-M)
int SpellData::LoadSpecialLand(wstring &spec_folder)
{

	// groups to load
	wchar_t* files[] = { L"\\solid.fs", L"\\edge.fs", L"\\grid.fs", L"\\select.fs" };
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

// get terrain pointer by terrain name or return NULL
Terrain* SpellData::GetTerrain(char* name)
{
	for (unsigned k = 0; k < this->terrain.size(); k++)
		if (_strcmpi(this->terrain[k]->name, name) == 0)
			return(this->terrain[k]);
	return(NULL);
}
