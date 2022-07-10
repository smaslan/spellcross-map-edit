//=============================================================================
// Sound related Spellcross stuff (samples and MIDI)
// 
// This code is part of Spellcross Map Editor project.
// (c) 2022, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
//#undef _HAS_STD_BYTE
//#define _HAS_STD_BYTE 0

#include "spell_midi.h"
#include "other.h"

#include <string>
#include <vector>
#include <stdexcept>


//=============================================================================
// MIDI related stuff.
//=============================================================================

// Spellcross MIDI file
SpellMIDIfile::SpellMIDIfile(std::vector<uint8_t>& data,std::string name)
{
    m_raw = data;
    filename = std::filesystem::path(name).replace_extension().string();
}
// return raw midi file data
std::vector<uint8_t>& SpellMIDIfile::GetData()
{
    return(m_raw);
}
// save loaded file data as
int SpellMIDIfile::SaveAs(std::wstring path)
{
    if(m_raw.empty())
        return(1);
    ofstream fw(path,ios::out | ios::binary);
    if(!fw)
        return(1);
    fw.write((char*)m_raw.data(),m_raw.size());
    fw.close();
    return(0);
}



// init MIDI engine
SpellMIDI::SpellMIDI(std::wstring& data_path, std::function<void(std::string)> status_list, std::function<void(std::string)> status_item)
{
    // try to get first available midi port and open MIDI player
    if(status_list)
        status_list(" - Initiating MIDI playback engine...");
    m_file = NULL;
    m_port = new cxxmidi::output::Default(0);
    m_player = new cxxmidi::player::PlayerAsync(m_port);
    m_volume = 0.5;

    // load music.fs (MIDI)
    if(status_list)
        status_list(" - Loading MUSIC.FS...");
    FSarchive* music_fs;
    wstring music_fs_path = std::filesystem::path(data_path) / std::filesystem::path("music.fs");
    try {
        music_fs = new FSarchive(music_fs_path,FSarchive::Options::DELZ_ALL);
    }catch(const runtime_error& error) {
        if(status_list)
            status_list("   - failed!");
        throw runtime_error(string_format("Loading MUSIC.FS archive failed (%s)!",error.what()));
    }
    for(auto& file : music_fs->GetFiles())
    {
        if(status_item)
            status_item(file->name);
        try {
            SpellMIDIfile* midi_file = new SpellMIDIfile(file->data,file->name);
            m_midi.push_back(midi_file);
        }catch(const runtime_error& error) {
            delete music_fs;
            if(status_list)
                status_list("   - failed!");
            throw runtime_error(string_format("Loading \"%s\" midi file failed!",file->name.c_str()));
        }
    }
    delete music_fs;
}

// cleanup MIDI engine
SpellMIDI::~SpellMIDI()
{
    // stop playback
    Stop();
    // loose engine stuff
    if(m_player)
        delete m_player;
    m_player = NULL;
    if(m_port)
        delete m_port;
    m_port = NULL;

    // loose midi files
    for(auto & mid : m_midi)
        delete mid;
    m_midi.clear();
}

// stop playback and cleanup
void SpellMIDI::Stop()
{
    if(!m_player || !m_port)
        return;
    m_player->Pause();
    m_player->SetFile(NULL);
    if(m_file)
        delete m_file;
    m_file = NULL;
}

// load and start playback of MIDI file
int SpellMIDI::Play(SpellMIDIfile* midi)
{
    // stop old playback
    Stop();

    // load new MIDI file
    try {
        m_file = new cxxmidi::File(midi->GetData());
    }
    catch(...) {
        return(1);
    }

    // store current file name
    m_file_name = midi->filename;

    // assign to engine
    m_player->SetFile(m_file);
    m_player->SetVolume(m_volume);

    // start playback
    m_player->Play();

    return(0);
}

// load and start playback of MIDI file
int SpellMIDI::Play(std::string name)
{
    for(auto & mid : m_midi)
        if(mid->filename.compare(name) == 0)
            return(Play(mid));
    return(1);
}

// play custom MIDI file
int SpellMIDI::Play(std::wstring path)
{
    // read file local buffer and close
    ifstream fr(path,ios::in | ios::binary | ios::ate);
    if(!fr)
        return(1);
    streampos flen = fr.tellg();
    fr.seekg(0);
    std::vector<uint8_t> data(flen);
    fr.read((char*)data.data(),flen);
    fr.close();

    // play it
    SpellMIDIfile file(data, std::filesystem::path(path).filename().string());
    Play(&file);

    return(0);
}


// set volume <0;1>
void SpellMIDI::SetVolume(double volume)
{
    if(!m_player)
        return;
    m_volume = volume;
    m_player->SetVolume(volume);
}
// get volume <0;1>
double SpellMIDI::GetVolume()
{
    return(m_volume);
}

// get all loaded midi files
std::vector<SpellMIDIfile*>& SpellMIDI::GetMIDI()
{
    return(m_midi);
}

// get currently selected/played MIDI file
SpellMIDIfile* SpellMIDI::GetCurrent()
{
    for(auto & mid : m_midi)
        if(mid->filename.compare(m_file_name) == 0)
            return(mid);
    return(NULL);
}

// get currently selected/played MIDI file
std::string SpellMIDI::GetCurrentName()
{
    return(m_file_name);
}

// export currently loaded midi file
int SpellMIDI::SaveCurrent(std::wstring path)
{
    if(!m_file)
        return(1);

    return(m_file->SaveAs(path));
}

// get file info string
std::string SpellMIDI::GetInfo(cxxmidi::File* mid)
{
    if(!mid)
        return("");

    std::string info;
    if(mid->Tracks() > 1)
    {
        // loop over tracks 
        int tid = 1;
        for(const auto& track : *mid)
        {
            info += string_format("Track %d: %s\n", tid, track.GetText().c_str());
            tid++;
        }
    }
    else
        for(const auto& track : *mid)
            info = track.GetText();
    
    return(info);
}
std::string SpellMIDI::GetInfo()
{
    if(!m_file)
        return("");
    return(GetInfo(m_file));
}
std::string SpellMIDI::GetInfo(std::string name)
{
    for(auto& mid : m_midi)
        if(mid->filename.compare(name) == 0)
        {
            cxxmidi::File file(mid->GetData());
            return(GetInfo(&file));
        }
    return("");
}
std::string SpellMIDI::GetInfo(std::wstring path)
{
    cxxmidi::File mid(path);
    return(GetInfo(&mid));
}
