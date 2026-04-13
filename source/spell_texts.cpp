#undef _HAS_STD_BYTE
#define _HAS_STD_BYTE 0

#include "spell_texts.h"
#include "other.h"
#include <stdexcept>
#include <string>

//using namespace std;

SpellTextRec::SpellTextRec()
{
    name = "";
    lang = SpellLang::CZE;
    audio = NULL;
    raw_text = "";
    text = L"";
    is_placeholder = false;
    text_panel = TextPanel::DEFAULT;
    modified = false;
}

// is empty record
bool SpellTextRec::isEmpty()
{
    return(text.empty() && raw_text.empty());
}


// basic text loader
SpellTextRec::SpellTextRec(std::string str, SpellLang lang, const char* name,SpellTextRec::TextPanel panel,SpellSample* audio,bool is_placeholder)
{    
    this->is_placeholder = is_placeholder;
    if(name)
        this->name = name;
    this->lang = lang;    
    this->audio = audio;
    this->raw_text = str;
    modified = false;
    
    // normalize linebreaks
    str = strrep(str, "\r\n", "\n");
    str = strrep(str, "\r", "\n");

    // eat word splits at end of lines
    str = strrep(str,"-\n","");
    
    // parse the string (remove control stuff and word wrapping)
    std::string buf(str.size()+1,'\0');
    char last_sym = '\0';
    char *data = buf.data();
    char *pstr = str.data();
    char *strend = &pstr[str.size()];
    while(pstr < strend)
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
        if(pstr[0] == '\n')
        {
            // line break
            if(last_sym == '.' || last_sym == ':' || last_sym == '\n') // discard word wrapping
                *data++ = '\n';
            else if(data > buf.data() && data[-1] != ' ')
                *data++ = ' ';                
            last_sym = '\n';
            pstr += 1;
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
        text = char2wstring(buf.c_str());
    }
    else if(lang == SpellLang::CZE)
    {
        // CZE: convert from weird CP895 to unicode
        text = char2wstringCP895(buf.c_str());
    }
    else
    {
        throw runtime_error("Unknown language requested in text decoder!");
    }

    // try to identify target panel
    text_panel = panel;
    if(panel != TextPanel::DEFAULT)
        text_panel = panel;
    else if(wildcmp("*.INF",name))
        text_panel = TextPanel::BM_RESEARCH_INFO;
    else if(wildcmp("*.BRF",name))
        text_panel = TextPanel::BM_RESEARCH_PANEL;
    else if(wildcmp("T*.S",name) || wildcmp("T*.OK",name) || wildcmp("T*.BAD",name) || wildcmp("E*",name) || wildcmp("U*",name) || wildcmp("MSG*",name))
        text_panel = TextPanel::MESSAGE;
    else if(wildcmp("T*",name))
        text_panel = TextPanel::BM_MISSION;
    if(text_panel == TextPanel::DEFAULT)
        text_panel = TextPanel::MESSAGE;
    
    // default panel sizes
    /*switch(text_panel)
    {   
    case TextPanel::UNIT_INFO:
        x_size = 370;
        y_size = 350;
        break;
    case TextPanel::BM_MISSION:
        x_size = 352;
        y_size = 126;
        break;
    case TextPanel::BM_RESEARCH_PANEL:
        x_size = 352;
        y_size = 123;
        break;
    case TextPanel::BM_RESEARCH_INFO:
        x_size = 376;
        y_size = 416;
        break;
    case TextPanel::MESSAGE:
    default:
        x_size = 375;
        y_size = -1;
        break;
    }*/
}

// update text using word wrapper and given window layout
int SpellTextRec::UpdateText(std::wstring text, int x_width, SpellFont* font,SpellTextRec::TextPanel panel)
{
    // update unicode source text
    this->text = text;
    
    // format text
    auto lines = WordWrap(font, x_width);

    // build full text 
    std::wstring str = L"";
    for(auto &line: lines.lines)
    {        
        for(auto &word: line.chunks)
        {
            str += word.text;
            if(&word != &line.chunks.back())
                str += L" ";
        }
        if(&line != &lines.lines.back())
            str += L"\r\n";
    }
    

    // convert code page
    if(lang == SpellLang::CZE)
        this->raw_text = wstring2stringCP895(str);
    else if(lang == SpellLang::ENG)
        this->raw_text = wstring2string(str);
    else
        return(1);

    // mark as modified
    this->modified = true;

    return(0);
 }

