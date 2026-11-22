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
#include "form_gr_encode.h"
#include <wx/app.h> // wxApp base class used in main.h
#include "main.h"
#include "fs_archive.h"
#include "LZ_spell.h"
#include "wx_other.h"

#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <wx/dragimag.h>
#include <wx/textdlg.h>
#include <wx/statbmp.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/utils.h>
#include <wx/busyinfo.h>
#include <wx/config.h>
#include <deque>
#include <array>
#include <cmath>
#include <algorithm>
#include <cstdint>
#include <vector>
#include <cstring>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <cctype>

///////////////////////////////////////////////////////////////////////////

FormUnitsEdit::FormUnitsEdit( wxWindow* parent, wxWindowID id,SpellData *spell_data, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) 
	: wxFrame( parent, id, title, pos, size, style ),
	m_spell_data(spell_data)
{
	// <wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormUnitsEdit' on 2026-10-19 18:36:50
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
	bSizer133 = new wxBoxSizer( wxHORIZONTAL );
	
	pgPropBasic = new wxPropertyGrid(pagePropBasic, wxID_PG_PROP_BASIC, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer133->Add( pgPropBasic, 1, wxEXPAND|wxALL, 5 );
	
	pgPropBasic2 = new wxPropertyGrid(pagePropBasic, wxID_PG_PROP_BASIC2, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer133->Add( pgPropBasic2, 1, wxALL|wxEXPAND, 5 );

	
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
	

	// </wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormUnitsEdit' on 2026-10-19 18:36:50

	RescaleWindowDPI(this);

	// Keep added controls outside the wxFormBuilder-generated section.
	const auto newUnitMenuId = wxWindow::NewControlId();
	mmFile->Insert(1, newUnitMenuId, "New unit...\tCtrl+N");
	Bind(wxEVT_MENU, &FormUnitsEdit::OnNewUnit, this, newUnitMenuId);
	auto newUnitButton = new wxButton(this, wxID_ANY, "New unit...");
	bSizer130->Add(newUnitButton, 0, wxALL|wxEXPAND, 5);
	newUnitButton->Bind(wxEVT_BUTTON, &FormUnitsEdit::OnNewUnit, this);
	auto testUnitButton = new wxButton(this, wxID_ANY, "Place / test on map...");
	bSizer130->Add(testUnitButton, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, 5);
	testUnitButton->Bind(wxEVT_BUTTON, &FormUnitsEdit::OnTestUnitOnMap, this);
	auto buildGraphicsButton = new wxButton(this, wxID_ANY, "Build PNG graphics / UNITS.FSU...");
	bSizer130->Add(buildGraphicsButton, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, 5);
	buildGraphicsButton->Bind(wxEVT_BUTTON, &FormUnitsEdit::OnBuildUnitGraphics, this);
	auto imageButton = new wxButton(this, wxID_ANY, "Import 3D model / build sprites...");
	bSizer130->Add(imageButton, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, 5);
	imageButton->Bind(wxEVT_BUTTON, &FormUnitsEdit::OnGenerateGameSprites, this);
	auto exportModButton = new wxButton(this, wxID_ANY, "Export mod COMMON.FS...");
	bSizer130->Add(exportModButton, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, 5);
	exportModButton->Bind(wxEVT_BUTTON, &FormUnitsEdit::OnExportUnitMod, this);
	Layout();
	
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
	Bind(wxEVT_PG_CHANGED,&FormUnitsEdit::OnPGpropChange,this,wxID_PG_PROP_BASIC2);
	Bind(wxEVT_PG_CHANGED,&FormUnitsEdit::OnPGpropChange,this,wxID_PG_PROP_GRP);
	Bind(wxEVT_PG_CHANGED,&FormUnitsEdit::OnPGpropChange,this,wxID_PG_PROP_SOUND);
	Bind(wxEVT_PG_CHANGED,&FormUnitsEdit::OnPGpropChange,this,wxID_PG_PROP_ACTION);
	
	lboxTargetDef->Bind(wxEVT_RIGHT_DOWN,&FormUnitsEdit::OnDefsPupupOpen,this);
	lboxUnits->Bind(wxEVT_RIGHT_DOWN,&FormUnitsEdit::OnUnitsPupupOpen,this);

	pgSndRes->Bind(wxEVT_PG_DOUBLE_CLICK,&FormUnitsEdit::OnSndDoubleClick,this);

	//pgPropGrp->Bind(wxEVT_PG_RIGHT_CLICK,&FormUnitsEdit::OnUnitPropsPupup,this);
	Bind(wxEVT_PG_RIGHT_CLICK,&FormUnitsEdit::OnUnitPropsPupupOpen,this,wxID_PG_PROP_BASIC);
	Bind(wxEVT_PG_RIGHT_CLICK,&FormUnitsEdit::OnUnitPropsPupupOpen,this,wxID_PG_PROP_BASIC2);
	Bind(wxEVT_PG_RIGHT_CLICK,&FormUnitsEdit::OnUnitPropsPupupOpen,this,wxID_PG_PROP_SOUND);
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
	{
		frame_limit = m_unit->gra_frames;
		grp_name = m_unit->gra;
	}
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
	// Preserve full paths: unit DEFs can live outside the working directory.
	openFileDialog.GetPaths(list);
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
		units->m_path = path;
		SetTitle(string_format("Spellcross Units Editor (%s)",path));
		lboxDEF->SetString(sid, path.filename().wstring());
		lboxTargetDef->SetString(sid, path.filename().wstring());
		SetStatusText("Saved JEDNOTKY.DEF successfully.");
		return;
	}

	// list of DEFs to save
	std::vector<std::shared_ptr<SpellUnits>> def_list;
	for(auto &id: list)
	{
		if(id < 0 || id >= m_defs.size())
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
	pgPropBasic2->Clear();
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
	//pgPropBasic->AppendIn(grp,new wxIntPropertyExt("AP per forrest move",wxT(""),&m_unit->apfw,0,255));
	pgPropBasic->AppendIn(grp,new wxIntPropertyExt("AP per shot",wxT(""),&m_unit->aps,0,255));
	pgPropBasic->AppendIn(grp,new wxIntPropertyExt("Rounds to dig",wxT(""),&m_unit->dig_turns,0,255));
	pgPropBasic->Thaw();
	pgPropBasic->FitColumns();


	pgPropBasic2->Freeze();
	pgPropBasic2->Clear();
	grp = pgPropBasic2->Append(new wxPropertyCategory("Special flags:"));
	pgPropBasic2->AppendIn(grp,new wxBoolPropertyExt("Walk movement",wxT(""),&m_unit->utype,SpellUnitRec::UTYPE_WALK));
	pgPropBasic2->AppendIn(grp,new wxBoolPropertyExt("Hover movement",wxT(""),&m_unit->utype,SpellUnitRec::UTYPE_HOVER));
	pgPropBasic2->AppendIn(grp,new wxBoolPropertyExt("Flight movement",wxT(""),&m_unit->utype,SpellUnitRec::UTYPE_FLY));
	pgPropBasic2->AppendIn(grp,new wxBoolPropertyExt("Has turret",wxT(""),&m_unit->utype,SpellUnitRec::UTYPE_TURRET));
	pgPropBasic2->AppendIn(grp,new wxBoolPropertyExt("Flash&bones",wxT(""),&m_unit->utype,SpellUnitRec::UTYPE_FLAHS));
	pgPropBasic2->AppendIn(grp,new wxBoolPropertyExt("Demon?",wxT(""),&m_unit->utype,SpellUnitRec::UTYPE_DEMON));

	grp = pgPropBasic2->Append(new wxPropertyCategory("Fire flags:"));
	pgPropBasic2->AppendIn(grp,new wxBoolPropertyExt("No unit turn before fire",wxT(""),&m_unit->fire_flags,SpellUnitRec::FIRE_NO_TURN));
	pgPropBasic2->AppendIn(grp,new wxBoolPropertyExt("Artillery fire",wxT(""),&m_unit->fire_flags,SpellUnitRec::FIRE_INDIRECT));
	pgPropBasic2->AppendIn(grp,new wxBoolPropertyExt("Steal enemy AP",wxT(""),&m_unit->fire_flags,SpellUnitRec::FIRE_STEAL_AP));
	pgPropBasic2->AppendIn(grp,new wxBoolPropertyExt("Reset sprite immediately after fire anim",wxT(""),&m_unit->fire_flags,SpellUnitRec::FIRE_RESET_SPRITE_AFTER));
	pgPropBasic2->AppendIn(grp,new wxBoolPropertyExt("Cannot fire from slopes",wxT(""),&m_unit->fire_flags,SpellUnitRec::FIRE_NOT_SLOPES));
	pgPropBasic2->AppendIn(grp,new wxBoolPropertyExt("MLRS side-fire",wxT(""),&m_unit->fire_flags,SpellUnitRec::FIRE_MLRS));
	pgPropBasic2->AppendIn(grp,new wxBoolPropertyExt("Uses fire projectile",wxT(""),&m_unit->fire_flags,SpellUnitRec::FIRE_FIRE_PROJECTILE));
	pgPropBasic2->AppendIn(grp,new wxBoolPropertyExt("Sensitive to fire",wxT(""),&m_unit->fire_flags,SpellUnitRec::FIRE_FIRE_SENSITIVE));
	if (m_units->isEng())
	{
		pgPropBasic2->AppendIn(grp, new wxBoolPropertyExt("Fire-healed (ENG engine only)", wxT(""), &m_unit->fire_flags, SpellUnitRec::FIRE_FIRE_HEALED));
		pgPropBasic2->AppendIn(grp, new wxBoolPropertyExt("Ignore light target defence (HonzaQ patch V1.20+)", wxT(""), &m_unit->fire_flags, SpellUnitRec::FIRE_IGNORE_LIGHT_TARGET_DEFENCE));
		pgPropBasic2->AppendIn(grp, new wxBoolPropertyExt("Ignore armored target defence (HonzaQ patch V1.20+)", wxT(""), &m_unit->fire_flags, SpellUnitRec::FIRE_IGNORE_ARMORED_TARGET_DEFENCE));
		pgPropBasic2->AppendIn(grp, new wxBoolPropertyExt("Ignore air target defence (HonzaQ patch V1.20+)", wxT(""), &m_unit->fire_flags, SpellUnitRec::FIRE_IGNORE_AIR_TARGET_DEFENCE));
	}
	pgPropBasic2->Thaw();
	pgPropBasic2->FitColumns();


	
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
	pgPropGrp->AppendIn(grp,new wxEnumPropertyExt("Main graphics resource",wxT("anim_main"),MapToPGenumChoices(grp_res_list),&m_unit->gra));
	pgPropGrp->AppendIn(grp,new wxIntPropertyExt("Main graphics movement frames limit",wxT("anim_main_frames"),&m_unit->gra_frames,0,255));
	pgPropGrp->AppendIn(grp,new wxEnumPropertyExt("Aux Graphics resource (tank turret)",wxT(""),MapToPGenumChoices(grp_res_list),&m_unit->grb));
	pgPropGrp->AppendIn(grp,new wxEnumPropertyExt("Projectile name",wxT(""),MapToPGenumChoices(proj_names),&m_unit->projetile_name));
	pgPropGrp->AppendIn(grp,new wxEnumPropertyExt("Unit INFO.FS resource name",wxT(""),MapToPGenumChoices(info_names),&m_unit->info));
	pgPropGrp->AppendIn(grp,new wxEnumPropertyExt("Unit glyph icon name",wxT(""),MapToPGenumChoices(icon_names),&m_unit->icon));
	pgPropBasic->AppendIn(grp,new wxIntPropertyExt("Visible man count",wxT(""),&m_unit->vis,0,5));
	
	grp = pgPropGrp->Append(new wxPropertyCategory("Light unit attack:"));
	pgPropGrp->AppendIn(grp,new wxBoolPropertyExt("Visible projectile",wxT(""),&m_unit->projectile_visible,SpellUnitRec::PROJECTILE_LIGHT));
	pgPropGrp->AppendIn(grp,new wxEnumPropertyExt("Attack animation",wxT("anim_light"),MapToPGenumChoices(grp_res_list),&m_unit->anim_atack_light_name));
	pgPropGrp->AppendIn(grp,new wxIntPropertyExt("Animation frames limit",wxT("anim_light_frames"),&m_unit->anim_atack_light_frames,0,255));
	pgPropGrp->AppendIn(grp,new wxEnumPropertyExt("Shot PNM",wxT(""),MapToPGenumChoices(pnm_names),&m_unit->pnm_light_shot_name));
	pgPropGrp->AppendIn(grp,new wxEnumPropertyExt("Target hit PNM",wxT(""),MapToPGenumChoices(pnm_names),&m_unit->pnm_light_hit_name));

	grp = pgPropGrp->Append(new wxPropertyCategory("Armored unit attack:"));
	pgPropGrp->AppendIn(grp,new wxBoolPropertyExt("Visible projectile",wxT(""),&m_unit->projectile_visible,SpellUnitRec::PROJECTILE_ARMOR));
	pgPropGrp->AppendIn(grp,new wxEnumPropertyExt("Attack animation",wxT("anim_armored"),MapToPGenumChoices(grp_res_list),&m_unit->anim_atack_armor_name));
	pgPropGrp->AppendIn(grp,new wxIntPropertyExt("Animation frames limit",wxT("anim_armored_frames"),&m_unit->anim_atack_armor_frames,0,255));
	pgPropGrp->AppendIn(grp,new wxEnumPropertyExt("Shot PNM",wxT(""),MapToPGenumChoices(pnm_names),&m_unit->pnm_armored_shot_name));
	pgPropGrp->AppendIn(grp,new wxEnumPropertyExt("Target hit PNM",wxT(""),MapToPGenumChoices(pnm_names),&m_unit->pnm_armored_hit_name));

	grp = pgPropGrp->Append(new wxPropertyCategory("Air unit attack:"));
	pgPropGrp->AppendIn(grp,new wxBoolPropertyExt("Visible projectile",wxT(""),&m_unit->projectile_visible,SpellUnitRec::PROJECTILE_AIR));
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






// Export a complete, standalone COMMON.FS mod copy with the edited DEF inside.
// The original archive is never modified; other files remain byte-for-byte.
void FormUnitsEdit::OnExportUnitMod(wxCommandEvent& event)
{
	if(!m_units || !m_spell_data || m_spell_data->common_fs_source_path.empty())
	{
		wxMessageBox("Open a working JEDNOTKY.DEF and load game assets first.",
			"Export unit mod",wxOK|wxICON_INFORMATION,this);
		return;
	}
	if(m_spell_data->units)
	{
		for(const auto& rec : m_units->GetUnits())
		{
			if(rec && rec->type_id >= m_spell_data->units->Count() && rec->icon.empty())
			{
				wxMessageBox("Some new units have no icon. Generate a custom icon from artwork "
					"before exporting them.", "Export unit mod",wxOK|wxICON_EXCLAMATION,this);
				return;
			}
		}
	}
	std::vector<uint8_t> data;
	if(m_units->GenerateDEF(data))
	{
		wxMessageBox("Cannot serialize unit definitions. Verify graphics names and unit attributes.",
			"Export unit mod",wxOK|wxICON_ERROR,this);
		return;
	}
	wxDirDialog saveDialog(this,"Select mod output folder (COMMON.FS + optionally UNITS.FSU)",
		m_spell_data->export_path,wxDD_DIR_MUST_EXIST);
	if(saveDialog.ShowModal() != wxID_OK)
		return;
	auto dir = std::filesystem::path(saveDialog.GetPath().ToStdWstring());
	auto dest = dir / "COMMON.FS";
	const auto infoDest=dir / "INFO.FS";
	if(std::filesystem::exists(dest) ||
		(!m_custom_info_files.empty() && std::filesystem::exists(infoDest)))
	{
		wxMessageBox("COMMON.FS already exists at destination. Select a new mod folder.\n"
			"The original and existing mods will never be overwritten automatically.",
			"Export unit mod",wxOK|wxICON_EXCLAMATION,this);
		return;
	}
	try
	{
		FSarchive base(m_spell_data->common_fs_source_path.wstring(),FSarchive::Options::NONE);
		if(base.AddFile("JEDNOTKY.DEF",data,true))
			throw std::runtime_error(base.m_last_error);
		// I_*.LZ unit icons must travel with their JEDNOTKY.DEF references.
		for(auto& [name,compressed] : m_custom_icon_lz)
		{
			if(base.AddFile(name,compressed,false))
				throw std::runtime_error("Cannot insert new unit icon "+name+": "+base.m_last_error);
		}
		if(!m_custom_info_files.empty())
		{
			if(m_spell_data->info_fs_source_path.empty())
				throw std::runtime_error("INFO.FS source archive is not configured.");
			FSarchive infoBase(m_spell_data->info_fs_source_path.wstring(),FSarchive::Options::NONE);
			for(auto& [name,bytes] : m_custom_info_files)
				if(infoBase.AddFile(name,bytes,false))
					throw std::runtime_error("Cannot append "+name+" to INFO.FS: "+infoBase.m_last_error);
			if(infoBase.SaveFile(infoDest.wstring()))
				throw std::runtime_error("Cannot export INFO.FS: "+infoBase.m_last_error);
		}
		if(base.SaveFile(dest.wstring()))
			throw std::runtime_error(base.m_last_error);
		m_spell_data->export_path = dir.wstring();

		wxMessageBox("Exported mod COMMON.FS with new icons and JEDNOTKY.DEF:\n"
			+ wxString(dest.wstring()) +
			"\n\nNew portraits, if generated, are in INFO.FS alongside it.\n"
			"Keep UNITS.FSU in the same mod folder.\n"
			"Load this mod folder through the editor's game configuration "
			"or Reloaded before launching a game test.",
			"Export unit mod",wxOK|wxICON_INFORMATION,this);
	}
	catch(const std::exception& error)
	{
		wxMessageBox("Unable to build COMMON.FS:\n" +
			wxString::FromUTF8(error.what()),"Export unit mod",wxOK|wxICON_ERROR,this);
	}
}


// Spellcross ground sprites do not have to fit inside a 48-pixel-tall
// rectangle. A map cell is 80px wide, but people/vehicles stand ABOVE its
// ground plane. Match a ground template's visible dimensions if available.
struct GroundSpriteSize {
	int width;
	int height;
};
static GroundSpriteSize GetGroundSpriteSize(const FSU_resource* reference,
	bool walker, int sizeLimit, int scalePercent)
{
	int refWidth=0, refHeight=0;
	if(reference)
	{
		auto measure=[&](const FSU_sprite* sprite)
		{
			if(!sprite || sprite->y_size<=0 || sprite->x_max<=sprite->x_min)
				return;
			refWidth=std::max(refWidth,sprite->x_max-sprite->x_min);
			refHeight=std::max(refHeight,sprite->y_size);
		};
		// First slope or first animation frame across the available azimuths.
		if(!reference->stat.lists[0].empty())
			for(auto* sprite:reference->stat.lists[0])
				measure(sprite);
		else
			for(const auto& az:reference->anim.lists)
				if(!az.empty())
					measure(az[0]);
	}
	// The reference dimensions are the unscaled footprint at 100%.
	// "Max object size" limits that BASELINE; the user-visible percentage
	// must then multiply the baseline, not get silently clipped back to 80.
	// Original FSU row offsets are SIGNED 16-bit when decoded (LoadResource),
	// so sprites may extend left of the tile's x=40 origin. SaveSprite stores
	// that signed offset as an identical 16-bit two's-complement bit pattern.
	const int baseWidth=refWidth>0?refWidth:(walker?34:72);
	const int baseHeight=refHeight>0?refHeight:56;
	const double fit=std::min({1.0,
		static_cast<double>(sizeLimit)/baseWidth,
		static_cast<double>(sizeLimit)/baseHeight});
	const double scale=std::clamp(scalePercent,50,200)/100.0;
	// Avoid excessive FSU sprite dimensions and huge palette archives.
	// The physical model may overlap neighboring tiles, as existing large
	// vehicle sprites do; never claim a tile-width clamp in the UI.
	const int width=std::clamp(static_cast<int>(std::lround(baseWidth*fit*scale)),1,320);
	const int height=std::clamp(static_cast<int>(std::lround(baseHeight*fit*scale)),1,320);
	return {width,height};
}

// Match original ground FSU offsets, not the sprite's invisible canvas.
// The map draws non-walking units at tile.y_ofs and FSU at (y_ofs-128).
// On a flat 80px diamond the terrain CENTER is roughly 32px below that
// origin, so a hardcoded bottom at 127 made imported vehicles hover.
static const FSU_sprite* GroundReferenceSprite(const FSU_resource* reference,
	int slope, int direction, bool walker)
{
	if(!reference || reference->stat.azimuths<=0)
		return nullptr;
	const int useSlope=(!walker && reference->stat.slopes>=13)?slope:0;
	const auto& row=reference->stat.lists[useSlope];
	if(row.empty())
		return nullptr;
	const size_t index=static_cast<size_t>(
		std::lround(static_cast<double>(direction)*row.size()/16.0))%row.size();
	return row[index];
}
static int GroundFootOffset(const wxImage& sprite, const FSU_resource* reference,
	int slope, int direction, bool walker, int adjustment)
{
	if(!sprite.IsOk())
		throw std::runtime_error("Invalid ground sprite.");
	const int width=sprite.GetWidth(), height=sprite.GetHeight();
	int bottom=-1;
	if(sprite.HasAlpha())
	{
		const auto* alpha=sprite.GetAlpha();
		for(int y=0;y<height;++y)
			for(int x=0;x<width;++x)
				if(alpha[static_cast<size_t>(y)*width+x]>=128)
					bottom=y;
	}
	else
		bottom=height-1;
	if(bottom<0)
		throw std::runtime_error("Ground sprite has no opaque pixels after scaling.");

	// Template anchor includes slope-specific body height and terrain contact.
	// Use it only for complete original ground resources, not for the
	// incomplete 8-heading custom resources from older editor revisions.
	int referenceBottom=160;
	const auto* original=GroundReferenceSprite(reference,slope,direction,walker);
	const bool validReference=reference && reference->stat.azimuths>=16 &&
		(walker ? reference->anim.frames>1 : reference->stat.slopes>=13);
	if(validReference && original && original->y_size>0)
		referenceBottom=original->y_ofs+original->y_size-1;
	return referenceBottom-bottom+adjustment;
}

// Original tracked/wheeled vehicle FSU resources use 13 terrain slopes
// (A..M) and 16 azimuths. Generate slope-dependent vertical shearing
// from Spellcross's own tile-vertex height patterns. These are constrained
// preview/FSU poses, not a substitute for actual suspension animation.
static wxImage AdaptGroundFrameToSlope(const wxImage& source,int slope)
{
	if(slope==0)
		return source.Copy();
	static constexpr int heights[13][4]={
		{0,0,0,0},{1,1,1,0},{0,1,0,0},{0,1,1,0},
		{0,0,1,0},{0,1,1,1},{0,0,1,1},{0,0,0,1},
		{1,0,1,1},{1,0,0,1},{1,0,0,0},{1,1,0,1},
		{1,1,0,0}};
	if(slope<0 || slope>=13 || !source.IsOk() || !source.HasAlpha())
		throw std::runtime_error("Invalid slope sprite conversion.");
	const int w=source.GetWidth(),h=source.GetHeight();
	wxImage result(w,h);
	result.InitAlpha();
	std::memset(result.GetData(),0,static_cast<size_t>(w)*h*3);
	std::memset(result.GetAlpha(),0,static_cast<size_t>(w)*h);
	for(int y=0;y<h;++y)
		for(int x=0;x<w;++x)
		{
			// Side-to-side roll; front-to-back slope subtly changes pitch.
			const double localX=(x-(w-1)/2.0)/40.0;
			const double localY=(y-(h-1)/2.0)/48.0;
			const double dy=5.0*(heights[slope][0]-heights[slope][2])*localX+
				3.0*(heights[slope][1]-heights[slope][3])*localY;
			const int sy=static_cast<int>(std::lround(y+dy));
			if(sy<0 || sy>=h)continue;
			const size_t dst=static_cast<size_t>(y)*w+x;
			const size_t src=static_cast<size_t>(sy)*w+x;
			std::memcpy(result.GetData()+3*dst,source.GetData()+3*src,3);
			result.GetAlpha()[dst]=source.GetAlpha()[src];
		}
	return result;
}

// Produce genuine small, palettizable Spellcross graphics from large artwork.
// For top-down subjects (e.g. aircraft), rotating the cut-out generates a
// usable static eight-azimuth preview. It is not 3D reconstruction.
static std::vector<wxImage> ConvertArtworkToUnitFrames(wxImage image,
	int targetSize, bool eraseBackground, int tolerance, bool eightDirections,
	bool groundUnit, bool groundWalker, const FSU_resource* reference,
	int sourceHeading, int groundScalePercent)
{
	if(!image.IsOk() || image.GetWidth() < 2 || image.GetHeight() < 2)
		throw std::runtime_error("Invalid source image.");

	const int width = image.GetWidth();
	const int height = image.GetHeight();
	const size_t count = static_cast<size_t>(width)*height;
	bool hasRealTransparency = false;
	if(image.HasAlpha())
	{
		auto alpha = image.GetAlpha();
		for(size_t i=0;i<count;++i)
			if(alpha[i] < 240)
			{
				hasRealTransparency = true;
				break;
			}
	}
	else
		image.InitAlpha();

	auto* rgb = image.GetData();
	auto* alpha = image.GetAlpha();
	if(!rgb || !alpha)
		throw std::runtime_error("Could not access image pixels.");

	if(eraseBackground && !hasRealTransparency)
	{
		// Erase only contiguous pixels connected to the outer border. Never
		// erase similar colors inside the subject (e.g. pale airplane wings).
		std::array<size_t,4> corners = {0,static_cast<size_t>(width-1),
			static_cast<size_t>(height-1)*width,count-1};
		std::array<int,3> background = {0,0,0};
		for(size_t k : corners)
			for(int ch=0;ch<3;++ch)
				background[ch] += rgb[3*k+ch];
		for(int ch=0;ch<3;++ch)
			background[ch] /= 4;
		const int threshold2 = 3*tolerance*tolerance;
		for(size_t k : corners)
		{
			int distance2=0;
			for(int ch=0;ch<3;++ch)
			{
				int d = static_cast<int>(rgb[3*k+ch])-background[ch];
				distance2 += d*d;
			}
			if(distance2 > threshold2*4)
				throw std::runtime_error(
					"Background is not uniform at the image corners. "
					"Use a transparent PNG, or disable automatic background removal.");
		}
		std::vector<uint8_t> visited(count,0);
		std::deque<size_t> queue;
		auto add = [&](size_t k)
		{
			if(visited[k])
				return;
			visited[k] = 1;
			int distance2=0;
			for(int ch=0;ch<3;++ch)
			{
				const int d=static_cast<int>(rgb[3*k+ch])-background[ch];
				distance2+=d*d;
			}
			if(distance2 <= threshold2)
				queue.push_back(k);
		};
		for(int x=0;x<width;++x)
		{
			add(static_cast<size_t>(x));
			add(static_cast<size_t>(height-1)*width+x);
		}
		for(int y=0;y<height;++y)
		{
			add(static_cast<size_t>(y)*width);
			add(static_cast<size_t>(y)*width+width-1);
		}
		while(!queue.empty())
		{
			size_t k=queue.front();
			queue.pop_front();
			alpha[k]=0;
			int x=static_cast<int>(k%width);
			int y=static_cast<int>(k/width);
			if(x>0) add(k-1);
			if(x+1<width) add(k+1);
			if(y>0) add(k-width);
			if(y+1<height) add(k+width);
		}
	}

	int xmin=width, ymin=height, xmax=-1, ymax=-1;
	for(int y=0;y<height;++y)
		for(int x=0;x<width;++x)
			if(alpha[static_cast<size_t>(y)*width+x] >= 24)
			{
				xmin=std::min(xmin,x);
				ymin=std::min(ymin,y);
				xmax=std::max(xmax,x);
				ymax=std::max(ymax,y);
			}
	if(xmax<0)
		throw std::runtime_error("No visible object remains after background removal.");

	// Retain a small transparent border to prevent clipping during filtering.
	const int padding = std::max(2,std::max(xmax-xmin,ymax-ymin)/100);
	xmin=std::max(0,xmin-padding);
	ymin=std::max(0,ymin-padding);
	xmax=std::min(width-1,xmax+padding);
	ymax=std::min(height-1,ymax+padding);
	image=image.GetSubImage(wxRect(xmin,ymin,xmax-xmin+1,ymax-ymin+1));

	// Ground sprites must fit the map cell horizontally, but upright figures
	// and tanks can extend ABOVE the cell; use the cloned unit's dimensions.
	const auto dims=GetGroundSpriteSize(reference,groundWalker,targetSize,groundScalePercent);
	const int canvasW=groundUnit?std::max(80,dims.width+12)
		:static_cast<int>(std::ceil(targetSize*1.5));
	const int canvasH=groundUnit?std::max(96,dims.height+12)
		:static_cast<int>(std::ceil(targetSize*1.5));
	// Ground controls must not shrink the artwork back to 56px after scaling.
	const int imageMaxW=groundUnit?dims.width:targetSize;
	const int imageMaxH=groundUnit?dims.height:targetSize;
	const double scale=std::min({
		static_cast<double>(imageMaxW)/image.GetWidth(),
		static_cast<double>(imageMaxH)/image.GetHeight(),
		groundUnit?2.0:1.0});
	const int newWidth=std::max(1,static_cast<int>(std::lround(image.GetWidth()*scale)));
	const int newHeight=std::max(1,static_cast<int>(std::lround(image.GetHeight()*scale)));
	if(newWidth!=image.GetWidth() || newHeight!=image.GetHeight())
		image.Rescale(newWidth,newHeight,wxIMAGE_QUALITY_BICUBIC);

	const int variants=eightDirections?8:1;
	std::vector<wxImage> frames;
	frames.reserve(variants);

	// Measure the silhouettes of the cloned unit's real FSU azimuths. Those
	// images already use Spellcross's viewing angle / reference origin.
	// We cannot reconstruct unseen 3D surfaces from one picture, but
	// matching its directional footprint avoids a simple spinning cut-out.
	std::array<double,8> widthFactor, heightFactor;
	std::array<double,8> offsetX, offsetY;
	widthFactor.fill(1.0);
	heightFactor.fill(1.0);
	offsetX.fill(0.0);
	offsetY.fill(0.0);
	std::array<const FSU_sprite*,8> sample{};
	if(reference)
	{
		for(int heading=0;heading<8;++heading)
		{
			if(reference->stat.azimuths>0 && !reference->stat.lists[0].empty())
			{
				const auto& stat=reference->stat.lists[0];
				sample[heading]=stat[std::min(static_cast<size_t>(
					std::lround(heading*reference->stat.azimuths/8.0))%stat.size(),stat.size()-1)];
			}
			else if(reference->anim.azimuths>0 && !reference->anim.lists.empty())
			{
				const auto index=static_cast<size_t>(
					std::lround(heading*reference->anim.azimuths/8.0))%reference->anim.lists.size();
				if(!reference->anim.lists[index].empty())
					sample[heading]=reference->anim.lists[index][0];
			}
		}
		const auto baseline=sample[0];
		if(baseline && baseline->x_max>baseline->x_min && baseline->y_size>0)
		{
			const double refW=baseline->x_max-baseline->x_min;
			const double refH=baseline->y_size;
			const double cx0=(baseline->x_min+baseline->x_max)/2.0;
			const double cy0=baseline->y_ofs+baseline->y_size/2.0;
			for(int heading=0;heading<8;++heading)
			{
				const auto* sprite=sample[heading];
				if(!sprite || sprite->x_max<=sprite->x_min || sprite->y_size<=0)
					continue;
				widthFactor[heading]=std::clamp((sprite->x_max-sprite->x_min)/refW,0.55,1.35);
				heightFactor[heading]=std::clamp(sprite->y_size/refH,0.55,1.35);
				offsetX[heading]=std::clamp((sprite->x_min+sprite->x_max)/2.0-cx0,-16.0,16.0);
				offsetY[heading]=std::clamp(sprite->y_ofs+sprite->y_size/2.0-cy0,-12.0,12.0);
			}
		}
	}

	for(int heading=0;heading<variants;++heading)
	{
		wxImage frame(canvasW,canvasH);
		frame.InitAlpha();
		std::memset(frame.GetData(),0,static_cast<size_t>(canvasW)*canvasH*3);
		std::memset(frame.GetAlpha(),0,static_cast<size_t>(canvasW)*canvasH);
		// FSU azimuth order follows the original unit; sourceHeading defines
		// which direction the source image currently points to.
		const int az=(heading-sourceHeading+8)%8;
		const double angle=(eightDirections?az:0)*3.14159265358979323846/4.0;
		const double sn=std::sin(angle), cs=std::cos(angle);
		const double cx=(image.GetWidth()-1)/2.0;
		const double cy=(image.GetHeight()-1)/2.0;
		const double warpX=widthFactor[heading], warpY=heightFactor[heading];
		const double shiftX=offsetX[heading];
		const double shiftY=offsetY[heading];
		for(int y=0;y<canvasH;++y)
		{
			const double dy=(y-(canvasH-1)/2.0-shiftY)/warpY;
			for(int x=0;x<canvasW;++x)
			{
				const double dx=(x-(canvasW-1)/2.0-shiftX)/warpX;
				const int sx=static_cast<int>(std::lround(cx+dx*cs+dy*sn));
				const int sy=static_cast<int>(std::lround(cy-dx*sn+dy*cs));
				if(sx<0 || sy<0 || sx>=image.GetWidth() || sy>=image.GetHeight())
					continue;
				const size_t src=static_cast<size_t>(sy)*image.GetWidth()+sx;
				const size_t dst=static_cast<size_t>(y)*canvasW+x;
				std::memcpy(frame.GetData()+3*dst,image.GetData()+3*src,3);
				frame.GetAlpha()[dst]=image.GetAlpha()[src];
			}
		}
		frames.push_back(std::move(frame));
	}
	return frames;
}

// Render actual model geometry rather than rotating a flattened PNG.
// Blender runs outside the editor: no extra 3D library or engine dependency.
static std::vector<wxImage> RenderModelWithBlender(wxWindow* parent,
	const std::filesystem::path& model, int views, int elevation, int heading,
	int lighting, std::vector<wxImage>* walkingFrames)
{
	static wxString blenderExecutable;
	if(blenderExecutable.empty())
	{
		blenderExecutable=wxGetenv("SPELLCROSS_BLENDER_EXE");
		if(blenderExecutable.empty())
			if(auto* cfg=wxConfigBase::Get(false))
				cfg->Read("CustomUnits/BlenderExecutable",&blenderExecutable);
	}
	if(blenderExecutable.empty() ||
		!std::filesystem::is_regular_file(std::filesystem::path(blenderExecutable.ToStdWstring())))
	{
		wxFileDialog browse(parent,"Locate Blender executable (install Blender first)",
			wxEmptyString,"blender.exe",
			"Blender application (blender.exe)|blender.exe|Executables (*.exe)|*.exe",
			wxFD_OPEN|wxFD_FILE_MUST_EXIST);
		if(browse.ShowModal()!=wxID_OK)
			throw std::runtime_error("Blender executable was not selected.");
		blenderExecutable=browse.GetPath();
		if(auto* cfg=wxConfigBase::Get(false))
			cfg->Write("CustomUnits/BlenderExecutable",blenderExecutable);
	}
	const auto exePath=std::filesystem::path(wxStandardPaths::Get().GetExecutablePath().ToStdWstring());
	const auto exeDir=exePath.parent_path();
	const std::vector<std::filesystem::path> scriptCandidates={
		exeDir/"render_unit_model.py",
		exeDir/"../../../source/tools/render_unit_model.py",
		std::filesystem::current_path()/"source/tools/render_unit_model.py"
	};
	std::filesystem::path script;
	for(const auto& candidate : scriptCandidates)
		if(std::filesystem::is_regular_file(candidate))
		{
			script=candidate;
			break;
		}
	if(script.empty())
		throw std::runtime_error(
			"Cannot find render_unit_model.py. Keep the script next to the editor "
			"executable, or run the editor from the source checkout.");

	// Unique scratch directory. Never write render results over a user's model
	// or over a pre-existing mod folder.
	const wxString tempName=wxFileName::CreateTempFileName("spellcross_model_");
	if(tempName.empty())
		throw std::runtime_error("Cannot create a temporary render directory.");
	const auto tempDir=std::filesystem::path(tempName.ToStdWstring());
	std::filesystem::remove(tempDir);
	std::filesystem::create_directories(tempDir);
	struct TempDirCleanup{
		std::filesystem::path value;
		~TempDirCleanup(){std::error_code ec; std::filesystem::remove_all(value,ec);}
	} cleanup{tempDir};

	const auto quoted=[](const std::filesystem::path& p)
	{
		return wxString("\"")+wxString(p.wstring())+"\"";
	};
	const auto program=std::filesystem::path(blenderExecutable.ToStdWstring());
	wxString cmd=quoted(program)+
		" --background --factory-startup --python "+quoted(script)+
		" -- --input "+quoted(model)+" --output "+quoted(tempDir)+
		wxString::Format(" --views %d --resolution 1024 --elevation %d --heading %d --lighting %d --frame 1 --walk-frames %d",
			views,elevation,heading*45,lighting,walkingFrames?10:0);
	wxArrayString stdOut,stdErr;
	long result=0;
	{
		wxBusyCursor waiting;
		wxBusyInfo busy("Rendering model in Blender. Generating real 3D camera views...",parent);
		result=wxExecute(cmd,stdOut,stdErr,wxEXEC_SYNC|wxEXEC_HIDE_CONSOLE);
	}
	if(result!=0)
	{
		wxString detail;
		for(size_t k=(stdErr.size()>8?stdErr.size()-8:0);k<stdErr.size();++k)
			detail+=stdErr[k]+"\n";
		if(detail.empty())
			for(size_t k=(stdOut.size()>5?stdOut.size()-5:0);k<stdOut.size();++k)
				detail+=stdOut[k]+"\n";
		throw std::runtime_error(
			("Blender render failed (exit "+std::to_string(result)+").\n"+
			detail.ToStdString()));
	}
	std::vector<wxImage> images;
	for(int i=0;i<views;++i)
	{
		auto png=tempDir/wxString::Format("view_%02d.png",i).ToStdString();
		wxImage frame;
		if(!frame.LoadFile(wxString(png.wstring()),wxBITMAP_TYPE_PNG) || !frame.HasAlpha())
			throw std::runtime_error("Blender did not produce a transparent RGBA frame: "+png.string());
		images.push_back(std::move(frame));
	}
	if(walkingFrames)
	{
		// Static models do not contain a Blender Action; use still poses as
		// a safe fallback. When an animation exists, Blender exports all 80
		// movement images and the editor preserves those distinct frames.
		const auto first=tempDir/"move_00_00.png";
		if(std::filesystem::exists(first))
			for(int pose=0;pose<10;++pose)
				for(int direction=0;direction<8;++direction)
				{
					const auto png=tempDir/wxString::Format("move_%02d_%02d.png",
						pose,direction).ToStdString();
					wxImage image;
					if(!image.LoadFile(wxString(png.wstring()),wxBITMAP_TYPE_PNG) ||
						!image.HasAlpha())
						throw std::runtime_error("Incomplete Blender movement pose export: "+
							png.string());
					walkingFrames->push_back(std::move(image));
				}
	}
	return images;
}

// Maintain one common crop and scale for ALL 3D azimuths.
// Per-frame auto-crop would make the aircraft grow/shrink as it turns.
static std::vector<wxImage> FitRenderedModelFrames(const std::vector<wxImage>& raw,
	int targetSize, bool groundUnit, bool groundWalker,
	const FSU_resource* reference, int groundScalePercent)
{
	if(raw.empty() || !raw[0].IsOk() || !raw[0].HasAlpha())
		throw std::runtime_error("The model renderer produced no usable frames.");
	const int sourceW=raw[0].GetWidth(),sourceH=raw[0].GetHeight();
	int xMin=sourceW,yMin=sourceH,xMax=-1,yMax=-1;
	for(const auto& image:raw)
	{
		if(!image.IsOk() || !image.HasAlpha() ||
			image.GetWidth()!=sourceW || image.GetHeight()!=sourceH)
			throw std::runtime_error("Rendered model frames have incompatible sizes.");
		const uint8_t* a=image.GetAlpha();
		for(int y=0;y<sourceH;++y)
			for(int x=0;x<sourceW;++x)
				if(a[static_cast<size_t>(y)*sourceW+x]>=16)
				{
					xMin=std::min(xMin,x); yMin=std::min(yMin,y);
					xMax=std::max(xMax,x); yMax=std::max(yMax,y);
				}
	}
	if(xMax<xMin || yMax<yMin)
		throw std::runtime_error("The model renders are entirely transparent.");
	const int margin=3;
	xMin=std::max(0,xMin-margin);
	yMin=std::max(0,yMin-margin);
	xMax=std::min(sourceW-1,xMax+margin);
	yMax=std::min(sourceH-1,yMax+margin);
	const int cropW=xMax-xMin+1, cropH=yMax-yMin+1;
	const auto groundDimensions=GetGroundSpriteSize(reference,groundWalker,
		targetSize,groundScalePercent);
	const int maxW=groundUnit?groundDimensions.width:targetSize;
	const int maxH=groundUnit?groundDimensions.height:targetSize;
	const double scale=std::min({static_cast<double>(maxW)/cropW,
		static_cast<double>(maxH)/cropH,1.0});
	const int outW=std::max(1,static_cast<int>(std::round(cropW*scale)));
	const int outH=std::max(1,static_cast<int>(std::round(cropH*scale)));
	const int canvasW=groundUnit?std::max(80,groundDimensions.width+12)
		:static_cast<int>(std::ceil(targetSize*1.5));
	const int canvasH=groundUnit?std::max(96,groundDimensions.height+8)
		:static_cast<int>(std::ceil(targetSize*1.5));
	std::vector<wxImage> result;
	result.reserve(raw.size());
	for(const auto& source:raw)
	{
		auto img=source.GetSubImage(wxRect(xMin,yMin,cropW,cropH));
		if(img.GetWidth()!=outW || img.GetHeight()!=outH)
			img.Rescale(outW,outH,wxIMAGE_QUALITY_BICUBIC);
		wxImage frame(canvasW,canvasH);
		frame.InitAlpha();
		std::memset(frame.GetData(),0,static_cast<size_t>(canvasW)*canvasH*3);
		std::memset(frame.GetAlpha(),0,static_cast<size_t>(canvasW)*canvasH);
		// Keep the visible model near the ground line in the preview.
		// The exact game attachment is based on the opaque pixel row below.
		const int ox=(canvasW-outW)/2;
		const int oy=groundUnit?canvasH-outH-2:(canvasH-outH)/2;
		for(int y=0;y<outH;++y)
			for(int x=0;x<outW;++x)
			{
				const size_t sp=static_cast<size_t>(y)*outW+x;
				const size_t dp=static_cast<size_t>(y+oy)*canvasW+x+ox;
				std::memcpy(frame.GetData()+3*dp,img.GetData()+3*sp,3);
				frame.GetAlpha()[dp]=img.GetAlpha()[sp];
			}
		result.push_back(std::move(frame));
	}
	return result;
}

// Lighten dark midtones and recover edges lost when downsampling a large
// model render into a 40-100px game sprite. RGB only: preserve exact alpha.
// Work after resizing but before converting to the fixed Spellcross palette.
static void EnhanceGameSprite(wxImage& image)
{
	if(!image.IsOk())
		return;
	const int w=image.GetWidth(),h=image.GetHeight();
	if(w<=0 || h<=0)
		return;
	const size_t pixels=static_cast<size_t>(w)*h;
	auto* rgb=image.GetData();
	const auto* alpha=image.HasAlpha()?image.GetAlpha():nullptr;
	if(!rgb)
		return;
	std::vector<uint8_t> tonemapped(pixels*3);
	auto byteClamp=[](double n)->uint8_t
	{
		return static_cast<uint8_t>(std::clamp(static_cast<int>(std::lround(n)),0,255));
	};

	// Subtle shadow-only correction. Leave highlights unchanged, so a model
	// with white textures does not get bleached out before palette conversion.
	for(size_t p=0;p<pixels;++p)
	{
		if(alpha && alpha[p]<16)
		{
			std::copy_n(rgb+3*p,3,tonemapped.data()+3*p);
			continue;
		}
		double c[3];
		for(int k=0;k<3;++k)
		{
			const double value=rgb[3*p+k]/255.0;
			// Correct only deep shadows, with zero lift at and above 0.45.
			const double shadow=std::max(0.0,0.45-value)/0.45;
			c[k]=std::clamp(value+0.025*shadow*shadow,0.0,1.0);
		}
		const double gray=0.2126*c[0]+0.7152*c[1]+0.0722*c[2];
		for(int k=0;k<3;++k)
			tonemapped[3*p+k]=byteClamp(
				std::clamp(gray+(c[k]-gray)*1.02,0.0,1.0)*255.0);
	}

	// Alpha-aware 3x3 Gaussian unsharp mask. The transparent canvas must not
	// darken edge pixels, and alpha itself must NEVER be sharpened.
	constexpr int weight[3]={1,2,1};
	for(int y=0;y<h;++y)
		for(int x=0;x<w;++x)
		{
			const size_t p=static_cast<size_t>(y)*w+x;
			if(alpha && alpha[p]<200)
			{
				std::copy_n(tonemapped.data()+3*p,3,rgb+3*p);
				continue;
			}
			double sums[3]={0.0,0.0,0.0};
			double weightSum=0.0;
			for(int dy=-1;dy<=1;++dy)
			{
				const int yy=y+dy;
				if(yy<0 || yy>=h)continue;
				for(int dx=-1;dx<=1;++dx)
				{
					const int xx=x+dx;
					if(xx<0 || xx>=w)continue;
					const size_t q=static_cast<size_t>(yy)*w+xx;
					const double coverage=alpha?alpha[q]/255.0:1.0;
					const double factor=weight[dx+1]*weight[dy+1]*coverage;
					if(factor<=0.0)continue;
					for(int k=0;k<3;++k)
						sums[k]+=tonemapped[3*q+k]*factor;
					weightSum+=factor;
				}
			}
			for(int k=0;k<3;++k)
			{
				const double c=tonemapped[3*p+k];
				const double blur=weightSum>0.0?sums[k]/weightSum:c;
				rgb[3*p+k]=byteClamp(c+0.18*(c-blur));
			}
		}
}

// High resolution illustration/photo -> cutout, 96px-scale, optional 8
// headings -> original UNITS.PAL quantization -> FSU -> immediately selectable
// graphics group for the currently edited unit.
void FormUnitsEdit::OnGenerateGameSprites(wxCommandEvent& event)
{
	wxFileDialog chooseArt(this,"Select 3D unit model or legacy PNG",
		wxEmptyString,wxEmptyString,
		"3D models (*.glb;*.gltf;*.fbx)|*.glb;*.gltf;*.fbx|"
		"glTF binary (*.glb)|*.glb|glTF (*.gltf)|*.gltf|"
		"FBX (*.fbx)|*.fbx|Legacy PNG (*.png)|*.png",
		wxFD_OPEN|wxFD_FILE_MUST_EXIST);
	if(chooseArt.ShowModal()!=wxID_OK)
		return;
	GenerateGameSpritesFromSource(std::filesystem::path(chooseArt.GetPath().ToStdWstring()));
}

void FormUnitsEdit::GenerateGameSpritesFromSource(const std::filesystem::path& source)
{
	if(!m_unit || !m_spell_data || !m_spell_data->units_fsu)
	{
		wxMessageBox("Select your new unit in the DEF editor first.",
			"Create game sprites",wxOK|wxICON_INFORMATION,this);
		return;
	}
	std::string ext=source.extension().string();
	for(char& ch:ext)ch=static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
	const bool model3D=(ext==".glb" || ext==".gltf" || ext==".fbx");
	if(!model3D && ext!=".png")
	{
		wxMessageBox("Supported sources: .glb, .gltf, .fbx and legacy .png.",
			"Unit 3D model",wxOK|wxICON_EXCLAMATION,this);
		return;
	}
	wxDialog options(this,wxID_ANY,"Create Spellcross unit graphics",
		wxDefaultPosition,wxSize(520,400),wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
	auto layout=new wxBoxSizer(wxVERTICAL);
	layout->Add(new wxStaticText(&options,wxID_ANY,
		model3D ? "Render real 3D camera azimuths using Blender. Each heading is\n"
			"a different view of your actual model, not a rotated flat picture."
			: "Legacy PNG conversion: reference-guided flat rotations only.\n"
			"Use a 3D model for physically correct perspectives."),
		0,wxALL,12);
	auto grid=new wxFlexGridSizer(2,8,10);
	grid->AddGrowableCol(1,1);
	grid->Add(new wxStaticText(&options,wxID_ANY,"FSU graphics group:"),0,wxALIGN_CENTER_VERTICAL);
	auto groupField=new wxTextCtrl(&options,wxID_ANY,"U001");
	grid->Add(groupField,1,wxEXPAND);
	grid->Add(new wxStaticText(&options,wxID_ANY,"Map unit scale:"),0,wxALIGN_CENTER_VERTICAL);
	wxArrayString unitModes;
	unitModes.Add("Air - may extend beyond one tile");
	unitModes.Add("Ground - centered on tile (large sprites may overhang)");
	auto unitMode=new wxChoice(&options,wxID_ANY,wxDefaultPosition,wxDefaultSize,unitModes);
	unitMode->SetSelection(m_unit->isAir()?0:1);
	grid->Add(unitMode,1,wxEXPAND);
	grid->Add(new wxStaticText(&options,wxID_ANY,"Ground sprite format:"),0,wxALIGN_CENTER_VERTICAL);
	wxArrayString groundStyles;
	groundStyles.Add("Vehicle: 13 slopes x 16 directions (208 frames)");
	groundStyles.Add("Walker: 16 static + 10 x 8 movement (96 frames)");
	auto groundStyle=new wxChoice(&options,wxID_ANY,wxDefaultPosition,wxDefaultSize,groundStyles);
	groundStyle->SetSelection(m_unit->isWalk()?1:0);
	groundStyle->Enable(unitMode->GetSelection()==1);
	grid->Add(groundStyle,1,wxEXPAND);
	grid->Add(new wxStaticText(&options,wxID_ANY,"Reference size limit at 100% (px):"),0,wxALIGN_CENTER_VERTICAL);
	auto sizeField=new wxSpinCtrl(&options,wxID_ANY,"",wxDefaultPosition,wxDefaultSize,
		wxSP_ARROW_KEYS,32,160,m_unit->isAir()?96:144);
	grid->Add(sizeField,1,wxEXPAND);
	grid->Add(new wxStaticText(&options,wxID_ANY,"Ground size relative to template (%):"),
		0,wxALIGN_CENTER_VERTICAL);
	auto groundScaleField=new wxSpinCtrl(&options,wxID_ANY,"",wxDefaultPosition,
		wxDefaultSize,wxSP_ARROW_KEYS,50,200,125);
	groundScaleField->Enable(unitMode->GetSelection()==1);
	grid->Add(groundScaleField,1,wxEXPAND);
	grid->Add(new wxStaticText(&options,wxID_ANY,"Facing directions:"),0,wxALIGN_CENTER_VERTICAL);
	wxArrayString modes;
	modes.Add(model3D ? "Native 3D directions (8 air / 16 ground)" : "8 reference-guided 2D rotations");
	modes.Add(model3D ? "One 3D camera view" : "One fixed PNG view");
	auto rotationField=new wxChoice(&options,wxID_ANY,wxDefaultPosition,wxDefaultSize,modes);
	rotationField->SetSelection(0);
	grid->Add(rotationField,1,wxEXPAND);
	grid->Add(new wxStaticText(&options,wxID_ANY,"Model yaw / legacy PNG direction:"),0,wxALIGN_CENTER_VERTICAL);
	wxArrayString sourceHeadings;
	for(int k=0;k<8;++k)
		sourceHeadings.Add(wxString::Format("Template direction %d",k+1));
	auto sourceHeadingField=new wxChoice(&options,wxID_ANY,wxDefaultPosition,
		wxDefaultSize,sourceHeadings);
	sourceHeadingField->SetSelection(0);
	grid->Add(sourceHeadingField,1,wxEXPAND);
	grid->Add(new wxStaticText(&options,wxID_ANY,"3D camera elevation (degrees):"),0,wxALIGN_CENTER_VERTICAL);
	auto cameraElevation=new wxSpinCtrl(&options,wxID_ANY,"",wxDefaultPosition,
		wxDefaultSize,wxSP_ARROW_KEYS,0,90,m_unit->isAir()?65:35);
	cameraElevation->Enable(model3D);
	grid->Add(cameraElevation,1,wxEXPAND);
	grid->Add(new wxStaticText(&options,wxID_ANY,"Ground contact adjustment (px):"),0,wxALIGN_CENTER_VERTICAL);
	auto contactOffset=new wxSpinCtrl(&options,wxID_ANY,"",wxDefaultPosition,
		wxDefaultSize,wxSP_ARROW_KEYS,-24,24,0);
	contactOffset->Enable(unitMode->GetSelection()==1);
	grid->Add(contactOffset,1,wxEXPAND);
	unitMode->Bind(wxEVT_CHOICE,[&](wxCommandEvent&)
	{
		const bool groundSelected=unitMode->GetSelection()==1;
		groundStyle->Enable(groundSelected);
		groundScaleField->Enable(groundSelected);
		contactOffset->Enable(groundSelected);
	});
	grid->Add(new wxStaticText(&options,wxID_ANY,"3D lighting intensity (%):"),0,wxALIGN_CENTER_VERTICAL);
	auto lightingField=new wxSpinCtrl(&options,wxID_ANY,"",wxDefaultPosition,
		wxDefaultSize,wxSP_ARROW_KEYS,25,200,100);
	lightingField->Enable(model3D);
	grid->Add(lightingField,1,wxEXPAND);
	grid->Add(new wxStaticText(&options,wxID_ANY,"Background tolerance (PNG):"),0,wxALIGN_CENTER_VERTICAL);
	auto toleranceField=new wxSpinCtrl(&options,wxID_ANY,"",wxDefaultPosition,wxDefaultSize,
		wxSP_ARROW_KEYS,0,100,32);
	toleranceField->Enable(!model3D);
	grid->Add(toleranceField,1,wxEXPAND);
	layout->Add(grid,0,wxEXPAND|wxLEFT|wxRIGHT,12);
	auto backgroundField=new wxCheckBox(&options,wxID_ANY,
		"Remove plain background connected to image edges");
	backgroundField->SetValue(!model3D);
	backgroundField->Enable(!model3D);
	layout->Add(backgroundField,0,wxALL,12);
	auto enhanceField=new wxCheckBox(&options,wxID_ANY,
		"Enhance fine details (gentle, no strong brightness boost)");
	enhanceField->SetValue(true);
	layout->Add(enhanceField,0,wxLEFT|wxRIGHT|wxBOTTOM,12);
	layout->Add(new wxStaticText(&options,wxID_ANY,
		model3D ? "Blender renders transparent RGBA frames with a fixed orthographic\n"
			"camera scale. Your model and textures remain unchanged."
			: "Transparent source PNGs keep their transparency. The original\n"
			"artwork is never modified. A preview is shown before exporting."),
		0,wxLEFT|wxRIGHT|wxBOTTOM,12);
	auto buttons=new wxStdDialogButtonSizer();
	buttons->AddButton(new wxButton(&options,wxID_OK,"Preview sprites"));
	buttons->AddButton(new wxButton(&options,wxID_CANCEL,"Cancel"));
	buttons->Realize();
	layout->Add(buttons,0,wxALL|wxALIGN_RIGHT,12);
	options.SetSizerAndFit(layout);
	if(options.ShowModal()!=wxID_OK)
		return;

	auto group=groupField->GetValue().ToStdString();
	bool valid=!group.empty() && group.size()<=5;
	for(unsigned char c:group)
		valid=valid && ((c>='A'&&c<='Z')||(c>='0'&&c<='9'));
	if(!valid || m_spell_data->units_fsu->GetResource(group.c_str()))
	{
		wxMessageBox("Use a unique 1-5 character group name (A-Z or 0-9).",
			"Create game sprites",wxOK|wxICON_EXCLAMATION,this);
		return;
	}

	std::vector<wxImage> frames;
	std::vector<wxImage> walkingFrames;
	wxImage original; // also needed later for INFO.FS artwork generation
	try
	{
		if(model3D)
		{
			const bool animateWalk=unitMode->GetSelection()==1 &&
				groundStyle->GetSelection()==1 && rotationField->GetSelection()==0;
			std::vector<wxImage> walkingRaw;
			auto renders=RenderModelWithBlender(this,source,
				rotationField->GetSelection()==0?(unitMode->GetSelection()==1?16:8):1,
				cameraElevation->GetValue(),sourceHeadingField->GetSelection(),
				lightingField->GetValue(),animateWalk?&walkingRaw:nullptr);
			original=renders.front().Copy(); // info-portrait source: actual 3D render
			const auto staticCount=renders.size();
			// Quantize/downscale movement and standing frames with exactly one
			// scale/crop so an animated 3D unit does not change size mid-stride.
			for(auto& img:walkingRaw)
				renders.push_back(std::move(img));
			auto converted=FitRenderedModelFrames(renders,sizeField->GetValue(),
				unitMode->GetSelection()==1,groundStyle->GetSelection()==1,m_unit->gr_base,
				groundScaleField->GetValue());
			for(size_t i=0;i<converted.size();++i)
				if(i<staticCount)
					frames.push_back(std::move(converted[i]));
				else
					walkingFrames.push_back(std::move(converted[i]));
		}
		else
		{
			if(!original.LoadFile(wxString(source.wstring()),wxBITMAP_TYPE_PNG))
				throw std::runtime_error("Cannot load the selected source PNG.");
			frames=ConvertArtworkToUnitFrames(original,sizeField->GetValue(),
				backgroundField->GetValue(),toleranceField->GetValue(),
				rotationField->GetSelection()==0,unitMode->GetSelection()==1,
				groundStyle->GetSelection()==1,m_unit->gr_base,
				sourceHeadingField->GetSelection(),groundScaleField->GetValue());
		}
		// Improve readability for the low-resolution indexed game palette.
		// Applied to both 3D and legacy PNG input, not to the original file.
		if(enhanceField->GetValue())
		{
			for(auto& sprite:frames)
				EnhanceGameSprite(sprite);
			for(auto& sprite:walkingFrames)
				EnhanceGameSprite(sprite);
		}
		// The original game indexes 8 air headings, but 16 ground headings.
		// Legacy PNG generates only 8 angles; repeat each for both 22.5deg
		// ground azimuth slots, without changing the air-unit behavior.
		const int expected=unitMode->GetSelection()==1?16:8;
		if(frames.size()==1)
		{
			const auto one=frames.front().Copy();
			while(frames.size()<static_cast<size_t>(expected))
				frames.push_back(one.Copy());
		}
		else if(frames.size()==8 && expected==16)
		{
			std::vector<wxImage> expanded;
			expanded.reserve(16);
			for(const auto& image:frames)
			{
				expanded.push_back(image.Copy());
				expanded.push_back(image.Copy());
			}
			frames=std::move(expanded);
		}
		if(frames.size()!=static_cast<size_t>(expected))
			throw std::runtime_error("Unexpected directional frame count.");
	}
	catch(const std::exception& error)
	{
		wxMessageBox("Cannot prepare sprites:\n"+wxString::FromUTF8(error.what()),
			"Create game sprites",wxOK|wxICON_ERROR,this);
		return;
	}

	wxDialog preview(this,wxID_ANY,"Preview game-sized sprites",
		wxDefaultPosition,wxSize(420,400),wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
	auto previewLayout=new wxBoxSizer(wxVERTICAL);
	previewLayout->Add(new wxStaticText(&preview,wxID_ANY,
		"Preview before quantization to Spellcross's original palette:"),
		0,wxALL,10);
	auto spriteView=new wxStaticBitmap(&preview,wxID_ANY,wxBitmap(frames[0]));
	previewLayout->Add(spriteView,1,wxALL|wxALIGN_CENTER_HORIZONTAL,10);
	// Explicitly show the actual, opaque bitmap footprint. Canvas dimensions
	// include transparent padding, so those numbers alone are misleading.
	auto visibleSize=[](const wxImage& image)->wxString
	{
		const int width=image.GetWidth(),height=image.GetHeight();
		int left=width,top=height,right=-1,bottom=-1;
		const auto* alpha=image.HasAlpha()?image.GetAlpha():nullptr;
		for(int y=0;y<height;++y)
			for(int x=0;x<width;++x)
			{
				const size_t p=static_cast<size_t>(y)*width+x;
				if(alpha && alpha[p]<128)
					continue;
				left=std::min(left,x); top=std::min(top,y);
				right=std::max(right,x); bottom=std::max(bottom,y);
			}
		return wxString::Format("Visible object: %d x %d pixels (canvas %d x %d)",
			std::max(0,right-left+1),std::max(0,bottom-top+1),width,height);
	};
	auto visibleInfo=new wxStaticText(&preview,wxID_ANY,visibleSize(frames[0]));
	previewLayout->Add(visibleInfo,0,wxLEFT|wxRIGHT|wxBOTTOM,10);
	wxArrayString headings;
	for(size_t i=0;i<frames.size();++i)
		headings.Add(wxString::Format("Direction %d",static_cast<int>(i)+1));
	auto headingChoice=new wxChoice(&preview,wxID_ANY,wxDefaultPosition,
		wxDefaultSize,headings);
	headingChoice->SetSelection(0);
	headingChoice->Bind(wxEVT_CHOICE,[&](wxCommandEvent&)
	{
		const auto& sprite=frames[headingChoice->GetSelection()];
		spriteView->SetBitmap(wxBitmap(sprite));
		visibleInfo->SetLabel(visibleSize(sprite));
		preview.Layout();
	});
	previewLayout->Add(headingChoice,0,wxEXPAND|wxLEFT|wxRIGHT|wxBOTTOM,10);
	auto previewButtons=new wxStdDialogButtonSizer();
	previewButtons->AddButton(new wxButton(&preview,wxID_OK,"Build game graphics"));
	previewButtons->AddButton(new wxButton(&preview,wxID_CANCEL,"Cancel"));
	previewButtons->Realize();
	previewLayout->Add(previewButtons,0,wxALL|wxALIGN_RIGHT,10);
	preview.SetSizerAndFit(previewLayout);
	if(preview.ShowModal()!=wxID_OK)
		return;

	wxDirDialog chooseOutput(this,"Select output folder for generated sprites and mod UNITS.FSU",
		m_spell_data->export_path,wxDD_DIR_MUST_EXIST);
	if(chooseOutput.ShowModal()!=wxID_OK)
		return;
	const auto output=std::filesystem::path(chooseOutput.GetPath().ToStdWstring());
	const auto spritesDir=output/("spellcross_source_"+group);
	if(std::filesystem::exists(spritesDir) ||
		std::filesystem::exists(output/"UNITS.FSU") ||
		std::filesystem::exists(output/("NEW_"+group+".FSU")) ||
		std::filesystem::exists(output/("spellcross_fsu_"+group)))
	{
		wxMessageBox("The output already contains generated sprite files or UNITS.FSU.\n"
			"Choose an empty mod output folder; nothing will be overwritten.",
			"Create game sprites",wxOK|wxICON_EXCLAMATION,this);
		return;
	}
	try
	{
		std::filesystem::create_directories(spritesDir);
		auto* pal=m_spell_data->GetPalette("UNITS.PAL");
		if(!pal)
			throw std::runtime_error("Original UNITS.PAL is unavailable.");
		if(pal->SaveInfo(spritesDir/"spellcross_units.palinfo"))
			throw std::runtime_error("Could not export palette metadata.");

		const bool ground=unitMode->GetSelection()==1;
		const bool walker=ground && groundStyle->GetSelection()==1;
		const FSU_resource* templateGraphics=m_unit->gr_base;
		auto saveSprite=[&](const wxImage& image, const std::string& code,
			int slope,int direction)
		{
			const auto pngPath=spritesDir/(code+".png");
			const auto infoPath=spritesDir/(code+".info");
			if(!image.SaveFile(wxString(pngPath.wstring()),wxBITMAP_TYPE_PNG))
				throw std::runtime_error("Cannot save generated PNG "+code);
			SpellGresInfo info;
			info.path=infoPath;
			info.info_name=infoPath.filename().string();
			info.raw_name=code;
			info.raw_img_name=pngPath.filename().string();
			info.format=SpellGresInfo::Format::FSU;
			info.pal_name="spellcross_units.palinfo";
			info.colors_str="128-223";
			info.x_size=image.GetWidth();
			info.y_size=image.GetHeight();
			info.x_offset=40-image.GetWidth()/2;
			info.y_offset=ground
				?GroundFootOffset(image,templateGraphics,slope,direction,walker,
					contactOffset->GetValue())
				:128-image.GetHeight()/2;
			info.resampling=wxIMAGE_QUALITY_NEAREST;
			info.is_transparent=true;
			if(info.SaveInfo(infoPath))
				throw std::runtime_error("Cannot save sprite metadata "+code);
		};

		if(!ground)
		{
			// Preserve the working aircraft layout (8× A/F images).
			for(size_t i=0;i<frames.size();++i)
				for(char prefix:{'A','F'})
					saveSprite(frames[i],
						(wxString::Format("%c%02d",prefix,static_cast<int>(i))).ToStdString(),
						0,static_cast<int>(i)*2);
		}
		else if(!walker)
		{
			// Native tracked/wheeled vehicle layout: 13 terrain slopes A..M,
			// 16 camera azimuths each = 208 static FSU records. Names 000..C0F.
			for(int slope=0;slope<13;++slope)
				for(int direction=0;direction<16;++direction)
				{
					const auto name=wxString::Format("%X%02X",slope,direction).ToStdString();
					const auto image=AdaptGroundFrameToSlope(frames[direction],slope);
					saveSprite(image,name,slope,direction);
				}
		}
		else
		{
			// Infantry/creatures: 10 walking poses×8 headings followed by
			// 16 standing headings = 96 native FSU records. Animated glTF/FBX
			// clips provide real motion, otherwise use safe static fallback.
			if(!walkingFrames.empty() && walkingFrames.size()!=80)
				throw std::runtime_error("Invalid 3D walking animation frame count.");
			for(int pose=0;pose<10;++pose)
				for(int direction=0;direction<8;++direction)
				{
					const auto code=wxString::Format("0%X%X",pose,direction*2).ToStdString();
					const auto& frame=walkingFrames.empty()
						?frames[direction*2]:walkingFrames[pose*8+direction];
					saveSprite(frame,code,0,direction*2);
				}
			for(int direction=0;direction<16;++direction)
				saveSprite(frames[direction],
					wxString::Format("F%02X",direction).ToStdString(),0,direction);
		}

		// The Air/Ground selection is game data, not merely the sprite size.
		// When a new unit was cloned from an aircraft, it otherwise kept the
		// AIR classification and hovered 150 pixels over the terrain!
		if(BuildUnitGraphicsFromFolder(spritesDir,output,group))
		{
			if(unitMode->GetSelection()==1)
			{
				const int kind=m_unit->utype & SpellUnitRec::UTYPE_TYPE_MASK;
				if(walker)
				{
					if(kind==SpellUnitRec::UTYPE_TYPE_AIR ||
						kind==SpellUnitRec::UTYPE_TYPE_ARMORED)
						m_unit->utype=(m_unit->utype & ~SpellUnitRec::UTYPE_TYPE_MASK)
							| SpellUnitRec::UTYPE_TYPE_LIGHT;
					m_unit->utype |= SpellUnitRec::UTYPE_WALK;
				}
				else
				{
					if(kind==SpellUnitRec::UTYPE_TYPE_AIR)
						m_unit->utype=(m_unit->utype & ~SpellUnitRec::UTYPE_TYPE_MASK)
							| SpellUnitRec::UTYPE_TYPE_ARMORED;
					m_unit->utype &= ~SpellUnitRec::UTYPE_WALK;
				}
				// The 3D model includes the turret/body as a single asset.
				m_unit->utype &= ~(SpellUnitRec::UTYPE_FLY | SpellUnitRec::UTYPE_TURRET);
				// Synchronize the Basic / movement properties with the updated
				// ground class before the unit is registered for map placement.
				wxCommandEvent refreshProperties;
				OnSelectUnit(refreshProperties);
			}
			if(!CreateUnitIcon(frames[0],group))
				wxMessageBox("FSU built, but custom unit icon could not be created.",
					"Create unit graphics",wxOK|wxICON_EXCLAMATION,this);
			if(!CreateUnitInfoArt(original,group))
				wxMessageBox("FSU built, but INFO.FS artwork could not be created.",
					"Create unit graphics",wxOK|wxICON_EXCLAMATION,this);
		}
	}
	catch(const std::exception& error)
	{
		wxMessageBox("Sprite creation failed:\n"+wxString::FromUTF8(error.what())+
			"\n\nGenerated intermediates (if any) are in the selected output folder.",
			"Create game sprites",wxOK|wxICON_ERROR,this);
	}
}

// Use the finished game sprite to generate a NEW 60-pixel-wide tactical icon.
// The original template's icon is not copied. Icons are stored as I_*.LZ
// resources inside COMMON.FS, in the same 128-223 UNITS.PAL color range.
bool FormUnitsEdit::CreateUnitIcon(const wxImage& sprite,const std::string& group)
{
	if(!m_unit || !m_spell_data)
		return false;
	auto* pal=m_spell_data->GetPalette("UNITS.PAL");
	if(!pal || !sprite.IsOk() || !sprite.HasAlpha())
		return false;
	const std::string iconStem="I_"+group;
	const std::string iconFile=iconStem+".LZ";
	if(iconStem.size()>8 || m_spell_data->gres.GetResource(iconStem.c_str()))
	{
		wxMessageBox("Cannot assign new unit icon: name already exists or is too long: "
			+wxString::FromUTF8(iconStem.c_str()),"Custom unit icon",wxOK|wxICON_ERROR,this);
		return false;
	}

	// Original icon resources have fixed 60-pixel widths. Match the height of
	// the original icon only, not its image content.
	int iconW=60, iconH=60;
	auto* oldIcon=m_spell_data->gres.GetResource(m_unit->icon);
	if(oldIcon && oldIcon->x_size==60 && oldIcon->y_size>=24 && oldIcon->y_size<=128)
		iconH=oldIcon->y_size;

	const auto* alpha=sprite.GetAlpha();
	int l=sprite.GetWidth(), t=sprite.GetHeight(), r=-1, b=-1;
	for(int y=0;y<sprite.GetHeight();++y)
		for(int x=0;x<sprite.GetWidth();++x)
			if(alpha[static_cast<size_t>(y)*sprite.GetWidth()+x]>=24)
			{
				l=std::min(l,x);
				t=std::min(t,y);
				r=std::max(r,x);
				b=std::max(b,y);
			}
	if(r<l || b<t)
		return false;
	auto img=sprite.GetSubImage(wxRect(l,t,r-l+1,b-t+1));
	const double scale=std::min(static_cast<double>(iconW-8)/img.GetWidth(),
		static_cast<double>(iconH-8)/img.GetHeight());
	const int scaledW=std::max(1,static_cast<int>(std::lround(img.GetWidth()*scale)));
	const int scaledH=std::max(1,static_cast<int>(std::lround(img.GetHeight()*scale)));
	img.Rescale(scaledW,scaledH,wxIMAGE_QUALITY_BICUBIC);
	
	// Paint a solid new background, using palette color #128. Never copy
	// pixels from the original unit's icon; only its frame dimensions.
	auto rgb=pal->GetPal();
	wxImage output(iconW,iconH);
	auto* dest=output.GetData();
	for(int p=0;p<iconW*iconH;++p)
		for(int ch=0;ch<3;++ch)
			dest[3*p+ch]=rgb[128][ch];
	const int x0=(iconW-scaledW)/2;
	const int y0=(iconH-scaledH)/2;
	for(int y=0;y<scaledH;++y)
		for(int x=0;x<scaledW;++x)
		{
			const size_t sourcePos=static_cast<size_t>(y)*scaledW+x;
			const int coverage=img.HasAlpha()?img.GetAlpha()[sourcePos]:255;
			if(coverage<24)
				continue;
			const size_t targetPos=static_cast<size_t>(y+y0)*iconW+(x+x0);
			for(int ch=0;ch<3;++ch)
				dest[3*targetPos+ch]=static_cast<uint8_t>(
					(img.GetData()[3*sourcePos+ch]*coverage +
					rgb[128][ch]*(255-coverage))/255);
		}

	pal->ClearUserRange();
	pal->AddUserRangeStr("128-223");
	SpellGraphicItem icon;
	std::vector<ImgQuantize::Pixel> buffer;
	auto bitmap=wxBitmap(output);
	if(icon.Encode(bitmap,buffer,false,iconStem,pal,1.0,0.0,0.0,
		iconW,iconH,wxIMAGE_QUALITY_NEAREST,0,128))
	{
		wxMessageBox("Cannot quantize custom unit icon.","Custom unit icon",wxOK|wxICON_ERROR,this);
		return false;
	}
	std::vector<uint8_t> compressed;
	LZspell compressor(icon.pixels.data(),static_cast<int>(icon.pixels.size()),compressed);
	if(compressed.empty())
	{
		wxMessageBox("Cannot compress custom unit icon.","Custom unit icon",wxOK|wxICON_ERROR,this);
		return false;
	}
	// Verify the LZ encoding before exposing a new asset.
	LZWexpand decoder(128000);
	auto decoded=decoder.Decode(compressed);
	if(decoded!=icon.pixels)
	{
		wxMessageBox("Custom icon LZ verification failed.","Custom unit icon",wxOK|wxICON_ERROR,this);
		return false;
	}
	if(m_spell_data->gres.AddRaw(icon.pixels.data(),static_cast<int>(icon.pixels.size()),
		iconW,iconH,iconFile.c_str(),pal,true))
	{
		wxMessageBox("Cannot register new icon in running editor.","Custom unit icon",wxOK|wxICON_ERROR,this);
		return false;
	}
	// In-memory previews, JEDNOTKY.DEF and later COMMON.FS export now agree.
	m_custom_icon_lz[iconFile]=std::move(compressed);
	m_unit->icon=iconStem;
	m_unit->icon_glyph=m_spell_data->gres.GetResource(iconStem.c_str());
	canvasIcon->Refresh();
	SetStatusText("Custom icon "+iconStem+" created. Export COMMON.FS to include it in the mod.");
	return true;
}

// INFO.FS uses a separate per-unit 192-color palette and 437px wide
// compressed art. Generate a distinct portrait instead of reusing the template.
bool FormUnitsEdit::CreateUnitInfoArt(const wxImage& artwork,const std::string& group)
{
	if(!m_unit || !m_spell_data || !artwork.IsOk())
		return false;
	const std::string artStem="N"+group;
	const std::string artFile=artStem+".LZ";
	const std::string palFile=artStem+".PAL";
	if(artStem.size()>8 || m_spell_data->gres_info.GetResource(artStem.c_str()) ||
		m_spell_data->GetPalette(palFile))
	{
		wxMessageBox("An INFO.FS resource with this name already exists.",
			"Custom unit artwork",wxOK|wxICON_ERROR,this);
		return false;
	}

	constexpr int artW=437, artH=192;
	wxImage canvas(artW,artH);
	std::memset(canvas.GetData(),0,static_cast<size_t>(artW)*artH*3);
	const double scale=std::min(static_cast<double>(artW-8)/artwork.GetWidth(),
		static_cast<double>(artH-8)/artwork.GetHeight());
	const int w=std::max(1,static_cast<int>(std::lround(artwork.GetWidth()*scale)));
	const int h=std::max(1,static_cast<int>(std::lround(artwork.GetHeight()*scale)));
	auto img=artwork.Copy();
	img.Rescale(w,h,wxIMAGE_QUALITY_BICUBIC);
	for(int y=0;y<h;++y)
		for(int x=0;x<w;++x)
		{
			const size_t p=static_cast<size_t>(y)*w+x;
			const int a=img.HasAlpha()?img.GetAlpha()[p]:255;
			const size_t target=static_cast<size_t>(y+(artH-h)/2)*artW+(x+(artW-w)/2);
			for(int ch=0;ch<3;++ch)
				canvas.GetData()[3*target+ch]=static_cast<uint8_t>(
					img.GetData()[3*p+ch]*a/255);
		}

	// Derive a dedicated original-format INFO.FS palette (192 x RGB).
	std::vector<ImgQuantize::Pixel> input;
	input.reserve(static_cast<size_t>(artW)*artH);
	const auto* rgb=canvas.GetData();
	for(int p=0;p<artW*artH;++p)
		input.emplace_back(rgb[3*p],rgb[3*p+1],rgb[3*p+2],255);
	auto median=ImgQuantize::GenMedianCutPalette(input,191);
	if(median.empty())
		return false;
	std::vector<uint8_t> palData(192*3,0);
	for(size_t i=0;i<std::min(static_cast<size_t>(191),median.size());++i)
	{
		palData[3*(i+1)]=median[i].r;
		palData[3*(i+1)+1]=median[i].g;
		palData[3*(i+1)+2]=median[i].b;
	}
	SpellPalette artPal(palFile);
	if(artPal.Insert(palData,palFile,0,192))
		return false;
	artPal.ClearUserRange();
	artPal.AddUserRangeStr("1-191");
	SpellGraphicItem rendered;
	std::vector<ImgQuantize::Pixel> temp;
	auto bmp=wxBitmap(canvas);
	if(rendered.Encode(bmp,temp,false,artStem,&artPal,1.0,0.0,0.0,
		artW,artH,wxIMAGE_QUALITY_NEAREST,0,128))
		return false;
	std::vector<uint8_t> compressed;
	LZspell encoder(rendered.pixels.data(),static_cast<int>(rendered.pixels.size()),compressed);
	if(compressed.empty())
		return false;
	LZWexpand decoder(300000);
	auto decoded=decoder.Decode(compressed);
	if(decoded!=rendered.pixels)
		return false;

	// Register the palette and image in the live editor only after validation.
	auto* livePal=m_spell_data->AddPalette(palFile);
	if(!livePal || livePal->Insert(palData,palFile,0,192) ||
		m_spell_data->gres_info.AddRaw(rendered.pixels.data(),static_cast<int>(rendered.pixels.size()),
			artW,artH,artFile.c_str(),livePal,true))
		return false;
	m_custom_info_files[artFile]=std::move(compressed);
	m_custom_info_files[palFile]=std::move(palData);
	m_unit->info=artStem;
	m_unit->info_imgs.clear();
	auto* glyph=m_spell_data->gres_info.GetResource(artStem.c_str());
	if(glyph)
		m_unit->info_imgs.push_back(glyph);
	canvasInfo->Refresh();
	SetStatusText("Custom INFO.FS art "+artStem+" generated.");
	return true;
}

// Import all PNG frames in a folder as a fresh FSU resource, merge that resource
// into a copy of the currently loaded UNITS.FSU, then link it in-memory without
// replacing existing resources referenced by units already placed on maps.
void FormUnitsEdit::OnBuildUnitGraphics(wxCommandEvent& event)
{
	if(!m_unit || !m_spell_data || !m_spell_data->units_fsu ||
		m_spell_data->units_fsu_source_path.empty())
	{
		wxMessageBox("Select a unit and load the original game assets first.",
			"Build unit graphics",wxOK|wxICON_INFORMATION,this);
		return;
	}

	wxTextEntryDialog nameDialog(this,
		"Enter a unique FSU group name (1-5 uppercase letters or digits):",
		"New UNITS.FSU graphics group","U001");
	if(nameDialog.ShowModal() != wxID_OK)
		return;
	std::string group = nameDialog.GetValue().ToStdString();
	bool valid = !group.empty() && group.size() <= 5;
	for(unsigned char ch : group)
		valid = valid && ((ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'));
	if(!valid)
	{
		wxMessageBox("Group name must be 1-5 characters: A-Z or 0-9.",
			"Build unit graphics",wxOK|wxICON_EXCLAMATION,this);
		return;
	}
	if(m_spell_data->units_fsu->GetResource(group.c_str()))
	{
		wxMessageBox("This FSU group already exists. Choose a unique name; existing units must not be overwritten.",
			"Build unit graphics",wxOK|wxICON_EXCLAMATION,this);
		return;
	}

	wxDirDialog sourceDialog(this,"Select folder of PNG unit sprites (all directions/frames)",
		m_spell_data->export_path,wxDD_DIR_MUST_EXIST);
	if(sourceDialog.ShowModal() != wxID_OK)
		return;
	const std::filesystem::path source = sourceDialog.GetPath().ToStdWstring();
	wxDirDialog outputDialog(this,"Select an export folder for modded UNITS.FSU",
		m_spell_data->export_path,wxDD_DIR_MUST_EXIST);
	if(outputDialog.ShowModal() != wxID_OK)
		return;
	const std::filesystem::path output = outputDialog.GetPath().ToStdWstring();
	m_spell_data->export_path = output.wstring();
	BuildUnitGraphicsFromFolder(source,output,group);
}

// Keep the core archive transformation reusable from manual and artwork import.
bool FormUnitsEdit::BuildUnitGraphicsFromFolder(const std::filesystem::path& source,
	const std::filesystem::path& output,const std::string& group)
{
	if(!m_unit || !m_spell_data || !m_spell_data->units_fsu ||
		m_spell_data->units_fsu_source_path.empty())
		return false;
	const auto stage = output / ("spellcross_fsu_" + group);
	const auto stage_group = stage / group;
	const auto unitArchive = output / ("NEW_" + group + ".FSU");
	const auto outputArchive = output / "UNITS.FSU";
	if(std::filesystem::exists(stage) || std::filesystem::exists(unitArchive) ||
		std::filesystem::exists(outputArchive))
	{
		wxMessageBox("Output already contains a generated file or working folder.\n"
			"Choose an empty export folder. No existing files will be overwritten.",
			"Build unit graphics",wxOK|wxICON_EXCLAMATION,this);
		return false;
	}
	auto* palette = m_spell_data->GetPalette("UNITS.PAL");
	if(!palette)
	{
		wxMessageBox("The loaded game has no UNITS.PAL palette.","Build unit graphics",
			wxOK|wxICON_ERROR,this);
		return false;
	}
	palette->ClearUserRange();
	palette->AddUserRangeStr("128-223");

	struct SourceSprite {
		std::filesystem::path png;
		std::string code;
		SpellGresInfo meta;
	};
	std::vector<SourceSprite> sources;
	try
	{
		for(auto& entry : std::filesystem::directory_iterator(source))
		{
			if(!entry.is_regular_file())
				continue;
			auto path = entry.path();
			auto ext = path.extension().string();
			for(char& ch : ext)
				ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
			if(ext != ".png")
				continue;

			SpellGresInfo meta;
			auto metaPath = path;
			metaPath.replace_extension(L".info");
			std::string code = path.stem().string();
			if(std::filesystem::exists(metaPath))
			{
				if(meta.LoadInfo(metaPath) || !meta.isUnitsFSU() ||
					meta.img_name != path.filename().string())
				{
					wxMessageBox("Incompatible metadata for " + wxString(path.filename().wstring()),
						"Build unit graphics",wxOK|wxICON_ERROR,this);
					return false;
				}
				code = meta.name;
			}
			for(char& ch : code)
				ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
			bool valid_code = code.size() == 3;
			for(unsigned char ch : code)
				valid_code = valid_code && ((ch >= 'A' && ch <= 'Z') ||
					(ch >= '0' && ch <= '9') || ch == '_');
			if(!valid_code)
			{
				wxMessageBox("Every PNG needs a three-character sprite code "
					"(file name or matching .info):\n" + wxString(path.filename().wstring()),
					"Build unit graphics",wxOK|wxICON_ERROR,this);
				return false;
			}
			for(const auto& other : sources)
				if(other.code == code)
				{
					wxMessageBox("Duplicate FSU sprite code: " + wxString::FromUTF8(code.c_str()),
						"Build unit graphics",wxOK|wxICON_ERROR,this);
					return false;
				}
			sources.push_back({path,code,meta});
		}
		if(sources.empty())
		{
			wxMessageBox("The source folder contains no PNG sprites.", "Build unit graphics",
				wxOK|wxICON_EXCLAMATION,this);
			return false;
		}
		if(sources.size() > 255)
		{
			wxMessageBox("Too many sprites for one group.", "Build unit graphics",
				wxOK|wxICON_ERROR,this);
			return false;
		}

		// Only after all inputs have been validated do we create export files.
		std::filesystem::create_directories(stage_group);
		for(auto& item : sources)
		{
			wxBitmap bmp;
			if(!bmp.LoadFile(wxString(item.png.wstring()),wxBITMAP_TYPE_PNG))
				throw std::runtime_error("Failed loading PNG sprite: " + item.png.string());
			SpellGraphicItem gres;
			std::vector<ImgQuantize::Pixel> pixels;
			// A negative shadow color means "disabled", not opaque white.
			// Do not accidentally encode white parts of aircraft as shadows.
			int* shadow_color = nullptr;
			if(item.meta.isLoaded() && item.meta.shadow_color[0]>=0 &&
				item.meta.shadow_color[1]>=0 && item.meta.shadow_color[2]>=0)
				shadow_color = item.meta.shadow_color;
			auto x_size = item.meta.isLoaded() ? item.meta.x_size : bmp.GetWidth();
			auto y_size = item.meta.isLoaded() ? item.meta.y_size : bmp.GetHeight();
			if(x_size <= 0 || y_size <= 0 || x_size > 4096 || y_size > 4096)
				throw std::runtime_error("Invalid sprite dimensions");
			const int threshold = item.meta.isLoaded() ? item.meta.alpha_threshold : 128;
			const int x_ofs = item.meta.isLoaded() ? item.meta.x_offset : 0;
			const int y_ofs = item.meta.isLoaded() ? item.meta.y_offset : 0;
			auto resample = item.meta.isLoaded() ?
				(wxImageResizeQuality)item.meta.resampling : wxIMAGE_QUALITY_NEAREST;
			if(gres.Encode(bmp,pixels,false,item.code,palette,
				1.0,0.0,0.0,x_size,y_size,resample,0,threshold,shadow_color,0xFD))
				throw std::runtime_error("Unable to encode PNG sprite");
			if(FSU_sprite::SaveSprite(stage_group / item.code,gres.pixels,gres.x_size,
				x_ofs,y_ofs,0xFD))
				throw std::runtime_error("Failed encoding FSU sprite " + item.code);
		}

		FSUarchive justNew;
		if(justNew.LoadFolder(stage,group))
			throw std::runtime_error(justNew.m_last_error);
		if(justNew.Save(unitArchive))
			throw std::runtime_error("Failed to create standalone FSU resource");

		// Decode the small temporary archive only (not the entire game's FSU).
		FSUarchive decoded(unitArchive.wstring());
		auto* resource = decoded.GetResource(group.c_str());
		if(!resource || (!resource->stat.azimuths && !resource->anim.azimuths))
			throw std::runtime_error("Sprite naming does not form a valid FSU resource");
		// Original FSU parser infers heading/slope/frame counts from the
		// three-character filenames. Verify it detected the intended native
		// layout before merging anything into the user's mod archive.
		if(sources.size()==208 &&
			(resource->stat.azimuths!=16 || resource->stat.slopes!=13))
			throw std::runtime_error("Ground vehicle sprite layout is not 13 slopes x 16 directions.");
		if(sources.size()==96 &&
			(resource->stat.azimuths!=16 || resource->stat.slopes!=1 ||
			 resource->anim.azimuths!=8 || resource->anim.frames!=10))
			throw std::runtime_error("Walker sprite layout is not 10 x 8 movement plus 16 standing.");
		if(sources.size()==16 &&
			(resource->stat.azimuths!=8 || resource->anim.azimuths!=8))
			throw std::runtime_error("Aircraft sprite layout is not 8 movement plus 8 static.");

		// Merge compressed, untouched original sprites with the new group.
		FSUarchive merged(m_spell_data->units_fsu_source_path.wstring(),
			FSUarchive::Options::NO_DECODE);
		if(merged.LoadFolder(stage,group))
			throw std::runtime_error(merged.m_last_error);
		if(merged.Save(outputArchive))
			throw std::runtime_error("Failed saving modded UNITS.FSU");

		// Copy the decoded new resource into the live archive without replacing
		// the archive or any existing FSU_resource pointers used on a map.
		if(m_spell_data->units_fsu->AddResource(resource))
			throw std::runtime_error("Failed registering new FSU resource in editor");
		m_unit->gra = group;
		m_unit->gr_base = m_spell_data->units_fsu->GetResource(group.c_str());
		// Also handle units created before this workflow existed. Once their
		// primary graphic is custom, secondary reference graphics from the
		// original template must not suddenly appear during actions.
		m_unit->grb.clear();
		m_unit->gr_aux = nullptr;
		m_unit->anim_atack_light_name.clear();
		m_unit->anim_atack_armor_name.clear();
		m_unit->anim_atack_air_name.clear();
		m_unit->gr_attack_light = nullptr;
		m_unit->gr_attack_armor = nullptr;
		m_unit->gr_attack_air = nullptr;
		m_unit->action_fsu_name.clear();
		m_unit->gr_action = nullptr;
		m_unit->die_anim_name.clear();
		m_unit->gr_die = nullptr;

		wxCommandEvent refresh;
		OnSelectUnit(refresh);
		SetStatusText("UNITS.FSU built and graphics linked to selected unit. Save DEF, then place/test on map.");
		wxMessageBox("Graphics built successfully.\n\n"
			"Modded UNITS.FSU: " + wxString(outputArchive.wstring()) +
			"\n\nThe graphic group is now available in this editor session. "
			"Save your JEDNOTKY.DEF and click Place / test on map.",
			"Build unit graphics",wxOK|wxICON_INFORMATION,this);
		return true;
	}
	catch(const std::exception& error)
	{
		wxMessageBox("Graphics build failed:\n" + wxString::FromUTF8(error.what()) +
			"\n\nAny output already written is in the selected export folder.",
			"Build unit graphics",wxOK|wxICON_ERROR,this);
		return false;
	}
}


// Register only the selected, newly appended DEF record. The SpellData linker
// resolves FSU, icon, animation and sound pointers before making it visible.
void FormUnitsEdit::OnTestUnitOnMap(wxCommandEvent& event)
{
	if(!m_units || !m_spell_data || !m_spell_data->units)
	{
		wxMessageBox("Open a working JEDNOTKY.DEF first.", "Unit map test",
			wxOK|wxICON_INFORMATION, this);
		return;
	}
	int id = lboxUnits->GetNextItem(-1,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
	if(id < 0 || !m_units->GetUnit(id))
	{
		wxMessageBox("Select the custom unit you want to test.", "Unit map test",
			wxOK|wxICON_INFORMATION, this);
		return;
	}
	if(m_units->GetUnit(id)->icon.empty())
	{
		wxMessageBox("This unit has no custom icon yet. Convert its artwork first, "
			"or assign an icon explicitly under Graphics.", "Unit map test",
			wxOK|wxICON_EXCLAMATION,this);
		return;
	}

	// Existing map units may hold raw pointers; only register new IDs.
	const int next_id = m_spell_data->units->Count();
	if(id < next_id)
	{
		wxMessageBox("This unit is already loaded for map testing. To update its definition, "
			"reload game assets from the mod folder before testing it again.",
			"Unit map test",wxOK|wxICON_EXCLAMATION,this);
		return;
	}

	std::vector<uint8_t> def;
	if(m_units->GenerateDEF(def))
	{
		wxMessageBox("Cannot serialize JEDNOTKY.DEF. Check unit names and resource fields.",
			"Unit map test",wxOK|wxICON_ERROR,this);
		return;
	}
	const size_t rec_size = m_units->isEng() ? 207 : 206;
	if(def.size() != static_cast<size_t>(m_units->Count()) * rec_size)
	{
		wxMessageBox("Invalid DEF record length.", "Unit map test",wxOK|wxICON_ERROR,this);
		return;
	}
	for(int uid = next_id; uid <= id; ++uid)
	{
		const size_t start = static_cast<size_t>(uid) * rec_size;
		std::vector<uint8_t> record(def.begin() + start,def.begin() + start + rec_size);
		std::string error;
		if(m_spell_data->RegisterUnitForMap(record,uid,error))
		{
			wxMessageBox(wxString::Format("Cannot register unit #%d:\n",uid) +
				wxString::FromUTF8(error.c_str()),"Unit map test",wxOK|wxICON_ERROR,this);
			return;
		}
	}

	auto* main = dynamic_cast<MainFrame*>(GetParent());
	if(!main || !main->OpenCustomUnitPlacement(id))
	{
		wxMessageBox("Unit registered in the editor. Load a map and use the usual Add unit tool "
			"to place it. It remains available until game data are reloaded.",
			"Unit map test",wxOK|wxICON_INFORMATION,this);
		return;
	}
	SetStatusText("Unit registered and selected in map placement tool. Save the mod assets separately.");
}


// Guided unit creation: templates provide stats, external Blender provides
// genuine 3D directional graphics for .glb/.gltf/.fbx models.
void FormUnitsEdit::OnNewUnit(wxCommandEvent& event)
{
	if(!m_units)
	{
		wxMessageBox("Open a JEDNOTKY.DEF file first (File > Open units DEF file(s)).",
			"New unit", wxOK|wxICON_INFORMATION, this);
		return;
	}

	wxDialog dialog(this, wxID_ANY, "Create Spellcross unit", wxDefaultPosition,
		wxSize(660, 470), wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
	auto outer = new wxBoxSizer(wxVERTICAL);
	auto pages = new wxNotebook(&dialog, wxID_ANY);

	// Page 1: pick a safe set of default values by cloning an existing unit.
	auto basics = new wxPanel(pages);
	auto basicsSizer = new wxBoxSizer(wxVERTICAL);
	basicsSizer->Add(new wxStaticText(basics, wxID_ANY,
		"Start from a similar unit to inherit combat stats, sounds and animations."),
		0, wxALL, 10);
	auto basicFields = new wxFlexGridSizer(2, 8, 10);
	basicFields->AddGrowableCol(1, 1);
	basicFields->Add(new wxStaticText(basics, wxID_ANY, "Template:"), 0, wxALIGN_CENTER_VERTICAL);
	wxArrayString templates;
	templates.Add("(empty definition)");
	for(int id = 0; id < m_units->Count(); ++id)
		templates.Add(OnGetUnitName(id));
	auto templateChoice = new wxChoice(basics, wxID_ANY, wxDefaultPosition, wxDefaultSize, templates);
	auto selectedUnit = lboxUnits->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	templateChoice->SetSelection(selectedUnit >= 0 && selectedUnit < m_units->Count() ? selectedUnit + 1 :
		(m_units->Count() > 0 ? 1 : 0));
	basicFields->Add(templateChoice, 1, wxEXPAND);
	basicFields->Add(new wxStaticText(basics, wxID_ANY, "Unit name:"), 0, wxALIGN_CENTER_VERTICAL);
	auto nameField = new wxTextCtrl(basics, wxID_ANY, "New unit");
	nameField->SetMaxLength(27); // JEDNOTKY.DEF fixed-width unit name
	basicFields->Add(nameField, 1, wxEXPAND);
	basicsSizer->Add(basicFields, 0, wxEXPAND|wxALL, 10);
	basicsSizer->Add(new wxStaticText(basics, wxID_ANY,
		"After creation, edit all attack, defence, movement, graphics and sound\n"
		"attributes in the existing Basic / Graphics / Sounds / Actions tabs."),
		0, wxALL, 10);
	basics->SetSizer(basicsSizer);
	pages->AddPage(basics, "1. Definition", true);

	// Page 2: regular high-resolution artwork, not a pre-built game sprite.
	auto graphics = new wxPanel(pages);
	auto graphicsSizer = new wxBoxSizer(wxVERTICAL);
	graphicsSizer->Add(new wxStaticText(graphics, wxID_ANY,
		"Recommended: select a real 3D model (.glb, .gltf or .fbx).\n"
		"Blender renders eight accurate directional views from actual geometry.\n"
		"Legacy PNG import is available but cannot create real 3D viewpoints."),
		0, wxALL, 10);
	auto pngRow = new wxBoxSizer(wxHORIZONTAL);
	auto pngField = new wxTextCtrl(graphics, wxID_ANY);
	auto browse = new wxButton(graphics, wxID_ANY, "Browse...");
	pngRow->Add(pngField, 1, wxRIGHT|wxEXPAND, 5);
	pngRow->Add(browse, 0);
	graphicsSizer->Add(new wxStaticText(graphics,wxID_ANY,"Source 3D model / legacy PNG:"),
		0,wxLEFT|wxRIGHT|wxTOP,10);
	graphicsSizer->Add(pngRow,0,wxEXPAND|wxALL,10);
	graphicsSizer->Add(new wxStaticText(graphics, wxID_ANY,
		"3D rendering requires Blender installed on your computer.\n"
		"Model files and original textures will not be modified."),
		0, wxALL, 10);
	browse->Bind(wxEVT_BUTTON, [&](wxCommandEvent&)
	{
		wxFileDialog pick(&dialog, "Select 3D unit model", wxEmptyString,
			wxEmptyString,
			"3D models (*.glb;*.gltf;*.fbx)|*.glb;*.gltf;*.fbx|"
			"glTF binary (*.glb)|*.glb|glTF (*.gltf)|*.gltf|"
			"FBX (*.fbx)|*.fbx|Legacy PNG (*.png)|*.png",
			wxFD_OPEN|wxFD_FILE_MUST_EXIST);
		if(pick.ShowModal() == wxID_OK)
			pngField->SetValue(pick.GetPath());
	});
	graphics->SetSizer(graphicsSizer);
	pages->AddPage(graphics, "2. Source artwork");

	outer->Add(pages, 1, wxALL|wxEXPAND, 8);
	auto buttons = new wxStdDialogButtonSizer();
	auto createButton = new wxButton(&dialog, wxID_OK, "Create unit");
	buttons->AddButton(createButton);
	buttons->AddButton(new wxButton(&dialog, wxID_CANCEL, "Cancel"));
	buttons->Realize();
	outer->Add(buttons, 0, wxALL|wxALIGN_RIGHT, 10);
	dialog.SetSizer(outer);
	dialog.SetMinSize(wxSize(610, 415));
	dialog.Layout();

	// Validate without closing, so a wrong name/path can be corrected.
	createButton->Bind(wxEVT_BUTTON, [&](wxCommandEvent&)
	{
		wxString unitName = nameField->GetValue();
		unitName.Trim(true).Trim(false);
		if(unitName.IsEmpty())
		{
			wxMessageBox("Enter a unit name.", "New unit", wxOK|wxICON_EXCLAMATION, &dialog);
			pages->SetSelection(0);
			return;
		}
		if(!pngField->GetValue().IsEmpty())
		{
			const auto picked=std::filesystem::path(pngField->GetValue().ToStdWstring());
			std::string extension=picked.extension().string();
			for(char& c:extension)
				c=static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
			bool validType=extension==".glb" || extension==".gltf" ||
				extension==".fbx" || extension==".png";
			if(!validType || !std::filesystem::is_regular_file(picked))
			{
				wxMessageBox("Select an existing .glb, .gltf, .fbx or .png file.",
					"New unit",wxOK|wxICON_ERROR,&dialog);
				pages->SetSelection(1);
				return;
			}
			if(extension==".png")
			{
				wxImage testImage;
				if(!testImage.LoadFile(pngField->GetValue(),wxBITMAP_TYPE_PNG))
				{
					wxMessageBox("Cannot read legacy PNG.","New unit",wxOK|wxICON_ERROR,&dialog);
					pages->SetSelection(1);
					return;
				}
			}
		}
		dialog.EndModal(wxID_OK);
	});

	if(dialog.ShowModal() != wxID_OK)
		return;

	wxString unitName = nameField->GetValue();
	unitName.Trim(true).Trim(false);
	const auto sourcePNG = pngField->GetValue();
	const int templateIndex = templateChoice->GetSelection() - 1;
	auto unit = m_units->AddUnit(templateIndex);
	unit->name = unitName.ToStdWstring();
	// Inherit game mechanics from the template, never its visual identity.
	// The artwork converter creates a dedicated I_*.LZ glyph; without artwork
	// the icon and info art must be deliberately supplied before gameplay.
	unit->icon.clear();
	unit->icon_glyph = nullptr;
	unit->info.clear();
	unit->info_imgs.clear();
	// Do not keep the source unit's secondary visuals or action sprites.
	unit->grb.clear();
	unit->gr_aux = nullptr;
	unit->anim_atack_light_name.clear();
	unit->anim_atack_armor_name.clear();
	unit->anim_atack_air_name.clear();
	unit->gr_attack_light = nullptr;
	unit->gr_attack_armor = nullptr;
	unit->gr_attack_air = nullptr;
	unit->action_fsu_name.clear();
	unit->gr_action = nullptr;
	unit->die_anim_name.clear();
	unit->gr_die = nullptr;

	// Refresh the virtual list and select the newly created record.
	wxCommandEvent refresh(wxEVT_COMMAND_LISTBOX_SELECTED, wxID_LBOX_DEFS);
	refresh.SetEventObject(lboxDEF);
	refresh.SetInt(lboxDEF->GetSelection());
	OnSelectDEF(refresh);
	lboxUnits->SetItemState(-1, 0, wxLIST_STATE_SELECTED);
	lboxUnits->SetItemState(unit->type_id, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	lboxUnits->EnsureVisible(unit->type_id);
	wxCommandEvent unitSelection;
	OnSelectUnit(unitSelection);
	lboxUnits->Refresh();

	if(!sourcePNG.IsEmpty())
	{
		GenerateGameSpritesFromSource(std::filesystem::path(sourcePNG.ToStdWstring()));
	}
	else
		SetStatusText("New unit added. Edit its attributes and save/export DEF when ready.");

}


// units popup menu
void FormUnitsEdit::OnUnitsPupupOpen(wxMouseEvent& event)
{
	wxMenu menu;

	menu.Append((int)PopupActions::CLR_ALL_UNITS,"Clear all");
	menu.Append((int)PopupActions::SEL_ALL_UNITS,"Select all");
	menu.AppendSeparator();
	menu.Append((int)PopupActions::ADD_UNIT,"New unit...");

	auto unit_id = lboxUnits->GetNextItem(-1,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
	if(unit_id >= 0 && lboxUnits->GetSelectedItemCount() == 1)
		menu.Append((int)PopupActions::DUP_UNIT,"Duplicate unit");

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
		wxCommandEvent createEvent;
		OnNewUnit(createEvent);
	}
	else if(menu_id == PopupActions::DUP_UNIT)
	{
		if(!m_units)
			return;
		auto unit_id = lboxUnits->GetNextItem(-1,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
		if(unit_id < 0)
			return;

		auto unit = m_units->AddUnit();		
		m_units->CopyUnit(unit_id,unit->type_id);		
						
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





class pgPropData : public wxClientData {
public:
	wxPGProperty* prop;
	std::string* anim_name;
};

// props popup menu
void FormUnitsEdit::OnUnitPropsPupupOpen(wxPropertyGridEvent& event)
{
	wxMenu menu;

	auto prop = event.GetProperty();
	if(!prop)
		return;
	if(!m_unit)
		return;

	std::string *anim_name = NULL;
	auto name = prop->GetName();
	if(name == "anim_main_frames")
		anim_name = &m_unit->gra;
	else if(name == "anim_light_frames")
		anim_name = &m_unit->anim_atack_light_name;
	else if(name == "anim_armored_frames")
		anim_name = &m_unit->anim_atack_armor_name;
	else if(name == "anim_air_frames")
		anim_name = &m_unit->anim_atack_air_name;
	else if(name == "spec_anim_frames")
		anim_name = &m_unit->action_fsu_name;
	else if(name == "die_anim_frames")
		anim_name = &m_unit->die_anim_name;	
	if(anim_name)
	{
		auto grp = m_spell_data->units_fsu->GetResource(*anim_name);
		if(grp && grp->anim.frames)
			menu.Append((int)PopupActions::RESET_ANIM_FRAMES,"Set frames to max");
	}

	menu.Append((int)PopupActions::COPY_PROP,string_format("Copy property '%s' to target DEFs", prop->GetLabel().ToStdString()));
	menu.Append((int)PopupActions::COPY_PROPS_PAGE,string_format("Copy properties of this page to target DEFs"));
		
	auto data = new pgPropData();
	data->prop = prop;
	data->anim_name = anim_name;
	menu.SetClientObject(data);
				
	menu.Connect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(FormUnitsEdit::OnUnitPropsPupup),NULL,this);
	PopupMenu(&menu);
}
void FormUnitsEdit::OnUnitPropsPupup(wxCommandEvent& event)
{
	auto menu_id = (PopupActions)event.GetId();
	auto menu = (wxMenu*)event.GetEventObject();
	if(!menu)
		return;
	auto data = (pgPropData*)menu->GetClientObject();
	if(!data)
		return;

	if(menu_id == PopupActions::RESET_ANIM_FRAMES)
	{
		auto *anim_name = data->anim_name;
		if(!anim_name)
			return;
		if(!m_unit)
			return;

		auto grp = m_spell_data->units_fsu->GetResource(*anim_name);
		if(!grp || !grp->anim.frames)
			return;
		auto frames = grp->anim.frames;
		
		if(anim_name == &m_unit->gra)
			m_unit->gra_frames = frames;
		else if(anim_name == &m_unit->anim_atack_light_name)
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
	else if(menu_id == PopupActions::COPY_PROP || menu_id == PopupActions::COPY_PROPS_PAGE)
	{
		// copy property to target DEFs

		// target DEFs
		std::vector<std::shared_ptr<SpellUnits>> def_list;
		wxArrayInt sel;
		lboxTargetDef->GetSelections(sel);
		for(auto &id: sel)
			if(id >= 0 && id < m_defs.size())
				def_list.push_back(m_defs[id]);

		// target unit
		if(!m_units)
			return;
		if(!m_unit)
			return;
		int uid = m_unit->type_id;		

		std::vector<wxPGProperty*> prop_list;
		if(menu_id == PopupActions::COPY_PROP)
		{
			// single prop
			auto prop = data->prop;
			if(!prop)
				return;
			prop_list.push_back(prop);
		}
		else if(menu_id == PopupActions::COPY_PROPS_PAGE)
		{
			// all props
			auto prop = data->prop;
			if(!prop)
				return;
			auto pg = prop->GetGrid();
			if(!pg)
				return;			
			for(auto it = pg->GetIterator(wxPG_ITERATE_PROPERTIES); !it.AtEnd(); it++)
				prop_list.push_back(*it);
		}
		
		// for each property:
		for(auto &prop: prop_list)
		{
			if(!prop)			
				continue;
			auto obj = (wxPGobj*)prop->GetClientObject();
			if(!obj)
				return;

			// for each target:
			for(auto &def: def_list)
			{
				auto target_unit = def->GetUnit(uid);
				if(!target_unit)
					continue;
				obj->Copy(m_unit,target_unit);
			}
		}
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
		obj->Update();
		
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
			if(name == "anim_main")
				anim_name = m_unit->gra;
			else if(name == "anim_light")
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
			if(name == "anim_main")
				m_unit->gra_frames = frames;
			else if(name == "anim_light")
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



