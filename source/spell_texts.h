#pragma once

#include "fs_archive.h"
#include "spell_sound.h"
#include "spell_font.h"
#include <string>

enum class SpellLang {ENG, CZE};

class SpellTextChunk
{
public:
    wstring text;
    int pos_x;
    int pos_y;
    int size_x;

    SpellTextChunk(int xx, int yy, int xs, wstring str) {text = str; pos_x = xx; pos_y = yy; size_x = xs;};
};

typedef vector<SpellTextChunk> SpellTextChunks;

class SpellTextRec
{
public:
    string raw_text;
    wstring text;
    SpellLang lang;
    char name[8+4+1];
    SpellSample *audio;

    SpellTextRec(const char *str,int str_len,SpellLang lang,const char* name=NULL,SpellSample *audio=NULL);
    SpellTextChunks WordWrap(SpellFont *font, int x_limit);
};


class SpellTexts
{
private:

    SpellLang lang;
    vector<SpellTextRec*> list;

public:
    
    SpellTexts(FSarchive *fs,SpellLang lang,SpellSounds *sounds=NULL);
    ~SpellTexts();
    SpellTextRec* GetText(const char* name);
    SpellTextRec *GetText(string &name);
};
