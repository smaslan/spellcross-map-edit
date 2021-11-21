//=============================================================================
// Unsorted Spellcross data handling routines.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#pragma once

#include "windows.h"
#include "cstdint"
#include <vector>
#include <list>
#include <string>
#include "fs_archive.h"
#include "fsu_archive.h"
#include "sprites.h"
#include "spell_units.h"

using namespace std;

class SpellDefCmd
{
	public:
		// command name string
		std::string name;		
		vector<std::string> *parameters;
		std::string comment;

		SpellDefCmd(std::string command, std::string params, std::string comment);
		~SpellDefCmd();
};

class SpellDefSection
{
private:
	vector<SpellDefCmd*> list;

public:
	SpellDefSection();
	~SpellDefSection();
	int Size();

	void Add(SpellDefCmd* cmd);
	SpellDefCmd* operator[](int index);
};

class SpellDEF
{
	private: 
		char *data;

	public:
		SpellDEF(wstring &path);
		SpellDEF(uint8_t *data, int size);
		~SpellDEF();

		SpellDefSection *GetSection(std::string section);
};





class SpellData
{
public:
	// terrains data array
	vector<Terrain*> terrain;
	// special tile graphics
	struct {
		Sprite edge[13];
		Sprite select[13];
		Sprite grid[13];
		Sprite solid[13];
	} special;
	// units graphics (FSU)
	FSUarchive *units_fsu;
	// units
	SpellUnits* units;

	SpellData(wstring &data_path);
	~SpellData();
	int LoadSpecialLand(wstring &path);
	Terrain* GetTerrain(char* name);
	Terrain* GetTerrain(int index);
	int GetTerrainCount();

};






