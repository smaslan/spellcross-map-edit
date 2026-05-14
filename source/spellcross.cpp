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

int SpellData::FindArchive(std::vector<std::filesystem::path> &paths, std::string name, std::filesystem::path &arch_path, std::filesystem::path *dir_path, bool optional, std::string *error_msg)
{
	if(error_msg)
		error_msg->clear();
	if(dir_path)
		dir_path->clear();
	arch_path.clear();

	for(auto &dir: paths)
	{
		if(dir.empty())
			continue;
		auto path = dir / name;
		if(!std::filesystem::exists(path))
			continue;
		// found
		if(dir_path)
			*dir_path = dir;
		arch_path = path;
		return(0);
	}
	// not found
	if(optional)
		return(-1);
	if(!error_msg)
		return(1);
	// build error message
	*error_msg = string_format("Non-optional game archive \"%s\" not found at any of paths:\n",name.c_str());
	for(auto &dir: paths)
		if(!dir.empty())
			*error_msg += string_format("  %ls\n",dir.wstring().c_str());
	return(1);
}

// make empty Spellcross data
SpellData::SpellData()
{
	last_error = "";
	font = NULL;
	font7 = NULL;
	units = NULL;
	units_fsu = NULL;
	sounds = NULL;
	midi = NULL;
	texts = NULL;
	research_texts = NULL;
	L2_classes = NULL;
	unit_bonuses = NULL;
	common_fs = NULL;
	terrain_fs = NULL;
	videos = NULL;
}

// make with loading Spellcross data 
SpellData::SpellData(std::filesystem::path& data_path,std::filesystem::path& cd_data_path,std::filesystem::path& mod_path,std::filesystem::path& spec_path,std::function<void(std::string)> status_list,std::function<void(std::string)> status_item)
{
	SpellData();
	if(Reload(data_path,cd_data_path,mod_path,spec_path,status_list,status_item))
		throw runtime_error(last_error);
}

