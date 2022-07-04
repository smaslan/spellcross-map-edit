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
#include "spell_font.h"
#include "spell_graphics.h"
#include "spell_sound.h"
#include "spell_texts.h"

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
		
	void Add(SpellDefCmd* cmd);
	int Size();
	SpellDefCmd* operator[](int index);
	vector<SpellDefCmd*> &GetData();
};

class SpellDEF
{
	private: 
		string data;

	public:
		SpellDEF(string str);
		SpellDEF(wstring &path);
		SpellDEF(uint8_t *data, int size);
		~SpellDEF();

		SpellDefSection *GetSection(std::string section);
};

class SpellClassFileRec
{
private:
public:
	int index;
	vector<string> head;
	vector<string> items;
};

class SpellClassFile
{
private:
public:
	vector<SpellClassFileRec> list;
	SpellClassFile(string text,string regexp_head,int reg_index_item=-1);
};


class SpellStringTableRec
{
private:
public:
	string label;
	string raw;
	wstring text;
	SpellStringTableRec(string label, string raw, wstring text);
};
class SpellStringTable
{
private:
	string raw_text;
	string lang;
public:
	SpellStringTable(FSarchive *fs, string name);
	string &GetRaw();
	
	vector<SpellStringTableRec> list;
};




class SpellData
{
private:
	int LoadSpecialLand(wstring& path);
	int LoadAuxGraphics(FSarchive* fs,std::function<void(std::string)> status_item=NULL);
	
	FSarchive* common_fs;
	FSarchive* terrain_fs;

public:
	// data paths
	std::wstring data_path;
	std::wstring cd_data_path;

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
	// palettes
	uint8_t map_pal[256][3]; /* map environment common pal (index: 128 - 255) */
	// fonts
	SpellFont* font;
	SpellFont* font7;
	// generic graphics
	SpellGraphics gres;
	// last path
	wstring spell_data_root;
	// info.fs
	FSarchive* info;
	// sound stuff
	SpellSounds* sounds;
	// texts.fs
	SpellTexts* texts;
	// L2 object class parameters
	SpellL2classes *L2_classes;
	// unit bonuses (BONUSES.DEF)
	UnitBonuses *unit_bonuses;

	SpellData(wstring& data_path,wstring& cd_data_path,wstring& spec_path,std::function<void(std::string)> status_list=NULL,std::function<void(std::string)> status_item=NULL);
	~SpellData();	
	Terrain* GetTerrain(const char* name);
	Terrain* GetTerrain(int index);
	int GetTerrainCount();
	int BuildSpriteContextOfMaps(wstring folder,string terrain_name,std::function<void(std::string)> status_cb);

	AnimPNM *pnm_sipka;
};






