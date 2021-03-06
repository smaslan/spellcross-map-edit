//=============================================================================
// Sound related Spellcross stuff (samples and MIDI)
// 
// This code is part of Spellcross Map Editor project.
// (c) 2022, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#undef _HAS_STD_BYTE
#define _HAS_STD_BYTE 0

#include "spell_sound.h"
#include "fs_archive.h"
#include "LZ_spell.h"
#include "map.h"
#include "other.h"
#include "riff_loader.h"
#include "RtAudio.h"

#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <regex>
#include <filesystem>
#include <sstream>
#include <future>
#include <chrono>
#include <thread>

using namespace std;


//=============================================================================
// Sound channels stuff.
// This is pool of RtAudio channels used to polyphonic playback of samples.
//=============================================================================

// initialize stream channels
SoundChannels::SoundChannels(int count)
{
    chn_index.assign(count,0);
    channels.assign(count,NULL);
    is_loop.assign(count,0);
    for(auto & chn : channels)
        chn = new RtAudio();    
    last_channel = 0;
    volume = 0.5;
}
// close stream channels
SoundChannels::~SoundChannels()
{
    // loose all stream channels
    for(auto& chn : channels)
    {
        // signalize stop to eventually running callback
        chn->setStreamTime(FAKE_STOP_TIME_MARK*2.0);
        // wait to actually stop
        while(chn->isStreamRunning())
            this_thread::sleep_for(10ms);
        chn->closeStream();
        // delete it
        delete chn;
    }
    channels.clear();
}
// get first available channel
RtAudio* SoundChannels::GetChannel(int is_loop)
{    
    lock.lock();
    // look for first free stream
    for(int cid = 0; cid < channels.size(); cid++)
    {
        auto *chn = channels[cid];        
        if(!chn->isStreamRunning())
        {
            if(chn->isStreamOpen())
                chn->closeStream();
            chn_index[cid] = ++last_channel;
            this->is_loop[cid] = is_loop;
            lock.unlock();
            return(chn);
        }
    }
    lock.unlock();
    return(NULL);
    // no empty channels found: use oldest one
    /*int min_index = 1<<30;
    int chn_id = 0;
    for(int cid = 0; cid < channels.size(); cid++)
    {
        if(chn_index[cid] < min_index && !this->is_loop[cid])
        {
            min_index = chn_index[cid];
            chn_id = cid;
        }
    }
    chn_index[chn_id] = ++last_channel;
    this->is_loop[chn_id] = is_loop;
    auto *chn = channels[chn_id];
    chn->abortStream();
    chn->closeStream();
    return(chn);*/
}
// set/get global volume
void SoundChannels::SetVolume(double vol)
{
    volume = max(min(vol,1.0),0.0);
}
double SoundChannels::GetVolume()
{
    return(volume);
}



//=============================================================================
// Common Spellcross sounds class.
// Contains sound effect channels, samples and midi engine and files.
//=============================================================================

// cleanup sounds
SpellSounds::~SpellSounds()
{
    // cleanup sound effect channels
    if(channels)
        delete channels;

    // cleanup samples
    for(auto & aux : aux_sounds)
        delete aux;
    aux_sounds.clear();      
    samples.clear();
}

