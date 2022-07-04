//=============================================================================
// Unsorted Spellcross data handling routines.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
//#pragma once
#ifndef _SPRITES_H_
#define _SPRITES_H_

//#include "windows.h"
#include "cstdint"
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <thread>

#include "fs_archive.h"
#include "fsu_archive.h"
#include "map_types.h"
#include "spell_font.h"
#include "spell_filter.h"
#include "spell_sound.h"
#include "spell_graphics.h"

#include "wx/dcbuffer.h"


#define MAX_STR 256
#define MAX_SPRITE_NAME 8


class SpellL2classRec
{
public:
	std::string tag;
	std::string label;
	std::string name;
	int index;
	int defence;
	int hp;
	int sound_class_id;
	SpellSound* sound_hit;
	SpellSound* sound_destruct;
	SpellL2classRec();
	~SpellL2classRec();
};

class DestructibleRec
{
public:
	DestructibleRec();
	SpellL2classRec* destructible;
	std::string pnm_name;
	int target;
	int destructed;
	int two_stage_desctruct;
	std::string alt_name;
};

class SpellL2classes
{
private:
	std::vector<SpellL2classRec*> wall_list;
	std::vector<SpellL2classRec*> bridge_list;
	std::vector<SpellL2classRec*> spec_list;
public:
	SpellL2classes(FSarchive* fs,SpellSounds* sounds,std::function<void(std::string)> status_list=NULL,std::function<void(std::string)> status_item=NULL);
	~SpellL2classes();
	DestructibleRec GetClass(const char *sprite_name);
};




typedef struct {
	double x, y, z;
}TFxyz;

typedef struct {
	int x, y;
}Txy;

