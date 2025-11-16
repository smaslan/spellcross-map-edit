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
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/dialog.h>
#include "spellcross.h"
#include "map.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormMissionParams
///////////////////////////////////////////////////////////////////////////////
class FormMissionParams : public wxDialog
{
	private:
		SpellData *m_spell_data;
		SpellMap *m_spell_map;

	protected:
		enum
		{
			wxID_WIN_MISSION_PARAMS = 5999,
			wxID_CH_TEXT,
			wxID_CH_START_TEXT,
			wxID_CH_GOOD_END_TEXT,
			wxID_CH_BAD_END_TEXT,
			wxID_CB_NIGHT,
			wxID_TEXT_PREVIEW,
			wxID_BTN_OK,
		};

		wxStaticText* m_staticText75;
		wxChoice* chText;
		wxStaticText* m_staticText76;
		wxChoice* chStartText;
		wxStaticText* m_staticText77;
		wxChoice* chGoodEndText;
		wxStaticText* m_staticText78;
		wxChoice* chBadEndText;
		wxCheckBox* cbNight;
		wxStaticLine* m_staticline28;
		wxStaticText* txtPrevLbl;
		wxTextCtrl* txtPreview;
		wxStaticLine* m_staticline27;
		wxButton* btnOK;

	public:

		FormMissionParams( wxWindow* parent,SpellData* spell_data,SpellMap *spell_map,wxWindowID id = wxID_WIN_MISSION_PARAMS, const wxString& title = wxT("Mission Parameters"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 593,388 ), long style = wxDEFAULT_DIALOG_STYLE );
		~FormMissionParams();
		void OnClose(wxCommandEvent& event);
		void OnSelectTxt(wxCommandEvent& event);

};

