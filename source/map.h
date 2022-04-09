//=============================================================================
// Spellcross MAP related routines: Loading, rendering.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
//#pragma once
#ifndef _MAP_H_
#define _MAP_H_

#include "windows.h"
#include "cstdint"
#include <vector>
#include <thread>
#include <mutex>
#include <list>
#include <string>
#include <tuple>
#include "fs_archive.h"
#include "fsu_archive.h"
#include "spell_units.h"
#include "spellcross.h"
#include "map_types.h"
#include "spell_hud_buttons.h"

#include "wx/dcbuffer.h"

#include <algorithm>

using namespace std;

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

	MapLayer4() {};
	MapLayer4(AnimPNM* pnm, int x_pos=0, int y_pos=0, int x_ofs=0, int y_ofs=0, int frame_ofs=0, int frame_limit=-1);
	~MapLayer4();
};

class AStarNode {
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


/*class MapXY
{
public:
	int x;
	int y;
	bool IsSelected() {return(x >= 0 && y >= 0);};
	MapXY() {x=0;y=0;};
};*/

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
		SpecUnit1,
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
			value = Values::SpecUnit1;
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
	// action points
	int action_points;
	// unit active (set except insertion time)
	int is_active;
	// unit visible?
	int is_visible;
	// enemy?
	int is_enemy;
	// morale level
	int morale;
	// unit in placement (selected and moving)
	int in_placement;
	// unit moved flag (cleared when rendered)
	int was_moved;

	// pointer to next unit to draw (for correct render order)
	MapUnit* next;
	int assigned;

	// link s between parent and child unit
	MapUnit* parent;
	MapUnit* child;


	// unit rendering state
	vector<AStarNode> move_nodes;
	int move_state;
	int move_step;
	int isMoving() { return(move_state != MOVE_STATE_IDLE); };
	FSU_resource *in_animation;
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
	MapUnit *attack_target;
	int is_target;
	int isAttacker() {return(attack_state != ATTACK_STATE_IDLE);};
	int isTarget() {return(is_target);};

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

	
	FSU_resource* GetShotAnim(MapUnit* target,int* frame_stop=NULL);
	AnimPNM* GetTargetHitPNM(MapUnit* target);
	AnimPNM* GetFirePNM(MapUnit* target);
	tuple<int,int> GetFirePNMorigin(MapUnit* target,double azimuth);

	int AreSoundsDone();
	int PlayReport();
	SpellSound* sound_report;
	int PlayContact();
	SpellSound* sound_contact;
	int PlayMove();
	int PlayStop();
	SpellSound* sound_move;
	int PlayFire(MapUnit* target);
	int PlayHit(MapUnit* target);
	SpellAttackSound* sound_attack_light;
	SpellAttackSound* sound_attack_armor;
	SpellAttackSound* sound_attack_air;
	int PlayDie();
	SpellSound* sound_die;
	int PlayBeingHit();
	SpellSound* sound_hit;
	int PlayAction();
	SpellSound* sound_action;



	MapUnit();
	MapUnit(MapUnit& obj);
	int MorphUnit(SpellUnitRec *target,int health=0);
	int ClearSounds();
	~MapUnit();
	int Render(Terrain* data,uint8_t* buffer,uint8_t* buf_end,int buf_x_pos,int buf_y_pos,int buf_x_size,uint8_t* filter,Sprite* sprt,int show_hud);

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
};

// map sounds
class MapSound
{	
private:
	SpellSample* m_sample;
	MapXY m_pos;
public:
		
	MapSound(MapXY pos, SpellSample *sample);
	const char *GetName();
	MapXY GetPosition();
	SpellSample* GetSample();
};

// map ambient loop sounds
class MapLoopSounds
{
private:
	int x_size;
	int y_size;
public:
	// list unique map sounds
	vector<SpellSound*> list;
	// list of map loop sound
	vector<MapSound> sounds;
	// map of volumes per sound [sound_id][MapXY]
	vector<vector<int8_t>> maps;

