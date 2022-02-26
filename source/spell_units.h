//=============================================================================
// Loader of Spellcross units definition file JEDNOTKY.DEF.
// Loads CZ or EN version from binary data input.
// Decoders binary to list of unit records.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#pragma once

#include "fsu_archive.h"
#include "sprites.h"
#include "spell_graphics.h"
#include <vector>
#include <tuple>

//using namespace std;

class SpellUnitRec
{
	public:
		// name
		char name[28];

		// attack
		int alig;
		int aarm;
		int aair;
		int aobj;
		int def;

		// range
		int rmax;
		int rsfl;

		// sight
		int sdir;
		int sind;

		// count
		int cnt;

		// action points
		int ap;
		int aps;
		int apw;
		int apfw;

		// dig
		int dig;

		// rank
		int ramin;
		int ramax;

		// sound classes
		int smov;
		int slig;
		int sarm;
		int sair;
		int shit;
		int sspc;
		int ssel;

		// animations/graphics
		char info[9];
		char gra[6];
		char grb[6];
		char icon[9];
		char anlig[6];
		char anarm[6];
		char anair[6];
		int anligf;
		int anarmf;
		int anairf;
		char anhlig[9];
		char anharm[9];
		char anhair[9];
		char anslig[9];
		char ansarm[9];
		char ansair[9];
		char prj[13];
		int prjf;
		int vis;

		// other
		int utype;
		int res1;
		int res2;
		int res3;

		// special actions
		int said;
		char saan[6];
		int saanf;
		int saap;
		int sap1;
		int sap2;
		int sap3;

		// die action
		int daid;
		char daan[6];
		int daanf;
		int dap1;
		int dap2;
		int dap3;

		// FSU data links
		FSU_resource* gr_base; /* unit base graphics */
		FSU_resource* gr_aux; /* unit aux graphics (turret for tanks) */
		// icon link
		SpellGraphicItem *icon_glyph;

		SpellUnitRec();
		tuple<int,int> Render(uint8_t* buffer, uint8_t* buf_end, int buf_x_pos, int buf_y_pos, int buf_x_size,
			uint8_t* filter,uint8_t* shadow_filter, Sprite *sprt, int azim, int frame);
		

		int isAir();
		int isLight();
		int isHeavy();
		int hasTurret();
		int isWalk();
		int isHover();
		int isFly();
		int isTank();

private:

	// multiple man in unit placement diameter in tile (pixels)
	static constexpr double MAN_RING_DIAMETER = 15.0;

	// air unit height in projected space (pixels)
	static constexpr int AIR_UNIT_FLY_HEIGHT = 100;

	static constexpr int UTYPE_TYPE_MASK = 0x30;
	static constexpr int UTYPE_TYPE_AIR = 0x00;
	static constexpr int UTYPE_TYPE_LIGHT = 0x10;
	static constexpr int UTYPE_TYPE_ARMORED = 0x20;
	static constexpr int UTYPE_TURRET = 0x01;
	static constexpr int UTYPE_WALK = 0x02;
	static constexpr int UTYPE_FLY = 0x04;
	static constexpr int UTYPE_HOVER = 0x08;
	static constexpr int UTYPE_FLAHS = 0x40;
	static constexpr int UTYPE_DEMON = 0x80;


	
};

class SpellUnits
{
private:
	int is_eng;
	vector<SpellUnitRec*> units;

public:	
	SpellUnits(uint8_t* data, int dlen, FSUarchive *fsu, SpellGraphics *graphics);
	~SpellUnits();
	int Count();
	SpellUnitRec* GetUnit(int uid);
};

