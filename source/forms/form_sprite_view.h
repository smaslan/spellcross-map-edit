///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/choice.h>
#include <wx/frame.h>
#include <wx/slider.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include <wx/treectrl.h>
#include <wx/listctrl.h>
#include <wx/dnd.h>

#include <wx_other.h>
#include "spellcross.h"
#include "sprites.h"

///////////////////////////////////////////////////////////////////////////


/*class wxListCtrlSprite : public wxListCtrl{
	Terrain *m_terr = NULL;
public:
	wxListCtrlSprite(wxWindow* parent,wxWindowID id,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize,long style=wxLC_ICON,const wxValidator& validator=wxDefaultValidator,const wxString& name=wxListCtrlNameStr) 
		: wxListCtrl(parent, id, pos, size, style, validator, name) {};
	//virtual ~wxListCtrlSprite() = default;
	wxString OnGetItemText(long item,long column) const;
	int OnGetItemImage(long item) const;

	void SetTerrain(Terrain *terr) {m_terr = terr;};
};*/


///////////////////////////////////////////////////////////////////////////////
/// Class FormSprite
///////////////////////////////////////////////////////////////////////////////
class FormSprite : public wxFrame
{
	private:
		SpellData *spell_data;

		int sprite_id;
		Terrain *m_terrain;
		Sprite *m_sprite;
		bool m_was_set;

		void OnClose(wxCloseEvent& ev);
		void OnCloseClick(wxCommandEvent& event);
		void OnSelectClick(wxCommandEvent& event);
		void OnTerrainChange(wxCommandEvent& event);
		void OnSelectSpriteAlt(wxCommandEvent& event);
		void OnSelectSpriteBtn(wxCommandEvent& event);
		void OnSelectNeighbor(wxCommandEvent& event);
		void OnCanvasRepaint(wxPaintEvent& event);
		void OnChangeGamma(wxCommandEvent& event);
		void OnChangeZoom(wxCommandEvent& event);
		void OnQchange(wxCommandEvent& event);
		void OnSelectEdgeBtn(wxCommandEvent& event);

		void OnFlagsChange(wxCommandEvent& event);
		void OnEdgeClassChange(wxCommandEvent& event);
		void OnEdgeShadeChange(wxCommandEvent& event);
		//void OnSpecClassChange(wxCommandEvent& event);

		void OnAssignKnowns(wxCommandEvent& event);
		void OnClearAllContext(wxCommandEvent& event);
		void OnClearContext(wxCommandEvent& event);		
		void OnUpdateContext(wxCommandEvent& event);
		void OnUpdateContextCb(string status);
		void OnAutoShadeFlags(wxCommandEvent& event);
		void OnSaveTileContext(wxCommandEvent& event);

		void SelectTerrain();
		void SelectQuad();
		void SetFlags();
		void SetEdgeClasses();
		void SetShadingFlags();
		//void SetSpecClasses();
		Terrain* FindTerrain();

		//void OnToolClassChange(wxCommandEvent& event);
		//void OnToolClassItemChange(wxCommandEvent& event);

		void FillAltList();


		// tile edge classes
		static constexpr struct {
			int cls;
			const char* name;
		} class_list[] = {
			{Sprite::CLASS_GENERIC, "Generic"},
			{Sprite::CLASS_GRASS, "Grass"},
			{Sprite::CLASS_DARK_GRASS, "Dark grass"},
			{Sprite::CLASS_MUD, "Mud"},
			{Sprite::CLASS_SWAMP, "Swamp"},
			{Sprite::CLASS_BLOOD, "Blood"},
			{Sprite::CLASS_ASH, "Ash"},
			{Sprite::CLASS_HIGH_LAND, "High land"},
			{Sprite::CLASS_SAND, "Sand"},
			{Sprite::CLASS_ROAD, "Road"},
			{Sprite::CLASS_DIRT_ROAD, "Dirt road"},
			{Sprite::CLASS_MUD_PATH, "Mud path"},
			{0,NULL}
		};

		static constexpr int TERR_ID0 = 990;