// load sound stuff from spellcross fs data folder
SpellSounds::SpellSounds(FSarchive *common_fs, wstring& fs_data_path, int count,std::function<void(std::string)> status_list,std::function<void(std::string)> status_item)
{            
    channels = NULL;
    
    // load sound.fs (samples)
    if(status_list)
        status_list(" - Loading SAMPLES.FS...");
    FSarchive* samples_fs;
    try{
        wstring samples_fs_path = std::filesystem::path(fs_data_path) / std::filesystem::path("samples.fs");
        samples_fs = new FSarchive(samples_fs_path);
    }catch(const runtime_error& error) {        
        if(status_list)
            status_list("   - failed!");
        throw runtime_error("Loading SAMPLES.FS archive failed!");
    }

    // for each samples file:
    samples.reserve(samples_fs->Count());
    for(int fid = 0; fid < samples_fs->Count(); fid++)
    {
        // get file
        const char *name;
        uint8_t* data;
        int size;
        samples_fs->GetFile(fid,&data,&size,&name);
                
        if(wildcmp("1-*",name))
        {
            // 16-bit unsigned, 11025Hz, mono: convert to signed
            if(status_item)
                status_item(name);
            samples.emplace_back();
            SpellSample *smpl = &samples.back();
            strcpy_s(smpl->name, sizeof(smpl->name), name);
            smpl->fs = 11025;
            smpl->channels = 1;
            smpl->samples = size/2;
            smpl->data.resize(smpl->samples);
            uint16_t *ptr = (uint16_t*)data;
            for(int k = 0; k < smpl->samples; k++)
                smpl->data[k] = (int16_t)(*ptr++ - (uint16_t)0x8000u);
        }
        else if(wildcmp("8*",name))
        {
            // 8-bit unsigned, 11025Hz, mono: convert to 16bit signed mono
            if(status_item)
                status_item(name);
            samples.emplace_back();
            SpellSample* smpl = &samples.back();
            strcpy_s(smpl->name,sizeof(smpl->name),name);
            smpl->fs = 11025;
            smpl->channels = 1;
            smpl->samples = size;
            smpl->data.resize(smpl->samples);
            uint8_t* ptr = (uint8_t*)data;
            for(int k = 0; k < smpl->samples; k++)
                smpl->data[k] = (int16_t)((((uint16_t)*ptr++)<<8) - (uint16_t)0x8000u);
        }
        else if(wildcmp("U*",name))
        {
            // 8-bit unsigned, 11025Hz, mono (with RIFF WAVE header)            
            if(status_item)
                status_item(name);                       
            try{
                // try parse RIFF WAVE
                RIFF riff(data,size);
                // store PCM data
                samples.emplace_back();
                SpellSample* smpl = &samples.back();
                strcpy_s(smpl->name,sizeof(smpl->name),name);
                smpl->channels = riff.channels;
                smpl->fs = riff.fs;
                smpl->samples = riff.samples;
                riff.ConvertPCM(smpl->data);
            }catch(const runtime_error& error) {
                if(status_list)
                    status_list("   - failed!");
                throw runtime_error(string_format("Parsing sample \"%s\" failed!",name));
            }
        }
        else if(wildcmp("M*",name))
        {
            // 16-bit unsigned, 11025Hz, stereo: convert to signed
            if(status_item)
                status_item(name);
            samples.emplace_back();
            SpellSample* smpl = &samples.back();
            strcpy_s(smpl->name,sizeof(smpl->name),name);
            smpl->fs = 11025;
            smpl->channels = 2;
            smpl->samples = size/4;
            smpl->data.resize(smpl->channels*smpl->samples);
            uint16_t* ptr = (uint16_t*)data;
            for(int k = 0; k < smpl->samples; k++)
                smpl->data[k] = (int16_t)(*ptr++ - (uint16_t)0x8000u);
        }
    }    
    samples.shrink_to_fit();
    delete samples_fs;
    
    // init stream channels
    if(status_list)
        status_list(" - Creating sound streaming channels...");
    try{
        channels = new SoundChannels(count);
    }catch(const runtime_error& error) {
        if(status_list)
            status_list("   - failed!");
        throw runtime_error("Creating sound stream channels failed!");
    }

    // load common.fs

    /*std:future<void> tasks[6];
    tasks[0] = std::async(std::launch::async,&SpellSounds::ParseSoundClasses2,this,common_fs,"A_SOUNDS.DEF",&attack_sounds);
    tasks[1] = std::async(std::launch::async,&SpellSounds::ParseSoundClasses2,this,common_fs,"M_SOUNDS.DEF",&move_sounds);
    tasks[2] = std::async(std::launch::async,&SpellSounds::ParseSoundClasses2,this,common_fs,"H_SOUNDS.DEF",&report_sounds);
    tasks[3] = std::async(std::launch::async,&SpellSounds::ParseSoundClasses2,this,common_fs,"O_SOUNDS.DEF",&object_sounds);
    tasks[4] = std::async(std::launch::async,&SpellSounds::ParseSoundClasses2,this,common_fs,"S_SOUNDS.DEF",&spec_sounds);
    tasks[5] = std::async(std::launch::async,&SpellSounds::ParseSoundClasses2,this,common_fs,"Z_SOUNDS.DEF",&hit_sounds);
    for(int pid = 0; pid < 6; pid++)
        tasks[pid].get();*/
    
    string text;

    if(status_list)
        status_list(" - Parsing attack sound classes...");
    text = common_fs->GetFile("A_SOUNDS.DEF");
    attack_sounds = ParseSoundClasses(text);

    if(status_list)
        status_list(" - Parsing move sound classes...");
    text = common_fs->GetFile("M_SOUNDS.DEF");
    move_sounds = ParseSoundClasses(text);

    if(status_list)
        status_list(" - Parsing report sound classes...");
    text = common_fs->GetFile("H_SOUNDS.DEF");
    report_sounds = ParseSoundClasses(text);
    
    if(status_list)
        status_list(" - Parsing object sound classes...");
    text = common_fs->GetFile("O_SOUNDS.DEF");
    object_sounds = ParseSoundClasses(text);

    if(status_list)
        status_list(" - Parsing special sound classes...");
    text = common_fs->GetFile("S_SOUNDS.DEF");
    spec_sounds = ParseSoundClasses(text);

    if(status_list)
        status_list(" - Parsing hit sound classes...");
    text = common_fs->GetFile("Z_SOUNDS.DEF");
    hit_sounds = ParseSoundClasses(text);

    if(status_list)
        status_list(" - Parsing special sound effect classes...");
    text = common_fs->GetFile("SAMPLES.DEF");    
    SpellClassFile auxtable(text,";\\s*([^\\r^\\n]+)\\r?\\n?([^;]+)");    
    for(int k = 0; k < auxtable.list.size(); k++)
    {
        auto aux = &auxtable.list[k];
        if(aux->items.empty())
            continue;
        auto &name = aux->items[0];
        auto *smpl = GetSample(name.c_str());
        if(name.compare("#") != 0 && smpl)
            aux_sounds.push_back(new SpellSound(channels,smpl));
        else
            aux_sounds.push_back(NULL);
    }
    if(aux_sounds.size() >= 44)
    {
        // assign shortcuts
        vector<SpellSound **> list = {
            NULL, /* 0 - strategy map buttons */
            &aux_samples.btn_hover, /* 1 - mission selection hover */
            NULL, /* 2 - mission selection click  */
            &aux_samples.turret_rotate, /* 3 - rotate turret */
            NULL, /* 4 - big map button */
            NULL, /* 5 - scrollbar item select */
            NULL, /* 6 - scrollbar click */
            NULL, /* 7 - scrollbar move */
            NULL, /* 8 - big map attack button */
            NULL, /* 9 - big map cancel button */
            NULL, /* 10 - big map hierarchy part select button */
            NULL, /* 11 - big map hierarchy save unit button */
            NULL, /* 12 - big map hierarchy delete unit button */
            NULL, /* 13 - big map units upgrade button */
            NULL, /* 14 - big map units recruit button */
            NULL, /* 15 - big map units disband button */
            NULL, /* 16 - big map units OK button */
            NULL, /* 17 - big map units save unit button */
            NULL, /* 18 - big map buy button */
            &aux_samples.btn_end_turn, /* 19 - end turn button */
            NULL, /* 20 - big map OS attack */
            NULL, /* 21 - big map OS attack-win */
            NULL, /* 22 - big map OS end turn */
            &aux_samples.oponent_fire_os, /* 23 - oponent fire OS */
            &aux_samples.oponent_fire_aliance, /* 24 - oponent fire Aliance */
            &aux_samples.unit_level_up, /* 25 - aliance up level */
            NULL, /* 26 - unit info button */
            NULL, /* 27 - tap 1 */
            NULL, /* 28 - tap 2 */
            NULL, /* 29 - tap 3 */
            NULL, /* 30 - tap 4 */
            NULL, /* 31 - button load */
            NULL, /* 32 - button save */
            NULL, /* 33 - button gamma */
            NULL, /* 34 - button music volume */
            NULL, /* 35 - button sound volume */
            NULL, /* 36 - button change resolution */
            NULL, /* 37 - main menu button hover */
            NULL, /* 38 - main menu button clock */
            NULL, /* 39 - war map button */
            NULL, /* 40 - info button */
            NULL, /* 41 - mission start in warmap button */
            NULL, /* 42 - portal unit */
            NULL /* 43 - big map end */
        };
        for(int k = 0; k < list.size(); k++)
        {
            auto **smpl = list[k];
            if(!smpl)
                continue;
            *smpl = aux_sounds[k];
        }
    }

}

