//=============================================================================
// Spellcross MAP related routines: Loading, rendering.
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
#include <tuple>
#include "fs_archive.h"
#include "fsu_archive.h"
#include "spell_units.h"
#include "spellcross.h"

#include "wx/dcbuffer.h"

using namespace std;
//using namespace System;

#define MAX_STR 256
#define MAX_SPRITE_NAME 8

class MapLayer3
{
	public:
		// animation location in map
		int x_pos;
		int y_pos;
		int frame_ofs;
		int frame_limit;
		AnimL1* anim;

		MapLayer3(AnimL1* anm, int x_pos, int y_pos, int frame_ofs, int frame_limit);
		~MapLayer3();		
};

class MapLayer4
{
public:
	// animation location in map
	int x_pos;
	int y_pos;
	int x_ofs;
	int y_ofs;
	int frame_ofs;
	int frame_limit;
	AnimPNM* anim;

	MapLayer4(AnimPNM* pnm, int x_pos, int y_pos, int x_ofs, int y_ofs, int frame_ofs, int frame_limit);
	~MapLayer4();
};


class MapXY
{
public:
	int x;
	int y;
	bool IsSelected();
	MapXY() {x=0;y=0;};
};

class MapUnitType
{
public:
	enum Values : int
	{
		Unknown,
		PatrolUnit,
		WaitForContact,
		NormalUnit,
		ToughDefence
	};
	MapUnitType() = default;
	constexpr MapUnitType(Values type) : value(type) { }

	constexpr bool operator==(MapUnitType type) const { return value == type.value; }
	constexpr bool operator!=(MapUnitType type) const { return value != type.value; }
	void operator=(const char *type) {
		if (_strcmpi(type, "PatrolUnit") == 0)
			value = Values::NormalUnit;
		else if (_strcmpi(type, "WaitForContact") == 0)
			value = Values::WaitForContact;
		else if (_strcmpi(type, "NormalUnit") == 0)
			value = Values::NormalUnit;
		else if (_strcmpi(type, "ToughDefence") == 0)
			value = Values::ToughDefence;
		else
			value = Values::Unknown;
	}

private:
	Values value;
};

class MapUnit
{
public:
	// unit idnetifier index within map
	int id;
	// unit type ID
	int type_id;
	SpellUnitRec *unit;
	// position
	MapXY coor;
	// experience
	int experience;
	// man count
	int man;
	// unit type/behaviour
	MapUnitType type;
	// custom name
	char name[100];

	// pointer to next unit to draw (for correct render order)
	MapUnit* next;
	int assigned;

	// unit rendering state
	int azimuth;
	int azimuth_anim;
	int frame;

	int GetXY();
};

// Scroller
class TScroll
{
private:
	// cursor position (within screen)
	int xref,yref;
	// scroll position
	int dx,dy;
	// scroll state
	int state;
	// selection size
	int size;
	// modified?
	int modified;

	static constexpr int MAX_SEL_SIZE = 8;

public:	
	TScroll();
	void Reset();
	void SetRef(int x, int y);
	void Move(int x, int y);
	void Idle();
	int ResizeSelection(int delta);
	bool wasModified();
	bool Commit();
	
	tuple<int,int> CheckScroll(int x_limit,int y_limit);
	tuple<int,int> GetScroll();
	tuple<int,int> GetCursor();
	int GetSize();
};


typedef struct{
	int x;
	int y;
	int edir;
	int elev;
}TTileElevMod;

class SpellMap
{
	private:
		int surf_x;
		int surf_y;
		int last_surf_x;
		int last_surf_y;
		// indexed raster buffer		
		int pic_x_size;
		int pic_y_size;
		uint8_t* pic;
		uint8_t* pic_end; /* buffer end */
		// scroll and surface range in tiles
		int xs_size;
		int ys_size;
		int xs_ofs;
		int ys_ofs;
		// local palette (after gamma correction)
		uint8_t pal[256][3];
		// special tiles pointers
		Sprite* start_sprite;
		Sprite* escape_sprite;

		// tile selection
		MapXY sel;
		vector<MapXY> msel;

		// layer visibility flags
		bool wL1, wL2, wL3, wL4, wSTCI, wUnits;

		// last gamma
		double last_gamma;
		double gamma;

		static constexpr int MAP_BACK_COLOR = 230; // map background color index
		static constexpr int MSYOFS = 150; // map render Y-offset from top (this should make enough space for highest elevation)
		static constexpr int MSYOFST = 6; // map render Y-offset from top in tiles

		static constexpr int ELEV_MIN = 0; // minimum tile elevation
		static constexpr int ELEV_MAX = 8; // maximum tile elevation

		
		int GetFlagHeight(MapXY* sel);
		int GetFlagFlag(MapXY* sel);
		MapXY GetTileNeihgborXY(int x,int y,int angle);
		MapXY GetTileNeihgborXY(MapXY mxy,int angle);
		int EditElevNbrQuad(int x,int y,int elv,int* min);
		int EditElevNbrDbl(int x,int y,int elv);
		void EditElevNbrRule(uint8_t* flag,TTileElevMod* mod,int x,int y,int elv);
		void EditElevNbr(uint8_t* flag,int elv,int edir,int x,int y);
		void EditElevSlope(uint8_t* flag);
		void EditElevText(uint8_t* flag);

	public:

		static constexpr int SELECT_ADD = 1;
		static constexpr int SELECT_CLEAR = 2;
		static constexpr int SELECT_XOR = 3;

		// this map path
		wstring map_path;
		wstring def_path;
		// size
		int x_size;
		int y_size;
		// terrain
		char terrain_name[14];
		Terrain* terrain;
		SpellData* spelldata;
		// layers:
		Sprite **L1; // terrain array
		int* elev; // terrain elevation array
		Sprite **L2; // objects array
		uint8_t* flags; // flags array
		vector<MapLayer3*> L3; // ANM list
		vector<MapLayer4*> L4; // PNM list				
		vector<MapXY> start; // start tiles list
		vector<MapXY> escape; // escape tiles list
		MapUnit** Lunit; // units layer array
		uint8_t *select; // selection flags array
		// list of units
		vector<MapUnit*> units;

		
		SpellMap();
		~SpellMap();
		void Close();
		int Load(wstring &path, SpellData* spelldata);
		void SortUnits();
		int IsLoaded();
		bool TileIsVisible(int x, int y);
		
		MapXY GetSelection(wxBitmap& bmp,TScroll* scroll);
		vector<MapXY> &GetSelections(wxBitmap& bmp, TScroll* scroll);
		void SelectTiles(vector<MapXY> tiles,int mode);
		void SelectTiles(int mode);
		int RenderPrepare(wxBitmap& bmp, TScroll* scroll);
		int Render(wxBitmap &bmp, TScroll* scroll);

		void SetRender(bool wL1, bool wL2, bool wL3, bool wL4, bool wSECI, bool wUnits);
		void SetGamma(double gamma);
		int Tick();
		wstring GetTopPath();
		
		int ConvXY(int x, int y);
		int ConvXY(MapXY &mxy);
		int ConvXY(MapXY *mxy);
			
		char *GetL1tileName(wxBitmap& bmp,TScroll* scroll);
		char* GetL2tileName(wxBitmap& bmp,TScroll* scroll);
		tuple<int,int,int> GetTileFlags(wxBitmap& bmp,TScroll* scroll);

		int EditElev(wxBitmap& bmp,TScroll* scroll,int step);

		MapXY GetNeighborTile(int x,int y,int quad);
		int BuildSpriteContext();
};





