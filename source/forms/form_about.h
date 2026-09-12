///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

// <wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormAbout' on 2026-09-12 10:31:22
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

// </wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormAbout' on 2026-09-12 10:31:22

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormAbout
///////////////////////////////////////////////////////////////////////////////
class FormAbout : public wxDialog
{
	private:

		void OnURL(wxTextUrlEvent& event);

	protected:
		// <wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormAbout' on 2026-09-12 10:31:22
		enum
		{
			wxID_FORM_ABOUT = 5999,
			wxID_TXT_VER,
			wxID_TXT_NAME,
			wxID_TXT_EMAIL,
			wxID_TXT_URL,
			wxID_TXT_LICENSE,
			wxID_TXT_DESC,
			wxID_BTN_OK,
		};
		
		wxStaticText* m_staticText80;
		wxStaticLine* m_staticline29;
		wxTextCtrl* m_textCtrl17;
		wxTextCtrl* m_textCtrl171;
		wxTextCtrl* m_textCtrl172;
		wxTextCtrl* m_textCtrl173;
		wxTextCtrl* m_textCtrl174;
		wxTextCtrl* txtVersion;
		wxTextCtrl* txtName;
		wxTextCtrl* txtEmail;
		wxTextCtrl* txtURL;
		wxTextCtrl* txtLicense;
		wxStaticLine* m_staticline30;
		wxTextCtrl* txtDesc;
		wxStaticLine* m_staticline31;
		wxButton* btnOK;

		// </wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormAbout' on 2026-09-12 10:31:22

	public:

		FormAbout( wxWindow* parent,std::string ver_label, wxWindowID id = wxID_FORM_ABOUT, const wxString& title = wxT("About Spellcross Map Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1000,500 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~FormAbout();
		void OnClose(wxCommandEvent& event);

};

