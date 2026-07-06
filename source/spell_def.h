//=============================================================================
// Spellcross DEF file parsing stuff
// 
// This code is part of Spellcross Map Editor project.
// (c) 2026, Stanislav Maslan, s.maslan@seznam.cz
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
		std::vector<std::string> parameters;
		std::string comment;
		std::string full_command;

		std::string sub_name;
		std::vector<std::string> sub_params;
		std::string sub_full_command;

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
		SpellDEF(string &str);
		SpellDEF(wstring &path);
		SpellDEF(uint8_t *data, int size);
		~SpellDEF();

		SpellDefSection *GetSection(std::string section);
};

