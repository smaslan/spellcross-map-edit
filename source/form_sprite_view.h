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

#include "spellcross.h"
#include "sprites.h"

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class FormSprite
///////////////////////////////////////////////////////////////////////////////
class FormSprite : public wxFrame
{
	private:
		SpellData *spell_data;
			
		void OnClose(wxCommandEvent& event);
		void OnTerrainChange(wxCommandEvent& event);
		void OnSelectSprite(wxCommandEvent& event);
		void OnSelectSpriteBtn(wxCommandEvent& event);
		void OnSelectNeighbor(wxCommandEvent& event);
		void OnCanvasRepaint(wxPaintEvent& event);
		void OnChangeGamma(wxCommandEvent& event);
		void OnChangeZoom(wxCommandEvent& event);
		void OnQchange(wxCommandEvent& event);

		void OnFlagsChange(wxCommandEvent& event);
		void OnEdgeClassChange(wxCommandEvent& event);
		void OnEdgeShadeChange(wxCommandEvent& event);

		void OnUpdateContext(wxCommandEvent& event);
		void OnAutoShadeFlags(wxCommandEvent& event);
		void OnSaveTileContext(wxCommandEvent& event);

		void SelectTerrain();
		void SelectQuad();
		void SetFlags();
		void SetEdgeClasses();
		void SetShadingFlags();
		Terrain* FindTerrain();


		// tile edge classes
		static constexpr struct {
			int cls;
			const char* name;
		} class_list[] = {
			{SpriteContext::CLASS_GENERIC, "Generic"},
			{SpriteContext::CLASS_GRASS, "Grass"},
			{SpriteContext::CLASS_DARK_GRASS, "Dark grass"},
			{SpriteContext::CLASS_MUD, "Mud"},
			{SpriteContext::CLASS_SWAMP, "Swamp"},
			{SpriteContext::CLASS_BLOOD, "Blood"},
			{SpriteContext::CLASS_ASH, "Ash"},
			{SpriteContext::CLASS_HIGH_LAND, "High land"},
			{SpriteContext::CLASS_SAND, "Sand"},
			{0,NULL}
		};

		static constexpr int TERR_ID0 = 990;

	protected:
		enum
		{
			wxID_BTN_CLOSE = 1000,
			wxID_BTN_NEXT,
			wxID_BTN_PREV,
			wxID_EDIT_TILE_CONTEXT_AUTO,
			wxID_BTN_AUTO_SHADING,
			wxID_BTN_SAVE_CONTEXT,
			wxID_LBOX_SPRITES,
			wxID_SLIDE_GAMMA,
			wxID_CB_ZOOM,
			wxID_LBOX_NEIGHBOR,
			wxID_CH_SIDE,
			wxID_CB_IS_GRASS,
			wxID_CB_IS_DGRASS,
			wxID_CB_IS_BLOOD,
			wxID_CB_IS_MUD,
			wxID_CB_IS_SWAMP,
			wxID_CB_IS_ASH,
			wxID_CB_IS_HIGH_LAND,
			wxID_CB_IS_ASH_ROAD,
			wxID_CB_IS_BROKE_ASH_ROAD,
			wxID_CB_IS_DIRT_ROAD,
			wxID_CB_IS_MUD_PATH,
			wxID_CB_IS_CLIFF,
			wxID_CB_IS_WATER,
			wxID_CB_IS_WBRIDGE,
			wxID_CB_IS_BRIDGE,
			wxID_CB_IS_FORD,
			wxID_CB_IS_SAND,
			wxID_CHB_Q1_CLASS,
			wxID_CHB_Q2_CLASS,
			wxID_CHB_Q3_CLASS,
			wxID_CHB_Q4_CLASS,
			wxID_CB_SHADE_Q1,
			wxID_CB_SHADE_Q2,
			wxID_CB_SHADE_Q3,
			wxID_CB_SHADE_Q4,
			wxID_CB_SHADE_C1,
			wxID_CB_SHADE_C2,
			wxID_CB_SHADE_C3,
			wxID_CB_SHADE_C4
		};

		wxMenuBar* mMenu;
		wxMenu* mnuFile;
		wxMenu* mnuTerr;
		wxMenu* mnuEdit;
		wxStaticText* txtSpriteList;
		wxListBox* lboxSprites;
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
		wxCheckBox* cbIsBlood;
		wxCheckBox* cbIsMud;
		wxCheckBox* cbIsSwamp;
		wxCheckBox* cbIsAsh;
		wxCheckBox* cbIsHigh;
		wxCheckBox* cbIsRoad;
		wxCheckBox* cbIsBrokeAshroad;
		wxCheckBox* cbIsDirtRoad;
		wxCheckBox* cbIsMudPath;
		wxCheckBox* cbIsCliff;
		wxCheckBox* cbIsWater;
		wxCheckBox* cbIsWBridge;
		wxCheckBox* cbIsBridge;
		wxCheckBox* cbIsFord;
		wxCheckBox* cbIsSand;
		wxStaticText* m_staticText5;
		wxChoice* chbQ1class;
		wxStaticText* m_staticText6;
		wxChoice* chbQ2class;
		wxStaticText* m_staticText7;
		wxChoice* chbQ3class;
		wxStaticText* m_staticText8;
		wxChoice* chbQ4class;
		wxStaticText* txtShadeEdge;
		wxCheckBox* cbShadeQ1;
		wxCheckBox* cbShadeQ2;
		wxCheckBox* cbShadeQ3;
		wxCheckBox* cbShadeQ4;
		wxCheckBox* cbShadeC1;
		wxCheckBox* cbShadeC2;
		wxCheckBox* cbShadeC3;
		wxCheckBox* cbShadeC4;
		wxStatusBar* statBar;

	public:

		FormSprite( wxWindow* parent, SpellData *spell_data, wxWindowID id = wxID_ANY, const wxString& title = wxT("Sprite viewer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 717,508 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL|wxSTAY_ON_TOP);
		~FormSprite();

};

