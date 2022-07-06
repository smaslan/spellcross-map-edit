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
    double volume;
public:
    SoundChannels(int count=4);
    ~SoundChannels();
    RtAudio *GetChannel();
    void SetVolume(double vol);
    double GetVolume();
    
    static constexpr double FAKE_STOP_TIME_MARK = 1e9;
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
    int is_playing;
    int auto_delete;
    int force_loop;
    int32_t left_vol;
    int32_t right_vol;

    void SpellSoundInit();

    SoundChannels* streams;

    std::function<void(void)> user_frame_callback;

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

    int SetPanning(double left_vol=1.0,double right_vol=1.0);
    int Play(bool auto_delete=false, bool loop=false, std::function<void(void)> frame_callback=NULL, double frame_step=0.02);
    double GetPlaybackTime();
    int StopMove();
    int Stop(double wait=0.0);
    int isDone();
    void cb_DoneFlag();
    int cb_GetFrame(int16_t* buffer,int count);
    int cb_GetSmplLeft();
    int cb_GetSmplSize();
    int16_t* cb_GetSmplData(int16_t* buffer, int count);
    int isAutoDelete();
    SpellSample *GetSample(int id=0);
    void cb_user_callback();
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
    int isDone();
};


class SpellSounds
{
private:
    // plain samples
    std::vector<SpellSample> samples;
    // sample classes
    std::vector<SpellSoundClassFile> attack_sounds;
    std::vector<SpellSoundClassFile> move_sounds;
    std::vector<SpellSoundClassFile> report_sounds;
    std::vector<SpellSoundClassFile> object_sounds;
    std::vector<SpellSoundClassFile> spec_sounds;
    std::vector<SpellSoundClassFile> hit_sounds;
    std::vector<SpellSound*> aux_sounds;
    
    vector<SpellSoundClassFile> ParseSoundClasses(std::string text);
    void ParseSoundClasses2(FSarchive* fs,const char* name,vector<SpellSoundClassFile>* cls);

public:
    
    SoundChannels* channels;
    
    SpellSounds(FSarchive* common_fs,wstring &fs_data_path, int count=16,std::function<void(std::string)> status_list=NULL,std::function<void(std::string)> status_item=NULL);
    ~SpellSounds();
    SpellSample *GetSample(const char *name);

    SpellSound* GetSound(const char* name);
    SpellSound* GetMoveClass(int index);
    SpellSound* GetReportClass(int index);
    SpellSound* GetContactClass(int index);
    SpellAttackSound* GetAttackClass(int index);
    SpellSound* GetHitClass(int index);
    SpellSound* GetDieClass(int index);
    SpellSound* GetSpecialClass(int index);
    SpellSound* GetObjectHitClass(int index);
    SpellSound* GetObjectDestructClass(int index);

    class AuxSamples{
    public:
        SpellSound* turret_rotate;
        SpellSound* btn_end_turn;
        SpellSound* oponent_fire_aliance;
        SpellSound* oponent_fire_os;
        SpellSound* unit_level_up;
    };
    AuxSamples aux_samples;
        
};

