///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/statusbr.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/slider.h>
#include <wx/choice.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/filedlg.h>

#include "spellcross.h"
#include "sprites.h"
#include "map.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormObjects
///////////////////////////////////////////////////////////////////////////////
class FormObjects : public wxFrame
{
	private:

		SpellData* spell_data;
		SpellMap* spell_map;

		static constexpr int TERR_ID0 = 990;

		void OnTerrainChange(wxCommandEvent& event);
		void OnSaveObjects(wxCommandEvent& event);
		void OnSelectObject(wxCommandEvent& event);
		void OnSelectObjectBtn(wxCommandEvent& event);
		void OnChangeGamma(wxCommandEvent& event);
		void OnPaintCanvas(wxPaintEvent& event);

		void FillToolsClasses();
		void UpdateToolClassesView();
		void FillToolItemsList();
		void OnToolClassChange(wxCommandEvent& event);
		void OnToolClassItemChange(wxCommandEvent& event);
		

		Terrain* FindTerrain();
		void SelectTerrain();

	protected:
		enum
		{
			wxID_SB_MAIN = 1000,
			wxID_LB_OBJECTS,
			wxID_CANVAS,
			wxID_TXT_GAMMA,
			wxID_SLIDE_GAMMA,
			wxID_CHB_CLASS,
			wxID_CHB_GROUP,
			wxID_MM_SAVE_OBJECTS,
			wxID_MM_CLOSE,
			wxID_MM_NEXT,
			wxID_MM_PREV
		};

		wxStatusBar* sbar;
		wxStaticText* m_staticText14;
		wxListBox* lbObjects;
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

		//FormObjects( wxWindow* parent,SpellData* spell_data, wxWindowID id = wxID_ANY, const wxString& title = "Objects viewer", const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 400,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL|wxSTAY_ON_TOP);
		FormObjects(wxWindow* parent,SpellData* spell_data,wxWindowID id = wxID_ANY,const wxString& title = wxT("Objects viewer"),const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxSize(1000,600),long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL|wxSTAY_ON_TOP);
		~FormObjects();

		void SetMap(SpellMap *map);

};
