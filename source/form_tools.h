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
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/filedlg.h>

#include <filesystem>

#include "spellcross.h"
#include "map.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormTools
///////////////////////////////////////////////////////////////////////////////
class FormTools : public wxFrame
{
	private:

		SpellData* spell_data;
		SpellMap* spell_map;

		static constexpr int TERR_ID0 = 990;

		Terrain* FindTerrain();
		void OnTerrainChange(wxCommandEvent& event);
		void OnToolsetSelect(wxCommandEvent& event);
		void OnToolsetRename(wxCommandEvent& event);
		void OnToolsetNew(wxCommandEvent& event);
		void OnToolsetRemove(wxCommandEvent& event);
		void OnToolsetMove(wxCommandEvent& event);

		void OnToolSelect(wxCommandEvent& event);
		void OnToolRename(wxCommandEvent& event);
		void OnToolNew(wxCommandEvent& event);
		void OnToolRemove(wxCommandEvent& event);
		void OnToolMove(wxCommandEvent& event);

		void OnClose(wxCloseEvent& ev);
		void OnCloseClick(wxCommandEvent& event);
		void OnSaveContext(wxCommandEvent& event);

		void SelectTerrain();
		void SelectToolset();
		void SelectTool();

	protected:
		enum
		{
			wxID_LB_TOOLSET = 1000,
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
			wxID_CLR_TOOLS
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

		FormTools( wxWindow* parent, SpellData *spelldata, wxWindowID id = wxID_ANY, const wxString& title = wxT("Tools editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 870,450 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL| wxSTAY_ON_TOP);
		~FormTools();

		void SetMap(SpellMap* map);

};

