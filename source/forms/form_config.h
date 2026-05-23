///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

// <wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormConfig' on 2026-05-12 21:36:44
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
#include <wx/sizer.h>
#include <wx/listbox.h>
#include <wx/panel.h>
#include <wx/statbox.h>
#include <wx/slider.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/statline.h>
#include <wx/textctrl.h>
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

// </wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormConfig' on 2026-05-12 21:36:44

#include <filesystem>
#include <map>

class SpellConfig
{
public:
	std::filesystem::path spell_path;
	std::vector<std::filesystem::path> spell_path_hist;
	
	std::filesystem::path spell_cd_path;
	std::vector<std::filesystem::path> spell_cd_path_hist;
	
	std::filesystem::path spell_mod_path;
	std::vector<std::filesystem::path> spell_mod_path_hist;

	std::filesystem::path spec_data_path;
	std::filesystem::path units_aux_data_path;
	
	std::map<std::string, std::filesystem::path> context_path;
	bool hide_map_warnings;
};

///////////////////////////////////////////////////////////////////////////////
/// Class FormSound
///////////////////////////////////////////////////////////////////////////////
class FormConfig : public wxDialog
{
private:
	
	void OnClose(wxCloseEvent& ev);
	void OnCloseClick(wxCommandEvent& event);

	std::filesystem::path GetChoicePaths(wxChoice* choice,std::vector<std::filesystem::path>& list);
	void FillPathHistory(wxChoice* choice,std::vector<std::filesystem::path>& list,std::filesystem::path& path);
	void ChoiceCheckPaths(wxChoice* choice);
	std::wstring GetPathChoiceLastPath(wxChoice* choice,std::wstring default_path=L"");
	void OnSelectPath(wxCommandEvent& event);

	std::wstring cstr_path_browse = L">>> Browse Path <<<";
	std::wstring cstr_path_empty = L">>> No Path <<<";

	SpellConfig *m_config;

protected:
	
	// <wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormConfig' on 2026-05-12 21:36:44
	enum
	{
		wxID_CH_SPELL_PATH = 5999,
		wxID_BTN_SPELL_PATH,
		wxID_CH_SPELLCD_PATH,
		wxID_BTN_SPELLCD_PATH,
		wxID_CH_MOD_PATH,
		wxID_BTN_MOD_PATH,
		wxID_HIDE_MAP_LOAD_WARN,
		wxID_BTN_OK,
	};
	
	wxStaticText* m_staticText119;
	wxChoice* chSpellPath;
	wxBitmapButton* btnSpellPath;
	wxStaticText* m_staticText120;
	wxChoice* chSpellcdPath;
	wxBitmapButton* btnSpellcdPath;
	wxStaticText* m_staticText121;
	wxChoice* chModPath;
	wxBitmapButton* btnModPath;
	wxStaticLine* m_staticline43;
	wxCheckBox* cbHideMapLoadWarnings;
	wxStaticLine* m_staticline44;
	wxButton* btnOK;

	// </wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormConfig' on 2026-05-12 21:36:44

public:

	FormConfig(wxWindow* parent,wxWindowID id = wxID_ANY,SpellConfig* config=NULL,const wxString& title = wxT("Configuration"),const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxSize(830,305),long style = wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP);
	~FormConfig();

};


