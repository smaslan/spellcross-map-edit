///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include <wx/dirdlg.h>
#include "form_config.h"
#include "other.h"
#include "wx_other.h"

///////////////////////////////////////////////////////////////////////////

FormConfig::FormConfig(wxWindow* parent,wxWindowID id,SpellConfig *config,const wxString& title,const wxPoint& pos,const wxSize& size,long style) : wxDialog(parent,id,title,pos,size,style)
{
	m_config = config;

	// === AUTO GENERATED STUFF STARTS HERE ===
	// <wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormConfig' on 2026-05-12 21:36:43
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer115;
	bSizer115 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText119 = new wxStaticText( this, wxID_ANY, wxT("Spellcross installation DATA folder:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText119->Wrap( -1 );
	bSizer115->Add( m_staticText119, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	wxBoxSizer* bSizer116;
	bSizer116 = new wxBoxSizer( wxHORIZONTAL );
	
	wxArrayString chSpellPathChoices;
	chSpellPath = new wxChoice( this, wxID_CH_SPELL_PATH, wxDefaultPosition, wxDefaultSize, chSpellPathChoices, 0 );
	chSpellPath->SetSelection( 0 );
	bSizer116->Add( chSpellPath, 1, wxRIGHT|wxLEFT, 5 );
	
	btnSpellPath = new wxBitmapButton( this, wxID_BTN_SPELL_PATH, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxBORDER_NONE );
	bSizer116->Add( btnSpellPath, 0, wxRIGHT|wxLEFT, 5 );
	
	
	bSizer115->Add( bSizer116, 0, wxEXPAND, 5 );
	
	m_staticText120 = new wxStaticText( this, wxID_ANY, wxT("Spellcross CD DATA path (leave empty if no CD data available):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText120->Wrap( -1 );
	bSizer115->Add( m_staticText120, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	wxBoxSizer* bSizer1161;
	bSizer1161 = new wxBoxSizer( wxHORIZONTAL );
	
	wxArrayString chSpellcdPathChoices;
	chSpellcdPath = new wxChoice( this, wxID_CH_SPELLCD_PATH, wxDefaultPosition, wxDefaultSize, chSpellcdPathChoices, 0 );
	chSpellcdPath->SetSelection( 0 );
	bSizer1161->Add( chSpellcdPath, 1, wxRIGHT|wxLEFT, 5 );
	
	btnSpellcdPath = new wxBitmapButton( this, wxID_BTN_SPELLCD_PATH, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxBORDER_NONE );
	bSizer1161->Add( btnSpellcdPath, 0, wxRIGHT|wxLEFT, 5 );
	
	
	bSizer115->Add( bSizer1161, 0, wxEXPAND, 5 );
	
	m_staticText121 = new wxStaticText( this, wxID_ANY, wxT("Mod make folder path (optional, priority over above load paths):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText121->Wrap( -1 );
	bSizer115->Add( m_staticText121, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	wxBoxSizer* bSizer11611;
	bSizer11611 = new wxBoxSizer( wxHORIZONTAL );
	
	wxArrayString chModPathChoices;
	chModPath = new wxChoice( this, wxID_CH_MOD_PATH, wxDefaultPosition, wxDefaultSize, chModPathChoices, 0 );
	chModPath->SetSelection( 0 );
	bSizer11611->Add( chModPath, 1, wxRIGHT|wxLEFT, 5 );
	
	btnModPath = new wxBitmapButton( this, wxID_BTN_MOD_PATH, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxBORDER_NONE );
	bSizer11611->Add( btnModPath, 0, wxRIGHT|wxLEFT, 5 );
	
	
	bSizer115->Add( bSizer11611, 0, wxEXPAND|wxBOTTOM, 5 );
	
	m_staticline43 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer115->Add( m_staticline43, 0, wxEXPAND | wxALL, 5 );
	
	cbHideMapLoadWarnings = new wxCheckBox( this, wxID_HIDE_MAP_LOAD_WARN, wxT("Hide map loading warning (missing resources)"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer115->Add( cbHideMapLoadWarnings, 0, wxALL, 5 );
	
	
	bSizer115->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticline44 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer115->Add( m_staticline44, 0, wxEXPAND | wxALL, 5 );
	
	btnOK = new wxButton( this, wxID_BTN_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	btnOK->SetMinSize( wxSize( -1,30 ) );
	
	bSizer115->Add( btnOK, 0, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer115 );
	this->Layout();
	
	this->Centre( wxBOTH );
	

	// </wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormConfig' on 2026-05-12 21:36:43
	// === AUTO GENERATED STUFF STARTS HERE ===

	// set icon
	/*wxIcon appIcon;
	appIcon.LoadFile("IDI_ICON2",wxBITMAP_TYPE_ICO_RESOURCE);
	if(appIcon.IsOk())
		SetIcon(appIcon);*/

	auto op_sz = FromDIP(wxSize(16,16));
	btnSpellPath->SetBitmap(LoadSVGiconsBundle("IDR_OPEN3").GetBitmap(op_sz));
	btnSpellPath->SetBitmapHover(LoadSVGiconsBundle("IDR_OPEN2").GetBitmap(op_sz));
	btnSpellcdPath->SetBitmap(LoadSVGiconsBundle("IDR_OPEN3").GetBitmap(op_sz));
	btnSpellcdPath->SetBitmapHover(LoadSVGiconsBundle("IDR_OPEN2").GetBitmap(op_sz));
	btnModPath->SetBitmap(LoadSVGiconsBundle("IDR_OPEN3").GetBitmap(op_sz));
	btnModPath->SetBitmapHover(LoadSVGiconsBundle("IDR_OPEN2").GetBitmap(op_sz));


	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormConfig::OnCloseClick,this,wxID_BTN_OK);
	Bind(wxEVT_CLOSE_WINDOW,&FormConfig::OnClose,this);

	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormConfig::OnSelectPath,this,wxID_BTN_SPELL_PATH);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormConfig::OnSelectPath,this,wxID_BTN_SPELLCD_PATH);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormConfig::OnSelectPath,this,wxID_BTN_MOD_PATH);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormConfig::OnSelectPath,this,wxID_CH_SPELL_PATH);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormConfig::OnSelectPath,this,wxID_CH_SPELLCD_PATH);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormConfig::OnSelectPath,this,wxID_CH_MOD_PATH);

	FillPathHistory(chSpellPath, config->spell_path_hist, config->spell_path);
	FillPathHistory(chSpellcdPath,config->spell_cd_path_hist, config->spell_cd_path);
	FillPathHistory(chModPath,config->spell_mod_path_hist, config->spell_mod_path);
	cbHideMapLoadWarnings->SetValue(config->hide_map_warnings);

	// assign button shortcuts
	std::vector<wxAcceleratorEntry> entries;
	entries.emplace_back(wxACCEL_NORMAL,WXK_RETURN,wxID_BTN_OK);
	entries.emplace_back(wxACCEL_NORMAL,WXK_ESCAPE,wxID_BTN_OK);
	wxAcceleratorTable accel(entries.size(),entries.data());
	this->SetAcceleratorTable(accel);

}

// destructor
FormConfig::~FormConfig()
{
}

// on form close
void FormConfig::OnClose(wxCloseEvent& ev)
{
	m_config->spell_path = GetChoicePaths(chSpellPath,m_config->spell_path_hist);
	m_config->spell_cd_path = GetChoicePaths(chSpellcdPath,m_config->spell_cd_path_hist);
	m_config->spell_mod_path = GetChoicePaths(chModPath,m_config->spell_mod_path_hist);
	m_config->hide_map_warnings = cbHideMapLoadWarnings->GetValue();

	EndModal(true);
}

// close button
void FormConfig::OnCloseClick(wxCommandEvent& event)
{	
	Close();
}



// fill path choice history
void FormConfig::FillPathHistory(wxChoice *choice, std::vector<std::filesystem::path>& list, std::filesystem::path &path)
{
	choice->Freeze();
	choice->Clear();
	for(auto &item: list)
		choice->Append(item.wstring());		
	choice->Append(cstr_path_browse);
	choice->Append(cstr_path_empty);
	choice->Thaw();

	ChoiceCheckPaths(choice);

	if(list.empty() || path.empty())
		choice->SetStringSelection(cstr_path_empty);
	else
		choice->Select(0);
}

// store list of choice paths
std::filesystem::path FormConfig::GetChoicePaths(wxChoice* choice,std::vector<std::filesystem::path> &list)
{
	// remove rubbish
	ChoiceCheckPaths(choice);

	// make list of history paths
	list.clear();	
	if(choice->GetStringSelection() == cstr_path_empty)
		list.push_back("");
	for(auto& item: choice->GetStrings())
	{
		if(item == cstr_path_browse || item == cstr_path_empty)
			continue;
		list.push_back(item.ToStdWstring());
	}
	if(choice->GetStringSelection() == cstr_path_empty)
		return("");
	if(list.empty())
		return("");
	return(list[0]);
}

// check listed paths (remove duplicates, non-existent, move selected to top)
void FormConfig::ChoiceCheckPaths(wxChoice* choice)
{
	auto path = choice->GetStringSelection().ToStdWstring();
	if(path == cstr_path_browse || path == cstr_path_empty)
		choice->Select(-1);

	// make list of unique paths
	std::vector<std::wstring> list;
	for(auto& item: choice->GetStrings())
	{
		if(item == cstr_path_browse || item == cstr_path_empty)
			continue;
		if(!std::filesystem::exists(std::filesystem::path(item.ToStdWstring())))
			continue;
		bool found = false;
		for(auto &path: list)
			if(iequals(item.ToStdWstring(), path))
			{
				found = true;
				break;
			}
		if(found)
			continue;
		list.push_back(item.ToStdWstring());
	}
	

	// refill list
	choice->Freeze();
	choice->Clear();
	for(auto& item: list)
		choice->Append(item);
	choice->Append(cstr_path_empty);
	choice->Append(cstr_path_browse);
	choice->Thaw();
	if(path == cstr_path_empty)
		choice->SetStringSelection(cstr_path_empty);
	else if(choice->SetStringSelection(path))
	{
		// make sure selected path is first
		choice->Delete(choice->GetSelection());
		choice->Insert(path,0);
		choice->Select(0);
	}
	else
		choice->SetStringSelection(cstr_path_empty);
}

// get first valid path of choice path selector or deftaul if none found
std::wstring FormConfig::GetPathChoiceLastPath(wxChoice* choice,std::wstring default_path)
{
	if(!choice || choice->GetCount() < 3 || choice->GetStringSelection() == cstr_path_empty)
		return(default_path);
	return(choice->GetString(0).ToStdWstring());
}

// on change spellcross path
void FormConfig::OnSelectPath(wxCommandEvent& event)
{
	auto id = event.GetId();
	auto choice = (wxChoice*)FindItem(id);
	std::string title = "";
	if(id == wxID_BTN_SPELL_PATH || id == wxID_CH_SPELL_PATH)
	{
		choice = chSpellPath;
		title = "Select Spellcross installation folder";
	}
	if(id == wxID_BTN_SPELLCD_PATH || id == wxID_CH_SPELLCD_PATH)
	{
		choice = chSpellcdPath;
		title = "Select Spellcross CD folder";
	}
	if(id == wxID_BTN_MOD_PATH || id == wxID_CH_MOD_PATH)
	{
		choice = chModPath;
		title = "Select Spellcross mod folder";
	}
	if(choice->GetStringSelection() == cstr_path_browse || id == wxID_BTN_SPELLCD_PATH || id == wxID_BTN_SPELL_PATH || id == wxID_BTN_MOD_PATH)
	{
		// browse new path
		auto def_path = GetPathChoiceLastPath(choice);
		wxDirDialog saveDirDialog(this,title,def_path,wxDD_DIR_MUST_EXIST);
		if(saveDirDialog.ShowModal() == wxID_CANCEL)
			return;
		choice->Insert(saveDirDialog.GetPath(),0);
		choice->Select(0);
	}
	// check validity
	ChoiceCheckPaths(choice);
}