// load or reaload Spellcross data
int SpellData::Reload(std::filesystem::path &data_path,std::filesystem::path& cd_data_path,std::filesystem::path &mod_path,std::filesystem::path& spec_path,std::function<void(std::string)> status_list,std::function<void(std::string)> status_item)
{
	// cleanup old data
	Cleanup();

	// store data paths for dynamic loading
	this->data_path = data_path;
	this->cd_data_path = cd_data_path;

	uint8_t* data;
	int size;
	
	// store path
	spell_data_root = data_path;

	// list of FS/FSU data search folders in order by priority
	std::vector<std::filesystem::path> fs_search_paths = {mod_path, data_path, cd_data_path};

	std::filesystem::path arch_path;
	std::string err_msg;
		
	// load COMMON.FS
	if(status_list)
		status_list("Loading COMMON.FS archive...");	
	if(FindArchive(fs_search_paths, "COMMON.FS",arch_path, NULL, false, &err_msg))
	{
		Cleanup();
		if(status_list)
			status_list(" - failed!");
		last_error = err_msg;
		return(1);
	}
	try{
		common_fs = new FSarchive(arch_path,FSarchive::Options::DELZ_ALL);
	}catch(const runtime_error& error){
		Cleanup();
		if(status_list)
			status_list(" - failed!");
		last_error = string_format("Loading COMMON.FS archive failed (%s)!",error.what());
		return(1);
	}

	// load INFO.FS (units art)
	if(status_list)
		status_list("Loading INFO.FS archive...");
	if(FindArchive(fs_search_paths,"INFO.FS",arch_path,NULL,false,&err_msg))
	{
		Cleanup();
		if(status_list)
			status_list(" - failed!");
		last_error = err_msg;
		return(1);
	}
	try {
		info_fs = new FSarchive(arch_path,FSarchive::Options::DELZ_ALL);
	}
	catch(const runtime_error& error) {
		Cleanup();
		if(status_list)
			status_list(" - failed!");
		last_error = string_format("Decoding INFO.FS failed (%s)!",error.what());
		return(1);
	}
		
	// load sound stuff
	if(status_list)
		status_list("Loading sound samples...");
	try{
		sounds = new SpellSounds(common_fs,data_path,16,status_list,status_item);
	}catch(const runtime_error& error) {
		Cleanup();
		if(status_list)
			status_list(" - failed!");
		last_error = string_format("Loading sound samples filed (%s)!",error.what());
		return(1);
	}

	// load MIDI stuff
	if(status_list)
		status_list("Loading MIDI files...");			
	try {		
		midi = new SpellMIDI(fs_search_paths,true,status_list,status_item);
	}catch(const runtime_error& error) {
		Cleanup();
		if(status_list)
			status_list(" - failed!");
		last_error = string_format("Loading MIDI files failed:\n%s",error.what());
		return(1);
	}
	
	// load L2 object classes stuff
	if(status_list)
		status_list("Loading destructible object classes...");
	try{
		L2_classes = new SpellL2classes(common_fs,sounds,status_list,status_item);
	}catch(const runtime_error& error) {
		Cleanup();
		if(status_list)
			status_list(" - failed!");
		last_error = string_format("Loading destructible object classes failed (%s)!",error.what());
		return(1);
	}

	// load unit bonuses BONUSES.DEF
	if(status_list)
		status_list("Loading unit experience bonuses...");
	try{
		string bonus_def = common_fs->GetFile("BONUSES.DEF");
		unit_bonuses = new UnitBonuses(bonus_def);
	}catch(const runtime_error& error) {
		Cleanup();
		if(status_list)
			status_list(" - failed!");
		last_error = string_format("Loading unit bonuses failed (%s)!",error.what());
		return(1);
	}
	
	// load palette chunks
	if(status_list)
		status_list("Loading palette files...");
	if(LoadPalettes(common_fs,info_fs))
	{
		auto msg = last_error;
		Cleanup();
		if(status_list)
			status_list(string_format(" - failed with message: %s",msg.c_str()));
		last_error = string_format("missing palette file(s) in COMMON.FS: %s",msg.c_str());
		return(1);
	}

	// load generic graphic resources
	if(status_list)
		status_list("Loading common graphics resources...");
	if(LoadAuxGraphics(common_fs,status_item))
	{
		auto msg = last_error;
		Cleanup();
		if(status_list)
			status_list(string_format(" - failed with message: %s",msg.c_str()));
		last_error = string_format("Loading common graphics resources failed with message: %s",msg.c_str());
		return(1);
	}

	// load info graphics resources
	if(status_list)
		status_list("Loading unit info graphics resources...");
	if(LoadInfoGraphics(info_fs,status_item))
	{
		auto msg = last_error;
		Cleanup();
		if(status_list)
			status_list(string_format(" - failed with message: %s",msg.c_str()));
		last_error = string_format("Loading info graphics resources failed with message: %s",msg.c_str());
		return(1);
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
		
		// try load FS archive
		if(FindArchive(fs_search_paths,name,arch_path,NULL,false,&err_msg))
		{
			Cleanup();
			if(status_list)
				status_list(" - failed!");
			last_error = err_msg;
			return(1);
		}
		try{
			terrain_fs = new FSarchive(arch_path);
		}catch(const runtime_error& error) {
			Cleanup();
			if(status_list)
				status_list("   - failed!");
			last_error = string_format("Loading \"%ls\" archive failed:\n%s",arch_path,error.what());
			return(1);
		}

		// try load aux FS data
		auto aux_path = std::filesystem::path(spec_path) / std::filesystem::path(name);
		try{			
			terrain_fs->Append(aux_path);
		}catch(...){
			// do nothing, optional data
		};
		
		// make new terrain
		Terrain* new_terrain = new Terrain();
		if(new_terrain->Load(terrain_fs, map_pal, &gres, L2_classes, status_item))
		{
			Cleanup();
			if(status_list)
				status_list(" - failed!");
			last_error = string_format("Loading terrain \"%s\" failed!",name.c_str());
			return(1);
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
	if(FindArchive(fs_search_paths,"UNITS.FSU",arch_path,NULL,false,&err_msg))
	{
		Cleanup();
		if(status_list)
			status_list(" - failed!");
		last_error = err_msg;
		return(1);
	}
	try{
		units_fsu = new FSUarchive(arch_path, FSUarchive::Options::NONE, status_item);
	}catch(const runtime_error& error) {
		Cleanup();
		if(status_list)
			status_list(" - failed!");
		last_error = string_format("Loading UNITS.FSU units graphics failed (%s)!",error.what());
		return(1);
	}	
	

	// load JEDNOTKY.DEF units definition file
	if(status_list)
		status_list("Loading units definitions (JEDNOTKY.DEF)...");
	if(common_fs->GetFile("JEDNOTKY.DEF", &data, &size))
	{		
		Cleanup();
		if(status_list)
			status_list(" - loading failed!");
		last_error = "JEDNOTKY.DEF not found in COMMON.FS!";
		return(1);
	}
	try{
		units = new SpellUnits(data, size, units_fsu, info_fs, &gres, &gres_info, sounds, unit_bonuses);
	}catch(const runtime_error& error) {
		Cleanup();
		if(status_list)
			status_list(" - decoding failed!");
		last_error = string_format("Decoding JEDNOTKY.DEF failed (%s)!",error.what());
		return(1);
	}

	// load font file
	if(status_list)
		status_list("Loading font files...");
	if(common_fs->GetFile("FONT_001.FNT",&data,&size))
	{
		// failed
		Cleanup();
		if(status_list)
			status_list(" - loading main spellcross font file failed!");
		last_error = "Font file FONT_001.FNT not found in FS archive!";
		return(1);
	}
	try{
		font = new SpellFont(data, size);
	}catch(const runtime_error& error) {
		Cleanup();
		if(status_list)
			status_list(" - failed!");
		last_error = string_format("Decoding font file failed (%s)!",error.what());
		return(1);
	}

	// close common.fs
	delete common_fs;
	common_fs = NULL;


	// load TEXTS.FS
	if(status_list)
		status_list("Loading string tables...");
	if(FindArchive(fs_search_paths,"TEXTS.FS",arch_path,NULL,false,&err_msg))
	{
		Cleanup();
		if(status_list)
			status_list(" - failed!");
		last_error = err_msg;
		return(1);
	}	
	try{
		auto texts_fs = new FSarchive(arch_path);
		texts = new SpellTexts(texts_fs, SpellLang::CZE,SpellTextRec::TextPanel::DEFAULT, sounds); // ###todo: decode language somehow?
		delete texts_fs;
	}catch(const runtime_error& error) {		
		Cleanup();
		if(status_list)
			status_list(" - failed!");
		last_error = string_format("Decoding string tables failed (%s)!",error.what());
		return(1);
	}

	// load RESEARCH.FS
	if(status_list)
		status_list("Loading research...");
	//wstring research_path = std::filesystem::path(data_path) / std::filesystem::path("RESEARCH.FS");
	if(FindArchive(fs_search_paths,"RESEARCH.FS",arch_path,NULL,false,&err_msg))
	{
		Cleanup();
		if(status_list)
			status_list(" - failed!");
		last_error = err_msg;
		return(1);
	}
	FSarchive* rsch_fs;
	try{
		rsch_fs = new FSarchive(arch_path);
	}catch(const runtime_error& error) {
		Cleanup();
		if(status_list)
			status_list(" - failed!");
		last_error = string_format("Decoding string tables failed:\n%s!",error.what());
		return(1);
	}
	if(LoadResearch(rsch_fs,status_item))
	{
		delete rsch_fs;
		Cleanup();
		if(status_list)
			status_list(" - failed!");
		last_error = string_format("Loading research string failed!");
		return(1);
	}
	delete rsch_fs;
		

	// load special tiles
	if(status_list)
		status_list("Loading special tiles (selection, etc.)...");
	if(GenerateSpecialTiles())
	{
		Cleanup();
		if(status_list)
			status_list(" - failed!");
		last_error = "Loading special selection tiles failed!";
		return(1);
	}

	// load aux 7pix font
	if(status_list)
		status_list("Loading aux 7-pix font...");
	wstring font7_path = std::filesystem::path(spec_path) / std::filesystem::path("font_spellcross_7pix.fnt");
	try{
		font7 = new SpellFont(font7_path);
	}catch(const runtime_error& error) {
		Cleanup();
		if(status_list)
			status_list(" - failed!");
		last_error = string_format("Failed loading aux font ''%ls'' (%s)!",font7_path.c_str(),error.what());
		return(1);
	}

	// load aux 14pix font and merge it to spellcross font
	if(status_list)
		status_list("Loading aux 14-pix font...");
	wstring font14_path = std::filesystem::path(spec_path) / std::filesystem::path("font_spellcross_14pix.fnt");
	try{
		SpellFont font14_aux(font14_path);
		font->Merge(font14_aux);
	}catch(const runtime_error& error) {
		Cleanup();
		if(status_list)
			status_list(" - failed!");
		last_error = string_format("Failed loading aux font ''%ls'' (%s)!",font14_path.c_str(),error.what());
		return(1);
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
		videos = new SpellVideoResources(fs_search_paths);
	}catch(const runtime_error& error) {
		Cleanup();
		if(status_list)
			status_list(" - failed!");
		last_error = string_format("Loading video resources from MOVIE.FS and SPEAKER.FS failed:\n%s",error.what());
		return(1);
	}
	
	return(0);
}

// cleanup spellcross data
SpellData::~SpellData()
{
	Cleanup();
}
void SpellData::Cleanup()
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
	if(info_fs)
		delete info_fs;
	info_fs = NULL;
	if(sounds)
		delete sounds;
	sounds = NULL;
	if(midi)
		delete midi;
	midi = NULL;
	if(texts)
		delete texts;
	texts = NULL;
	if(research_texts)
		delete research_texts;
	research_texts = NULL;
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
	gres.Clear();
}

// check last error message
std::string SpellData::GetLastError()
{
	return(last_error);
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
int SpellData::LoadPalettes(FSarchive* fs_common, FSarchive* fs_info)
{
	last_error = "";
	std::vector<uint8_t> chunk;
	SpellPalette *pal;
	
	// palette for unints only (used for encoding unit sprites)
	SpellPalette* units_pal = AddPalette("UNITS.PAL");

	// make empty map palette (bottom 128 colors are from terrains)
	pal = AddPalette("MAP");
	
	// UNITS.PAL chunk	
	if(fs_common->GetFile("UNITS.PAL",chunk) || chunk.size() != 96*3)
	{
		last_error = string_format("Missing or corrupted palette: UNITS.PAL!");
		return(1); // missing item!
	}	
	pal->Insert(chunk,"UNITS.PAL",128);
	units_pal->Insert(chunk,"UNITS.PAL",128);
		
	// load SYSTEM.PAL palette chunk for maps
	if(fs_common->GetFile("SYSTEM.PAL",chunk) || chunk.size() != 32*3)
	{
		last_error = string_format("Missing or corrupted palette: SYSTEM.PAL!");
		return(1); // missing item!
	}
	pal->Insert(chunk,"SYSTEM.PAL",224);

	// load CURSOR.PAL palette chunk for maps - ###todo: not sure where to place this
	/*if(common_fs->GetFile("CURSOR.PAL",&data,&size) || size != 6*3)
	{
		Cleanup();
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
	if(fs_common->GetFile("BIG_MAP.PAL",chunk) || chunk.size() != 64*3)
	{
		last_error = string_format("Missing or corrupted palette: BIG_MAP.PAL!");
		return(1); // missing item!
	}
	pal->Insert(chunk,"BIG_MAP.PAL",192);
	// _SHARED1.PAL chunk
	if(fs_common->GetFile("_SHARED1.PAL",chunk) || chunk.size() != 128*3)
	{
		last_error = string_format("Missing or corrupted palette: _SHARED1.PAL!");
		return(1); // missing item!
	}
	pal->Insert(chunk,"_SHARED1.PAL",0);


	// make empty main menu palette
	pal = AddPalette("MAINMENU.PAL");

	// MAINMENU.PAL chunk	
	if(fs_common->GetFile("MAINMENU.PAL",chunk) || chunk.size() != 256*3)
	{
		last_error = string_format("Missing or corrupted palette: MAINMENU.PAL!");
		return(1); // missing item!
	}
	pal->Insert(chunk,"MAINMENU.PAL");

	// make empty main menu palette
	pal = AddPalette("STRATEGY.PAL");

	// MAINMENU.PAL chunk	
	if(fs_common->GetFile("STRATEGY.PAL",chunk) || chunk.size() != 256*3)
	{
		last_error = string_format("Missing or corrupted palette: STRATEGY.PAL!");
		return(1); // missing item!
	}
	pal->Insert(chunk,"STRATEGY.PAL");


	// info frame palette
	pal = AddPalette("PALETA.NRM");
	if(fs_info->GetFile("PALETA.NRM",chunk) || chunk.size() != 256*3)
	{
		last_error = string_format("Missing or corrupted palette: PALETA.NRM (INFO.FS)!");
		return(1); // missing item!
	}
	pal->Insert(chunk,"PALETA.NRM");
					

	return(0);
}

class SpellGrpParams{
public:
	enum GrpType{
		RAW = 0,
		CUR,
		PNM,
		ICO
	};
	std::string wild;
	std::string palette;
	GrpType type;
	int width;	
	bool solid;
};


// load info graphics resources
int SpellData::LoadResearch(FSarchive* fs,std::function<void(std::string)> status_item)
{
	if(!fs)
		return(1);

	research_texts = new SpellTexts(fs, SpellLang::CZE,SpellTextRec::TextPanel::DEFAULT, NULL);
	if(!research_texts)
		return(1);

	return(0);
}

// load info graphics resources
int SpellData::LoadInfoGraphics(FSarchive* fs,std::function<void(std::string)> status_item)
{
	last_error = "";

	// frame chunks
	std::vector<SpellGrpParams> grp_list ={
		{"NAMELSTA.LZ", "PALETA.NRM", SpellGrpParams::GrpType::RAW, 200, true}, // 
		{"INFOLSTA.LZ", "PALETA.NRM", SpellGrpParams::GrpType::RAW, 433, true}, // 
		{"INFOPAN.LZ", "PALETA.NRM", SpellGrpParams::GrpType::RAW, 640, true}, // 
		{"IBG_600.LZ", "PALETA.NRM", SpellGrpParams::GrpType::RAW, 800, true}, // 
		{"IBG_768.LZ", "PALETA.NRM", SpellGrpParams::GrpType::RAW, 1024, true}, //
		{"*.LZ", "", SpellGrpParams::GrpType::RAW, 437, true}, // general unit images
	};

	// for each file:
	for(auto& file: fs->GetFiles())
	{
		// get file data
		const char* name = file->name.c_str();
		int flen = file->data.size();
		uint8_t* data = file->data.data();
		uint8_t* data_end = &data[flen];

		// try to identify each graphical resource:
		for(auto& item: grp_list)
		{
			if(!wildcmp(item.wild.c_str(),file->name.c_str()))
				continue;
			// found matching resource

			SpellPalette *pal;
			if(item.palette.empty())
			{
				// no palette, check if dedicated exists?
				auto pal_name = std::filesystem::path(file->name).stem().concat(".PAL").string();
				std::vector<uint8_t> pal_data;
				fs->GetFile(pal_name.c_str(),pal_data);
				if(pal_data.size() != 3*192)
					break;
				// load it
				pal = AddPalette(pal_name);
				pal->Insert(pal_data);
			}
			else
				pal = GetPalette(item.palette);
			if(!pal)
				break;
			
			// try load resource
			if(item.palette.empty() && item.type == SpellGrpParams::GrpType::RAW)
				gres_info.AddRaw(data,flen,item.width,0,name,pal,item.solid);
			else if(item.type == SpellGrpParams::GrpType::RAW)
				gres.AddRaw(data,flen,item.width,0,name,pal,item.solid);
			break;
		}

		if(status_item)
			status_item(name);
	}

	gres.inf_panel = gres.GetResource("INFOPAN.LZ");
	if(!gres.inf_panel)
	{
		last_error = string_format("Missing mandatory graphics resource: INFOPAN.LZ from INFO.FS!");
		return(1); // missing item!
	}
	
	return(0);
}

// load generic graphics resources
int SpellData::LoadAuxGraphics(FSarchive *fs,std::function<void(std::string)> status_item)
{
	last_error = "";

	std::vector<SpellGrpParams> grp_list = {
		{"I_*.LZ", "MAP", SpellGrpParams::GrpType::RAW, 60, true}, // unit icons
		{"LEVEL_??.LZ", "", SpellGrpParams::GrpType::RAW, 379, false}, // territories
		{"HMLA__??.LZ", "LEVEL_??.PAL", SpellGrpParams::GrpType::RAW, 379, true}, // territories in fog
		{"BIG_MAP.LZ", "", SpellGrpParams::GrpType::RAW, 640, true}, // big map main
		{"BM_LSTA.LZ", "", SpellGrpParams::GrpType::RAW, 65, false}, // big map chunk
		{"LISTA_?.LZ", "MAP", SpellGrpParams::GrpType::RAW, 640, false}, // war map bottom panel
		{"LISTA_0B.LZ0", "MAP", SpellGrpParams::GrpType::RAW, 160, false}, // war map right panel overlay
		{"LISTAPAT.LZ", "MAP", SpellGrpParams::GrpType::RAW, 32, false}, // war map bottom panel side filling
		{"GU_LISTA.LZ", "MAP", SpellGrpParams::GrpType::RAW, 145, false}, // war map unit selection sub-panel
		{"LEV_GFK.LZ", "MAP", SpellGrpParams::GrpType::RAW, 9, false}, // experience mark
		{"M_*.LZ", "MAP", SpellGrpParams::GrpType::RAW, 340, false}, // war map end message
		{"RAD??.LZ", "MAP", SpellGrpParams::GrpType::RAW, 78, false}, // war radar		
		{"MAINMENU.LZ", "MAINMENU.PAL", SpellGrpParams::GrpType::RAW, 640, true}, // main menu background
		{"MAINM_*.LZ", "MAINMENU.PAL", SpellGrpParams::GrpType::RAW, 255, true}, // main menu element highlighted
		{"MAINMD*.LZ", "MAINMENU.PAL", SpellGrpParams::GrpType::RAW, 255, true}, // main menu element grayed
		{"MM_LOAD.LZ", "MAINMENU.PAL", SpellGrpParams::GrpType::RAW, 402, false}, // loader frame
		{"DIFFIC.LZ", "MAINMENU.PAL", SpellGrpParams::GrpType::RAW, 249, false}, // difficulty frame
		{"DIFF??.LZ", "MAINMENU.PAL", SpellGrpParams::GrpType::RAW, 200, false}, // difficulty options
		{"TUTMNU.LZ", "MAINMENU.PAL", SpellGrpParams::GrpType::RAW, 250, false}, // tutorial panel
		{"T0*.LZ", "MAINMENU.PAL", SpellGrpParams::GrpType::RAW, 200, false}, // tutorial options
		{"LOGO0001.LZ", "", SpellGrpParams::GrpType::RAW, 640, true}, // logo
		{"NO_CD.LZ", "", SpellGrpParams::GrpType::RAW, 640, true}, // no cd splash
		{"JRC_LOGO.LZ", "", SpellGrpParams::GrpType::RAW, 640, true}, // JRC logo splash
		{"CAULDRON.LZ", "", SpellGrpParams::GrpType::RAW, 640, true}, // cauldron logo splash
		{"PICTURE.LZ", "", SpellGrpParams::GrpType::RAW, 640, true}, // some picture
		{"SKUSKA.LZ", "", SpellGrpParams::GrpType::RAW, 640, true}, // some picture
		{"OPT_BAR.LZ", "BIG_MAP.PAL", SpellGrpParams::GrpType::RAW, 10, false}, // big map window frame part
		{"OPTIONS.LZ", "BIG_MAP.PAL", SpellGrpParams::GrpType::RAW, 569, true}, // big map options panel
		{"BUY.LZ", "BIG_MAP.PAL", SpellGrpParams::GrpType::RAW, 406, true}, // big map buy panel
		{"RSRCH_BG.LZ", "BIG_MAP.PAL", SpellGrpParams::GrpType::RAW, 406, true}, // big map research panel
		{"STATS.LZ", "BIG_MAP.PAL", SpellGrpParams::GrpType::RAW, 569, true}, // big map statistics panel
		{"FACTORY.LZ", "BIG_MAP.PAL", SpellGrpParams::GrpType::RAW, 569, true}, // big map factory panel
		{"HIERARCH.LZ", "BIG_MAP.PAL", SpellGrpParams::GrpType::RAW, 406, true}, // big map hierarchy panel
		{"UNITS.LZ", "BIG_MAP.PAL", SpellGrpParams::GrpType::RAW, 406, true}, // big map hierarchy panel
		{"INFO.LZ", "BIG_MAP.PAL", SpellGrpParams::GrpType::RAW, 412, true}, // big map info panel
		{"RES_BAR.LZ", "MAP", SpellGrpParams::GrpType::RAW, 0, true}, // something for research, does not match big map palette?
		{"VM?_FULL.LZ", "BIG_MAP.PAL", SpellGrpParams::GrpType::RAW, 575, true}, // big map panel chunks (outer frame)
		{"VMB_LST1.LZ", "BIG_MAP.PAL", SpellGrpParams::GrpType::RAW, 163, true}, // big map panel chunks
		{"VMB_LST2.LZ", "BIG_MAP.PAL", SpellGrpParams::GrpType::RAW, 241, true}, // big map panel chunks
		{"VMM_LST1.LZ", "BIG_MAP.PAL", SpellGrpParams::GrpType::RAW, 163, true}, // big map panel chunks
		{"VMM_LST2.LZ", "BIG_MAP.PAL", SpellGrpParams::GrpType::RAW, 406, true}, // big map panel chunks
		{"VMR_LST1.LZ", "BIG_MAP.PAL", SpellGrpParams::GrpType::RAW, 120, true}, // big map panel chunks
		{"VMU_LST1.LZ", "BIG_MAP.PAL", SpellGrpParams::GrpType::RAW, 154, true}, // big map panel chunks
		{"VMU_LST2.LZ", "BIG_MAP.PAL", SpellGrpParams::GrpType::RAW, 241, true}, // big map panel chunks
		{"VMU_SLCT.LZ", "BIG_MAP.PAL", SpellGrpParams::GrpType::RAW, 73, true}, // big map panel chunks
		{"VM?_*.LZ", "BIG_MAP.PAL", SpellGrpParams::GrpType::RAW, 0, true}, // big map panel chunks - other
		{"SB_BAR*.LZ", "BIG_MAP.PAL", SpellGrpParams::GrpType::RAW, 16, true}, // big map panel chunks (vertical scroll bar)
		{"SB_BG*.LZ", "BIG_MAP.PAL", SpellGrpParams::GrpType::RAW, 163, true}, // big map panel chunks (list boxes)
		{"SB_*.LZ", "BIG_MAP.PAL", SpellGrpParams::GrpType::RAW, 22, true}, // big map panel chunks (up/down buttons)
		{"GOTOLSTA.LZ", "MAP", SpellGrpParams::GrpType::RAW, 176, false}, // war map center panel
		{"MAP_OPT.LZ", "MAP", SpellGrpParams::GrpType::RAW, 436, false}, // war map options panel
		{"WM_STAT.LZ", "MAP", SpellGrpParams::GrpType::RAW, 408, false}, // war map stats panel
		{"ET_*.LZ", "BIG_MAP.PAL", SpellGrpParams::GrpType::RAW, 41, true}, // big map buttons
		{"LED_*.LZ", "BIG_MAP.PAL", SpellGrpParams::GrpType::RAW, 9, false}, // some leds (no idea from where?)
		{"VM*.ICO", "BIG_MAP.PAL", SpellGrpParams::GrpType::ICO, 0, false}, // big map ICO files
		{"LASTTERT.ICO", "BIG_MAP.PAL", SpellGrpParams::GrpType::ICO, 0, false}, // big map ICO files
		{"INFO?*.ICO", "BIG_MAP.PAL", SpellGrpParams::GrpType::ICO, 0, false}, // info panel ICO files (###todo: not sure about palette, maybe PALETA.NRM from info.fs?)
		{"*.ICO", "MAP", SpellGrpParams::GrpType::ICO, 0, false}, // ICO files
		{"BIGM*.BTN", "BIG_MAP.PAL", SpellGrpParams::GrpType::ICO, 0, true}, // big map BTN files
		{"BM*.BTN", "BIG_MAP.PAL", SpellGrpParams::GrpType::ICO, 0, true}, // big map BTN files
		{"VM*.BTN", "BIG_MAP.PAL", SpellGrpParams::GrpType::ICO, 0, true}, // big map BTN files
		{"I*.BTN", "PALETA.NRM", SpellGrpParams::GrpType::ICO, 0, true}, // info panel BTN files
		{"*.BTN", "MAP", SpellGrpParams::GrpType::ICO, 0, false}, // BTN files
		{"*.CUR", "MAP", SpellGrpParams::GrpType::CUR, 0, false}, // cursors
		{"*.GFK", "UNITS.PAL", SpellGrpParams::GrpType::RAW, 21, false}, // projectiles
		{"I_TAB", "MAP", SpellGrpParams::GrpType::RAW, 45, true}, // raw icon files in war map panel
		{"I_*", "MAP", SpellGrpParams::GrpType::RAW, 20, true}, // raw icon files in war map panel
		{"RAM?HORZ.DTA", "MAP", SpellGrpParams::GrpType::RAW, 76, false}, // frame part
		{"RAM*.DTA", "MAP", SpellGrpParams::GrpType::RAW, 10, false}, // frame part
		{"*.PNM", "MAP", SpellGrpParams::GrpType::PNM, 0, false}, // PNM animations
		{"*.LZ", "MAP", SpellGrpParams::GrpType::RAW, 0, false} // other stuff
	};


	// for each file:
	for(auto & file: fs->GetFiles())
	{
		// get file data
		const char *name = file->name.c_str();
		int flen = file->data.size();
		uint8_t *data = file->data.data();
		uint8_t* data_end = &data[flen];
				
		// try to identify each graphical resource:
		for(auto &item: grp_list)
		{
			if(!wildcmp(item.wild.c_str(), file->name.c_str()))
				continue;
			// found matching resource

			// select palette
			SpellPalette* pal = NULL;
			auto pal_name = item.palette;
			if(item.palette.find('?') != std::string::npos)
			{
				// replace ? in palette name by file name symbol
				for(int k = 0; k < min(pal_name.length(), file->name.length()); k++)
					if(item.wild[k] == '?')
						pal_name[k] = item.wild[k];
			}
			if(pal_name.empty())
			{
				// no palette, check if dedicated exists?
				pal_name = std::filesystem::path(file->name).stem().concat(".PAL").string();
				pal = GetPalette(pal_name);
				if(!pal)
				{
					// not loaded, try load it
					std::vector<uint8_t> pal_data;
					fs->GetFile(pal_name.c_str(),pal_data);
					if(pal_data.size() == 3*256)
					{
						// load it
						pal = AddPalette(pal_name);
						pal->Insert(pal_data);
					}
					else
						pal_name = "";
				}
			}
			if(pal_name.empty())
				break; // no palette identified - skip item
			pal = GetPalette(pal_name);
			if(!pal)
				break; // cannot find palette - skip item

			// try load resource
			if(item.type == SpellGrpParams::GrpType::RAW)
				gres.AddRaw(data, flen, item.width, 0, name, pal, item.solid);
			else if(item.type == SpellGrpParams::GrpType::CUR)
				gres.AddCUR(data, flen, name, pal);
			else if(item.type == SpellGrpParams::GrpType::ICO)
				gres.AddICO(data, flen, name, pal, item.solid);
			else if(item.type == SpellGrpParams::GrpType::PNM)
				gres.AddPNM(data, flen, name);
			break;
		}

		if(status_item)
			status_item(name);
	}

	// make round LED indicators for mission HUD
	auto pal = GetPalette("MAP");
	gres.AddLED(204,"RLED_OFF",pal);
	gres.AddLED(253,"RLED_ON",pal);
	gres.AddLED(229,"YLED_ON",pal);		

	// ###todo: make sure none of following is missing!
	// make direct (fast) links to some resoruces
	class SpellGrpShortsList{
	public:
		SpellGraphicItem** item;
		std::string name;		
	};
	std::vector<SpellGrpShortsList> short_links_list = {
		{&gres.red_led_off,"RLED_OFF"},
		{&gres.red_led_on, "RLED_ON"},
		{&gres.yellow_led_on, "YLED_ON"},
		{&gres.wm_hud, "LISTA_0"},
		{&gres.wm_hud_enemy, "LISTA_1"},
		{&gres.wm_hud_sides, "LISTAPAT"},
		{&gres.wm_hud_overlay, "LISTA_0B"},
		{&gres.wm_form[0], "WM_FORM0"},
		{&gres.wm_form[1], "WM_FORM1"},
		{&gres.wm_form[2], "WM_FORM2"},
		{&gres.wm_exp_mark, "LEV_GFK"},
		{&gres.wm_freeze, "MRAZIK"},
		{&gres.wm_paralyze, "PARALIZ"},
		{&gres.wm_btn_idle, "MAINB__D"},
		{&gres.wm_btn_hover, "MAINB__A"},
		{&gres.wm_btn_press, "MAINB__P"},
		{&gres.wm_glyph_air, "AIRUNIT"},
		{&gres.wm_glyph_center_unit, "CENTRUNT"},
		{&gres.wm_glyph_down, "DOWN"},
		{&gres.wm_glyph_up, "UP"},
		{&gres.wm_glyph_radar_down, "RADAROFF"},
		{&gres.wm_glyph_radar_up, "RADARON"},
		{&gres.wm_glyph_end_turn, "ENDTURN"},
		{&gres.wm_glyph_goto_unit, "GOTOUNIT"},
		{&gres.wm_glyph_ground, "GRNDUNIT"},
		{&gres.wm_glyph_map, "MAP"},
		{&gres.wm_glyph_heal, "HEAL"},
		{&gres.wm_glyph_unit_info, "INFO"},
		{&gres.wm_glyph_next, "NEXT"},
		{&gres.wm_glyph_options, "OPTIONS"},
		{&gres.wm_glyph_next_unused, "PREVIOUS"},
		{&gres.wm_glyph_retreat, "RETREAT"},
		{&gres.wm_glyph_end_placement, "UKONCEN"},
		{&gres.wm_glyph_info, "UNITINFO"},
		{&gres.wm_glyph_place_unit, "UKONCEN"},
		{&gres.wm_sel_tab, "I_TAB"},
		{&gres.wm_sel_attack, "I_ATTACK"},
		{&gres.wm_sel_move, "I_MOVE"},
		{&gres.wm_sel_upper, "I_UPPER"},
		{&gres.wm_sel_lower, "I_LOWER"},
		{&gres.wm_sel_select, "I_SELECT"},
		{&gres.wm_frame_horz, "RAM2HORZ.DTA"},
		{&gres.wm_frame_vert, "RAM2VERT.DTA"},
		{&gres.wm_frame_corner, "RAM2ROH.DTA"},
		{&gres.wm_map_opt_frame, "MAP_OPT"},
		{&gres.wm_map_opt_btn_disabled, "WMOPT__D"},
		{&gres.wm_map_opt_btn_idle, "WMOPT__N"},
		{&gres.wm_map_opt_btn_hover, "WMOPT__A"},
		{&gres.wm_map_opt_btn_down, "WMOPT__P"},
		{&gres.wm_map_units_list, "GU_LISTA"},
		{&gres.bm_info_panel, "INFO.LZ"},
		{&gres.bm_research_panel, "RSRCH_BG.LZ"},
		{&gres.bm_mission_info_panel, "VMM_LST2.LZ"}
	};	
	for(auto &item: short_links_list)
	{
		auto grpi = gres.GetResource(item.name.c_str());
		if(!grpi)
		{
			last_error = string_format("Missing mandatory graphics resource: %s!",item.name.c_str());
			return(1); // missing item!
		}
		*item.item = grpi;
	}
	

	// render cursors
	class SpellCurShortsList {
	public:
		wxCursor** item;
		std::string name;
	};
	std::vector<SpellCurShortsList> short_cur_links_list ={
		{&gres.cur_pointer,"SIPKA.CUR"},
		{&gres.cur_wait,"WAIT.CUR"},
		{&gres.cur_select,"SELECT.CUR"},
		{&gres.cur_question,"OTAZNIK.CUR"},
		{&gres.cur_move,"DOJAZD.CUR"},
		{&gres.cur_attack_down,"TARGT_D.CUR"},
		{&gres.cur_attack_up,"TARGT_U.CUR"},
		{&gres.cur_attack_up_down,"OTAZNIK.CUR"}
	};
	for(auto& item: short_cur_links_list)
	{
		auto wcur = gres.RenderCUR(item.name.c_str());
		if(!wcur)
		{
			last_error = string_format("Missing mandatory graphics resource: %s!",item.name.c_str());
			return(1); // missing item!
		}
		*item.item = wcur;
	}

	// unit cursor
	gres.pnm_sipka = gres.GetPNM("SIPKA");
	if(!gres.pnm_sipka)
	{
		last_error = string_format("Missing mandatory graphics resource: SIPKA!");
		return(1); // missing item!
	}
	
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
int SpellPalette::SaveInfo(std::filesystem::path path)
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

	return(0);
}

// load palette from info file
int SpellPalette::LoadInfo(std::filesystem::path path)
{
	Clear();

	// load source info
	std::string infostr;
	if(loadstr(path,infostr))
		return(1);
	auto info = get_text_lines(infostr);

	// palette name
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
	auto chunks = get_text_lines(assigned, true, ',');
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

// render palette color selection into canvas
int SpellPalette::RenderPaletteColor(wxBitmap& bmp,int x_size,int x_pos,uint8_t* filter)
{
	// canvas size
	int surf_x = bmp.GetWidth();
	int surf_y = bmp.GetHeight();

	// palette position and size in canvas
	int x_color_width = x_size/256;
	int x_ofs = (x_size - x_color_width*256)/2;
	int x_end = x_ofs + x_color_width*256;

	int is_selected = x_pos >= x_ofs && x_pos < x_end;
	int pal_id = (is_selected)?((x_pos - x_ofs)/x_color_width):-1;
	if(filter)
		pal_id = filter[pal_id];

	uint8_t(*pal)[3] = (uint8_t(*)[3])m_pal.data();

	// render 24bit RGB data to raw bmp buffer
	wxNativePixelData data(bmp);
	wxNativePixelData::Iterator p(data);
	for(int y = 0; y < surf_y; ++y)
	{
		uint8_t* scan = p.m_ptr;
		for(int x = 0; x < surf_x; x++)
		{
			if(x > 0 && x < surf_x-1 && y > 0 && y < surf_y-1)
			{
				if(is_selected)
				{
					*scan++ = pal[pal_id][2];
					*scan++ = pal[pal_id][1];
					*scan++ = pal[pal_id][0];
				}
				else
				{
					uint8_t color = (!(x&8) ^ !(y&8))?0x88:0xAA;
					*scan++ = color;
					*scan++ = color;
					*scan++ = color;
				}
			}
			else
			{
				*scan++ = 0x00;
				*scan++ = 0x00;
				*scan++ = 0x00;
			}
		}
		p.OffsetY(data,1);
	}

	return(pal_id);
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

