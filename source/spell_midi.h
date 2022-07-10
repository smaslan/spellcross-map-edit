//=============================================================================
// Sound related Spellcross stuff (loaders)
// 
// This code is part of Spellcross Map Editor project.
// (c) 2022, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#pragma once

#include "fs_archive.h"
#include "cxxmidi/file.hpp"
#include "cxxmidi/output/default.hpp"
#include "cxxmidi/player/player_async.hpp"

#include "cstdint"
#include <vector>
#include <string>
#include <functional>


class SpellMIDIfile
{
private:
    std::vector<uint8_t> m_raw;    
public:
    SpellMIDIfile(std::vector<uint8_t> &data, std::string name);
    std::vector<uint8_t> &GetData();    
    std::string filename; /* no ext */
    int SaveAs(std::wstring path);
};

class SpellMIDI
{
private:
    // engine
    cxxmidi::output::Default* m_port;
    cxxmidi::player::PlayerAsync* m_player;
    double m_volume;

    // current file
    cxxmidi::File *m_file;
    std::string m_file_name;

    // all midi files
    std::vector<SpellMIDIfile*> m_midi;
    

public:

    SpellMIDI(std::wstring &data_path, std::function<void(std::string)> status_list=NULL, std::function<void(std::string)> status_item=NULL);
    ~SpellMIDI();
    
    int Play(SpellMIDIfile *midi);
    int Play(std::string name);
    int Play(std::wstring path);
    void Stop();
    void SetVolume(double volume=1.0);
    double GetVolume();
    std::vector<SpellMIDIfile*> &GetMIDI();
    SpellMIDIfile *GetCurrent();
    std::string GetCurrentName();
    int SaveCurrent(std::wstring path);
    
    std::string GetInfo(cxxmidi::File *file);
    std::string GetInfo();
    std::string GetInfo(std::string name);
    std::string GetInfo(std::wstring path);
};

