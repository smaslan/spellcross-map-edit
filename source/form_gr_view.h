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
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/statusbr.h>
#include <wx/stattext.h>
#include <wx/listbox.h>
#include <wx/spinctrl.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/panel.h>
#include <wx/frame.h>

#include "spellcross.h"
#include "fs_archive.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormGResView
///////////////////////////////////////////////////////////////////////////////
class FormGResView : public wxFrame
{
private:
	SpellData *spell_data;
	FSarchive *common;
	
	void OnClose(wxCloseEvent& ev);
	void OnCloseClick(wxCommandEvent& event);
	void OnPaintCanvas(wxPaintEvent& event);
	void OnSelectFile(wxCommandEvent& event);
	void OnWidthChange(wxSpinEvent& event);

	void LoadFileList();
	void LoadGrpList();

protected:
	enum
	{
		wxID_MM_CLOSE = 1000,
		wxID_SB,
		wxID_LB_FILES,
		wxID_SPIN_W,
		wxID_CANVAS
	};

	wxMenuBar* m_menubar5;
	wxMenu* mmFile;
	wxStatusBar* sbar;
	wxStaticText* m_staticText31;
	wxListBox* lboxFiles;
	wxStaticText* m_staticText32;
	wxSpinCtrl* spinWidth;
	wxStaticLine* m_staticline8;
	wxPanel* canvas;

public:

	FormGResView(wxWindow* parent,SpellData* spell_data,wxWindowID id = wxID_ANY,const wxString& title = wxT("Graphics resource viewer"),const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxSize(875,513),long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL|wxSTAY_ON_TOP);

	~FormGResView();

};