SpellSound* SpellSounds::GetSound(const char *name)
{
    SpellSound* sound = new SpellSound(channels,GetSample(name));
    return(sound);
}
SpellSound* SpellSounds::GetMoveClass(int index)
{
    if(index >= move_sounds.size())
        return(NULL);
    SpellSound *sound = new SpellSound(channels, move_sounds[index]);
    return(sound);
}
SpellSound* SpellSounds::GetReportClass(int index)
{
    if(index >= report_sounds.size())
        return(NULL);
    SpellSound* sound = new SpellSound(channels,{report_sounds[index].items[0], report_sounds[index].items[1]});
    return(sound);
}
SpellSound* SpellSounds::GetContactClass(int index)
{
    if(index >= report_sounds.size())
        return(NULL);
    SpellSound* sound = new SpellSound(channels,{report_sounds[index].items[2]});
    return(sound);
}
SpellAttackSound* SpellSounds::GetAttackClass(int index)
{
    if(index >= attack_sounds.size())
        return(NULL);
    SpellAttackSound* sound = new SpellAttackSound(channels,&attack_sounds[index]);
    return(sound);
}
SpellSound* SpellSounds::GetHitClass(int index)
{
    if(index >= hit_sounds.size())
        return(NULL);
    SpellSound* sound = new SpellSound(channels,hit_sounds[index].items[0]);
    return(sound);
}
SpellSound* SpellSounds::GetDieClass(int index)
{
    if(index >= hit_sounds.size())
        return(NULL);
    SpellSound* sound = new SpellSound(channels,hit_sounds[index].items[1]);
    return(sound);
}
SpellSound* SpellSounds::GetSpecialClass(int index)
{
    if(index >= spec_sounds.size())
        return(NULL);
    SpellSound* sound = new SpellSound(channels,spec_sounds[index].items[0]);
    return(sound);
}
SpellSound* SpellSounds::GetObjectHitClass(int index)
{
    if(index >= object_sounds.size())
        return(NULL);
    SpellSound* sound = new SpellSound(channels,object_sounds[index].items[0]);
    return(sound);
}
SpellSound* SpellSounds::GetObjectDestructClass(int index)
{
    if(index >= object_sounds.size())
        return(NULL);
    SpellSound* sound = new SpellSound(channels,object_sounds[index].items[1]);
    return(sound);
}



