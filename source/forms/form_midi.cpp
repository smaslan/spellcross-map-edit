///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_midi.h"
#include <filesystem>
#include <wx/dirdlg.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>

///////////////////////////////////////////////////////////////////////////

FormMIDI::FormMIDI( wxWindow* parent, SpellData *spell_data, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	m_spell_data = spell_data;
	
	// === AUTO GENERATED START ===
	this->SetSizeHints(wxDefaultSize,wxDefaultSize);
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENU));

	mmenu = new wxMenuBar(0);
	mmFile = new wxMenu();
	wxMenuItem* mmCustomFolder;
	mmCustomFolder = new wxMenuItem(mmFile,wxID_MM_CUST_FOLDER,wxString(wxT("Select MIDI folder")) + wxT('\t') + wxT("Ctrl+O"),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmCustomFolder);

	wxMenuItem* mmSaveFile;
	mmSaveFile = new wxMenuItem(mmFile,wxID_MM_SAVE_MIDI,wxString(wxT("Export MIDI file")) + wxT('\t') + wxT("Ctrl+S"),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmSaveFile);

	wxMenuItem* mmSaveAll;
	mmSaveAll = new wxMenuItem(mmFile,wxID_MM_SAVE_ALL,wxString(wxT("Export all MIDI")),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmSaveAll);

	mmFile->AppendSeparator();

	wxMenuItem* mmExit;
	mmExit = new wxMenuItem(mmFile,wxID_MM_EXIT,wxString(wxT("Exit")),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmExit);

	mmenu->Append(mmFile,wxT("File"));

	this->SetMenuBar(mmenu);

	wxBoxSizer* bSizer60;
	bSizer60 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer63;
	bSizer63 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer61;
	bSizer61 = new wxBoxSizer(wxVERTICAL);

	labelList = new wxStaticText(this,wxID_ANY,wxT("MIDI list:"),wxDefaultPosition,wxDefaultSize,0);
	labelList->Wrap(-1);
	bSizer61->Add(labelList,0,wxTOP|wxRIGHT|wxLEFT,5);

	lbList = new wxListBox(this,wxID_LB_LIST,wxDefaultPosition,wxDefaultSize,0,NULL,wxLB_ALWAYS_SB);
	lbList->SetMinSize(wxSize(250,-1));

	bSizer61->Add(lbList,1,wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND,5);

	m_staticText69 = new wxStaticText(this,wxID_ANY,wxT("Source:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText69->Wrap(-1);
	bSizer61->Add(m_staticText69,0,wxALL,5);

	rbSrcMusicFS = new wxRadioButton(this,wxID_RB_SRC_MUSIC_FS,wxT("MUSIC.FS"),wxDefaultPosition,wxDefaultSize,0);
	bSizer61->Add(rbSrcMusicFS,0,wxRIGHT|wxLEFT,5);

	wxBoxSizer* bSizer67;
	bSizer67 = new wxBoxSizer(wxHORIZONTAL);

	rbSrcFolder = new wxRadioButton(this,wxID_RB_SRC_FOLDER,wxT("Custom folder"),wxDefaultPosition,wxDefaultSize,0);
	bSizer67->Add(rbSrcFolder,0,wxALL,5);

	btnSrcFolder = new wxButton(this,wxID_BTN_SRC_FOLDER,wxT("Open"),wxDefaultPosition,wxDefaultSize,0);
	bSizer67->Add(btnSrcFolder,0,wxRIGHT|wxLEFT,5);


	bSizer61->Add(bSizer67,0,wxEXPAND,5);


	bSizer63->Add(bSizer61,0,wxEXPAND,5);

	m_staticline24 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_VERTICAL);
	bSizer63->Add(m_staticline24,0,wxEXPAND|wxTOP|wxBOTTOM,5);

	wxBoxSizer* bSizer62;
	bSizer62 = new wxBoxSizer(wxVERTICAL);

	labelInfo = new wxStaticText(this,wxID_ANY,wxT("MIDI description:"),wxDefaultPosition,wxDefaultSize,0);
	labelInfo->Wrap(-1);
	bSizer62->Add(labelInfo,0,wxTOP|wxRIGHT|wxLEFT,5);

	txtInfo = new wxTextCtrl(this,wxID_TXT_INFO,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE|wxTE_WORDWRAP);
	bSizer62->Add(txtInfo,1,wxEXPAND|wxRIGHT|wxLEFT,5);

	m_staticText68 = new wxStaticText(this,wxID_ANY,wxT("MIDI volume:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText68->Wrap(-1);
	bSizer62->Add(m_staticText68,0,wxTOP|wxRIGHT|wxLEFT,5);

	wxBoxSizer* bSizer66;
	bSizer66 = new wxBoxSizer(wxHORIZONTAL);


	bSizer62->Add(bSizer66,1,wxEXPAND,5);

	slideVolume = new wxSlider(this,wxID_SLIDE_VOLUME,50,0,100,wxDefaultPosition,wxDefaultSize,wxSL_AUTOTICKS|wxSL_BOTH|wxSL_HORIZONTAL|wxSL_MIN_MAX_LABELS|wxSL_SELRANGE);
	bSizer62->Add(slideVolume,0,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);

	m_staticline26 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_HORIZONTAL);
	bSizer62->Add(m_staticline26,0,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);

	wxBoxSizer* bSizer65;
	bSizer65 = new wxBoxSizer(wxHORIZONTAL);

	btnStop = new wxButton(this,wxID_BTN_STOP,wxT("STOP"),wxDefaultPosition,wxDefaultSize,0);
	bSizer65->Add(btnStop,1,wxRIGHT|wxLEFT,5);

	btnPlay = new wxButton(this,wxID_BTN_PLAY,wxT("PLAY"),wxDefaultPosition,wxDefaultSize,0);
	bSizer65->Add(btnPlay,1,wxRIGHT|wxLEFT,5);


	bSizer62->Add(bSizer65,0,wxEXPAND,5);


	bSizer63->Add(bSizer62,1,wxEXPAND,5);


	bSizer60->Add(bSizer63,1,wxEXPAND,5);

	m_staticline23 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_HORIZONTAL);
	bSizer60->Add(m_staticline23,0,wxEXPAND|wxTOP|wxRIGHT|wxLEFT,5);

	wxBoxSizer* bSizer64;
	bSizer64 = new wxBoxSizer(wxHORIZONTAL);


	bSizer64->Add(0,0,1,0,5);

	btnOK = new wxButton(this,wxID_BTN_OK,wxT("EXIT"),wxDefaultPosition,wxDefaultSize,0);
	bSizer64->Add(btnOK,0,wxALL,5);


	bSizer60->Add(bSizer64,0,wxEXPAND,5);


	this->SetSizer(bSizer60);
	this->Layout();

	this->Centre(wxBOTH);

	// === AUTO GENERATED END ===

	Bind(wxEVT_CLOSE_WINDOW,&FormMIDI::OnClose,this);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormMIDI::OnExitClick,this,wxID_BTN_OK);
	Bind(wxEVT_MENU,&FormMIDI::OnExitClick,this,wxID_MM_EXIT);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormMIDI::OnSelectFolderClick,this,wxID_BTN_SRC_FOLDER);
	Bind(wxEVT_MENU,&FormMIDI::OnSelectFolderClick,this,wxID_MM_CUST_FOLDER);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormMIDI::OnPlayClick,this,wxID_BTN_PLAY);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormMIDI::OnStopClick,this,wxID_BTN_STOP);
	Bind(wxEVT_COMMAND_SLIDER_UPDATED,&FormMIDI::OnSetVolume,this,wxID_SLIDE_VOLUME);
	Bind(wxEVT_COMMAND_RADIOBUTTON_SELECTED,&FormMIDI::OnChangeSource,this,wxID_RB_SRC_MUSIC_FS);
	Bind(wxEVT_COMMAND_RADIOBUTTON_SELECTED,&FormMIDI::OnChangeSource,this,wxID_RB_SRC_FOLDER);
	Bind(wxEVT_MENU,&FormMIDI::OnSaveFileClick,this,wxID_MM_SAVE_MIDI);
	Bind(wxEVT_MENU,&FormMIDI::OnSaveAllClick,this,wxID_MM_SAVE_ALL);
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormMIDI::OnSelectMidClick,this,wxID_LB_LIST);


	// default source is MUSIC.FS
	rbSrcMusicFS->SetValue(true);

	// load list of music
	ChangeSource();

	// set initial volume
	auto midi = m_spell_data->midi;
	slideVolume->SetValue(midi->GetVolume()*100);

}

