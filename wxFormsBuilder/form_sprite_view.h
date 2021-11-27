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
#include <wx/slider.h>
#include <wx/checkbox.h>
#include <wx/statline.h>
#include <wx/choice.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define wxID_EDIT_TILE_CONTEXT_AUTO 1000
#define wxID_LBOX_SPRITES 1001
#define wxID_SLIDE_GAMMA 1002
#define wxID_CB_ZOOM 1003
#define wxID_LBOX_NEIGHBOR 1004
#define wxID_CH_SIDE 1005
#define wxID_CB_IS_GRASS 1006
#define wxID_CB_IS_DGRASS 1007
#define wxID_CB_IS_BLOOD 1008
#define wxID_CB_IS_MUD 1009
#define wxID_CB_IS_SWAMP 1010
#define wxID_CB_IS_ASH 1011
#define wxID_CB_IS_HIGH_LAND 1012
#define wxID_CB_IS_ASH_ROAD 1013
#define wxID_CB_IS_BROKE_ASH_ROAD 1014
#define wxID_CB_IS_DIRT_ROAD 1015
#define wxID_CB_IS_MUD_PATH 1016
#define wxID_CB_IS_CLIFF 1017
#define wxID_CB_IS_WATER 1018
#define wxID_CB_IS_WBRIDGE 1019
#define wxID_CB_IS_BRIDGE 1020
#define wxID_CB_IS_FORD 1021
#define wxID_CHB_Q1_CLASS 1022
#define wxID_CHB_Q2_CLASS 1023
#define wxID_CHB_Q3_CLASS 1024
#define wxID_CHB_Q4_CLASS 1025

///////////////////////////////////////////////////////////////////////////////
/// Class FormSprite
///////////////////////////////////////////////////////////////////////////////
class FormSprite : public wxFrame
{
	private:

	protected:
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
		wxStaticText* m_staticText5;
		wxChoice* chbQ1class;
		wxStaticText* m_staticText6;
		wxChoice* chbQ2class;
		wxStaticText* m_staticText7;
		wxChoice* chbQ3class;
		wxStaticText* m_staticText8;
		wxChoice* chbQ4class;

	public:

		FormSprite( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Sprite viewer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1001,508 ), long style = wxDEFAULT_FRAME_STYLE|wxSTAY_ON_TOP|wxTAB_TRAVERSAL );

		~FormSprite();

};