	MapLoopSounds(int x_size,int y_size);	
	~MapLoopSounds();
	int UpdateMaps();
	tuple<double,double> GetViewVolume(int index,int x0,int y0,int x_sz,int y_sz);
	int UpdateVolumes(int xs_ofs,int ys_ofs,int xs_size,int ys_size);
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
	void SetPos(int x,int y);
	void Idle();
	int ResizeSelection(int delta);
	bool wasModified();
	bool Commit();
	
	tuple<int,int> CheckScroll(int x_limit,int y_limit);
	tuple<int,int> CheckScroll(int x_min,int y_min,int x_limit,int y_limit);
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
		// game mode
		int game_mode;		
		// render surface
		int surf_x;
		int surf_y;
		int last_surf_x;
		int last_surf_y;
		int surf_modified;		
		int surf_x_origin; /* surface origin in raster buffer */
		int surf_y_origin;		
		// indexed raster buffer		
		int pic_x_size;
		int pic_y_size;
		uint8_t* pic;
		uint8_t* pic_end; /* buffer end */
		vector<int> pic_y_buffer; /* used to find top most rendered pixel of HUD to cut off mouse area */
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
		// unit pointer animation
		MapLayer4 pnm_sipka;

		// tile selection
		MapXY sel;
		vector<MapXY> msel;
		
		// filter map
		vector<uint8_t*> filter;
		
		// temp layers for debug mostly
		vector<MapXY> dbg_ord;

		//static constexpr int UNIT_PATH_IDLE = -1;
		//int unit_path_state;
		mutex unit_action_lock;
		//vector<AStarNode> unit_path;		

		// currently selected unit
		MapUnit *unit_selection;
		int unit_selection_mod;
		int unit_sel_land_preference;
		// current view state of map tiles
		vector<int> units_view;
		vector<int> units_view_mem; /* this is copy used to keep view of all but selected unit */
		vector<int> units_view_flags;
		vector<int> units_view_flags_mem;		
		// units view mask map
		vector<uint8_t> units_view_mask; // mask with objects
		vector<uint8_t> units_view_mask_0; // mask without object
		vector<uint16_t> units_view_map;
		static constexpr int units_view_mask_size = 10;
		int units_view_mask_x_size;
		int units_view_mask_y_size;
		// unit attack range stuff
		vector<int> unit_attack_map;
		int UnitAttackRangeInit();
		int CalcUnitAttackRange(MapUnit* unit);		
		// unit range map
		vector<AStarNode> unit_range_nodes_buffer; // this is preinitialized buffer holding the nodes
		vector<AStarNode> unit_range_nodes; // this is working buffer
		vector<int> unit_ap_left;
		vector<int> unit_fire_left;
		thread *unit_range_th;
		int unit_range_th_control;
		MapUnit unit_range_th_unit;
		mutex unit_range_th_lock;
		int unit_range_view_mode;
		int unit_range_view_mode_lock;
		uint8_t *GetUnitRangeFilter(int mxy);
		int FindUnitRangeLock(bool state);
		int FindUnitRange_th();
		int InitUnitRangeStuff();		
		int viewingUnitMoveRange() {return(unit_range_view_mode == UNIT_RANGE_MOVE);};
		int viewingUnitAttackRange() { return(unit_range_view_mode == UNIT_RANGE_ATTACK);};
		static constexpr int UNIT_RANGE_TH_IDLE = 0x00;
		static constexpr int UNIT_RANGE_TH_RUN = 0x01;
		static constexpr int UNIT_RANGE_TH_STOP = 0x02;
		static constexpr int UNIT_RANGE_TH_EXIT = 0x03;		
		// units moved flag (can be set to force view tiles recalc)
		//int units_moved;		
		int UnitsMoved(int clear=false);
		tuple<int,int> GetUnitsViewMask(int x,int y,int plain_tile_id=-1);
		tuple<int,int,int> GetUnitsViewTileCenter(int x,int y);
		tuple<int,int,int> GetUnitsViewTileCenter(MapXY mxy);
		vector<Txyz> GetUnitsViewTilePixels(int x,int y,int all=false);
		vector<Txyz> GetUnitsViewTilePixels(MapXY mxy,int all=false);
		// unit types
		static constexpr int UNIT_TYPE_ALIANCE = 0x01;
		static constexpr int UNIT_TYPE_OS = 0x02;
		
