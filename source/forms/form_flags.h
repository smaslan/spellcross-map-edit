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
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/dialog.h>

#include "spellcross.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormFlags
///////////////////////////////////////////////////////////////////////////////
class FormFlags : public wxDialog
{
	private:

		void OnClose(wxCommandEvent& event);
		void OnCanvasRepaint(wxPaintEvent& event);
		void OnSelect(wxCommandEvent& event);
		void SelectFlag(int flag);		

		std::vector<Sprite*> m_sprites;
		std::vector<int> m_list;
		Terrain *m_terrain;

	protected:
		enum
		{
			wxID_CHB_FLAG = 5999,
			wxID_CANVAS,
			wxID_BTN_OK,
			wxID_BTN_CANCEL,
		};

		wxStaticText* m_staticText90;
		wxChoice* chbFlag;
		wxPanel* canvas;
		wxButton* btnOk;
		wxButton* btnCancel;

	public:

		FormFlags( wxWindow* parent,Terrain* terr,int flags,wxWindowID id = wxID_ANY, const wxString& title = wxT("Setup tile parameters"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 368,116 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~FormFlags();
		int GetSelectedFlags();
		

};

