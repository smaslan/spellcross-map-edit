///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
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
#include <wx/slider.h>
#include <wx/checkbox.h>
#include <wx/statline.h>
#include <wx/choice.h>
#include <wx/textctrl.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/timer.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/notebook.h>
#include <wx/radiobut.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormSprite
///////////////////////////////////////////////////////////////////////////////
class FormSprite : public wxFrame
{
	private:

	protected:
		enum
		{
			wxID_BTN_CLOSE = 5999,
			wxID_BTN_SEL_Q1,
			wxID_BTN_SEL_Q2,
			wxID_BTN_SEL_Q3,
			wxID_BTN_SEL_Q4,
			wxID_BTN_NEXT,
			wxID_BTN_PREV,
			wxID_BTN_SET_KNOWS,
			wxID_BTN_CLR_CONTEXT,
			wxID_EDIT_TILE_CONTEXT_AUTO,
			wxID_BTN_AUTO_SHADING,
			wxID_BTN_SAVE_CONTEXT,
			wxID_LBOX_SPRITES,
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
			wxID_CHB_TOOL_CLASS,
			wxID_CHB_TOOL_OBJ_GROUP,
			wxID_CB_TOOL_GLYPH,
		};

		wxMenuBar* mMenu;
		wxMenu* mnuFile;
		wxMenu* mnuTerr;
		wxMenu* mnuEdit;
		wxStaticText* txtSpriteList;
		wxListBox* lboxSprites;
		wxStaticText* txtAltList;
		wxListBox* lboxAlt;
		wxPanel* canvas;
		wxStaticText* txtGamma;
		wxSlider* slideGamma;
		wxCheckBox* cbZoom;
		wxStaticText* m_staticText2;
		wxListBox* lboxNeighbor;
		wxStaticLine* m_staticline1;
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
		wxStaticText* m_staticText13;
		wxChoice* chbToolClass;
		wxStaticText* m_staticText14;
		wxChoice* chbToolObjGroup;
		wxCheckBox* cbToolGlyph;
		wxStatusBar* statBar;

	public:
		int tile_flags;

		FormSprite( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Sprite viewer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1250,640 ), long style = wxDEFAULT_FRAME_STYLE|wxFRAME_FLOAT_ON_PARENT|wxTAB_TRAVERSAL );

		~FormSprite();

};

///////////////////////////////////////////////////////////////////////////////
/// Class FormObjects
///////////////////////////////////////////////////////////////////////////////
class FormObjects : public wxFrame
{
	private:

	protected:
		enum
		{
			wxID_SB_MAIN = 5999,
			wxID_LB_OBJECTS,
			wxID_BTN_UP,
			wxID_BTN_DOWN,
			wxID_CANVAS,
			wxID_TXT_GAMMA,
			wxID_SLIDE_GAMMA,
			wxID_CHB_CLASS,
			wxID_CHB_GROUP,
			wxID_MM_SAVE_OBJECTS,
			wxID_MM_CLOSE,
			wxID_MM_NEXT,
			wxID_MM_PREV,
			wxID_MM_REMOVE,
			wxID_MM_RENAME,
		};

		wxStatusBar* sbar;
		wxStaticText* m_staticText14;
		wxListBox* lbObjects;
		wxButton* btnUp;
		wxButton* btnDown;
		wxStaticLine* m_staticline5;
		wxStaticText* m_staticText15;
		wxPanel* canvas;
		wxStaticLine* m_staticline8;
		wxStaticText* m_staticText19;
		wxTextCtrl* txtName;
		wxStaticLine* m_staticline9;
		wxStaticText* txtGamma;
		wxSlider* slideGamma;
		wxStaticLine* m_staticline6;
		wxStaticText* m_staticText17;
		wxChoice* chbObjectClass;
		wxStaticText* m_staticText18;
		wxChoice* chbObjectsGroup;
		wxMenuBar* m_menubar2;
		wxMenu* mnuFile;
		wxMenu* mnuTerr;
		wxMenu* mnuEdit;

	public:

		FormObjects( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Objects viewer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1000,600 ), long style = wxDEFAULT_FRAME_STYLE|wxFRAME_FLOAT_ON_PARENT|wxTAB_TRAVERSAL );

		~FormObjects();

};

///////////////////////////////////////////////////////////////////////////////
/// Class FormNewObject
///////////////////////////////////////////////////////////////////////////////
class FormNewObject : public wxDialog
{
	private:

