#pragma once

#include <string>
#include "fs_archive.h"

class SpellCAN
{
private:

    vector<uint8_t> audio;
    vector<vector<uint8_t>> video;
    int x_size;
    int y_size;

    int Decode(uint8_t *data, int len);

public:

    SpellCAN(FSarchive *fs, std::string name);
    SpellCAN(std::wstring path);

    int GetFramesCount();
    int SaveFrames(std::wstring folder_path);

};
