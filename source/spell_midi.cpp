//=============================================================================
// Sound related Spellcross stuff (samples and MIDI)
// 
// This code is part of Spellcross Map Editor project.
// (c) 2022, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#undef _HAS_STD_BYTE
#define _HAS_STD_BYTE 0

//#undef __RTAUDIO_DUMMY__
//#define __WINDOWS_DS__

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


