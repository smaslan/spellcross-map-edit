//=============================================================================
// Spellcross MAP related routines: Loading, saving, rendering, editting.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021-2025, Stanislav Maslan, s.maslan@seznam.cz
// url: https://github.com/smaslan/spellcross-map-edit
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
//#pragma once
#ifndef _MAP_H_
#define _MAP_H_

#include "cstdint"
#include <vector>
#include <queue>
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
#include "spell_map_event.h"

#include "wx/dcbuffer.h"

#include <algorithm>

using namespace std;

#define MAX_STR 256
#define MAX_SPRITE_NAME 8


// map L1/L2 sprite item
class MapSprite
{
private:

public:
	// sprite data pointer
	Sprite* L1 = NULL;
	Sprite* L2 = NULL;
	// elevation
	int elev;
	// elevation
	uint8_t flags;
	// hit points if destructible
	int hp;
	// being attack PNM animation
	int is_target;
	AnimPNM* hit_pnm;
	int hit_pnm_frame;


	//MapSprite(Sprite* sprite);
	MapSprite();
	~MapSprite();

	void SetL1(Sprite* sprite,int elev=-1);
	void SetL2(Sprite* sprite,int flags=-1);
	int UpdateDestructible();

	Sprite* GetDestructible();
	int GetMaxHP();
	int isDestructible();
	int isTarget();

	int PlayHit();
	int PlayDestruct();
};

// map tile animation ANM
class MapLayer3
{
	public:
		// animation location in map
		int x_pos;
		int y_pos;
		int frame_ofs;
		int frame_limit;
		AnimL1* anim;
		bool in_placement;

		MapLayer3(AnimL1* anm=NULL, int x_pos=-1, int y_pos=-1, int frame_ofs=0, int frame_limit=0);
		~MapLayer3();
		bool Compare(MapLayer3 *anm);
};

// map sprite animation PNM
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
	bool in_placement;

	MapLayer4() {};
	MapLayer4(AnimPNM* pnm, int x_pos=0, int y_pos=0, int x_ofs=0, int y_ofs=0, int frame_ofs=0, int frame_limit=-1);
	~MapLayer4();
	bool Compare(MapLayer4* pnm);
};


/*class MapXY
{
public:
	int x;
	int y;
	bool IsSelected() {return(x >= 0 && y >= 0);};
	MapXY() {x=0;y=0;};
};*/


// map sound item
class MapSound
{	
private:
	SpellSample* m_sample;
	MapXY m_pos;
	bool m_is_loop;

public:
	bool in_placement;	

	enum SoundType {
		RANDOM = 0,
		LOOP,
		BOTH
	};
			
	MapSound(MapXY pos, SpellSample *sample,SoundType type);
	SoundType GetType() {return(m_is_loop?(MapSound::SoundType::LOOP):(MapSound::SoundType::RANDOM));};
	bool isLoop() {return(m_is_loop);};
	bool isRandom() { return(!m_is_loop); };
	const char *GetName();
	MapXY GetPosition();
	void SetPosition(MapXY &pos);
	SpellSample* GetSample();
	void SetSample(SpellSample* sample);
	void SetType(SoundType type);
};

// map ambient sounds
class MapSounds
{
private:
	int x_size;
	int y_size;
	SpellData *m_spell_data;
public:
	
	// list unique map sounds
	vector<SpellSound*> list;
	// list of map loop sound
	vector<MapSound> sounds;
	// map of volumes per sound [sound_id][MapXY]
	vector<vector<int8_t>> maps;

	MapSounds(SpellData* spell_data,int x_size,int y_size);
	~MapSounds();
	void ClearSounds();
	int InitSounds();
	int UpdateMaps();
	tuple<double,double> GetViewVolume(int index,int x0,int y0,int x_sz,int y_sz);
	int UpdateVolumes(int xs_ofs,int ys_ofs,int xs_size,int ys_size);
};



// map scroller
class TScroll
{
private:
	// screen size
	int xsize,ysize;
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
	// moved?
	int moved;