void SpellSounds::ParseSoundClasses2(FSarchive* fs, const char *name, vector<SpellSoundClassFile> *cls)
{
    auto text = fs->GetFile(name);
    *cls = ParseSoundClasses(text);
}

SpellSoundClassFile::SpellSoundClassFile()
{
    name = "";
    index = 0;
}

vector<SpellSoundClassFile> SpellSounds::ParseSoundClasses(string text)
{
    vector<SpellSoundClassFile> lines;

    int max_id = 0;
    //auto start_pos = text.find(";[");
    auto start_pos = 0;
    if(start_pos != string::npos)
    {
        text = text.substr(start_pos);        
        regex secexp(";\\s*\\[([\\d]+)\\]([^\\n\\r]+)\\r?\\n?(;)?\\r?\\n?([^;]+)");
        do {
            smatch match;
            std::regex_search(text,match,secexp);
            if(match.size() < 5)
                break;

            SpellSoundClassFile cls;
            cls.name = match[2].str();
            cls.index = atoi(match[1].str().c_str());
            auto ss = stringstream{match[4]};
            int first = true;
            for(std::string line; getline(ss,line,'\n');)
            {
                if(first && match[3].compare(";")==0)
                {
                    first = false;
                    continue;
                }
                auto cr = line.find('\r');
                if(cr != string::npos)
                    line.resize(cr);
                if(line.empty())
                    continue;
                cls.items_names.push_back(line);
                if(line.compare("#") != 0)
                    cls.items.push_back(GetSample(line.c_str()));
                else
                    cls.items.push_back(NULL);
            }
            // place class to proper ID position
            max_id = max(max_id, cls.index);
            lines.resize(max_id+1);
            lines[cls.index] = cls;

            text = match.suffix();
        } while(true);
    }
    return(lines);
}

