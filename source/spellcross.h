//=============================================================================
// Unsorted Spellcross data handling routines.
// Top level loaders of Spellcross data.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021 - 2026, Stanislav Maslan, s.maslan@seznam.cz
// url: https://github.com/smaslan/spellcross-map-edit
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#pragma once

//#include "windows.h"
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
#include "spell_midi.h"
#include "spell_texts.h"
#include "spell_video.h"

//using namespace std;


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
	int GenerateSpecialTiles();
	int LoadPalettes(FSarchive* fs_common,FSarchive* fs_info);
	int LoadAuxGraphics(FSarchive* fs,std::function<void(std::string)> status_item=NULL);
	int LoadInfoGraphics(FSarchive* fs,std::function<void(std::string)> status_item=NULL);
	int LoadResearch(FSarchive* fs,std::function<void(std::string)> status_item=NULL);
	
	std::string last_error;
	FSarchive* common_fs;
	FSarchive* terrain_fs;
	FSarchive* info_fs;

public:
	// data paths
	std::wstring data_path;
	std::wstring cd_data_path;
	std::wstring export_path;

	
	// terrains data array
	vector<Terrain*> terrain;
	// special tile graphics
	struct {
		Sprite select[13];
		Sprite grid[13];
		Sprite solid[13];
	} special;
	// units graphics (FSU)
	FSUarchive *units_fsu;
	// units
	SpellUnits* units;
	// palettes
	uint8_t (*map_pal)[3]; /* map environment common pal (index: 128 - 255) */
	// fonts
	SpellFont* font;
	SpellFont* font7;
	// general graphics
	SpellGraphics gres;
	// info unit graphics
	SpellGraphics gres_info;
	// last path
	wstring spell_data_root;		
	// sound stuff
	SpellSounds* sounds;
	// sound stuff
	SpellMIDI* midi;
	// texts.fs
	SpellTexts* texts;
	SpellTexts* research_texts;
	// L2 object class parameters
	SpellL2classes *L2_classes;
	// unit bonuses (BONUSES.DEF)
	UnitBonuses *unit_bonuses;
	// videos
	SpellVideoResources *videos;


	// palettes
	std::vector<SpellPalette*> pal_list;
	SpellPalette *AddPalette(std::string name);
	SpellPalette *GetPalette(std::string name);
	uint8_t* GetPaletteData(std::string name);	
	
	static int FindArchive(std::vector<std::filesystem::path>& paths,std::string name,std::filesystem::path& arch_path,std::filesystem::path* dir_path=NULL,bool optional=false,std::string* error_msg=NULL);
	
	SpellData();
	SpellData(std::filesystem::path& data_path,std::filesystem::path& cd_data_path,std::filesystem::path& mod_path,std::filesystem::path& spec_path,std::function<void(std::string)> status_list=NULL,std::function<void(std::string)> status_item=NULL);
	int Reload(std::filesystem::path& data_path,std::filesystem::path& cd_data_path,std::filesystem::path& mod_path,std::filesystem::path& spec_path,std::function<void(std::string)> status_list=NULL,std::function<void(std::string)> status_item=NULL);
	void Cleanup();
	~SpellData();
	std::string GetLastError();
	Terrain* GetTerrain(const char* name);
	Terrain* GetTerrain(int index);
	int GetTerrainCount();
	int BuildSpriteContextOfMaps(wstring folder,string terrain_name,std::function<void(std::string)> status_cb);	
	int BuildHouseObjectsOfMaps(wstring folder,string terrain_name,std::function<void(std::string)> status_cb);
};