	static constexpr int MAX_SEL_SIZE = 8;

public:	
	TScroll();
	void SetSurface(int xx, int yy);
	tuple<int,int> GetSurface();
	void Reset();
	void SetRef(int x, int y);
	void Move(int x, int y);
	void SetPos(int x,int y);
	int Idle();
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


// main spellcross map class
class SpellMap
{
	private:		
		// map state
		bool is_valid;
		// last error string
		std::string last_error;
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
		// special tiles pointers
		Sprite* start_sprite;
		Sprite* escape_sprite;
		Sprite* target_sprite;
		// unit pointer animation
		MapLayer4 pnm_sipka;

		// tile selection
		MapXY sel;
		vector<MapXY> msel;
		
		// filter map
		vector<uint8_t*> filter;
		
		// temp layers for debug mostly
		vector<MapXY> dbg_ord;

		// map edit lock (recoursive - multiple lock() calls per thread possible)
		std::recursive_mutex map_lock;
				
		// attack state stuff
		std::vector<MapSprite*> attack_explosion_list;

		
		int units_view_debug_mode;

		// currently selected unit
		MapUnit *unit_selection;
		int unit_selection_mod;
		int unit_sel_land_preference;

		// sound selection
		MapSound *sound_selection;
		// animation selection
		MapLayer3* anm_selection;
		MapLayer4 *pnm_selection;
		
		// unit range map		
		int unit_range_view_mode;
		int unit_range_view_mode_lock;
		uint8_t *GetUnitRangeFilter(int x,int y);
		uint8_t *default_filter;
		uint8_t *render_filter;
		int viewingUnitMoveRange() {return(unit_range_view_mode == UNIT_RANGE_MOVE);};
		int viewingUnitAttackRange() { return(unit_range_view_mode == UNIT_RANGE_ATTACK);};
		static constexpr int UNIT_RANGE_TH_IDLE = 0x00;
		static constexpr int UNIT_RANGE_TH_RUN = 0x01;
		static constexpr int UNIT_RANGE_TH_STOP = 0x02;
		static constexpr int UNIT_RANGE_TH_EXIT = 0x03;		
		// units moved flag (can be set to force view tiles recalc)
		int UnitsMoved(int clear=false);		
		// unit types
		static constexpr int UNIT_TYPE_ALIANCE = 0x01;
		static constexpr int UNIT_TYPE_OS = 0x02;
		
		// current HUD buttons list
		vector<SpellBtnHUD*> hud_buttons;
		int hud_enabled;


		// layer visibility flags
		bool wL1, wL2, wL3, wL4, wSTCI, wUnits;
		bool wSound, wSoundLoop, wEvents;
		bool wHighlight_obj;
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

		// stuff selection blink state
		int sel_blink_state;

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

		std::string GetLastError() {return last_error;};

		// message box stuff
		typedef std::function<void(SpellTextRec*,bool,std::function<void(bool)>)> SpellMessageCraeteFunPtr;
		typedef std::function<bool(void)> SpellMessagePollingFunPtr;
		int ShowMessage(SpellTextRec *msg, bool is_yesno, std::function<void(bool)> callback);
		void SetMessageInterface(SpellMessageCraeteFunPtr create_msg,SpellMessagePollingFunPtr msg_checker);
		SpellMessageCraeteFunPtr m_msg_creator;
		SpellMessagePollingFunPtr m_msg_checker;
		
		// map modification locks
		int LockMap();
		int ReleaseMap();
		//void LockUnitRanging(bool lock);
		void HaltUnitRanging(bool clear_tasks=false);
		void ResumeUnitRanging(bool resume=true);


		// default scroller
		TScroll scroller;

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
		vector<MapSprite> tiles; // terrain tiles map (L1+L2)
		vector<MapLayer3> L3; // ANM list
		vector<MapLayer4> L4; // PNM list				
		vector<MapXY> start; // start tiles list
		vector<MapXY> escape; // escape tiles list
		vector<MapXY> target; // target tiles list
		vector<MapUnit*> Lunit; // units layer array
		vector<uint8_t> select; // selection flags array
		vector<uint32_t> L1_flags; // terrain class flags array
		// list of units
		vector<MapUnit*> units;
		// map events
		SpellMapEvents *events;
		// map sounds
		//vector<MapSound> sounds;
		// looping sounds
		MapSounds *sounds;

