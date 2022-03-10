#pragma once

#include "sprites.h"
#include <vector>
#include "cstdint"


using namespace std;

class SpellGraphicItem
{
public:
	int x_size;
	int y_size;
	int x_ofs;
	int y_ofs;
	char name[13];
	vector<uint8_t> pixels;
	uint8_t *pal;

	uint8_t *GetPixels(int y = 0, int x = 0);
	int Render(uint8_t *buf,uint8_t* buf_end,int buf_x_size,int x_pos,int y_pos);
	wxBitmap* Render(int x_size=-1,int y_size=-1,int transparent=false);
};

class SpellProjectile
{
public:
	char name[13];
	SpellGraphicItem* glyphs[16];
	SpellProjectile(SpellGraphicItem &glyph);
	int Insert(SpellGraphicItem &glyph);
	int Check();
	SpellGraphicItem* GetGlyph(double angle);
};

// generic graphic resource
class SpellGraphics
{
private:
	vector<SpellGraphicItem> items;
	vector<AnimPNM*> pnms;
	vector<SpellProjectile> projectiles;
public:
	SpellGraphics();
	~SpellGraphics();
	int AddRaw(uint8_t *data, int dlen, int x_size, int y_size, char *name,uint8_t *pal,int with_ext=0);
	int AddICO(uint8_t* data,int dlen,char* name,uint8_t* pal);
	int AddCUR(uint8_t* data,int dlen,char* name,uint8_t* pal);
	int AddLED(int color,const char* name,uint8_t* pal);
	int Count();
	SpellGraphicItem *GetResource(int index);
	SpellGraphicItem *GetResource(const char *name);

	// PNM animations	
	int AddPNM(uint8_t* data,int dlen,char* name);
	AnimPNM *GetPNM(char *name);

	// direct links to common items
	SpellGraphicItem *wm_hud;
	SpellGraphicItem *wm_hud_enemy;
	SpellGraphicItem *wm_hud_sides;
	SpellGraphicItem* wm_hud_overlay;
	SpellGraphicItem* wm_form[3];
	SpellGraphicItem* red_led_off;
	SpellGraphicItem* red_led_on;
	SpellGraphicItem* yellow_led_on;
	SpellGraphicItem* wm_exp_mark;
	SpellGraphicItem* wm_freeze;
	SpellGraphicItem* wm_paralyze;
	SpellGraphicItem* wm_btn_idle;
	SpellGraphicItem* wm_btn_hover;
	SpellGraphicItem* wm_btn_press;
	SpellGraphicItem* wm_glyph_air;
	SpellGraphicItem* wm_glyph_center_unit;
	SpellGraphicItem* wm_glyph_radar_down;
	SpellGraphicItem* wm_glyph_radar_up;
	SpellGraphicItem* wm_glyph_end_turn;
	SpellGraphicItem* wm_glyph_goto_unit;
	SpellGraphicItem* wm_glyph_ground;
	SpellGraphicItem* wm_glyph_map;
	SpellGraphicItem* wm_glyph_heal;
	SpellGraphicItem* wm_glyph_unit_info;
	SpellGraphicItem* wm_glyph_next;
	SpellGraphicItem* wm_glyph_options;
	SpellGraphicItem* wm_glyph_next_unused;
	SpellGraphicItem* wm_glyph_retreat;
	SpellGraphicItem* wm_glyph_end_placement;
	SpellGraphicItem* wm_glyph_info;

	// projectile lists
	int SortProjectiles();
	SpellProjectile *GetProjectile(char *name);
};
