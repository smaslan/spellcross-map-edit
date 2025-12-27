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
#include <wx/choice.h>
#include <wx/spinctrl.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/dialog.h>

#include "spellcross.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormNewObject
///////////////////////////////////////////////////////////////////////////////
class FormNewMap : public wxDialog
{
	private:

		SpellData *m_spell_data;

		void OnClose(wxCommandEvent& event);
		void OnCancel(wxCommandEvent& event);

	protected:
		enum
		{
			wxID_CHB_TERR = 5999,
			wxID_SPIN_X,
			wxID_SPIN_Y,
			wxID_SPIN_ELEV,
			wxID_BTN_OK,
			wxID_BTN_CANCEL,
		};

		wxStaticText* m_staticText92;
		wxChoice* chbTerrain;
		wxStaticText* m_staticText96;
		wxStaticText* m_staticText97;
		wxSpinCtrl* spinXsize;
		wxStaticText* m_staticText98;
		wxSpinCtrl* spinYsize;
		wxStaticText* m_staticText99;
		wxSpinCtrl* spinElev;
		wxStaticLine* m_staticline4;
		wxButton* btnOk;
		wxButton* btnCancel;


	public:

		FormNewMap(wxWindow* parent,SpellData* spell_data,wxWindowID id = wxID_ANY,const wxString& title = wxT("New object"),const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxSize(400,190),long style = wxDEFAULT_DIALOG_STYLE);
		~FormNewMap();
		
		std::string GetTerrain();
		std::tuple<int,int> GetSize();
		int GetElev();
};
