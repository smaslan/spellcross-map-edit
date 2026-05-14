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
#include "spell_midi.h"
#include "spell_texts.h"
#include "spell_video.h"

//using namespace std;

class SpellDefCmd
{
	public:
		// command name string
		std::string name;		
		vector<std::string> *parameters;
		std::string comment;
		std::string full_command;

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


// palette record
class SpellPalette
{
public:
	std::string m_name;
	std::vector<uint8_t> m_pal;
	std::vector<uint8_t> m_used;

	class Chunk{
	public:
		std::string name;
		int offset;
		int size;
	};
	std::vector<Chunk> m_chunks;

	SpellPalette();
	SpellPalette(std::string name);
	void Clear();
	int Insert(std::vector<uint8_t> &data,std::string name="",int offset=0);
	int Insert(std::wstring path,int offset=0,std::string used="");
	std::tuple<int,int> GetRange(int start=0);
	std::string GetRangeString();
	int Save(std::wstring path);
	int SaveChunks(std::wstring directory_path);
	int LoadInfo(std::filesystem::path path);
	int SaveInfo(std::filesystem::path path);
	int Render(wxBitmap& bmp);
	int RenderPaletteColor(wxBitmap& bmp,int x_size,int x_pos,uint8_t* filter=NULL);
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








