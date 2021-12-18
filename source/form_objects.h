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
#include <wx/sizer.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/htmllbox.h>
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

		Terrain* FindTerrain();
		void SelectTerrain();

	protected:
		enum
		{
			wxID_MM_SAVE_OBJECTS = 1000,
			wxID_MM_CLOSE
		};

		wxStatusBar* m_statusBar2;
		wxSimpleHtmlListBox* htmlList;
		wxMenuBar* m_menubar2;
		wxMenu* m_menu4;
		wxMenu* mnuTerr;

	public:

		FormObjects( wxWindow* parent,SpellData* spell_data, wxWindowID id = wxID_ANY, const wxString& title = "Objects viewer", const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 400,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL|wxSTAY_ON_TOP);
		~FormObjects();

		void SetMap(SpellMap *map);

};
