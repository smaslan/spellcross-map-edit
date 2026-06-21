///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

// <wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormTextEdit' on 2026-06-19 16:20:39
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

// </wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormTextEdit' on 2026-06-19 16:20:39

#include <filesystem>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormTextEdit
///////////////////////////////////////////////////////////////////////////////
class FormTextEdit : public wxFrame
{
private:

	void OnClose(wxCloseEvent& ev);
	void OnCloseClick(wxCommandEvent& event);
	void OnMoveText(wxCommandEvent& event);
	void OnOpenClick(wxCommandEvent& event);
	void OnChangeFilter(wxCommandEvent& event);
	void OnChangeSource(wxCommandEvent& event);
	void OnSaveText(wxCommandEvent& event);

	std::filesystem::path m_dir_left;
	std::filesystem::path m_dir_right;
	std::filesystem::path m_dir;

protected:
	
	// <wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormTextEdit' on 2026-06-19 16:20:39
	enum
	{
		wxID_MM_EXIT = 5999,
		wxID_MM_LEFT_OPEN,
		wxID_MM_LEFT_SAVE,
		wxID_MM_LEFT_SAVE_AS,
		wxID_MM_RIGHT_OPEN,
		wxID_MM_RIGHT_SAVE,
		wxID_MM_RIGHT_SAVE_AS,
		wxID_TXT_LEFT_FILTER,
		wxID_LIST_LEFT,
		wxID_CB_LEFT_ENCODING,
		wxID_TXT_LEFT,
		wxID_BTN_TO_LEFT,
		wxID_BTN_TO_RIGHT,
		wxID_TXT_RIGHT_FILTER,
		wxID_LIST_RIGHT,
		wxID_CB_RIGHT_ENCODING,
		wxID_TXT_RIGHT,
		wxID_SBAR,
	};
	
	wxMenuBar* m_menubar14;
	wxMenu* mFile;
	wxMenu* mLeft;
	wxMenu* mRight;
	wxStaticText* m_staticText122;
	wxTextCtrl* txtLeftFilter;
	wxStaticText* m_staticText123;
	wxListBox* listLeft;
	wxStaticText* m_staticText125;
	wxChoice* chLeftEncoding;
	wxStaticText* m_staticText124;
	wxTextCtrl* txtLeft;
	wxStaticLine* m_staticline45;
	wxBitmapButton* btnToLeft;
	wxBitmapButton* btnToRight;
	wxStaticLine* m_staticline451;
	wxStaticText* m_staticText1221;
	wxTextCtrl* txtRightFilter;
	wxStaticText* m_staticText1231;
	wxListBox* listRight;
	wxStaticText* m_staticText1251;
	wxChoice* chRightEncoding;
	wxStaticText* m_staticText1241;
	wxTextCtrl* txtRight;
	wxStatusBar* sbar;

	// </wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormTextEdit' on 2026-06-19 16:20:39

public:

	FormTextEdit(wxWindow* parent,wxWindowID id = wxID_ANY,const wxString& title = wxT("Generic text editor"),const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxSize(1170,600),long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL);

	~FormTextEdit();

};