	protected:
		enum
		{
			wxID_TXT_DESC = 5999,
			wxID_BTN_OK,
		};

		wxStaticText* m_staticText11;
		wxTextCtrl* txtDescription;
		wxStaticLine* m_staticline4;
		wxButton* btnOk;

	public:

		FormNewObject( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("New object"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 400,120 ), long style = wxDEFAULT_DIALOG_STYLE );

		~FormNewObject();

};

///////////////////////////////////////////////////////////////////////////////
/// Class FormTools
///////////////////////////////////////////////////////////////////////////////
class FormTools : public wxFrame
{
	private:

	protected:
		enum
		{
			wxID_LB_TOOLSET = 5999,
			wxID_TXT_NEW_TOOLSET,
			wxID_TXT_TOOLSET_TITLE,
			wxID_CHB_SCALE,
			wxID_SC_WIDTH,
			wxID_SC_HEIGHT,
			wxID_BTN_TOOLSET_NEW,
			wxID_BTN_TOOLSET_RENAME,
			wxID_BTN_TOOLSET_REM,
			wxID_BTN_TOOLSET_UP,
			wxID_BTN_TOOLSET_DOWN,
			wxID_LB_TOOL,
			wxID_BTN_TOOL_NEW,
			wxID_BTN_TOOL_RENAME,
			wxID_BTN_TOOL_REM,
			wxID_BTN_TOOL_UP,
			wxID_BTN_TOOL_DOWN,
			wxID_MM_SAVE,
			wxID_MM_CLOSE,
			wxID_MM_CLR_TOOLSETS,
			wxID_CLR_TOOLS,
		};

		wxStatusBar* sbar;
		wxStaticText* m_staticText19;
		wxListBox* lbToolset;
		wxStaticText* m_staticText21;
		wxTextCtrl* txtNewToolset;
		wxStaticText* m_staticText23;
		wxTextCtrl* txtToolsetTitle;
		wxStaticText* m_staticText26;
		wxChoice* chbScaling;
		wxStaticText* m_staticText27;
		wxSpinCtrl* scWidth;
		wxStaticText* m_staticText28;
		wxSpinCtrl* scHeight;
		wxButton* btnAddToolset;
		wxButton* btnRenameToolset;
		wxButton* btnToolsetDel;
		wxButton* btnToolsetUp;
		wxButton* btnToolsetDown;
		wxStaticText* m_staticText20;
		wxListBox* lbTool;
		wxStaticText* m_staticText22;
		wxTextCtrl* txtNewTool;
		wxButton* btnNewTool;
		wxButton* btnRenameTool;
		wxButton* btnToolDel;
		wxButton* btnToolUp;
		wxButton* btnToolDown;
		wxMenuBar* mmBar;
		wxMenu* mmFile;
		wxMenu* mmTerrain;
		wxMenu* mmEdit;

	public:

		FormTools( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Tools editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 870,518 ), long style = wxDEFAULT_FRAME_STYLE|wxFRAME_FLOAT_ON_PARENT|wxTAB_TRAVERSAL );

		~FormTools();

};

///////////////////////////////////////////////////////////////////////////////
/// Class FormPalView
///////////////////////////////////////////////////////////////////////////////
class FormPalView : public wxFrame
{
	private:

	protected:
		enum
		{
			wxID_FRM_PAL_VIEW = 5999,
			wxID_MM_CLOSE,
			wxID_CANVAS,
			wxID_COLOR,
			wxID_STAT_BAR,
			wxID_TIMER,
		};

		wxMenuBar* mMenu;
		wxMenu* mmFile;
		wxMenu* mmTerrain;
		wxMenu* mmFilter;
		wxStaticText* m_staticText28;
		wxPanel* canvas;
		wxStaticText* m_staticText29;
		wxPanel* color;
		wxStaticText* txtFilterRed;
		wxSlider* slideRed;
		wxStaticText* txtFilterGreen;
		wxSlider* slideGreen;
		wxStaticText* txtFilterBlue;
		wxSlider* slideBlue;
		wxStatusBar* sbar;
		wxTimer timer;

	public:

