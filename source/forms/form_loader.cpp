///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_loader.h"
#include <wx/stdpaths.h>

#include <filesystem>

#include "other.h"
#include "wx_other.h"
#include "simpleini.h"


///////////////////////////////////////////////////////////////////////////

FormLoader::FormLoader(wxWindow* parent,SpellData *&spell_data, SpellConfig &cfg, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	// === AUTO GENERATER START ===
	
	this->SetSizeHints(wxDefaultSize,wxDefaultSize);

	wxBoxSizer* szrLoader;
	szrLoader = new wxBoxSizer(wxVERTICAL);

	m_staticText63 = new wxStaticText(this,wxID_ANY,wxT("Progress:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText63->Wrap(-1);
	szrLoader->Add(m_staticText63,0,wxTOP|wxRIGHT|wxLEFT,5);

	txtList = new wxTextCtrl(this,wxID_TXT_LIST,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP);
	szrLoader->Add(txtList,1,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);

	m_staticText62 = new wxStaticText(this,wxID_ANY,wxT("Loading item:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText62->Wrap(-1);
	szrLoader->Add(m_staticText62,0,wxTOP|wxRIGHT|wxLEFT,5);

	txtItem = new wxTextCtrl(this,wxID_TXT_ITEM,wxEmptyString,wxDefaultPosition,wxDefaultSize,0);
	szrLoader->Add(txtItem,0,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);

	btnOK = new wxButton(this,wxID_BTN_OK,wxT("EXIT"),wxDefaultPosition,wxDefaultSize,0);
	szrLoader->Add(btnOK,0,wxALL|wxEXPAND,5);

	this->SetSizer(szrLoader);
	this->Layout();

	this->Centre(wxBOTH);
	
	// === AUTO GENERATER END ===

	Bind(wxEVT_THREAD,&FormLoader::OnRefreshItem,this,wxID_TXT_ITEM);
	Bind(wxEVT_THREAD,&FormLoader::OnRefreshList,this,wxID_TXT_LIST);
	Bind(wxEVT_THREAD,&FormLoader::OnLoaderExit,this,wxID_TH_EXIT);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormLoader::OnExitClick,this,wxID_BTN_OK);
	Bind(wxEVT_CLOSE_WINDOW,&FormLoader::OnClose,this);

	// by default hide exist button
	btnOK->Enable(false);

	// initiate loader in its own thread
	m_failed = false;
	m_exit_msg = "";
	loader = new std::thread(&FormLoader::Loader,this,this,std::ref(cfg),std::ref(spell_data));	
}

FormLoader::~FormLoader()
{
	
}

void FormLoader::OnClose(wxCloseEvent& ev)
{	
	// cleanup worker thread
	loader->join();
	delete loader;

	// return state
	EndModal(m_failed);
}

// manual panel close click
void FormLoader::OnExitClick(wxCommandEvent &event)
{
	Close();
}

// fetch exit error message
std::string FormLoader::GetExitMessage()
{
	return(m_exit_msg);
}


// -------------------------------------------------------------------------------------------------
// Data loader thread
// -------------------------------------------------------------------------------------------------
void FormLoader::Loader(wxWindow *parent,SpellConfig &cfg,SpellData* &spell_data)
{
	// get exec path
	auto exe_path = GetExecutableDir();

	// special data folder
	auto spec_folder = exe_path / cfg.spec_data_path;
	// units aux data path
	auto units_aux_data_path = exe_path / cfg.units_aux_data_path;

	// try load spellcross data
	if(spell_data->Reload(cfg.spell_path,cfg.spell_cd_path,cfg.spell_mod_path,spec_folder,bind(&FormLoader::UpdateList,this,placeholders::_1),bind(&FormLoader::UpdateItem,this,placeholders::_1)))
	{
		UpdateList(spell_data->GetLastError());
		LoaderExit(true,spell_data->GetLastError());
		return;
	}
		
	// try load units.fsu aux metadata
	UpdateList("Loading units aux data...");
	if(spell_data->units_fsu->LoadAuxData(units_aux_data_path))
	{
		spell_data->Cleanup();		
		UpdateList(string_format(" - failed loading units aux data from ''%ls''!",units_aux_data_path.c_str()));
		LoaderExit(true,string_format("Failed loading units aux data from ''%ls''!",units_aux_data_path.c_str()));
		return;
	}
	
	// for each terrain load tile context
	UpdateList("Loading terrain context data...");
	for(auto & terr : spell_data->terrain)
	{
		UpdateList(string_format(" - loading ''%s''...",terr->name.c_str()));
		auto terr_id = cfg.context_path.find(terr->name);		
		if(terr_id == cfg.context_path.end())
			continue;		
		std::filesystem::path cont_path = exe_path / terr_id->second;
		if(terr->InitSpriteContext(cont_path))
		{
			UpdateList(string_format("   - context ''%ls'' not found...",cont_path.c_str()));
		}
		// try add special tools
		terr->AddSpecialTools();
	}			

	// exit
	LoaderExit();
}

// end loader
void FormLoader::LoaderExit(bool hold,std::string message)
{	
	auto evt = new wxThreadEvent(wxEVT_THREAD,wxID_TH_EXIT);
	evt->SetInt(hold);
	evt->SetString(message);
	this->QueueEvent(evt);		
}
// end loader event
void FormLoader::OnLoaderExit(wxThreadEvent& event)
{	
	m_exit_msg = event.GetString();
	m_failed = event.GetInt();

	// show manual exit button
	btnOK->Enable(true);
	SetWindowStyle(GetWindowStyle() | wxCLOSE_BOX);

	// optional auto-exit command?
	if(!m_failed)
		Close();
}


// update actual item info
void FormLoader::UpdateItem(std::string text)
{
	auto evt = new wxThreadEvent(wxEVT_THREAD,wxID_TXT_ITEM);
	evt->SetString(text);
	this->QueueEvent(evt);
}
// update progress list
void FormLoader::UpdateList(std::string text)
{
	auto evt = new wxThreadEvent(wxEVT_THREAD,wxID_TXT_LIST);
	evt->SetString(text);
	this->QueueEvent(evt);
}

// on progress/item update
void FormLoader::OnRefreshItem(wxThreadEvent& event)
{
	auto str = event.GetString();
	txtItem->ChangeValue(str);
}
void FormLoader::OnRefreshList(wxThreadEvent& event)
{
	auto str = event.GetString() + "\n";	
	txtList->AppendText(str);
}