		// local palette (after gamma correction)
		uint8_t pal[256][3];

		

		class MissionParams
		{
		public:
			bool is_night = false;
			std::string mission_text = "";
			std::string start_text = "";
			std::string end_ok_text = "";
			std::string end_bad_text = "";
			void Clear() { is_night = false; mission_text = "";start_text = "";end_ok_text = "";end_bad_text = "";};
		};
		MissionParams params;

		// --- unit view/attack range stuff
		class ViewRange
		{
		public:

			// current view state of map tiles
			vector<int> view;
			vector<int> view_mem; /* this is copy used to keep view of all but selected unit */

			// attack range map
			vector<int> attack_map;

			// units view mask map
			vector<uint8_t> view_mask; // mask with objects
			vector<uint8_t> view_mask_0; // mask without object
			vector<uint16_t> view_map;
			
			enum class ClearMode
			{
				NONE = 0, /* no view clear */
				HIDE, /* clear map to currently not visible, leave just seen units visible */
				HIDE_UNITS, /* hide units */
				HIDE_ENEMY, /* hide enemy units */
				HIDE_ALL, /* HIDE & HIDE_UNITS */
				RESET /* clear to never seen */
			};

			ViewRange(SpellMap* map);
			~ViewRange();
			void ResultLock(bool lock);
			int isIdle();
			int WaitIdle();
			//void Lock();
			//void Unlock();
			
			bool Halt(bool clear_tasks=false);
			void Resume(bool resume=true);
			void ClearTasks();
			
			int PrepareUnitsViewMask(bool immediate=false);
			wxBitmap* ExportUnitsViewZmap();
			int ClearUnitsView(ClearMode clear=ClearMode::HIDE, bool immediate=false);
			void ClearEvents(bool cleanup=false);
			void AddUnitView(MapUnit* unit,ClearMode clear=ClearMode::NONE,int* new_contact=NULL,vector<SpellMapEventRec*>* events=NULL);
			int GetResults(vector<SpellMapEventRec*>* events=NULL);
			int WaitResults(vector<SpellMapEventRec*>* events=NULL);
			int AddUnitsView(int unit_type=UNIT_TYPE_ALIANCE,int clear=true,MapUnit* except_unit=NULL);
			int StoreUnitsView(bool immediate=false);
			int RestoreUnitsView(bool immediate=false);
			int CalcAttackRange(MapUnit *unit, bool immediate=false);

		private:

			enum class Action
			{
				NONE = 0, /* no special action */
				MASK, /* update view mask */
				CLEAR, /* clear unit(s) view */
				STORE, /* store current view state */
				RESTORE, /* restore stored view state */
				FIRE /* update unit fire range */
			};
			
			class Task
			{
			public:
				// reference unit
				MapUnit *unit;
				// clear mode
				ClearMode clear;
				// action id
				Action action;
				// collect events
				int detect_events;

				Task(MapUnit *unit, ClearMode clear, Action action, bool detect_events)
				{
					this->unit = unit;
					this->clear = clear;
					this->action = action;
					this->detect_events = detect_events;
				};
			};

			// back ref to map
			SpellMap* map;

			// worker thread
			std::thread* thread;
			std::mutex result_lock; // result access lock
			std::mutex ctrl_lock; // worker control lock
			std::mutex queue_lock; // task queue lock
			std::deque<Task> pending; // pending worker tasks
			enum ThreadCtrl
			{
				IDLE = 0,
				BUSY,
				STOP,
				HALTED,
				EXIT
			};
			volatile ThreadCtrl state; // worker state/control variable
			bool is_halted;
			int was_halted;
			

			// Z-mask grid size
			static constexpr int VIEW_MASK_GRID = 10;
			int view_mask_x_size;
			int view_mask_y_size;

