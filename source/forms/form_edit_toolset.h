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
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormEditToolset
///////////////////////////////////////////////////////////////////////////////
class Terrain;

class FormEditToolset : public wxDialog
{
private:
	Terrain *m_terr = NULL;
	int m_toolset_id = -1;

	void OnCloseClick(wxCommandEvent& event);
	void OnClose(wxCloseEvent& ev);

protected:
	enum
	{
		wxID_TXT_NAME = 5999,
		wxID_TXT_DESC,
		wxID_CHB_SCALE,
		wxID_SC_WIDTH,
		wxID_SC_HEIGHT,
		wxID_BTN_OK,
	};

	wxStaticText* m_staticText11;
	wxTextCtrl* txtName;
	wxStaticText* m_staticText111;
	wxTextCtrl* txtDescription;
	wxStaticText* m_staticText26;
	wxChoice* chbScaling;
	wxStaticText* m_staticText27;
	wxSpinCtrl* scWidth;
	wxStaticText* m_staticText28;
	wxSpinCtrl* scHeight;
	wxStaticLine* m_staticline4;
	wxButton* btnOk;

public:

	FormEditToolset(wxWindow* parent,Terrain *terr,int toolset_id, wxWindowID id = wxID_ANY,const wxString& title = wxT("Edit Toolset Parameters"),const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxSize(400,100),long style = wxDEFAULT_DIALOG_STYLE);

	~FormEditToolset();

};

