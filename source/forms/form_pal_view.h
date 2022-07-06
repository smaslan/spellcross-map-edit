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
#include <wx/panel.h>
#include <wx/slider.h>
#include <wx/sizer.h>
#include <wx/statusbr.h>
#include <wx/timer.h>
#include <wx/frame.h>

#include "spellcross.h"
#include "sprites.h"
#include "map.h"
#include "spell_filter.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FromPalView
///////////////////////////////////////////////////////////////////////////////
class FormPalView : public wxFrame
{
	
	private:

		SpellData* spell_data;
		SpellMap* spell_map;

		int sel_pos_x;
		int relative_time;

		SpellFilterRec *filter;

		static constexpr int TERR_ID0 = 990;
		static constexpr int FILTER_ID0 = 900;

		void OnClose(wxCloseEvent& ev);
		void OnCloseClick(wxCommandEvent& event);
		void OnPaintCanvas(wxPaintEvent& event);
		void OnPaintColor(wxPaintEvent& event);
		void OnTerrainChange(wxCommandEvent& event);
		void OnFilterChange(wxCommandEvent& event);
		void OnCanvasMouseMove(wxMouseEvent& event);
		void OnCanvasMouseLeave(wxMouseEvent& event);
		void OnTimer(wxTimerEvent& event);
		void OnChangeFilterRGB(wxCommandEvent& event);
		void OnSaveFilterFile(wxCommandEvent& event);

		Terrain* FindTerrain();
		//void SelectTerrain();
		void ListFilters();


	protected:
		enum
		{
			wxID_FRM_PAL_VIEW = 1000,
			wxID_MM_CLOSE,
			wxID_CANVAS,
			wxID_COLOR,
			wxID_STAT_BAR,
			wxID_TIMER
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

		FormPalView(wxWindow* parent,SpellData* spell_data,wxWindowID id = wxID_FRM_PAL_VIEW,const wxString& title = wxT("Spellcross palette viewer"),const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxSize(950,300),long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL|wxFRAME_FLOAT_ON_PARENT);
		~FormPalView();

		void SetMap(SpellMap* map);

};