			// collected detected events
			int was_new_contact;
			std::vector<SpellMapEventRec*> event_list;
			
			
			void Worker();
			//void Stop();
			int AddViewUnitTask(MapUnit *unit,ClearMode clear=ClearMode::NONE,bool rec_events=false,bool force=false);
			int ClearUnitsViewCore(ClearMode clear,std::vector<int>* p_view=NULL);
			int PrepareUnitsViewMaskCore();
			int RestoreUnitsViewCore();
			tuple<int,int> GetUnitsViewMask(int x,int y,int plain_tile_id);
			tuple<int,int,int> GetUnitsViewTileCenter(MapXY mxy);
			tuple<int,int,int> GetUnitsViewTileCenter(int x,int y);
			vector<Txyz> GetUnitsViewTilePixels(MapXY mxy,int all);
			vector<Txyz> GetUnitsViewTilePixels(int x,int y,int all);
			int AttackRangeInit();

		};
		ViewRange* unit_view;

		
		// --- unit move range stuff
		class MoveRange
		{
		public:
			MoveRange(SpellMap* map);
			~MoveRange();
			void Halt(bool clear_tasks=false);
			void Resume(bool resume=true);
			void FindRange(MapUnit* unit);
			void ResultLock(bool state);
			vector<AStarNode> FindPath(MapUnit* unit,MapXY target);

			vector<AStarNode> range_nodes_buffer; // this is preinitialized buffer holding the nodes
			vector<AStarNode> range_nodes; // this is working buffer
			
			vector<int> ap_left;
			vector<int> fire_left;

		private:
			// back ref to map
			SpellMap* map;

			class Task
			{
			public:
				MapUnit *unit;
				Task(MapUnit *unit)
				{
					this->unit = unit;
				}
			};

			// worker thread
			std::thread* thread;
			std::mutex result_lock; // result data lock
			std::mutex ctrl_lock; // thread control lock
			std::mutex queue_lock; // task queue lock
			std::deque<Task> pending; // tasks queue
			enum ThreadCtrl
			{
				IDLE = 0,
				BUSY,
				STOP,
				HALTED,
				EXIT
			};
			volatile ThreadCtrl state;
			bool is_halted;
			int was_halted;

			void Worker();
		};
		MoveRange *unit_range;


		// map state save
		class SavedState
		{
		public:		
			// tiles copy
			vector<MapSprite> tiles; // terrain tiles map (L1+L2)
			// list of units
			vector<MapUnit*> units;
			MapUnit *sel_unit;
			// map events
			SpellMapEvents* events;
			// view mask
			vector<int> units_view;
			
			SavedState();
			~SavedState();
			void Clear();
			MapUnit* GetUnit(int id);
		};
		class Saves
		{
		private:
			vector<SavedState*> saves;
			SavedState initial;
			int max_saves;
			SpellMap *map;
		public:
			Saves(SpellMap *parent);
			~Saves();
			int Clear(int count=4);			
			int canSave();
			int canLoad();			
			int Save(SpellMap::SavedState* slot=NULL);
			int LoadByID(int index=-1);
			int Load(SpellMap::SavedState* save=NULL);
			int SaveInitial();
			int LoadInitial();
		};
		Saves *saves;

		
		SpellMap();
		~SpellMap();
		int SetGameMode(int new_mode);
		int isGameMode();
		void Close();
		int Create(SpellData* spelldata,const char* terr_name,int x,int y);
		int Load(wstring &path, SpellData* spelldata);
		int SaveDTA(std::wstring path);
		int SaveDEF(std::wstring path);
		int IsLoaded();
		bool TileIsVisible(int x, int y);
		tuple<int,int> GetSurfPos(MapXY &pos);

		
		
