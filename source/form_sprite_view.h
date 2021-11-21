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

///////////////////////////////////////////////////////////////////////////

#define wxID_LBOX_SPRITES 1000
#define wxID_SLIDE_GAMMA 1001
#define wxID_CB_ZOOM 1002
#define wxID_LBOX_NEIGHBOR 1003
#define wxID_CH_SIDE 1004

///////////////////////////////////////////////////////////////////////////////
/// Class FormSprite
///////////////////////////////////////////////////////////////////////////////
class FormSprite : public wxFrame
{
	private:
		SpellData *spell_data;

		static constexpr int TERR_ID0 = 1000;

		void OnTerrainChange(wxCommandEvent& event);
		void OnSelectSprite(wxCommandEvent& event);
		void OnSelectNeighbor(wxCommandEvent& event);
		void OnCanvasRepaint(wxPaintEvent& event);
		void OnChangeGamma(wxCommandEvent& event);
		void OnChangeZoom(wxCommandEvent& event);
		void OnQchange(wxCommandEvent& event);

		void SelectTerrain();
		void SelectQuad();
		Terrain* FindTerrain();


	protected:
		wxMenuBar* mMenu;
		wxMenu* mnuFile;
		wxMenu* mnuTerr;
		wxStaticText* txtSpriteList;
		wxListBox* lboxSprites;
		wxPanel* canvas;
		wxStaticText* txtGamma;
		wxSlider* slideGamma;
		wxStaticText* m_staticText2;
		wxListBox* lboxNeighbor;
		wxStaticLine* m_staticline1;
		wxStaticText* m_staticText3;
		wxChoice* chbSide;
		wxCheckBox* cbZoom;

	public:

		FormSprite( wxWindow* parent, SpellData *spell_data, wxWindowID id = wxID_ANY, const wxString& title = wxT("Sprite viewer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 717,508 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL|wxSTAY_ON_TOP);
		~FormSprite();

};

