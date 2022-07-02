#pragma once

#include <string>
#include <tuple>
#include "fs_archive.h"
#include "spell_sound.h"

class SpellVideo
{
private:

    SpellSample audio;
    vector<vector<uint8_t>> frames;
    uint8_t pal[256][3];
    int x_size;
    int y_size;
    double fps;

    int DecodeCAN(uint8_t *data, int len);
    int DecodeDPK(uint8_t* data,int len);

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

};