SpellSample *SpellSounds::GetSample(const char *name)
{
    for(auto & smpl : samples)
        if(strcmp(smpl.name,name)==0)
            return(&smpl);
    return(NULL);
}






//-----------------------------------------------------------------------------
// Attack sound object (contains sample(s) reference and stream channels reference)
// contains several subsounds for different target types
//-----------------------------------------------------------------------------
SpellAttackSound::SpellAttackSound(SoundChannels* channels,SpellSoundClassFile* sound_class)
{
    if(sound_class->items.size() < 4)
        throw runtime_error("Ivalid size of attack sound class!");
    shot = new SpellSound(channels,sound_class->items.at(0));
    hit_flash = new SpellSound(channels,sound_class->items.at(1));
    hit_armor = new SpellSound(channels,sound_class->items.at(2));
    hit_miss = new SpellSound(channels,sound_class->items.at(3));
}
SpellAttackSound::SpellAttackSound(SpellAttackSound &ref)
{
    shot = new SpellSound(*ref.shot);
    hit_flash = new SpellSound(*ref.hit_flash);
    hit_armor = new SpellSound(*ref.hit_armor);
    hit_miss = new SpellSound(*ref.hit_miss);
}
SpellAttackSound::~SpellAttackSound()
{
    delete shot;
    delete hit_flash;
    delete hit_armor;
    delete hit_miss;
}
// all possible sounds done (or idle)?
int SpellAttackSound::isDone()
{
    return(shot->isDone() && hit_flash->isDone() && hit_armor->isDone() && hit_miss->isDone());
}



//-----------------------------------------------------------------------------
// Sound object (contains sample(s) reference and stream channels reference)
//-----------------------------------------------------------------------------

// common construct
void SpellSound::SpellSoundInit()
{
    dac = NULL;
    sub_id = samples.size();
    control = CTRL_IDLE;
    // find first non-empty sample
    for(auto & smpl : samples)
    {
        if(smpl)
        {
            channels = smpl->channels;
            count = smpl->samples;
            fs = smpl->fs;
        }
    }
    left_vol = 65535;
    right_vol = 65535;
    force_loop = false;
    auto_delete = false;
}
// define movement sound (start-move-stop)
SpellSound::SpellSound(SoundChannels* channels,SpellSoundClassFile &move_class)
{
    samples = move_class.items;
    mode = MODE_MOVE;
    streams = channels;
    SpellSoundInit();
}
// define sound (or random set of sounds) for action
SpellSound::SpellSound(SoundChannels* channels,vector<SpellSample*> sounds)
{
    samples = sounds;
    mode = MODE_RAND;
    streams = channels;
    SpellSoundInit();
}
// define sound (or random set of sounds) for action
SpellSound::SpellSound(SoundChannels* channels,SpellSample* sound)
{
    samples = {sound};
    mode = MODE_RAND;
    streams = channels;
    SpellSoundInit();
}
// cleanup sound
SpellSound::~SpellSound()
{
}
// copy
SpellSound::SpellSound(SpellSound& ref)
{
    samples = ref.samples;
    mode = ref.mode;
    streams = ref.streams;    
    channels = ref.channels;
    count = ref.count;
    fs = ref.fs;
    left_vol = 65535;
    right_vol = 65535;
    force_loop = ref.force_loop;
    dac = NULL;
    sub_id = samples.size();
    control = CTRL_IDLE;
    is_playing = false;
}

// stream callback
int SpellSound_RtCallback(void* outputBuffer,void* inputBuffer,unsigned int nBufferFrames,
    double streamTime,RtAudioStreamStatus status,void* userData)
{
    SpellSound* data = (SpellSound*)userData;
    int stop = data->cb_GetFrame((int16_t*)outputBuffer,nBufferFrames);

    // try call user callback
    data->cb_user_callback();
    
    // force stop signalization?
    if(streamTime >= SoundChannels::FAKE_STOP_TIME_MARK)
        stop = 1;
    
    // automatic object delete?
    if(stop != 0 && data->isAutoDelete())
        delete data;
    else if(stop != 0)
        data->cb_DoneFlag();

    return(stop);
}


