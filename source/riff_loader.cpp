//=============================================================================
// Simple RIFF container parser (immune to rubbish data leftovers by Spellcross
// creators in some files :-).
// 
// This code is part of Spellcross Map Editor project.
// (c) 2022, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================

#include "riff_loader.h"
#include <string>
#include <stdexcept>


// parse RIFF container (no conversions, just checking and parsing headers)
RIFF::RIFF(uint8_t* data,int len)
{
    auto *dptr = data;
    auto dend = &data[len];
    
    // check RIFF header
    if(len < 12 || std::memcmp(dptr, "RIFF", 4) != 0)
        throw std::runtime_error("Not a RIFF container (no RIFF header).");
    dptr += 4;
    uint32_t riff_len = *(uint32_t*)dptr; dptr += sizeof(uint32_t);
    if(riff_len + 8 > len)
        throw std::runtime_error("Input RIFF file possibly incomplete!");
    auto riffend = &dptr[len];

    // check format
    if(std::memcmp(dptr,"WAVE",4) != 0)
        throw std::runtime_error("RIFF is not a WAVE format.");
    dptr += 4;

    // parse WAVE chunk
    uint32_t pcm_count;
    int complete_level = 0;
    do{
        if(&dptr[8] > riffend)
            throw std::runtime_error("Input RIFF file possibly incomplete!");
        uint32_t sub_chunk_size = *((uint32_t*)&dptr[4]);
        if(&dptr[8 + sub_chunk_size] > riffend)
            throw std::runtime_error("Input RIFF file possibly incomplete!");
        dptr += 8;
        if(std::memcmp(&dptr[-8],"fmt ",4) == 0)
        {
            // format sub-chunk
            if(sub_chunk_size != 16)
                throw std::runtime_error("Input RIFF file has corrupted \"fmt \" sub-chunk!");
        
            // check PCM format
            uint16_t format = *((uint16_t*)&dptr[0]);
            if(format != 1)
                throw std::runtime_error("Input RIFF file format is not PCM!");
        
            // get data format
            channels = *((uint16_t*)&dptr[2]);
            fs = *((uint32_t*)&dptr[4]);
            bits = *((uint16_t*)&dptr[14]);

            complete_level++;
        }
        else if(std::memcmp(&dptr[-8],"data",4) == 0)
        {
            // data sub-chunk            
            pcm_count = sub_chunk_size;
            pcm_data = dptr;
            
            complete_level++;
        }
        dptr += sub_chunk_size;
    
    }while(dptr < riffend && complete_level < 2);
    if(complete_level != 2)
        throw std::runtime_error("Input RIFF WAVE file is missing either \"data\" or \"fmt \" sub-chunk!");
    
    // calculate samples count
    samples = pcm_count/channels/(bits>>3);

}

// convert PCM to target format and store to target
int RIFF::ConvertPCM(std::vector<int16_t>& target)
{
    int total = samples*channels;
    target.resize(total);
    if(bits == 8)
    {                
        // 8-bit unsigned format to 16-bit signed
        for(int k = 0; k < total; k++)
            target[k] = (int16_t)((((uint16_t)pcm_data[k])<<8) - (uint16_t)0x8000u);
    }
    else if(bits == 16)
    {
        // 16-bit signed format to 16-bit signed
        std::memcpy(target.data(), pcm_data, total*2);
    }
    else
        return(1);
    return(0);
}