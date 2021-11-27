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

#include "wx/dcbuffer.h"

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
		// sprite context (loaded independently from other sources)
		SpriteContext context;
		
		// void constructor
		Sprite();
		~Sprite();		
		void Render(uint8_t* buffer,uint8_t* buf_end,int buf_x,int buf_y, int x_size);
		void Render(uint8_t* buffer, uint8_t* buf_end, int buf_x, int buf_y, int x_size, uint8_t* filter);
		int Decode(uint8_t* data, char* name);
		void GetTileModel(TFxyz* vert, int* face=NULL, int* face_count=NULL);
		int GetTileEdge(int edge,TFxyz* vert);
		TFxyz ProjectVertex(TFxyz *vert);
		void RenderTileWires(uint8_t* buffer, uint8_t* buf_end, int buf_x, int buf_y, int x_size, uint8_t color);
		char GetSlope();
		
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



class SpriteContext
{
public:
	SpriteContext();
	~SpriteContext();

	int ReserveContext(int quadrant,int size);
	int AddContext(int quadrant, Sprite *sprite, bool no_check=false);
	int GetContextCount(int quadrant);
	Sprite *GetContext(int quadrant,int index);
	uint32_t SetFlags(uint32_t new_flags);
	uint32_t GetFlags();
	void SetEdgeClass(int edge, uint32_t new_class);
	uint32_t GetEdgeClass(int edge);
	uint32_t SetShadingFlags(uint32_t new_flags);
	uint32_t OrShadingFlags(uint32_t new_flags);
	uint32_t ClrShadingFlags(uint32_t new_flags);
	uint32_t GetShadingFlags();

	static constexpr int MODE_SET = 0;
	static constexpr int MODE_ADD = 1;
	static constexpr int MODE_CLR = 2;
	
	static constexpr uint32_t IS_GRASS =       0x00000001;
	static constexpr uint32_t IS_DGRASS =      0x00000002;
	static constexpr uint32_t IS_BLOOD =       0x00000004;
	static constexpr uint32_t IS_MUD =         0x00000008;
	static constexpr uint32_t IS_SWAPM =       0x00000010;
	static constexpr uint32_t IS_ASH =         0x00000020;
	static constexpr uint32_t IS_HIGHLAND =    0x00000040;
	static constexpr uint32_t IS_ROAD =        0x00000080;
	static constexpr uint32_t IS_BROKE_ROAD =  0x00000100;
	static constexpr uint32_t IS_DIRT_ROAD =   0x00000200;
	static constexpr uint32_t IS_MUD_PATH =    0x00000400;
	static constexpr uint32_t IS_CLIFF =       0x00000800;
	static constexpr uint32_t IS_WATER =       0x00001000;
	static constexpr uint32_t IS_WOOD_BRIDGE = 0x00002000;
	static constexpr uint32_t IS_BRIDGE =      0x00004000;
	static constexpr uint32_t IS_FORD =        0x00008000;
	static constexpr uint32_t IS_SAND =        0x00010000;

	static constexpr uint32_t CLASS_GENERIC = 0;
	static constexpr uint32_t CLASS_GRASS = 1;
	static constexpr uint32_t CLASS_DARK_GRASS = 2;
	static constexpr uint32_t CLASS_MUD = 3;
	static constexpr uint32_t CLASS_SWAMP = 4;
	static constexpr uint32_t CLASS_ASH = 5;
	static constexpr uint32_t CLASS_BLOOD = 6;
	static constexpr uint32_t CLASS_HIGH_LAND = 7;
	static constexpr uint32_t CLASS_SAND = 8;

	static constexpr uint32_t SHADING_SIDE_Q1 = 0x01;
	static constexpr uint32_t SHADING_SIDE_Q2 = 0x02;
	static constexpr uint32_t SHADING_SIDE_Q3 = 0x04;
	static constexpr uint32_t SHADING_SIDE_Q4 = 0x08;
	static constexpr uint32_t SHADING_CORNER_Q1 = 0x10;
	static constexpr uint32_t SHADING_CORNER_Q2 = 0x20;
	static constexpr uint32_t SHADING_CORNER_Q3 = 0x40;
	static constexpr uint32_t SHADING_CORNER_Q4 = 0x80;

private:
	// quadrant lists of allowed tile neighbors
	vector<Sprite*> quad[4];
	uint32_t shading;
	uint32_t flags;
	uint32_t edge_class[4];
};

class Terrain
{
private:
	//double gamma;
	double last_gamma;
	uint8_t gamma_pal[256][3];

	wstring context_path;

public:
	// terrain name
	char name[MAX_STR];
	// sprites of particular layers
	vector<Sprite*> sprites;
	// sprites context
	SpriteContext* context;
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

	enum WildMode { FIRST,RANDOM };

	// void contructor
	Terrain();
	~Terrain();
	int Load(wstring &path);
	Sprite* GetSprite(const char* name);
	Sprite* GetSprite(int index);
	int GetSpriteID(Sprite *spr);
	int GetSpriteID(const char* name);
	int GetSpriteCount();
	Sprite* GetSpriteWild(const char* wild,WildMode mode);
	AnimL1* GetANM(const char* name);
	AnimPNM* GetPNM(const char* name);
	
	int InitSpriteContext(wstring& path);
	int SaveSpriteContext(wstring& path);
	wstring &GetSpriteContextPath();
	int UpdateSpriteContext();
	int InitSpriteContextShading();

	int RenderPreview(wxBitmap& bmp,int count,int* data,int flags,double gamma);

	


	static constexpr int RENDER_CENTER = 0x01;
	static constexpr int RENDER_ZOOMX2 = 0x02;
};

