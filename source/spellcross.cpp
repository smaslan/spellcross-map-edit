//=============================================================================
// Unsorted Spellcross data handling routines.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021-2022, Stanislav Maslan, s.maslan@seznam.cz
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


//using namespace std;


//=============================================================================
// class SpellDEF - DEF files parser
//=============================================================================
SpellDefCmd::SpellDefCmd(std::string command, std::string params, std::string comment)
{
	name = command;
	this->comment = comment;
	this->full_command = command + "(" + params + ")";
	// split parameters
	std::regex regexz(",");
	parameters = new vector<std::string>(std::sregex_token_iterator(params.begin(), params.end(), regexz, -1), std::sregex_token_iterator());
	// cleanup params if empty
	if(params.empty())
		parameters->clear();
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
// get all sections
vector<SpellDefCmd*>& SpellDefSection::GetData()
{
	return(list);
}



// construct from file
SpellDEF::SpellDEF(wstring &path)
{
	// try open file
	ifstream fr(path.c_str(), ios::in | ios::binary | ios::ate);
	if (!fr.is_open())
		throw runtime_error("Cannot open DEF file!");

	// read to local buffer and close
	streampos flen = fr.tellg();
	fr.seekg(0);
	data.resize(flen);
	fr.read(data.data(), flen);
	fr.close();
}
// construct from data buffer
SpellDEF::SpellDEF(uint8_t* data, int size)
{
	this->data.resize(size);
	std::memcpy((void*)this->data.data(), (void*)data, size);
}
// construct from string
SpellDEF::SpellDEF(string str)
{
	data = str;
}
// destroy
SpellDEF::~SpellDEF()
{
	data.clear();
}

// returns vector of commands in given section
SpellDefSection *SpellDEF::GetSection(std::string section)
{
	//vector<SpellDefCmd*> cmd_list;
	SpellDefSection *sec_data = new SpellDefSection();
	
	std::string data = std::string(this->data);
	
	// get first occurence of key
	auto sec_pos = data.find(section.c_str());
	if(sec_pos == std::string::npos)
	{
		// not found - return empty
		delete sec_data;
		return(NULL);
	}
	data = data.substr(sec_pos);
	
	if(section.find("(") != std::string::npos)
		section = section.replace(section.find("("), 1, "\\(");
	if (section.find(")") != std::string::npos)
		section = section.replace(section.find(")"), 1, "\\)");
	
	// look for entire section
	regex secexp(".*" + section + " +\\{([^\\}]+)");
	smatch match;
	std::regex_search(data, match, secexp);
	if(match.size() < 2)
	{
		// failed
		delete sec_data;
		return(NULL);
	}

	// found
	std::string sectxt = match[1];
		
	while (1)
	{
		std::regex cmdexp("(.*\\r*\\n*) *([a-zA-Z]+)\\(([^\\)]*)\\)\\r*?\\n*?");
		std::regex_search(sectxt, match, cmdexp);
		if(match.size() != 4)
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
// spellcross class lists parser
//=============================================================================
// parse spellcross class DEF files
//  text - DEF file content
//  regexp_head - regexp header to parse out single record of the DEF
//  reg_index_item - <0 to put records as they goes, >=0 to pick regexpt captured item to be used as result position number
SpellClassFile::SpellClassFile(string text,string regexp_head,int reg_index_item)
{
	// skip comments
	size_t start_pos = 0;
	auto ss = stringstream{text};
	for(std::string line; getline(ss,line,'\n');)
	{
		if(line.empty() || line[0] != ';')
		{
			start_pos = ss.tellg();
			break;
		}
	}
	
	int max_id = 0;	
	if(start_pos != string::npos)
	{
		text = text.substr(start_pos);
		regex secexp(regexp_head);
		do {
			smatch match;
			std::regex_search(text,match,secexp);
			if(match.size() < 2)
				break;
			int head_count = match.size();

			SpellClassFileRec cls;
			// strore header items
			for(int k = 1; k < head_count-1; k++)
				cls.head.push_back(match[k].str());
			
			// parse data
			auto ss = stringstream{match[head_count-1]};
			int first = true;
			for(std::string line; getline(ss,line,'\n');)
			{
				if(!line.empty() && line[0] == ';')
					continue; // still comment
				auto cr = line.find('\r');
				if(cr != string::npos)
					line.resize(cr);
				if(line.empty())
					break;
				cls.items.push_back(line);
			}
			// place class to proper ID position
			if(reg_index_item >= 0)
			{
				cls.index = std::stoi(cls.head[reg_index_item]);
				max_id = max(max_id,cls.index);
				list.resize(max_id+1);
				list[cls.index] = cls;
			}
			else
			{
				cls.index = -1;
				list.push_back(cls);
			}

			text = match.suffix();
		} while(true);
	}	
}



//=============================================================================
// multilingual 
//=============================================================================
SpellStringTableRec::SpellStringTableRec(string label,string raw,wstring text)
{
	this->label = label;
	this->raw = raw;
	this->text = text;
}

SpellStringTable::SpellStringTable(FSarchive* fs,string name)
{
	vector<string> lang_list = {"CZ","ENG","PL"};
	for(auto& lang_tag : lang_list)
	{
		lang = lang_tag;
		raw_text = fs->GetFile((name + "." + lang_tag).c_str());
		if(!raw_text.empty())
			break;
	}
	if(raw_text.empty())
		throw std::runtime_error("Cannot find stringtable file!");

	// try parse
	SpellClassFile strtable(raw_text, ";\\s*([^\\r^\\n]+)\\r?\\n?([^;]+)");
	
	list.reserve(strtable.list.size());
	for(auto & rec : strtable.list)
	{
		if(rec.head.empty() || rec.items.empty())
			throw std::runtime_error("Invalid string table data!");

		// decode
		string &raw = rec.items[0];
		wstring text = L"";
		if(lang.compare("CZ") == 0)
			text = char2wstringCP895(raw.c_str());
		else if(lang.compare("ENG") == 0)
			text = char2wstring(raw.c_str());
		else if(lang.compare("PL") == 0)
			text = char2wstring(raw.c_str()); // ###todo: shoud be CP1250

		// store to list
		list.emplace_back(rec.head[0],raw,text);		
	}	
}

string &SpellStringTable::GetRaw()
{
	return(raw_text);
}







//=============================================================================
// class SpellData
//=============================================================================

SpellData::SpellData(wstring &data_path,wstring& cd_data_path,wstring& spec_path,std::function<void(std::string)> status_list,std::function<void(std::string)> status_item)
{
	font = NULL;
	font7 = NULL;
	units = NULL;
	units_fsu = NULL;
	info = NULL;
	sounds = NULL;
	midi = NULL;
	texts = NULL;
	L2_classes = NULL;
	unit_bonuses = NULL;		
	common_fs = NULL;
	terrain_fs = NULL;
	videos = NULL;

	// store data paths for dynamic loading
	this->data_path = data_path;
	this->cd_data_path = cd_data_path;

	uint8_t* data;
	int size;
	
	// store path
	spell_data_root = data_path;

	// load COMMON.FS
	if(status_list)
		status_list("Loading COMMON.FS archive...");	
	wstring common_path = std::filesystem::path(data_path) / std::filesystem::path("COMMON.FS");
	try{
		common_fs = new FSarchive(common_path,FSarchive::Options::DELZ_ALL);
	}catch(const runtime_error& error){
		this->~SpellData();
		if(status_list)
			status_list(" - failed!");
		throw runtime_error(string_format("Loading COMMON.FS archive failed (%s)!",error.what()));
	}
		
	// load sound stuff
	if(status_list)
		status_list("Loading sound samples...");
	try{
		sounds = new SpellSounds(common_fs,data_path,16,status_list,status_item);
	}catch(const runtime_error& error) {
		this->~SpellData();
		if(status_list)
			status_list(" - failed!");
		throw runtime_error(string_format("Loading sound samples filed (%s)!",error.what()));
	}

	// load MIDI stuff
	if(status_list)
		status_list("Loading MIDI files...");
	try {
		midi = new SpellMIDI(data_path,status_list,status_item);
	}
	catch(const runtime_error& error) {
		this->~SpellData();
		if(status_list)
			status_list(" - failed!");
		throw runtime_error(string_format("Loading MIDI files (%s)!",error.what()));
	}
	
	// load L2 object classes stuff
	if(status_list)
		status_list("Loading destructible object classes...");
	try{
		L2_classes = new SpellL2classes(common_fs,sounds,status_list,status_item);
	}catch(const runtime_error& error) {
		this->~SpellData();
		if(status_list)
			status_list(" - failed!");
		throw runtime_error(string_format("Loading destructible object classes failed (%s)!",error.what()));
	}

	// load unit bonuses BONUSES.DEF
	if(status_list)
		status_list("Loading unit experience bonuses...");
	try{
		string bonus_def = common_fs->GetFile("BONUSES.DEF");
		unit_bonuses = new UnitBonuses(bonus_def);
	}catch(const runtime_error& error) {
		this->~SpellData();
		if(status_list)
			status_list(" - failed!");
		throw runtime_error(string_format("Loading unit bonuses failed (%s)!",error.what()));
	}

	// load UNITS.PAL palette chunk for maps
	if(status_list)
		status_list("Loading palette files...");
	if(LoadPalettes(common_fs))
	{
		this->~SpellData();
		if(status_list)
			status_list(" - missing palette file(s)!");
		throw runtime_error("missing palette file(s) in COMMON.FS!");
	}
		
	
	// load generic graphic resources
	if(status_list)
		status_list("Loading common graphics resources...");
	if(LoadAuxGraphics(common_fs,status_item))
	{
		this->~SpellData();
		if(status_list)
			status_list(" - failed!");
		throw runtime_error("Loading common graphics resources failed!");
	}
	
	// load terrains
	if(status_list)
		status_list("Loading terrain graphics data...");
	vector<string> terrain_list = {"T11.FS", "PUST.FS", "DEVAST.FS"};
	for(auto & name : terrain_list)
	{
		// load terrain
		if(status_list)
			status_list(string_format(" - loading terrain \"%s\"",name.c_str()));

		// load FS
		wstring path = std::filesystem::path(data_path) / std::filesystem::path(name);
		try{
			terrain_fs = new FSarchive(path);
		}catch(const runtime_error& error) {
			this->~SpellData();
			if(status_list)
				status_list("   - failed!");
			throw runtime_error(string_format("Loading \"%ls\" archive failed (%s)!",path,error.what()));
		}

		// try load aux FS data
		wstring aux_path = std::filesystem::path(spec_path) / std::filesystem::path(name);
		try{			
			terrain_fs->Append(aux_path);
		}catch(...){
			// do nothing, optional data
		};
		
		// make new terrain
		Terrain* new_terrain = new Terrain();
		if(new_terrain->Load(terrain_fs, map_pal, &gres, L2_classes, status_item))
		{
			this->~SpellData();
			if(status_list)
				status_list(" - failed!");
			throw runtime_error(string_format("Loading terrain \"%s\" failed!",name.c_str()));
		}

		// loose FS data
		delete terrain_fs;
		terrain_fs = NULL;

		// store to list
		terrain.push_back(new_terrain);
	}

		
	// load FSU data
	if(status_list)
		status_list("Loading units graphics data (UNITS.FSU)...");
	wstring fsu_path = std::filesystem::path(data_path) / std::filesystem::path("UNITS.FSU");
	try{
		units_fsu = new FSUarchive(fsu_path, status_item);
	}catch(const runtime_error& error) {
		this->~SpellData();
		if(status_list)
			status_list(" - failed!");
		throw runtime_error(string_format("Loading UNITS.FSU units graphics failed (%s)!",error.what()));
	}	


	// load JEDNOTKY.DEF units definition file
	if(status_list)
		status_list("Loading units definitions (JEDNOTKY.DEF)...");
	if(common_fs->GetFile("JEDNOTKY.DEF", &data, &size))
	{		
		this->~SpellData();
		if(status_list)
			status_list(" - loading failed!");
		throw runtime_error("JEDNOTKY.DEF not found in COMMON.FS!");
	}
	try{
		units = new SpellUnits(data, size, units_fsu, &gres, sounds, unit_bonuses);
	}catch(const runtime_error& error) {
		this->~SpellData();
		if(status_list)
			status_list(" - decoding failed!");
		throw runtime_error(string_format("Decoding JEDNOTKY.DEF failed (%s)!",error.what()));
	}

	// load font file
	if(status_list)
		status_list("Loading font files...");
	if(common_fs->GetFile("FONT_001.FNT",&data,&size))
	{
		// failed
		this->~SpellData();
		if(status_list)
			status_list(" - loading main spellcross font file failed!");
		throw std::exception("Font file FONT_001.FNT not found in FS archive!");
	}
	try{
		font = new SpellFont(data, size);
	}catch(const runtime_error& error) {
		this->~SpellData();
		if(status_list)
			status_list(" - failed!");
		throw runtime_error(string_format("Decoding font file failed (%s)!",error.what()));
	}

	// close common.fs
	delete common_fs;
	common_fs = NULL;


	// load TEXTS.FS
	if(status_list)
		status_list("Loading string tables...");
	wstring texts_path = std::filesystem::path(data_path) / std::filesystem::path("TEXTS.FS");
	FSarchive* texts_fs = new FSarchive(texts_path);
	try{
		texts = new SpellTexts(texts_fs, SpellLang::CZE, sounds); // ###todo: decode language somehow?
	}catch(const runtime_error& error) {
		delete texts_fs;
		this->~SpellData();
		if(status_list)
			status_list(" - failed!");
		throw runtime_error(string_format("Decoding string tables failed (%s)!",error.what()));
	}
	delete texts_fs;

	
	// load INFO.FS (units art)
	if(status_list)
		status_list("Loading units info/renders (INFO.FS)...");
	wstring info_path = std::filesystem::path(cd_data_path) / std::filesystem::path("INFO.FS");
	try{
		info = new FSarchive(info_path);
	}catch(const runtime_error& error) {
		this->~SpellData();
		if(status_list)
			status_list(" - failed!");
		throw runtime_error(string_format("Decoding INFO.FS failed (%s)!",error.what()));
	}
	

	// load special tiles
	if(status_list)
		status_list("Loading special tiles (selection, etc.)...");
	if(GenerateSpecialTiles())
	{
		this->~SpellData();
		if(status_list)
			status_list(" - failed!");
		throw runtime_error("Loading special selection tiles failed!");
	}

	// load aux 7pix font
	if(status_list)
		status_list("Loading aux 7-pix font...");
	wstring font7_path = std::filesystem::path(spec_path) / std::filesystem::path("font_spellcross_7pix.fnt");
	try{
		font7 = new SpellFont(font7_path);
	}catch(const runtime_error& error) {
		this->~SpellData();
		if(status_list)
			status_list(" - failed!");
		throw runtime_error(string_format("Failed loading aux font ''%ls'' (%s)!",font7_path.c_str(),error.what()));
	}

	// load aux 14pix font and merge it to spellcross font
	if(status_list)
		status_list("Loading aux 14-pix font...");
	wstring font14_path = std::filesystem::path(spec_path) / std::filesystem::path("font_spellcross_14pix.fnt");
	try{
		SpellFont font14_aux(font14_path);
		font->Merge(font14_aux);
	}catch(const runtime_error& error) {
		this->~SpellData();
		if(status_list)
			status_list(" - failed!");
		throw runtime_error(string_format("Failed loading aux font ''%ls'' (%s)!",font14_path.c_str(),error.what()));
	}
	
	// copy some global stuff to terrains
	for(const auto & terr : terrain)
	{
		terr->font = font;
		terr->font7 = font7;
	}

	// load video resources
	if(status_list)
		status_list("Loading video resources (MOVIE.FS, SPEAKER.FS)...");		
	try {
		auto vid_paths ={cd_data_path,data_path};
		videos = new SpellVideoResources(vid_paths);
	}
	catch(const runtime_error& error) {
		this->~SpellData();
		if(status_list)
			status_list(" - failed!");
		throw runtime_error(string_format("Loading video resources from MOVIE.FS and SPEAKER.FS failed (%s)!",error.what()));
	}
	
}

// cleanup spellcross data
SpellData::~SpellData()
{
	// destroy terrain data
	for(auto & terr : terrain)
		delete terr;
	terrain.clear();	
	// clear units list
	if(units)
		delete units;
	units = NULL;
	// delete FSU unit data
	if(units_fsu)
		delete units_fsu;
	units_fsu = NULL;
	// delete font
	if(font)
		delete font;
	font = NULL;
	if(font7)
		delete font7;
	font7 = NULL;
	if(info)
		delete info;
	info = NULL;
	if(sounds)
		delete sounds;
	sounds = NULL;
	if(midi)
		delete midi;
	midi = NULL;
	if(texts)
		delete texts;
	texts = NULL;
	if(L2_classes)
		delete L2_classes;
	L2_classes = NULL;
	if(unit_bonuses)
		delete unit_bonuses;
	unit_bonuses = NULL;
	if(common_fs)
		delete common_fs;
	common_fs = NULL;
	if(terrain_fs)
		delete terrain_fs;
	terrain_fs = NULL;
	if(videos)
		delete videos;
	videos = NULL;
	for(auto &pal: pal_list)
		delete pal;
	pal_list.clear();
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

// auto build house objects (DMAx_xxx tiles) from all available spellcross maps
int SpellData::BuildHouseObjectsOfMaps(wstring folder,string terrain_name,std::function<void(std::string)> status_cb)
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
			if(map.Load(map_path,this))
				continue;

			// check if this is correct terrain type
			if(terrain_name.compare(map.terrain_name))
			{
				map.Close();
				continue;
			}

			string name = wstring2string(entry.path().filename());
			string status = string_format("Processing map #%d: %s",++count,name.c_str());
			if(status_cb)
				status_cb(status);

			// build context from entire map			
			map.BuildHouseObjects();
			map.Close();			
		}
	}


	string status = string_format("Done (%d files processed)",count);
	if(status_cb)
		status_cb(status);

	return(0);
}



// load palette resources from COMMON.FS
int SpellData::LoadPalettes(FSarchive* fs)
{
	std::vector<uint8_t> chunk;
	SpellPalette *pal;

	// make empty map palette (bottom 128 colors are from terrains)
	pal = AddPalette("MAP");
	
	// UNITS.PAL chunk	
	if(fs->GetFile("UNITS.PAL",chunk) || chunk.size() != 96*3)
		return(1);
	pal->Insert(chunk,"UNITS.PAL",128);
		
	// load SYSTEM.PAL palette chunk for maps
	if(fs->GetFile("SYSTEM.PAL",chunk) || chunk.size() != 32*3)
		return(1);
	pal->Insert(chunk,"SYSTEM.PAL",224);

	// load CURSOR.PAL palette chunk for maps - ###todo: not sure where to place this
	/*if(common_fs->GetFile("CURSOR.PAL",&data,&size) || size != 6*3)
	{
		this->~SpellData();
		if(status_list)
			status_list(" - missing CURSOR.PAL!");
		throw runtime_error("CURSOR.PAL not found in COMMON.FS!");
	}
	std::memcpy(&map_pal[218][0],data,size);*/

	// make shortcut to map palette
	map_pal = (uint8_t(*)[3])pal->m_pal.data();


	
	// make empty big map palette
	pal = AddPalette("BIG_MAP.PAL");

	// BIG_MAP.PAL chunk	
	if(fs->GetFile("BIG_MAP.PAL",chunk) || chunk.size() != 64*3)
		return(1);
	pal->Insert(chunk,"BIG_MAP.PAL",192);
	// _SHARED1.PAL chunk
	if(fs->GetFile("_SHARED1.PAL",chunk) || chunk.size() != 128*3)
		return(1);
	pal->Insert(chunk,"_SHARED1.PAL",0);


	// make empty main menu palette
	pal = AddPalette("MAINMENU.PAL");

	// MAINMENU.PAL chunk	
	if(fs->GetFile("MAINMENU.PAL",chunk) || chunk.size() != 256*3)
		return(1);
	pal->Insert(chunk,"MAINMENU.PAL");

	// make empty main menu palette
	pal = AddPalette("STRATEGY.PAL");

	// MAINMENU.PAL chunk	
	if(fs->GetFile("STRATEGY.PAL",chunk) || chunk.size() != 256*3)
		return(1);
	pal->Insert(chunk,"STRATEGY.PAL");
					

	return(0);
}

// load generic graphics resources
int SpellData::LoadAuxGraphics(FSarchive *fs,std::function<void(std::string)> status_item)
{
	// default map palette
	auto *map_pal = GetPalette("MAP");

	// for each file:
	for(auto & file : fs->GetFiles())
	{
		// get file data
		const char *name = file->name.c_str();
		int flen = file->data.size();
		uint8_t *data = file->data.data();
		uint8_t* data_end = &data[flen];

		// try load specific palette of matching name if not there yet
		auto pal_name = std::filesystem::path(name).stem().string() + ".PAL";		
		std::vector<uint8_t> pal_data;
		if(!fs->GetFile(pal_name.c_str(),pal_data))
		{
			if(pal_data.size() == 3*256 && !GetPalette(pal_name))
			{
				auto pal = AddPalette(pal_name);
				pal->Insert(pal_data);
			}
		}


		if(wildcmp("I_*.LZ", name))
		{
			// units icons, fized width 60
			gres.AddRaw(data, flen, 60,flen/60, name, map_pal);
		}
		else if(wildcmp("LEVEL_??.LZ",name))
		{
			// big map territory images
			
			// make new palette
			std::string pal_name = name;
			pal_name.resize(8);
			pal_name += ".PAL";
			auto pal = GetPalette(pal_name);
			if(!pal)
			{
				pal = AddPalette(pal_name);
				std::vector<uint8_t> chunk;
				if(fs->GetFile(pal_name.c_str(),chunk))
					return(1);
				pal->Insert(chunk,pal_name,128);
			}
			
			int w = 379;
			gres.AddRaw(data,flen,w,flen/w,name,pal,false);
		}
		else if(wildcmp("HMLA__??.LZ",name))
		{
			// big map territory images (background)
			std::string pal_name = "LEVEL_??.PAL";
			pal_name[6] = name[6];
			pal_name[7] = name[7];
			auto pal = GetPalette(pal_name);
			if(!pal)
			{
				pal = AddPalette(pal_name);
				std::vector<uint8_t> chunk;
				if(fs->GetFile(pal_name.c_str(),chunk))
					return(1);
				pal->Insert(chunk,pal_name,128);
			}

			int w = 379;
			gres.AddRaw(data,flen,w,flen/w,name,pal,true);
		}
		else if(strcmp(name,"BIG_MAP.LZ") == 0)
		{
			// big map main
			auto pal = GetPalette("BIG_MAP.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,640,flen/640,name,pal);
		}
		else if(strcmp(name,"BM_LSTA.LZ") == 0)
		{
			// big map chunk
			auto pal = GetPalette("BIG_MAP.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,65,flen/65,name,pal);
		}
		else if(strcmp(name, "LISTA_0.LZ") == 0 || strcmp(name,"LISTA_1.LZ") == 0)
		{
			// war map bottom panel
			gres.AddRaw(data,flen,640,flen/640,name,map_pal);
		}
		else if(strcmp(name,"LISTA_0B.LZ0") == 0)
		{
			// war map right panel overlay
			gres.AddRaw(data,flen,160,flen/160,name,map_pal);
		}
		else if(strcmp(name,"LISTAPAT.LZ") == 0)
		{
			// war map bottom panel side filling
			gres.AddRaw(data,flen,32,flen/32,name,map_pal);
		}
		else if(strcmp(name,"GU_LISTA.LZ") == 0)
		{
			// war map unit selection sub-panel
			gres.AddRaw(data,flen,145,flen/145,name,map_pal);
		}
		else if(strcmp(name,"LEV_GFK.LZ") == 0)
		{
			// experience mark
			gres.AddRaw(data,flen,9,flen/9,name,map_pal);
		}
		else if(strcmp(name,"M_ACCOMP.LZ") == 0 || strcmp(name,"M_FAILED.LZ") == 0)
		{
			// war map end title
			gres.AddRaw(data,flen,340,flen/340,name,map_pal);
		}
		else if(wildcmp("MAINMENU.LZ",name))
		{
			// main menu
			auto pal = GetPalette("MAINMENU.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,640,flen/640,name,pal,true);
		}
		else if(wildcmp("MAINM_*.LZ",name))
		{
			// main menu element highlighted
			auto pal = GetPalette("MAINMENU.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,255,flen/255,name,pal,true);
		}
		else if(wildcmp("MAINMD*.LZ",name))
		{
			// main menu element dark
			auto pal = GetPalette("MAINMENU.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,255,flen/255,name,pal,true);
		}
		else if(wildcmp("MM_LOAD.LZ",name))
		{
			// loader frame
			auto pal = GetPalette("MAINMENU.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,402,flen/402,name,pal);
		}
		else if(wildcmp("DIFFIC.LZ",name))
		{
			// difficulty frame
			auto pal = GetPalette("MAINMENU.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,249,flen/249,name,pal);
		}
		else if(wildcmp("DIFF??.LZ",name))
		{
			// difficulty ptions
			auto pal = GetPalette("MAINMENU.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,200,flen/200,name,pal);
		}
		else if(wildcmp("LOGO0001.LZ",name))
		{
			// logo
			auto pal = GetPalette("LOGO0001.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,640,flen/640,name,pal,true);
		}
		else if(wildcmp("NO_CD.LZ",name))
		{
			// no CD
			auto pal = GetPalette("NO_CD.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,640,flen/640,name,pal,true);
		}
		else if(wildcmp("JRC_LOGO.LZ",name))
		{
			// jrc logo
			auto pal = GetPalette("JRC_LOGO.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,640,flen/640,name,pal,true);
		}
		else if(wildcmp("CAULDRON.LZ",name))
		{
			// jrc logo
			auto pal = GetPalette("CAULDRON.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,640,flen/640,name,pal,true);
		}
		else if(wildcmp("PICTURE.LZ",name))
		{
			// picture
			auto pal = GetPalette("PICTURE.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,640,flen/640,name,pal,true);
		}
		else if(wildcmp("SKUSKA.LZ",name))
		{
			// picture
			auto pal = GetPalette("SKUSKA.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,640,flen/640,name,pal,true);
		}
		else if(strcmp(name,"OPT_BAR.LZ") == 0)
		{
			// window frame
			auto pal = GetPalette("BIG_MAP.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,10,flen/10,name,map_pal);
		}
		else if(strcmp(name,"OPTIONS.LZ") == 0)
		{
			// big map game options frame
			auto pal = GetPalette("BIG_MAP.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,569,flen/569,name,pal,true);
		}
		else if(strcmp(name,"BUY.LZ") == 0)
		{
			// buy panel
			auto pal = GetPalette("BIG_MAP.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,406,flen/406,name,pal,true);
		}
		else if(strcmp(name,"RSRCH_BG.LZ") == 0)
		{
			// buy panel
			auto pal = GetPalette("BIG_MAP.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,406,flen/406,name,pal,true);
		}
		else if(strcmp(name,"RES_BAR.LZ") == 0)
		{
			// ?
			gres.AddRaw(data,flen,416,flen/416,name,map_pal,true);
		}
		else if(strcmp(name,"STATS.LZ") == 0)
		{
			// big map stats panel
			auto pal = GetPalette("BIG_MAP.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,569,flen/569,name,pal,true);
		}
		else if(strcmp(name,"FACTORY.LZ") == 0)
		{
			// factory panel
			auto pal = GetPalette("BIG_MAP.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,569,flen/569,name,pal,true);
		}
		else if(strcmp(name,"HIERARCH.LZ") == 0)
		{
			// hierarchy panel
			auto pal = GetPalette("BIG_MAP.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,406,flen/406,name,pal,true);
		}
		else if(strcmp(name,"UNITS.LZ") == 0)
		{
			// hierarchy panel
			auto pal = GetPalette("BIG_MAP.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,406,flen/406,name,pal,true);
		}
		else if(strcmp(name,"INFO.LZ") == 0)
		{
			// big map info panel
			auto pal = GetPalette("BIG_MAP.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,412,flen/412,name,pal,true);
		}
		else if(wildcmp("VM?_FULL.LZ",name))
		{
			// big map panel chunks (outer frame)
			auto pal = GetPalette("BIG_MAP.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,575,flen/575,name,pal,true);
		}
		else if(wildcmp("VMB_LST1.LZ",name))
		{
			// big map panel chunks
			auto pal = GetPalette("BIG_MAP.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,163,flen/163,name,pal,true);
		}
		else if(wildcmp("VMB_LST2.LZ",name))
		{
			// big map panel chunks
			auto pal = GetPalette("BIG_MAP.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,241,flen/241,name,pal,true);
		}
		else if(wildcmp("VMM_LST1.LZ",name))
		{
			// big map panel chunks
			auto pal = GetPalette("BIG_MAP.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,163,flen/163,name,pal,true);
		}
		else if(wildcmp("VMM_LST2.LZ",name))
		{
			// big map panel chunks
			auto pal = GetPalette("BIG_MAP.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,406,flen/406,name,pal,true);
		}
		else if(wildcmp("VMR_LST1.LZ",name))
		{
			// big map panel chunks
			auto pal = GetPalette("BIG_MAP.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,120,flen/120,name,pal,true);
		}
		else if(wildcmp("VMU_LST1.LZ",name))
		{
			// big map panel chunks
			auto pal = GetPalette("BIG_MAP.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,154,flen/154,name,pal,true);
		}
		else if(wildcmp("VMU_LST2.LZ",name))
		{
			// big map panel chunks
			auto pal = GetPalette("BIG_MAP.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,241,flen/241,name,pal,true);
		}
		else if(wildcmp("VMU_SLCT.LZ",name))
		{
			// big map panel chunks
			auto pal = GetPalette("BIG_MAP.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,73,flen/73,name,pal,true);
		}
		else if(wildcmp("VM?_*.LZ",name))
		{
			// big map panel chunks (outer frame)
			auto pal = GetPalette("BIG_MAP.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,0,0,name,pal,true);
		}
		else if(wildcmp("SB_BAR*.LZ",name))
		{
			// big map panel chunks (vertical scroll bar)
			auto pal = GetPalette("BIG_MAP.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,16,flen/16,name,pal,true);
		}
		else if(wildcmp("SB_BG*.LZ",name))
		{
			// big map panel chunks (list boxes)
			auto pal = GetPalette("BIG_MAP.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,163,flen/163,name,pal,true);
		}
		else if(wildcmp("SB_*.LZ",name))
		{
			// big map panel chunks (up/down buttons)
			auto pal = GetPalette("BIG_MAP.PAL");
			if(!pal)
				return(1);
			gres.AddRaw(data,flen,22,flen/22,name,pal,true);
		}
		else if(strcmp(name,"GOTOLSTA.LZ") == 0)
		{
			// map center panel
			gres.AddRaw(data,flen,176,flen/176,name,map_pal);
		}
		else if(strcmp(name,"MAP_OPT.LZ") == 0)
		{
			// window frame
			gres.AddRaw(data,flen,436,flen/436,name,map_pal);
		}
		else if(strcmp(name,"WM_STAT.LZ") == 0)
		{
			// was map stats
			gres.AddRaw(data,flen,408,flen/408,name,map_pal);
		}
		else if(wildcmp("*.ICO",name) || wildcmp("*.BTN",name))
		{
			// ICO files (compression like in PNM files)			
			gres.AddICO(data, flen, name,map_pal);
		}
		else if(wildcmp("*.CUR",name))
		{
			// CUR files (simple bitmaps with dimensions and transparencies)
			gres.AddCUR(data,flen,name,map_pal);
		}
		else if(wildcmp("*.GFK",name))
		{
			// GFK projection files: fixed 21x21 pixel with transparencies
			gres.AddRaw(data,flen,21,21,name,map_pal);
		}
		else if(strcmp(name,"I_ATTACK") == 0 || strcmp(name,"I_LOWER") == 0 || strcmp(name,"I_MOVE") == 0 || strcmp(name,"I_UPPER") == 0 || strcmp(name,"I_SELECT") == 0)
		{
			// raw icon files
			gres.AddRaw(data,flen,20,20,name,map_pal,true);
		}
		else if(strcmp(name,"I_TAB") == 0)
		{
			// raw icon files
			gres.AddRaw(data,flen,60,45,name,map_pal,true);
		}
		else if(wildcmp("RAM?HORZ.DTA",name))
		{
			// frame part
			gres.AddRaw(data,flen,76,flen/76,name,map_pal);
		}
		else if(wildcmp("RAM*.DTA",name))
		{
			// frame part
			gres.AddRaw(data,flen,10,flen/10,name,map_pal);
		}
		else if(wildcmp("*.PNM",name))
		{
			// PNM animations:
			gres.AddPNM(data,flen,name);
		}
		else if(wildcmp("*.LZ",name))
		{
			// unknown *.LZ stuff

			// try fetch palette of matching name or use default map palette
			auto spec_pal = GetPalette(pal_name);
			if(!spec_pal)
				spec_pal = map_pal;

			gres.AddRaw(data,flen,0,0,name,spec_pal);
		}
		else
			continue;

		if(status_item)
			status_item(name);
	}

	// make round LED indicators for mission HUD
	gres.AddLED(204,"RLED_OFF",map_pal);
	gres.AddLED(253,"RLED_ON",map_pal);
	gres.AddLED(229,"YLED_ON",map_pal);
	
	// --- DO NOT ADD ANYTHING TO LIST AFTER HERE!!! it would change memory locations!

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
	gres.wm_map_opt_frame = gres.GetResource("MAP_OPT");
	gres.wm_map_opt_btn_disabled = gres.GetResource("WMOPT__D");
	gres.wm_map_opt_btn_idle = gres.GetResource("WMOPT__N");
	gres.wm_map_opt_btn_hover = gres.GetResource("WMOPT__A");
	gres.wm_map_opt_btn_down = gres.GetResource("WMOPT__P");
	gres.wm_map_units_list = gres.GetResource("GU_LISTA");

	// render cursors
	gres.cur_pointer = gres.RenderCUR("SIPKA.CUR");
	gres.cur_wait = gres.RenderCUR("WAIT.CUR");
	gres.cur_select = gres.RenderCUR("SELECT.CUR");
	gres.cur_question = gres.RenderCUR("OTAZNIK.CUR");
	gres.cur_move = gres.RenderCUR("DOJAZD.CUR");
	gres.cur_attack_down = gres.RenderCUR("TARGT_D.CUR");
	gres.cur_attack_up = gres.RenderCUR("TARGT_U.CUR");
	gres.cur_attack_up_down = gres.RenderCUR("OTAZNIK.CUR");

	// unit cursor
	gres.pnm_sipka = gres.GetPNM("SIPKA");
	
	// order projectiles
	gres.SortProjectiles();

	return(0);
}


// generate special tiles (solid A-M, grid A-M, selection A-M), call only after at least one terrain loaded!
int SpellData::GenerateSpecialTiles()
{
	for(int slope = 'A'; slope <= 'M'; slope++)
	{
		// get any tile for the slope
		std::string wild = "PLx*";
		wild[2] = slope;
		Sprite *spr = terrain[0]->GetSpriteWild(wild.c_str(),Terrain::WildMode::FIRST);
		if(!spr)
			return(1);
		special.select[slope - 'A'] = *spr;
		special.solid[slope - 'A'] = special.select[slope - 'A'];
		Sprite *sel = &special.select[slope - 'A'];
		Sprite* solid = &special.solid[slope - 'A'];
		
		// clear selection sprite
		uint8_t* data = sel->data.data();
		for(int y = 0; y < sel->y_size; y++)
		{
			int ofs = *(int*)data; data += sizeof(int);
			int len = *(int*)data; data += sizeof(int);
			std::memset(data, 0x00, len);
			data += len;
		}

		// clear solid sprite
		data = solid->data.data();
		for(int y = 0; y < solid->y_size; y++)
		{
			int ofs = *(int*)data; data += sizeof(int);
			int len = *(int*)data; data += sizeof(int);
			std::memset(data,0xFF,len);
			data += len;
		}
		
		// make selection frame
		const int fwidth = 3;
		const int fxgap = 3;
		const int fygap = 2;
		const uint8_t sel_color = 0xC1;
		/*for(int y = spr->y_ofs+fygap; y < spr->y_ofs + spr->y_size - fygap; y++)
		{
			int x_first = -1;
			int x_last = -1;
			for(int x = spr->x_ofs; x <= spr->x_ofs + spr->x_size; x++)
			{
				auto *pix = spr->GetPixel(x,y);
				if(pix && x_first < 0)
					x_first = x;
				if(pix)
					x_last = x;
			}
			if(x_last < 0 || x_first < 0)
				continue;
			for(int x = x_first + fxgap; x <= min(x_first + fxgap + fwidth,x_last - fxgap); x++)
				spr->SetPixel(x,y,sel_color);
			for(int x = x_last - fxgap; x >= max(x_last - fxgap - fwidth,x_first + fxgap); x--)
				spr->SetPixel(x,y,sel_color);
		}*/
		for(int x = sel->x_ofs+3; x < sel->x_ofs + sel->x_size - 3; x++)
		{
			int y_first = -1000;
			int y_last = -1000;
			for(int y = sel->y_ofs; y <= sel->y_ofs + sel->y_size; y++)
			{
				auto* pix = sel->GetPixel(x,y);
				if(pix && y_first <= -1000)
					y_first = y;
				if(y_first > -1000 && !pix)
				{
					y_last = y - 1;
					break;
				}
			}
			if(y_last <= -1000 || y_first <= -1000)
				continue;
			for(int y = y_first + fygap; y <= min(y_first + fygap + fwidth,y_last - fygap); y++)
				sel->SetPixel(x,y,sel_color);
			for(int y = y_last - fygap; y >= max(y_last - fygap - fwidth,y_first + fygap); y--)
				sel->SetPixel(x,y,sel_color);
		}

		// make grid edges
		const uint8_t grid_color = 0xC1;
		special.grid[slope - 'A'] = special.solid[slope - 'A'];
		Sprite* grid = &special.grid[slope - 'A'];
		for(int x = grid->x_ofs; x < grid->x_ofs + grid->x_size; x++)
		{
			int y_first = -1000;
			for(int y = sel->y_ofs; y <= sel->y_ofs + sel->y_size; y++)
			{
				auto* pix = sel->GetPixel(x,y);
				if(pix && y_first <= -1000)
				{
					y_first = y;
					grid->SetPixel(x,y,grid_color);
				}
				if(y_first > -1000 && !pix)
				{
					grid->SetPixel(x,y - 1,grid_color);
					break;
				}
			}
		}
		
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
	for(auto & terr : terrain)
		if(_stricmp(terr->name.c_str(), name) == 0)
			return(terr);
	return(NULL);
}
// get terrain pointer by index or NULL
Terrain* SpellData::GetTerrain(int index)
{
	if(index >= terrain.size())
		return(NULL);
	return(terrain[index]);
}



//=============================================================================
// Spellcross palette
//=============================================================================

// make palette record
SpellPalette::SpellPalette()
{
	m_name = "empty";
	m_pal.assign(3*256,0);
	m_used.assign(256,0);
}

// make palette record
SpellPalette::SpellPalette(std::string name)
{
	m_name = name;
	m_pal.assign(3*256,0);
	m_used.assign(256,0);
}

// clear palette colors
void SpellPalette::Clear()
{
	m_pal.assign(3*256,0);
	m_used.assign(256,0);
	m_name = "";
	m_chunks.clear();
}

// place chunk of data to palette with offset (0 - 255)
int SpellPalette::Insert(std::vector<uint8_t>& data,std::string name,int offset)
{
	if(data.size() % 3 || data.size()/3 + offset > 256)
		return(1);
	memcpy(m_pal.data() + offset*3,data.data(),data.size());
	memset(m_used.data() + offset,1,data.size()/3);
	
	// add chunk record
	SpellPalette::Chunk chunk;
	chunk.name = name;
	chunk.offset = offset;
	chunk.size = data.size()/3;
	m_chunks.push_back(chunk);
		
	return(0);
}

// place chunk of data to palette from file with offset (0 - 255)
int SpellPalette::Insert(std::wstring path,int offset,std::string used)
{
	ifstreamext fr(path.c_str(),ios::in);
	if(!fr.is_open())
		return(1);
	auto chunk = fr.read_vector();
	fr.close();
	auto name = std::filesystem::path(path).filename().string();
	if(Insert(chunk,name,offset))
		return(1);
	
	// try assign mask of used colors from string style: 0, 128-191, ...
	if(used.empty())
		return(1);
	m_used.assign(256,0);

	// parse used string
	std::regex regexz(",");
	auto chunks = vector<std::string>(std::sregex_token_iterator(used.begin(),used.end(),regexz,-1),std::sregex_token_iterator());
	for(auto &chunk: chunks)
	{
		auto list = regexp_get(chunk,"\\s*([\\d]+)\\s*-*\\s*([\\d]+)*");
		if(list.size() < 1)
			return(1);
		int from = std::atoi(list[0].c_str());
		int to = -1;
		if(list.size() >= 2 && !list[1].empty())
			to = std::atoi(list[1].c_str());
		if(from > 255 || to > 255)
			return(1);		
		m_used[from] = 1;
		if(to > 0)
			std::fill(m_used.begin() + from, m_used.begin() + to, 1);
	}
	return(0);
}

// get assigned range
std::tuple<int,int> SpellPalette::GetRange(int start)
{
	auto beg = std::find(m_used.begin() + start, m_used.end(), 1);
	if(beg == m_used.end())
		return std::tuple(-1,-1);
	auto end = std::find(beg,m_used.end(),0);
	if(end == m_used.end())
		end = m_used.end();	
	return std::tuple(beg - m_used.begin(),end - m_used.begin() - 1);
}

// get assigned range string
std::string SpellPalette::GetRangeString()
{
	int offset = 0;
	bool was0 = false;
	std::string palstr = "";
	while(true)
	{
		auto [p1,p2] = GetRange(offset);
		if(p1 < 0 || p2 < 0)
			break;
		if(p1 == 0)
			was0 = true;
		if(offset)
			palstr += ", ";
		palstr += string_format("%d-%d",p1,p2);
		offset = p2 + 1;
	}
	if(!was0)
	palstr = "0, " + palstr;
	return(palstr);
}

// save palette to file
int SpellPalette::Save(std::wstring path)
{
	// try open file
	ofstreamext fw(path,ios::out | ios::binary | ios::trunc);
	if(!fw.is_open())
		return(1);

	fw.write((const char*)m_pal.data(),m_pal.size());
	fw.close();

	return(0);
}

// save palette chunks to file
int SpellPalette::SaveChunks(std::wstring directory_path)
{
	// for each palette chunk:
	for(auto &chunk: m_chunks)
	{
		if(chunk.offset + chunk.size > m_pal.size()/3)
			return(1);

		// try make file
		auto path = std::filesystem::path(directory_path).append(chunk.name).wstring();
		ofstreamext fw(path,ios::out | ios::binary | ios::trunc);
		if(!fw.is_open())
			return(1);
				
		// store chunk
		fw.write((const char*)m_pal.data() + chunk.offset*3,chunk.size*3);
		fw.close();
	}

	return(0);
}



// save palette as info file
int SpellPalette::SaveInfo(std::wstring path)
{
	std::string info = "";

	info += string_format("// Spellcross palette meta file (autogenerated by Spellcross Map Editor)\n");
	info += string_format("name:: %s\n",m_name.c_str());
	info += string_format("size:: %d\n",m_pal.size()/3);
	info += string_format("assigned:: %s\n",GetRangeString().c_str());

	std::vector<std::string> list;
	for(int k = 0; k < m_pal.size() - 2; k += 3)
		list.push_back(string_format("%3u, %3u, %3u",m_pal[k + 0],m_pal[k + 1],m_pal[k + 2]));
	info += string_format("\n");
	info += info_make_text_vector("colors",list,"// format: R, G, B");

	std::vector<std::string> chunk_names;
	for(auto &chunk: m_chunks)
		chunk_names.push_back(chunk.name);	
	info += string_format("\n");
	info += info_make_text_vector("chunks list",chunk_names);
			
	for(auto& chunk: m_chunks)
	{
		std::string cinf = "";
		
		cinf += string_format("offset:: %d\n",chunk.offset);
		cinf += string_format("size:: %d\n\n",chunk.size);
		
		std::vector<std::string> list;
		for(int k = chunk.offset; k < chunk.offset + chunk.size; k += 3)
			list.push_back(string_format("%3u, %3u, %3u",m_pal[k + 0],m_pal[k + 1],m_pal[k + 2]));
		cinf += info_make_text_vector("colors",list,"// format: R, G, B");

		info += string_format("\n");
		info += info_make_section(chunk.name,cinf);
	}
	
	// try open file
	ofstreamext fw(path,ios::out | ios::binary | ios::trunc);
	if(!fw.is_open())
		return(1);
	fw.write((const char*)info.data(),info.size());
	fw.close();
}

// load palette from info file
int SpellPalette::LoadInfo(std::wstring path)
{
	Clear();

	// load source info
	ifstreamext fr(path.c_str(),ios::in);
	if(!fr.is_open())
		return(1);
	auto info = fr.read_str();
	fr.close();

	m_name = info_get_string(info, "name");
	if(m_name.empty())
		return(1);
	
	// read common colors list
	auto colors_list = info_get_text_vector(info,"colors");
	int cid = 0;
	for(auto &color: colors_list)
	{
		int r,g,b;
		if(cid >= 3*256 || std::sscanf(color.c_str(),"%d,%d,%d",&r,&g,&b) != 3)
		{
			Clear();
			return(1);
		}
		m_pal[cid + 0] = r;
		m_pal[cid + 1] = g;
		m_pal[cid + 2] = b;
		cid += 3;		
	}

	// parse assigned string
	auto assigned = info_get_string(info,"assigned");
	if(assigned.empty())
	{
		Clear();
		return(1);
	}
	std::regex regexz(",");
	auto chunks = vector<std::string>(std::sregex_token_iterator(assigned.begin(),assigned.end(),regexz,-1),std::sregex_token_iterator());
	for(auto& chunk: chunks)
	{
		auto list = regexp_get(chunk,"\\s*([\\d]+)\\s*-*\\s*([\\d]+)*");
		if(list.size() < 1)
		{
			Clear();
			return(1);
		}
		int from = std::atoi(list[0].c_str());
		int to = -1;
		if(list.size() >= 2 && !list[1].empty())
			to = std::atoi(list[1].c_str());
		if(from > 255 || to > 255)
		{
			Clear();
			return(1);
		}
		m_used[from] = 1;
		if(to > 0)
			std::fill(m_used.begin() + from,m_used.begin() + to,1);
	}

	// parse chunks
	auto chunk_list = info_get_text_vector(info, "chunks list");
	for(auto &chunk_name: chunk_list)
	{
		auto cinf = info_get_section(info,chunk_name);
		Chunk chunk;
		chunk.name = chunk_name;
		chunk.offset = info_get_int(cinf,"offset",-1);
		chunk.size = info_get_int(cinf,"size",-1);
		if(chunk.size < 0 || chunk.offset < 0 || chunk.offset + chunk.size > 256)
		{
			Clear();
			return(1);
		}
		m_chunks.push_back(chunk);
	}

	return(0);
}


// render palette into bitmap (scale up as much as possible)
int SpellPalette::Render(wxBitmap& bmp)
{
	// canvas size
	int surf_x = bmp.GetWidth();
	int surf_y = bmp.GetHeight();

	int x_color_width = surf_x/256;
	int x_ofs = (surf_x - x_color_width*256)/2;
	int x_end = x_ofs + x_color_width*256;

	// split vertically
	int filter_y_limit = surf_y/2;

	// palette
	uint8_t (*pal)[3] = (uint8_t(*)[3])m_pal.data();

	// render 24bit RGB data to raw bmp buffer
	wxNativePixelData data(bmp);
	wxNativePixelData::Iterator p(data);
	for(int y = 0; y < surf_y; ++y)
	{
		uint8_t* scan = p.m_ptr;
		for(int x = 0; x < surf_x; x++)
		{
			if(x < 1 || x >= surf_x-1 || y < 1 || y >= surf_y-1)
			{
				*scan++ = 0x00;
				*scan++ = 0x00;
				*scan++ = 0x00;
			}
			else if(x >= x_ofs && x < x_end)
			{
				int color = (x - x_ofs)/x_color_width;
				*scan++ = pal[color][2];
				*scan++ = pal[color][1];
				*scan++ = pal[color][0];
			}
			else
			{
				uint8_t color = (!(x&32) ^ !(y&32))?0x88:0xAA;
				*scan++ = color;
				*scan++ = color;
				*scan++ = color;
			}
		}
		p.OffsetY(data,1);
	}

	return(0);
}



// make new palette if not there yet, return pointer
SpellPalette* SpellData::AddPalette(std::string name)
{
	auto pal = GetPalette(name);
	if(!pal)
		pal = new SpellPalette(name);
	pal_list.push_back(pal);
	return(pal);
}

// get palette by name
SpellPalette* SpellData::GetPalette(std::string name)
{
	for(auto& pal: pal_list)
		if(!pal->m_name.compare(name))
			return(pal);
	return(NULL);
}

// get palette by name
uint8_t* SpellData::GetPaletteData(std::string name)
{
	auto pal = GetPalette(name);
	if(!pal)
		return(NULL);
	return(pal->m_pal.data());
}