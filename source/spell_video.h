#pragma once

#include <string>
#include <tuple>
#include "fs_archive.h"
#include "spell_sound.h"



// video resource
class SpellVideoResource
{
public:
    SpellVideoResource(std::string name,FSarchive* fs) { this->name = name; this->fs = fs; };
    std::string name;
    FSarchive* fs;
};

// list of all found spellcross video resources
class SpellVideoResources
{
private:

    std::vector<SpellVideoResource> vid_list;
    std::vector<FSarchive*> fs_list; // list of loaded FS archives
    
public:

    SpellVideoResources(std::vector<std::wstring> folders);
    ~SpellVideoResources();

    std::vector<std::string> GetNames(std::string wild="*");

};


// vide file
class SpellVideo
{
private:

    enum class SpellVideoType {
        CAN = 1,
        DPK,
        DP2
    };

    SpellSample audio;
    vector<vector<uint8_t>> frames;
    uint8_t pal[256][3];
    int x_size;
    int y_size;
    double fps;
    SpellVideoType type;

    int DecodeCAN(uint8_t* data,int len);
    int DecodeDPK(uint8_t* data,int len);
    int DecodeDP2(uint8_t* dp2,int dp2_len,uint8_t* riff,int riff_len);

public:       
    
    SpellVideo(FSarchive *fs, std::string name);
    SpellVideo(std::wstring path);

    std::tuple<int,int> GetResolution();
    int GetFramesCount();
    int SaveFrames(std::wstring folder_path);
    uint8_t* GetFrame(int id);
    int GetFrameID(double time);
    uint8_t* GetPalette();
    SpellSample *GetSound();
    bool isDPK() {return(type == SpellVideoType::DPK);};
    bool isCAN() { return(type == SpellVideoType::CAN); };
    bool isDP2() { return(type == SpellVideoType::DP2); };


};
