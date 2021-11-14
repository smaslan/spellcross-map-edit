//=============================================================================
// Unsorted Spellcross data handling routines.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#pragma once

#include "windows.h"
#include "cstdint"
#include <vector>
#include <list>
#include <string>
#include "fs_archive.h"
#include "fsu_archive.h"

using namespace std;



#define MAX_STR 256
#define MAX_SPRITE_NAME 8

typedef struct {
	double x, y, z;
}TFxyz;

typedef struct {
	int x, y;
}Txy;


class Sprite
{
	public:
		// sprite name tag
		char name[MAX_SPRITE_NAME + 1];
		// sprite data
		uint8_t* data;
		// sprite size
		int x_size;
		int y_size;
		// sprite offset
		int y_ofs;
		int x_ofs;
		// land type
		int land_type;
		// has transparency
		int has_transp;

		// void constructor
		Sprite();
		~Sprite();		
		void Render(uint8_t* buffer,uint8_t* buf_end,int buf_x,int buf_y, int x_size);
		void Render(uint8_t* buffer, uint8_t* buf_end, int buf_x, int buf_y, int x_size, uint8_t* filter);
		int Decode(uint8_t* data, char* name);
		void GetTileModel(TFxyz* vert, int* face, int* face_count);
		TFxyz ProjectVertex(TFxyz *vert);
		void RenderTileWires(uint8_t* buffer, uint8_t* buf_end, int buf_x, int buf_y, int x_size, uint8_t color);
		
		double GetTileZ(double x, double y);
		double GetTileProjY(double x, double y);

	private:
		int MaskHasTransp(uint8_t* mask);

		// tile elevation step in pixels
		static constexpr double TILE_ELEVATION = 22.5;
		// tile projection angle (degrees)
		static constexpr double PROJECTION_ANGLE = 37.0;


};

class AnimL1
{
	public:
		// animation name
		char name[MAX_SPRITE_NAME + 1];
		// frames list
		vector<Sprite*> frames;
		// sprite size
		int x_size;
		int y_size;
		// sprite vertical offset
		int y_ofs;

		// void constructor
		AnimL1();
		~AnimL1();
		int Decode(uint8_t* data, char* name);

};

class AnimPNM
{
	public:
		// animation name
		char name[MAX_SPRITE_NAME + 1];
		// frames list
		vector<Sprite*> frames;
		// sprite data limits relative to origin defined by frames
		int x_min;
		int x_max;
		int y_min;
		int y_max;
								
		// void constructor
		AnimPNM();
		~AnimPNM();
		int Decode(uint8_t* data, char* name);

};

class Terrain
{
	public:
		// terrain name
		char name[MAX_STR];
		// sprites of particular layers
		vector<Sprite*> sprites;
		// Layer 1 animations (ANM)
		vector<AnimL1*> anms;
		// Layer 4 animations (PNM)
		vector<AnimPNM*> pnms;
		// color palette
		uint8_t pal[256][3];
		// filters
		struct {
			uint8_t darkpal[256];
			uint8_t darkpal2[256];
			uint8_t darker[256];
			uint8_t bluepal[256];
			uint8_t dbluepal[256];
			uint8_t redpal[256];
		} filter; 

		// void contructor
		Terrain();
		~Terrain();
		int Load(wstring &path);
		Sprite* GetSprite(const char* name);
		AnimL1* GetANM(const char* name);
		AnimPNM* GetPNM(const char* name);
};