// splits text by lines to fit maximum width
SpellTextLines SpellTextRec::WordWrap(SpellFont* font,int x_limit, int y_gap, SpellTextLines::WrapMode mode)
{
    std::wstring text = this->text;
    text.erase(text.find_last_not_of(L"\n\r") + 1);
    text += L"\n";
    wchar_t *pstr = (wchar_t*)text.c_str();     

    int y_step = font->GetHeight() + y_gap;

    SpellTextLines wrap;
    wrap.limit_x = x_limit;
    wrap.line_step = y_step;
    wrap.mode = mode;
    
    int w_space = font->GetSymbolWidth(L' ');

    SpellTextLine line;

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
            int len = chunk.length();
            chunk.erase(chunk.find_last_not_of(L" ") + 1);
            pos_x -= w_space*(len - chunk.length());

            if(chunk.size())
            {
                auto word = chunk.substr(last_len);
                int len = word.length();
                word.erase(word.find_last_not_of(L" ") + 1);
                int ofs_x = pos_x - w_space*(len - word.length());
                line.chunks.emplace_back(line_pix,pos_y,ofs_x - line_pix,y_step,x_limit,word);
            }
            
            bool is_word_break = chunk.length() >= 2 && chunk[chunk.size()-1] == L'-' && chunk[chunk.size()-2] != L' ' && chunk[chunk.size()-2] != L'\n';            
            line.pos_y = pos_y;
            line.size_x = pos_x;
            line.size_y = y_step;
            line.no_stretch = !is_word_break;
            wrap.lines.push_back(line);

            chunk = L"";
            pos_x = 0;
            pos_y += y_step;
            last_len = 0;
            line_pix = 0;
            w_str = pstr + 1;

            

            line.chunks.clear();
        }
        else
        {
            if(*pstr != ' ' && (p_sym == ' ' || p_sym == '\n'))
            {
                // store word start
                
                if(chunk.size())
                {                    
                    auto word = chunk.substr(last_len);
                    int len = word.length();
                    word.erase(word.find_last_not_of(L" ") + 1);
                    int ofs_x = pos_x - w_space*(len - word.length());
                    line.chunks.emplace_back(line_pix,pos_y,ofs_x - line_pix,y_step,x_limit,word);
                }                
                last_len = chunk.size();
                line_pix = pos_x;
                w_str = pstr;
            }                
            int sym_x = font->GetSymbolWidth(*pstr);
            pos_x += sym_x;
            if(pos_x >= x_limit)
            {
                // word won't fit, step back                
                if(!last_len)
                {
                    // not even one word fits - split word
                    if(pos_x - sym_x <= 0)
                        break; // not even one symbol fits - leave

                    auto word = chunk;
                    int len = word.length();
                    word.erase(word.find_last_not_of(L" ") + 1);
                    int ofs_x = pos_x - w_space*(len - word.length());
                    line.chunks.emplace_back(line_pix,pos_y,ofs_x - line_pix,y_step,x_limit,word);

                    line_pix = pos_x - sym_x;
                    w_str = pstr;
                }
                else
                    chunk.resize(last_len);                            
                
                int len = chunk.length();
                chunk.erase(chunk.find_last_not_of(L" ") + 1);
                line_pix -= w_space*(len - chunk.length());

                line.pos_y = pos_y;
                line.size_x = line_pix;
                line.size_y = y_step;
                line.no_stretch = false;
                wrap.lines.push_back(line);

                chunk = L"";
                pos_x = 0;
                pos_y += y_step;
                pstr = w_str;
                p_sym = ' ';
                last_len = 0;
                line_pix = 0;

                line.chunks.clear();
                continue;
            }
            chunk.insert(chunk.end(),*pstr);
        }
        p_sym = *pstr++;
    }

    for(auto &line: wrap.lines)
    {
        if(mode == SpellTextLines::WrapMode::CENTER)
        {
            // center line
            int ofs_x = (wrap.limit_x - line.size_x)/2;
            for(auto &word: line.chunks)
                word.pos_x += ofs_x;
        }
        else if(mode == SpellTextLines::WrapMode::STRETCH && !line.no_stretch)
        {
            // stretch line
            if(line.chunks.size() < 2)
                continue;
            double step_x = (double)(wrap.limit_x - line.size_x)/(line.chunks.size() - 1);
            double gap_x = 0.0;
            int ofs_x = 0;
            for(int k = 1; k < line.chunks.size(); k++)
            {
                auto word = &line.chunks[k];
                gap_x += step_x;
                int pix_x = (int)round(gap_x);
                ofs_x += pix_x;
                gap_x -= pix_x;
                word->pos_x += ofs_x;
            }
        }
    }


    return(wrap);
}


// load all text files from TEXTS.FS archive
SpellTexts::SpellTexts(FSarchive* fs, SpellLang lang,SpellTextRec::TextPanel panel,SpellSounds* sounds)
{
    this->lang = lang;

    // load all files (assuming they are all texts)
    for(auto & src : fs->GetFiles())    
    {        
        // try fetch message narration
        SpellSample *narration = NULL;
        if(sounds && src->name[0] == 'U')
            narration = sounds->GetSample(src->name.c_str());

        // try load
        std::string str(src->data.begin(), src->data.end());
        AddText(src->name,str,lang,panel,narration);
    }
}

// add text resource to the list
SpellTextRec* SpellTexts::AddText(std::string name,std::string raw_text,SpellLang lang,SpellTextRec::TextPanel panel,SpellSample* narration,bool is_placeholder)
{
    if(name.empty())
        return(NULL);

    // check duplicates
    for(auto item: list)
        if(iequals(item->name,name))
            return(NULL);

    // make text
    auto txt = new SpellTextRec(raw_text,lang,name.c_str(),panel,narration,is_placeholder);
    if(txt)
        list.push_back(txt);

    return(txt);
}

// remove placeholder resources
int SpellTexts::RemovePlaceholders()
{
    int count = 0;
    vector<SpellTextRec*> temp_list;
    for(auto &txt: list)
    {
        if(txt->is_placeholder)
        {
            delete txt;
            count++;
        }
        else
            temp_list.push_back(txt);
    }
    list = temp_list;
    return(count);
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
        if(iequals(txt->name,name))
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
