///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include "spellcross.h"
#include "map.h"

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/statusbr.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/menu.h>
#include <wx/stattext.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/textctrl.h>
#include <wx/spinctrl.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormUnits
///////////////////////////////////////////////////////////////////////////////
class FormUnits : public wxFrame
{
	private:

		void OnClose(wxCloseEvent& ev);
		void OnCloseClick(wxCommandEvent& event);
		void OnSelectUnit(wxCommandEvent& event);
		void OnSelectArt(wxCommandEvent& event);
		void OnPaintIcon(wxPaintEvent& event);
		void OnPaintArt(wxPaintEvent& event);

		void SelectUnit(MapUnit *unit=NULL);
		void WriteInfo();

		SpellData *m_spell_data;
		MapUnit *m_unit;
		bool m_update;

	protected:
		enum
		{
			wxID_SB = 1000,
			wxID_MM_EXIT,
			wxID_MM_SET,
			wxID_LB_UNITS,
			wxID_CANVAS_ICON,
			wxID_PROPS,
			wxID_NAME,
			wxID_SPIN_HEALTH,
			wxID_CANVAS_ART,
			wxID_LB_ART,
			wxID_TXT_INFO
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
		wxStaticText* m_staticText39;
		wxTextCtrl* txtName;
		wxStaticText* m_staticText40;
		wxSpinCtrl* spinHealth;
		wxStaticLine* m_staticline10;
		wxStaticText* m_staticText36;
		wxPanel* art_canvas;
		wxStaticLine* m_staticline11;
		wxStaticText* m_staticText37;
		wxListBox* lboxArt;
		wxStaticLine* m_staticline12;
		wxStaticText* m_staticText38;
		wxTextCtrl* txtInfo;

	public:

		FormUnits(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Units viewer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1024,720 ),
			long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL|wxSTAY_ON_TOP);
		~FormUnits();

		void SetSpellData(SpellData *spelldata);
		void SetMapUnit(MapUnit* unit=NULL);
		bool DoUpdateUnit();
		

};

