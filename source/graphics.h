#pragma once

#include <cstdint>
#include <vector>

class ImgQuantize{

public:

    struct Pixel
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    static std::vector<Pixel> GenMedianCutPalette(const std::vector<Pixel>& source,int numColors);

private:
    
};