		// current HUD buttons list
		vector<SpellBtnHUD*> hud_buttons;
		int hud_enabled;


		// sound stuff
		

		// layer visibility flags
		bool wL1, wL2, wL3, wL4, wSTCI, wUnits;
		bool wSound, wSoundLoop;
		int w_unit_hud;

		// last gamma
		double last_gamma;
		double gamma;

		//static constexpr int MAP_BACK_COLOR = 230; // map background color index
		static constexpr int MAP_BACK_COLOR = 254; // map background color index
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

		// shading flags stuff
		vector<int> shading;
		vector<int> shading_mask;
		void SyncShading();
		int CheckTileShading(MapXY& pos,Sprite* spr);

		class t_xypos{
		public:
			int x;
			int y;
			t_xypos Add(int xx,int yy) { t_xypos pos; pos.x = x+xx; pos.y = y+yy; return(pos);};
		};

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
		uint32_t *L1_flags; // terrain class flags array
		// list of units
		vector<MapUnit*> units;
		// map sounds
		vector<MapSound> sounds;
		// looping sounds
		MapLoopSounds *sound_loops;

		
		SpellMap();
		~SpellMap();
		int SetGameMode(int new_mode);
		int isGameMode();
		void Close();
		int Create(SpellData* spelldata,const char* terr_name,int x,int y);
		int Load(wstring &path, SpellData* spelldata);		
		int IsLoaded();
		bool TileIsVisible(int x, int y);
		
		MapXY GetSelection(wxBitmap& bmp,TScroll* scroll);
		vector<MapXY> &GetSelections(wxBitmap& bmp, TScroll* scroll);
		vector<Sprite*> GetL1sprites(vector<MapXY> &selection);
		vector<Sprite*> GetL2sprites(vector<MapXY>& selection);
		vector<uint8_t> GetFlags(vector<MapXY>& selection);
		vector<MapXY> GetPersistSelections();
		void SelectTiles(vector<MapXY> tiles,int mode);
		void SelectTiles(int mode);
		int IvalidateTiles(vector<MapXY> tiles,std::function<void(std::string)> status_cb=NULL);
		int RenderPrepare(wxBitmap& bmp, TScroll* scroll);
		vector<MapXY> GetVisibleTiles();
		tuple<int,int> GetMapSurfaceSize();
		int isRenderSurfModified();
		int CommitRenderSurfModified();		
		int Render(wxBitmap &bmp, TScroll* scroll,SpellTool* tool=NULL,std::function<void(void)> hud_buttons_cb=NULL);
		
		int GetHUDstate();
		int SetHUDstate(int state);
		int RenderHUD(uint8_t* buf,uint8_t* buf_end,int buf_x_size,MapXY* cursor,MapUnit *cursor_unit,std::function<void(void)> hud_buttons_cb=NULL);
		int RenderHUDrect(uint8_t* buf,uint8_t* buf_end,int buf_x_size,int x1,int y1,int w,int h,uint8_t color);
		int GetHUDtop(int surf_x_pos);
		SpellBtnHUD* CreateHUDbutton(SpellGraphicItem* glyph,t_xypos& hud_pos,t_xypos &pos,uint8_t* buf,uint8_t* buf_end,int buf_x_size,
			int action_id,std::function<void(void)> cb_press,std::function<void(void)> cb_hover,int disabled_glyph=false);
		SpellBtnHUD *GetHUDbutton(int id);
		void ClearHUDbuttons(bool only_invalidate=false);
		void CleanupInvalidHUDbuttons();
		void InvalidateHUDbuttons();
		vector<SpellBtnHUD*> *GetHUDbuttons();
		
		void OnHUDnextUnit();
		void OnHUDnextUnfinishedUnit();
		void OnHUDswitchAirLand();
		void OnHUDswitchUnitHUD();
		void OnHUDswitchEndTurn();
		void OnHUDhealUnit();
		void OnHUDturretToggle();
		void OnHUDradarToggle();
		void OnHUDairLandTakeOff();
		void OnHUDfortresToggle();
		void OnHUDcreateUnit();
		

