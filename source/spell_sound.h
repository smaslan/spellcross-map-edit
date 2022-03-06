//=============================================================================
// Sound related Spellcross stuff (loaders)
// 
// This code is part of Spellcross Map Editor project.
// (c) 2022, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#pragma once

#include "fs_archive.h"

#include "RtAudio.h"
#include "cstdint"
#include <vector>
#include <list>
#include <string>

using namespace std;

class SpellSample
{
public:
    vector<int16_t> data;
    int channels;
    int samples;
    int fs;
    char name[13];

    SpellSample() {};
};

class SpellSoundClassFile
{
public:
    int index;
    string name;
    vector<string> items_names;
    vector<SpellSample*> items;

    SpellSoundClassFile();
};


class SoundChannels
{    
private:
    vector<RtAudio*> channels;
    vector<int> chn_index;
    int last_channel;
public:
    SoundChannels(int count=4);
    ~SoundChannels();
    RtAudio *GetChannel();
};

class SpellSound
{
private:
    RtAudio* dac;
    vector<SpellSample*> samples;
    int sub_id;
    int control;
    int mode;
    int channels;
    int count;
    int fs;
    int pos;

    void SpellSoundInit();

    SoundChannels* streams;    

public:

    static constexpr int NEXT_FRAME_CONTINUE = 0;
    static constexpr int NEXT_FRAME_STOP = 1;
    static constexpr int NEXT_FRAME_SKIP = 2;
    static constexpr int NEXT_FRAME_FADE = 3;

    static constexpr int MODE_RAND = 0;
    static constexpr int MODE_MOVE = 1;

    static constexpr int CTRL_IDLE = 0;
    static constexpr int CTRL_END_MOVE = 1;
    static constexpr int CTRL_STOP = 2;

    SpellSound(SpellSound &ref);
    SpellSound(SoundChannels* channels,SpellSoundClassFile& move_class);
    SpellSound(SoundChannels* channels,vector<SpellSample*> sounds);
    SpellSound(SoundChannels* channels,SpellSample* sound);
    ~SpellSound();

    int Play();
    int StopMove();
    int isDone();
    int cb_GetFrame(int16_t* buffer,int count);
    int cb_GetSmplLeft();
    int cb_GetSmplSize();
    int16_t* cb_GetSmplData(int count);
    //int cb_GotoNextSubsample();
};


class SpellAttackSound
{
public:    
    SpellSound *shot;
    SpellSound *hit_flash;
    SpellSound *hit_armor;
    SpellSound *hit_miss;

    SpellAttackSound(SoundChannels* channels,SpellSoundClassFile* sound_class);
    SpellAttackSound(SpellAttackSound& ref);
    ~SpellAttackSound();
};



class SpellSounds
{
private:
    // plain samples
    vector<SpellSample> samples;
    // sample classes
    vector<SpellSoundClassFile> attack_sounds;
    vector<SpellSoundClassFile> move_sounds;
    vector<SpellSoundClassFile> report_sounds;
    vector<SpellSoundClassFile> object_sounds;
    vector<SpellSoundClassFile> spec_sounds;
    vector<SpellSoundClassFile> hit_sounds;    

    vector<SpellSoundClassFile> ParseSoundClasses(string text);
    void ParseSoundClasses2(FSarchive* fs,const char* name,vector<SpellSoundClassFile>* cls);

    SoundChannels *channels;

public:
    SpellSounds(wstring &fs_data_path, int count=4);
    ~SpellSounds();
    SpellSample *GetSample(const char *name);

    SpellSound* GetMoveClass(int index);
    SpellSound* GetReportClass(int index);
    SpellSound* GetContactClass(int index);
    SpellAttackSound* GetAttackClass(int index);
        
};

