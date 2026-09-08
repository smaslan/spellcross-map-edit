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

std::vector<ImgQuantize::Pixel> ImgQuantize::GenMedianCutPalette(const std::vector<ImgQuantize::Pixel>& source,int numColors)
{
    typedef std::vector<Pixel> Box;
    struct RangeBox{
        int begin;
        int end;
        std::vector<ImgQuantize::Pixel> *channel;
        uint8_t range;
    };

    Box data = source;
    std::vector<RangeBox> boxes;
    boxes.push_back(RangeBox(0,source.size() - 1,&data,0));

    while(boxes.size() < numColors)
    {
        /* for each box, sort the boxes pixels according to the colour it has the most range in */
        for(RangeBox& box: boxes)
            if(box.range == 0)
            {
                uint8_t redRange =   std::max_element(box.channel->begin() + box.begin,box.channel->begin() + box.end,red_comp)->r   - std::min_element(box.channel->begin() + box.begin,box.channel->begin() + box.end,red_comp)->r;
                uint8_t greenRange = std::max_element(box.channel->begin() + box.begin,box.channel->begin() + box.end,green_comp)->g - std::min_element(box.channel->begin() + box.begin,box.channel->begin() + box.end,green_comp)->g;
                uint8_t blueRange =  std::max_element(box.channel->begin() + box.begin,box.channel->begin() + box.end,blue_comp)->b  - std::min_element(box.channel->begin() + box.begin,box.channel->begin() + box.end,blue_comp)->b;
                if(redRange >= greenRange && redRange >= blueRange)
                {
                    std::sort(box.channel->begin() + box.begin,box.channel->begin() + box.end,red_comp);
                    box.range = redRange;
                }
                else if(greenRange >= redRange && greenRange >= blueRange)
                {
                    std::sort(box.channel->begin() + box.begin,box.channel->begin() + box.end,green_comp);
                    box.range = greenRange;
                }
                else
                {
                    std::sort(box.channel->begin() + box.begin,box.channel->begin() + box.end,blue_comp);
                    box.range = blueRange;
                }
            }

        // sort boxes by color range
        std::sort(boxes.begin(),boxes.end(),[](const RangeBox& a,const RangeBox& b) {return a.range < b.range;});
        
        auto itr = std::prev(boxes.end()); 
        auto box = *itr;
        boxes.erase(itr);

        // leave if nothing more to split (was not in original code - would loop forever when not enough colours?)
        if(box.end - box.begin <= 1)
            break;

        // the box is sorted already, so split at median
        RangeBox A = {box.begin,box.begin + (box.end - box.begin + 1)/2 - 1,box.channel,0};
        RangeBox B ={box.begin + (box.end - box.begin + 1)/2,box.end,box.channel,0};
        boxes.push_back(A);
        boxes.push_back(B);
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
    for(const RangeBox& box: boxes)
    {
        int redAccum = 0;
        int greenAccum = 0;
        int blueAccum = 0;
        std::for_each(box.channel->begin() + box.begin,box.channel->begin() + box.end,[&](const Pixel& p)
            {
                redAccum += p.r;
                greenAccum += p.g;
                blueAccum += p.b;
            });
        int size = box.end - box.begin + 1;
        redAccum /= size;
        greenAccum /= size;
        blueAccum /= size;

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




// geometric distance of colors
double ImgQuantize::Pixel::distance_squared(const Pixel& other) const {
    int dr = static_cast<int>(r) - other.r;
    int dg = static_cast<int>(g) - other.g;
    int db = static_cast<int>(b) - other.b;
    return(dr*dr + dg*dg + db*db);
}

// Determines which of the 8 children a color belongs to at a specific bit depth
int ImgQuantize::OctreeNode::get_child_index(const Pixel& cls,int depth) {
    int shift = 7 - depth;
    int r_bit = (cls.r >> shift) & 1;
    int g_bit = (cls.g >> shift) & 1;
    int b_bit = (cls.b >> shift) & 1;
    return((r_bit << 2) | (g_bit << 1) | b_bit);
}

// insert node 
void ImgQuantize::ColorOctree::insert_recursive(OctreeNode* node,const Pixel& color,int depth)
{
    if(depth == MAX_DEPTH) {
        node->is_leaf = true;
        node->color = color;
        return;
    }

    int index = OctreeNode::get_child_index(color,depth);
    if(!node->children[index]) {
        node->children[index] = std::make_unique<OctreeNode>();
    }

    insert_recursive(node->children[index].get(),color,depth + 1);
}

void ImgQuantize::ColorOctree::search_recursive(const OctreeNode* node,const Pixel& target,int depth,Pixel& best_match,double& min_dist_sq)
{
    if(!node)
        return;        
    
    if(node->is_leaf) {
        double dist_sq = target.distance_squared(node->color);
        if(dist_sq < min_dist_sq) {
            min_dist_sq = dist_sq;
            best_match = node->color;
        }
        return;
    }

    // 1. Prioritize the child node that matches the target's bit path
    int preferred_index = OctreeNode::get_child_index(target,depth);
    if(node->children[preferred_index]) {
        search_recursive(node->children[preferred_index].get(),target,depth + 1,best_match,min_dist_sq);
    }

    // 2. Check remaining branches (pruning can be added here based on bounding boxes)
    for(int i = 0; i < 8; ++i) {
        if(i == preferred_index || !node->children[i]) continue;

        search_recursive(node->children[i].get(),target,depth + 1,best_match,min_dist_sq);
    }
}