		FormPalView( wxWindow* parent, wxWindowID id = wxID_FRM_PAL_VIEW, const wxString& title = wxT("Spellcross palette viewer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 950,300 ), long style = wxDEFAULT_FRAME_STYLE|wxFRAME_FLOAT_ON_PARENT|wxTAB_TRAVERSAL );

		~FormPalView();

};

///////////////////////////////////////////////////////////////////////////////
/// Class FormGResView
///////////////////////////////////////////////////////////////////////////////
class FormGResView : public wxFrame
{
	private:

	protected:
		enum
		{
			wxID_MM_CLOSE = 5999,
			wxID_SB,
			wxID_LB_FILES,
			wxID_SPIN_W,
			wxID_CANVAS,
		};

		wxMenuBar* m_menubar5;
		wxMenu* mmFile;
		wxStatusBar* sbar;
		wxStaticText* m_staticText31;
		wxListBox* lboxFiles;
		wxStaticText* m_staticText32;
		wxSpinCtrl* spinWidth;
		wxStaticLine* m_staticline8;
		wxPanel* canvas;

	public:

		FormGResView( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Graphics resource viewer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 875,513 ), long style = wxDEFAULT_FRAME_STYLE|wxFRAME_FLOAT_ON_PARENT|wxTAB_TRAVERSAL );

		~FormGResView();

};

///////////////////////////////////////////////////////////////////////////////
/// Class FormUnits
///////////////////////////////////////////////////////////////////////////////
class FormUnits : public wxFrame
{
	private:

	protected:
		enum
		{
			wxID_SB = 5999,
			wxID_MM_SAVE_AUX,
			wxID_MM_EXIT,
			wxID_MM_COPY_GRP_ORG,
			wxID_MM_PASTE_GRP_ORG,
			wxID_MM_SET,
			wxID_LB_UNITS,
			wxID_CANVAS_ICON,
			wxID_PROPS,
			wxID_SPIN_ID,
			wxID_NAME,
			wxID_SPIN_HEALTH,
			wxID_UNIT_BEHAVE,
			wxID_UNIT_TYPE,
			wxID_PAGE_CTRL,
			wxID_PAGE_ART,
			wxID_CANVAS_ART,
			wxID_LB_ART,
			wxID_TXT_INFO,
			wxID_PAGE_GRP,
			wxID_CANVAS_GRP,
			wxID_CHB_GRP_TYPE,
			wxID_TXT_GRP_AZIM,
			wxID_SLIDE_AZIM,
			wxID_TXT_GRP_SLOPE,
			wxID_SLIDE_SLOPE,
			wxID_SLIDE_FRAME,
			wxID_SLIDE_GRP_ZOOM,
			wxID_SLIDE_GRP_GAMMA,
			wxID_CB_GRP_ORG,
			wxID_CB_GRP_FIRE_ORG,
			wxID_CB_GRP_FIRE_ORG_MEAN,
			wxID_CB_GRP_FIRE_CENTER,
		};

		wxStatusBar* sbar;
		wxMenuBar* mmenu;
		wxMenu* mmFile;
		wxMenu* mmEdit;
		wxStaticText* m_staticText32;
		wxListBox* lboxUnits;
		wxStaticLine* m_staticline9;
		wxStaticText* m_staticText35;
		wxPanel* icon_canvas;
		wxStaticText* m_staticText34;
		wxPropertyGrid* props;
		wxStaticLine* m_staticline13;
		wxStaticText* m_staticText60;
		wxSpinCtrl* spinID;
		wxStaticText* m_staticText39;
		wxTextCtrl* txtName;
		wxStaticText* m_staticText40;
		wxSpinCtrl* spinHealth;
		wxStaticText* m_staticText75;
		wxSlider* slideXP;
		wxStaticText* m_staticText401;
		wxChoice* chUnitBehave;
		wxStaticText* m_staticText74;
		wxChoice* chUnitType;
		wxStaticLine* m_staticline10;
		wxNotebook* pages;
		wxPanel* pageArt;
		wxStaticText* m_staticText36;
		wxPanel* art_canvas;
		wxStaticLine* m_staticline11;
		wxStaticText* m_staticText37;
		wxListBox* lboxArt;
		wxStaticLine* m_staticline12;
		wxStaticText* m_staticText38;
		wxTextCtrl* txtInfo;
		wxPanel* pageGrp;
		wxStaticText* m_staticText42;
		wxPanel* grp_canvas;
		wxStaticLine* m_staticline14;
		wxStaticText* m_staticText43;
		wxChoice* chbGrpType;
		wxStaticText* txtGrpZim;
		wxSlider* slideAzim;
		wxStaticLine* m_staticline15;
		wxStaticText* txtGrpSlope;
		wxSlider* slideSlope;
		wxStaticLine* m_staticline16;
		wxStaticText* m_staticText46;
		wxSlider* slideFrame;
		wxStaticLine* m_staticline17;
		wxStaticText* txtGrpZoom;
		wxSlider* slideGrpZoom;
		wxStaticText* txtGrpGamma;
		wxSlider* slideGrpGamma;
		wxStaticLine* m_staticline18;
		wxCheckBox* cbGrpOrigin;
		wxCheckBox* cbGrpFireOrg;
		wxCheckBox* cbGrpFireMean;
		wxCheckBox* cbGrpFireCenter;
		wxListBox* m_listBox11;

