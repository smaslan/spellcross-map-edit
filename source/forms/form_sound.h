///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

// <wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormSound' on 2026-08-28 14:10:44
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

// </wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormSound' on 2026-08-28 14:10:44

#include "spellcross.h"
#include "map.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormSound
///////////////////////////////////////////////////////////////////////////////
class FormSound : public wxFrame
{
	private:

		SpellData *m_spell_data;
		SpellSound* m_sound;
		SpellSample* m_sample; 
		bool m_was_set;

		void OnClose(wxCloseEvent& ev);
		void OnCloseClick(wxCommandEvent& event);
		void OnSelectClick(wxCommandEvent& event);
		void OnSelectSound(wxCommandEvent& event);
		void OnPlayClick(wxCommandEvent& event);
		void OnStopClick(wxCommandEvent& event);
		void OnSearchUnit(wxCommandEvent& event);

		void FillList();

	protected:
		// <wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormSound' on 2026-08-28 14:10:44
		enum
		{
			wxID_MM_SELECT = 5999,
			wxID_MM_CLOSE,
			wxID_TXT_SEARCH,
			wxID_LB_LIST,
			wxID_GRID_PROPS,
			wxID_CH_MAP_SND_TYPE,
			wxID_BTN_PLAY,
			wxID_BTN_STOP,
		};
		
		wxMenuBar* m_menubar10;
		wxMenu* m_menu19;
		wxStaticText* m_staticText90;
		wxTextCtrl* txtSearch;
		wxStaticText* m_staticText93;
		wxListBox* lbList;
		wxStaticLine* m_staticline32;
		wxStaticText* m_staticText91;
		wxPropertyGrid* gridProps;
		wxStaticText* m_staticText92;
		wxChoice* chMapSndType;
		wxButton* btnPlay;
		wxButton* btnStop;

		// </wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormSound' on 2026-08-28 14:10:44

	public:

		FormSound(wxWindow* parent,SpellData* spell_data,wxWindowID id = wxID_ANY, const wxString& title = wxT("Sound resources preview"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 600,400 ), long style = wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER|wxFRAME_FLOAT_ON_PARENT|wxTAB_TRAVERSAL );
		~FormSound();
		SpellSample* GetSelectedSound();
		MapSound::SoundType GetMapSoundType();
		void SetSound(std::string name,MapSound::SoundType type);
		bool WasSoundSet();

};

