///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

// <wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormANM' on 2026-06-20 10:21:33
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
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/listbox.h>
#include <wx/panel.h>
#include <wx/statbox.h>
#include <wx/slider.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/statline.h>
#include <wx/treectrl.h>
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
#include <wx/scrolbar.h>
#include <wx/bmpbuttn.h>

// </wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormANM' on 2026-06-20 10:21:33

#include <tuple>

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
		AnimPNM *m_pnm;
		bool m_anm_was_set;
		bool m_is_pnm;
		bool m_is_common;
		bool m_ok;

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
		void OnExportClick(wxCommandEvent& event);
		
		void SetTerrain(Terrain* terr);
		Terrain* FindTerrain();
		void SelectTerrain();
		void SelectANM();
		
	protected:
		
		// <wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormANM' on 2026-06-20 10:21:33
		enum
		{
			wxID_MM_EXPRT = 5999,
			wxID_MM_SELECT,
			wxID_MM_CLOSE,
			wxID_CH_SRC,
			wxID_TXT_FILTER,
			wxID_TXT_RESOURCES,
			wxID_LB_LIST,
			wxID_TXT_FRAMES,
			wxID_LB_FRAMES,
			wxID_CB_ANIM,
			wxID_CANVAS,
			wxID_SPIN_XOFS,
			wxID_SPIN_YOFS,
			wxID_TXT_GAMMA,
			wxID_SLIDE_GAMMA,
			wxID_CB_ZOOM,
		};
		
		wxMenuBar* mnu;
		wxMenu* mnuFile;
		wxMenu* mmTerrain;
		wxStaticText* m_staticText117;
		wxChoice* chSource;
		wxStaticText* m_staticText118;
		wxTextCtrl* txtFilter;
		wxStaticText* txtResources;
		wxListBox* lbList;
		wxStaticText* txtFrames;
		wxListBox* lbFrames;
		wxCheckBox* cbAnimate;
		wxPanel* canvas;
		wxStaticText* m_staticText88;
		wxSpinCtrl* spinXofs;
		wxStaticText* m_staticText89;
		wxSpinCtrl* spinYofs;
		wxStaticText* txtGamma;
		wxSlider* slideGamma;
		wxCheckBox* cbZoom;

		// </wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormANM' on 2026-06-20 10:21:33

	public:

		FormANM( wxWindow* parent,SpellData* spell_data,bool is_pnm,wxWindowID id = wxID_ANY, const wxString& title = wxT("Terrain layer animations ANM"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 700,500 ), long style = wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER|wxFRAME_FLOAT_ON_PARENT|wxTAB_TRAVERSAL );
		~FormANM();
		void SetANM(Terrain* terr,AnimL1* anm);
		void SetPNM(Terrain* terr,AnimPNM* pnm,int x_ofs=0,int y_ofs=0);
		Terrain* GetSelectedTerrain();
		AnimL1* GetSelectedAnim();
		AnimPNM* GetSelectedPNM();
		std::tuple<int,int> GetPNMoffset();
		bool wasPNM();
		bool WasAnmSet();

};

