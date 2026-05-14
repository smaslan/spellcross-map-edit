///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "forms.h"

///////////////////////////////////////////////////////////////////////////

FormSprite::FormSprite( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 1050,700 ), wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	mMenu = new wxMenuBar( 0 );
	mnuFile = new wxMenu();
	wxMenuItem* btnSave;
	btnSave = new wxMenuItem( mnuFile, wxID_BTN_SAVE, wxString( wxT("Save sprite") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	mnuFile->Append( btnSave );

	mnuFile->AppendSeparator();

	wxMenuItem* btnSelect;
	btnSelect = new wxMenuItem( mnuFile, wxID_BTN_SELECT, wxString( wxT("Select and Close") ) + wxT('\t') + wxT("Enter"), wxEmptyString, wxITEM_NORMAL );
	mnuFile->Append( btnSelect );

	wxMenuItem* btnClose;
	btnClose = new wxMenuItem( mnuFile, wxID_BTN_CLOSE, wxString( wxT("Close") ) + wxT('\t') + wxT("Esc"), wxEmptyString, wxITEM_NORMAL );
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

	wxMenuItem* btnGenWallCont;
	btnGenWallCont = new wxMenuItem( mnuEdit, wxID_BTN_GEN_WALL_CONT, wxString( wxT("Generate wall neighbors") ) , wxEmptyString, wxITEM_NORMAL );
	mnuEdit->Append( btnGenWallCont );

	wxMenuItem* btnClearContext;
	btnClearContext = new wxMenuItem( mnuEdit, wxID_BTN_CLR_CONTEXT, wxString( wxT("Clear tile context") ) , wxEmptyString, wxITEM_NORMAL );
	mnuEdit->Append( btnClearContext );

	wxMenuItem* btnClearAllContext;
	btnClearAllContext = new wxMenuItem( mnuEdit, wxID_BTN_CLR_ALL_CONTEXT, wxString( wxT("Clear all tiles context") ) , wxEmptyString, wxITEM_NORMAL );
	mnuEdit->Append( btnClearAllContext );

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

	mnuTools = new wxMenu();
	wxMenuItem* mmNewToolset;
	mmNewToolset = new wxMenuItem( mnuTools, wxID_MM_NEW_TOOLSET, wxString( wxT("New Toolset") ) + wxT('\t') + wxT("Ctrl+N"), wxEmptyString, wxITEM_NORMAL );
	mnuTools->Append( mmNewToolset );

	wxMenuItem* mmNewTool;
	mmNewTool = new wxMenuItem( mnuTools, wxID_MM_NEW_TOOL, wxString( wxT("New Tool") ) + wxT('\t') + wxT("Ctrl+T"), wxEmptyString, wxITEM_NORMAL );
	mnuTools->Append( mmNewTool );

	wxMenuItem* mmRemoveTool;
	mmRemoveTool = new wxMenuItem( mnuTools, wxID_MM_REMOVE_TOOL, wxString( wxT("Remove Tool(set)") ) + wxT('\t') + wxT("Delete"), wxEmptyString, wxITEM_NORMAL );
	mnuTools->Append( mmRemoveTool );

	wxMenuItem* mmEditToolset;
	mmEditToolset = new wxMenuItem( mnuTools, wxID_MM_EDIT_TOOLSET, wxString( wxT("Edit Toolset") ) + wxT('\t') + wxT("Ctrl+E"), wxEmptyString, wxITEM_NORMAL );
	mnuTools->Append( mmEditToolset );

	mMenu->Append( mnuTools, wxT("Tools") );

	this->SetMenuBar( mMenu );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* sizerSpriteList;
	sizerSpriteList = new wxBoxSizer( wxVERTICAL );

	txtSpriteList = new wxStaticText( this, wxID_ANY, wxT("Sprite list:"), wxDefaultPosition, wxDefaultSize, 0 );
	txtSpriteList->Wrap( -1 );
	sizerSpriteList->Add( txtSpriteList, 0, wxLEFT|wxTOP, 5 );

	lboxSprites = new wxListCtrlVirtual(this,wxID_LBOX_SPRITES,wxDefaultPosition,wxSize(120,-1),wxLC_NO_HEADER|wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VIRTUAL|wxALWAYS_SHOW_SB|wxVSCROLL);
	sizerSpriteList->Add( lboxSprites, 1, wxEXPAND|wxBOTTOM|wxLEFT, 5 );


	bSizer1->Add( sizerSpriteList, 0, wxEXPAND|wxRIGHT, 5 );

	wxBoxSizer* bSizer241;
	bSizer241 = new wxBoxSizer( wxVERTICAL );

	txtAltList = new wxStaticText( this, wxID_TXT_ALT, wxT("Alternatives:"), wxDefaultPosition, wxDefaultSize, 0 );
	txtAltList->Wrap( -1 );
	bSizer241->Add( txtAltList, 0, wxTOP|wxLEFT, 5 );

	lboxAlt = new wxListBox( this, wxID_LBOX_ALT, wxDefaultPosition, wxSize( 120,-1 ), 0, NULL, 0|wxALWAYS_SHOW_SB|wxVSCROLL );
	bSizer241->Add( lboxAlt, 1, wxBOTTOM|wxLEFT|wxRIGHT, 5 );


	bSizer1->Add( bSizer241, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );


	bSizer4->Add( 0, 0, 0, wxALL|wxEXPAND, 4 );

	wxStaticBoxSizer* sizerCanvas;
	sizerCanvas = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT(" Sprite(s) view:") ), wxVERTICAL );

	canvas = new wxPanel( sizerCanvas->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sizerCanvas->Add( canvas, 1, wxEXPAND | wxALL, 5 );


	bSizer4->Add( sizerCanvas, 1, wxALL|wxEXPAND, 5 );

	txtGamma = new wxStaticText( this, wxID_ANY, wxT("Set gamma correction:"), wxDefaultPosition, wxDefaultSize, 0 );
	txtGamma->Wrap( -1 );
	bSizer4->Add( txtGamma, 0, wxLEFT|wxTOP, 5 );

	slideGamma = new wxSlider( this, wxID_SLIDE_GAMMA, 1300, 500, 2000, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	bSizer4->Add( slideGamma, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 1 );

	cbZoom = new wxCheckBox( this, wxID_CB_ZOOM, wxT("Zoom 2x"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( cbZoom, 0, wxALL, 5 );


	bSizer1->Add( bSizer4, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Neighboring sprites:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer3->Add( m_staticText2, 0, wxLEFT|wxTOP, 5 );

	lboxNeighbor = new wxListBox( this, wxID_LBOX_NEIGHBOR, wxDefaultPosition, wxSize( 120,-1 ), 0, NULL, 0|wxVSCROLL );
	bSizer3->Add( lboxNeighbor, 1, wxBOTTOM, 5 );

	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Side:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer3->Add( m_staticText3, 0, 0, 5 );

	wxString chbSideChoices[] = { wxT("Q1"), wxT("Q2"), wxT("Q3"), wxT("Q4") };
	int chbSideNChoices = sizeof( chbSideChoices ) / sizeof( wxString );
	chbSide = new wxChoice( this, wxID_CH_SIDE, wxDefaultPosition, wxDefaultSize, chbSideNChoices, chbSideChoices, 0 );
	chbSide->SetSelection( 0 );
	bSizer3->Add( chbSide, 0, wxBOTTOM|wxEXPAND, 5 );


	bSizer1->Add( bSizer3, 0, wxEXPAND, 5 );

	sizerFlags = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT(" Tile properties:") ), wxVERTICAL );

	cbUseAsGlyph = new wxCheckBox( sizerFlags->GetStaticBox(), wxID_CB_USE_AS_GLYPH, wxT("Use as class glyph"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerFlags->Add( cbUseAsGlyph, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	cbFaultCont = new wxCheckBox( sizerFlags->GetStaticBox(), wxID_CB_FAULTY, wxT("Faulty context"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerFlags->Add( cbFaultCont, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	m_staticline2 = new wxStaticLine( sizerFlags->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizerFlags->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );

	sizerTerrFlags = new wxBoxSizer( wxVERTICAL );

	m_checkBox49 = new wxCheckBox( sizerFlags->GetStaticBox(), wxID_ANY, wxT("Check Me!"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerTerrFlags->Add( m_checkBox49, 0, wxALL, 5 );


	sizerFlags->Add( sizerTerrFlags, 1, wxEXPAND, 0 );


	bSizer1->Add( sizerFlags, 0, wxEXPAND|wxLEFT|wxTOP, 5 );

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

	cbToolGlyph = new wxCheckBox( sbSizer6->GetStaticBox(), wxID_CB_TOOL_GLYPH, wxT("Use as tool glyph"), wxDefaultPosition, wxDefaultSize, 0 );
	cbToolGlyph->SetValue(true);
	sbSizer6->Add( cbToolGlyph, 0, wxALL, 5 );

	treeCtrlObjects = new wxTreeCtrl( sbSizer6->GetStaticBox(), wxID_TREE_OBJECTS, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_EDIT_LABELS|wxTR_HIDE_ROOT|wxTR_SINGLE|wxALWAYS_SHOW_SB|wxVSCROLL );
	sbSizer6->Add( treeCtrlObjects, 1, wxALL|wxEXPAND, 5 );


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

	treeCtrlClasses = new wxTreeCtrl( this, wxID_TRC_CLASSES, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_EDIT_LABELS|wxTR_HIDE_ROOT|wxTR_SINGLE|wxALWAYS_SHOW_SB|wxVSCROLL );
	szrList->Add( treeCtrlClasses, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );


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

	txtGamma = new wxStaticText( this, wxID_TXT_GAMMA, wxT("Gamma correction:"), wxDefaultPosition, wxDefaultSize, 0 );
	txtGamma->Wrap( -1 );
	szrView->Add( txtGamma, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	slideGamma = new wxSlider( this, wxID_SLIDE_GAMMA, 1300, 500, 2000, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	szrView->Add( slideGamma, 0, wxEXPAND|wxALL, 1 );


	szrMain->Add( szrView, 1, wxEXPAND, 5 );


	this->SetSizer( szrMain );
	this->Layout();
	m_menubar2 = new wxMenuBar( 0 );
	mnuFile = new wxMenu();
	wxMenuItem* btnSaveList;
	btnSaveList = new wxMenuItem( mnuFile, wxID_MM_SAVE_OBJECTS, wxString( wxT("Save objects") ) , wxEmptyString, wxITEM_NORMAL );
	mnuFile->Append( btnSaveList );

	mnuFile->AppendSeparator();

	wxMenuItem* btnClose;
	btnClose = new wxMenuItem( mnuFile, wxID_MM_CLOSE, wxString( wxT("Close") ) + wxT('\t') + wxT("Escape"), wxEmptyString, wxITEM_NORMAL );
	mnuFile->Append( btnClose );

	m_menubar2->Append( mnuFile, wxT("File") );

	mnuTerr = new wxMenu();
	m_menubar2->Append( mnuTerr, wxT("Terrain") );

	mnuEdit = new wxMenu();
	wxMenuItem* mmRemove;
	mmRemove = new wxMenuItem( mnuEdit, wxID_MM_REMOVE, wxString( wxT("Delete") ) + wxT('\t') + wxT("Delete"), wxEmptyString, wxITEM_NORMAL );
	mnuEdit->Append( mmRemove );

	wxMenuItem* mmRename;
	mmRename = new wxMenuItem( mnuEdit, wxID_MM_RENAME, wxString( wxT("Rename") ) + wxT('\t') + wxT("Ctrl+R"), wxEmptyString, wxITEM_NORMAL );
	mnuEdit->Append( mmRename );

	wxMenuItem* mmNewClass;
	mmNewClass = new wxMenuItem( mnuEdit, wxID_MM_NEW_CLASS, wxString( wxT("New toolset") ) + wxT('\t') + wxT("Ctrl+N"), wxEmptyString, wxITEM_NORMAL );
	mnuEdit->Append( mmNewClass );

	wxMenuItem* mmNewTool;
	mmNewTool = new wxMenuItem( mnuEdit, wxID_MM_NEW_TOOL, wxString( wxT("New tool") ) + wxT('\t') + wxT("Ctrl+T"), wxEmptyString, wxITEM_NORMAL );
	mnuEdit->Append( mmNewTool );

	wxMenuItem* mmEditToolset;
	mmEditToolset = new wxMenuItem( mnuEdit, wxID_MM_EDIT_TOOLSET, wxString( wxT("Edit toolset") ) + wxT('\t') + wxT("Ctrl+E"), wxEmptyString, wxITEM_NORMAL );
	mnuEdit->Append( mmEditToolset );

	m_menubar2->Append( mnuEdit, wxT("Edit") );

	this->SetMenuBar( m_menubar2 );


	this->Centre( wxBOTH );
}

FormObjects::~FormObjects()
{
}

FormNewObject::FormNewObject( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 400,-1 ), wxDefaultSize );

	wxBoxSizer* szrNewObj;
	szrNewObj = new wxBoxSizer( wxVERTICAL );

	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("Object description:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	szrNewObj->Add( m_staticText11, 0, wxLEFT|wxTOP, 5 );

	txtDescription = new wxTextCtrl( this, wxID_TXT_DESC, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	szrNewObj->Add( txtDescription, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

	m_staticText92 = new wxStaticText( this, wxID_ANY, wxT("Objects class:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText92->Wrap( -1 );
	szrNewObj->Add( m_staticText92, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	wxArrayString chbClassChoices;
	chbClass = new wxChoice( this, wxID_CHB_CLASS, wxDefaultPosition, wxDefaultSize, chbClassChoices, 0 );
	chbClass->SetSelection( 0 );
	szrNewObj->Add( chbClass, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	m_staticline4 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	szrNewObj->Add( m_staticline4, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer92;
	bSizer92 = new wxBoxSizer( wxHORIZONTAL );

	btnOk = new wxButton( this, wxID_BTN_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer92->Add( btnOk, 1, wxALL|wxEXPAND, 5 );

	btnCancel = new wxButton( this, wxID_BTN_CANCEL, wxT("CANCEL"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer92->Add( btnCancel, 1, wxALL|wxEXPAND, 5 );


	szrNewObj->Add( bSizer92, 1, wxEXPAND, 5 );


	this->SetSizer( szrNewObj );
	this->Layout();

	this->Centre( wxBOTH );
}

FormNewObject::~FormNewObject()
{
}

FormEditToolset::FormEditToolset( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 400,-1 ), wxDefaultSize );

	wxBoxSizer* szrNewObj;
	szrNewObj = new wxBoxSizer( wxVERTICAL );

	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("Toolset name (keep short):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	szrNewObj->Add( m_staticText11, 0, wxLEFT|wxTOP, 5 );

	txtName = new wxTextCtrl( this, wxID_TXT_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	szrNewObj->Add( txtName, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );

	m_staticText111 = new wxStaticText( this, wxID_ANY, wxT("Toolset description:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText111->Wrap( -1 );
	szrNewObj->Add( m_staticText111, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	txtDescription = new wxTextCtrl( this, wxID_TXT_DESC, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	szrNewObj->Add( txtDescription, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );

	wxBoxSizer* szrScaling;
	szrScaling = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* szrScale;
	szrScale = new wxBoxSizer( wxVERTICAL );

	m_staticText26 = new wxStaticText( this, wxID_ANY, wxT("Toolset glyph scaling mode:"), wxDefaultPosition, wxDefaultSize, 0 );
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


	szrNewObj->Add( szrScaling, 1, wxEXPAND, 5 );

	m_staticline4 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	szrNewObj->Add( m_staticline4, 0, wxEXPAND | wxALL, 5 );

	btnOk = new wxButton( this, wxID_BTN_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	szrNewObj->Add( btnOk, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( szrNewObj );
	this->Layout();

	this->Centre( wxBOTH );
}

FormEditToolset::~FormEditToolset()
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
	this->SetSizeHints( wxSize( 950,400 ), wxDefaultSize );
	this->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ) );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );

	mMenu = new wxMenuBar( 0 );
	mmFile = new wxMenu();
	wxMenuItem* mmSavePalInfo;
	mmSavePalInfo = new wxMenuItem( mmFile, wxID_MM_SAVE_PALINFO, wxString( wxT("Export palette info") ) , wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmSavePalInfo );

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

	wxBoxSizer* bSizer112;
	bSizer112 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer116;
	bSizer116 = new wxBoxSizer( wxVERTICAL );

	m_staticText113 = new wxStaticText( this, wxID_ANY, wxT("Filter (wildcard: *?):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText113->Wrap( -1 );
	bSizer116->Add( m_staticText113, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	txtNameFilter = new wxTextCtrl( this, wxID_TXT_FILTER, wxT("*"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer116->Add( txtNameFilter, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );

	m_staticText112 = new wxStaticText( this, wxID_ANY, wxT("Palettes list:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText112->Wrap( -1 );
	bSizer116->Add( m_staticText112, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	listPal = new wxListBox( this, wxID_LIST_PAL, wxDefaultPosition, wxSize( 180,-1 ), 0, NULL, 0 );
	bSizer116->Add( listPal, 1, wxBOTTOM|wxRIGHT|wxLEFT, 5 );


	bSizer112->Add( bSizer116, 0, wxEXPAND, 5 );

	m_staticline41 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer112->Add( m_staticline41, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );

	wxBoxSizer* bSizer114;
	bSizer114 = new wxBoxSizer( wxVERTICAL );

	m_staticText28 = new wxStaticText( this, wxID_ANY, wxT("Palette:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText28->Wrap( -1 );
	bSizer114->Add( m_staticText28, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	canvas = new wxPanel( this, wxID_CANVAS, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer114->Add( canvas, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	m_staticText29 = new wxStaticText( this, wxID_ANY, wxT("Selected color:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText29->Wrap( -1 );
	bSizer114->Add( m_staticText29, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	color = new wxPanel( this, wxID_COLOR, wxDefaultPosition, wxSize( -1,20 ), wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	color->SetMaxSize( wxSize( -1,20 ) );

	bSizer114->Add( color, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

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


	bSizer114->Add( szFilter, 0, wxEXPAND, 5 );


	bSizer112->Add( bSizer114, 1, wxEXPAND, 5 );


	szCanvas->Add( bSizer112, 1, wxEXPAND, 5 );


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
	mmClose = new wxMenuItem( mmFile, wxID_MM_CLOSE, wxString( wxT("Close") ) + wxT('\t') + wxT("ESC"), wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmClose );

	wxMenuItem* mmExport;
	mmExport = new wxMenuItem( mmFile, wxID_MM_EXPORT, wxString( wxT("Export") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmExport );

	wxMenuItem* mmExportAll;
	mmExportAll = new wxMenuItem( mmFile, wxID_MM_EXPORT_ALL, wxString( wxT("Export all") ) , wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmExportAll );

	m_menubar5->Append( mmFile, wxT("File") );

	this->SetMenuBar( m_menubar5 );

	sbar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_SB );
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer( wxVERTICAL );

	m_staticText104 = new wxStaticText( this, wxID_ANY, wxT("Source:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText104->Wrap( -1 );
	bSizer27->Add( m_staticText104, 0, wxLEFT|wxTOP, 5 );

	wxArrayString chSourceChoices;
	chSource = new wxChoice( this, wxID_CH_SOURCE, wxDefaultPosition, wxDefaultSize, chSourceChoices, 0 );
	chSource->SetSelection( 0 );
	bSizer27->Add( chSource, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	m_staticText97 = new wxStaticText( this, wxID_ANY, wxT("Filter (wildcard: *?):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText97->Wrap( -1 );
	bSizer27->Add( m_staticText97, 0, wxLEFT|wxTOP, 5 );

	txtFilter = new wxTextCtrl( this, wxID_TXT_FILTER, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer27->Add( txtFilter, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	m_staticText31 = new wxStaticText( this, wxID_ANY, wxT("Files:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText31->Wrap( -1 );
	bSizer27->Add( m_staticText31, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	lboxFiles = new wxListBox( this, wxID_LB_FILES, wxDefaultPosition, wxSize( 180,-1 ), 0, NULL, 0|wxALWAYS_SHOW_SB|wxVSCROLL );
	bSizer27->Add( lboxFiles, 1, wxLEFT|wxRIGHT, 5 );

	m_staticText32 = new wxStaticText( this, wxID_ANY, wxT("Width:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText32->Wrap( -1 );
	bSizer27->Add( m_staticText32, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	spinWidth = new wxSpinCtrl( this, wxID_SPIN_W, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	bSizer27->Add( spinWidth, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	cbTransparent = new wxCheckBox( this, wxID_CB_TRANSPARENT, wxT("Is transparent?"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer27->Add( cbTransparent, 0, wxALL, 5 );


	bSizer26->Add( bSizer27, 0, wxEXPAND, 5 );

	m_staticline8 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer26->Add( m_staticline8, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );

	wxBoxSizer* bSizer97;
	bSizer97 = new wxBoxSizer( wxVERTICAL );

	canvas = new wxPanel( this, wxID_CANVAS, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer97->Add( canvas, 1, wxALL|wxEXPAND, 5 );

	m_staticline32 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer97->Add( m_staticline32, 0, wxEXPAND | wxALL, 5 );

	m_staticText98 = new wxStaticText( this, wxID_ANY, wxT("Palette:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText98->Wrap( -1 );
	bSizer97->Add( m_staticText98, 0, wxLEFT|wxRIGHT, 5 );

	palette = new wxPanel( this, wxID_PALETTE, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	palette->SetMaxSize( wxSize( -1,50 ) );

	bSizer97->Add( palette, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );


	bSizer26->Add( bSizer97, 1, wxEXPAND, 5 );


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

	lblSpriteRender = new wxStaticText( pageGrp, wxID_LBL_SPRITE_RENDER, wxT("Sprite render:"), wxDefaultPosition, wxDefaultSize, 0 );
	lblSpriteRender->Wrap( -1 );
	bSizer36->Add( lblSpriteRender, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

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
	mmOk = new wxMenuItem( mmFile, wxID_MM_OK, wxString( wxT("Place/Update") ) + wxT('\t') + wxT("Enter"), wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmOk );

	wxMenuItem* mmExit;
	mmExit = new wxMenuItem( mmFile, wxID_MM_EXIT, wxString( wxT("Exit") ) + wxT('\t') + wxT("ESC"), wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmExit );

	mMenu->Append( mmFile, wxT("Events") );

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

	btnCleanup = new wxButton( this, wxID_BTN_CLEANUP, wxT("Cleanup Events"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer531->Add( btnCleanup, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );


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

FormAbout::FormAbout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 600,350 ), wxDefaultSize );

	wxBoxSizer* bSizer73;
	bSizer73 = new wxBoxSizer( wxVERTICAL );

	m_staticText80 = new wxStaticText( this, wxID_ANY, wxT("Spellcross Map Editor"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText80->Wrap( -1 );
	m_staticText80->SetFont( wxFont( 16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );

	bSizer73->Add( m_staticText80, 0, wxALIGN_CENTER|wxALL, 5 );

	m_staticline29 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer73->Add( m_staticline29, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

	wxBoxSizer* bSizer74;
	bSizer74 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer75;
	bSizer75 = new wxBoxSizer( wxVERTICAL );

	m_textCtrl17 = new wxTextCtrl( this, wxID_ANY, wxT("Version:"), wxDefaultPosition, wxDefaultSize, wxTE_RICH|wxBORDER_NONE );
	m_textCtrl17->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	m_textCtrl17->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	bSizer75->Add( m_textCtrl17, 0, wxALL|wxEXPAND, 5 );

	m_textCtrl171 = new wxTextCtrl( this, wxID_ANY, wxT("Author:"), wxDefaultPosition, wxDefaultSize, wxTE_RICH|wxBORDER_NONE );
	m_textCtrl171->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	m_textCtrl171->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	bSizer75->Add( m_textCtrl171, 0, wxALL, 5 );

	m_textCtrl172 = new wxTextCtrl( this, wxID_ANY, wxT("Email:"), wxDefaultPosition, wxDefaultSize, wxTE_RICH|wxBORDER_NONE );
	m_textCtrl172->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	m_textCtrl172->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	bSizer75->Add( m_textCtrl172, 0, wxALL, 5 );

	m_textCtrl173 = new wxTextCtrl( this, wxID_ANY, wxT("URL:"), wxDefaultPosition, wxDefaultSize, wxTE_RICH|wxBORDER_NONE );
	m_textCtrl173->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	m_textCtrl173->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	bSizer75->Add( m_textCtrl173, 0, wxALL, 5 );

	m_textCtrl174 = new wxTextCtrl( this, wxID_ANY, wxT("License:"), wxDefaultPosition, wxDefaultSize, wxTE_RICH|wxBORDER_NONE );
	m_textCtrl174->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	m_textCtrl174->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	bSizer75->Add( m_textCtrl174, 0, wxALL, 5 );


	bSizer74->Add( bSizer75, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer76;
	bSizer76 = new wxBoxSizer( wxVERTICAL );

	txtVersion = new wxTextCtrl( this, wxID_TXT_VER, wxT("V1.00, 2025-11-16"), wxDefaultPosition, wxDefaultSize, wxTE_RICH|wxBORDER_NONE );
	txtVersion->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	txtVersion->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	bSizer76->Add( txtVersion, 0, wxBOTTOM|wxEXPAND|wxTOP, 5 );

	txtName = new wxTextCtrl( this, wxID_TXT_NAME, wxT("Stanislav Mašláň"), wxDefaultPosition, wxDefaultSize, wxTE_RICH|wxBORDER_NONE );
	txtName->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	txtName->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	bSizer76->Add( txtName, 0, wxBOTTOM|wxEXPAND|wxRIGHT|wxTOP, 5 );

	txtEmail = new wxTextCtrl( this, wxID_TXT_EMAIL, wxT("s.maslan@seznam.cz"), wxDefaultPosition, wxDefaultSize, wxTE_RICH|wxBORDER_NONE );
	txtEmail->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	txtEmail->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	bSizer76->Add( txtEmail, 0, wxBOTTOM|wxEXPAND|wxTOP, 5 );

	txtURL = new wxTextCtrl( this, wxID_TXT_URL, wxT("https://github.com/smaslan/spellcross-map-edit"), wxDefaultPosition, wxDefaultSize, wxTE_AUTO_URL|wxTE_RICH|wxBORDER_NONE );
	txtURL->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	txtURL->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	bSizer76->Add( txtURL, 0, wxBOTTOM|wxEXPAND|wxTOP, 5 );

	txtLicense = new wxTextCtrl( this, wxID_TXT_LICENSE, wxT("MIT License, (c) 2025, Stanislav Mašláň"), wxDefaultPosition, wxDefaultSize, wxTE_RICH|wxBORDER_NONE );
	txtLicense->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	txtLicense->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	bSizer76->Add( txtLicense, 0, wxBOTTOM|wxEXPAND|wxTOP, 5 );


	bSizer74->Add( bSizer76, 1, wxEXPAND, 5 );


	bSizer73->Add( bSizer74, 0, wxEXPAND, 5 );

	m_staticline30 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer73->Add( m_staticline30, 0, wxEXPAND | wxALL, 5 );

	txtDesc = new wxTextCtrl( this, wxID_TXT_DESC, wxT("Very experimental editor for Spellcross map files and collection of Spellcross data loaders and viewers."), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH|wxTE_WORDWRAP|wxBORDER_NONE );
	txtDesc->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	bSizer73->Add( txtDesc, 1, wxALL|wxEXPAND, 5 );

	m_staticline31 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer73->Add( m_staticline31, 0, wxEXPAND | wxALL, 5 );

	btnOK = new wxButton( this, wxID_BTN_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer73->Add( btnOK, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer73 );
	this->Layout();

	this->Centre( wxBOTH );
}

FormAbout::~FormAbout()
{
}

FormANM::FormANM( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 700,500 ), wxDefaultSize );
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

	mmTerrain = new wxMenu();
	m_menubar10->Append( mmTerrain, wxT("Terrain") );

	this->SetMenuBar( m_menubar10 );

	wxBoxSizer* bSizer77;
	bSizer77 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer78;
	bSizer78 = new wxBoxSizer( wxVERTICAL );

	m_staticText117 = new wxStaticText( this, wxID_ANY, wxT("Source:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText117->Wrap( -1 );
	bSizer78->Add( m_staticText117, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	wxArrayString chSourceChoices;
	chSource = new wxChoice( this, wxID_CH_SRC, wxDefaultPosition, wxDefaultSize, chSourceChoices, 0 );
	chSource->SetSelection( 0 );
	bSizer78->Add( chSource, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );

	m_staticText118 = new wxStaticText( this, wxID_ANY, wxT("Wildcard filter (*?):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText118->Wrap( -1 );
	bSizer78->Add( m_staticText118, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	txtFilter = new wxTextCtrl( this, wxID_TXT_FILTER, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer78->Add( txtFilter, 0, wxRIGHT|wxLEFT|wxEXPAND, 5 );

	txtResources = new wxStaticText( this, wxID_TXT_RESOURCES, wxT("ANM resources:"), wxDefaultPosition, wxDefaultSize, 0 );
	txtResources->Wrap( -1 );
	bSizer78->Add( txtResources, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	lbList = new wxListBox( this, wxID_LB_LIST, wxDefaultPosition, wxSize( 150,-1 ), 0, NULL, wxLB_ALWAYS_SB );
	bSizer78->Add( lbList, 1, wxBOTTOM|wxLEFT|wxRIGHT, 5 );


	bSizer77->Add( bSizer78, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer79;
	bSizer79 = new wxBoxSizer( wxVERTICAL );

	txtFrames = new wxStaticText( this, wxID_TXT_FRAMES, wxT("Frames:"), wxDefaultPosition, wxDefaultSize, 0 );
	txtFrames->Wrap( -1 );
	bSizer79->Add( txtFrames, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	lbFrames = new wxListBox( this, wxID_LB_FRAMES, wxDefaultPosition, wxSize( 150,-1 ), 0, NULL, wxLB_ALWAYS_SB );
	bSizer79->Add( lbFrames, 1, wxBOTTOM|wxLEFT|wxRIGHT, 5 );

	cbAnimate = new wxCheckBox( this, wxID_CB_ANIM, wxT("Animate"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer79->Add( cbAnimate, 0, wxALL, 5 );


	bSizer77->Add( bSizer79, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer80;
	bSizer80 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );


	bSizer5->Add( 0, 0, 0, wxALL|wxEXPAND, 4 );

	wxStaticBoxSizer* sizerCanvas;
	sizerCanvas = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT(" Frame view: ") ), wxVERTICAL );

	canvas = new wxPanel( sizerCanvas->GetStaticBox(), wxID_CANVAS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sizerCanvas->Add( canvas, 1, wxALL|wxEXPAND, 5 );


	bSizer5->Add( sizerCanvas, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

	wxBoxSizer* sizeOffsets;
	sizeOffsets = new wxBoxSizer( wxHORIZONTAL );

	m_staticText88 = new wxStaticText( this, wxID_ANY, wxT("X offset:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText88->Wrap( -1 );
	sizeOffsets->Add( m_staticText88, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	spinXofs = new wxSpinCtrl( this, wxID_SPIN_XOFS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	sizeOffsets->Add( spinXofs, 1, wxALL, 5 );

	m_staticText89 = new wxStaticText( this, wxID_ANY, wxT("Y offset:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText89->Wrap( -1 );
	sizeOffsets->Add( m_staticText89, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	spinYofs = new wxSpinCtrl( this, wxID_SPIN_YOFS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	sizeOffsets->Add( spinYofs, 1, wxALL, 5 );


	bSizer5->Add( sizeOffsets, 0, wxEXPAND, 5 );

	txtGamma = new wxStaticText( this, wxID_TXT_GAMMA, wxT("Set gamma correction:"), wxDefaultPosition, wxDefaultSize, 0 );
	txtGamma->Wrap( -1 );
	bSizer5->Add( txtGamma, 0, wxLEFT|wxTOP, 5 );

	slideGamma = new wxSlider( this, wxID_SLIDE_GAMMA, 1300, 500, 2000, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	bSizer5->Add( slideGamma, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 1 );

	cbZoom = new wxCheckBox( this, wxID_CB_ZOOM, wxT("Zoom 2x"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( cbZoom, 0, wxALL, 5 );


	bSizer80->Add( bSizer5, 1, wxEXPAND, 5 );


	bSizer77->Add( bSizer80, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer77 );
	this->Layout();

	this->Centre( wxBOTH );
}

FormANM::~FormANM()
{
}

FormSound::FormSound( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
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
}

FormSound::~FormSound()
{
}

FormFlags::FormFlags( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 400,350 ), wxDefaultSize );

	wxBoxSizer* bSizer87;
	bSizer87 = new wxBoxSizer( wxVERTICAL );

	m_staticText90 = new wxStaticText( this, wxID_ANY, wxT("Tile flag value:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText90->Wrap( -1 );
	bSizer87->Add( m_staticText90, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	wxArrayString chbFlagChoices;
	chbFlag = new wxChoice( this, wxID_CHB_FLAG, wxDefaultPosition, wxDefaultSize, chbFlagChoices, 0 );
	chbFlag->SetSelection( 0 );
	bSizer87->Add( chbFlag, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

	wxStaticBoxSizer* sizerCanvas;
	sizerCanvas = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT(" Sprite preview: ") ), wxVERTICAL );

	canvas = new wxPanel( sizerCanvas->GetStaticBox(), wxID_CANVAS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sizerCanvas->Add( canvas, 1, wxALL|wxEXPAND, 5 );


	bSizer87->Add( sizerCanvas, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer88;
	bSizer88 = new wxBoxSizer( wxHORIZONTAL );

	btnOk = new wxButton( this, wxID_BTN_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer88->Add( btnOk, 1, wxALL, 5 );

	btnCancel = new wxButton( this, wxID_BTN_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer88->Add( btnCancel, 1, wxALL, 5 );


	bSizer87->Add( bSizer88, 0, wxEXPAND, 5 );


	this->SetSizer( bSizer87 );
	this->Layout();

	this->Centre( wxBOTH );
}

FormFlags::~FormFlags()
{
}

FormNewMap::FormNewMap( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
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
}

FormNewMap::~FormNewMap()
{
}

FormGResEncoder::FormGResEncoder( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );

	m_menubar12 = new wxMenuBar( 0 );
	mmFile = new wxMenu();
	wxMenuItem* mmOpen;
	mmOpen = new wxMenuItem( mmFile, wxID_MM_OPEN, wxString( wxT("Open resource") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmOpen );

	wxMenuItem* mmSave;
	mmSave = new wxMenuItem( mmFile, wxID_MM_SAVE, wxString( wxT("Save resource") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmSave );

	wxMenuItem* mmExportAll;
	mmExportAll = new wxMenuItem( mmFile, wxID_MM_SAVE_ALL, wxString( wxT("Save All Resource") ) + wxT('\t') + wxT("Ctrl+Shift+S"), wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmExportAll );

	wxMenuItem* mmSavePal;
	mmSavePal = new wxMenuItem( mmFile, wxID_MM_SAVE_PAL, wxString( wxT("Save palette") ) + wxT('\t') + wxT("Ctrl+P"), wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmSavePal );

	mmFile->AppendSeparator();

	wxMenuItem* mmExit;
	mmExit = new wxMenuItem( mmFile, wxID_MM_EXIT, wxString( wxT("Exit") ) + wxT('\t') + wxT("ESC"), wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmExit );

	m_menubar12->Append( mmFile, wxT("File") );

	this->SetMenuBar( m_menubar12 );

	sbar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_SBAR );
	wxBoxSizer* bSizer98;
	bSizer98 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer101;
	bSizer101 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer104;
	bSizer104 = new wxBoxSizer( wxVERTICAL );

	m_staticText103 = new wxStaticText( this, wxID_ANY, wxT("Others sharing palette:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText103->Wrap( -1 );
	bSizer104->Add( m_staticText103, 0, wxLEFT|wxTOP, 5 );

	lboxList = new wxListBox( this, wxID_LB_LIST, wxDefaultPosition, wxDefaultSize, 0, NULL, 0|wxVSCROLL );
	lboxList->SetMinSize( wxSize( 150,-1 ) );

	bSizer104->Add( lboxList, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );


	bSizer101->Add( bSizer104, 0, wxEXPAND, 5 );

	m_staticline37 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer101->Add( m_staticline37, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer99;
	bSizer99 = new wxBoxSizer( wxVERTICAL );

	m_staticText99 = new wxStaticText( this, wxID_ANY, wxT("Source:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText99->Wrap( -1 );
	bSizer99->Add( m_staticText99, 0, wxLEFT|wxTOP, 5 );

	canvasSrc = new wxPanel( this, wxID_CANVAS_SRC, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer99->Add( canvasSrc, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );


	bSizer101->Add( bSizer99, 1, wxEXPAND, 5 );

	m_staticline33 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer101->Add( m_staticline33, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer100;
	bSizer100 = new wxBoxSizer( wxVERTICAL );

	m_staticText100 = new wxStaticText( this, wxID_ANY, wxT("Output:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText100->Wrap( -1 );
	bSizer100->Add( m_staticText100, 0, wxLEFT|wxTOP, 5 );

	canvasRes = new wxPanel( this, wxID_CANVAS_RES, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer100->Add( canvasRes, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );


	bSizer101->Add( bSizer100, 1, wxEXPAND, 5 );


	bSizer98->Add( bSizer101, 1, wxEXPAND, 5 );

	m_staticline34 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer98->Add( m_staticline34, 0, wxBOTTOM|wxEXPAND, 5 );

	m_staticText102 = new wxStaticText( this, wxID_ANY, wxT("Palette:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText102->Wrap( -1 );
	bSizer98->Add( m_staticText102, 0, wxLEFT, 5 );

	palette = new wxPanel( this, wxID_PALETTE, wxDefaultPosition, wxSize( -1,-1 ), wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	palette->SetMaxSize( wxSize( -1,50 ) );

	bSizer98->Add( palette, 1, wxEXPAND | wxALL, 5 );

	m_staticline36 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer98->Add( m_staticline36, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer102;
	bSizer102 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer103;
	bSizer103 = new wxBoxSizer( wxVERTICAL );

	m_staticText101 = new wxStaticText( this, wxID_ANY, wxT("Min color distance for dithering:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText101->Wrap( -1 );
	bSizer103->Add( m_staticText101, 0, wxLEFT, 5 );

	slideMinDither = new wxSlider( this, wxID_SLIDE_MIN_DITHER, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_LABELS );
	slideMinDither->SetMinSize( wxSize( 200,-1 ) );

	bSizer103->Add( slideMinDither, 1, wxALL|wxEXPAND, 5 );


	bSizer102->Add( bSizer103, 0, wxEXPAND, 5 );

	m_staticline35 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer102->Add( m_staticline35, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	wxBoxSizer* bSizer114;
	bSizer114 = new wxBoxSizer( wxVERTICAL );

	m_staticText115 = new wxStaticText( this, wxID_ANY, wxT("Extra x-offset:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText115->Wrap( -1 );
	bSizer114->Add( m_staticText115, 0, wxRIGHT|wxLEFT, 5 );

	spinExtraXoffset = new wxSpinCtrl( this, wxID_SPIN_EX_OFS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -200, 200, 0 );
	spinExtraXoffset->SetMinSize( wxSize( 100,-1 ) );

	bSizer114->Add( spinExtraXoffset, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	m_staticText116 = new wxStaticText( this, wxID_ANY, wxT("Extra y-offset:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText116->Wrap( -1 );
	bSizer114->Add( m_staticText116, 0, wxRIGHT|wxLEFT, 5 );

	spinExtraYoffset = new wxSpinCtrl( this, wxID_SPIN_EY_OFS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -200, 200, 0 );
	spinExtraYoffset->SetMinSize( wxSize( 100,-1 ) );

	bSizer114->Add( spinExtraYoffset, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );


	bSizer102->Add( bSizer114, 0, wxEXPAND, 5 );

	m_staticline42 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer102->Add( m_staticline42, 0, wxEXPAND | wxALL, 5 );


	bSizer102->Add( 0, 0, 1, wxEXPAND, 5 );

	btnRegen = new wxButton( this, wxID_BTN_REGEN, wxT("Regenerate"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer102->Add( btnRegen, 0, wxALL|wxEXPAND, 5 );

	btnRegenPalette = new wxButton( this, wxID_BTN_REGEN_PAL, wxT("Regenerate\nPalette"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer102->Add( btnRegenPalette, 0, wxALL|wxEXPAND, 5 );


	bSizer98->Add( bSizer102, 0, wxEXPAND, 5 );


	this->SetSizer( bSizer98 );
	this->Layout();

	this->Centre( wxBOTH );
}

FormGResEncoder::~FormGResEncoder()
{
}

FormText::FormText( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	m_menubar13 = new wxMenuBar( 0 );
	m_menu24 = new wxMenu();
	wxMenuItem* mmExit;
	mmExit = new wxMenuItem( m_menu24, wxID_MM_EXIT, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu24->Append( mmExit );

	wxMenuItem* mmSave;
	mmSave = new wxMenuItem( m_menu24, wxID_MM_SAVE, wxString( wxT("Export Resource") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	m_menu24->Append( mmSave );

	wxMenuItem* mmSaveAll;
	mmSaveAll = new wxMenuItem( m_menu24, wxID_MM_SAVE_ALL, wxString( wxT("Export All") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu24->Append( mmSaveAll );

	m_menubar13->Append( m_menu24, wxT("File") );

	m_menu25 = new wxMenu();
	wxMenuItem* mmRestore;
	mmRestore = new wxMenuItem( m_menu25, wxID_MM_RESTORE, wxString( wxT("Revert chagnes") ) + wxT('\t') + wxT("Ctrl+R"), wxEmptyString, wxITEM_NORMAL );
	m_menu25->Append( mmRestore );

	wxMenuItem* mmApplyChanges;
	mmApplyChanges = new wxMenuItem( m_menu25, wxID_MM_APPLY, wxString( wxT("Save changes") ) + wxT('\t') + wxT("Ctrl+Enter"), wxEmptyString, wxITEM_NORMAL );
	m_menu25->Append( mmApplyChanges );

	m_menubar13->Append( m_menu25, wxT("Edit") );

	this->SetMenuBar( m_menubar13 );

	sbar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_SBAR );
	wxBoxSizer* bSizer105;
	bSizer105 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer106;
	bSizer106 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer107;
	bSizer107 = new wxBoxSizer( wxVERTICAL );

	m_staticText106 = new wxStaticText( this, wxID_ANY, wxT("Source:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText106->Wrap( -1 );
	bSizer107->Add( m_staticText106, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	wxArrayString chSourceChoices;
	chSource = new wxChoice( this, wxID_CH_SOURCE, wxDefaultPosition, wxDefaultSize, chSourceChoices, 0 );
	chSource->SetSelection( 0 );
	bSizer107->Add( chSource, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5 );

	m_staticText107 = new wxStaticText( this, wxID_ANY, wxT("Filter (wildcard: ?*):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText107->Wrap( -1 );
	bSizer107->Add( m_staticText107, 0, wxRIGHT|wxLEFT, 5 );

	txtFilter = new wxTextCtrl( this, wxID_TXT_WILD, wxT("*"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer107->Add( txtFilter, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5 );

	m_staticText108 = new wxStaticText( this, wxID_ANY, wxT("Resources:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText108->Wrap( -1 );
	bSizer107->Add( m_staticText108, 0, wxRIGHT|wxLEFT, 5 );

	listResources = new wxListBox( this, wxID_LIST_RESOURCES, wxDefaultPosition, wxSize( 180,-1 ), 0, NULL, 0 );
	bSizer107->Add( listResources, 1, wxBOTTOM|wxLEFT|wxRIGHT, 5 );


	bSizer106->Add( bSizer107, 0, wxEXPAND, 5 );

	m_staticline39 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer106->Add( m_staticline39, 0, wxEXPAND|wxTOP, 5 );

	wxBoxSizer* bSizer108;
	bSizer108 = new wxBoxSizer( wxVERTICAL );

	m_staticText109 = new wxStaticText( this, wxID_ANY, wxT("Source text:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText109->Wrap( -1 );
	bSizer108->Add( m_staticText109, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	textEdit = new wxTextCtrl( this, wxID_TXT_EDIT, wxEmptyString, wxDefaultPosition, wxSize( 400,-1 ), wxTE_MULTILINE );
	bSizer108->Add( textEdit, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	m_staticText111 = new wxStaticText( this, wxID_ANY, wxT("Target window:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText111->Wrap( -1 );
	bSizer108->Add( m_staticText111, 0, wxRIGHT|wxLEFT, 5 );

	wxArrayString chTargetChoices;
	chTarget = new wxChoice( this, wxID_CH_TARGET, wxDefaultPosition, wxDefaultSize, chTargetChoices, 0 );
	chTarget->SetSelection( 0 );
	bSizer108->Add( chTarget, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );

	m_staticText114 = new wxStaticText( this, wxID_ANY, wxT("Text align mode:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText114->Wrap( -1 );
	bSizer108->Add( m_staticText114, 0, wxRIGHT|wxLEFT, 5 );

	wxArrayString chAlignChoices;
	chAlign = new wxChoice( this, wxID_CH_ALIGN, wxDefaultPosition, wxDefaultSize, chAlignChoices, 0 );
	chAlign->SetSelection( 0 );
	bSizer108->Add( chAlign, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );


	bSizer106->Add( bSizer108, 0, wxEXPAND, 5 );

	m_staticline40 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer106->Add( m_staticline40, 0, wxEXPAND|wxTOP, 5 );

	wxBoxSizer* bSizer109;
	bSizer109 = new wxBoxSizer( wxVERTICAL );

	m_staticText110 = new wxStaticText( this, wxID_ANY, wxT("Render:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText110->Wrap( -1 );
	bSizer109->Add( m_staticText110, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	wxBoxSizer* bSizer110;
	bSizer110 = new wxBoxSizer( wxHORIZONTAL );

	canvasText = new wxPanel( this, wxID_CANVAS_TEXT, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer110->Add( canvasText, 1, wxEXPAND | wxALL, 5 );

	scrollText = new wxScrollBar( this, wxID_SCROLL_TEXT, wxDefaultPosition, wxDefaultSize, wxSB_VERTICAL );
	bSizer110->Add( scrollText, 0, wxEXPAND|wxALL, 5 );


	bSizer109->Add( bSizer110, 1, wxEXPAND, 5 );


	bSizer106->Add( bSizer109, 1, wxEXPAND, 5 );


	bSizer105->Add( bSizer106, 1, wxEXPAND, 5 );

	m_staticline38 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer105->Add( m_staticline38, 0, wxEXPAND | wxALL, 5 );

	m_staticText105 = new wxStaticText( this, wxID_ANY, wxT("Font preview:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText105->Wrap( -1 );
	bSizer105->Add( m_staticText105, 0, wxLEFT, 5 );

	canvasFont = new wxPanel( this, wxID_CANVAS_FONT, wxDefaultPosition, wxSize( -1,80 ), wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer105->Add( canvasFont, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );


	this->SetSizer( bSizer105 );
	this->Layout();

	this->Centre( wxBOTH );
}

FormText::~FormText()
{
}

FormConfig::FormConfig( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
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
}

FormConfig::~FormConfig()
{
}
