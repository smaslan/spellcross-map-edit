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
#include <wx/checkbox.h>
#include <wx/panel.h>
#include <wx/statbox.h>
#include <wx/slider.h>
#include <wx/frame.h>
#include <wx/timer.h>

#include "spellcross.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormANM
///////////////////////////////////////////////////////////////////////////////
class FormANM : public wxFrame
{
	private:

		static constexpr int TERR_ID0 = 7000;

		SpellData *m_spell_data;
		Terrain *m_terrain;
		AnimL1 *m_anim;

		wxTimer m_timer;

		void OnClose(wxCloseEvent& ev);
		void OnCloseClick(wxCommandEvent& event);
		void OnSelectClick(wxCommandEvent& event);
		void OnTerrainChange(wxCommandEvent& event);
		void OnSelectANM(wxCommandEvent& event);
		void OnSelectFrame(wxCommandEvent& event);
		void OnCanvasRepaint(wxPaintEvent& event);
		void OnChangeZoom(wxCommandEvent& event);
		void OnChangeGamma(wxCommandEvent& event);
		void OnTimer(wxTimerEvent& event);
		
		Terrain* FindTerrain();
		void SelectTerrain();
		void SelectANM();
		
	protected:
		enum
		{
			wxID_MM_SELECT = 5999,
			wxID_MM_CLOSE,
			wxID_LB_LIST,
			wxID_TXT_FRAMES,
			wxID_LB_FRAMES,
			wxID_CB_ANIM,
			wxID_CANVAS,
			wxID_TXT_GAMMA,
			wxID_SLIDE_GAMMA,
			wxID_CB_ZOOM,
		};

		wxMenuBar* m_menubar10;
		wxMenu* m_menu19;
		wxMenu* mmTerrain;
		wxStaticText* m_staticText93;
		wxListBox* lbList;
		wxStaticText* txtFrames;
		wxListBox* lbFrames;
		wxCheckBox* cbAnimate;
		wxPanel* canvas;
		wxStaticText* txtGamma;
		wxSlider* slideGamma;
		wxCheckBox* cbZoom;

	public:

		FormANM( wxWindow* parent,SpellData* spell_data,wxWindowID id = wxID_ANY, const wxString& title = wxT("Terrain layer animations ANM"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 700,500 ), long style = wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER|wxSTAY_ON_TOP|wxTAB_TRAVERSAL );
		~FormANM();
		Terrain* GetSelectedTerrain();
		AnimL1* GetSelectedAnim();

};