		MapXY GetSelection(TScroll* scroll=NULL);
		vector<MapXY> &GetSelections(TScroll* scroll=NULL);
		void ClearSelections();
		vector<Sprite*> GetL1sprites(vector<MapXY> &selection);
		vector<Sprite*> GetL2sprites(vector<MapXY>& selection);
		vector<uint8_t> GetFlags(vector<MapXY>& selection);
		int GetFlags(MapXY selection);
		void SetFlags(MapXY selection,int flags);
		void SetFlags(std::vector<MapXY> selections,int flags);
		int EditTileSprite(Sprite* spr,MapXY* pos=NULL);
		MapSprite* GetTile(MapXY* pos=NULL);
		MapSprite* CheckObj(MapXY* pos=NULL);
		int RemoveObj(MapXY* pos=NULL);
		MapLayer3* CheckANM(MapXY* pos=NULL);
		int RemoveANM(MapXY* pos=NULL);
		int PlaceANM(MapXY* pos,AnimL1* anm);
		void SelectANM(MapLayer3* anm=NULL);
		MapLayer3* SelectedANM();
		int MoveANM(MapLayer3* anm,MapXY pos);
		MapLayer4* CheckPNM(MapXY* pos=NULL);
		int RemovePNM(MapXY* pos=NULL);
		void SelectPNM(MapLayer4* pnm=NULL);
		MapLayer4* SelectedPNM();
		int PlacePNM(MapXY* pos,AnimPNM* pnm,int x_ofs,int y_ofs);
		int MovePNM(MapLayer4* pnm,MapXY pos);
		MapSound *CheckSound(MapXY* pos=NULL,MapSound::SoundType type=MapSound::SoundType::BOTH);
		void SoundSelect(MapSound* sound=NULL);
		MapSound* SoundSelected();
		int SoundMove(MapSound* sound,MapXY pos,bool remap=false);
		int SoundRemove(MapXY* pos=NULL);
		int SoundEdit(SpellSample* new_sound,MapSound::SoundType type=MapSound::SoundType::BOTH,MapXY* pos=NULL);
		MapSound* SoundAdd(SpellSample* new_sound,MapSound::SoundType type,MapXY* pos=NULL);
		vector<MapXY> GetPersistSelections();
		void SelectTiles(vector<MapXY> tiles,int mode);
		void SelectTiles(int mode);
		int IvalidateTiles(vector<MapXY> tiles,std::function<void(std::string)> status_cb=NULL);
		int RenderPrepare(TScroll* scroll=NULL);
		vector<MapXY> GetVisibleTiles();
		tuple<int,int> GetMapSurfaceSize();
		int isRenderSurfModified();
		int CommitRenderSurfModified();		
		int Render(wxBitmap &bmp, TScroll* scroll=NULL,SpellTool* tool=NULL,std::function<void(void)> hud_buttons_cb=NULL);
		int GetRender(uint8_t* buf, int x_size, int y_size, int x_pos, int y_pos);
		void SetDefaultRenderFilter(uint8_t* filter);
		void SetRenderFilter(uint8_t* filter);
		int ScrollToTile(MapXY &tile,TScroll* scroll=NULL);
		int ScrollToUnit(MapUnit *unit=NULL,TScroll* scroll=NULL);
				
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
		

		MapUnit* GetUnit(int id);		
		int RemoveUnit(MapUnit* unit,bool from_events=false);		
		MapUnit* ExtractUnit(MapUnit* unit);
		int RemoveAllUnits();		
		int AddUnit(MapUnit* unit);		
		MapUnit* CreateUnit(MapUnit* parent=NULL,SpellUnitRec* new_type=NULL);
		int PlaceUnit(MapUnit* unit);
		int AssignUnitID(MapUnit* unit);
		int SortUnitIDs();
		void SortUnits();
		MapUnit *GetCursorUnit(TScroll* scroll=NULL);
		MapUnit* CanSelectUnit(MapXY pos);
		MapUnit *SelectUnit(MapUnit* new_unit,bool scroll_to=false);
		int UnitChanged(int clear=false);
		MapUnit* GetSelectedUnit();
		//int PrepareUnitsViewMask();
		//wxBitmap *ExportUnitsViewZmap();
		//int ClearUnitsView(int to_unseen=false);
		//int AddUnitView(MapUnit* unit, vector<SpellMapEventRec*> *event_list=NULL);
		//int AddUnitsView(int unit_type=UNIT_TYPE_ALIANCE,int clear=true,MapUnit *except_unit=NULL);
		void SetUnitsViewDebugMode(bool debug);
		bool isUnitsViewDebugMode();
		//int StoreUnitsView();
		//int RestoreUnitsView();
		void InvalidateUnitsView();
		//vector<AStarNode> FindUnitPath(MapUnit* unit,MapXY target);
		//int FindUnitRange(MapUnit* unit);
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
		int GetUnitOptions(TScroll* scroll=NULL);
		enum {
			UNIT_OPT_UPPER = 1,
			UNIT_OPT_LOWER = 2,
			UNIT_OPT_MOVE = 4,
			UNIT_OPT_SELECT = 8,
			UNIT_OPT_ATTACK = 16
		};
		int FinishUnits();
		std::vector<MapXY> UpdateDestructible(MapXY target_pos);
		std::vector<MapSprite*> GetDestructibleList(MapXY target_pos);

