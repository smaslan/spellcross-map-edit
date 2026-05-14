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
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

#include <thread>

#include "form_config.h"
#include "spellcross.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormLoader
///////////////////////////////////////////////////////////////////////////////
class FormLoader : public wxDialog
{
	private:

		std::thread* loader;
		std::string m_exit_msg;
		int m_failed;

		void Loader(wxWindow* parent,SpellConfig& cfg, SpellData* &spell_data);

		void LoaderExit(bool hold=false,std::string message="");
		void UpdateItem(std::string text);
		void UpdateList(std::string text);

		void OnClose(wxCloseEvent& ev);
		void OnExitClick(wxCommandEvent& event);
		void OnLoaderExit(wxThreadEvent& event);
		void OnRefreshItem(wxThreadEvent& event);
		void OnRefreshList(wxThreadEvent& event);

	protected:
		const int wxID_TH_EXIT = 2000;
		enum
		{
			wxID_TXT_LIST = 1000,
			wxID_TXT_ITEM,
			wxID_BTN_OK
		};

		wxStaticText* m_staticText63;
		wxTextCtrl* txtList;
		wxStaticText* m_staticText62;
		wxTextCtrl* txtItem;
		wxButton* btnOK;

	public:

		FormLoader(wxWindow* parent, SpellData *&spell_data,SpellConfig &cfg, wxWindowID id = wxID_ANY, const wxString& title = wxT("Loading Spellcross Data..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 550,372 ), long style = wxCAPTION|wxSTAY_ON_TOP );
		~FormLoader();
		std::string GetExitMessage();

};


