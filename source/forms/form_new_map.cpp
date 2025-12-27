///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_new_map.h"

///////////////////////////////////////////////////////////////////////////

FormNewMap::FormNewMap(wxWindow* parent,SpellData* spell_data,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style) : wxDialog(parent,id,title,pos,size,style)
{
this->SetSizeHints( wxSize( 400,-1 ), wxDefaultSize );

	wxBoxSizer* szrNewMap;
	szrNewMap = new wxBoxSizer( wxVERTICAL );

	m_staticText92 = new wxStaticText( this, wxID_ANY, wxT("Terrain type:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText92->Wrap( -1 );
	szrNewMap->Add( m_staticText92, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	wxArrayString chbTerrainChoices;
	chbTerrain = new wxChoice( this, wxID_CHB_TERR, wxDefaultPosition, wxDefaultSize, chbTerrainChoices, 0 );
	chbTerrain->SetSelection( 0 );
	szrNewMap->Add( chbTerrain, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	m_staticText96 = new wxStaticText( this, wxID_ANY, wxT("Map parameters:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText96->Wrap( -1 );
	szrNewMap->Add( m_staticText96, 0, wxRIGHT|wxLEFT, 5 );

	wxBoxSizer* bSizer97;
	bSizer97 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText97 = new wxStaticText( this, wxID_ANY, wxT("x:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText97->Wrap( -1 );
	bSizer97->Add( m_staticText97, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	spinXsize = new wxSpinCtrl( this, wxID_SPIN_X, wxT("50"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 10, 200, 0 );
	spinXsize->SetMinSize( wxSize( 80,-1 ) );

	bSizer97->Add( spinXsize, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText98 = new wxStaticText( this, wxID_ANY, wxT("y:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText98->Wrap( -1 );
	bSizer97->Add( m_staticText98, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	spinYsize = new wxSpinCtrl( this, wxID_SPIN_Y, wxT("50"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 10, 200, 0 );
	spinYsize->SetMinSize( wxSize( 80,-1 ) );

	bSizer97->Add( spinYsize, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText99 = new wxStaticText( this, wxID_ANY, wxT("Elevation:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText99->Wrap( -1 );
	bSizer97->Add( m_staticText99, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	spinElev = new wxSpinCtrl( this, wxID_SPIN_ELEV, wxT("2"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 8, 0 );
	spinElev->SetMinSize( wxSize( 80,-1 ) );

	bSizer97->Add( spinElev, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	szrNewMap->Add( bSizer97, 0, wxEXPAND, 5 );

	m_staticline4 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	szrNewMap->Add( m_staticline4, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer92;
	bSizer92 = new wxBoxSizer( wxHORIZONTAL );

	btnOk = new wxButton( this, wxID_BTN_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer92->Add( btnOk, 1, wxALL|wxEXPAND, 5 );

	btnCancel = new wxButton( this, wxID_BTN_CANCEL, wxT("CANCEL"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer92->Add( btnCancel, 1, wxALL|wxEXPAND, 5 );


	szrNewMap->Add( bSizer92, 1, wxEXPAND, 5 );


	this->SetSizer( szrNewMap );
	this->Layout();

	this->Centre( wxBOTH );
	// === END OF AUTO GENERATED STUFF ===
	Fit();

	m_spell_data = spell_data;

	// set icon
	wxIcon appIcon;
	appIcon.LoadFile("IDI_ICON2",wxBITMAP_TYPE_ICO_RESOURCE);
	if(appIcon.IsOk())
		SetIcon(appIcon);

	// fill terrains list
	chbTerrain->Freeze();
	chbTerrain->Clear();
	for(int k = 0; k < m_spell_data->GetTerrainCount(); k++)
		chbTerrain->Append(m_spell_data->GetTerrain(k)->name);
	chbTerrain->Thaw();
	chbTerrain->Select(0);

	// assign button shortcuts
	std::vector<wxAcceleratorEntry> entries;
	entries.emplace_back(wxACCEL_NORMAL,WXK_RETURN,wxID_BTN_OK);
	entries.emplace_back(wxACCEL_NORMAL,WXK_ESCAPE,wxID_BTN_CANCEL);
	wxAcceleratorTable accel(entries.size(),entries.data());
	this->SetAcceleratorTable(accel);

	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormNewMap::OnClose,this,wxID_BTN_OK);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormNewMap::OnCancel,this,wxID_BTN_CANCEL);
}

FormNewMap::~FormNewMap()
{
}

// on close form
void FormNewMap::OnClose(wxCommandEvent& event)
{
	EndModal(wxID_OK);
}

// on cancel form
void FormNewMap::OnCancel(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}

// return stuff
std::string FormNewMap::GetTerrain()
{
	return(chbTerrain->GetStringSelection().ToStdString());
}
std::tuple<int,int> FormNewMap::GetSize()
{
	return(std::tuple(spinXsize->GetValue(),spinYsize->GetValue()));
}
int FormNewMap::GetElev()
{
	return(spinElev->GetValue());
}