	protected:
		enum
		{
			wxID_BTN_SAVE = 5999,
			wxID_BTN_SELECT,
			wxID_BTN_CLOSE,
			wxID_BTN_SEL_Q1,
			wxID_BTN_SEL_Q2,
			wxID_BTN_SEL_Q3,
			wxID_BTN_SEL_Q4,
			wxID_BTN_NEXT,
			wxID_BTN_PREV,
			wxID_BTN_SET_KNOWS,
			wxID_BTN_CLR_CONTEXT,
			wxID_BTN_CLR_ALL_CONTEXT,
			wxID_EDIT_TILE_CONTEXT_AUTO,
			wxID_BTN_AUTO_SHADING,
			wxID_BTN_SAVE_CONTEXT,
			wxID_MM_NEW_TOOLSET,
			wxID_MM_NEW_TOOL,
			wxID_MM_REMOVE_TOOL,
			wxID_MM_EDIT_TOOLSET,
			wxID_TXT_ALT,
			wxID_LBOX_ALT,
			wxID_SLIDE_GAMMA,
			wxID_CB_ZOOM,
			wxID_LBOX_NEIGHBOR,
			wxID_CH_SIDE,
			wxID_CB_IS_GRASS,
			wxID_CB_IS_DGRASS,
			wxID_CB_IS_SAND,
			wxID_CB_IS_MUD,
			wxID_CB_IS_ASH,
			wxID_CB_IS_SWAMP,
			wxID_CB_IS_BLOOD,
			wxID_CB_IS_HIGH_LAND,
			wxID_CB_IS_WATER,
			wxID_CB_IS_RIDGE,
			wxID_CB_IS_CLIFF,
			wxID_CB_IS_ASH_ROAD,
			wxID_CB_IS_DIRT_ROAD,
			wxID_CB_IS_MUD_PATH,
			wxID_CB_IS_WALL_BASE,
			wxID_CB_IS_WBRIDGE,
			wxID_CB_IS_BRIDGE,
			wxID_CB_IS_FORD,
			wxID_CB_IS_RIDGE_BRIDGE,
			wxID_CB_IS_BROKEN,
			wxID_CB_IS_SHADOW,
			wxID_CB_IS_SCAR,
			wxID_CB_IS_OBJECT,
			wxID_CB_USE_AS_GLYPH,
			wxID_CB_FAULTY,
			wxID_CHB_Q1_CLASS,
			wxID_CB_Q1_NOFILT,
			wxID_CHB_Q2_CLASS,
			wxID_CB_Q2_NOFILT,
			wxID_CHB_Q3_CLASS,
			wxID_CB_Q3_NOFILT,
			wxID_CHB_Q4_CLASS,
			wxID_CB_Q4_NOFILT,
			wxID_CB_SHADE_Q1,
			wxID_CB_SHADE_Q2,
			wxID_CB_SHADE_Q3,
			wxID_CB_SHADE_Q4,
			wxID_CB_SHADE_C1,
			wxID_CB_SHADE_C2,
			wxID_CB_SHADE_C3,
			wxID_CB_SHADE_C4,
			wxID_EDIT_MAP_TILE_FLAGS,
			wxID_CB_TOOL_GLYPH,
			wxID_TREE_OBJECTS,

			wxID_LBOX_SPRITES
		};

