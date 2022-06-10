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

// forward refs
class SpellMapEventRec;
class MapSprite;


class UnitBonus
{
public:
	int level;
	int defence;
	int attack;
	int attack_count;
	int move;

	UnitBonus();
};

class UnitBonuses
{
private:
	vector<UnitBonus> list;

public:
	UnitBonuses(string bonuses_def);
	UnitBonus* GetBonus(int level);
};



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
		int dig_turns;

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

		// bonus levels link
		UnitBonuses *bonuses;
		

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
		int hasProjectile(SpellUnitRec *target=NULL);
		int usingTeleportMove();
		int isFlashAndBones();
		int isMetal();

		int canAttack(SpellUnitRec* target);
		int canAttackObject();
		int GetMaxDig();		
		
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
	SpellUnits(uint8_t* data, int dlen, FSUarchive *fsu, SpellGraphics *graphics,SpellSounds* sounds,UnitBonuses *bonuses);
	~SpellUnits();
	int Count();
	SpellUnitRec* GetUnit(int uid);
	vector<SpellUnitRec*> &GetUnits();
};




class AStarNode
{
public:
	MapXY pos;
	MapXY parent_pos;
	int g_cost;
	int h_cost;
	int f_cost;
	int closed;
	AStarNode();
	static constexpr int INIT_COST = 1<<30;
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
		ToughDefence,
		SpecUnit,
		MissionUnit
	};
	MapUnitType() = default;
	constexpr MapUnitType(Values type) : value(type) { }

	constexpr bool operator==(MapUnitType type) const { return value == type.value; }
	constexpr bool operator!=(MapUnitType type) const { return value != type.value; }
	void operator=(const char* type) {
		if(_strcmpi(type,"PatrolUnit") == 0)
			value = Values::NormalUnit;
		else if(_strcmpi(type,"WaitForContact") == 0)
			value = Values::WaitForContact;
		else if(_strcmpi(type,"NormalUnit") == 0)
			value = Values::NormalUnit;
		else if(_strcmpi(type,"ToughDefence") == 0)
			value = Values::ToughDefence;
		else if(_strcmpi(type,"MissionUnit") == 0)
			value = Values::MissionUnit;
		else if(_strcmpi(type,"SpecUnit1") == 0)
			value = Values::SpecUnit;
		else if(_strcmpi(type,"SpecUnit2") == 0)
			value = Values::SpecUnit;
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
	SpellUnitRec* unit;
	// position
	MapXY coor;
	// experience
	int experience;
	// man count (health)
	int man;
	int wounded;
	// unit type/behaviour
	MapUnitType type;
	// custom name
	char name[100];
	// commander id or zero	
	int commander_id;
	int is_commander;
	// dig in
	int dig_level;
	// dig turns counter
	int dig_turns;
	// turns count since last action
	int idle_turns;
	// action points
	int action_points;
	// unit active (set except insertion time)
	int is_active;
	// unit hide (used temporarily when destroying)?
	int hide;
	// enemy?
	int is_enemy;
	// event created?
	int is_event;
	// morale level
	int morale;
	// unit in placement (selected and moving)
	int in_placement;
	// unit moved flag (cleared when rendered)
	int was_moved;
	// unit was already seen?
	int was_seen;
	// unit visible level (this is used for in-game checking of the unit in view range)
	int is_visible;


	// pointer to next unit to draw (for correct render order)
	MapUnit* next;
	int assigned;

	// link s between parent and child unit
	MapUnit* parent;
	MapUnit* child;

	// linked event (creator)
	SpellMapEventRec *map_event;

	// linked event (SeeUnit)
	SpellMapEventRec *trig_event;

	// unit rendering state
	vector<AStarNode> move_nodes;
	int move_state;
	int move_step;
	int isMoving() { return(move_state != MOVE_STATE_IDLE); };
	FSU_resource* in_animation;
	int azimuth;
	int azimuth_turret;
	int frame;
	int frame_stop;
	double unit_angle; /* angle in 3D view */
	double attack_angle; /* angle in projected 2D view */
	//double projectile_angle;
	double attack_dist;
	int attack_proj_step;
	int attack_proj_delay;
	int attack_state;
	int attack_hit_frame;
	AnimPNM* attack_hit_pnm;
	AnimPNM* attack_fire_pnm;
	int attack_fire_x_org;
	int attack_fire_y_org;
	int attack_fire_frame;
	MapUnit* attack_target;
	MapXY attack_target_obj;
	int is_target;
	int isAttacker() { return(attack_state != ATTACK_STATE_IDLE); };
	int isTarget() { return(is_target); };

	// unit radar state
	int radar_up;
	// unit flight altitude [%]
	int altitude;

	int action_state;
	int action_step;

	static constexpr int ACTION_STATE_IDLE = 0;
	static constexpr int ACTION_STATE_AIR_LAND = 1;
	static constexpr int ACTION_STATE_AIR_TAKEOFF = 2;
	static constexpr int ACTION_STATE_AIR_FINISH = 3;
	static constexpr int ACTION_STATE_TURRET_DOWN = 4;
	static constexpr int ACTION_STATE_TURRET_FINISH = 5;
	static constexpr int ACTION_STATE_RADAR_UP = 6;
	static constexpr int ACTION_STATE_RADAR_DOWN = 7;
	static constexpr int ACTION_STATE_TO_FORT = 8;
	static constexpr int ACTION_STATE_FROM_FORT = 9;
	static constexpr int ACTION_STATE_FROM_FORT_TURRET = 10;
	static constexpr int ACTION_STATE_CREATE_UNIT = 11;
	static constexpr int ACTION_STATE_KAMIKAZE = 12;
	static constexpr int ACTION_STATE_KAMIKAZE_EXPLOSION = 13;

	static constexpr int ATTACK_STATE_IDLE = 0;
	static constexpr int ATTACK_STATE_DIR = 1;
	static constexpr int ATTACK_STATE_TURN = 2;
	static constexpr int ATTACK_STATE_SHOT = 3;
	static constexpr int ATTACK_STATE_FLIGHT = 4;
	static constexpr int ATTACK_STATE_HIT = 5;

	static constexpr int MOVE_STATE_IDLE = 0;
	static constexpr int MOVE_STATE_TURRET = 1;
	static constexpr int MOVE_STATE_MOVE = 2;
	static constexpr int MOVE_STATE_TELEPORT_IN = 3;
	static constexpr int MOVE_STATE_TELEPORT_OUT = 4;


	FSU_resource* GetShotAnim(MapUnit* target=NULL,int* frame_stop=NULL);
	AnimPNM* GetTargetHitPNM(MapUnit* target=NULL);
	AnimPNM* GetFirePNM(MapUnit* target=NULL);
	tuple<int,int> GetFirePNMorigin(MapUnit* target,double azimuth);

	int AreSoundsDone();
	int PlayReport();
	int PlayContact();
	int PlayMove();
	int PlayStop();
	SpellSound* sound_move;
	int PlayFire(MapUnit* target=NULL);
	int PlayHit(bool missed=false);
	int PlayHit(MapUnit* target,bool missed=false);
	int PlayDie();
	int PlayBeingHit();
	int PlayAction();

	enum AttackResult{
		Missed = 0,
		Hit,
		Kill
	};

	int GetAttack(MapUnit* target=NULL);
	AttackResult DamageTarget(MapSprite* target);


	MapUnit();
	MapUnit(MapUnit& obj,bool relink_event_trigger=false);
	int MorphUnit(SpellUnitRec* target,int health=0);
	int ClearSounds();
	~MapUnit();
	int Render(Terrain* data,uint8_t* buffer,uint8_t* buf_end,int buf_x_pos,int buf_y_pos,int buf_x_size,uint8_t* filter,uint8_t* hud_filter,Sprite* sprt,int show_hud);

	int ResetAP();
	int GetMaxAP();
	int HasMaxAP();
	int GetWalkAP();
	int GetFireCount(int ext_ap=-1);
	int GetMaxFireCount();
	int UpdateFireAP();
	int GetAPperFire();
	int CanSpecAction();
	int HasWounded();
	int Heal();
	int ResetHealth();
	int UpdateDigLevel();
	void ClearDigLevel();
	void ResetTurnsCounter();
	void IncrementTurnsCounter();
	void ActivateUnit();
	int isActive();

};