class Txyz {
public:
	int x,y,z;
	Txyz(int xx,int yy,int zz) {x=xx;y=yy;z=zz;};
};


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
		// sprite index in terrain
		int index;
		// destructible parameters (or NULL)
		SpellL2classRec *destructible;
		// alternative-paired sprite when destructible
		Sprite *destructible_alt;
		int is_destructed;
		int two_stage_desctruct;
		int is_target;
		AnimPNM *destroy_pnm;
		
		// void constructor
		Sprite();
		~Sprite();		
		void SetIndex(int id);
		int GetIndex();
		void Render(uint8_t* buffer,uint8_t* buf_end,int buf_x,int buf_y, int x_size);
		void Render(uint8_t* buffer, uint8_t* buf_end, int buf_x, int buf_y, int x_size, uint8_t* filter);
		wxBitmap* Render(uint8_t* pal,double gamma=1.3, int x_size=-1, int y_size=-1, bool no_zoom=true);
		int Decode(uint8_t* data, char* name);
		void GetTileModel(TFxyz* vert, int* face=NULL, int* face_count=NULL);
		int GetTileEdge(int edge,TFxyz* vert);
		TFxyz ProjectVertex(TFxyz *vert);
		void RenderTileWires(uint8_t* buffer, uint8_t* buf_end, int buf_x, int buf_y, int x_size, uint8_t color);
		char GetSlope();
		
		double GetTileZ(double x, double y);
		double GetTileProjY(double x, double y);

		int CompareSpriteContextAlt(Sprite *alt);
		int CheckShadingValid(Sprite* neig,int eid);
		int CheckNeighborValid(Sprite* neig,int eid);

		// sprite context stuff
		int ReserveContext(int quadrant,int tile,int size);
		int AddContext(int quadrant,Sprite* sprite,bool no_check=false);
		int GetContextCount(int quadrant);
		int GetContextCount(int quadrant, int tile);
		int GetContextCount(int quadrant,int tile,int random);
		int GetContextCountRng(int quadrant,int tile);
		Sprite* GetContext(int quadrant,int index);
		Sprite* GetContext(int quadrant,int tile,int index);		
		Sprite* GetContext(int quadrant,int tile,int index,int random);
		Sprite* GetContextRng(int quadrant,int tile,int index);
		void RemoveContext(int quadrant,int tile,vector<int> &list);
		void RemoveContext(int quadrant,vector<int>& list);
		int CheckContext(int quadrant,Sprite *sprite);
		int ClearContext(int quadrant);
		int RandomizeContext();
		int ShuffleContext();
		uint32_t SetFlags(uint32_t new_flags);
		uint32_t GetFlags();
		uint32_t SetGlyphFlags(uint32_t new_flags);
		uint32_t GetGlyphFlags();
		void SetEdgeClass(int edge,uint32_t new_class);
		uint32_t GetEdgeClass(int edge);
		uint32_t SetShadingFlags(uint32_t new_flags);
		uint32_t GetShadingFlags();
		uint32_t SetShadingMask(uint32_t mask);
		uint32_t GetShadingMask();
		bool isClassGlyph() { return(!!(flags2 & IS_GLYPH));};
		bool isSideShaded(int side);
		bool isSideShadedMask(int side);
		bool isCornerShaded(int corner);

		int GetSpecClassCount();
		std::string GetSpecClassName(int id);
		void SetSpecClass(uint32_t class_id);
		uint32_t GetSpecClass();

		void SetToolClass(int id);
		void SetToolClassGroup(int id);
		int GetToolClass();
		int GetToolClassGroup();


		static constexpr int MODE_SET = 0;
		static constexpr int MODE_ADD = 1;
		static constexpr int MODE_CLR = 2;

		static constexpr uint32_t IS_GRASS =        0x00000001;
		static constexpr uint32_t IS_DGRASS =       0x00000002;
		static constexpr uint32_t IS_BLOOD =        0x00000004;
		static constexpr uint32_t IS_MUD =          0x00000008;
		static constexpr uint32_t IS_SWAPM =        0x00000010;
		static constexpr uint32_t IS_ASH =          0x00000020;
		static constexpr uint32_t IS_HIGHLAND =     0x00000040;
		static constexpr uint32_t IS_ROAD =         0x00000080;
		static constexpr uint32_t IS_BROKEN =       0x00000100;
		static constexpr uint32_t IS_DIRT_ROAD =    0x00000200;
		static constexpr uint32_t IS_MUD_PATH =     0x00000400;
		static constexpr uint32_t IS_CLIFF =        0x00000800;
		static constexpr uint32_t IS_WATER =        0x00001000;
		static constexpr uint32_t IS_WOOD_BRIDGE =  0x00002000;
		static constexpr uint32_t IS_BRIDGE =       0x00004000;
		static constexpr uint32_t IS_FORD =         0x00008000;
		static constexpr uint32_t IS_SAND =         0x00010000;
		static constexpr uint32_t IS_SHADOW =       0x00020000;
		static constexpr uint32_t IS_RIDGE =        0x00040000;
		static constexpr uint32_t IS_RIDGE_BRIDGE = 0x00080000;
		static constexpr uint32_t IS_WALL_BASE =    0x00100000;
		static constexpr uint32_t IS_OBJECT =       0x00200000;
		static constexpr uint32_t IS_SCAR =         0x00400000;
		
		static constexpr uint32_t IS_GLYPH =           0x80000000; /* use tile as glyph for general tile class display */
		static constexpr uint32_t IS_TOOL_ITEM_GLYPH = 0x40000000; /* use tile as glyph for tool item */
		static constexpr uint32_t IS_FAULTY =          0x20000000; /* sprite known to have faulty context in existing maps */
		static constexpr uint32_t Q1_NOFILT =          0x01000000; /* not filter sprite context in Q1 */
		static constexpr uint32_t Q2_NOFILT =          0x02000000; /* not filter sprite context in Q2 */
		static constexpr uint32_t Q3_NOFILT =          0x04000000; /* not filter sprite context in Q3 */
		static constexpr uint32_t Q4_NOFILT =          0x08000000; /* not filter sprite context in Q4 */

		// special tile classes
		static constexpr uint32_t SPEC_CLASS_NONE = 0;
		static constexpr uint32_t SPEC_CLASS_GRAVE = 1; /* graves of any kind */
		static constexpr uint32_t SPEC_CLASS_RUBBLE1 = 2; /* light rubble */
		static constexpr uint32_t SPEC_CLASS_RUBBLE2 = 3; /* average dense rubble */
		static constexpr uint32_t SPEC_CLASS_RUBBLE3 = 4; /* more dense rubble */
		static constexpr uint32_t SPEC_CLASS_ROCK = 5; /* big rock */
		static constexpr uint32_t SPEC_CLASS_FIREPLACE = 6; /* fireplaces */
		static constexpr uint32_t SPEC_CLASS_SCAR = 7; /* generic texture stuff on the basic ground like scars, high grass and stuff */
		static constexpr uint32_t SPEC_CLASS_OBJECT = 8; /* like buildings, etc. */		
		static constexpr uint32_t SPEC_CLASS_MAX = SPEC_CLASS_OBJECT; /* highest class index */

		static constexpr uint32_t CLASS_GENERIC = 0;
		static constexpr uint32_t CLASS_GRASS = 1;
		static constexpr uint32_t CLASS_DARK_GRASS = 2;
		static constexpr uint32_t CLASS_MUD = 3;
		static constexpr uint32_t CLASS_SWAMP = 4;
		static constexpr uint32_t CLASS_ASH = 5;
		static constexpr uint32_t CLASS_BLOOD = 6;
		static constexpr uint32_t CLASS_HIGH_LAND = 7;
		static constexpr uint32_t CLASS_SAND = 8;
		static constexpr uint32_t CLASS_ROAD = 9;
		static constexpr uint32_t CLASS_DIRT_ROAD = 10;
		static constexpr uint32_t CLASS_MUD_PATH = 11;

		static constexpr uint32_t SHADING_SIDE_Q1 = 0x01;
		static constexpr uint32_t SHADING_SIDE_Q2 = 0x02;
		static constexpr uint32_t SHADING_SIDE_Q3 = 0x04;
		static constexpr uint32_t SHADING_SIDE_Q4 = 0x08;
		static constexpr uint32_t SHADING_CORNER_Q1 = 0x10;
		static constexpr uint32_t SHADING_CORNER_Q2 = 0x20;
		static constexpr uint32_t SHADING_CORNER_Q3 = 0x40;
		static constexpr uint32_t SHADING_CORNER_Q4 = 0x80;

		enum {
			NEIGHBOR_INVALID = 0,
			NEIGHBOR_VALID = 1,
			NEIGHBOR_UNDECIDED = 2
		};

		// tile elevation step in pixels
		static constexpr double TILE_ELEVATION = 22.5;

		// tile projection angle (degrees)
		static constexpr double PROJECTION_ANGLE = 37.0;

	private:
		int MaskHasTransp(uint8_t* mask);
		
		

		// sprite context stuff:
		// quadrant lists of allowed tile neighbors [quadrant, tile_type]
		std::vector<Sprite*> quad[4]['M'-'A'+1];
		uint32_t shading;
		uint32_t flags;
		uint32_t flags2;
		uint32_t edge_class[4];
		uint32_t spec_class;

		// randomized context
		std::vector<std::vector<Sprite*>> quad_rng[4]['M'-'A'+1];

		// tool classes 
		uint32_t tool_class;
		uint32_t tool_group;
};