		wxMenuBar* mMenu;
		wxMenu* mnuFile;
		wxMenu* mnuTerr;
		wxMenu* mnuEdit;
		wxMenu* mnuTools;
		wxStaticText* txtSpriteList;
		wxStaticText* txtAltList;
		wxListBox* lboxAlt;
		wxPanel* canvas;
		wxStaticText* txtGamma;
		wxSlider* slideGamma;
		wxCheckBox* cbZoom;
		wxStaticText* m_staticText2;
		wxListBox* lboxNeighbor;
		wxStaticText* m_staticText3;
		wxChoice* chbSide;
		wxCheckBox* cbIsGrass;
		wxCheckBox* cbIsDarkGrass;
		wxCheckBox* cbIsSand;
		wxCheckBox* cbIsMud;
		wxCheckBox* cbIsAsh;
		wxCheckBox* cbIsSwamp;
		wxCheckBox* cbIsBlood;
		wxCheckBox* cbIsHigh;
		wxCheckBox* cbIsWater;
		wxCheckBox* cbIsRidge;
		wxCheckBox* cbIsCliff;
		wxCheckBox* cbIsRoad;
		wxCheckBox* cbIsDirtRoad;
		wxCheckBox* cbIsMudPath;
		wxCheckBox* cbIsWallBase;
		wxCheckBox* cbIsWBridge;
		wxCheckBox* cbIsBridge;
		wxCheckBox* cbIsFord;
		wxCheckBox* cbIsRidgeBridge;
		wxCheckBox* cbIsBroken;
		wxCheckBox* cbIsShadow;
		wxCheckBox* cbIsScar;
		wxCheckBox* cbIsObject;
		wxStaticLine* m_staticline2;
		wxCheckBox* cbUseAsGlyph;
		wxCheckBox* cbFaultCont;
		wxStaticText* m_staticText5;
		wxChoice* chbQ1class;
		wxCheckBox* cbQ1nofilt;
		wxStaticText* m_staticText6;
		wxChoice* chbQ2class;
		wxCheckBox* cbQ2nofilt;
		wxStaticText* m_staticText7;
		wxChoice* chbQ3class;
		wxCheckBox* cbQ3nofilt;
		wxStaticText* m_staticText8;
		wxChoice* chbQ4class;
		wxCheckBox* cbQ4nofilt;
		wxStaticText* txtShadeEdge;
		wxCheckBox* cbShadeQ1;
		wxCheckBox* cbShadeQ2;
		wxCheckBox* cbShadeQ3;
		wxCheckBox* cbShadeQ4;
		wxCheckBox* cbShadeC1;
		wxCheckBox* cbShadeC2;
		wxCheckBox* cbShadeC3;
		wxCheckBox* cbShadeC4;
		wxStaticText* txtFlags;
		wxTextCtrl* editMapFlags;
		wxCheckBox* cbToolGlyph;
		wxTreeCtrl* treeCtrlObjects;
		wxStatusBar* statBar;

		wxListCtrlVirtual* lboxSprites;

		wxTreeItemId m_drag_item;
		class TreeNode : wxTreeItemData {
		public:
			Sprite* m_spr = NULL;
			int m_class_id = -1;
			int m_tool_id = -1;
			TreeNode(Sprite* spr) { m_spr = spr; };
			TreeNode(int class_id) { m_class_id = class_id; };
			TreeNode(int class_id,int tool_id) { m_class_id = class_id; m_tool_id = tool_id; };
		};

		wxImageList* imlist = NULL;
			

		std::string MakeToolsetTitle(std::string name,std::string desc);
		std::string GetToolsetTitle(int toolset_class_id);
		void RenameToolset(int toolset_class_id,std::string title);
		void FillToolsTree();
		wxString OnGetItemText(long item);
		int OnGetItemImage(long item);
		
		void OnDragSprite(wxListEvent& event);
		void OnDragSpriteEnd(wxTreeEvent& evt);
		void OnTreeClassBeginLabelEdit(wxTreeEvent& evt);
		void OnTreeClassEndLabelEdit(wxTreeEvent& evt);
		void OnTreeClassBeginDrag(wxTreeEvent& evt);
		void OnTreeClassEndDrag(wxTreeEvent& evt);
		void OnTreeSelectionChanged(wxTreeEvent& evt);
		void OnRemoveToolset(wxCommandEvent& evt);
		void OnNewToolset(wxCommandEvent& evt);
		void OnNewTool(wxCommandEvent& evt);
		void OnEditToolset(wxCommandEvent& evt);
		void OnTreeClassMenu(wxTreeEvent& evt);
		void OnTreeClassMenuClick(wxCommandEvent& evt);

		enum TreeMenu{
			MNU_REMOVE = 0,
			MNU_NEW_TOOL,
			MNU_NEW_TOOLSET,
			MNU_EDIT_TOOLSET
		};

		class SpriteDropTarget : public wxTextDropTarget {
		public:
			FormSprite* m_owner;
			SpriteDropTarget(FormSprite* owner) { m_owner = owner; };
			bool OnDropText(wxCoord x,wxCoord y,const wxString& data);
		};
		

	public:

		FormSprite( wxWindow* parent, SpellData *spell_data, wxWindowID id = wxID_ANY, const wxString& title = wxT("Sprite viewer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1250,700 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL|wxFRAME_FLOAT_ON_PARENT);
		~FormSprite();
		
		void SetSprite(Terrain* terr,Sprite* spr=NULL);
		Terrain *GetSelectedTerrain();
		Sprite *GetSelectedSprite();
		bool wasSet();

		public: enum Icons {
			MULTI = 0,
			SINGLE,
			FOLDER,
			FOLDER_OPEN,
			SPR_A,
			SPR_OBJ
		};
};