		// events stuff
		SpellMapEventsList event_list; // pending events
		SpellMapEventRec *selected_event; // currently selected event (only in placement)		
		int ResetUnitEvents();
		int MissionStartEvent();
		int ProcEventsList(SpellMapEventsList &list);
		SpellMapEventRec *GetCursorEvent(TScroll* scroll=NULL);
		int SelectEvent(SpellMapEventRec *evt);
		SpellMapEventRec* GetSelectEvent();
		int UpdateEventUnit(SpellMapEventRec *evt, MapUnit* unit);
		
		

		void SetRender(bool wL1, bool wL2, bool wL3, bool wL4, bool wSECI, bool wUnits, bool wSound, bool wSoundLoop, bool wEvents,bool highlight_obj);
		void SetGamma(double gamma);
		double GetGamma();
		int Tick();
		wstring GetTopPath();
		
		int ConvXY(int x, int y);
		int ConvXY(MapXY &mxy);
		int ConvXY(MapXY *mxy);

			
		int GetElevation(TScroll* scroll=NULL);
		const char *GetL1tileName(TScroll* scroll=NULL);
		const char* GetL2tileName(TScroll* scroll=NULL);
		tuple<int,int,int> GetTileFlags(TScroll* scroll=NULL);

		int EditElev(int step,TScroll* scroll=NULL);
		int ReTexture(uint8_t* modz,std::function<void(std::string)> status_cb=NULL);
		void SyncL1flags();

		int EditClass(vector<MapXY>& selection,SpellTool* tool,std::function<void(std::string)> status_cb=NULL);

		MapXY GetNeighborTile(int x,int y,int quad);
		MapXY GetNeighborTile(MapXY xy,int quad);
		MapXY GetNeighborTile8D(int x,int y,int angle);
		MapXY GetNeighborTile8D(MapXY mxy,int angle);
		int BuildSpriteContext();

		enum{
			HUD_ACTION_MINIMAP = 1000,
			HUD_ACTION_UNITS,
			HUD_ACTION_MAP_OPTIONS
		};

		MapSound* GetRandomSound(double* left=NULL,double* right=NULL);

		
		
		
		// copy&paste stuff
		class Layers{
		public:
			int lay1;
			int lay2;
			int anm;
			int pnm;
		};
		
		class CopyBuffer{
		public:
			std::vector<MapXY> pos;
			std::vector<MapSprite> tiles;
			std::vector<MapLayer3> anms;
			std::vector<MapLayer4> pnms;
		};
		CopyBuffer copy_buf;

		void ClearBuffer();
		int SetBuffer(SpellObject* obj);
		int SetBuffer(Sprite* spr);
		int SetBuffer(AnimL1* anm);
		int SetBuffer(AnimPNM* pnm,int x_ofs=0,int y_ofs=0);
		void CutBuffer(std::vector<MapXY>& posxy,Layers layers);
		void CopyBuffer(std::vector<MapXY> &posxy,Layers layers);
		void PasteBuffer(std::vector<MapSprite>& tiles,std::vector<MapLayer3>& anms,std::vector<MapLayer4>& pnms,std::vector<MapXY>& posxy);
		bool isCopyBufferFull();
		int PasteRandSprites(std::vector<MapSprite>& tiles,std::vector<MapXY>& posxy,std::vector<Sprite*>& sprites,bool force_rand);
		int DeleteSelObjects(std::vector<MapXY>& posxy,SpellMap::Layers layers);

		enum{
			SPEC_TILE_START = 0,
			SPEC_TILE_ESCAPE,
			SPEC_TILE_TARGET
		};
		int PlaceStartEscape(vector<MapXY>& posxy,int spec_tile_type);


		
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
