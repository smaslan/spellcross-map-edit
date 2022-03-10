//=============================================================================
// Sound related Spellcross stuff (loaders)
// 
// This code is part of Spellcross Map Editor project.
// (c) 2022, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#undef _HAS_STD_BYTE
#define _HAS_STD_BYTE 0

//#undef __RTAUDIO_DUMMY__
//#define __WINDOWS_DS__

#include "spell_sound.h"
#include "fs_archive.h"
#include "other.h"


#include "RtAudio.h"

#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <regex>
#include <filesystem>
#include <sstream>
#include <future>

using namespace std;


// initialize stream channels
SoundChannels::SoundChannels(int count)
{
    chn_index.assign(count,0);
    channels.assign(count,NULL);
    for(auto & chn : channels)
        chn = new RtAudio();    
    last_channel = 0;
}
// close stream channels
SoundChannels::~SoundChannels()
{
    // loose all stream channels
    for(auto& chn : channels)
        delete chn;
    channels.clear();
}
// get first available channel
RtAudio* SoundChannels::GetChannel()
{
    // look for first free stream
    for(int cid = 0; cid < channels.size(); cid++)
    {
        auto *chn = channels[cid];        
        if(!chn->isStreamRunning())
        {
            if(chn->isStreamOpen())
                chn->closeStream();
            chn_index[cid] = ++last_channel;
            return(chn);
        }
    }
    // no empty channels found: use oldest one
    int min_index = 1<<30;
    int chn_id = 0;
    for(int cid = 0; cid < channels.size(); cid++)
    {
        if(chn_index[cid] < min_index)
        {
            min_index = chn_index[cid];
            chn_id = cid;
        }
    }
    chn_index[chn_id] = ++last_channel;
    auto *chn = channels[chn_id];
    chn->abortStream();
    chn->closeStream();
    return(chn);
}


// cleanup sounds
SpellSounds::~SpellSounds()
{
    samples.clear();

    if(channels)
        delete channels; 
}