FormMIDI::~FormMIDI()
{
}

// on close window
void FormMIDI::OnClose(wxCloseEvent& ev)
{
	DeletePendingEvents();
	wxQueueEvent(GetParent(),new wxCloseEvent(ev));
}

// EXIT button pressed
void FormMIDI::OnExitClick(wxCommandEvent &ev)
{
	Close();
}

// change volume
void FormMIDI::OnSetVolume(wxCommandEvent &ev)
{
	double volume = 0.01*slideVolume->GetValue();
	m_spell_data->midi->SetVolume(volume);
}

// PLAY button pressed
void FormMIDI::OnPlayClick(wxCommandEvent& ev)
{
	if(!lbList->GetCount())
		return;
	auto midi = m_spell_data->midi;

	std::string name = lbList->GetString(lbList->GetSelection()).ToStdString();
	if(rbSrcMusicFS->GetValue())
		midi->Play(name);
	else
	{
		// from custom file
		auto path = std::filesystem::path(m_src_folder) / std::filesystem::path(name);

		// try play the custom MIDI
		midi->Play(path);
	}
	
}

// STOP button pressed
void FormMIDI::OnStopClick(wxCommandEvent& ev)
{
	m_spell_data->midi->Stop();
}

// select source
void FormMIDI::OnChangeSource(wxCommandEvent &ev)
{
	ChangeSource();
}