int SpellSound::isAutoDelete()
{
    return(auto_delete);
}

// call user callback from frame callback (keep it short!)
void SpellSound::cb_user_callback()
{
    if(user_frame_callback)
        user_frame_callback();
}

// signalize playback is done
void SpellSound::cb_DoneFlag()
{
    is_playing = false;
}

// copy sample data to DAC frame
int SpellSound::cb_GetFrame(int16_t *buffer, int count)
{
    //SpellSample *smpl;
    int smpl_size = cb_GetSmplSize();
    while(count)
    {
        if(control == CTRL_STOP)
        {
            // immediate stop
            sub_id = samples.size();
            memset(buffer,0,count*2);            
            return(1);
        }

        if(mode == MODE_RAND)
        {
            // single sample mode:
            if(pos >= samples[sub_id]->samples)
            {
                if(force_loop)
                {
                    // reset position to loop
                    pos = 0;
                }
                else
                {
                    // finito: fill with zeros
                    memset(buffer,0,count*2);
                    count = 0;
                    sub_id = samples.size();
                    return(1);
                }
            }

            int smpl_count = min(count, samples[sub_id]->samples - pos);            
            buffer = cb_GetSmplData(buffer, smpl_count);
            count -= smpl_count;
            continue;
        }
        else
        {
            // multi-samples mode:
            if(sub_id >= samples.size())
            {
                // finito: fill with zeros
                memset(buffer,0,count*2);
                count = 0;
                return(1);
            }

            if(sub_id == 0)
            {
                // move start:

                if(control == CTRL_END_MOVE)
                {
                    // skip start:
                    sub_id = 2;
                    pos = 0;
                    continue;
                }
                
                if(!samples[sub_id] || pos >= samples[sub_id]->samples)
                {
                    sub_id++;
                    pos = 0;
                    continue;
                }
                
            }
            else if(sub_id == 1)
            {
                // move sustain:

                if(control == CTRL_END_MOVE)
                {
                    // stop sustain:
                    sub_id++;
                    pos = 0;
                    continue;
                }

                if(!samples[sub_id])
                {
                    // loop silence:
                    memset(buffer,0,count*2);
                    count = 0;
                    return(0);
                }

                if(pos >= samples[sub_id]->samples)
                {
                    // loop:
                    pos = 0;
                }

            }
            else
            {
                // move stop:

                if(!samples[sub_id] || pos >= samples[sub_id]->samples)
                {
                    // terminate:
                    memset(buffer,0,count*2);
                    count = 0;
                    sub_id++;
                    return(1);
                }

            }

            int smpl_count = min(count,samples[sub_id]->samples - pos);
            if(smpl_count)
                buffer = cb_GetSmplData(buffer,smpl_count);
            count -= smpl_count;
            continue;

        }
    }
    // continue
    return(0);
}

int16_t *SpellSound::cb_GetSmplData(int16_t* buffer, int count)
{
    SpellSample* smpl = samples[sub_id];
    if(!smpl)
        return(NULL);
    auto *data = &smpl->data[smpl->channels*pos];
    pos += count;

    double volume = streams->GetVolume();
    uint32_t g_vol = (uint32_t)(65534.0*volume);
    uint32_t left  = (g_vol*left_vol)>>16;
    uint32_t right  = (g_vol*right_vol)>>16;

    if(smpl->channels == 1)
    {
        // one source channel, two output channels
        for(int k = 0; k < count; k++)
        {
            int32_t smpl = *data++;
            *buffer++ = (int16_t)((left * smpl)>>16);
            *buffer++ = (int16_t)((right * smpl)>>16);
        }
    }
    else
    {
        // two source channels, two output channels
        for(int k = 0; k < count; k++)
        {
            *buffer++ = (int16_t)((left * ((int32_t)*data++))>>16);
            *buffer++ = (int16_t)((right * ((int32_t)*data++))>>16);
        }
    }
    return(buffer);
}

int SpellSound::cb_GetSmplSize()
{
    SpellSample* smpl = samples[sub_id];
    if(!smpl)
        return(0);
    return(smpl->channels);
}

