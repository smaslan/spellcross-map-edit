///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_sprite_view.h"

///////////////////////////////////////////////////////////////////////////

FormSprite::FormSprite( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 1050,600 ), wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	mMenu = new wxMenuBar( 0 );
	mnuFile = new wxMenu();
	wxMenuItem* m_menuItem1;
	m_menuItem1 = new wxMenuItem( mnuFile, wxID_ANY, wxString( wxT("Save sprite") ) , wxEmptyString, wxITEM_NORMAL );
	mnuFile->Append( m_menuItem1 );

	mnuFile->AppendSeparator();

	wxMenuItem* btnClose;
	btnClose = new wxMenuItem( mnuFile, wxID_BTN_CLOSE, wxString( wxT("Close") ) , wxEmptyString, wxITEM_NORMAL );
	mnuFile->Append( btnClose );

	mMenu->Append( mnuFile, wxT("File") );

	mnuTerr = new wxMenu();
	mMenu->Append( mnuTerr, wxT("Terrain") );

	mnuEdit = new wxMenu();
	wxMenuItem* btnSelQ1;
	btnSelQ1 = new wxMenuItem( mnuEdit, wxID_BTN_SEL_Q1, wxString( wxT("Select edge Q1") ) + wxT('\t') + wxT("F1"), wxEmptyString, wxITEM_NORMAL );
	mnuEdit->Append( btnSelQ1 );

	wxMenuItem* btnSelQ2;
	btnSelQ2 = new wxMenuItem( mnuEdit, wxID_BTN_SEL_Q2, wxString( wxT("Select edge Q2") ) + wxT('\t') + wxT("F2"), wxEmptyString, wxITEM_NORMAL );
	mnuEdit->Append( btnSelQ2 );

	wxMenuItem* btnSelQ3;
	btnSelQ3 = new wxMenuItem( mnuEdit, wxID_BTN_SEL_Q3, wxString( wxT("Select edge Q3") ) + wxT('\t') + wxT("F3"), wxEmptyString, wxITEM_NORMAL );
	mnuEdit->Append( btnSelQ3 );

	wxMenuItem* btnSelQ4;
	btnSelQ4 = new wxMenuItem( mnuEdit, wxID_BTN_SEL_Q4, wxString( wxT("Select edge Q4") ) + wxT('\t') + wxT("F4"), wxEmptyString, wxITEM_NORMAL );
	mnuEdit->Append( btnSelQ4 );

	mnuEdit->AppendSeparator();

	wxMenuItem* btnNext;
	btnNext = new wxMenuItem( mnuEdit, wxID_BTN_NEXT, wxString( wxT("Next sprite") ) + wxT('\t') + wxT("]"), wxEmptyString, wxITEM_NORMAL );
	mnuEdit->Append( btnNext );

	wxMenuItem* btnPrev;
	btnPrev = new wxMenuItem( mnuEdit, wxID_BTN_PREV, wxString( wxT("Previous sprite") ) + wxT('\t') + wxT("["), wxEmptyString, wxITEM_NORMAL );
	mnuEdit->Append( btnPrev );

	mnuEdit->AppendSeparator();

	wxMenuItem* btnSetKnownParams;
	btnSetKnownParams = new wxMenuItem( mnuEdit, wxID_BTN_SET_KNOWS, wxString( wxT("Set parameters of known sprites") ) , wxEmptyString, wxITEM_NORMAL );
	mnuEdit->Append( btnSetKnownParams );

	wxMenuItem* btnClearContext;
	btnClearContext = new wxMenuItem( mnuEdit, wxID_BTN_CLR_CONTEXT, wxString( wxT("Clear tile context") ) , wxEmptyString, wxITEM_NORMAL );
	mnuEdit->Append( btnClearContext );

	wxMenuItem* btnBuildContext;
	btnBuildContext = new wxMenuItem( mnuEdit, wxID_EDIT_TILE_CONTEXT_AUTO, wxString( wxT("Update tiles' context") ) , wxT("Auto generate tile context from edge class data. Not it may take some time!"), wxITEM_NORMAL );
	mnuEdit->Append( btnBuildContext );

	wxMenuItem* btnAutoShadeFlags;
	btnAutoShadeFlags = new wxMenuItem( mnuEdit, wxID_BTN_AUTO_SHADING, wxString( wxT("Auto set shading flags") ) , wxT("This will try to automatically decode sprite names such as PL???_?? and set the edge/corner shading flags."), wxITEM_NORMAL );
	mnuEdit->Append( btnAutoShadeFlags );

	wxMenuItem* btnSaveContext;
	btnSaveContext = new wxMenuItem( mnuEdit, wxID_BTN_SAVE_CONTEXT, wxString( wxT("Save context data") ) , wxT("Save this terrain's sprite context data to a file. "), wxITEM_NORMAL );
	mnuEdit->Append( btnSaveContext );

	mMenu->Append( mnuEdit, wxT("Edit") );

	this->SetMenuBar( mMenu );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	txtSpriteList = new wxStaticText( this, wxID_ANY, wxT("Sprite list:"), wxDefaultPosition, wxDefaultSize, 0 );
	txtSpriteList->Wrap( -1 );
	bSizer2->Add( txtSpriteList, 0, wxLEFT|wxTOP, 5 );

	lboxSprites = new wxListBox( this, wxID_LBOX_SPRITES, wxDefaultPosition, wxSize( 120,-1 ), 0, NULL, 0|wxALWAYS_SHOW_SB|wxVSCROLL );
	bSizer2->Add( lboxSprites, 1, wxBOTTOM|wxLEFT, 5 );


	bSizer1->Add( bSizer2, 0, wxEXPAND|wxRIGHT, 5 );

	wxBoxSizer* bSizer241;
	bSizer241 = new wxBoxSizer( wxVERTICAL );

	txtAltList = new wxStaticText( this, wxID_TXT_ALT, wxT("Alternatives:"), wxDefaultPosition, wxDefaultSize, 0 );
	txtAltList->Wrap( -1 );
	bSizer241->Add( txtAltList, 0, wxTOP|wxLEFT, 5 );

	lboxAlt = new wxListBox( this, wxID_LBOX_ALT, wxDefaultPosition, wxSize( 120,-1 ), 0, NULL, 0|wxALWAYS_SHOW_SB|wxVSCROLL );
	bSizer241->Add( lboxAlt, 1, wxBOTTOM|wxRIGHT|wxLEFT, 5 );


	bSizer1->Add( bSizer241, 0, wxEXPAND, 5 );

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

	cbIsSand = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_SAND, wxT("Sand"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsSand, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsMud = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_MUD, wxT("Mud"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsMud, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsAsh = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_ASH, wxT("Ash"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsAsh, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsSwamp = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_SWAMP, wxT("Swamp"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsSwamp, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsBlood = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_BLOOD, wxT("Blood"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsBlood, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsHigh = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_HIGH_LAND, wxT("High land"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsHigh, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsWater = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_WATER, wxT("Water"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsWater, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsRidge = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_RIDGE, wxT("Ridge"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsRidge, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsCliff = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_CLIFF, wxT("Cliff"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsCliff, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsRoad = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_ASH_ROAD, wxT("Asphalt road"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsRoad, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsDirtRoad = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_DIRT_ROAD, wxT("Dirt road"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsDirtRoad, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsMudPath = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_MUD_PATH, wxT("Mud path"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsMudPath, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsWallBase = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_WALL_BASE, wxT("Wall base"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsWallBase, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsWBridge = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_WBRIDGE, wxT("Wooden bridge"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsWBridge, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsBridge = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_BRIDGE, wxT("Bridge"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsBridge, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsFord = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_FORD, wxT("Ford"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsFord, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsRidgeBridge = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_RIDGE_BRIDGE, wxT("Ridge bridge"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsRidgeBridge, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsBroken = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_BROKEN, wxT("Broken/damaged"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsBroken, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsShadow = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_SHADOW, wxT("Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsShadow, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsScar = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_SCAR, wxT("Terrain scar"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsScar, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	cbIsObject = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_IS_OBJECT, wxT("Object"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbIsObject, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	m_staticline2 = new wxStaticLine( sbSizer2->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sbSizer2->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );

	cbUseAsGlyph = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_USE_AS_GLYPH, wxT("Use as class glyph"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbUseAsGlyph, 0, wxLEFT|wxRIGHT, 5 );

	cbFaultCont = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_CB_FAULTY, wxT("Faulty context"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( cbFaultCont, 0, wxLEFT|wxRIGHT|wxTOP, 5 );


	bSizer1->Add( sbSizer2, 0, wxEXPAND|wxLEFT|wxTOP, 5 );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Tile Params:") ), wxVERTICAL );

	sbSizer3->SetMinSize( wxSize( 300,-1 ) );
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText5 = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("Q1 class:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	bSizer21->Add( m_staticText5, 0, wxLEFT|wxTOP, 5 );

	wxArrayString chbQ1classChoices;
	chbQ1class = new wxChoice( sbSizer3->GetStaticBox(), wxID_CHB_Q1_CLASS, wxDefaultPosition, wxDefaultSize, chbQ1classChoices, 0 );
	chbQ1class->SetSelection( 0 );
	bSizer21->Add( chbQ1class, 1, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	cbQ1nofilt = new wxCheckBox( sbSizer3->GetStaticBox(), wxID_CB_Q1_NOFILT, wxT("Don't filter"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( cbQ1nofilt, 0, wxALL, 5 );


	sbSizer3->Add( bSizer21, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText6 = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("Q2 class:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer22->Add( m_staticText6, 0, wxLEFT|wxTOP, 5 );

	wxArrayString chbQ2classChoices;
	chbQ2class = new wxChoice( sbSizer3->GetStaticBox(), wxID_CHB_Q2_CLASS, wxDefaultPosition, wxDefaultSize, chbQ2classChoices, 0 );
	chbQ2class->SetSelection( 0 );
	bSizer22->Add( chbQ2class, 1, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	cbQ2nofilt = new wxCheckBox( sbSizer3->GetStaticBox(), wxID_CB_Q2_NOFILT, wxT("Don't filter"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer22->Add( cbQ2nofilt, 0, wxALL, 5 );


	sbSizer3->Add( bSizer22, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText7 = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("Q3 class:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer23->Add( m_staticText7, 0, wxLEFT|wxTOP, 5 );

	wxArrayString chbQ3classChoices;
	chbQ3class = new wxChoice( sbSizer3->GetStaticBox(), wxID_CHB_Q3_CLASS, wxDefaultPosition, wxDefaultSize, chbQ3classChoices, 0 );
	chbQ3class->SetSelection( 0 );
	bSizer23->Add( chbQ3class, 1, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	cbQ3nofilt = new wxCheckBox( sbSizer3->GetStaticBox(), wxID_CB_Q3_NOFILT, wxT("Don't filter"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer23->Add( cbQ3nofilt, 0, wxALL, 5 );


	sbSizer3->Add( bSizer23, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText8 = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("Q4 class:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer24->Add( m_staticText8, 0, wxLEFT|wxTOP, 5 );

	wxArrayString chbQ4classChoices;
	chbQ4class = new wxChoice( sbSizer3->GetStaticBox(), wxID_CHB_Q4_CLASS, wxDefaultPosition, wxDefaultSize, chbQ4classChoices, 0 );
	chbQ4class->SetSelection( 0 );
	bSizer24->Add( chbQ4class, 1, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	cbQ4nofilt = new wxCheckBox( sbSizer3->GetStaticBox(), wxID_CB_Q4_NOFILT, wxT("Don't filter"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer24->Add( cbQ4nofilt, 0, wxALL, 5 );


	sbSizer3->Add( bSizer24, 1, wxEXPAND, 5 );

	txtShadeEdge = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("Shaded edges:"), wxDefaultPosition, wxDefaultSize, 0 );
	txtShadeEdge->Wrap( -1 );
	sbSizer3->Add( txtShadeEdge, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 2, 4, 0, 0 );

	cbShadeQ1 = new wxCheckBox( sbSizer3->GetStaticBox(), wxID_CB_SHADE_Q1, wxT("Q1"), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE|wxCHK_ALLOW_3RD_STATE_FOR_USER );
	gSizer1->Add( cbShadeQ1, 0, wxALL, 5 );

	cbShadeQ2 = new wxCheckBox( sbSizer3->GetStaticBox(), wxID_CB_SHADE_Q2, wxT("Q2"), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE|wxCHK_ALLOW_3RD_STATE_FOR_USER );
	gSizer1->Add( cbShadeQ2, 0, wxALL, 5 );

	cbShadeQ3 = new wxCheckBox( sbSizer3->GetStaticBox(), wxID_CB_SHADE_Q3, wxT("Q3"), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE|wxCHK_ALLOW_3RD_STATE_FOR_USER );
	gSizer1->Add( cbShadeQ3, 0, wxALL, 5 );

	cbShadeQ4 = new wxCheckBox( sbSizer3->GetStaticBox(), wxID_CB_SHADE_Q4, wxT("Q4"), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE|wxCHK_ALLOW_3RD_STATE_FOR_USER );
	gSizer1->Add( cbShadeQ4, 0, wxALL, 5 );

	cbShadeC1 = new wxCheckBox( sbSizer3->GetStaticBox(), wxID_CB_SHADE_C1, wxT("C1"), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE|wxCHK_ALLOW_3RD_STATE_FOR_USER );
	gSizer1->Add( cbShadeC1, 0, wxALL, 5 );

	cbShadeC2 = new wxCheckBox( sbSizer3->GetStaticBox(), wxID_CB_SHADE_C2, wxT("C2"), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE|wxCHK_ALLOW_3RD_STATE_FOR_USER );
	gSizer1->Add( cbShadeC2, 0, wxALL, 5 );

	cbShadeC3 = new wxCheckBox( sbSizer3->GetStaticBox(), wxID_CB_SHADE_C3, wxT("C3"), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE|wxCHK_ALLOW_3RD_STATE_FOR_USER );
	gSizer1->Add( cbShadeC3, 0, wxALL, 5 );

	cbShadeC4 = new wxCheckBox( sbSizer3->GetStaticBox(), wxID_CB_SHADE_C4, wxT("C4"), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE|wxCHK_ALLOW_3RD_STATE_FOR_USER );
	gSizer1->Add( cbShadeC4, 0, wxALL, 5 );


	sbSizer3->Add( gSizer1, 0, 0, 5 );

	txtFlags = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("Map tile flags:"), wxDefaultPosition, wxDefaultSize, 0 );
	txtFlags->Wrap( -1 );
	sbSizer3->Add( txtFlags, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	editMapFlags = new wxTextCtrl( sbSizer3->GetStaticBox(), wxID_EDIT_MAP_TILE_FLAGS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	sbSizer3->Add( editMapFlags, 0, wxBOTTOM|wxLEFT|wxRIGHT, 5 );


	bSizer10->Add( sbSizer3, 0, wxLEFT|wxTOP|wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Object/class/tools sorting:") ), wxVERTICAL );

	m_staticText13 = new wxStaticText( sbSizer6->GetStaticBox(), wxID_ANY, wxT("Objects class:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	sbSizer6->Add( m_staticText13, 0, wxLEFT|wxTOP, 5 );

	wxArrayString chbToolClassChoices;
	chbToolClass = new wxChoice( sbSizer6->GetStaticBox(), wxID_CHB_TOOL_CLASS, wxDefaultPosition, wxDefaultSize, chbToolClassChoices, 0 );
	chbToolClass->SetSelection( 0 );
	sbSizer6->Add( chbToolClass, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	m_staticText14 = new wxStaticText( sbSizer6->GetStaticBox(), wxID_ANY, wxT("Objects group:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	sbSizer6->Add( m_staticText14, 0, wxLEFT|wxTOP, 5 );

	wxArrayString chbToolObjGroupChoices;
	chbToolObjGroup = new wxChoice( sbSizer6->GetStaticBox(), wxID_CHB_TOOL_OBJ_GROUP, wxDefaultPosition, wxDefaultSize, chbToolObjGroupChoices, 0 );
	chbToolObjGroup->SetSelection( 0 );
	sbSizer6->Add( chbToolObjGroup, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	cbToolGlyph = new wxCheckBox( sbSizer6->GetStaticBox(), wxID_CB_TOOL_GLYPH, wxT("Use as tool glyph"), wxDefaultPosition, wxDefaultSize, 0 );
	cbToolGlyph->SetValue(true);
	sbSizer6->Add( cbToolGlyph, 0, wxALL, 5 );


	bSizer10->Add( sbSizer6, 1, wxEXPAND|wxLEFT|wxTOP, 5 );


	bSizer1->Add( bSizer10, 0, wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();
	statBar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );

	this->Centre( wxBOTH );
}

FormSprite::~FormSprite()
{
}

FormObjects::FormObjects( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 800,400 ), wxDefaultSize );
	this->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNTEXT ) );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );

	sbar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_SB_MAIN );
	wxBoxSizer* szrMain;
	szrMain = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* szrList;
	szrList = new wxBoxSizer( wxVERTICAL );

	szrList->SetMinSize( wxSize( 250,-1 ) );
	m_staticText14 = new wxStaticText( this, wxID_ANY, wxT("Objects list:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	szrList->Add( m_staticText14, 0, wxLEFT|wxTOP, 5 );

	lbObjects = new wxListBox( this, wxID_LB_OBJECTS, wxDefaultPosition, wxSize( 100,-1 ), 0, NULL, wxLB_ALWAYS_SB );
	szrList->Add( lbObjects, 1, wxLEFT|wxRIGHT|wxEXPAND, 5 );

	wxGridSizer* szrUpDown;
	szrUpDown = new wxGridSizer( 0, 2, 0, 0 );

	btnUp = new wxButton( this, wxID_BTN_UP, wxT("Up"), wxDefaultPosition, wxDefaultSize, 0 );
	szrUpDown->Add( btnUp, 0, wxALL|wxEXPAND, 5 );

	btnDown = new wxButton( this, wxID_BTN_DOWN, wxT("Down"), wxDefaultPosition, wxDefaultSize, 0 );
	szrUpDown->Add( btnDown, 0, wxALL|wxEXPAND, 5 );


	szrList->Add( szrUpDown, 0, wxEXPAND, 5 );


	szrMain->Add( szrList, 0, wxEXPAND, 5 );

	m_staticline5 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	szrMain->Add( m_staticline5, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* szrView;
	szrView = new wxBoxSizer( wxVERTICAL );

	m_staticText15 = new wxStaticText( this, wxID_ANY, wxT("Object preview:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	szrView->Add( m_staticText15, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	canvas = new wxPanel( this, wxID_CANVAS, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	szrView->Add( canvas, 1, wxEXPAND | wxALL, 5 );

	m_staticline8 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	szrView->Add( m_staticline8, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

	m_staticText19 = new wxStaticText( this, wxID_ANY, wxT("Object name (keep it short, press Shift+Enter to confirm):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	szrView->Add( m_staticText19, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	txtName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	szrView->Add( txtName, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	m_staticline9 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	szrView->Add( m_staticline9, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

	txtGamma = new wxStaticText( this, wxID_TXT_GAMMA, wxT("Gamma correction:"), wxDefaultPosition, wxDefaultSize, 0 );
	txtGamma->Wrap( -1 );
	szrView->Add( txtGamma, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	slideGamma = new wxSlider( this, wxID_SLIDE_GAMMA, 1300, 500, 2000, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	szrView->Add( slideGamma, 0, wxEXPAND|wxALL, 1 );


	szrMain->Add( szrView, 1, wxEXPAND, 5 );

	m_staticline6 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	szrMain->Add( m_staticline6, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* szrProps;
	szrProps = new wxBoxSizer( wxVERTICAL );

	szrProps->SetMinSize( wxSize( 250,-1 ) );
	m_staticText17 = new wxStaticText( this, wxID_ANY, wxT("Objects class:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	szrProps->Add( m_staticText17, 0, wxLEFT|wxTOP, 5 );

	wxArrayString chbObjectClassChoices;
	chbObjectClass = new wxChoice( this, wxID_CHB_CLASS, wxDefaultPosition, wxDefaultSize, chbObjectClassChoices, 0 );
	chbObjectClass->SetSelection( 0 );
	szrProps->Add( chbObjectClass, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	m_staticText18 = new wxStaticText( this, wxID_ANY, wxT("Objects group:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	szrProps->Add( m_staticText18, 0, wxLEFT|wxTOP, 5 );

	wxArrayString chbObjectsGroupChoices;
	chbObjectsGroup = new wxChoice( this, wxID_CHB_GROUP, wxDefaultPosition, wxDefaultSize, chbObjectsGroupChoices, 0 );
	chbObjectsGroup->SetSelection( 0 );
	szrProps->Add( chbObjectsGroup, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );


	szrMain->Add( szrProps, 0, wxEXPAND, 5 );


	this->SetSizer( szrMain );
	this->Layout();
	m_menubar2 = new wxMenuBar( 0 );
	mnuFile = new wxMenu();
	wxMenuItem* btnSaveList;
	btnSaveList = new wxMenuItem( mnuFile, wxID_MM_SAVE_OBJECTS, wxString( wxT("Save objects") ) , wxEmptyString, wxITEM_NORMAL );
	mnuFile->Append( btnSaveList );

	mnuFile->AppendSeparator();

	wxMenuItem* btnClose;
	btnClose = new wxMenuItem( mnuFile, wxID_MM_CLOSE, wxString( wxT("Close") ) , wxEmptyString, wxITEM_NORMAL );
	mnuFile->Append( btnClose );

	m_menubar2->Append( mnuFile, wxT("File") );

	mnuTerr = new wxMenu();
	m_menubar2->Append( mnuTerr, wxT("Terrain") );

	mnuEdit = new wxMenu();
	wxMenuItem* btnNext;
	btnNext = new wxMenuItem( mnuEdit, wxID_MM_NEXT, wxString( wxT("Next item") ) + wxT('\t') + wxT("Ctrl+]"), wxEmptyString, wxITEM_NORMAL );
	mnuEdit->Append( btnNext );

	wxMenuItem* btnPrev;
	btnPrev = new wxMenuItem( mnuEdit, wxID_MM_PREV, wxString( wxT("Previous item") ) + wxT('\t') + wxT("Ctrl+["), wxEmptyString, wxITEM_NORMAL );
	mnuEdit->Append( btnPrev );

	mnuEdit->AppendSeparator();

	wxMenuItem* btnRemove;
	btnRemove = new wxMenuItem( mnuEdit, wxID_MM_REMOVE, wxString( wxT("Delete object") ) + wxT('\t') + wxT("Shift+Delete"), wxEmptyString, wxITEM_NORMAL );
	mnuEdit->Append( btnRemove );

	wxMenuItem* mmRename;
	mmRename = new wxMenuItem( mnuEdit, wxID_MM_RENAME, wxString( wxT("Rename object") ) + wxT('\t') + wxT("Shift+Enter"), wxEmptyString, wxITEM_NORMAL );
	mnuEdit->Append( mmRename );

	m_menubar2->Append( mnuEdit, wxT("Edit") );

	this->SetMenuBar( m_menubar2 );


	this->Centre( wxBOTH );
}

FormObjects::~FormObjects()
{
}

FormNewObject::FormNewObject( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 400,120 ), wxDefaultSize );

	wxBoxSizer* szrNewObj;
	szrNewObj = new wxBoxSizer( wxVERTICAL );

	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("Object description:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	szrNewObj->Add( m_staticText11, 0, wxLEFT|wxTOP, 5 );

	txtDescription = new wxTextCtrl( this, wxID_TXT_DESC, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	szrNewObj->Add( txtDescription, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );


	szrNewObj->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticline4 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	szrNewObj->Add( m_staticline4, 0, wxEXPAND | wxALL, 5 );

	btnOk = new wxButton( this, wxID_BTN_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	szrNewObj->Add( btnOk, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( szrNewObj );
	this->Layout();

	this->Centre( wxBOTH );
}

FormNewObject::~FormNewObject()
{
}

FormTools::FormTools( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );

	sbar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );
	wxBoxSizer* szrMain;
	szrMain = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* szrToolset;
	szrToolset = new wxBoxSizer( wxVERTICAL );

	m_staticText19 = new wxStaticText( this, wxID_ANY, wxT("Toolsets:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	szrToolset->Add( m_staticText19, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	lbToolset = new wxListBox( this, wxID_LB_TOOLSET, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB );
	szrToolset->Add( lbToolset, 1, wxEXPAND|wxRIGHT|wxLEFT, 5 );

	m_staticText21 = new wxStaticText( this, wxID_ANY, wxT("Toolset name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	szrToolset->Add( m_staticText21, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	txtNewToolset = new wxTextCtrl( this, wxID_TXT_NEW_TOOLSET, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	szrToolset->Add( txtNewToolset, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );

	m_staticText23 = new wxStaticText( this, wxID_ANY, wxT("Toolset title:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText23->Wrap( -1 );
	szrToolset->Add( m_staticText23, 0, wxRIGHT|wxLEFT, 5 );

	txtToolsetTitle = new wxTextCtrl( this, wxID_TXT_TOOLSET_TITLE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	szrToolset->Add( txtToolsetTitle, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );

	wxBoxSizer* szrScaling;
	szrScaling = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* szrScale;
	szrScale = new wxBoxSizer( wxVERTICAL );

	m_staticText26 = new wxStaticText( this, wxID_ANY, wxT("Glyph scaling:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText26->Wrap( -1 );
	szrScale->Add( m_staticText26, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	wxArrayString chbScalingChoices;
	chbScaling = new wxChoice( this, wxID_CHB_SCALE, wxDefaultPosition, wxDefaultSize, chbScalingChoices, 0 );
	chbScaling->SetSelection( 0 );
	szrScale->Add( chbScaling, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );


	szrScaling->Add( szrScale, 1, wxEXPAND, 5 );

	wxBoxSizer* szrWidth;
	szrWidth = new wxBoxSizer( wxVERTICAL );

	m_staticText27 = new wxStaticText( this, wxID_ANY, wxT("Width:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText27->Wrap( -1 );
	szrWidth->Add( m_staticText27, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	scWidth = new wxSpinCtrl( this, wxID_SC_WIDTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 32, 200, 80 );
	scWidth->SetMinSize( wxSize( 80,-1 ) );
	scWidth->SetMaxSize( wxSize( 80,-1 ) );

	szrWidth->Add( scWidth, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );


	szrScaling->Add( szrWidth, 0, wxEXPAND, 5 );

	wxBoxSizer* szrHeight;
	szrHeight = new wxBoxSizer( wxVERTICAL );

	m_staticText28 = new wxStaticText( this, wxID_ANY, wxT("Height:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText28->Wrap( -1 );
	szrHeight->Add( m_staticText28, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	scHeight = new wxSpinCtrl( this, wxID_SC_HEIGHT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 32, 200, 80 );
	scHeight->SetMinSize( wxSize( 80,-1 ) );
	scHeight->SetMaxSize( wxSize( 80,-1 ) );

	szrHeight->Add( scHeight, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );


	szrScaling->Add( szrHeight, 0, wxEXPAND, 5 );


	szrToolset->Add( szrScaling, 0, wxEXPAND, 5 );

	wxGridSizer* szrToolsetBtn;
	szrToolsetBtn = new wxGridSizer( 0, 5, 0, 0 );

	btnAddToolset = new wxButton( this, wxID_BTN_TOOLSET_NEW, wxT("New"), wxDefaultPosition, wxDefaultSize, 0 );
	szrToolsetBtn->Add( btnAddToolset, 0, wxALL|wxEXPAND, 5 );

	btnRenameToolset = new wxButton( this, wxID_BTN_TOOLSET_RENAME, wxT("Update"), wxDefaultPosition, wxDefaultSize, 0 );
	szrToolsetBtn->Add( btnRenameToolset, 0, wxALL|wxEXPAND, 5 );

	btnToolsetDel = new wxButton( this, wxID_BTN_TOOLSET_REM, wxT("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	szrToolsetBtn->Add( btnToolsetDel, 0, wxALL|wxEXPAND, 5 );

	btnToolsetUp = new wxButton( this, wxID_BTN_TOOLSET_UP, wxT("Up"), wxDefaultPosition, wxDefaultSize, 0 );
	szrToolsetBtn->Add( btnToolsetUp, 0, wxALL|wxEXPAND, 5 );

	btnToolsetDown = new wxButton( this, wxID_BTN_TOOLSET_DOWN, wxT("Down"), wxDefaultPosition, wxDefaultSize, 0 );
	szrToolsetBtn->Add( btnToolsetDown, 0, wxALL|wxEXPAND, 5 );


	szrToolset->Add( szrToolsetBtn, 0, wxEXPAND, 5 );


	szrMain->Add( szrToolset, 1, wxEXPAND, 5 );

	wxBoxSizer* szrTool;
	szrTool = new wxBoxSizer( wxVERTICAL );

	m_staticText20 = new wxStaticText( this, wxID_ANY, wxT("Tools:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText20->Wrap( -1 );
	szrTool->Add( m_staticText20, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	lbTool = new wxListBox( this, wxID_LB_TOOL, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB );
	szrTool->Add( lbTool, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	m_staticText22 = new wxStaticText( this, wxID_ANY, wxT("Tool name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText22->Wrap( -1 );
	szrTool->Add( m_staticText22, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	txtNewTool = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	szrTool->Add( txtNewTool, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );

	wxGridSizer* srzToolBtn;
	srzToolBtn = new wxGridSizer( 0, 5, 0, 0 );

	btnNewTool = new wxButton( this, wxID_BTN_TOOL_NEW, wxT("New"), wxDefaultPosition, wxDefaultSize, 0 );
	srzToolBtn->Add( btnNewTool, 0, wxALL|wxEXPAND, 5 );

	btnRenameTool = new wxButton( this, wxID_BTN_TOOL_RENAME, wxT("Rename"), wxDefaultPosition, wxDefaultSize, 0 );
	srzToolBtn->Add( btnRenameTool, 0, wxALL|wxEXPAND, 5 );

	btnToolDel = new wxButton( this, wxID_BTN_TOOL_REM, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	srzToolBtn->Add( btnToolDel, 0, wxALL|wxEXPAND, 5 );

	btnToolUp = new wxButton( this, wxID_BTN_TOOL_UP, wxT("Up"), wxDefaultPosition, wxDefaultSize, 0 );
	srzToolBtn->Add( btnToolUp, 0, wxALL|wxEXPAND, 5 );

	btnToolDown = new wxButton( this, wxID_BTN_TOOL_DOWN, wxT("Down"), wxDefaultPosition, wxDefaultSize, 0 );
	srzToolBtn->Add( btnToolDown, 0, wxALL|wxEXPAND, 5 );


	szrTool->Add( srzToolBtn, 0, wxEXPAND, 5 );


	szrMain->Add( szrTool, 1, wxEXPAND, 5 );


	this->SetSizer( szrMain );
	this->Layout();
	mmBar = new wxMenuBar( 0 );
	mmFile = new wxMenu();
	wxMenuItem* mmSave;
	mmSave = new wxMenuItem( mmFile, wxID_MM_SAVE, wxString( wxT("Save list") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmSave );

	mmFile->AppendSeparator();

	wxMenuItem* mmClose;
	mmClose = new wxMenuItem( mmFile, wxID_MM_CLOSE, wxString( wxT("Close") ) , wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmClose );

	mmBar->Append( mmFile, wxT("File") );

	mmTerrain = new wxMenu();
	mmBar->Append( mmTerrain, wxT("Terrain") );

	mmEdit = new wxMenu();
	wxMenuItem* mmClrAllToolset;
	mmClrAllToolset = new wxMenuItem( mmEdit, wxID_MM_CLR_TOOLSETS, wxString( wxT("Remove all toolsets") ) , wxEmptyString, wxITEM_NORMAL );
	mmEdit->Append( mmClrAllToolset );

	wxMenuItem* mmClrAllTools;
	mmClrAllTools = new wxMenuItem( mmEdit, wxID_CLR_TOOLS, wxString( wxT("Remove all tools") ) , wxEmptyString, wxITEM_NORMAL );
	mmEdit->Append( mmClrAllTools );

	mmBar->Append( mmEdit, wxT("Edit") );

	this->SetMenuBar( mmBar );


	this->Centre( wxBOTH );
}

FormTools::~FormTools()
{
}

FormPalView::FormPalView( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 950,300 ), wxDefaultSize );
	this->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ) );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );

	mMenu = new wxMenuBar( 0 );
	mmFile = new wxMenu();
	wxMenuItem* mmClose;
	mmClose = new wxMenuItem( mmFile, wxID_MM_CLOSE, wxString( wxT("Close") ) , wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmClose );

	mMenu->Append( mmFile, wxT("File") );

	mmTerrain = new wxMenu();
	mMenu->Append( mmTerrain, wxT("Terrain") );

	mmFilter = new wxMenu();
	mMenu->Append( mmFilter, wxT("Filter") );

	this->SetMenuBar( mMenu );

	wxBoxSizer* szCanvas;
	szCanvas = new wxBoxSizer( wxVERTICAL );

	m_staticText28 = new wxStaticText( this, wxID_ANY, wxT("Palette:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText28->Wrap( -1 );
	szCanvas->Add( m_staticText28, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	canvas = new wxPanel( this, wxID_CANVAS, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	szCanvas->Add( canvas, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	m_staticText29 = new wxStaticText( this, wxID_ANY, wxT("Selected color:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText29->Wrap( -1 );
	szCanvas->Add( m_staticText29, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	color = new wxPanel( this, wxID_COLOR, wxDefaultPosition, wxSize( -1,20 ), wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	color->SetMaxSize( wxSize( -1,20 ) );

	szCanvas->Add( color, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	wxBoxSizer* szFilter;
	szFilter = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* szRed;
	szRed = new wxBoxSizer( wxVERTICAL );

	txtFilterRed = new wxStaticText( this, wxID_ANY, wxT("Red:"), wxDefaultPosition, wxDefaultSize, 0 );
	txtFilterRed->Wrap( -1 );
	szRed->Add( txtFilterRed, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	slideRed = new wxSlider( this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL );
	szRed->Add( slideRed, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );


	szFilter->Add( szRed, 1, wxEXPAND, 5 );

	wxBoxSizer* szGreen;
	szGreen = new wxBoxSizer( wxVERTICAL );

	txtFilterGreen = new wxStaticText( this, wxID_ANY, wxT("Green:"), wxDefaultPosition, wxDefaultSize, 0 );
	txtFilterGreen->Wrap( -1 );
	szGreen->Add( txtFilterGreen, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	slideGreen = new wxSlider( this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL );
	szGreen->Add( slideGreen, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );


	szFilter->Add( szGreen, 1, wxEXPAND, 5 );

	wxBoxSizer* szBlue;
	szBlue = new wxBoxSizer( wxVERTICAL );

	txtFilterBlue = new wxStaticText( this, wxID_ANY, wxT("Blue:"), wxDefaultPosition, wxDefaultSize, 0 );
	txtFilterBlue->Wrap( -1 );
	szBlue->Add( txtFilterBlue, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	slideBlue = new wxSlider( this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL );
	szBlue->Add( slideBlue, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );


	szFilter->Add( szBlue, 1, wxEXPAND, 5 );


	szCanvas->Add( szFilter, 0, wxEXPAND, 5 );


	this->SetSizer( szCanvas );
	this->Layout();
	sbar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_STAT_BAR );
	timer.SetOwner( this, timer.GetId() );

	this->Centre( wxBOTH );
}

FormPalView::~FormPalView()
{
}

FormGResView::FormGResView( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );

	m_menubar5 = new wxMenuBar( 0 );
	mmFile = new wxMenu();
	wxMenuItem* mmClose;
	mmClose = new wxMenuItem( mmFile, wxID_MM_CLOSE, wxString( wxT("Close") ) , wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmClose );

	m_menubar5->Append( mmFile, wxT("File") );

	this->SetMenuBar( m_menubar5 );

	sbar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_SB );
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer( wxVERTICAL );

	m_staticText31 = new wxStaticText( this, wxID_ANY, wxT("Files:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText31->Wrap( -1 );
	bSizer27->Add( m_staticText31, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	lboxFiles = new wxListBox( this, wxID_LB_FILES, wxDefaultPosition, wxSize( 150,-1 ), 0, NULL, 0|wxALWAYS_SHOW_SB|wxVSCROLL );
	bSizer27->Add( lboxFiles, 1, wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	m_staticText32 = new wxStaticText( this, wxID_ANY, wxT("Width:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText32->Wrap( -1 );
	bSizer27->Add( m_staticText32, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	spinWidth = new wxSpinCtrl( this, wxID_SPIN_W, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	bSizer27->Add( spinWidth, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


	bSizer26->Add( bSizer27, 0, wxEXPAND, 5 );

	m_staticline8 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer26->Add( m_staticline8, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );

	canvas = new wxPanel( this, wxID_CANVAS, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer26->Add( canvas, 1, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer26 );
	this->Layout();

	this->Centre( wxBOTH );
}

FormGResView::~FormGResView()
{
}

FormUnits::FormUnits( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );

	sbar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_SB );
	mmenu = new wxMenuBar( 0 );
	mmFile = new wxMenu();
	wxMenuItem* mmSaveAux;
	mmSaveAux = new wxMenuItem( mmFile, wxID_MM_SAVE_AUX, wxString( wxT("Save Aux Data") ) , wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmSaveAux );

	mmFile->AppendSeparator();

	wxMenuItem* mmExit;
	mmExit = new wxMenuItem( mmFile, wxID_MM_EXIT, wxString( wxT("Exit") ) + wxT('\t') + wxT("Esc"), wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmExit );

	mmenu->Append( mmFile, wxT("File") );

	mmEdit = new wxMenu();
	wxMenuItem* mmCopyGrpOrg;
	mmCopyGrpOrg = new wxMenuItem( mmEdit, wxID_MM_COPY_GRP_ORG, wxString( wxT("Copy fire origins") ) + wxT('\t') + wxT("Ctrl+C"), wxEmptyString, wxITEM_NORMAL );
	mmEdit->Append( mmCopyGrpOrg );

	wxMenuItem* mmPasteGrpOrg;
	mmPasteGrpOrg = new wxMenuItem( mmEdit, wxID_MM_PASTE_GRP_ORG, wxString( wxT("Paste fire origins") ) + wxT('\t') + wxT("Ctrl+V"), wxEmptyString, wxITEM_NORMAL );
	mmEdit->Append( mmPasteGrpOrg );

	mmEdit->AppendSeparator();

	wxMenuItem* mmSet;
	mmSet = new wxMenuItem( mmEdit, wxID_MM_SET, wxString( wxT("Update/place unit") ) + wxT('\t') + wxT("Enter"), wxEmptyString, wxITEM_NORMAL );
	mmEdit->Append( mmSet );

	mmenu->Append( mmEdit, wxT("Edit") );

	this->SetMenuBar( mmenu );

	wxBoxSizer* szMain;
	szMain = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* szList;
	szList = new wxBoxSizer( wxVERTICAL );

	szList->SetMinSize( wxSize( 180,-1 ) );
	m_staticText32 = new wxStaticText( this, wxID_ANY, wxT("Units list:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText32->Wrap( -1 );
	szList->Add( m_staticText32, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	lboxUnits = new wxListBox( this, wxID_LB_UNITS, wxDefaultPosition, wxSize( -1,-1 ), 0, NULL, wxLB_ALWAYS_SB|wxALWAYS_SHOW_SB|wxVSCROLL );
	lboxUnits->SetMaxSize( wxSize( 180,-1 ) );

	szList->Add( lboxUnits, 1, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );


	szMain->Add( szList, 0, wxEXPAND, 5 );

	m_staticline9 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	szMain->Add( m_staticline9, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );

	wxBoxSizer* szProps;
	szProps = new wxBoxSizer( wxVERTICAL );

	m_staticText35 = new wxStaticText( this, wxID_ANY, wxT("Unit icon:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText35->Wrap( -1 );
	szProps->Add( m_staticText35, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	icon_canvas = new wxPanel( this, wxID_CANVAS_ICON, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	icon_canvas->SetMaxSize( wxSize( -1,100 ) );

	szProps->Add( icon_canvas, 1, wxEXPAND|wxALL, 5 );

	m_staticText34 = new wxStaticText( this, wxID_ANY, wxT("Properties:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText34->Wrap( -1 );
	szProps->Add( m_staticText34, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	props = new wxPropertyGrid(this, wxID_PROPS, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	props->SetMinSize( wxSize( 200,-1 ) );

	szProps->Add( props, 1, wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	m_staticline13 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	szProps->Add( m_staticline13, 0, wxEXPAND | wxALL, 5 );

	m_staticText60 = new wxStaticText( this, wxID_ANY, wxT("Unit index:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText60->Wrap( -1 );
	szProps->Add( m_staticText60, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	spinID = new wxSpinCtrl( this, wxID_SPIN_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
	szProps->Add( spinID, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	m_staticText39 = new wxStaticText( this, wxID_ANY, wxT("Custom name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText39->Wrap( -1 );
	szProps->Add( m_staticText39, 0, wxLEFT|wxRIGHT, 5 );

	txtName = new wxTextCtrl( this, wxID_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	szProps->Add( txtName, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	m_staticText40 = new wxStaticText( this, wxID_ANY, wxT("Health:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText40->Wrap( -1 );
	szProps->Add( m_staticText40, 0, wxRIGHT|wxLEFT, 5 );

	spinHealth = new wxSpinCtrl( this, wxID_SPIN_HEALTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	szProps->Add( spinHealth, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	m_staticText75 = new wxStaticText( this, wxID_ANY, wxT("Experience:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText75->Wrap( -1 );
	szProps->Add( m_staticText75, 0, wxLEFT|wxRIGHT, 5 );

	slideXP = new wxSlider( this, wxID_ANY, 50, 0, 12, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_MIN_MAX_LABELS|wxSL_SELRANGE|wxSL_VALUE_LABEL );
	szProps->Add( slideXP, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	m_staticText401 = new wxStaticText( this, wxID_ANY, wxT("Behaviour:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText401->Wrap( -1 );
	szProps->Add( m_staticText401, 0, wxLEFT|wxRIGHT, 5 );

	wxArrayString chUnitBehaveChoices;
	chUnitBehave = new wxChoice( this, wxID_UNIT_BEHAVE, wxDefaultPosition, wxDefaultSize, chUnitBehaveChoices, 0 );
	chUnitBehave->SetSelection( 0 );
	szProps->Add( chUnitBehave, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

	m_staticText74 = new wxStaticText( this, wxID_ANY, wxT("Special unit type (event-units only):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText74->Wrap( -1 );
	szProps->Add( m_staticText74, 0, wxLEFT|wxRIGHT, 5 );

	wxArrayString chUnitTypeChoices;
	chUnitType = new wxChoice( this, wxID_UNIT_TYPE, wxDefaultPosition, wxDefaultSize, chUnitTypeChoices, 0 );
	chUnitType->SetSelection( 0 );
	szProps->Add( chUnitType, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );


	szMain->Add( szProps, 0, wxEXPAND, 5 );

	m_staticline10 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	szMain->Add( m_staticline10, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );

	wxBoxSizer* szArt;
	szArt = new wxBoxSizer( wxVERTICAL );

	pages = new wxNotebook( this, wxID_PAGE_CTRL, wxDefaultPosition, wxDefaultSize, 0 );
	pageArt = new wxPanel( pages, wxID_PAGE_ART, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer35;
	bSizer35 = new wxBoxSizer( wxVERTICAL );

	m_staticText36 = new wxStaticText( pageArt, wxID_ANY, wxT("Unit art:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText36->Wrap( -1 );
	bSizer35->Add( m_staticText36, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	art_canvas = new wxPanel( pageArt, wxID_CANVAS_ART, wxDefaultPosition, wxSize( -1,-1 ), wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	art_canvas->SetMinSize( wxSize( -1,400 ) );

	bSizer35->Add( art_canvas, 1, wxEXPAND | wxALL, 5 );

	m_staticline11 = new wxStaticLine( pageArt, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer35->Add( m_staticline11, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* szArtSel;
	szArtSel = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* szArtSelector;
	szArtSelector = new wxBoxSizer( wxVERTICAL );

	szArtSelector->SetMinSize( wxSize( 150,-1 ) );
	m_staticText37 = new wxStaticText( pageArt, wxID_ANY, wxT("Art selection:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText37->Wrap( -1 );
	szArtSelector->Add( m_staticText37, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	lboxArt = new wxListBox( pageArt, wxID_LB_ART, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB );
	szArtSelector->Add( lboxArt, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


	szArtSel->Add( szArtSelector, 0, wxEXPAND, 5 );

	m_staticline12 = new wxStaticLine( pageArt, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	szArtSel->Add( m_staticline12, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );

	wxBoxSizer* szInfo;
	szInfo = new wxBoxSizer( wxVERTICAL );

	m_staticText38 = new wxStaticText( pageArt, wxID_ANY, wxT("Unit description:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText38->Wrap( -1 );
	szInfo->Add( m_staticText38, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	txtInfo = new wxTextCtrl( pageArt, wxID_TXT_INFO, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_LEFT|wxTE_MULTILINE|wxTE_WORDWRAP );
	szInfo->Add( txtInfo, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


	szArtSel->Add( szInfo, 1, wxEXPAND, 5 );


	bSizer35->Add( szArtSel, 1, wxEXPAND, 5 );


	pageArt->SetSizer( bSizer35 );
	pageArt->Layout();
	bSizer35->Fit( pageArt );
	pages->AddPage( pageArt, wxT("Unit art"), false );
	pageGrp = new wxPanel( pages, wxID_PAGE_GRP, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer36;
	bSizer36 = new wxBoxSizer( wxVERTICAL );

	m_staticText42 = new wxStaticText( pageGrp, wxID_ANY, wxT("Sprite render:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText42->Wrap( -1 );
	bSizer36->Add( m_staticText42, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	grp_canvas = new wxPanel( pageGrp, wxID_CANVAS_GRP, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer36->Add( grp_canvas, 1, wxEXPAND | wxALL, 5 );

	m_staticline14 = new wxStaticLine( pageGrp, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer36->Add( m_staticline14, 0, wxEXPAND | wxALL, 5 );

	m_staticText43 = new wxStaticText( pageGrp, wxID_ANY, wxT("Graphics type:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText43->Wrap( -1 );
	bSizer36->Add( m_staticText43, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	wxArrayString chbGrpTypeChoices;
	chbGrpType = new wxChoice( pageGrp, wxID_CHB_GRP_TYPE, wxDefaultPosition, wxDefaultSize, chbGrpTypeChoices, 0 );
	chbGrpType->SetSelection( 0 );
	bSizer36->Add( chbGrpType, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	wxBoxSizer* bSizer37;
	bSizer37 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer38;
	bSizer38 = new wxBoxSizer( wxVERTICAL );

	txtGrpZim = new wxStaticText( pageGrp, wxID_TXT_GRP_AZIM, wxT("Azimuth:"), wxDefaultPosition, wxDefaultSize, 0 );
	txtGrpZim->Wrap( -1 );
	bSizer38->Add( txtGrpZim, 0, wxRIGHT|wxLEFT, 5 );

	slideAzim = new wxSlider( pageGrp, wxID_SLIDE_AZIM, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_BOTTOM|wxSL_HORIZONTAL|wxSL_MIN_MAX_LABELS|wxSL_VALUE_LABEL );
	bSizer38->Add( slideAzim, 0, wxALL|wxEXPAND, 5 );


	bSizer37->Add( bSizer38, 1, wxEXPAND, 5 );

	m_staticline15 = new wxStaticLine( pageGrp, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer37->Add( m_staticline15, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer39;
	bSizer39 = new wxBoxSizer( wxVERTICAL );

	txtGrpSlope = new wxStaticText( pageGrp, wxID_TXT_GRP_SLOPE, wxT("Slope:"), wxDefaultPosition, wxDefaultSize, 0 );
	txtGrpSlope->Wrap( -1 );
	bSizer39->Add( txtGrpSlope, 0, wxRIGHT|wxLEFT, 5 );

	slideSlope = new wxSlider( pageGrp, wxID_SLIDE_SLOPE, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_BOTTOM|wxSL_HORIZONTAL|wxSL_MIN_MAX_LABELS|wxSL_VALUE_LABEL );
	bSizer39->Add( slideSlope, 0, wxALL|wxEXPAND, 5 );


	bSizer37->Add( bSizer39, 1, wxEXPAND, 5 );

	m_staticline16 = new wxStaticLine( pageGrp, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer37->Add( m_staticline16, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer40;
	bSizer40 = new wxBoxSizer( wxVERTICAL );

	m_staticText46 = new wxStaticText( pageGrp, wxID_ANY, wxT("Frame:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText46->Wrap( -1 );
	bSizer40->Add( m_staticText46, 0, wxRIGHT|wxLEFT, 5 );

	slideFrame = new wxSlider( pageGrp, wxID_SLIDE_FRAME, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_BOTTOM|wxSL_HORIZONTAL|wxSL_MIN_MAX_LABELS|wxSL_VALUE_LABEL );
	bSizer40->Add( slideFrame, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


	bSizer37->Add( bSizer40, 1, wxEXPAND, 5 );


	bSizer36->Add( bSizer37, 0, wxEXPAND, 5 );

	m_staticline17 = new wxStaticLine( pageGrp, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer36->Add( m_staticline17, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer41;
	bSizer41 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer42;
	bSizer42 = new wxBoxSizer( wxVERTICAL );

	txtGrpZoom = new wxStaticText( pageGrp, wxID_ANY, wxT("Zoom:"), wxDefaultPosition, wxDefaultSize, 0 );
	txtGrpZoom->Wrap( -1 );
	bSizer42->Add( txtGrpZoom, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	slideGrpZoom = new wxSlider( pageGrp, wxID_SLIDE_GRP_ZOOM, 1, 1, 4, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_BOTTOM|wxSL_HORIZONTAL|wxSL_LABELS|wxSL_MIN_MAX_LABELS );
	bSizer42->Add( slideGrpZoom, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	txtGrpGamma = new wxStaticText( pageGrp, wxID_ANY, wxT("Gamma correction:"), wxDefaultPosition, wxDefaultSize, 0 );
	txtGrpGamma->Wrap( -1 );
	bSizer42->Add( txtGrpGamma, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	slideGrpGamma = new wxSlider( pageGrp, wxID_SLIDE_GRP_GAMMA, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_BOTTOM|wxSL_HORIZONTAL );
	bSizer42->Add( slideGrpGamma, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


	bSizer41->Add( bSizer42, 1, wxEXPAND, 5 );

	m_staticline18 = new wxStaticLine( pageGrp, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer41->Add( m_staticline18, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer43;
	bSizer43 = new wxBoxSizer( wxVERTICAL );

	cbGrpOrigin = new wxCheckBox( pageGrp, wxID_CB_GRP_ORG, wxT("Show origin"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer43->Add( cbGrpOrigin, 0, wxALL, 5 );

	cbGrpFireOrg = new wxCheckBox( pageGrp, wxID_CB_GRP_FIRE_ORG, wxT("Show fire origin"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer43->Add( cbGrpFireOrg, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	cbGrpFireMean = new wxCheckBox( pageGrp, wxID_CB_GRP_FIRE_ORG_MEAN, wxT("Show fire origins mean"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer43->Add( cbGrpFireMean, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	cbGrpFireCenter = new wxCheckBox( pageGrp, wxID_CB_GRP_FIRE_CENTER, wxT("Show turret origin"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer43->Add( cbGrpFireCenter, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );


	bSizer41->Add( bSizer43, 1, wxEXPAND, 5 );


	bSizer36->Add( bSizer41, 0, wxEXPAND, 5 );


	pageGrp->SetSizer( bSizer36 );
	pageGrp->Layout();
	bSizer36->Fit( pageGrp );
	pages->AddPage( pageGrp, wxT("Unit graphics"), true );

	szArt->Add( pages, 1, wxEXPAND | wxALL, 5 );


	szMain->Add( szArt, 1, wxEXPAND, 5 );

	m_listBox11 = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB );
	m_listBox11->SetMinSize( wxSize( 200,-1 ) );

	szMain->Add( m_listBox11, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( szMain );
	this->Layout();

	this->Centre( wxBOTH );
}

FormUnits::~FormUnits()
{
}

FormEvent::FormEvent( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 900,600 ), wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );

	mMenu = new wxMenuBar( 0 );
	mmFile = new wxMenu();
	wxMenuItem* mmOk;
	mmOk = new wxMenuItem( mmFile, wxID_MM_OK, wxString( wxT("Place/Update") ) + wxT('\t') + wxT("Ctrl+Enter"), wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmOk );

	wxMenuItem* mmExit;
	mmExit = new wxMenuItem( mmFile, wxID_MM_EXIT, wxString( wxT("Exit") ) + wxT('\t') + wxT("ESC"), wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmExit );

	mMenu->Append( mmFile, wxT("File") );

	this->SetMenuBar( mMenu );

	wxBoxSizer* bSizer47;
	bSizer47 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer531;
	bSizer531 = new wxBoxSizer( wxVERTICAL );

	bSizer531->SetMinSize( wxSize( 200,-1 ) );
	m_staticText57 = new wxStaticText( this, wxID_ANY, wxT("Events list:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText57->Wrap( -1 );
	bSizer531->Add( m_staticText57, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	lbEvents = new wxListBox( this, wxID_LB_EVENTS, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB );
	bSizer531->Add( lbEvents, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	wxBoxSizer* bSizer54;
	bSizer54 = new wxBoxSizer( wxHORIZONTAL );

	btnAddEvent = new wxButton( this, wxID_BTN_ADD_EVENT, wxT("New"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer54->Add( btnAddEvent, 1, wxALL, 5 );

	btnRemoveEvent = new wxButton( this, wxID_BTN_REM_EVENT, wxT("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer54->Add( btnRemoveEvent, 1, wxALL, 5 );


	bSizer531->Add( bSizer54, 0, wxEXPAND, 5 );


	bSizer47->Add( bSizer531, 0, wxEXPAND, 5 );

	m_staticline21 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer47->Add( m_staticline21, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer50;
	bSizer50 = new wxBoxSizer( wxVERTICAL );

	bSizer50->SetMinSize( wxSize( 300,-1 ) );
	m_staticText50 = new wxStaticText( this, wxID_ANY, wxT("Event type:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText50->Wrap( -1 );
	bSizer50->Add( m_staticText50, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	wxArrayString chbTypeChoices;
	chbType = new wxChoice( this, wxID_CHB_TYPE, wxDefaultPosition, wxDefaultSize, chbTypeChoices, 0 );
	chbType->SetSelection( 0 );
	bSizer50->Add( chbType, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );

	m_staticText62 = new wxStaticText( this, wxID_ANY, wxT("MissionObjective() description:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText62->Wrap( -1 );
	bSizer50->Add( m_staticText62, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	txtObjectiveDesc = new wxTextCtrl( this, wxID_TXT_OBJ_DESC, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	bSizer50->Add( txtObjectiveDesc, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	cbIsObjective = new wxCheckBox( this, wxID_CB_IS_OBJECTIVE, wxT("is MissionObjective?"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer50->Add( cbIsObjective, 0, wxALL, 5 );

	wxBoxSizer* bSizer48;
	bSizer48 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText52 = new wxStaticText( this, wxID_ANY, wxT("Map x-position:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText52->Wrap( -1 );
	m_staticText52->SetMinSize( wxSize( 110,-1 ) );

	bSizer48->Add( m_staticText52, 0, wxALIGN_CENTER|wxALL, 5 );

	spinXpos = new wxSpinCtrl( this, wxID_SPIN_XPOS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	spinXpos->SetMinSize( wxSize( 70,-1 ) );

	bSizer48->Add( spinXpos, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );

	m_staticText54 = new wxStaticText( this, wxID_ANY, wxT("Map y-position:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText54->Wrap( -1 );
	bSizer48->Add( m_staticText54, 0, wxALIGN_CENTER|wxALL, 5 );

	spinYpos = new wxSpinCtrl( this, wxID_SPIN_YPOS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	spinYpos->SetMinSize( wxSize( 70,-1 ) );

	bSizer48->Add( spinYpos, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );


	bSizer50->Add( bSizer48, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer56;
	bSizer56 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText59 = new wxStaticText( this, wxID_ANY, wxT("Trigger unit index:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText59->Wrap( -1 );
	m_staticText59->SetMinSize( wxSize( 110,-1 ) );

	bSizer56->Add( m_staticText59, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	spinTrigUnit = new wxSpinCtrl( this, wxID_SPIN_TRIG_UNIT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 99, 0 );
	spinTrigUnit->SetMinSize( wxSize( 70,-1 ) );

	bSizer56->Add( spinTrigUnit, 0, wxALL, 5 );


	bSizer50->Add( bSizer56, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer561;
	bSizer561 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText51 = new wxStaticText( this, wxID_ANY, wxT("Probability:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText51->Wrap( -1 );
	m_staticText51->SetMinSize( wxSize( 110,-1 ) );

	bSizer561->Add( m_staticText51, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	spinProb = new wxSpinCtrl( this, wxID_SPIN_PROB, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100, 100 );
	spinProb->SetMinSize( wxSize( 70,-1 ) );

	bSizer561->Add( spinProb, 0, wxALL, 5 );


	bSizer50->Add( bSizer561, 0, wxEXPAND, 5 );

	m_staticline20 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer50->Add( m_staticline20, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer55;
	bSizer55 = new wxBoxSizer( wxHORIZONTAL );

	strUnits = new wxStaticText( this, wxID_ANY, wxT("Spawned units count:"), wxDefaultPosition, wxDefaultSize, 0 );
	strUnits->Wrap( -1 );
	bSizer55->Add( strUnits, 0, wxALIGN_CENTER|wxRIGHT|wxLEFT, 5 );

	txtUnits = new wxTextCtrl( this, wxID_TXT_UNITS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	bSizer55->Add( txtUnits, 1, wxRIGHT|wxLEFT, 5 );


	bSizer50->Add( bSizer55, 0, wxEXPAND, 5 );

	m_staticline22 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer50->Add( m_staticline22, 0, wxEXPAND | wxALL, 5 );

	m_staticText74 = new wxStaticText( this, wxID_ANY, wxT("Play CAN animation:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText74->Wrap( -1 );
	bSizer50->Add( m_staticText74, 0, wxLEFT|wxRIGHT, 5 );

	wxArrayString chbCANanimChoices;
	chbCANanim = new wxChoice( this, wxID_CAN_ANIM, wxDefaultPosition, wxDefaultSize, chbCANanimChoices, 0 );
	chbCANanim->SetSelection( 0 );
	bSizer50->Add( chbCANanim, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

	m_staticline221 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer50->Add( m_staticline221, 0, wxEXPAND | wxALL, 5 );

	m_staticText53 = new wxStaticText( this, wxID_ANY, wxT("Event message(s) list:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText53->Wrap( -1 );
	bSizer50->Add( m_staticText53, 0, wxLEFT|wxRIGHT, 5 );

	wxArrayString chbMsgItemChoices;
	chbMsgItem = new wxChoice( this, wxID_CHB_MSG_ITEM, wxDefaultPosition, wxDefaultSize, chbMsgItemChoices, 0 );
	chbMsgItem->SetSelection( 0 );
	bSizer50->Add( chbMsgItem, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	wxBoxSizer* bSizer49;
	bSizer49 = new wxBoxSizer( wxHORIZONTAL );

	btnNewMsg = new wxButton( this, wxID_BTN_NEW_MSG, wxT("New"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer49->Add( btnNewMsg, 1, wxALL, 5 );

	btnDelMsg = new wxButton( this, wxID_BNT_DEL_MSG, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer49->Add( btnDelMsg, 1, wxALL, 5 );

	btnMsgUp = new wxButton( this, wxID_BTN_MSG_UP, wxT("Move up"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer49->Add( btnMsgUp, 1, wxALL, 5 );

	btnMsgDown = new wxButton( this, wxID_BTN_MSG_DOWN, wxT("Move down"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer49->Add( btnMsgDown, 1, wxALL, 5 );


	bSizer50->Add( bSizer49, 0, wxEXPAND, 5 );

	m_staticText55 = new wxStaticText( this, wxID_ANY, wxT("Message text resource:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText55->Wrap( -1 );
	bSizer50->Add( m_staticText55, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	lbMsg = new wxListBox( this, wxID_LB_MSG, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB|wxALWAYS_SHOW_SB|wxVSCROLL );
	bSizer50->Add( lbMsg, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


	bSizer47->Add( bSizer50, 0, wxEXPAND, 5 );

	m_staticline19 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer47->Add( m_staticline19, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );

	wxBoxSizer* bSizer52;
	bSizer52 = new wxBoxSizer( wxVERTICAL );

	m_staticText56 = new wxStaticText( this, wxID_ANY, wxT("Message text:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText56->Wrap( -1 );
	bSizer52->Add( m_staticText56, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	txtMessage = new wxTextCtrl( this, wxID_TXT_MSG, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_BESTWRAP|wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP );
	bSizer52->Add( txtMessage, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	wxBoxSizer* bSizer53;
	bSizer53 = new wxBoxSizer( wxHORIZONTAL );

	btnMsgPlay = new wxButton( this, wxID_BTN_PLAY_MSG, wxT("Play narration"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer53->Add( btnMsgPlay, 1, wxALL, 5 );

	btnMsgStop = new wxButton( this, wxID_BTN_STOP_MSG, wxT("Stop narration"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer53->Add( btnMsgStop, 1, wxALL, 5 );


	bSizer52->Add( bSizer53, 0, wxEXPAND, 5 );


	bSizer47->Add( bSizer52, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer47 );
	this->Layout();
	sBar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );

	this->Centre( wxBOTH );
}

FormEvent::~FormEvent()
{
}

FormLoader::FormLoader( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* szrLoader;
	szrLoader = new wxBoxSizer( wxVERTICAL );

	m_staticText63 = new wxStaticText( this, wxID_ANY, wxT("Progress:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText63->Wrap( -1 );
	szrLoader->Add( m_staticText63, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	txtList = new wxTextCtrl( this, wxID_TXT_LIST, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP );
	szrLoader->Add( txtList, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	m_staticText62 = new wxStaticText( this, wxID_ANY, wxT("Loading item:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText62->Wrap( -1 );
	szrLoader->Add( m_staticText62, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	txtItem = new wxTextCtrl( this, wxID_TXT_ITEM, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	szrLoader->Add( txtItem, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	btnOK = new wxButton( this, wxID_BTN_OK, wxT("EXIT"), wxDefaultPosition, wxDefaultSize, 0 );
	szrLoader->Add( btnOK, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( szrLoader );
	this->Layout();

	this->Centre( wxBOTH );
}

FormLoader::~FormLoader()
{
}

FormVideo::FormVideo( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	mmenu = new wxMenuBar( 0 );
	mmFile = new wxMenu();
	wxMenuItem* mmOpen;
	mmOpen = new wxMenuItem( mmFile, wxID_MM_OPEN, wxString( wxT("Open Video File") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmOpen );

	wxMenuItem* mmSaveFrames;
	mmSaveFrames = new wxMenuItem( mmFile, wxID_MM_SAVE_FRAMES, wxString( wxT("Save raw frames") ) , wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmSaveFrames );

	mmFile->AppendSeparator();

	wxMenuItem* mmExit;
	mmExit = new wxMenuItem( mmFile, wxID_MM_EXIT, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmExit );

	mmenu->Append( mmFile, wxT("File") );

	this->SetMenuBar( mmenu );

	wxBoxSizer* bSizer58;
	bSizer58 = new wxBoxSizer( wxVERTICAL );

	m_staticText64 = new wxStaticText( this, wxID_ANY, wxT("Frame view:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText64->Wrap( -1 );
	bSizer58->Add( m_staticText64, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	canvas = new wxPanel( this, wxID_CANVAS, wxDefaultPosition, wxSize( 320,200 ), wxTAB_TRAVERSAL );
	canvas->SetMinSize( wxSize( 320,200 ) );
	canvas->SetMaxSize( wxSize( 320,200 ) );

	bSizer58->Add( canvas, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	m_staticText65 = new wxStaticText( this, wxID_DATA_POS, wxT("Offset:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText65->Wrap( -1 );
	bSizer58->Add( m_staticText65, 0, wxALL, 5 );

	slideOffset = new wxSlider( this, wxID_SLIDE_OFS, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	bSizer58->Add( slideOffset, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer59;
	bSizer59 = new wxBoxSizer( wxHORIZONTAL );

	btnPlay = new wxButton( this, wxID_BTN_PLAY, wxT("Play"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer59->Add( btnPlay, 0, wxALL, 5 );

	btnStop = new wxButton( this, wxID_BTN_STOP, wxT("Stop"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer59->Add( btnStop, 0, wxALL, 5 );


	bSizer58->Add( bSizer59, 0, wxEXPAND, 5 );


	this->SetSizer( bSizer58 );
	this->Layout();
	m_statusBar8 = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );

	this->Centre( wxBOTH );
}

FormVideo::~FormVideo()
{
}

FormMIDI::FormMIDI( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );

	mmenu = new wxMenuBar( 0 );
	mmFile = new wxMenu();
	wxMenuItem* mmCustomFolder;
	mmCustomFolder = new wxMenuItem( mmFile, wxID_MM_CUST_FOLDER, wxString( wxT("Select MIDI folder") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmCustomFolder );

	wxMenuItem* mmSaveFile;
	mmSaveFile = new wxMenuItem( mmFile, wxID_MM_SAVE_MIDI, wxString( wxT("Export MIDI file") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmSaveFile );

	wxMenuItem* mmSaveAll;
	mmSaveAll = new wxMenuItem( mmFile, wxID_MM_SAVE_ALL, wxString( wxT("Export all MIDI") ) , wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmSaveAll );

	mmFile->AppendSeparator();

	wxMenuItem* mmExit;
	mmExit = new wxMenuItem( mmFile, wxID_MM_EXIT, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmExit );

	mmenu->Append( mmFile, wxT("File") );

	this->SetMenuBar( mmenu );

	wxBoxSizer* bSizer60;
	bSizer60 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer63;
	bSizer63 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer61;
	bSizer61 = new wxBoxSizer( wxVERTICAL );

	labelList = new wxStaticText( this, wxID_ANY, wxT("MIDI list:"), wxDefaultPosition, wxDefaultSize, 0 );
	labelList->Wrap( -1 );
	bSizer61->Add( labelList, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	lbList = new wxListBox( this, wxID_LB_LIST, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB );
	lbList->SetMinSize( wxSize( 250,-1 ) );

	bSizer61->Add( lbList, 1, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );

	m_staticText69 = new wxStaticText( this, wxID_ANY, wxT("Source:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText69->Wrap( -1 );
	bSizer61->Add( m_staticText69, 0, wxALL, 5 );

	rbSrcMusicFS = new wxRadioButton( this, wxID_RB_SRC_MUSIC_FS, wxT("MUSIC.FS"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer61->Add( rbSrcMusicFS, 0, wxRIGHT|wxLEFT, 5 );

	wxBoxSizer* bSizer67;
	bSizer67 = new wxBoxSizer( wxHORIZONTAL );

	rbSrcFolder = new wxRadioButton( this, wxID_RB_SRC_FOLDER, wxT("Custom folder"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer67->Add( rbSrcFolder, 0, wxALL, 5 );

	btnSrcFolder = new wxButton( this, wxID_BTN_SRC_FOLDER, wxT("Open"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer67->Add( btnSrcFolder, 0, wxRIGHT|wxLEFT, 5 );


	bSizer61->Add( bSizer67, 0, wxEXPAND, 5 );


	bSizer63->Add( bSizer61, 0, wxEXPAND, 5 );

	m_staticline24 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer63->Add( m_staticline24, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );

	wxBoxSizer* bSizer62;
	bSizer62 = new wxBoxSizer( wxVERTICAL );

	labelInfo = new wxStaticText( this, wxID_ANY, wxT("MIDI description:"), wxDefaultPosition, wxDefaultSize, 0 );
	labelInfo->Wrap( -1 );
	bSizer62->Add( labelInfo, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	txtInfo = new wxTextCtrl( this, wxID_TXT_INFO, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_WORDWRAP );
	bSizer62->Add( txtInfo, 1, wxEXPAND|wxRIGHT|wxLEFT, 5 );

	m_staticText68 = new wxStaticText( this, wxID_ANY, wxT("MIDI volume:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText68->Wrap( -1 );
	bSizer62->Add( m_staticText68, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	wxBoxSizer* bSizer66;
	bSizer66 = new wxBoxSizer( wxHORIZONTAL );


	bSizer62->Add( bSizer66, 1, wxEXPAND, 5 );

	slideVolume = new wxSlider( this, wxID_SLIDE_VOLUME, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_BOTH|wxSL_HORIZONTAL|wxSL_MIN_MAX_LABELS|wxSL_SELRANGE );
	bSizer62->Add( slideVolume, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	m_staticline26 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer62->Add( m_staticline26, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	wxBoxSizer* bSizer65;
	bSizer65 = new wxBoxSizer( wxHORIZONTAL );

	btnStop = new wxButton( this, wxID_BTN_STOP, wxT("STOP"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer65->Add( btnStop, 1, wxRIGHT|wxLEFT, 5 );

	btnPlay = new wxButton( this, wxID_BTN_PLAY, wxT("PLAY"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer65->Add( btnPlay, 1, wxRIGHT|wxLEFT, 5 );


	bSizer62->Add( bSizer65, 0, wxEXPAND, 5 );


	bSizer63->Add( bSizer62, 1, wxEXPAND, 5 );


	bSizer60->Add( bSizer63, 1, wxEXPAND, 5 );

	m_staticline23 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer60->Add( m_staticline23, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

	wxBoxSizer* bSizer64;
	bSizer64 = new wxBoxSizer( wxHORIZONTAL );


	bSizer64->Add( 0, 0, 1, 0, 5 );

	btnOK = new wxButton( this, wxID_BTN_OK, wxT("EXIT"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer64->Add( btnOK, 0, wxALL, 5 );


	bSizer60->Add( bSizer64, 0, wxEXPAND, 5 );


	this->SetSizer( bSizer60 );
	this->Layout();

	this->Centre( wxBOTH );
}

FormMIDI::~FormMIDI()
{
}

FormMissionParams::FormMissionParams( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 600,400 ), wxDefaultSize );

	wxBoxSizer* bSizer69;
	bSizer69 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer70;
	bSizer70 = new wxBoxSizer( wxVERTICAL );

	bSizer70->SetMinSize( wxSize( 250,-1 ) );
	m_staticText75 = new wxStaticText( this, wxID_ANY, wxT("Mission text resource:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText75->Wrap( -1 );
	bSizer70->Add( m_staticText75, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	wxArrayString chTextChoices;
	chText = new wxChoice( this, wxID_CH_TEXT, wxDefaultPosition, wxDefaultSize, chTextChoices, 0 );
	chText->SetSelection( 0 );
	bSizer70->Add( chText, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

	m_staticText76 = new wxStaticText( this, wxID_ANY, wxT("Mission start text resource:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText76->Wrap( -1 );
	bSizer70->Add( m_staticText76, 0, wxLEFT|wxRIGHT, 5 );

	wxArrayString chStartTextChoices;
	chStartText = new wxChoice( this, wxID_CH_START_TEXT, wxDefaultPosition, wxDefaultSize, chStartTextChoices, 0 );
	chStartText->SetSelection( 0 );
	bSizer70->Add( chStartText, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	m_staticText77 = new wxStaticText( this, wxID_ANY, wxT("Mission good end text resource:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText77->Wrap( -1 );
	bSizer70->Add( m_staticText77, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	wxArrayString chGoodEndTextChoices;
	chGoodEndText = new wxChoice( this, wxID_CH_GOOD_END_TEXT, wxDefaultPosition, wxDefaultSize, chGoodEndTextChoices, 0 );
	chGoodEndText->SetSelection( 0 );
	bSizer70->Add( chGoodEndText, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	m_staticText78 = new wxStaticText( this, wxID_ANY, wxT("Mission bad end txt resource:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText78->Wrap( -1 );
	bSizer70->Add( m_staticText78, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	wxArrayString chBadEndTextChoices;
	chBadEndText = new wxChoice( this, wxID_CH_BAD_END_TEXT, wxDefaultPosition, wxDefaultSize, chBadEndTextChoices, 0 );
	chBadEndText->SetSelection( 0 );
	bSizer70->Add( chBadEndText, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	cbNight = new wxCheckBox( this, wxID_CB_NIGHT, wxT("Is Night Mission?"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer70->Add( cbNight, 0, wxALL, 5 );


	bSizer70->Add( 0, 0, 1, wxEXPAND, 5 );


	bSizer71->Add( bSizer70, 0, wxEXPAND, 5 );

	m_staticline28 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer71->Add( m_staticline28, 0, wxBOTTOM|wxEXPAND|wxTOP, 5 );

	wxBoxSizer* bSizer72;
	bSizer72 = new wxBoxSizer( wxVERTICAL );

	txtPrevLbl = new wxStaticText( this, wxID_ANY, wxT("Text resource preview:"), wxDefaultPosition, wxDefaultSize, 0 );
	txtPrevLbl->Wrap( -1 );
	bSizer72->Add( txtPrevLbl, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	txtPreview = new wxTextCtrl( this, wxID_TEXT_PREVIEW, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_WORDWRAP );
	bSizer72->Add( txtPreview, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );


	bSizer71->Add( bSizer72, 1, wxEXPAND, 5 );


	bSizer69->Add( bSizer71, 1, wxEXPAND, 5 );

	m_staticline27 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer69->Add( m_staticline27, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

	btnOK = new wxButton( this, wxID_BTN_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer69->Add( btnOK, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );


	this->SetSizer( bSizer69 );
	this->Layout();

	this->Centre( wxBOTH );
}

FormMissionParams::~FormMissionParams()
{
}
