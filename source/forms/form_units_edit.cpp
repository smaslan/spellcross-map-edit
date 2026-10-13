///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_units_edit.h"
#include "spellcross.h"
#include "spell_units.h"
#include "spell_palette.h"
#include "spell_filter.h"
#include "wx_other.h"

#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <wx/dragimag.h>

///////////////////////////////////////////////////////////////////////////

FormUnitsEdit::FormUnitsEdit( wxWindow* parent, wxWindowID id,SpellData *spell_data, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) 
	: wxFrame( parent, id, title, pos, size, style ),
	m_spell_data(spell_data)
{
	// <wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormUnitsEdit' on 2026-10-12 19:24:03
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );
	
	m_statusBar12 = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );
	m_menubar15 = new wxMenuBar( 0 );
	mmFile = new wxMenu();
	wxMenuItem* mmOpen;
	mmOpen = new wxMenuItem( mmFile, wxID_MM_OPEN_DEF, wxString( wxT("Open units DEF file(s)") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmOpen );
	
	wxMenuItem* mmSave;
	mmSave = new wxMenuItem( mmFile, wxID_MM_SAVE_DEF, wxString( wxT("Save target DEF file(s)") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmSave );
	
	wxMenuItem* mmSaveAs;
	mmSaveAs = new wxMenuItem( mmFile, wxID_MM_SAVE_DEF_AS, wxString( wxT("Save target DEF file as") ) , wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmSaveAs );
	
	mmFile->AppendSeparator();
	
	wxMenuItem* mmExit;
	mmExit = new wxMenuItem( mmFile, wxID_MM_EXIT, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmExit );
	
	m_menubar15->Append( mmFile, wxT("File") );
	
	this->SetMenuBar( m_menubar15 );
	
	wxBoxSizer* bSizer129;
	bSizer129 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer131;
	bSizer131 = new wxBoxSizer( wxVERTICAL );
	
	bSizer131->SetMinSize( wxSize( 200,-1 ) );
	m_staticText138 = new wxStaticText( this, wxID_ANY, wxT("Units DEF files:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText138->Wrap( -1 );
	bSizer131->Add( m_staticText138, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	lboxDEF = new wxListBox( this, wxID_LBOX_DEFS, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB );
	bSizer131->Add( lboxDEF, 1, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	m_staticText140 = new wxStaticText( this, wxID_ANY, wxT("Target DEF file(s):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText140->Wrap( -1 );
	bSizer131->Add( m_staticText140, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	lboxTargetDef = new wxListBox( this, wxID_LBOX_TARGET_DEFS, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB|wxLB_MULTIPLE );
	bSizer131->Add( lboxTargetDef, 1, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	
	bSizer129->Add( bSizer131, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer130;
	bSizer130 = new wxBoxSizer( wxVERTICAL );
	
	bSizer130->SetMinSize( wxSize( 200,-1 ) );
	m_staticText137 = new wxStaticText( this, wxID_ANY, wxT("Units list:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText137->Wrap( -1 );
	bSizer130->Add( m_staticText137, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	lboxUnits = new wxListCtrlVirtual(this,wxID_LBOX_UNITS,wxDefaultPosition,wxSize(200,-1),wxLC_NO_HEADER|wxLC_REPORT|wxLC_VIRTUAL|wxALWAYS_SHOW_SB|wxVSCROLL);
	bSizer130->Add( lboxUnits, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer129->Add( bSizer130, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer132;
	bSizer132 = new wxBoxSizer( wxVERTICAL );
	
	pageProps = new wxNotebook( this, wxID_PAGE_PROPS, wxDefaultPosition, wxDefaultSize, 0 );
	pagePropBasic = new wxPanel( pageProps, wxID_PAGE_BASIC, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer133;
	bSizer133 = new wxBoxSizer( wxVERTICAL );
	
	pgPropBasic = new wxPropertyGrid(pagePropBasic, wxID_PG_PROP_BASIC, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer133->Add( pgPropBasic, 1, wxEXPAND|wxALL, 5 );
	
	
	pagePropBasic->SetSizer( bSizer133 );
	pagePropBasic->Layout();
	bSizer133->Fit( pagePropBasic );
	pageProps->AddPage( pagePropBasic, wxT("Basic"), true );
	pageGrp = new wxPanel( pageProps, wxID_PAGE_GRP, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer134;
	bSizer134 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer138;
	bSizer138 = new wxBoxSizer( wxVERTICAL );
	
	bSizer138->SetMinSize( wxSize( 400,-1 ) );
	pgPropGrp = new wxPropertyGrid(pageGrp, wxID_PG_PROP_GRP, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer138->Add( pgPropGrp, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer134->Add( bSizer138, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer139;
	bSizer139 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer152;
	bSizer152 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer153;
	bSizer153 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText141 = new wxStaticText( pageGrp, wxID_ANY, wxT("Unit icon:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText141->Wrap( -1 );
	bSizer153->Add( m_staticText141, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	canvasIcon = new wxPanel( pageGrp, wxID_CANVAS_ICON, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME|wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer153->Add( canvasIcon, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer152->Add( bSizer153, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer1531;
	bSizer1531 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText1411 = new wxStaticText( pageGrp, wxID_ANY, wxT("Unit info render:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1411->Wrap( -1 );
	bSizer1531->Add( m_staticText1411, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	canvasInfo = new wxPanel( pageGrp, wxID_CANVAS_INFO, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME|wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer1531->Add( canvasInfo, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer152->Add( bSizer1531, 1, wxEXPAND, 5 );
	
	
	bSizer139->Add( bSizer152, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer140;
	bSizer140 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer141;
	bSizer141 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText143 = new wxStaticText( pageGrp, wxID_ANY, wxT("Unit graphics:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText143->Wrap( -1 );
	bSizer141->Add( m_staticText143, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	canvasGrp = new wxPanel( pageGrp, wxID_CANVAS_GRP, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME|wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer141->Add( canvasGrp, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer140->Add( bSizer141, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer142;
	bSizer142 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText1431 = new wxStaticText( pageGrp, wxID_ANY, wxT("Aux graphics:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1431->Wrap( -1 );
	bSizer142->Add( m_staticText1431, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	canvasGrpAux = new wxPanel( pageGrp, wxID_CANVAS_GRP_AUX, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME|wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer142->Add( canvasGrpAux, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer140->Add( bSizer142, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer175;
	bSizer175 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText142 = new wxStaticText( pageGrp, wxID_ANY, wxT("Projectile:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText142->Wrap( -1 );
	bSizer175->Add( m_staticText142, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	canvasProjectile = new wxPanel( pageGrp, wxID_CANVAS_PROJECTILE, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME|wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer175->Add( canvasProjectile, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer140->Add( bSizer175, 1, wxEXPAND, 5 );
	
	
	bSizer139->Add( bSizer140, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer143;
	bSizer143 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer144;
	bSizer144 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText147 = new wxStaticText( pageGrp, wxID_ANY, wxT("Light fire anim.:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText147->Wrap( -1 );
	bSizer144->Add( m_staticText147, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	canvasLightFireAnim = new wxPanel( pageGrp, wxID_CANVAS_LIGHT_FIRE_ANIM, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME|wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer144->Add( canvasLightFireAnim, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer143->Add( bSizer144, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer1441;
	bSizer1441 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText1471 = new wxStaticText( pageGrp, wxID_ANY, wxT("Light fire PNM:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1471->Wrap( -1 );
	bSizer1441->Add( m_staticText1471, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	canvasLightFirePNM = new wxPanel( pageGrp, wxID_CANVAS_LIGHT_FIRE_PNM, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME|wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer1441->Add( canvasLightFirePNM, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer143->Add( bSizer1441, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer14411;
	bSizer14411 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText14711 = new wxStaticText( pageGrp, wxID_ANY, wxT("Light hit PNM:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14711->Wrap( -1 );
	bSizer14411->Add( m_staticText14711, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	canvasLightHitPNM = new wxPanel( pageGrp, wxID_CANVAS_LIGHT_HIT_PNM, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME|wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer14411->Add( canvasLightHitPNM, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer143->Add( bSizer14411, 1, wxEXPAND, 5 );
	
	
	bSizer139->Add( bSizer143, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer1431;
	bSizer1431 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer1442;
	bSizer1442 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText1472 = new wxStaticText( pageGrp, wxID_ANY, wxT("Armored fire anim.:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1472->Wrap( -1 );
	bSizer1442->Add( m_staticText1472, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	canvasArmoredFireAnim = new wxPanel( pageGrp, wxID_CANVAS_ARMORED_FIRE_ANIM, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME|wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer1442->Add( canvasArmoredFireAnim, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer1431->Add( bSizer1442, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer14412;
	bSizer14412 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText14712 = new wxStaticText( pageGrp, wxID_ANY, wxT("Armored fire PNM:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14712->Wrap( -1 );
	bSizer14412->Add( m_staticText14712, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	canvasArmoredFirePNM = new wxPanel( pageGrp, wxID_CANVAS_ARMORED_FIRE_PNM, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME|wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer14412->Add( canvasArmoredFirePNM, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer1431->Add( bSizer14412, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer144111;
	bSizer144111 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText147111 = new wxStaticText( pageGrp, wxID_ANY, wxT("Armored hit PNM:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText147111->Wrap( -1 );
	bSizer144111->Add( m_staticText147111, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	canvasArmoredHitPNM = new wxPanel( pageGrp, wxID_CANVAS_ARMORED_HIT_PNM, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME|wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer144111->Add( canvasArmoredHitPNM, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer1431->Add( bSizer144111, 1, wxEXPAND, 5 );
	
	
	bSizer139->Add( bSizer1431, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer14311;
	bSizer14311 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer14421;
	bSizer14421 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText14721 = new wxStaticText( pageGrp, wxID_ANY, wxT("Air fire anim.:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14721->Wrap( -1 );
	bSizer14421->Add( m_staticText14721, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	canvasAirFireAnim = new wxPanel( pageGrp, wxID_CANVAS_AIR_FIRE_ANIM, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME|wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer14421->Add( canvasAirFireAnim, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer14311->Add( bSizer14421, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer144121;
	bSizer144121 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText147121 = new wxStaticText( pageGrp, wxID_ANY, wxT("Air fire PNM:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText147121->Wrap( -1 );
	bSizer144121->Add( m_staticText147121, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	canvasAirFirePNM = new wxPanel( pageGrp, wxID_CANVAS_AIR_FIRE_PNM, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME|wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer144121->Add( canvasAirFirePNM, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer14311->Add( bSizer144121, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer1441111;
	bSizer1441111 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText1471111 = new wxStaticText( pageGrp, wxID_ANY, wxT("Air hit PNM:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1471111->Wrap( -1 );
	bSizer1441111->Add( m_staticText1471111, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	canvasAirHitPNM = new wxPanel( pageGrp, wxID_CANVAS_AIR_HIT_PNM, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME|wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer1441111->Add( canvasAirHitPNM, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer14311->Add( bSizer1441111, 1, wxEXPAND, 5 );
	
	
	bSizer139->Add( bSizer14311, 1, wxEXPAND, 5 );
	
	
	bSizer134->Add( bSizer139, 1, wxEXPAND, 5 );
	
	
	pageGrp->SetSizer( bSizer134 );
	pageGrp->Layout();
	bSizer134->Fit( pageGrp );
	pageProps->AddPage( pageGrp, wxT("Graphics"), false );
	pageSound = new wxPanel( pageProps, wxID_PAGE_SOUND, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer135;
	bSizer135 = new wxBoxSizer( wxVERTICAL );
	
	pgPropSound = new wxPropertyGrid(pageSound, wxID_PG_PROP_SOUND, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer135->Add( pgPropSound, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText170 = new wxStaticText( pageSound, wxID_ANY, wxT("Sound resource (double-click to play):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText170->Wrap( -1 );
	bSizer135->Add( m_staticText170, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	pgSndRes = new wxPropertyGrid(pageSound, wxID_PG_SND_RES, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer135->Add( pgSndRes, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	pageSound->SetSizer( bSizer135 );
	pageSound->Layout();
	bSizer135->Fit( pageSound );
	pageProps->AddPage( pageSound, wxT("Sounds"), false );
	pageAction = new wxPanel( pageProps, wxID_PAGE_ACTION, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer136;
	bSizer136 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer176;
	bSizer176 = new wxBoxSizer( wxVERTICAL );
	
	bSizer176->SetMinSize( wxSize( 400,-1 ) );
	pgPropAction = new wxPropertyGrid(pageAction, wxID_PG_PROP_ACTION, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer176->Add( pgPropAction, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer136->Add( bSizer176, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer177;
	bSizer177 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer181;
	bSizer181 = new wxBoxSizer( wxHORIZONTAL );
	
	bSizer181->SetMinSize( wxSize( -1,200 ) );
	wxBoxSizer* bSizer1532;
	bSizer1532 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText1412 = new wxStaticText( pageAction, wxID_ANY, wxT("Special action anim.:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1412->Wrap( -1 );
	bSizer1532->Add( m_staticText1412, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	canvasGrpSpec = new wxPanel( pageAction, wxID_CANVAS_GRP_SPEC, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME|wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer1532->Add( canvasGrpSpec, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer181->Add( bSizer1532, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer15321;
	bSizer15321 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText14121 = new wxStaticText( pageAction, wxID_ANY, wxT("Die animation:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14121->Wrap( -1 );
	bSizer15321->Add( m_staticText14121, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	canvasGrpDie = new wxPanel( pageAction, wxID_CANVAS_GRP_DIE, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME|wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer15321->Add( canvasGrpDie, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer181->Add( bSizer15321, 1, wxEXPAND, 5 );
	
	
	bSizer177->Add( bSizer181, 0, wxEXPAND, 5 );
	
	
	bSizer136->Add( bSizer177, 1, wxEXPAND, 5 );
	
	
	pageAction->SetSizer( bSizer136 );
	pageAction->Layout();
	bSizer136->Fit( pageAction );
	pageProps->AddPage( pageAction, wxT("Special actions"), false );
	
	bSizer132->Add( pageProps, 1, wxEXPAND | wxALL, 5 );
	
	
	bSizer129->Add( bSizer132, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer129 );
	this->Layout();
	
	this->Centre( wxBOTH );
	

	// </wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormUnitsEdit' on 2026-10-12 19:24:03

	RescaleWindowDPI(this);
	
	// set icon
	wxIcon appIcon;
	appIcon.LoadFile("IDI_ICON2",wxBITMAP_TYPE_ICO_RESOURCE);
	if(appIcon.IsOk())
		SetIcon(appIcon);

	mmOpen->SetBitmaps(LoadSVGiconsBundle("IDR_OPEN"));
	mmSave->SetBitmaps(LoadSVGiconsBundle("IDR_SAVE"));
	mmSaveAs->SetBitmaps(LoadSVGiconsBundle("IDR_SAVE"));
	mmExit->SetBitmaps(LoadSVGiconsBundle("IDR_EXIT"));

	canvasIcon->SetDoubleBuffered(true);
	canvasInfo->SetDoubleBuffered(true);
	canvasProjectile->SetDoubleBuffered(true);
	canvasGrp->SetDoubleBuffered(true);
	canvasGrpAux->SetDoubleBuffered(true);	
	canvasGrpSpec->SetDoubleBuffered(true);
	canvasGrpDie->SetDoubleBuffered(true);
	canvasLightFireAnim->SetDoubleBuffered(true);
	canvasLightFirePNM->SetDoubleBuffered(true);
	canvasLightHitPNM->SetDoubleBuffered(true);
	canvasArmoredFireAnim->SetDoubleBuffered(true);
	canvasArmoredFirePNM->SetDoubleBuffered(true);
	canvasArmoredHitPNM->SetDoubleBuffered(true);
	canvasAirFireAnim->SetDoubleBuffered(true);
	canvasAirFirePNM->SetDoubleBuffered(true);
	canvasAirHitPNM->SetDoubleBuffered(true);

	canvasIcon->Bind(wxEVT_PAINT,&FormUnitsEdit::OnPaintIcon,this,wxID_CANVAS_ICON);
	canvasInfo->Bind(wxEVT_PAINT,&FormUnitsEdit::OnPaintInfo,this,wxID_CANVAS_INFO);
	canvasProjectile->Bind(wxEVT_PAINT,&FormUnitsEdit::OnPaintProjectile,this,wxID_CANVAS_PROJECTILE);
	canvasGrp->Bind(wxEVT_PAINT,&FormUnitsEdit::OnPaintFSU,this,wxID_CANVAS_GRP);
	canvasGrpAux->Bind(wxEVT_PAINT,&FormUnitsEdit::OnPaintFSU,this,wxID_CANVAS_GRP_AUX);
	canvasGrpSpec->Bind(wxEVT_PAINT,&FormUnitsEdit::OnPaintFSU,this,wxID_CANVAS_GRP_SPEC);
	canvasGrpDie->Bind(wxEVT_PAINT,&FormUnitsEdit::OnPaintFSU,this,wxID_CANVAS_GRP_DIE);

	canvasLightFireAnim->Bind(wxEVT_PAINT,&FormUnitsEdit::OnPaintFSU,this,wxID_CANVAS_LIGHT_FIRE_ANIM);
	canvasLightFirePNM->Bind(wxEVT_PAINT,&FormUnitsEdit::OnPaintPNM,this,wxID_CANVAS_LIGHT_FIRE_PNM);
	canvasLightHitPNM->Bind(wxEVT_PAINT,&FormUnitsEdit::OnPaintPNM,this,wxID_CANVAS_LIGHT_HIT_PNM);
	canvasArmoredFireAnim->Bind(wxEVT_PAINT,&FormUnitsEdit::OnPaintFSU,this,wxID_CANVAS_ARMORED_FIRE_ANIM);
	canvasArmoredFirePNM->Bind(wxEVT_PAINT,&FormUnitsEdit::OnPaintPNM,this,wxID_CANVAS_ARMORED_FIRE_PNM);
	canvasArmoredHitPNM->Bind(wxEVT_PAINT,&FormUnitsEdit::OnPaintPNM,this,wxID_CANVAS_ARMORED_HIT_PNM);
	canvasAirFireAnim->Bind(wxEVT_PAINT,&FormUnitsEdit::OnPaintFSU,this,wxID_CANVAS_AIR_FIRE_ANIM);
	canvasAirFirePNM->Bind(wxEVT_PAINT,&FormUnitsEdit::OnPaintPNM,this,wxID_CANVAS_AIR_FIRE_PNM);
	canvasAirHitPNM->Bind(wxEVT_PAINT,&FormUnitsEdit::OnPaintPNM,this,wxID_CANVAS_AIR_HIT_PNM);

		
	lboxUnits->SetGetItemTextCb(bind(&FormUnitsEdit::OnGetUnitName,this,placeholders::_1));
	
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormUnitsEdit::OnCloseClick,this,wxID_MM_EXIT);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormUnitsEdit::OnOpenClick,this,wxID_MM_OPEN_DEF);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormUnitsEdit::OnSaveClick,this,wxID_MM_SAVE_DEF);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormUnitsEdit::OnSaveClick,this,wxID_MM_SAVE_DEF_AS);

	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormUnitsEdit::OnSelectDEF,this,wxID_LBOX_DEFS);
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormUnitsEdit::OnSelectTargetDEF,this,wxID_LBOX_TARGET_DEFS);		
	
	Bind(wxEVT_COMMAND_LIST_ITEM_SELECTED,&FormUnitsEdit::OnSelectUnit,this,wxID_LBOX_UNITS);
	Bind(wxEVT_LIST_BEGIN_DRAG,&FormUnitsEdit::OnUnitBeginDrag,this,wxID_LBOX_UNITS);

	Bind(wxEVT_PG_CHANGED,&FormUnitsEdit::OnPGpropChange,this,wxID_PG_PROP_BASIC);
	Bind(wxEVT_PG_CHANGED,&FormUnitsEdit::OnPGpropChange,this,wxID_PG_PROP_GRP);
	Bind(wxEVT_PG_CHANGED,&FormUnitsEdit::OnPGpropChange,this,wxID_PG_PROP_SOUND);
	Bind(wxEVT_PG_CHANGED,&FormUnitsEdit::OnPGpropChange,this,wxID_PG_PROP_ACTION);
	
	lboxTargetDef->Bind(wxEVT_RIGHT_DOWN,&FormUnitsEdit::OnDefsPupupOpen,this);
	lboxUnits->Bind(wxEVT_RIGHT_DOWN,&FormUnitsEdit::OnUnitsPupupOpen,this);

	pgSndRes->Bind(wxEVT_PG_DOUBLE_CLICK,&FormUnitsEdit::OnSndDoubleClick,this);

	//pgPropGrp->Bind(wxEVT_PG_RIGHT_CLICK,&FormUnitsEdit::OnUnitPropsPupup,this);
	Bind(wxEVT_PG_RIGHT_CLICK,&FormUnitsEdit::OnUnitPropsPupupOpen,this,wxID_PG_PROP_GRP);
	Bind(wxEVT_PG_RIGHT_CLICK,&FormUnitsEdit::OnUnitPropsPupupOpen,this,wxID_PG_PROP_ACTION);
	

	// make animation timer
	m_rel_time = 0;
	m_timer.SetOwner(this);
	this->Connect(wxEVT_TIMER,wxTimerEventHandler(FormUnitsEdit::OnTimer),NULL,this);
	m_timer.Start(1000/15);
	

}

// animation timer
void FormUnitsEdit::OnTimer(wxTimerEvent& event)
{
	m_rel_time++;
	RefreshGrp();
}

// refresh graphics
void FormUnitsEdit::RefreshGrp(bool all)
{
	if(all)
	{
		canvasIcon->Refresh();	
		canvasInfo->Refresh();
	}
	
	canvasProjectile->Refresh();
	canvasGrp->Refresh();
	canvasGrpAux->Refresh();
	canvasGrpSpec->Refresh();
	canvasGrpDie->Refresh();
	canvasLightFireAnim->Refresh();
	canvasLightFirePNM->Refresh();
	canvasLightHitPNM->Refresh();
	canvasArmoredFireAnim->Refresh();
	canvasArmoredFirePNM->Refresh();
	canvasArmoredHitPNM->Refresh();
	canvasAirFireAnim->Refresh();
	canvasAirFirePNM->Refresh();
	canvasAirHitPNM->Refresh();
	
}


// on paint icon
void FormUnitsEdit::OnPaintIcon(wxPaintEvent& event)
{
	if(!m_unit)
	{
		canvasIcon->ClearBackground();
		return;
	}	
	auto gres = m_spell_data->gres.GetResource(m_unit->icon);
	if(!gres)
		return;

	int x_surf = canvasIcon->GetClientSize().GetWidth();
	int y_surf = canvasIcon->GetClientSize().GetHeight();
	if(!x_surf || !y_surf)
	{
		canvasIcon->ClearBackground();
		return;
	}

	// render icon
	wxBitmap* bmp = gres->Render(x_surf,y_surf,true,false,1.5);
	wxPaintDC pdc(canvasIcon);
	pdc.DrawBitmap(*bmp,wxPoint(0,0));
	delete bmp;
}

// on paint info render
void FormUnitsEdit::OnPaintInfo(wxPaintEvent& event)
{
	if(!m_unit)
	{
		canvasInfo->ClearBackground();
		return;
	}
	auto gres = m_spell_data->gres_info.GetResource(m_unit->info);
	if(!gres)
	{
		canvasInfo->ClearBackground();
		return;
	}

	// render icon
	int x_surf = canvasInfo->GetClientSize().GetWidth();
	int y_surf = canvasInfo->GetClientSize().GetHeight();
	if(!x_surf || !y_surf)
	{
		canvasInfo->ClearBackground();
		return;
	}

	wxBitmap* bmp = gres->Render(true,false,1.5);
	int x_size = bmp->GetWidth();
	int y_size = bmp->GetHeight();
	if(x_size > x_surf || y_size > y_surf)
	{
		double aspect = (double)x_size/y_size;
		if((double)x_surf/x_size > (double)y_surf/y_size)
		{
			y_size = y_surf;
			x_size = (double)y_surf*aspect;
		}
		else
		{
			x_size = x_surf;
			y_size = (double)x_surf/aspect;
		}
		wxImage img = bmp->ConvertToImage();
		delete bmp;
		img.Rescale(x_size,y_size,wxIMAGE_QUALITY_HIGH);
		bmp = new wxBitmap(img);
	}
	
	wxPaintDC pdc(canvasInfo);
	pdc.DrawBitmap(*bmp,wxPoint((x_surf - x_size)/2,(y_surf - y_size)/2));
	delete bmp;
}


// on paint projectile
void FormUnitsEdit::OnPaintProjectile(wxPaintEvent& event)
{
	if(!m_unit)
	{
		canvasProjectile->ClearBackground();
		return;
	}
	auto proj = m_spell_data->gres.GetProjectile(m_unit->projetile_name);
	if(!proj)
		return;
	
	int fid = m_rel_time % 16;
	auto frame = proj->glyphs[fid];
	if(!frame)
	{
		canvasProjectile->ClearBackground();
		return;
	}

	int x_canvas = canvasProjectile->GetClientSize().GetWidth();
	int y_canvas = canvasProjectile->GetClientSize().GetHeight();
	if(!x_canvas || !y_canvas)
	{
		canvasProjectile->ClearBackground();
		return;
	}
		
	wxBitmap* bmp = frame->Render(true);
	int x_size = bmp->GetWidth();
	int y_size = bmp->GetHeight();	
	wxPaintDC pdc(canvasProjectile);
	pdc.Clear();
	pdc.DrawBitmap(*bmp,wxPoint((x_canvas - x_size)/2,(y_canvas - y_size)/2));
	delete bmp;
}

// on paint PNM
void FormUnitsEdit::OnPaintPNM(wxPaintEvent& event)
{
	auto canvas = (wxPanel*)event.GetEventObject();
	if(!m_unit || !canvas)
	{
		canvas->ClearBackground();
		return;
	}
		
	std::string pnm_name;
	if(event.GetId() == wxID_CANVAS_LIGHT_FIRE_PNM)
		pnm_name = m_unit->pnm_light_shot_name;
	else if(event.GetId() == wxID_CANVAS_LIGHT_HIT_PNM)
		pnm_name = m_unit->pnm_light_hit_name;
	else if(event.GetId() == wxID_CANVAS_ARMORED_FIRE_PNM)
		pnm_name = m_unit->pnm_armored_shot_name;
	else if(event.GetId() == wxID_CANVAS_ARMORED_HIT_PNM)
		pnm_name = m_unit->pnm_armored_hit_name;
	else if(event.GetId() == wxID_CANVAS_AIR_FIRE_PNM)
		pnm_name = m_unit->pnm_air_shot_name;
	else if(event.GetId() == wxID_CANVAS_AIR_HIT_PNM)
		pnm_name = m_unit->pnm_air_hit_name;
	else
	{
		canvas->ClearBackground();
		return;
	}
		
	auto pnm = m_spell_data->gres.GetPNM(pnm_name);
	if(!pnm)
	{
		canvas->ClearBackground();
		return;
	}
	int fid = m_rel_time % pnm->frames.size();
	auto frame = pnm->frames[fid];

	int x_size = canvas->GetClientSize().x;
	int y_size = canvas->GetClientSize().y;
	if(!x_size || !y_size)
	{
		canvas->ClearBackground();
		return;
	}
	wxBitmap buffer(x_size,y_size,24);
	wxPaintDC pdc(canvas);

	// render using any terrin (common palette chunk)
	auto terr = m_spell_data->terrain[0];
	terr->RenderPNMpreview(buffer,frame,Terrain::RENDER_CENTER,1.5);
	pdc.DrawBitmap(buffer,wxPoint(0,0));
}

// on paint FSU resource
void FormUnitsEdit::OnPaintFSU(wxPaintEvent& event)
{
	auto canvas = (wxPanel*)event.GetEventObject();
	if(!m_unit || !canvas)
	{
		canvas->ClearBackground();
		return;
	}
	
	std::string grp_name;
	int frame_limit = -1;
	if(event.GetId() == wxID_CANVAS_GRP)
		grp_name = m_unit->gra;
	else if(event.GetId() == wxID_CANVAS_GRP_AUX)
		grp_name = m_unit->grb;
	else if(event.GetId() == wxID_CANVAS_LIGHT_FIRE_ANIM)
	{
		grp_name = m_unit->anim_atack_light_name;
		frame_limit = m_unit->anim_atack_light_frames;
	}
	else if(event.GetId() == wxID_CANVAS_ARMORED_FIRE_ANIM)
	{
		grp_name = m_unit->anim_atack_armor_name;
		frame_limit = m_unit->anim_atack_armor_frames;
	}
	else if(event.GetId() == wxID_CANVAS_AIR_FIRE_ANIM)
	{
		grp_name = m_unit->anim_atack_air_name;
		frame_limit = m_unit->anim_atack_air_frames;
	}
	else if(event.GetId() == wxID_CANVAS_GRP_SPEC)
	{
		grp_name = m_unit->action_fsu_name;
		frame_limit = m_unit->action_fsu_frames;
	}
	else if(event.GetId() == wxID_CANVAS_GRP_DIE)
	{
		grp_name = m_unit->die_anim_name;
		frame_limit = m_unit->die_anim_frames;
	}
	else
	{
		canvas->ClearBackground();
		return;
	}
	auto grp = m_spell_data->units_fsu->GetResource(grp_name);
	if(!grp)
	{
		canvas->ClearBackground();
		return;
	}

	FSU_sprite *frame = NULL;
	if(grp->anim.frames > 1)
	{
		if(frame_limit <= 0)
			frame_limit = grp->anim.frames;
		frame_limit = std::min<int>(frame_limit,grp->anim.frames);
		int slazim = std::max(grp->anim.azimuths,grp->anim.slopes);
		int tid = m_rel_time % (frame_limit*slazim);
		int fid = tid % frame_limit;
		int said = tid / frame_limit;
		frame = grp->anim.lists[said][fid];
	}
	else
	{
		if(frame_limit <= 0)
			frame_limit = grp->list.size();
		frame_limit = std::min<int>(frame_limit,grp->list.size());
		int fid = m_rel_time % frame_limit;
		frame = &grp->list[fid];
	}

	int x_surf = canvas->GetClientSize().GetWidth();
	int y_surf = canvas->GetClientSize().GetHeight();
	if(!x_surf || !y_surf)
	{
		canvas->ClearBackground();
		return;
	}

	std::vector<uint8_t> buffer;
	buffer.assign(x_surf*y_surf,250);
	uint8_t* buf = &buffer[0];
	uint8_t* buf_end = &buf[x_surf*y_surf];

	// render unit
	int zoom = 1;
	int org_x = x_surf/2 - (80)*zoom/2;
	int org_y = y_surf/2 - (48)*zoom/2;
	auto terr = m_spell_data->terrain[0];
	frame->Render(buf,buf_end,org_x-0,org_y,x_surf,terr->filter->darkpal,NULL,zoom);

	// apply gamma
	uint8_t pal[256][3];
	memcpy((void*)pal,(void*)terr->pal->GetPal(),256*3);
	apply_gamma((uint8_t*)pal,1.5);

	// render 24bit RGB data to raw bmp buffer
	wxBitmap bmp(x_surf,y_surf,24);
	wxNativePixelData pdata(bmp);
	wxNativePixelData::Iterator p(pdata);
	for(int y = 0; y < y_surf; y++)
	{
		p.MoveTo(pdata,0,y);
		for(int x = 0; x < x_surf; x++)
		{
			p.Red() = pal[*buf][0];
			p.Green() = pal[*buf][1];
			p.Blue() = pal[*buf][2];
			buf++;
			p++;
		}
	}	
	wxPaintDC pdc(canvas);
	pdc.DrawBitmap(bmp,wxPoint(0,0));
}


FormUnitsEdit::~FormUnitsEdit()
{
}

// close window
void FormUnitsEdit::OnClose(wxCloseEvent& ev)
{
	// signalize to caller
	wxPostEvent(GetParent(),ev);
	ev.Skip();
}

// on close form
void FormUnitsEdit::OnCloseClick(wxCommandEvent& event)
{
	Close();
}

// open DEF file(s)
void FormUnitsEdit::OnOpenClick(wxCommandEvent& event)
{
	// split path to folder and file
	std::filesystem::path last_path = m_spell_data->export_fs_path;
	wstring dir = last_path.parent_path();
	wstring name = L"";

	// show open dialog
	wxFileDialog openFileDialog(this,_("Open Spellcross Units DEF file(s)"),dir,name,"Spellcross units DEF file (*.DEF)|*.DEF",
		wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_MULTIPLE);
	if(openFileDialog.ShowModal() == wxID_CANCEL)
		return;
	
	wxArrayString list;
	openFileDialog.GetFilenames(list);
	if(!list.empty())
		m_spell_data->export_fs_path = list[0].ToStdWstring();

	// make list of DEFs	
	lboxDEF->Freeze();
	lboxDEF->Clear();
	lboxTargetDef->Freeze();
	lboxTargetDef->Clear();
	m_defs.clear();
	for(auto &item: list)
	{
		std::filesystem::path path = item.ToStdWstring();

		// load DEF
		std::vector<uint8_t> data;
		if(loaddata(path,data))
		{
			return;
		}
		
		// parse it
		std::shared_ptr<SpellUnits> units;
		try{
			units = std::make_shared<SpellUnits>(data);
			units->m_path = path;
		}catch(const std::runtime_error& error) {
			continue;
		}
		m_defs.push_back(units);

		lboxDEF->Append(path.filename().string());
		lboxTargetDef->Append(path.filename().string());
	}
	lboxDEF->Thaw();
	if(lboxDEF->GetCount())
		lboxDEF->Select(0);
	lboxTargetDef->Thaw();
	if(lboxTargetDef->GetCount())
		lboxTargetDef->Select(0);
	
	event.SetEventObject(lboxDEF);
	event.SetInt(-1);
	OnSelectDEF(event);
}


// save DEF file(s)
void FormUnitsEdit::OnSaveClick(wxCommandEvent& event)
{	
		
	wxArrayInt list;
	lboxTargetDef->GetSelections(list);
	if(list.empty())
	{
		wxMessageBox("No target DEF files selected! Cannot save anything.","Saving DEF file(s)",wxICON_EXCLAMATION);
		return;
	}	
	if(list.size() > 1 && event.GetId() == wxID_MM_SAVE_DEF_AS)
	{
		wxMessageBox("Multiple target DEF files selected! Cannot Save As.","Saving DEF file(s)",wxICON_EXCLAMATION);
		return;
	}

	if(list.size() == 1)	
	{			
		auto sid = list[0];
		if(sid < 0 || sid >= m_defs.size())
			return;
		auto units = m_defs[list[0]];
				
		// split path to folder and file
		std::filesystem::path last_path = units->m_path;
		wstring dir = last_path.parent_path();
		wstring name = last_path.filename();

		// show path dialog
		wxFileDialog saveFileDialog(this,_("Save Spellcross Units DEF file(s)"),dir,name,"Spellcross units DEF file (*.DEF)|*.DEF",
			wxFD_SAVE);
		if(saveFileDialog.ShowModal() == wxID_CANCEL)
			return;
		auto path = std::filesystem::path(saveFileDialog.GetPath().ToStdWstring());
		if(std::filesystem::exists(path))
		{
			wxMessageDialog dlg(this,string_format("Selected DEF file \"%s\" exist!\nOverwrite?",path),"Save Spellcross Units DEF file(s)",wxYES_NO|wxYES_DEFAULT);
			if(dlg.ShowModal() != wxID_YES)
				return;
		}

		// generate DEF
		std::vector<uint8_t> def;
		if(units->GenerateDEF(def))
		{
			wxMessageBox(string_format("Failed genrating units DEF file \"%s\"!",path),"Save Spellcross Units DEF file(s)",wxICON_ERROR);
			return;
		}
		if(savedata(path, def))
		{
			wxMessageBox(string_format("Failed saving units DEF file \"%s\"!",path),"Save Spellcross Units DEF file(s)",wxICON_ERROR);
			return;
		}

		return;
	}

	// list of DEFs to save
	std::vector<std::shared_ptr<SpellUnits>> def_list;
	for(auto &id: list)
	{
		auto sid = list[0];
		if(sid < 0 || sid >= m_defs.size())
			return;
		def_list.push_back(m_defs[id]);
	}
	
	// target directory dialogue
	wxDirDialog saveDialog(this,_("Select save directory for units DEF file(s)"),def_list[0]->m_path.parent_path().wstring(),wxDD_DIR_MUST_EXIST);
	if(saveDialog.ShowModal() == wxID_CANCEL)
		return;
	auto dir = std::filesystem::path(saveDialog.GetPath().ToStdWstring());

	// make dest paths
	std::string info = "Following files will be saved:\n";
	for(auto &item: def_list)
		info += string_format("  %s\n", dir / item->m_path.filename());
	info += "\n";
	info += "Existing files will be overwritten. Continue?";

	// rather ask for overwrite
	wxMessageDialog overDial(this,info,"Exporting map renders",wxICON_EXCLAMATION|wxYES_NO|wxYES_DEFAULT);
	if(overDial.ShowModal() != wxID_YES)
		return;
	
	// for each DEF file
	for(auto& def: def_list)
	{
		auto path = dir / def->m_path.filename();
		std::vector<uint8_t> data;
		if(def->GenerateDEF(data))
		{
			wxMessageBox(string_format("Failed genrating units DEF file \"%s\"!",path),"Save Spellcross Units DEF file(s)",wxICON_ERROR);
			return;
		}
		if(savedata(path,data))
		{
			wxMessageBox(string_format("Failed saving units DEF file \"%s\"!",path),"Save Spellcross Units DEF file(s)",wxICON_ERROR);
			return;
		}
		def->m_path = path;
	}

}


// select DEF file
void FormUnitsEdit::OnSelectDEF(wxCommandEvent& event)
{	
	auto unit_id = lboxUnits->GetNextItem(-1,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
	
	lboxUnits->ClearAll();
	m_units.reset();
	m_unit = NULL;

	wxMouseState mouseState = wxGetMouseState();
	int item_id = event.GetInt();	
	if(!mouseState.ControlDown() && event.GetEventObject() == lboxTargetDef)
	{
		for(int k = 0; k < lboxTargetDef->GetCount(); k++)
			lboxTargetDef->SetSelection(k,false);
		lboxTargetDef->Select(item_id);
	}

	int sid = -1;
	if(event.GetEventObject() == lboxTargetDef)
	{
		wxArrayInt list;
		lboxDEF->GetSelections(list);
		if(list.empty())
			return;	
		sid = list[0];
	}
	else if(event.GetEventObject() == lboxDEF)
		sid = lboxDEF->GetSelection();
	if(sid < 0)
		return;
	m_units = m_defs[sid];

	SetTitle(string_format("Spellcross Units Editor (%s)",m_units->m_path));

	// make units list
	lboxUnits->Freeze();	
	lboxUnits->AppendColumn("list",wxLIST_FORMAT_LEFT,wxLIST_AUTOSIZE);
	lboxUnits->SetItemCount(m_units->Count());
	lboxUnits->SetColumnWidth(0,wxLIST_AUTOSIZE_USEHEADER);

	// select default	
	if(unit_id >= 0 && unit_id < lboxUnits->GetItemCount())
	{
		lboxUnits->SetItemState(unit_id,wxLIST_STATE_SELECTED,wxLIST_STATE_SELECTED);
		lboxUnits->EnsureVisible(unit_id);
	}
	lboxUnits->Thaw();
	
	event.SetInt(-1);
	OnSelectUnit(event);
}


// select target DEF file
void FormUnitsEdit::OnSelectTargetDEF(wxCommandEvent& event)
{
	wxMouseState mouseState = wxGetMouseState();	
	int item_id = event.GetInt();
	if(mouseState.ShiftDown())
	{
		auto pa = std::min(item_id,m_last_target_def_id);
		auto pb = std::max(item_id,m_last_target_def_id);
		for(int k = 0; k < lboxTargetDef->GetCount(); k++)
			if(k >= pa && k <= pb)
				lboxTargetDef->SetSelection(k,true);
	}
	else if(!mouseState.ControlDown())
	{
		for(int k = 0; k < lboxTargetDef->GetCount(); k++)
			lboxTargetDef->SetSelection(k,false);
		lboxTargetDef->Select(item_id);
	}
	m_last_target_def_id = item_id;	
}




// list: get unit name
wxString FormUnitsEdit::OnGetUnitName(long item)
{
	if(!m_units)
		return("");
	auto unit = m_units->GetUnit(item);
	if(!unit)
		return("");	
	auto title = string_format("#%02d: %s",unit->type_id,unit->name);
	return(title);
}

// select unit
void FormUnitsEdit::OnSelectUnit(wxCommandEvent& event)
{
	auto act_prop_sel = GetPGselectionID(pgPropAction);
	
	pgPropBasic->Clear();
	pgPropGrp->Clear();
	pgPropSound->Clear();
	pgPropAction->Clear();
	m_unit = NULL;

	auto item_id = lboxUnits->GetNextItem(-1,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
	if(!m_units)
		return;
	m_unit = m_units->GetUnit(item_id);
	if(!m_unit)
		return;

	// fill props
	pgPropBasic->Freeze();
	pgPropBasic->Clear();
	wxPGProperty* grp = NULL;
	
	grp = pgPropBasic->Append(new wxPropertyCategory("Basic parameters:"));
	pgPropBasic->AppendIn(grp,new wxStringPropertyExt("Unit name",wxT(""),&m_unit->name,27));
	pgPropBasic->AppendIn(grp,new wxEnumPropertyExt("Unit type",wxT(""),MapToPGenumChoices(SpellUnitRec::c_str_unit_types),&m_unit->utype,SpellUnitRec::UTYPE_TYPE_MASK));
	pgPropBasic->AppendIn(grp,new wxIntPropertyExt("Man/vehicles count",wxT(""),&m_unit->cnt,0,255));
	pgPropBasic->AppendIn(grp,new wxIntPropertyExt("XP minimum",wxT(""),&m_unit->exp_min,0,1048576));
	pgPropBasic->AppendIn(grp,new wxIntPropertyExt("XP maximum",wxT(""),&m_unit->exp_max,0,1048576));
	
	pgPropBasic->AppendIn(grp,new wxIntPropertyExt("Opportunity/Reaction fire initiative?",wxT(""),&m_unit->res3,0,255));
	pgPropBasic->AppendIn(grp,new wxIntPropertyExt("Weapon reliability (deprecated)",wxT(""),&m_unit->res2,0,255));

	grp = pgPropBasic->Append(new wxPropertyCategory("Attack, defence, sight:"));
	pgPropBasic->AppendIn(grp,new wxIntPropertyExt("Light attack",wxT(""),&m_unit->attack_light,0,255));
	pgPropBasic->AppendIn(grp,new wxIntPropertyExt("Armored attack",wxT(""),&m_unit->attack_armored,0,255));
	pgPropBasic->AppendIn(grp,new wxIntPropertyExt("Air attack",wxT(""),&m_unit->attack_air,0,255));
	pgPropBasic->AppendIn(grp,new wxIntPropertyExt("Objects attack",wxT(""),&m_unit->attack_objects,0,255));
	pgPropBasic->AppendIn(grp,new wxIntPropertyExt("Attack range",wxT(""),&m_unit->fire_range,0,255));
	pgPropBasic->AppendIn(grp,new wxIntPropertyExt("Defence",wxT(""),&m_unit->defence,0,255));
	pgPropBasic->AppendIn(grp,new wxIntPropertyExt("Sight",wxT(""),&m_unit->sdir,0,255));
	

	grp = pgPropBasic->Append(new wxPropertyCategory("Action points:"));
	pgPropBasic->AppendIn(grp,new wxIntPropertyExt("Action points",wxT(""),&m_unit->ap,0,255));
	pgPropBasic->AppendIn(grp,new wxIntPropertyExt("AP per move",wxT(""),&m_unit->apw,0,255));
	pgPropBasic->AppendIn(grp,new wxIntPropertyExt("AP per forrest move",wxT(""),&m_unit->apfw,0,255));
	pgPropBasic->AppendIn(grp,new wxIntPropertyExt("AP per shot",wxT(""),&m_unit->aps,0,255));
	pgPropBasic->AppendIn(grp,new wxIntPropertyExt("Rounds to dig",wxT(""),&m_unit->dig_turns,0,255));
	
	grp = pgPropBasic->Append(new wxPropertyCategory("Special flags:"));
	pgPropBasic->AppendIn(grp,new wxBoolPropertyExt("Walk movement",wxT(""),(bool*)&m_unit->utype,SpellUnitRec::UTYPE_WALK));
	pgPropBasic->AppendIn(grp,new wxBoolPropertyExt("Hover movement",wxT(""),(bool*)&m_unit->utype,SpellUnitRec::UTYPE_HOVER));
	pgPropBasic->AppendIn(grp,new wxBoolPropertyExt("Flight movement",wxT(""),(bool*)&m_unit->utype,SpellUnitRec::UTYPE_FLY));
	pgPropBasic->AppendIn(grp,new wxBoolPropertyExt("Has turret",wxT(""),(bool*)&m_unit->utype,SpellUnitRec::UTYPE_TURRET));
	pgPropBasic->AppendIn(grp,new wxBoolPropertyExt("Flash&bones",wxT(""),(bool*)&m_unit->utype,SpellUnitRec::UTYPE_FLAHS));
	pgPropBasic->AppendIn(grp,new wxBoolPropertyExt("Demon?",wxT(""),(bool*)&m_unit->utype,SpellUnitRec::UTYPE_DEMON));

	grp = pgPropBasic->Append(new wxPropertyCategory("Fire flags:"));
	pgPropBasic->AppendIn(grp,new wxBoolPropertyExt("High turret origing",wxT(""),(bool*)&m_unit->fire_flags,SpellUnitRec::FIRE_HIGH_TURRET));
	pgPropBasic->AppendIn(grp,new wxBoolPropertyExt("Artillery fire",wxT(""),(bool*)&m_unit->fire_flags,SpellUnitRec::FIRE_INDIRECT));
	pgPropBasic->AppendIn(grp,new wxBoolPropertyExt("Steal enemy AP",wxT(""),(bool*)&m_unit->fire_flags,SpellUnitRec::FIRE_STEAL_AP));
	pgPropBasic->AppendIn(grp,new wxBoolPropertyExt("Inefficient to armored",wxT(""),(bool*)&m_unit->fire_flags,SpellUnitRec::FIRE_INEFFICIENT_TO_ARMORRED));
	pgPropBasic->AppendIn(grp,new wxBoolPropertyExt("Cannot fire from slopes",wxT(""),(bool*)&m_unit->fire_flags,SpellUnitRec::FIRE_NOT_SLOPES));
	pgPropBasic->AppendIn(grp,new wxBoolPropertyExt("MLRS side-fire",wxT(""),(bool*)&m_unit->fire_flags,SpellUnitRec::FIRE_MLRS));
	pgPropBasic->AppendIn(grp,new wxBoolPropertyExt("Uses fire projectile",wxT(""),(bool*)&m_unit->fire_flags,SpellUnitRec::FIRE_FIRE_PROJECTILE));
	pgPropBasic->AppendIn(grp,new wxBoolPropertyExt("Sensitive to fire",wxT(""),(bool*)&m_unit->fire_flags,SpellUnitRec::FIRE_FIRE_SENSITIVE));
	if(m_units->isEng())
		pgPropBasic->AppendIn(grp,new wxBoolPropertyExt("Fire-healed",wxT(""),(bool*)&m_unit->fire_flags,SpellUnitRec::FIRE_FIRE_HEALED));
	
	pgPropBasic->Thaw();
	pgPropBasic->FitColumns();
	//setPGsize(pgPropBasic);


	
	// graphics stuff
	std::string name_none = "<none>";
	auto grp_res_list = m_spell_data->units_fsu->GetResourceMap();
	grp_res_list.insert({-1,name_none});
	auto proj_names = m_spell_data->gres.GetProjectilesMap();
	proj_names.insert({-1,name_none});
	auto pnm_names = m_spell_data->gres.GetPNMmap();
	pnm_names.insert({-1,name_none});
	auto info_names = m_spell_data->gres_info.GetResourceMap();
	info_names.insert({-1,name_none});
	auto icon_names = m_spell_data->gres.GetUnitGlyphsMap();
	icon_names.insert({-1,name_none});

	pgPropGrp->Freeze();
	pgPropGrp->Clear();

	grp = pgPropGrp->Append(new wxPropertyCategory("General:"));
	pgPropGrp->AppendIn(grp,new wxEnumPropertyExt("Graphics resource",wxT(""),MapToPGenumChoices(grp_res_list),&m_unit->gra));
	pgPropGrp->AppendIn(grp,new wxEnumPropertyExt("Turret Graphics resource",wxT(""),MapToPGenumChoices(grp_res_list),&m_unit->grb));
	pgPropGrp->AppendIn(grp,new wxEnumPropertyExt("Projectile name",wxT(""),MapToPGenumChoices(proj_names),&m_unit->projetile_name));
	pgPropGrp->AppendIn(grp,new wxEnumPropertyExt("Unit INFO.FS resource name",wxT(""),MapToPGenumChoices(info_names),&m_unit->info));
	pgPropGrp->AppendIn(grp,new wxEnumPropertyExt("Unit glyph icon name",wxT(""),MapToPGenumChoices(icon_names),&m_unit->icon));
	pgPropBasic->AppendIn(grp,new wxIntPropertyExt("Visible man count",wxT(""),&m_unit->vis,0,5));
	
	grp = pgPropGrp->Append(new wxPropertyCategory("Light unit attack:"));
	pgPropGrp->AppendIn(grp,new wxBoolPropertyExt("Visible projectile",wxT(""),(bool*)&m_unit->projectile_visible,SpellUnitRec::PROJECTILE_LIGHT));
	pgPropGrp->AppendIn(grp,new wxEnumPropertyExt("Attack animation",wxT("anim_light"),MapToPGenumChoices(grp_res_list),&m_unit->anim_atack_light_name));
	pgPropGrp->AppendIn(grp,new wxIntPropertyExt("Animation frames limit",wxT("anim_light_frames"),&m_unit->anim_atack_light_frames,0,255));
	pgPropGrp->AppendIn(grp,new wxEnumPropertyExt("Shot PNM",wxT(""),MapToPGenumChoices(pnm_names),&m_unit->pnm_light_shot_name));
	pgPropGrp->AppendIn(grp,new wxEnumPropertyExt("Target hit PNM",wxT(""),MapToPGenumChoices(pnm_names),&m_unit->pnm_light_hit_name));

	grp = pgPropGrp->Append(new wxPropertyCategory("Armored unit attack:"));
	pgPropGrp->AppendIn(grp,new wxBoolPropertyExt("Visible projectile",wxT(""),(bool*)&m_unit->projectile_visible,SpellUnitRec::PROJECTILE_ARMOR));
	pgPropGrp->AppendIn(grp,new wxEnumPropertyExt("Attack animation",wxT("anim_armored"),MapToPGenumChoices(grp_res_list),&m_unit->anim_atack_armor_name));
	pgPropGrp->AppendIn(grp,new wxIntPropertyExt("Animation frames limit",wxT("anim_armored_frames"),&m_unit->anim_atack_armor_frames,0,255));
	pgPropGrp->AppendIn(grp,new wxEnumPropertyExt("Shot PNM",wxT(""),MapToPGenumChoices(pnm_names),&m_unit->pnm_armored_shot_name));
	pgPropGrp->AppendIn(grp,new wxEnumPropertyExt("Target hit PNM",wxT(""),MapToPGenumChoices(pnm_names),&m_unit->pnm_armored_hit_name));

	grp = pgPropGrp->Append(new wxPropertyCategory("Air unit attack:"));
	pgPropGrp->AppendIn(grp,new wxBoolPropertyExt("Visible projectile",wxT(""),(bool*)&m_unit->projectile_visible,SpellUnitRec::PROJECTILE_AIR));
	pgPropGrp->AppendIn(grp,new wxEnumPropertyExt("Attack animation",wxT("anim_air"),MapToPGenumChoices(grp_res_list),&m_unit->anim_atack_air_name));
	pgPropGrp->AppendIn(grp,new wxIntPropertyExt("Animation frames limit",wxT("anim_air_frames"),&m_unit->anim_atack_air_frames,0,255));
	pgPropGrp->AppendIn(grp,new wxEnumPropertyExt("Shot PNM",wxT(""),MapToPGenumChoices(pnm_names),&m_unit->pnm_air_shot_name));
	pgPropGrp->AppendIn(grp,new wxEnumPropertyExt("Target hit PNM",wxT(""),MapToPGenumChoices(pnm_names),&m_unit->pnm_air_hit_name));
			
	pgPropGrp->Thaw();
	pgPropGrp->FitColumns();
	//pgPropGrp->SetAutoLayout(false);
	//setPGsize(pgPropGrp);

	
	// sounds stuff
	auto snd_attack_names = m_spell_data->sounds->GetAttackClassMap();
	auto snd_move_names = m_spell_data->sounds->GetMoveClassMap();
	auto snd_report_names = m_spell_data->sounds->GetReportClassMap();
	auto snd_hit_names = m_spell_data->sounds->GetHitClassMap();
	auto snd_spec_names = m_spell_data->sounds->GetSpecialClassMap();

	pgPropSound->Freeze();
	pgPropSound->Clear();

	pgPropSound->Append(new wxEnumPropertyExt("Unit movement class",wxT(""),MapToPGenumChoices(snd_move_names),&m_unit->smov));
	pgPropSound->Append(new wxEnumPropertyExt("Unit report class",wxT(""),MapToPGenumChoices(snd_report_names),&m_unit->ssel));
	pgPropSound->Append(new wxEnumPropertyExt("Special action class",wxT(""),MapToPGenumChoices(snd_spec_names),&m_unit->snd_action_id));

	pgPropSound->Append(new wxEnumPropertyExt("Light attack class",wxT(""),MapToPGenumChoices(snd_attack_names),&m_unit->slig));
	pgPropSound->Append(new wxEnumPropertyExt("Armored attack class",wxT(""),MapToPGenumChoices(snd_attack_names),&m_unit->sarm));
	pgPropSound->Append(new wxEnumPropertyExt("Air attack class",wxT(""),MapToPGenumChoices(snd_attack_names),&m_unit->sair));
	pgPropSound->Append(new wxEnumPropertyExt("Unit hit class",wxT(""),MapToPGenumChoices(snd_hit_names),&m_unit->shit));
		
	pgPropSound->Thaw();
	pgPropSound->FitColumns();
	setPGsize(pgPropSound,15,getPGcount(pgPropSound));
	pgPropSound->Layout();
	pgPropSound->GetParent()->Layout();


	// special action stuff

	auto unit_names = m_units->GetUnitsMap();

	pgPropAction->Freeze();
	pgPropAction->Clear();
	
	grp = pgPropAction->Append(new wxPropertyCategory("Special action:"));
	pgPropAction->AppendIn(grp,new wxEnumPropertyExt("Type",wxT("action_type"),MapToPGenumChoices(SpellUnitRec::c_str_action_types),&m_unit->action_id));
	pgPropAction->AppendIn(grp,new wxIntPropertyExt("AP per action",wxT(""),&m_unit->action_ap,0,255));
	
	auto par_name = m_unit->GetSpecActionParName(1);
	if(!par_name.empty())
		par_name = string_format(" (%s)",par_name);
	pgPropAction->AppendIn(grp,new wxIntPropertyExt(string_format("Parameter #1%s",par_name),wxT(""),&m_unit->action_params[0],0,255));
	par_name = m_unit->GetSpecActionParName(2);
	if(!par_name.empty())
		par_name = string_format(" (%s)",par_name);
	pgPropAction->AppendIn(grp,new wxIntPropertyExt(string_format("Parameter #2%s",par_name),wxT(""),&m_unit->action_params[1],0,255));
	par_name = m_unit->GetSpecActionParName(3);
	if(!par_name.empty())
		par_name = string_format(" (%s)",par_name);
	if(m_unit->isSpecActionUnitID(3))
		pgPropAction->AppendIn(grp,new wxEnumPropertyExt(string_format("Parameter #3%s",par_name),wxT(""),MapToPGenumChoices(unit_names),&m_unit->action_params[2]));
	else
		pgPropAction->AppendIn(grp,new wxIntPropertyExt(string_format("Parameter #3%s",par_name),wxT(""),&m_unit->action_params[2],0,255));
	pgPropAction->AppendIn(grp,new wxEnumPropertyExt("Animation resource",wxT("spec_anim"),MapToPGenumChoices(grp_res_list),&m_unit->action_fsu_name));
	pgPropAction->AppendIn(grp,new wxIntPropertyExt("Animation frames limit",wxT("spec_anim_frames"),&m_unit->action_fsu_frames,0,255));

	grp = pgPropAction->Append(new wxPropertyCategory("Die action:"));
	pgPropAction->AppendIn(grp,new wxEnumPropertyExt("Type",wxT("die_action_type"),MapToPGenumChoices(SpellUnitRec::c_str_die_action_types),&m_unit->die_action_id));		
	par_name = m_unit->GetDieActionParName(1);
	if(!par_name.empty())
		par_name = string_format(" (%s)",par_name);
	pgPropAction->AppendIn(grp,new wxIntPropertyExt(string_format("Parameter #1%s",par_name),wxT(""),&m_unit->die_action_params[0],0,255));
	par_name = m_unit->GetDieActionParName(2);
	if(!par_name.empty())
		par_name = string_format(" (%s)",par_name);
	pgPropAction->AppendIn(grp,new wxIntPropertyExt(string_format("Parameter #2%s",par_name),wxT(""),&m_unit->die_action_params[1],0,255));	
	par_name = m_unit->GetDieActionParName(3);
	if(!par_name.empty())
		par_name = string_format(" (%s)",par_name);
	if(m_unit->isDieActionUnitID(3))
		pgPropAction->AppendIn(grp,new wxEnumPropertyExt(string_format("Parameter #3%s",par_name),wxT(""),MapToPGenumChoices(unit_names),&m_unit->die_action_params[2]));
	else
		pgPropAction->AppendIn(grp,new wxIntPropertyExt(string_format("Parameter #3%s",par_name),wxT(""),&m_unit->die_action_params[2],0,255));		
		
	pgPropAction->AppendIn(grp,new wxEnumPropertyExt("Animation resource",wxT("die_anim"),MapToPGenumChoices(grp_res_list),&m_unit->die_anim_name));
	pgPropAction->AppendIn(grp,new wxIntPropertyExt("Animation frames limit",wxT("die_anim_frames"),&m_unit->die_anim_frames,0,255));

	pgPropAction->Thaw();
	pgPropAction->FitColumns();	
	//setPGsize(pgPropAction);

	//SetPQselectionByID(pgPropAction,act_prop_sel);

	RefreshGrp(true);
	FillSounds();
}

// fill sounds
void FormUnitsEdit::FillSounds()
{	
	pgSndRes->Clear();
	
	if(!m_unit)
		return;

	// fill props
	pgSndRes->Freeze();
	pgSndRes->Clear();
	wxPGProperty* grp = NULL;

	grp = pgSndRes->Append(new wxPropertyCategory("Unit reporting:"));	
	auto snd = m_spell_data->sounds->GetReportClass(m_unit->ssel);
	pgSndRes->AppendIn(grp,new wxStringProperty("Report A",wxT(""),snd->GetSampleName(0)));
	pgSndRes->AppendIn(grp,new wxStringProperty("Report B",wxT(""),snd->GetSampleName(1)));
	delete snd;	
	snd = m_spell_data->sounds->GetContactClass(m_unit->ssel);
	pgSndRes->AppendIn(grp,new wxStringProperty("Contact",wxT(""),snd->GetSampleName()));
	delete snd;

	grp = pgSndRes->Append(new wxPropertyCategory("Unit move:"));
	snd = m_spell_data->sounds->GetMoveClass(m_unit->smov);
	pgSndRes->AppendIn(grp,new wxStringProperty("Start",wxT(""),snd->GetSampleName(0)));
	pgSndRes->AppendIn(grp,new wxStringProperty("Go",wxT(""),snd->GetSampleName(1)));
	pgSndRes->AppendIn(grp,new wxStringProperty("Stop",wxT(""),snd->GetSampleName(2)));
	delete snd;

	grp = pgSndRes->Append(new wxPropertyCategory("Unit hit:"));
	snd = m_spell_data->sounds->GetHitClass(m_unit->shit);	
	pgSndRes->AppendIn(grp,new wxStringProperty("Hit",wxT(""),snd->GetSampleName()));
	delete snd;
	snd = m_spell_data->sounds->GetDieClass(m_unit->shit);	
	pgSndRes->AppendIn(grp,new wxStringProperty("Die",wxT(""),snd->GetSampleName()));
	delete snd;

	grp = pgSndRes->Append(new wxPropertyCategory("Light attack:"));
	auto attack_snd = m_spell_data->sounds->GetAttackClass(m_unit->slig);	
	pgSndRes->AppendIn(grp,new wxStringProperty("Shot",wxT(""),attack_snd->shot->GetSampleName()));
	pgSndRes->AppendIn(grp,new wxStringProperty("Hit flash",wxT(""),attack_snd->hit_flash->GetSampleName()));
	pgSndRes->AppendIn(grp,new wxStringProperty("Hit armor",wxT(""),attack_snd->hit_armor->GetSampleName()));
	pgSndRes->AppendIn(grp,new wxStringProperty("Missed",wxT(""),attack_snd->hit_miss->GetSampleName()));
	delete attack_snd;

	grp = pgSndRes->Append(new wxPropertyCategory("Armored attack:"));
	attack_snd = m_spell_data->sounds->GetAttackClass(m_unit->sarm);
	pgSndRes->AppendIn(grp,new wxStringProperty("Shot",wxT(""),attack_snd->shot->GetSampleName()));
	pgSndRes->AppendIn(grp,new wxStringProperty("Hit flash",wxT(""),attack_snd->hit_flash->GetSampleName()));
	pgSndRes->AppendIn(grp,new wxStringProperty("Hit armor",wxT(""),attack_snd->hit_armor->GetSampleName()));
	pgSndRes->AppendIn(grp,new wxStringProperty("Missed",wxT(""),attack_snd->hit_miss->GetSampleName()));
	delete attack_snd;

	grp = pgSndRes->Append(new wxPropertyCategory("Air attack:"));
	attack_snd = m_spell_data->sounds->GetAttackClass(m_unit->sair);
	pgSndRes->AppendIn(grp,new wxStringProperty("Shot",wxT(""),attack_snd->shot->GetSampleName()));
	pgSndRes->AppendIn(grp,new wxStringProperty("Hit flash",wxT(""),attack_snd->hit_flash->GetSampleName()));
	pgSndRes->AppendIn(grp,new wxStringProperty("Hit armor",wxT(""),attack_snd->hit_armor->GetSampleName()));
	pgSndRes->AppendIn(grp,new wxStringProperty("Missed",wxT(""),attack_snd->hit_miss->GetSampleName()));
	delete attack_snd;

	grp = pgSndRes->Append(new wxPropertyCategory("Other:"));
	snd = m_spell_data->sounds->GetSpecialClass(m_unit->snd_action_id);
	pgSndRes->AppendIn(grp,new wxStringProperty("Special action",wxT(""),snd->GetSampleName()));
	delete snd;
	
	pgSndRes->Thaw();
	pgSndRes->FitColumns();
	//setPGsize(pgSndRes);
}

void FormUnitsEdit::OnSndDoubleClick(wxPropertyGridEvent& event)
{
	wxPGProperty* prop = event.GetProperty();
	if(prop)
	{
		auto snd_name = prop->GetValue().GetString().ToStdString();
		auto snd = m_spell_data->sounds->GetSound(snd_name.c_str());	
		if(snd)
			snd->PlayAsync(0.5);
	}

	// Always call event.Skip() if you want the default action to also process
	event.Skip();
}


// unit drag/drop stuff
void FormUnitsEdit::OnUnitBeginDrag(wxListEvent& event)
{
	m_drag_unit_id = event.GetIndex();

	lboxUnits->Bind(wxEVT_MOTION,&FormUnitsEdit::OnUnitDragMotion,this);
	lboxUnits->Bind(wxEVT_LEFT_UP,&FormUnitsEdit::OnUnitEndDrag,this);
	lboxUnits->Bind(wxEVT_MOUSE_CAPTURE_LOST,&FormUnitsEdit::OnUnitDragLost,this);

	m_drag_unit_img = std::make_unique<wxDragImage>(OnGetUnitName(m_drag_unit_id));	
	m_drag_unit_img->BeginDrag(wxDefaultPosition,lboxUnits);
	m_drag_unit_img->Show();

	lboxUnits->CaptureMouse();
}
void FormUnitsEdit::OnUnitDragMotion(wxMouseEvent& event)
{
	if(m_drag_unit_img.get() && event.Dragging() && lboxUnits->HasCapture()) {
		// Move the drag image to the current mouse position
		// Convert to screen/client coordinates properly depending on BeginDrag setup
		wxPoint pos = event.GetPosition();
		m_drag_unit_img->Move(pos);
	}
	event.Skip();
}
void FormUnitsEdit::OnUnitDragLost(wxMouseCaptureLostEvent& event)
{
}
void FormUnitsEdit::OnUnitEndDrag(wxMouseEvent& event)
{
	if(m_drag_unit_img.get())
	{
		m_drag_unit_img->EndDrag();
		m_drag_unit_img->Hide();
	}

	if(m_drag_unit_id < 0)
		return;
	auto drag_id = m_drag_unit_id;

	if(lboxUnits->HasCapture())
		lboxUnits->ReleaseMouse();
	lboxUnits->Unbind(wxEVT_MOTION,&FormUnitsEdit::OnUnitDragMotion,this);
	lboxUnits->Unbind(wxEVT_LEFT_UP,&FormUnitsEdit::OnUnitEndDrag,this);
	lboxUnits->Unbind(wxEVT_MOUSE_CAPTURE_LOST,&FormUnitsEdit::OnUnitDragLost,this);
	m_drag_unit_id = -1;

	//wxMouseState mouseState = wxGetMouseState();
	wxPoint pos = event.GetPosition();
	int flags = 0;
	auto drop_id = lboxUnits->HitTest(pos,flags);
	if(drag_id >= 0 && drop_id >=0)
	{
		if(event.ControlDown())
			m_units->CopyUnit(drag_id,drop_id);
		else
			m_units->SwapUnits(drag_id, drop_id);
		lboxUnits->Refresh();
	}
}


// units popup menu
void FormUnitsEdit::OnUnitsPupupOpen(wxMouseEvent& event)
{
	wxMenu menu;

	menu.Append((int)PopupActions::CLR_ALL_UNITS,"Clear all");
	menu.Append((int)PopupActions::SEL_ALL_UNITS,"Select all");
	menu.AppendSeparator();
	menu.Append((int)PopupActions::ADD_UNIT,"Add new unit");

	wxArrayInt sel;
	lboxTargetDef->GetSelections(sel);
	if(lboxUnits->GetSelectedItemCount() > 0 && !sel.empty())
	{
		menu.AppendSeparator();
		menu.Append((int)PopupActions::COPY_TO_TARGET,"Copy unit(s) to target DEF(s)");
	}
	
	menu.Connect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(FormUnitsEdit::OnUnitsPupup),NULL,this);
	PopupMenu(&menu);
}
void FormUnitsEdit::OnUnitsPupup(wxCommandEvent& event)
{
	auto menu_id = (PopupActions)event.GetId();
	auto menu = (wxMenu*)event.GetEventObject();
	if(!menu)
		return;

	if(menu_id == PopupActions::CLR_ALL_UNITS)
	{
		lboxUnits->SetItemState(-1,0,wxLIST_STATE_SELECTED);
	}
	else if(menu_id == PopupActions::SEL_ALL_UNITS)
	{
		lboxUnits->SetItemState(-1,wxLIST_STATE_SELECTED,wxLIST_STATE_SELECTED);
	}
	else if(menu_id == PopupActions::ADD_UNIT)
	{
		if(!m_units)
			return;
		auto unit = m_units->AddUnit();
		
				
		wxCommandEvent evt;
		evt.SetEventObject(lboxTargetDef);
		OnSelectDEF(evt);

		lboxUnits->SetItemState(-1,0,wxLIST_STATE_SELECTED);
		lboxUnits->SetItemState(unit->type_id,wxLIST_STATE_SELECTED,wxLIST_STATE_SELECTED);
		lboxUnits->EnsureVisible(unit->type_id);
		lboxUnits->Refresh();
	}
	else if(menu_id == PopupActions::COPY_TO_TARGET)
	{
		// target DEFs
		std::vector<std::shared_ptr<SpellUnits>> def_list;
		wxArrayInt sel;
		lboxTargetDef->GetSelections(sel);
		for(auto &id: sel)
			if(id >= 0 && id < m_defs.size())
				def_list.push_back(m_defs[id]);
		
		// for each unit
		int item_id = -1;
		while((item_id = lboxUnits->GetNextItem(item_id,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED)) >= 0)
		{
			auto unit = m_units->GetUnit(item_id);
			if(!unit)
				continue;

			// copy to all defs
			for(auto &def: def_list)
				def->CopyUnit(unit, item_id);
		}		
	}
}




// props popup menu
void FormUnitsEdit::OnUnitPropsPupupOpen(wxPropertyGridEvent& event)
{
	wxMenu menu;

	auto prop = event.GetProperty();
	if(!prop)
		return;
	if(!m_unit)
		return;

	std::string *anim_name;
	auto name = prop->GetName();
	if(name == "anim_light_frames")
		anim_name = &m_unit->anim_atack_light_name;
	else if(name == "anim_armored_frames")
		anim_name = &m_unit->anim_atack_armor_name;
	else if(name == "anim_air_frames")
		anim_name = &m_unit->anim_atack_air_name;
	else if(name == "spec_anim_frames")
		anim_name = &m_unit->action_fsu_name;
	else if(name == "die_anim_frames")
		anim_name = &m_unit->die_anim_name;
	else
		return;
	auto grp = m_spell_data->units_fsu->GetResource(*anim_name);
	if(!grp)
		return;
	if(!grp->anim.frames)
		return;

	menu.Append((int)PopupActions::RESET_ANIM_FRAMES,"Set frames to max");

	menu.SetClientData(anim_name);
		
	menu.Connect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(FormUnitsEdit::OnUnitPropsPupup),NULL,this);
	PopupMenu(&menu);
}
void FormUnitsEdit::OnUnitPropsPupup(wxCommandEvent& event)
{
	auto menu_id = (PopupActions)event.GetId();
	auto menu = (wxMenu*)event.GetEventObject();
	if(!menu)
		return;

	if(menu_id == PopupActions::RESET_ANIM_FRAMES)
	{
		auto *anim_name = (std::string*)menu->GetClientData();
		if(!anim_name)
			return;
		if(!m_unit)
			return;

		auto grp = m_spell_data->units_fsu->GetResource(*anim_name);
		if(!grp || !grp->anim.frames)
			return;
		auto frames = grp->anim.frames;
		
		if(anim_name == &m_unit->anim_atack_light_name)
			m_unit->anim_atack_light_frames = frames;
		else if(anim_name == &m_unit->anim_atack_armor_name)
			m_unit->anim_atack_armor_frames = frames;
		else if(anim_name == &m_unit->anim_atack_air_name)
			m_unit->anim_atack_air_frames = frames;
		else if(anim_name == &m_unit->action_fsu_name)
			m_unit->action_fsu_frames = frames;
		else if(anim_name == &m_unit->die_anim_name)
			m_unit->die_anim_frames = frames;
		
		event.SetEventObject(lboxUnits);
		event.SetId(wxID_LBOX_UNITS);
		event.SetEventType(wxEVT_COMMAND_LIST_ITEM_SELECTED);
		wxPostEvent(this->GetEventHandler(),event);
	}
	

}




// edit unit props
void FormUnitsEdit::OnPGpropChange(wxPropertyGridEvent& event)
{
	auto pgrid = (wxPropertyGrid*)event.GetEventObject();
	if(!pgrid)
		return;
	auto prop = event.GetProperty();
	if(!prop)
		return;
	auto obj = (wxPGobj*)prop->GetClientObject();
	if(obj)
	{
		obj->Update(prop);
		
		auto name = prop->GetName().ToStdString();
		if(iequals({"action_type"s,"die_action_type"s},name) >= 0)
		{
			wxCommandEvent evt;
			OnSelectUnit(evt);
		}
		FillSounds();
		RefreshGrp(true);


		if(m_unit)
		{
			std::string anim_name;
			std::string name = prop->GetName().ToStdString();
			if(name == "anim_light")
				anim_name = m_unit->anim_atack_light_name;
			else if(name == "anim_armored")
				anim_name = m_unit->anim_atack_armor_name;
			else if(name == "anim_air")
				anim_name = m_unit->anim_atack_air_name;
			else if(name == "spec_anim")
				anim_name = m_unit->action_fsu_name;
			else if(name == "die_anim")
				anim_name = m_unit->die_anim_name;
			auto grp = m_spell_data->units_fsu->GetResource(anim_name);
			int frames = 0;
			if(grp && grp->anim.frames)
				frames = grp->anim.frames;
			if(name == "anim_light")
				m_unit->anim_atack_light_frames = frames;
			else if(name == "anim_armored")
				m_unit->anim_atack_armor_frames = frames;
			else if(name == "anim_air")
				m_unit->anim_atack_air_frames = frames;
			else if(name == "spec_anim")
				m_unit->action_fsu_frames = frames;
			else if(name == "die_anim")
				m_unit->die_anim_frames = frames;

			auto prop_frames = GetPGselectionID(pgrid,string_format("%s_frames",name));
			if(prop_frames)
				prop_frames->SetValueFromInt(frames);

		}
	}
}


// rules popup menu
void FormUnitsEdit::OnDefsPupupOpen(wxMouseEvent& event)
{
	wxMenu menu;
	
	menu.Append((int)PopupActions::CLR_ALL_DEFS,"Clear all");
	menu.Append((int)PopupActions::SEL_ALL_DEFS,"Select all");
	
	menu.Connect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(FormUnitsEdit::OnDefsPupup),NULL,this);
	PopupMenu(&menu);
}
void FormUnitsEdit::OnDefsPupup(wxCommandEvent& event)
{
	auto menu_id = (PopupActions)event.GetId();
	auto menu = (wxMenu*)event.GetEventObject();
	if(!menu)
		return;

	if(menu_id == PopupActions::CLR_ALL_DEFS)
	{
		for(int k = 0; k < lboxTargetDef->GetCount(); k++)
			lboxTargetDef->SetSelection(k,false);
	}
	else if(menu_id == PopupActions::SEL_ALL_DEFS)
	{
		for(int k = 0; k < lboxTargetDef->GetCount(); k++)
			lboxTargetDef->Select(k);
	}	
}