class AnimL1
{
	public:
		// animation name
		char name[MAX_SPRITE_NAME + 1];
		// frames list
		std::vector<Sprite*> frames;
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
		std::vector<Sprite*> frames;
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


// spellcross terrain object (group of sprites forming e.g. house)
class SpellObject
{
private:
	// item description
	std::string description;
	// sprite positions
	std::vector<MapXY> sprite_pos;
	// sprite pointers
	std::vector<Sprite*> L1_sprites;
	std::vector<Sprite*> L2_sprites;
	std::vector<uint8_t> flags;
	// bitmap data
	int surf_x;
	int surf_y;
	uint8_t* pic;
	uint8_t* pic_end;
	// tool classes 
	uint32_t tool_class;
	uint32_t tool_group;

	double last_gamma;
	uint8_t pal[256][3];
	uint8_t gamma_pal[256][3];

	int RenderObjectGlyph();

public:
	SpellObject(ifstream& fr,std::vector<Sprite*>& sprite_list,uint8_t* palette = NULL);
	SpellObject(std::vector<MapXY> xy,std::vector<Sprite*> L1_list,std::vector<Sprite*> L2_list,std::vector<uint8_t> flag_list, uint8_t *palette = NULL, std::string desc = "");
	~SpellObject();
	int RenderPreview(wxBitmap& bmp,double gamma=1.30);
	tuple<int, int> GetGlyphSize();
	wxBitmap *RenderPreview(double gamma = 1.30, int x_size = -1, int y_size = -1, bool no_zoom=true);
	int WriteToFile(ofstream& fw);	
	std::string GetDescription();
	void SetDescription(std::string name);
	int PlaceMapTiles(Sprite** L1,int x_size,int y_size,MapXY sel);

	void SetToolClass(int id);
	void SetToolClassGroup(int id);
	int GetToolClass();
	int GetToolClassGroup();
};


class SpellToolsGroup
{
private:
	
public:
	std::vector<std::string> items;
	std::string name;
	std::string title;
	int glyph_x;
	int glyph_y;
	int glyph_mode;

	static constexpr int SCALE_MEAN = 0;
	static constexpr int SCALE_MAX = 1;
	static constexpr int SCALE_MAX_NOZOOM = 2;
};


class Terrain
{
private:
	//double gamma;
	double last_gamma;
	uint8_t gamma_pal[256][3];
	// sprite context file path
	wstring context_path;
	// list of generic tile glyph pointers
	std::vector<Sprite *> glyphs[13];

	// tools list
	std::vector<SpellToolsGroup*> tools;

	void RandomizeSpriteContextTh(std::vector<Sprite*> *spr,int ofs,int step);

