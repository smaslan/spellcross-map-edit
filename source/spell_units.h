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
#include "spell_sound.h"
#include <vector>
#include <tuple>

//using namespace std;

class SpellUnitRec
{
	public:
		// name
		char name[28];

		// attack
		int attack_light;
		int attack_armored;
		int attack_air;
		int attack_objects;
		int defence;

		// range
		int fire_range;
		int fire_flags;
		static constexpr int FIRE_HIGH_TURRET = 0x01;
		static constexpr int FIRE_INDIRECT = 0x02;
		static constexpr int FIRE_STEAL_AP = 0x04;
		static constexpr int FIRE_INEFFICIENT_TO_ARMORRED = 0x08;
		static constexpr int FIRE_NOT_SLOPES = 0x10;
		static constexpr int FIRE_MLRS = 0x20;
		static constexpr int FIRE_FIRE_PROJECTILE = 0x40;
		static constexpr int FIRE_FIRE_SENSITIVE = 0x80;
		static constexpr int FIRE_FIRE_HEALED = 0x100;

		int isIndirectFire() {return(fire_flags & FIRE_INDIRECT || fire_flags & FIRE_MLRS);}; // any indirect shot


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
		int snd_action_id;
		int ssel;

		SpellSound* sound_move;
		SpellSound* sound_report;
		SpellSound* sound_contact;		
		SpellSound* sound_hit;
		SpellSound* sound_die;
		SpellSound* sound_action;
		SpellAttackSound* sound_attack_light;
		SpellAttackSound* sound_attack_armor;
		SpellAttackSound* sound_attack_air;

		// animations/graphics
		char info[9];
		char gra[6];
		char grb[6];
		char icon[9];
		char anim_atack_light_name[6];
		char anim_atack_armor_name[6];
		char anim_atack_air_name[6];
		int anim_atack_light_frames;
		int anim_atack_armor_frames;
		int anim_atack_air_frames;
		char pnm_light_hit_name[9];
		char pnm_armored_hit_name[9];
		char pnm_air_hit_name[9];
		char pnm_light_shot_name[9];
		char pnm_armored_shot_name[9];
		char pnm_air_shot_name[9];
		char projetile_name[13];
		int projectile_visible;
		int vis;

		// other
		int utype;
		int res1;
		int res2;
		int res3;

		// special actions
		int action_id;
		char action_fsu_name[6];
		int action_fsu_frames;
		int action_ap;
		int action_params[3];

		// die action
		int daid;
		char die_anim_name[6];
		int die_anim_frames;
		int dap1;
		int dap2;
		int dap3;

		int GetHP() {return((cnt==1)?(100):(cnt));};

		// FSU data links
		FSU_resource* gr_base; /* unit base graphics */
		FSU_resource* gr_aux; /* unit aux graphics (turret for tanks) */
		FSU_resource* gr_attack_light; /* light unit attack animation */
		FSU_resource* gr_attack_armor; /* armored unit attack animation */
		FSU_resource* gr_attack_air; /* air unit attack animation */
		FSU_resource* gr_action; /* unit action graphics */
		FSU_resource* gr_die; /* unit die graphics */
		// icon link
		SpellGraphicItem *icon_glyph;
		// projectile link
		SpellProjectile* projectile;
		// PNM shot/hit
		AnimPNM *pnm_light_hit;
		AnimPNM *pnm_armored_hit;
		AnimPNM *pnm_air_hit;
		AnimPNM *pnm_light_shot;
		AnimPNM *pnm_armored_shot;
		AnimPNM *pnm_air_shot;
		

		SpellUnitRec();
		~SpellUnitRec();
		tuple<int,int> Render(uint8_t* buffer, uint8_t* buf_end, int buf_x_pos, int buf_y_pos, int buf_x_size,
			uint8_t* filter,uint8_t* shadow_filter, Sprite *sprt, int azim,int azim_turret, int frame,FSU_resource* fsu_anim=NULL, int flight_alt=100);

		vector<string> GetArtList(FSarchive* info_fs);
		int GetArtCount(FSarchive* info_fs);
		//wxBitmap *RenderArt(FSarchive *info, string &name, int surf_x, int surf_y);
		