	public:

		FormUnits( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Units viewer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1099,710 ), long style = wxDEFAULT_FRAME_STYLE|wxSTAY_ON_TOP|wxTAB_TRAVERSAL );

		~FormUnits();

};

///////////////////////////////////////////////////////////////////////////////
/// Class FormEvent
///////////////////////////////////////////////////////////////////////////////
class FormEvent : public wxFrame
{
	private:

	protected:
		enum
		{
			wxID_MM_OK = 5999,
			wxID_MM_EXIT,
			wxID_LB_EVENTS,
			wxID_BTN_ADD_EVENT,
			wxID_BTN_REM_EVENT,
			wxID_CHB_TYPE,
			wxID_TXT_OBJ_DESC,
			wxID_CB_IS_OBJECTIVE,
			wxID_SPIN_XPOS,
			wxID_SPIN_YPOS,
			wxID_SPIN_TRIG_UNIT,
			wxID_SPIN_PROB,
			wxID_TXT_UNITS,
			wxID_CAN_ANIM,
			wxID_CHB_MSG_ITEM,
			wxID_BTN_NEW_MSG,
			wxID_BNT_DEL_MSG,
			wxID_BTN_MSG_UP,
			wxID_BTN_MSG_DOWN,
			wxID_LB_MSG,
			wxID_TXT_MSG,
			wxID_BTN_PLAY_MSG,
			wxID_BTN_STOP_MSG,
		};

		wxMenuBar* mMenu;
		wxMenu* mmFile;
		wxStaticText* m_staticText57;
		wxListBox* lbEvents;
		wxButton* btnAddEvent;
		wxButton* btnRemoveEvent;
		wxStaticLine* m_staticline21;
		wxStaticText* m_staticText50;
		wxChoice* chbType;
		wxStaticText* m_staticText62;
		wxTextCtrl* txtObjectiveDesc;
		wxCheckBox* cbIsObjective;
		wxStaticText* m_staticText52;
		wxSpinCtrl* spinXpos;
		wxStaticText* m_staticText54;
		wxSpinCtrl* spinYpos;
		wxStaticText* m_staticText59;
		wxSpinCtrl* spinTrigUnit;
		wxStaticText* m_staticText51;
		wxSpinCtrl* spinProb;
		wxStaticLine* m_staticline20;
		wxStaticText* strUnits;
		wxTextCtrl* txtUnits;
		wxStaticLine* m_staticline22;
		wxStaticText* m_staticText74;
		wxChoice* chbCANanim;
		wxStaticLine* m_staticline221;
		wxStaticText* m_staticText53;
		wxChoice* chbMsgItem;
		wxButton* btnNewMsg;
		wxButton* btnDelMsg;
		wxButton* btnMsgUp;
		wxButton* btnMsgDown;
		wxStaticText* m_staticText55;
		wxListBox* lbMsg;
		wxStaticLine* m_staticline19;
		wxStaticText* m_staticText56;
		wxTextCtrl* txtMessage;
		wxButton* btnMsgPlay;
		wxButton* btnMsgStop;
		wxStatusBar* sBar;

	public:

		FormEvent( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Event editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 915,605 ), long style = wxDEFAULT_FRAME_STYLE|wxFRAME_FLOAT_ON_PARENT|wxTAB_TRAVERSAL );

		~FormEvent();

};

///////////////////////////////////////////////////////////////////////////////
/// Class FormLoader
///////////////////////////////////////////////////////////////////////////////
class FormLoader : public wxDialog
{
	private:

	protected:
		enum
		{
			wxID_TXT_LIST = 5999,
			wxID_TXT_ITEM,
			wxID_BTN_OK,
		};

		wxStaticText* m_staticText63;
		wxTextCtrl* txtList;
		wxStaticText* m_staticText62;
		wxTextCtrl* txtItem;
		wxButton* btnOK;

