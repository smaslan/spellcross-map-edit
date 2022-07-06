#undef _HAS_STD_BYTE
#define _HAS_STD_BYTE 0

#include "spell_texts.h"
#include "other.h"
#include <stdexcept>
#include <string>

using namespace std;

// basic text loader
SpellTextRec::SpellTextRec(const char* str, int str_len, SpellLang lang, const char* name,SpellSample* audio)
{
    this->lang = lang;
    raw_text = str;
    strcpy_s(this->name, sizeof(this->name), name);
    this->audio = audio;
        
    // parse the string (remove control stuff and word wrapping)
    char* buf = new char[str_len + 1];
    memset(buf,'\0',str_len + 1);
    char last_sym = '\0';
    char *data = buf;
    char *pstr = (char*)str;
    while(pstr < &str[str_len])
    {
        if(*pstr == '~')
        {
            // possibly align mark - skip
            pstr++;
            continue;
        }
        if(*pstr == 0x1A)
        {
            // possibly some parameterized input mark?? - skip
            pstr++;
            continue;
        }        
        if(pstr[0] == '\r' && &pstr[1] < &str[str_len] && pstr[1] == '\n')
        {
            // line break
            if(last_sym == '.' || last_sym == '\n') // discard word wrapping
                *data++ = '\n';
            else if(data > buf && data[-1] != ' ')
                *data++ = ' ';                
            last_sym = '\n';
            pstr += 2;
            continue;
        }
        if(*pstr != ' ')
            last_sym = *pstr;
        *data++ = *pstr++;
    }
        
    // convert to unicode
    if(lang == SpellLang::ENG)
    {        
        // ENG: just copy, there should be no stuff above 127
        text = char2wstring(buf);
    }
    else if(lang == SpellLang::CZE)
    {
        // CZE: convert from weird CP895 to unicode
        text = char2wstringCP895(buf);
    }
    else
    {
        delete[] buf;
        throw runtime_error("Unknown language requested in text decoder!");
    }
    delete[] buf;

}


SpellTextChunks SpellTextRec::WordWrap(SpellFont* font,int x_limit)
{
    wchar_t *pstr = (wchar_t*)text.c_str();

    int y_step = font->GetHeight() + 1;

    SpellTextChunks lines;

    wstring chunk;
    int last_len = 0;
    int line_pix = 0;
    wchar_t *w_str = pstr;
    wchar_t p_sym = ' ';
    int pos_x = 0;
    int pos_y = 0;
    while(*pstr)
    {
        if(*pstr == '\n')
        {
            lines.emplace_back(0, pos_y, pos_x, chunk);
            chunk = L"";
            pos_x = 0;
            pos_y += y_step;            
        }
        else
        {
            if(*pstr != ' ' && (p_sym == ' ' || p_sym == '\n'))
            {
                // store word start
                last_len = chunk.size();
                w_str = pstr;
                line_pix = pos_x;
            }                
            pos_x += font->GetSymbolWidth(*pstr);
            if(pos_x >= x_limit)
            {
                // word won't fit, step back                
                chunk.resize(last_len);
                lines.emplace_back(0, pos_y, line_pix, chunk);
                chunk = L"";
                pos_x = 0;
                pos_y += y_step;
                pstr = w_str;                
                p_sym == ' ';                
            }
            chunk.insert(chunk.end(),*pstr);
        }
        p_sym = *pstr++;
    }
    if(!chunk.empty())
        lines.emplace_back(0,pos_y,pos_x,chunk);

    return(lines);
}


// load all text files from TEXTS.FS archive
SpellTexts::SpellTexts(FSarchive* fs, SpellLang lang,SpellSounds* sounds)
{
    this->lang = lang;

    // load all files (assuming they are all texts)
    for(int fid = 0; fid < fs->Count(); fid++)
    {
        // fetch file
        uint8_t *data;
        int len;
        const char *name;
        fs->GetFile(fid, &data, &len, &name);
        
        // try fetch message narration
        SpellSample *narration = NULL;
        if(sounds && name[0] == 'U')
            narration = sounds->GetSample(name);

        // try load
        auto txt = new SpellTextRec((char*)data,len,lang,name,narration);
        list.push_back(txt);
    }
}

// cleanup string table
SpellTexts::~SpellTexts()
{
    for(auto & txt : list)
        if(txt)
            delete txt;
    list.clear();
}

// try to get text by its name
SpellTextRec* SpellTexts::GetText(string &name)
{
    return(GetText(name.c_str()));
}
SpellTextRec* SpellTexts::GetText(const char *name)
{
    for(auto & txt : list)
        if(_strcmpi(txt->name, name) == 0)
            return(txt);
    return(NULL);
}
// try get text by order index
SpellTextRec* SpellTexts::GetText(int index)
{
    if(index < 0 || index >= list.size())
        return(NULL);
    return(list[index]);
}

// get index of text resource in the list by record pointer
int SpellTexts::GetTextId(SpellTextRec* text)
{
    auto id = find(list.begin(), list.end(), text);
    if(id == list.end())
        return(-1);
    return(id - list.begin());
}

// get full list of texts
vector<SpellTextRec*>& SpellTexts::GetTexts()
{
    return(list);
}