// load sound stuff from spellcross fs data folder
SpellSounds::SpellSounds(wstring& fs_data_path, int count)
{            
    channels = NULL;
    
    // load sound.fs (samples)
    wstring samples_fs_path = fs_data_path + L"\\samples.fs";
    FSarchive *samples_fs = new FSarchive(samples_fs_path);

    // for each samples file:
    samples.reserve(samples_fs->Count());
    for(int fid = 0; fid < samples_fs->Count(); fid++)
    {
        // get file
        char *name;
        uint8_t* data;
        int size;
        samples_fs->GetFile(fid,&data,&size,&name);
                
        if(wildcmp("1-*",name))
        {
            // 16-bit unsigned, 11025Hz, mono: convert to signed
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
            // 16-bit signed, 11025Hz, mono (with RIFF WAVE header)

        }
        else if(wildcmp("M*",name))
        {
            // 16-bit unsigned, 11025Hz, stereo: convert to signed
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



    /*RtAudio dac;
    int devs = dac.getDeviceCount();

    RtAudio::StreamParameters parameters;
    parameters.deviceId = dac.getDefaultOutputDevice()+1;
    parameters.nChannels = samples[0].channels;
    parameters.firstChannel = 0;
    unsigned int sampleRate = samples[0].fs;
    unsigned int bufferFrames = 256; // 256 sample frames
    samples[0].pos = 0;

    dac.openStream(&parameters,NULL,RTAUDIO_SINT16,sampleRate,&bufferFrames,&scb,(void*)&samples[0]);
    dac.startStream();
    
    Sleep(3000);*/


    // load common.fs
    wstring common_fs_path = fs_data_path + L"\\common.fs";
    FSarchive* common_fs = new FSarchive(common_fs_path);

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

    text = common_fs->GetFile("A_SOUNDS.DEF");
    attack_sounds = ParseSoundClasses(text);

    text = common_fs->GetFile("M_SOUNDS.DEF");
    move_sounds = ParseSoundClasses(text);

    text = common_fs->GetFile("H_SOUNDS.DEF");
    report_sounds = ParseSoundClasses(text);
    
    text = common_fs->GetFile("O_SOUNDS.DEF");
    object_sounds = ParseSoundClasses(text);

    text = common_fs->GetFile("S_SOUNDS.DEF");
    spec_sounds = ParseSoundClasses(text);

    text = common_fs->GetFile("Z_SOUNDS.DEF");
    hit_sounds = ParseSoundClasses(text);


    // init stream channels
    channels = new SoundChannels();

    
    /*for(int k = 0; k < move_sounds.size(); k++)
    {
        SpellSound snd(channels, move_sounds[k]);

        snd.Play();
        Sleep(3000);    
        snd.StopMove();

        while(!snd.isDone())
            Sleep(50);
    }*/

    delete common_fs;    
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
    dac = NULL;
    sub_id = samples.size();
    control = CTRL_IDLE;
}

// stream callback
int SpellSound_RtCallback(void* outputBuffer,void* inputBuffer,unsigned int nBufferFrames,
    double streamTime,RtAudioStreamStatus status,void* userData)
{
    SpellSound* data = (SpellSound*)userData;
    return(data->cb_GetFrame((int16_t*)outputBuffer,nBufferFrames));
}

// copy sample data to DAC frame
int SpellSound::cb_GetFrame(int16_t *buffer, int count)
{
    SpellSample *smpl;
    int smpl_size = cb_GetSmplSize();
    while(count)
    {
        if(control == CTRL_STOP)
        {
            // immediate stop
            sub_id = samples.size();
            memset(buffer,0,count*smpl_size);
            return(1);
        }

        if(mode == MODE_RAND)
        {
            // single sample mode:
            if(pos >= samples[sub_id]->samples)
            {
                // finito: fill with zeros
                memset(buffer,0,count*smpl_size);
                count = 0;
                sub_id = samples.size();
                return(1);
            }

            int smpl_count = min(count, samples[sub_id]->samples - pos);
            memcpy(buffer,cb_GetSmplData(smpl_count),smpl_count*smpl_size*sizeof(int16_t));
            count -= smpl_count;
            continue;
        }
        else
        {
            // multi-samples mode:
            if(sub_id >= samples.size())
            {
                // finito: fill with zeros
                memset(buffer,0,count*smpl_size);
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
                    memset(buffer,0,count*smpl_size);
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
                    memset(buffer,0,count*smpl_size);
                    count = 0;
                    sub_id++;
                    return(1);
                }

            }

            int smpl_count = min(count,samples[sub_id]->samples - pos);
            if(smpl_count)
                memcpy(buffer,cb_GetSmplData(smpl_count),smpl_count*smpl_size*sizeof(int16_t));
            count -= smpl_count;
            continue;

        }
    }
    // continue
    return(0);
}

int16_t *SpellSound::cb_GetSmplData(int count)
{
    SpellSample* smpl = samples[sub_id];
    if(!smpl)
        return(NULL);
    auto *data = &smpl->data[smpl->channels*pos];
    pos += count;
    return(data);
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


int SpellSound::Play()
{    
    

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
    dac = streams->GetChannel();

    // pick first (or random) substream
    sub_id = 0;
    SpellSample *smpl = samples[0];
    if(mode == MODE_RAND)
    {
        sub_id = std::rand()%samples.size();
        smpl = samples[sub_id];
        if(!smpl)
            return(1);
        fs = smpl->fs;
        channels = smpl->channels;    
    }
    /*if(!smpl)
        return(1);*/
    pos = 0;
    control = CTRL_IDLE;

    // init stream params
    RtAudio::StreamParameters parameters;
    parameters.deviceId = dac->getDefaultOutputDevice();
    parameters.firstChannel = 0;
    parameters.nChannels = channels;
    unsigned int sampleRate = fs;
    unsigned int bufferFrames = 256;

    // start stream    
    dac->openStream(&parameters,NULL,RTAUDIO_SINT16,sampleRate,&bufferFrames,&SpellSound_RtCallback,(void*)this);
    dac->startStream();
    
    return(0);
}

// send move stop command
int SpellSound::StopMove()
{
    control = CTRL_END_MOVE;
    return(0);
}

// check if playback is done
int SpellSound::isDone()
{
    if(mode == MODE_RAND && sub_id >= samples.size())
        return(true);
    if(mode == MODE_MOVE && sub_id >= samples.size())
        return(true);
    return(false);
}