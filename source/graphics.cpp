// Graphics routines
// based on: https://indiegamedev.net/2020/01/17/median-cut-with-floyd-steinberg-dithering-in-c/

#include "graphics.h"
#include <algorithm>


bool red_comp(const ImgQuantize::Pixel& a,const ImgQuantize::Pixel& b)
{
    return a.r < b.r;
}
bool green_comp(const ImgQuantize::Pixel& a,const ImgQuantize::Pixel& b)
{
    return a.g < b.g;
}
bool blue_comp(const ImgQuantize::Pixel& a,const ImgQuantize::Pixel& b)
{
    return a.b < b.b;
}

uint8_t determine_primary_color_and_sort_box(std::vector<ImgQuantize::Pixel>& box,int &which)
{
    uint8_t redRange =   std::max_element(box.begin(),box.end(),red_comp)->r   - std::min_element(box.begin(),box.end(),red_comp)->r;
    uint8_t greenRange = std::max_element(box.begin(),box.end(),green_comp)->g - std::min_element(box.begin(),box.end(),green_comp)->g;
    uint8_t blueRange =  std::max_element(box.begin(),box.end(),blue_comp)->b  - std::min_element(box.begin(),box.end(),blue_comp)->b;

    if(redRange >= greenRange && redRange >= blueRange)
    {
        if(which != 0)
            std::sort(box.begin(),box.end(),red_comp);
        which = 0;
        return(redRange);
    }
    else if(greenRange >= redRange && greenRange >= blueRange)
    {
        if(which != 1)
            std::sort(box.begin(),box.end(),green_comp);
        which = 1;
        return(greenRange);
    }
    else
    {
        if(which != 2)
            std::sort(box.begin(),box.end(),blue_comp);
        which = 2;
        return(blueRange);
    }
}

std::vector<ImgQuantize::Pixel> ImgQuantize::GenMedianCutPalette(const std::vector<ImgQuantize::Pixel>& source,int numColors)
{
    typedef std::vector<Pixel> Box;
    struct RangeBox{
        int which;
        uint8_t range;
        Box box;
    };

    std::vector<RangeBox> boxes;
    Box init = source;
    boxes.push_back(RangeBox(-1,0,init));

    while(boxes.size() < numColors)
    {
        /* for each box, sort the boxes pixels according to the colour it has the most range in */
        for(RangeBox& boxData : boxes)
            if(boxData.range == 0)
                boxData.range = determine_primary_color_and_sort_box(boxData.box,boxData.which);

        // sort boxes by color range
        std::sort(boxes.begin(),boxes.end(),[](const RangeBox& a,const RangeBox& b) {return a.range < b.range;});
        
        auto itr = std::prev(boxes.end()); 
        auto biggestBox = itr->box;
        auto which = itr->which;
        boxes.erase(itr);

        // leave if nothing more to split (was not in original code - would loop forever when not enough colours?)
        if(biggestBox.size() <= 1)
            break;

        // the box is sorted already, so split at median
        Box splitA(biggestBox.begin(),biggestBox.begin() + biggestBox.size() / 2);
        Box splitB(biggestBox.begin() + biggestBox.size() / 2,biggestBox.end());

        boxes.push_back(RangeBox(which,0,splitA));
        boxes.push_back(RangeBox(which,0,splitB));
    }      

    struct Pal{
        int r;
        int g;
        int b;
        double h;
        double s;
        double v;
        double x;
    };

    // each box in boxes can be averaged to determine the colour
    std::vector<Pal> palette;
    for(const RangeBox& boxData: boxes)
    {
        auto &box = boxData.box;
        int redAccum = 0;
        int greenAccum = 0;
        int blueAccum = 0;
        std::for_each(box.begin(),box.end(),[&](const Pixel& p)
            {
                redAccum += p.r;
                greenAccum += p.g;
                blueAccum += p.b;
            });
        redAccum /= box.size();
        greenAccum /= box.size();
        blueAccum /= box.size();

        Pal col;
        col.r = std::min((uint32_t)redAccum,255u);
        col.g = std::min((uint32_t)greenAccum,255u);
        col.b = std::min((uint32_t)blueAccum,255u);
        int cmax = std::max(std::max(col.r,col.g),col.b);
        int cmin = std::min(std::min(col.r,col.g),col.b);
        int delta = cmax - cmin;
        if(delta <= 0.0)
            col.h = 0.0;
        else if(cmax == col.r)
            col.h = 60.0*(col.g - col.b)/delta + 360.0;
        else if(cmax == col.g)
            col.h = 60.0*(col.b - col.r)/delta + 120.0;
        else
            col.h = 60.0*(col.r - col.g)/delta + 240.0;
        col.h = fmod(col.h + 360.0*2.0,360.0);
        if(!delta)
            col.s = 0.0;
        else
            col.s = (double)delta/cmax;
        col.v = cmax/256.0;

        //col.x = col.h*256.0*256.0 + col.s*256.0 + col.v;
        col.x = (col.h/360.0 + 1)*10 + col.v;

        palette.push_back(col);
    }

    auto uid = std::unique(palette.begin(),palette.end(),[](Pal &a,Pal &b){return a.r == b.r && a.g == b.g && a.b == b.b;});
    palette.resize(std::distance(palette.begin(),uid));
       
    
    //std::sort(palette.begin(),palette.end(),[](const Pal& a,const Pal& b) {return a.v < b.v;});
    //std::sort(palette.begin(),palette.end(),[](const Pal& a,const Pal& b) {return a.s < b.s;});
    //std::sort(palette.begin(), palette.end(),[](const Pal& a,const Pal& b) {return a.h < b.h;});
    std::sort(palette.begin(),palette.end(),[](const Pal& a,const Pal& b) {return a.x < b.x;});

    std::vector<Pixel> pal;
    for(auto &col: palette)
    {
        Pixel pix = {col.r,col.g,col.b};
        pal.push_back(pix);
    }


    return pal;
}