// on select custom MIDI folder
void FormMIDI::OnSelectFolderClick(wxCommandEvent &ev)
{
	wxDirDialog dlg(this, "Select User MIDI folder","");
	if(dlg.ShowModal() == wxID_CANCEL)
		return;

	m_src_folder = dlg.GetPath();
	
	// update files list
	rbSrcFolder->SetValue(true);
	ChangeSource();
}

// change music source
void FormMIDI::ChangeSource()
{
	auto midi = m_spell_data->midi;

	if(rbSrcMusicFS->GetValue())
	{
		// --- from MUSIC.FS:

		// fill list
		auto files = midi->GetMIDI();
		lbList->Freeze();
		lbList->Clear();
		for(auto & mid : files)
			lbList->Append(mid->filename);
		lbList->Thaw();

	}
	else
	{
		// --- from custom folder:

		auto folder = std::filesystem::path(m_src_folder);
		lbList->Freeze();
		lbList->Clear();
		if(!folder.empty())
		{
			for(const auto& entry : std::filesystem::directory_iterator(folder))
				if(entry.path().extension() == ".mid")
					lbList->Append(entry.path().filename().string());
		}
		lbList->Thaw();

	}
	
	// select playing mid
	if(!lbList->GetCount())
		return;
	auto file = midi->GetCurrentName();
	for(int k = 0; k < lbList->GetCount(); k++)
		if(lbList->GetString(k).compare(file) == 0)
		{
			lbList->Select(k);
			GetFileInfo();
		}
}

// generate current file info
void FormMIDI::GetFileInfo()
{
	auto midi = m_spell_data->midi;
	
	std::string info;
	std::string name = lbList->GetString(lbList->GetSelection()).ToStdString();
	if(rbSrcMusicFS->GetValue())
		info = midi->GetInfo(name);
	else
	{
		// from custom file
		auto path = std::filesystem::path(m_src_folder) / std::filesystem::path(name);
		info = midi->GetInfo(path);
	}

	txtInfo->SetValue(info);
}

// save currently loaded MIDI
void FormMIDI::OnSaveFileClick(wxCommandEvent& ev)
{
	auto midi = m_spell_data->midi;

	if(!midi->GetCurrent())
		return;

	auto name = midi->GetCurrentName();
	wxFileDialog dlg(this, "Select File Name to Save MIDI", "", name, "MIDI file (*.mid)|*.mid");
	if(dlg.ShowModal() == wxID_CANCEL)
		return;
	auto path = dlg.GetPath().ToStdWstring();

	if(midi->SaveCurrent(path))
		wxMessageBox("Exporting MIDI file failed!","Error",wxICON_ERROR);
}

// save all MIDI loaded from MUSIC.FS
void FormMIDI::OnSaveAllClick(wxCommandEvent& ev)
{
	wxDirDialog dlg(this,"Select folder to export MUSIC.FS MIDI","");
	if(dlg.ShowModal() == wxID_CANCEL)
		return;

	auto old_selection = lbList->GetSelection();

	auto midi = m_spell_data->midi;
	for(auto & xmi : midi->GetMIDI())
	{		
		for(int k = 0; k < lbList->GetCount(); k++)
			if(lbList->GetString(k).compare(xmi->filename) == 0)
			{
				lbList->Select(k);
				lbList->EnsureVisible(k);
				break;
			}
		// convert to XMI->MID and save
		cxxmidi::File file(xmi->GetData());
		auto path = std::filesystem::path(dlg.GetPath().ToStdWstring()) / std::filesystem::path(xmi->filename).replace_extension(".mid");
		file.SaveAs(path.wstring());
	}

	wxMessageBox("MIDI files exported!","Info",wxICON_INFORMATION);

	lbList->Select(old_selection);
	lbList->EnsureVisible(old_selection);
}

// on selection of MID in the list
void FormMIDI::OnSelectMidClick(wxCommandEvent &ev)
{
	if(!lbList->GetCount())
		return;
	GetFileInfo();
}