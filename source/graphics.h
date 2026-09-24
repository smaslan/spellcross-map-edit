#pragma once

#include <cstdint>
#include <vector>
#include <memory>

class ImgQuantize{

public:

    class Pixel
    {
    public:
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;

        double distance_squared(const Pixel& other) const;
        bool isBlack() const;
        bool isTransparent() const;
    };
    
    static std::vector<Pixel> GenMedianCutPalette(std::vector<Pixel> &pixels,int targetColors);

private:

    class Bucket
    {
    public:
        std::vector<Pixel>::iterator start;
        std::vector<Pixel>::iterator end;
        uint8_t minR,maxR,minG,maxG,minB,maxB;

        // make bucket from pixels
        Bucket(std::vector<Pixel>::iterator s,std::vector<Pixel>::iterator e);
        int getLongestSide() const;
    };

    // Node structure for the Octree
    class OctreeNode
    {
    public:
        bool is_leaf = false;
        Pixel color = {0, 0, 0}; // Valid only if is_leaf is true
        std::unique_ptr<OctreeNode> children[8] = {nullptr};

        // Determines which of the 8 children a color belongs to at a specific bit depth
        static int get_child_index(const Pixel& cls,int depth);
    };

    class ColorOctree
    {
    private:
        std::unique_ptr<OctreeNode> root;
        static const int MAX_DEPTH = 8; // Max depth for 8-bit color channels

        void insert_recursive(OctreeNode* node,const Pixel& color, int depth);        
        void search_recursive(const OctreeNode* node,const Pixel& target,int depth,Pixel& best_match,double& min_dist_sq);
    };
    
};
