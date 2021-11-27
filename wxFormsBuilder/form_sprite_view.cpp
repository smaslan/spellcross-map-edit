///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_sprite_view.h"

///////////////////////////////////////////////////////////////////////////

FormSprite::FormSprite( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	mMenu = new wxMenuBar( 0 );
	mnuFile = new wxMenu();
	wxMenuItem* m_menuItem1;
	m_menuItem1 = new wxMenuItem( mnuFile, wxID_ANY, wxString( wxT("Save sprite") ) , wxEmptyString, wxITEM_NORMAL );
	mnuFile->Append( m_menuItem1 );

	mnuFile->AppendSeparator();

	wxMenuItem* m_menuItem2;
	m_menuItem2 = new wxMenuItem( mnuFile, wxID_ANY, wxString( wxT("Close") ) , wxEmptyString, wxITEM_NORMAL );
	mnuFile->Append( m_menuItem2 );

	mMenu->Append( mnuFile, wxT("File") );

	mnuTerr = new wxMenu();
	mMenu->Append( mnuTerr, wxT("Terrain") );

	mnuEdit = new wxMenu();
	wxMenuItem* btnBuildContext;
	btnBuildContext = new wxMenuItem( mnuEdit, wxID_EDIT_TILE_CONTEXT_AUTO, wxString( wxT("Update tiles' context") ) , wxEmptyString, wxITEM_NORMAL );
	mnuEdit->Append( btnBuildContext );

	mMenu->Append( mnuEdit, wxT("Edit") );

	this->SetMenuBar( mMenu );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	txtSpriteList = new wxStaticText( this, wxID_ANY, wxT("Sprite list:"), wxDefaultPosition, wxDefaultSize, 0 );
	txtSpriteList->Wrap( -1 );
	bSizer2->Add( txtSpriteList, 0, wxLEFT|wxTOP, 5 );

	lboxSprites = new wxListBox( this, wxID_LBOX_SPRITES, wxDefaultPosition, wxSize( 120,-1 ), 0, NULL, 0|wxVSCROLL );
	bSizer2->Add( lboxSprites, 1, wxBOTTOM|wxLEFT, 5 );


	bSizer1->Add( bSizer2, 0, wxEXPAND|wxRIGHT, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );


	bSizer5->Add( 0, 0, 0, wxALL|wxEXPAND, 4 );

	wxStaticBoxSizer* sizerCanvas;
	sizerCanvas = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT(" Sprite(s) view:") ), wxVERTICAL );

	canvas = new wxPanel( sizerCanvas->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sizerCanvas->Add( canvas, 1, wxEXPAND | wxALL, 5 );


	bSizer5->Add( sizerCanvas, 1, wxALL|wxEXPAND, 5 );

	txtGamma = new wxStaticText( this, wxID_ANY, wxT("Set gamma correction:"), wxDefaultPosition, wxDefaultSize, 0 );
	txtGamma->Wrap( -1 );
	bSizer5->Add( txtGamma, 0, wxLEFT|wxTOP, 5 );

	slideGamma = new wxSlider( this, wxID_SLIDE_GAMMA, 1300, 500, 2000, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	bSizer5->Add( slideGamma, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 1 );

	cbZoom = new wxCheckBox( this, wxID_CB_ZOOM, wxT("Zoom 2x"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( cbZoom, 0, wxALL, 5 );


	bSizer4->Add( bSizer5, 1, wxEXPAND, 5 );


	bSizer1->Add( bSizer4, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Neighboring sprites:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer3->Add( m_staticText2, 0, wxLEFT|wxTOP, 5 );

	lboxNeighbor = new wxListBox( this, wxID_LBOX_NEIGHBOR, wxDefaultPosition, wxSize( 120,-1 ), 0, NULL, 0|wxVSCROLL );
	bSizer3->Add( lboxNeighbor, 1, wxBOTTOM|wxLEFT|wxRIGHT, 5 );

	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer3->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );

	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Side:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer3->Add( m_staticText3, 0, wxLEFT, 5 );

	wxString chbSideChoices[] = { wxT("Q1"), wxT("Q2"), wxT("Q3"), wxT("Q4") };
	int chbSideNChoices = sizeof( chbSideChoices ) / sizeof( wxString );
	chbSide = new wxChoice( this, wxID_CH_SIDE, wxDefaultPosition, wxDefaultSize, chbSideNChoices, chbSideChoices, 0 );
	chbSide->SetSelection( 0 );
	bSizer3->Add( chbSide, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );


	bSizer1->Add( bSizer3, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT(" Tile properties:") ), wxVERTICAL );

	cbIsGrass = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_GRASS, wxT("Normal grass"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsGrass, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsDarkGrass = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_DGRASS, wxT("Dark grass"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsDarkGrass, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsBlood = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_BLOOD, wxT("Blood"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsBlood, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsMud = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_MUD, wxT("Mud"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsMud, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsSwamp = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_SWAMP, wxT("Swamp"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsSwamp, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsAsh = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_ASH, wxT("Ash"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsAsh, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsHigh = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_HIGH_LAND, wxT("High land"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsHigh, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsRoad = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_ASH_ROAD, wxT("Asphalt road"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsRoad, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsBrokeAshroad = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_BROKE_ASH_ROAD, wxT("Broken asphalt road"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsBrokeAshroad, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsDirtRoad = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_DIRT_ROAD, wxT("Dirt road"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsDirtRoad, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsMudPath = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_MUD_PATH, wxT("Mud path"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsMudPath, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsCliff = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_CLIFF, wxT("Cliff"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsCliff, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsWater = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_WATER, wxT("Water"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsWater, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsWBridge = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_WBRIDGE, wxT("Wooden bridge"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsWBridge, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsBridge = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_BRIDGE, wxT("Bridge"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsBridge, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsFord = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_FORD, wxT("Ford"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsFord, 0, wxLEFT|wxRIGHT|wxTOP, 5 );


	bSizer1->Add( sbSizer2, 0, wxEXPAND|wxLEFT|wxTOP, 5 );

	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Tile Params:") ), wxVERTICAL );

	sbSizer3->SetMinSize( wxSize( 150,-1 ) );
	m_staticText5 = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("Q1 class:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	sbSizer3->Add( m_staticText5, 0, wxLEFT|wxTOP, 5 );

	wxArrayString chbQ1classChoices;
	chbQ1class = new wxChoice( sbSizer3->GetStaticBox(), wxID_CHB_Q1_CLASS, wxDefaultPosition, wxDefaultSize, chbQ1classChoices, 0 );
	chbQ1class->SetSelection( 0 );
	sbSizer3->Add( chbQ1class, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	m_staticText6 = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("Q2 class:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	sbSizer3->Add( m_staticText6, 0, wxLEFT|wxTOP, 5 );

	wxArrayString chbQ2classChoices;
	chbQ2class = new wxChoice( sbSizer3->GetStaticBox(), wxID_CHB_Q2_CLASS, wxDefaultPosition, wxDefaultSize, chbQ2classChoices, 0 );
	chbQ2class->SetSelection( 0 );
	sbSizer3->Add( chbQ2class, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	m_staticText7 = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("Q3 class:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	sbSizer3->Add( m_staticText7, 0, wxLEFT|wxTOP, 5 );

	wxArrayString chbQ3classChoices;
	chbQ3class = new wxChoice( sbSizer3->GetStaticBox(), wxID_CHB_Q3_CLASS, wxDefaultPosition, wxDefaultSize, chbQ3classChoices, 0 );
	chbQ3class->SetSelection( 0 );
	sbSizer3->Add( chbQ3class, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	m_staticText8 = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("Q4 class:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	sbSizer3->Add( m_staticText8, 0, wxLEFT|wxTOP, 5 );

	wxArrayString chbQ4classChoices;
	chbQ4class = new wxChoice( sbSizer3->GetStaticBox(), wxID_CHB_Q4_CLASS, wxDefaultPosition, wxDefaultSize, chbQ4classChoices, 0 );
	chbQ4class->SetSelection( 0 );
	sbSizer3->Add( chbQ4class, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );


	bSizer1->Add( sbSizer3, 0, wxEXPAND|wxLEFT|wxTOP, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );
}

FormSprite::~FormSprite()
{
}