		int isAir();
		int isLight();
		int isArmored();
		int isLand();
		int hasTurret();
		int isWalk();
		int isHover();
		int isFly();
		int isTank();
		int isMLRS();
		int hasProjectile(SpellUnitRec *target);
		int usingTeleportMove();

		int canAttack(SpellUnitRec* target);
		int canAttackObject();
		
		
		SpellGraphicItem* action_button_glyph;
		SpellGraphicItem* action_button_glyph_b;
		SpellGraphicItem* GetActionBtnGlyph(int alt=0);
		int hasSpecAction();
		int isActionTurretUp();
		int isActionTurretDown();
		int isActionToggleRadar();
		int isActionLand();
		int isActionTakeOff();
		int isActionToFortres();
		int isActionFromFortres();
		int isActionCreateUnit();
		int isActionKamikaze();

		// air unit height in projected space (pixels)
		static constexpr int AIR_UNIT_FLY_HEIGHT = 150;

		static constexpr double FIRE_RING_DIAMETER = 40.0;

private:

	// special actions
	static constexpr int SPEC_ACT_TOGGLE_RADAR = 1; // 1  - enable/disable radar (par3-radar indirect sight range)
	static constexpr int SPEC_ACT_SHOW_TURRET = 2; // 2  - show tank turret (UDES) (par3-unit to transform to)
	static constexpr int SPEC_ACT_HIDE_TURRET = 3; // 3  - hide tank turret (UDES) (par3-unit to transform to)
	static constexpr int SPEC_ACT_FIRE_TELEPORT = 4; // 4  - fire teleport movement (hell cavalery/demon)
	static constexpr int SPEC_ACT_CREATE_UNIT = 5; // 5  - create unit (par3-unit to create)
	static constexpr int SPEC_ACT_LOWER_MORALE = 6; // 6  - lower enemy morale (undead) (par1-range, par2-level, par3-range)
	static constexpr int SPEC_ACT_AIRCRAFT_UP = 7; // 7  - aircraft up (par3-unit to transform to)
	static constexpr int SPEC_ACT_AIRCRAFT_DOWN = 8; // 8  - aircraft land (par3-unit to transform to)
	static constexpr int SPEC_ACT_PARALYZE = 9; // 9  - paralyze enemy (harpya) (par1-range, par2-???, par3-???)
	static constexpr int SPEC_ACT_FREEZE = 11; // 11 - freeze enemy units (par1-range, par2-???, par3-???)
	static constexpr int SPEC_ACT_DRAGON_FEAR = 12; // 12 - dragons fear (par1-range, par2-???, par3-???)
	static constexpr int SPEC_ACT_AUTODESTRUCT = 13; // 13 - autodestruction (par1-range, par2-intensity, par3-???)
	static constexpr int SPEC_ACT_BREORN_SCREAM = 14; // 14 - breorns scream (par1-range, par2-???, par3-???)
	static constexpr int SPEC_ACT_KAMIZAZE = 15; // 15 - kamize attack
	static constexpr int SPEC_ACT_TRANSFORM_TO_FORTRES = 16; // 16 - transform to fortres (par3-unit to transform to)
	static constexpr int SPEC_ACT_TRANSFORM_FROM_FORTRES = 17; // 17 - transform from fortres (par3-unit to transfrom to)

	// multiple man in unit placement diameter in tile (pixels)
	static constexpr double MAN_RING_DIAMETER = 15.0;
	
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

	static constexpr int PROJECTILE_LIGHT = 0x01;
	static constexpr int PROJECTILE_ARMOR = 0x02;
	static constexpr int PROJECTILE_AIR = 0x04;

};

class SpellUnits
{
private:
	int is_eng;
	vector<SpellUnitRec*> units;

public:	
	SpellUnits(uint8_t* data, int dlen, FSUarchive *fsu, SpellGraphics *graphics,SpellSounds* sounds);
	~SpellUnits();
	int Count();
	SpellUnitRec* GetUnit(int uid);
	vector<SpellUnitRec*> &GetUnits();
};

