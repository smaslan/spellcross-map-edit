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
	texts = NULL;
	L2_classes = NULL;
	unit_bonuses = NULL;
		
	common = NULL;

	uint8_t* data;
	int size;
	
	// store path
	spell_data_root = data_path;

	// load COMMON.FS
	if(status_list)
		status_list("Loading COMMON.FS archive...");	
	wstring common_path = std::filesystem::path(data_path) / std::filesystem::path("COMMON.FS");
	try{
		common = new FSarchive(common_path);
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
		sounds = new SpellSounds(common,data_path,16,status_list,status_item);
	}catch(const runtime_error& error) {
		this->~SpellData();
		if(status_list)
			status_list(" - failed!");
		throw runtime_error(string_format("Loading sound samples filed (%s)!",error.what()));
	}
	
	// load L2 object classes stuff
	if(status_list)
		status_list("Loading destructible object classes...");
	try{
		L2_classes = new SpellL2classes(common,sounds,status_list,status_item);
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
		string bonus_def = common->GetFile("BONUSES.DEF");	
		unit_bonuses = new UnitBonuses(bonus_def);
	}catch(const runtime_error& error) {
		this->~SpellData();
		if(status_list)
			status_list(" - failed!");
		throw runtime_error(string_format("Loading unit bonuses failed (%s)!",error.what()));
	}

	// load terrains
	if(status_list)
		status_list("Loading terrain graphics data...");
	vector<string> terrain_list = {"T11.FS", "PUST.FS", "DEVAST.FS"};
	for(auto & name : terrain_list)
	{
		// terrain archive path
		wstring path = std::filesystem::path(data_path) / std::filesystem::path(name);
		// aux terrain data path
		wstring aux_path = std::filesystem::path(spec_path) / std::filesystem::path(name);

		// load terrain
		if(status_list)
			status_list(string_format(" - loading terrain ''%s''",name.c_str()));
		Terrain* new_terrain = new Terrain();
		if(new_terrain->Load(path, aux_path, L2_classes, status_item))
		{
			this->~SpellData();
			if(status_list)
				status_list(" - failed!");
			throw runtime_error(string_format("Loading terrain ''%s'' failed!",name.c_str()));
		}

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
		
	// load UNITS.PAL palette chunk and merge with terrain palette chunk
	if(status_list)
		status_list("Loading palette files...");
	if(common->GetFile("UNITS.PAL", &data, &size))
	{
		this->~SpellData();
		if(status_list)
			status_list(" - missing UNITS.PAL!");
		throw runtime_error("UNITS.PAL not found in COMMON.FS!");
	}
	for (unsigned k = 0; k < this->terrain.size(); k++)
		std::memcpy((void*)&this->terrain[k]->pal[128][0], (void*)data, 96 * 3);
	
	// load SYSTEM.PAL palette chunk and merge with terrain palette chunk
	if (common->GetFile("SYSTEM.PAL", &data, &size))
	{
		this->~SpellData();
		if(status_list)
			status_list(" - missing SYSTEM.PAL!");
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
	if(status_list)
		status_list("Loading common graphics resources...");
	if(LoadAuxGraphics(common,status_item))
	{
		this->~SpellData();
		if(status_list)
			status_list(" - failed!");
		throw runtime_error("Loading common graphics resources failed!");
	}
	
	// processing all other common files
	if(status_list)
		status_list("Loading common PNM animations...");
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
			char temp_name[13];
			strcpy_noext(temp_name,name);
			if(status_item)
				status_item(temp_name);
			AnimPNM* pnm = new AnimPNM();
			if(pnm->Decode(data,temp_name))
			{
				delete pnm;
				this->~SpellData();
				if(status_list)
					status_list(string_format(" - failed decoding ''%s''!",name));
				throw runtime_error(string_format("Failed decoding PNM animation ''%s''!",name));
			}
			pnms.push_back(pnm);			
		}
	}
	// hard assign some PNM links
	pnm_sipka = GetPNM("SIPKA");	

	// load JEDNOTKY.DEF units definition file
	if(status_list)
		status_list("Loading units definitions (JEDNOTKY.DEF)...");
	if(common->GetFile("JEDNOTKY.DEF", &data, &size))
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
	if(common->GetFile("FONT_001.FNT",&data,&size))
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
	delete common;
	common = NULL;


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
	if(LoadSpecialLand(spec_path))
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
	// delete pnm animations
	for(auto & pnm : pnms)
		delete pnm;
	pnms.clear();
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
	if(texts)
		delete texts;
	texts = NULL;
	if(L2_classes)
		delete L2_classes;
	L2_classes = NULL;
	if(unit_bonuses)
		delete unit_bonuses;
	unit_bonuses = NULL;
	if(common)
		delete common;
	common = NULL;
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
int SpellData::LoadAuxGraphics(FSarchive *fs,std::function<void(std::string)> status_item)
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
		int no_item = false;
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
		else if(strcmp(name,"MAP_OPT.LZ") == 0)
		{
			// window frame
			is_lzw = true;
			lzw->Decode(data,data_end,&data,&flen);
			gres.AddRaw(data,flen,436,flen/436,name,(uint8_t*)terrain[0]->pal);
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
		else
			no_item = true;

		if(!no_item && status_item)
			status_item(name);

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
	gres.wm_map_opt_frame = gres.GetResource("MAP_OPT");
	gres.wm_map_opt_btn_disabled = gres.GetResource("WMOPT__D");
	gres.wm_map_opt_btn_idle = gres.GetResource("WMOPT__N");
	gres.wm_map_opt_btn_hover = gres.GetResource("WMOPT__A");
	gres.wm_map_opt_btn_down = gres.GetResource("WMOPT__P");

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