	//int CheckNeighborValid(Sprite *ref,Sprite *neig,int eid);	

public:
	// terrain name
	std::string name;
	// sprites of particular layers
	std::vector<Sprite*> sprites;
	// sprites context
	//SpriteContext* context;
	// Layer 1 animations (ANM)
	std::vector<AnimL1*> anms;
	// Layer 4 animations (PNM)
	std::vector<AnimPNM*> pnms;
	// terrain objects list
	std::vector<SpellObject*> objects;
	// color palette
	uint8_t pal[256][3];
	// filters
	SpellFilters filter;	
	// fonts
	SpellFont *font;
	SpellFont *font7;

	enum WildMode { FIRST,RANDOM };

	// void contructor
	Terrain();
	~Terrain();
	int Load(FSarchive *terrain_fs, uint8_t map_pal[][3], SpellGraphics *gres, SpellL2classes* L2=NULL,std::function<void(std::string)> status_item=NULL);
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
	std::wstring &GetSpriteContextPath();
	void ClearSpriteContext();
	int RandomizeSpriteContext();
	int ShuffleSpriteContext();
	int UpdateSpriteContext(std::function<void(std::string)> status_cb=NULL);
	int InitSpriteContextShading();
	int UpdateTileGlyphs();
	Sprite *GetTileGlyph(Sprite *sprite,uint32_t flags = 0);

	int RenderPreview(wxBitmap& bmp,int count,int* data,int flags,double gamma);
		
	int AddObject(std::vector<MapXY> xy,std::vector<Sprite*> L1_list,std::vector<Sprite*> L2_list,std::vector<uint8_t> flag_list,uint8_t* palette,std::string desc);
	int RemoveObject(int id);
	int MoveObject(int posa, int posb);
	int RenameObject(int id, string name);
	int GetObjectsCount();
	SpellObject *GetObject(int id);
	std::vector<SpellObject*> &GetObjects();

	int GetToolsCount();
	std::string GetToolSetName(int id);
	std::string GetToolSetTitle(int id);
	int SetToolSetName(int id,std::string name);
	int SetToolSetTitle(int id,std::string title);
	int GetToolSetGlyphScalingMode(int id);
	int SetToolSetGlyphScalingMode(int id, int mode);
	tuple<int, int> GetToolSetGlyphScaling(int id);
	int SetToolSetGlyphScaling(int id, int x, int y);
	int GetToolSetItemsCount(int id);
	std::vector<std::string> GetToolSetItems(int toolset_id);
	std::string GetToolSetItem(int toolset_id, int tool_id);
	int AddToolSetItem(int toolset_id,std::string item, int position = -1);
	int RenameToolSetItem(int toolset_id,std::string item, int position);
	int RemoveToolSetItem(int toolset_id, int position);
	int MoveToolSetItem(int toolset_id, int posa, int posb);
	int AddToolSet(std::string name,std::string title, int position=-1);
	int RemoveToolSet(int position);
	int MoveToolSet(int posa, int posb);
	int GetToolSetID(string& name);
	int GetToolSetID(const char *name);
	wxBitmap* RenderToolSetItemImage(int tool_id, int item_id, double gamma=1.30, int x_size=-1, int y_size=-1, bool no_zoom=true);
	tuple<int, int> GetToolSetItemImageSize(int tool_id, int item_id);

	int RenderPalette(wxBitmap& bmp,uint8_t* filter=NULL,int relative_time=0);
	int RenderPaletteColor(wxBitmap& bmp,int x_size,int x_pos,uint8_t* filter=NULL);
	

	static constexpr int RENDER_CENTER = 0x01;
	static constexpr int RENDER_ZOOMX2 = 0x02;
};


// editing tool selection pointer (can point to class/tools (sprite) or objects)
class SpellTool
{
private:
	SpellObject* obj;
	int class_id;
	int tool_id;
public:	
	SpellTool() {obj = NULL; class_id = -1; tool_id = -1;};
	void Set(int class_id, int tool_id) {this->class_id = class_id; this->tool_id = tool_id; this->obj = NULL;};
	void Set() { this->class_id = -1; this->tool_id = -1; this->obj = NULL; };
	void Set(SpellObject *obj) {this->class_id = -1; this->tool_id = -1; this->obj = obj;};
	bool isObject() {return(!!obj);};
	bool isTool() { return(class_id >=0 && tool_id >= 0); };
	bool isActive() { return(isObject() || isTool()); };
	SpellObject* GetObject() {return(obj);};
	std::tuple<int,int> GetTool() { return(std::make_tuple(class_id, tool_id)); };
};

#endif