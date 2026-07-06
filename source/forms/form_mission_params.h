///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include "../map.h"
#include <map>

// <wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormMissionParams' on 2026-07-06 19:31:50
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

// </wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormMissionParams' on 2026-07-06 19:31:50

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormMissionParams
///////////////////////////////////////////////////////////////////////////////
class FormMissionParams : public wxDialog
{
	private:
		SpellData *m_spell_data;
		SpellMap *m_spell_map;

		std::map<int,std::string> m_units_list;

		void LoadRandRules();
		void OnSelectRandRule(wxCommandEvent& event);
		void OnUnitPropChange(wxPropertyGridEvent& event);
		void OnRulesClick(wxMouseEvent& event);
		void OnRulesPopup(wxCommandEvent& event);
		void OnUnitsRandChange(wxCommandEvent& event);

	protected:
		// <wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormMissionParams' on 2026-07-06 19:31:50
		enum
		{
			wxID_WIN_MISSION_PARAMS = 5999,
			wxID_CH_TEXT,
			wxID_CH_START_TEXT,
			wxID_CH_GOOD_END_TEXT,
			wxID_CH_BAD_END_TEXT,
			wxID_CB_NIGHT,
			wxID_TEXT_PREVIEW,
			wxID_LBOX_RAND_RULES,
			wxID_BTN_RAND_TO_AUTO,
			wxID_BTN_RAND_TO_OFF,
			wxID_BTN_RAND_ENEMY_AUTO,
			wxID_PG_RAND_RULE,
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
		wxStaticLine* m_staticline47;
		wxStaticText* m_staticText134;
		wxListBox* lboxRndRules;
		wxStaticText* m_staticText135;
		wxButton* btnRandToAuto;
		wxButton* btnRandToOff;
		wxButton* btnRandEnemyAuto;
		wxStaticText* m_staticText133;
		wxPropertyGrid* pgRandRule;
		wxStaticLine* m_staticline27;
		wxButton* btnOK;

		// </wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormMissionParams' on 2026-07-06 19:31:50

	public:

		FormMissionParams( wxWindow* parent,SpellData* spell_data,SpellMap *spell_map,wxWindowID id = wxID_WIN_MISSION_PARAMS, const wxString& title = wxT("Mission Parameters"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1000,400 ), long style = wxDEFAULT_DIALOG_STYLE );
		~FormMissionParams();
		void OnClose(wxCommandEvent& event);
		void OnSelectTxt(wxCommandEvent& event);

};

