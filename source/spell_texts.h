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

typedef vector<SpellTextChunk> SpellTextChunks;

class SpellTextLine
{
public:
    SpellTextChunks chunks;
    int pos_y;
    int size_x;
    int size_y;
    bool no_stretch;
};

class SpellTextLines
{
public:
    enum class WrapMode { LEFT,CENTER,STRETCH };

    std::vector<SpellTextLine> lines;
    int limit_x;
    int line_step;
    WrapMode mode;
};


class SpellTextRec
{
public:
    enum class TextPanel { DEFAULT, MESSAGE,UNIT_INFO,BM_RESEARCH_PANEL,BM_RESEARCH_INFO,BM_MISSION };

    bool modified;
    std::string name;
    std::string raw_text;
    std::wstring text;
    SpellLang lang;    
    SpellSample *audio;
    bool is_placeholder;
    // target panel type
    TextPanel text_panel;
    //int x_size;
    //int y_size;
        
    SpellTextRec();
    SpellTextRec(std::string str,SpellLang lang,const char* name=NULL,SpellTextRec::TextPanel panel=SpellTextRec::TextPanel::DEFAULT,SpellSample *audio=NULL,bool is_placeholder=false);
    SpellTextLines WordWrap(SpellFont *font, int x_limit,int y_gap=1,SpellTextLines::WrapMode mode=SpellTextLines::WrapMode::LEFT);
    int UpdateText(std::wstring text,int x_width,SpellFont* font,SpellTextRec::TextPanel panel=SpellTextRec::TextPanel::DEFAULT);
    bool isEmpty();
};


class SpellTexts
{
private:

    SpellLang lang;
    vector<SpellTextRec*> list;

public:
    
    SpellTexts(FSarchive *fs,SpellLang lang,SpellTextRec::TextPanel panel=SpellTextRec::TextPanel::DEFAULT,SpellSounds *sounds=NULL);
    ~SpellTexts();
    SpellTextRec* AddText(std::string name,std::string raw_text="",SpellLang lang=SpellLang::CZE,SpellTextRec::TextPanel panel=SpellTextRec::TextPanel::DEFAULT,SpellSample * narration=NULL,bool is_placeholder=false);
    int RemovePlaceholders();
    SpellTextRec* GetText(int index);
    SpellTextRec* GetText(const char* name);
    SpellTextRec *GetText(string &name);
    vector<SpellTextRec*> &GetTexts();
    int GetTextId(SpellTextRec* text);
};
