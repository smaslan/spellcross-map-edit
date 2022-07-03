#pragma once

#include <cstdint>
#include <vector>

class RIFF
{
private:
public:
    
    RIFF(uint8_t *data, int len);
    int ConvertPCM(std::vector<int16_t> &target);

    // format stuff
    int fs;
    int channels;
    int bits;
    int samples;
    
    // raw data offset
    uint8_t *pcm_data;
};
