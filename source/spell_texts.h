#pragma once

#include "fs_archive.h"
#include "spell_sound.h"
#include "spell_font.h"
#include <string>

enum class SpellLang {ENG, CZE};

class SpellTextChunk
{
public:
    std::wstring text;
    int pos_x;
    int pos_y;
    int size_x;
    int size_y;
    int limit_x;

    SpellTextChunk(int xx, int yy, int xs, int ys, int x_limit, wstring str) {text = str; pos_x = xx; pos_y = yy; size_x = xs; size_y=ys; limit_x = x_limit;};
};

/*class SpellTextChunks
{
public:

}*/
typedef vector<SpellTextChunk> SpellTextChunks;

class SpellTextRec
{
public:
    std::string name;
    std::string raw_text;
    std::wstring text;
    SpellLang lang;    
    SpellSample *audio;
    bool is_placeholder;
    
    SpellTextRec(std::string str,SpellLang lang,const char* name=NULL,SpellSample *audio=NULL,bool is_placeholder=false);
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
    SpellTextRec* AddText(std::string name,std::string raw_text="",SpellLang lang=SpellLang::CZE,SpellSample * narration=NULL,bool is_placeholder=false);
    int RemovePlaceholders();
    SpellTextRec* GetText(int index);
    SpellTextRec* GetText(const char* name);
    SpellTextRec *GetText(string &name);
    vector<SpellTextRec*> &GetTexts();
    int GetTextId(SpellTextRec* text);
};