	public:

		FormLoader( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Loading Spellcross Data..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 550,476 ), long style = wxCAPTION|wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP );

		~FormLoader();

};

///////////////////////////////////////////////////////////////////////////////
/// Class FormVideo
///////////////////////////////////////////////////////////////////////////////
class FormVideo : public wxFrame
{
	private:

	protected:
		enum
		{
			wxID_MM_OPEN = 5999,
			wxID_MM_SAVE_FRAMES,
			wxID_MM_EXIT,
			wxID_CANVAS,
			wxID_DATA_POS,
			wxID_SLIDE_OFS,
			wxID_BTN_PLAY,
			wxID_BTN_STOP,
		};

		wxMenuBar* mmenu;
		wxMenu* mmFile;
		wxStaticText* m_staticText64;
		wxPanel* canvas;
		wxStaticText* m_staticText65;
		wxSlider* slideOffset;
		wxButton* btnPlay;
		wxButton* btnStop;
		wxStatusBar* m_statusBar8;

	public:

		FormVideo( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Spellcross Video Viewer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 703,421 ), long style = wxDEFAULT_FRAME_STYLE|wxFRAME_FLOAT_ON_PARENT|wxTAB_TRAVERSAL );

		~FormVideo();

};

///////////////////////////////////////////////////////////////////////////////
/// Class FormMIDI
///////////////////////////////////////////////////////////////////////////////
class FormMIDI : public wxFrame
{
	private:

	protected:
		enum
		{
			wxID_WIN_MIDI = 5999,
			wxID_MM_CUST_FOLDER,
			wxID_MM_SAVE_MIDI,
			wxID_MM_SAVE_ALL,
			wxID_MM_EXIT,
			wxID_LB_LIST,
			wxID_RB_SRC_MUSIC_FS,
			wxID_RB_SRC_FOLDER,
			wxID_BTN_SRC_FOLDER,
			wxID_TXT_INFO,
			wxID_SLIDE_VOLUME,
			wxID_BTN_STOP,
			wxID_BTN_PLAY,
			wxID_BTN_OK,
		};

		wxMenuBar* mmenu;
		wxMenu* mmFile;
		wxStaticText* labelList;
		wxListBox* lbList;
		wxStaticText* m_staticText69;
		wxRadioButton* rbSrcMusicFS;
		wxRadioButton* rbSrcFolder;
		wxButton* btnSrcFolder;
		wxStaticLine* m_staticline24;
		wxStaticText* labelInfo;
		wxTextCtrl* txtInfo;
		wxStaticText* m_staticText68;
		wxSlider* slideVolume;
		wxStaticLine* m_staticline26;
		wxButton* btnStop;
		wxButton* btnPlay;
		wxStaticLine* m_staticline23;
		wxButton* btnOK;

	public:

		FormMIDI( wxWindow* parent, wxWindowID id = wxID_WIN_MIDI, const wxString& title = wxT("Spellcross MIDI player"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,500 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~FormMIDI();

};

///////////////////////////////////////////////////////////////////////////////
/// Class FormMissionParams
///////////////////////////////////////////////////////////////////////////////
class FormMissionParams : public wxDialog
{
	private:

	protected:
		enum
		{
			wxID_WIN_MISSION_PARAMS = 5999,
			wxID_CH_TEXT,
			wxID_CH_START_TEXT,
			wxID_CH_GOOD_END_TEXT,
			wxID_CH_BAD_END_TEXT,
			wxID_CB_NIGHT,
			wxID_TEXT_PREVIEW,
			wxID_BTN_OK,
		};

		wxStaticText* m_staticText75;
		wxChoice* chText;
		wxStaticText* m_staticText76;
		wxChoice* chStartText;
		wxStaticText* m_staticText77;
		wxChoice* chGoodEndText;
		wxStaticText* m_staticText78;
		wxChoice* chBadEndText;
		wxCheckBox* cbNight;
		wxStaticLine* m_staticline28;
		wxStaticText* txtPrevLbl;
		wxTextCtrl* txtPreview;
		wxStaticLine* m_staticline27;
		wxButton* btnOK;

	public:

		FormMissionParams( wxWindow* parent, wxWindowID id = wxID_WIN_MISSION_PARAMS, const wxString& title = wxT("Mission Parameters"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 593,388 ), long style = wxDEFAULT_DIALOG_STYLE );

		~FormMissionParams();

};