int SpellSound::cb_GetSmplLeft()
{
    SpellSample *smpl = samples[sub_id];
    if(!smpl)
        return(0);
    int left = (smpl->samples - pos)/smpl->channels;
    return(left);
}

// get spellcross sample
SpellSample* SpellSound::GetSample(int id)
{
    if(id >= samples.size())
        return(NULL);
    return(samples[id]);
}

// set relative volume of left/right channel
int SpellSound::SetPanning(double left_vol,double right_vol)
{
    this->left_vol = (int)(65534.0*max(min(left_vol,1.0),0.0));
    this->right_vol = (int)(65534.0*max(min(right_vol,1.0),0.0));
    return(0);
}

// async playback
void SpellSound::PlayAsync()
{
    Play(true);
}

// start playback, optional auto-object delete after playback, optional loop, optional callback function to be called every frame (time critical!)
int SpellSound::Play(bool auto_delete, bool loop, std::function<void(void)> frame_callback, double frame_step)
{    
    // automatically delete object after playaback?
    this->auto_delete = auto_delete;
    force_loop = loop;
    
    // store user callback pointer
    user_frame_callback = frame_callback;

    //control = CTRL_STOP;
    if(!isDone())
    {
        if(dac)
        {
            dac->abortStream();
            dac->closeStream();
        }
    }
    
    // get some stream channel
    dac = streams->GetChannel(loop);
    if(!dac)
    {
        if(auto_delete)
            delete this;
        return(1);
    }
    

    // pick first (or random) substream
    sub_id = 0;
    SpellSample *smpl = samples[0];
    if(mode == MODE_RAND)
    {
        sub_id = std::rand()%samples.size();
        smpl = samples[sub_id];
        if(!smpl)
        {
            // no sound defined
            if(auto_delete)
                delete this; // note: this is scary, but legal-ish???
            return(1);
        }
        fs = smpl->fs;
        channels = smpl->channels;    
    }
    /*if(!smpl)
        return(1);*/
    pos = 0;
    control = CTRL_IDLE;
    is_playing = true;

    // init stream params
    RtAudio::StreamParameters parameters;
    parameters.deviceId = dac->getDefaultOutputDevice();
    parameters.firstChannel = 0;
    parameters.nChannels = 2;
    unsigned int sampleRate = fs;
    unsigned int bufferFrames = max(32,min((int)(frame_step*fs),512))&(~1);

    // start stream    
    dac->openStream(&parameters,NULL,RTAUDIO_SINT16,sampleRate,&bufferFrames,&SpellSound_RtCallback,(void*)this);
    dac->startStream();
        
    return(0);
}

// get current playback time
double SpellSound::GetPlaybackTime()
{
    if(!dac)
        return(0.0);
    if(!dac->isStreamRunning())
        return(0.0);
    return(dac->getStreamTime());
}

// send move stop command
int SpellSound::StopMove()
{
    control = CTRL_END_MOVE;
    return(0);
}

// send stop command
int SpellSound::Stop(double wait)
{
    control = CTRL_STOP;
    if(wait < 1e-9)
        return(0);
    
    // wait to finish
    auto start = std::chrono::high_resolution_clock::now();
    while(!isDone())
    {
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = (std::chrono::duration_cast<std::chrono::milliseconds>(stop - start)).count();
        if((double)duration > wait*1000.0)
            return(1); // timeout
        this_thread::sleep_for(10ms);
    }

    return(0);
}

// check if playback is done
int SpellSound::isDone()
{
    /*if(dac == NULL)
        return(true);
    if(mode == MODE_RAND && sub_id >= samples.size())
        return(true);
    if(mode == MODE_MOVE && sub_id >= samples.size())
        return(true);*/
    return(!is_playing);
}





//-----------------------------------------------------------------------------
// Map sound object (header in map.h)
//-----------------------------------------------------------------------------
MapSound::MapSound(MapXY pos,SpellSample* sample)
{
    m_pos = pos;
    m_sample = sample;
}
const char* MapSound::GetName()
{
    if(!m_sample)
        return(NULL);
    return(m_sample->name);
}
MapXY MapSound::GetPosition()
{
    return(m_pos);
}
SpellSample *MapSound::GetSample()
{
    return(m_sample);
}