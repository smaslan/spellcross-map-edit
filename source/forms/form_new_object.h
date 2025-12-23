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
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/sizer.h>
#include <wx/dialog.h>
#include <wx/choice.h>

#include "spellcross.h"
#include "sprites.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormNewObject
///////////////////////////////////////////////////////////////////////////////
class FormNewObject : public wxDialog
{
	private:

		void OnClose(wxCommandEvent& event);
		void OnCancel(wxCommandEvent& event);

	protected:
		enum
		{
			wxID_TXT_DESC = 5999,
			wxID_CHB_CLASS,
			wxID_BTN_OK,
			wxID_BTN_CANCEL,
		};

		wxStaticText* m_staticText11;
		wxTextCtrl* txtDescription;
		wxStaticText* m_staticText92;
		wxChoice* chbClass;
		wxStaticLine* m_staticline4;
		wxButton* btnOk;
		wxButton* btnCancel;


	public:

		FormNewObject(wxWindow* parent,Terrain* terrain,wxWindowID id = wxID_ANY,const wxString& title = wxT("New object"),const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxSize(400,190),long style = wxDEFAULT_DIALOG_STYLE);
		//FormNewObject( wxWindow* parent, SpellData* spell_data, wxWindowID id = wxID_ANY, const wxString& title = wxT("New object"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,168 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL|wxSTAY_ON_TOP);
		//FormObjects( wxWindow* parent,SpellData* spell_data, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~FormNewObject();
		
		std::string GetDescription();
		int GetClass();
};