		int RemoveUnit(MapUnit* unit);
		int CreateUnit(MapUnit* parent,SpellUnitRec* new_type);
		void SortUnits();
		MapUnit *GetCursorUnit(wxBitmap& bmp,TScroll* scroll);
		MapUnit* CanSelectUnit(MapXY pos);
		MapUnit *SelectUnit(MapUnit* new_unit);
		int UnitChanged(int clear=false);
		MapUnit* GetSelectedUnit();
		int PrepareUnitsViewMask();
		wxBitmap *ExportUnitsViewZmap();
		int ClearUnitsView(int to_unseen=false);
		int AddUnitView(MapUnit* unit);
		int AddUnitsView(int unit_type=UNIT_TYPE_ALIANCE,int clear=true,MapUnit *except_unit=NULL);
		int StoreUnitsView();
		int RestoreUnitsView();
		void InvalidateUnitsView();
		vector<AStarNode> FindUnitPath(MapUnit* unit,MapXY target);
		int FindUnitRange(MapUnit* unit);
		int SetUnitRangeViewMode(int mode);
		int CanUnitMove(MapXY target);
		int MoveUnit(MapXY target);		
		int ResetUnitsAP();
		static constexpr int UNIT_RANGE_NONE = 0x00;
		static constexpr int UNIT_RANGE_MOVE = 0x01;
		static constexpr int UNIT_RANGE_ATTACK = 0x02;
		static constexpr int UNIT_RANGE_INCREMENT = -1;
		int CanUnitAttack(MapUnit* target);
		MapUnit* CanUnitAttackLand(MapXY pos);
		MapUnit* CanUnitAttackAir(MapXY pos);
		int CanUnitAttackObject(MapXY pos);
		int Attack(MapXY pos,int prefer_air);
		int AttackUnit(MapUnit* target);
		tuple<int,int> GetPosOrigin(MapXY pos);
		double GetUnitsAngle(MapUnit* ref,MapUnit* target);
		int GetUnitOptions(wxBitmap& bmp,TScroll* scroll);
		enum {
			UNIT_OPT_UPPER = 1,
			UNIT_OPT_LOWER = 2,
			UNIT_OPT_MOVE = 4,
			UNIT_OPT_SELECT = 8,
			UNIT_OPT_ATTACK = 16
		};
		

		void SetRender(bool wL1, bool wL2, bool wL3, bool wL4, bool wSECI, bool wUnits, bool wSound, bool wSoundLoop);
		void SetGamma(double gamma);
		int Tick();
		wstring GetTopPath();
		
		int ConvXY(int x, int y);
		int ConvXY(MapXY &mxy);
		int ConvXY(MapXY *mxy);

			
		int GetElevation(wxBitmap& bmp,TScroll* scroll);
		char *GetL1tileName(wxBitmap& bmp,TScroll* scroll);
		char* GetL2tileName(wxBitmap& bmp,TScroll* scroll);
		tuple<int,int,int> GetTileFlags(wxBitmap& bmp,TScroll* scroll);

		int EditElev(wxBitmap& bmp,TScroll* scroll,int step);
		int ReTexture(uint8_t* modz,std::function<void(std::string)> status_cb=NULL);
		void SyncL1flags();

		int EditClass(vector<MapXY>& selection,SpellTool* tool,std::function<void(std::string)> status_cb=NULL);

		MapXY GetNeighborTile(int x,int y,int quad);
		MapXY GetNeighborTile(MapXY xy,int quad);
		int BuildSpriteContext();

		enum{
			HUD_ACTION_MINIMAP = 1000
		};

		MapSound* GetRandomSound(double* left=NULL,double* right=NULL);
		
};


class SpellMapTxt
{
	vector<Sprite*> tile_list;
	int index;
public:
	int TilesCount();
	void AddTile(Sprite *tile);
	void ClearTiles();
	void Shuffle();
	int SetIndex(int id=0);
	int NextIndex();
	Sprite *GetTile(int id=-1);
};

#endif // !_MAP_H_
