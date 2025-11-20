///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_sound.h"
#include "other.h"

///////////////////////////////////////////////////////////////////////////

FormSound::FormSound( wxWindow* parent,SpellData* spell_data,wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	// === AUTO GENERATED STUFF STARTS HERE ===

	this->SetSizeHints( wxSize( 600,400 ), wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );

	m_menubar10 = new wxMenuBar( 0 );
	m_menu19 = new wxMenu();
	wxMenuItem* mmSelect;
	mmSelect = new wxMenuItem( m_menu19, wxID_MM_SELECT, wxString( wxT("Select and Close") ) + wxT('\t') + wxT("Enter"), wxEmptyString, wxITEM_NORMAL );
	m_menu19->Append( mmSelect );

	wxMenuItem* mmClose;
	mmClose = new wxMenuItem( m_menu19, wxID_MM_CLOSE, wxString( wxT("Close") ) + wxT('\t') + wxT("Esc"), wxEmptyString, wxITEM_NORMAL );
	m_menu19->Append( mmClose );

	m_menubar10->Append( m_menu19, wxT("File") );

	this->SetMenuBar( m_menubar10 );

	wxBoxSizer* bSizer77;
	bSizer77 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer78;
	bSizer78 = new wxBoxSizer( wxVERTICAL );

	m_staticText93 = new wxStaticText( this, wxID_ANY, wxT("Sound resources:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText93->Wrap( -1 );
	bSizer78->Add( m_staticText93, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	lbList = new wxListBox( this, wxID_LB_LIST, wxDefaultPosition, wxSize( 200,-1 ), 0, NULL, wxLB_ALWAYS_SB );
	bSizer78->Add( lbList, 1, wxBOTTOM|wxLEFT|wxRIGHT, 5 );

	m_staticText90 = new wxStaticText( this, wxID_ANY, wxT("Filter:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText90->Wrap( -1 );
	bSizer78->Add( m_staticText90, 0, wxLEFT|wxRIGHT, 5 );

	wxArrayString chFilterChoices;
	chFilter = new wxChoice( this, wxID_CHB_FILTER, wxDefaultPosition, wxDefaultSize, chFilterChoices, 0 );
	chFilter->SetSelection( 0 );
	bSizer78->Add( chFilter, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );


	bSizer77->Add( bSizer78, 0, wxEXPAND, 5 );

	m_staticline32 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer77->Add( m_staticline32, 0, wxBOTTOM|wxEXPAND|wxTOP, 5 );

	wxBoxSizer* bSizer93;
	bSizer93 = new wxBoxSizer( wxVERTICAL );

	m_staticText91 = new wxStaticText( this, wxID_ANY, wxT("Properties:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText91->Wrap( -1 );
	bSizer93->Add( m_staticText91, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	gridProps = new wxPropertyGrid(this, wxID_GRID_PROPS, wxDefaultPosition, wxSize( -1,200 ), wxPG_DEFAULT_STYLE);
	bSizer93->Add( gridProps, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	m_staticText92 = new wxStaticText( this, wxID_ANY, wxT("Map placement sound type:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText92->Wrap( -1 );
	bSizer93->Add( m_staticText92, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	wxArrayString chMapSndTypeChoices;
	chMapSndType = new wxChoice( this, wxID_CH_MAP_SND_TYPE, wxDefaultPosition, wxDefaultSize, chMapSndTypeChoices, 0 );
	chMapSndType->SetSelection( 0 );
	bSizer93->Add( chMapSndType, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );


	bSizer93->Add( 0, 0, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer94;
	bSizer94 = new wxBoxSizer( wxHORIZONTAL );

	btnPlay = new wxButton( this, wxID_BTN_PLAY, wxT("PLAY"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer94->Add( btnPlay, 1, wxALL, 5 );

	btnStop = new wxButton( this, wxID_BTN_STOP, wxT("STOP"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer94->Add( btnStop, 1, wxALL, 5 );


	bSizer93->Add( bSizer94, 0, wxEXPAND, 5 );


	bSizer77->Add( bSizer93, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer77 );
	this->Layout();

	this->Centre( wxBOTH );

	// === AUTO GENERATED STUFF STARTS HERE ===

	// back ref to spellcross data
	m_spell_data = spell_data;
	m_sound = NULL;
	m_sample = NULL;
	m_was_set = false;

	Bind(wxEVT_CLOSE_WINDOW,&FormSound::OnClose,this,this->m_windowId);
	Bind(wxEVT_MENU,&FormSound::OnCloseClick,this,wxID_MM_CLOSE);
	Bind(wxEVT_MENU,&FormSound::OnSelectClick,this,wxID_MM_SELECT);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormSound::OnSelectFilter,this,wxID_CHB_FILTER);
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormSound::OnSelectSound,this,wxID_LB_LIST);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormSound::OnPlayClick,this,wxID_BTN_PLAY);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormSound::OnStopClick,this,wxID_BTN_STOP);

	// fill filter selector
	chFilter->Clear();
	chFilter->Append("All");
	chFilter->Append("8-bit");
	chFilter->Append("16-bit");
	chFilter->Append("U");
	chFilter->Select(0);

	// map sound types
	chMapSndType->Clear();
	chMapSndType->Append("Random triggered sounds");
	chMapSndType->Append("Looping sounds");
	chMapSndType->Select(0);

	// fill samples list
	FillList();
}

// destructor
FormSound::~FormSound()
{
}

// on form close
void FormSound::OnClose(wxCloseEvent& ev)
{
	wxPostEvent(GetParent(),ev);
	ev.Skip();
	Destroy();
}

// close button
void FormSound::OnCloseClick(wxCommandEvent& event)
{
	// no exit sound selection	
	m_sample = NULL;
	Close();
}

// select + close button
void FormSound::OnSelectClick(wxCommandEvent& event)
{
	// exit with sound selection
	Close();
}

// get selected sound samples
SpellSample *FormSound::GetSelectedSound()
{
	return(m_sample);
}

// get map sound type
MapSound::SoundType FormSound::GetMapSoundType()
{
	if(chMapSndType->GetSelection() == 1)
		return(MapSound::SoundType::LOOP);
	return(MapSound::SoundType::RANDOM);
}

// set sound resource on startup
void FormSound::SetSound(std::string name, MapSound::SoundType type)
{
	auto snd_id = lbList->FindString(name);
	if(snd_id < 0)
		return;
	chMapSndType->Select(!!(type == MapSound::SoundType::LOOP));
	lbList->Select(snd_id);	
	wxCommandEvent evt;
	OnSelectSound(evt);
	m_was_set = true;
}

// was sound set on startup by SetSound()?
bool FormSound::WasSoundSet()
{
	return(m_was_set);
}


// fill samples list
void FormSound::FillList()
{
	m_sample = NULL;

	auto fid = chFilter->GetSelection();
	std::string filter = "All";
	if(fid >= 0)
		filter = chFilter->GetString(fid);
		
	lbList->Freeze();
	lbList->Clear();
	for(int k = 0; k < m_spell_data->sounds->GetSamplesCount(); k++)
	{
		auto name = m_spell_data->sounds->GetSample(k)->name;
		bool pass = false;
		if(filter == "All")
			pass = true;
		else if(filter == "8-bit" && wildcmp("8*",name))
			pass = true;
		else if(filter == "16-bit" && wildcmp("1*",name))
			pass = true;
		else if(filter == "U" && wildcmp("U*",name))
			pass = true;
		if(pass)
			lbList->Append(name);
	}
	lbList->Thaw();
}

// on select filter
void FormSound::OnSelectFilter(wxCommandEvent& event)
{
	FillList();
}

// on select sound resource
void FormSound::OnSelectSound(wxCommandEvent& event)
{
	m_sample = NULL;
	auto snd_id = lbList->GetSelection();
	if(snd_id < 0)
		return;
	std::string name = lbList->GetString(snd_id).ToStdString();
	m_sample = m_spell_data->sounds->GetSample(name.c_str());
		
	gridProps->Freeze();
	gridProps->Clear();
	gridProps->Append(new wxStringProperty(wxT("Name"),wxT(""),m_sample->name));
	gridProps->Append(new wxStringProperty(wxT("fs [Hz]"),wxT(""),string_format("%d",m_sample->fs)));
	gridProps->Append(new wxStringProperty(wxT("Samples"),wxT(""),string_format("%d",m_sample->samples)));
	gridProps->Append(new wxStringProperty(wxT("Duration [s]"),wxT(""),string_format("%.2f",(double)m_sample->samples/m_sample->fs)));
	gridProps->Thaw();

}

// on play sound
void FormSound::OnPlayClick(wxCommandEvent& event)
{
	auto snd_id = lbList->GetSelection();
	if(snd_id < 0)
		return;
	std::string name = lbList->GetString(snd_id).ToStdString();

	OnStopClick(event);

	m_sound = m_spell_data->sounds->GetSound(name.c_str());
	m_sound->Play();
}

// on stop sound
void FormSound::OnStopClick(wxCommandEvent& event)
{
	if(m_sound)
	{
		m_sound->Stop(true);
		delete m_sound;
		m_sound = NULL;
	}
}