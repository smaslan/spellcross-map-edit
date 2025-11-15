///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_units.h"
#include "other.h"

#include <wx/rawbmp.h>

///////////////////////////////////////////////////////////////////////////

FormUnits::FormUnits( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	// === AUTO GENERATED START ===	
	
	this->SetSizeHints(wxDefaultSize,wxDefaultSize);
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENU));

	sbar = this->CreateStatusBar(1,wxSTB_SIZEGRIP,wxID_SB);
	mmenu = new wxMenuBar(0);
	mmFile = new wxMenu();
	wxMenuItem* mmSaveAux;
	mmSaveAux = new wxMenuItem(mmFile,wxID_MM_SAVE_AUX,wxString(wxT("Save Aux Data")),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmSaveAux);

	mmFile->AppendSeparator();

	wxMenuItem* mmExit;
	mmExit = new wxMenuItem(mmFile,wxID_MM_EXIT,wxString(wxT("Exit")) + wxT('\t') + wxT("Esc"),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmExit);

	mmenu->Append(mmFile,wxT("File"));

	mmEdit = new wxMenu();
	wxMenuItem* mmCopyGrpOrg;
	mmCopyGrpOrg = new wxMenuItem(mmEdit,wxID_MM_COPY_GRP_ORG,wxString(wxT("Copy fire origins")) + wxT('\t') + wxT("Ctrl+C"),wxEmptyString,wxITEM_NORMAL);
	mmEdit->Append(mmCopyGrpOrg);

	wxMenuItem* mmPasteGrpOrg;
	mmPasteGrpOrg = new wxMenuItem(mmEdit,wxID_MM_PASTE_GRP_ORG,wxString(wxT("Paste fire origins")) + wxT('\t') + wxT("Ctrl+V"),wxEmptyString,wxITEM_NORMAL);
	mmEdit->Append(mmPasteGrpOrg);

	mmEdit->AppendSeparator();

	wxMenuItem* mmSet;
	mmSet = new wxMenuItem(mmEdit,wxID_MM_SET,wxString(wxT("Update/place unit")) + wxT('\t') + wxT("Enter"),wxEmptyString,wxITEM_NORMAL);
	mmEdit->Append(mmSet);

	mmenu->Append(mmEdit,wxT("Edit"));

	this->SetMenuBar(mmenu);

	wxBoxSizer* szMain;
	szMain = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* szList;
	szList = new wxBoxSizer(wxVERTICAL);

	szList->SetMinSize(wxSize(180,-1));
	m_staticText32 = new wxStaticText(this,wxID_ANY,wxT("Units list:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText32->Wrap(-1);
	szList->Add(m_staticText32,0,wxTOP|wxRIGHT|wxLEFT,5);

	lboxUnits = new wxListBox(this,wxID_LB_UNITS,wxDefaultPosition,wxSize(-1,-1),0,NULL,wxLB_ALWAYS_SB|wxALWAYS_SHOW_SB|wxVSCROLL);
	lboxUnits->SetMaxSize(wxSize(180,-1));

	szList->Add(lboxUnits,1,wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND,5);


	szMain->Add(szList,0,wxEXPAND,5);

	m_staticline9 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_VERTICAL);
	szMain->Add(m_staticline9,0,wxEXPAND|wxTOP|wxBOTTOM,5);

	wxBoxSizer* szProps;
	szProps = new wxBoxSizer(wxVERTICAL);

	m_staticText35 = new wxStaticText(this,wxID_ANY,wxT("Unit icon:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText35->Wrap(-1);
	szProps->Add(m_staticText35,0,wxTOP|wxRIGHT|wxLEFT,5);

	icon_canvas = new wxPanel(this,wxID_CANVAS_ICON,wxDefaultPosition,wxDefaultSize,wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL);
	icon_canvas->SetMaxSize(wxSize(-1,100));

	szProps->Add(icon_canvas,1,wxEXPAND|wxALL,5);

	m_staticText34 = new wxStaticText(this,wxID_ANY,wxT("Properties:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText34->Wrap(-1);
	szProps->Add(m_staticText34,0,wxTOP|wxRIGHT|wxLEFT,5);

	props = new wxPropertyGrid(this,wxID_PROPS,wxDefaultPosition,wxDefaultSize,wxPG_DEFAULT_STYLE);
	props->SetMinSize(wxSize(200,-1));

	szProps->Add(props,1,wxBOTTOM|wxRIGHT|wxLEFT,5);

	m_staticline13 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_HORIZONTAL);
	szProps->Add(m_staticline13,0,wxEXPAND | wxALL,5);

	m_staticText60 = new wxStaticText(this,wxID_ANY,wxT("Unit index:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText60->Wrap(-1);
	szProps->Add(m_staticText60,0,wxTOP|wxRIGHT|wxLEFT,5);

	spinID = new wxSpinCtrl(this,wxID_SPIN_ID,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxSP_ARROW_KEYS,0,100,0);
	szProps->Add(spinID,0,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);

	m_staticText39 = new wxStaticText(this,wxID_ANY,wxT("Custom name:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText39->Wrap(-1);
	szProps->Add(m_staticText39,0,wxLEFT|wxRIGHT,5);

	txtName = new wxTextCtrl(this,wxID_NAME,wxEmptyString,wxDefaultPosition,wxDefaultSize,0);
	szProps->Add(txtName,0,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);

	m_staticText40 = new wxStaticText(this,wxID_ANY,wxT("Health:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText40->Wrap(-1);
	szProps->Add(m_staticText40,0,wxRIGHT|wxLEFT,5);

	spinHealth = new wxSpinCtrl(this,wxID_SPIN_HEALTH,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxSP_ARROW_KEYS,0,10,0);
	szProps->Add(spinHealth,0,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);

	m_staticText75 = new wxStaticText(this,wxID_ANY,wxT("Experience:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText75->Wrap(-1);
	szProps->Add(m_staticText75,0,wxLEFT|wxRIGHT,5);

	slideXP = new wxSlider(this,wxID_ANY,50,0,12,wxDefaultPosition,wxDefaultSize,wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_MIN_MAX_LABELS|wxSL_SELRANGE|wxSL_VALUE_LABEL);
	szProps->Add(slideXP,0,wxEXPAND|wxLEFT|wxRIGHT,5);

	m_staticText401 = new wxStaticText(this,wxID_ANY,wxT("Behaviour:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText401->Wrap(-1);
	szProps->Add(m_staticText401,0,wxLEFT|wxRIGHT,5);

	wxArrayString chUnitBehaveChoices;
	chUnitBehave = new wxChoice(this,wxID_UNIT_BEHAVE,wxDefaultPosition,wxDefaultSize,chUnitBehaveChoices,0);
	chUnitBehave->SetSelection(0);
	szProps->Add(chUnitBehave,0,wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT,5);

	m_staticText74 = new wxStaticText(this,wxID_ANY,wxT("Special unit type (event-units only):"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText74->Wrap(-1);
	szProps->Add(m_staticText74,0,wxLEFT|wxRIGHT,5);

	wxArrayString chUnitTypeChoices;
	chUnitType = new wxChoice(this,wxID_UNIT_TYPE,wxDefaultPosition,wxDefaultSize,chUnitTypeChoices,0);
	chUnitType->SetSelection(0);
	szProps->Add(chUnitType,0,wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT,5);


	szMain->Add(szProps,0,wxEXPAND,5);

	m_staticline10 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_VERTICAL);
	szMain->Add(m_staticline10,0,wxEXPAND|wxTOP|wxBOTTOM,5);

	wxBoxSizer* szArt;
	szArt = new wxBoxSizer(wxVERTICAL);

	pages = new wxNotebook(this,wxID_PAGE_CTRL,wxDefaultPosition,wxDefaultSize,0);
	pageArt = new wxPanel(pages,wxID_PAGE_ART,wxDefaultPosition,wxDefaultSize,wxTAB_TRAVERSAL);
	wxBoxSizer* bSizer35;
	bSizer35 = new wxBoxSizer(wxVERTICAL);

	m_staticText36 = new wxStaticText(pageArt,wxID_ANY,wxT("Unit art:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText36->Wrap(-1);
	bSizer35->Add(m_staticText36,0,wxTOP|wxRIGHT|wxLEFT,5);

	art_canvas = new wxPanel(pageArt,wxID_CANVAS_ART,wxDefaultPosition,wxSize(-1,-1),wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL);
	art_canvas->SetMinSize(wxSize(-1,400));

	bSizer35->Add(art_canvas,1,wxEXPAND | wxALL,5);

	m_staticline11 = new wxStaticLine(pageArt,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_HORIZONTAL);
	bSizer35->Add(m_staticline11,0,wxEXPAND | wxALL,5);

	wxBoxSizer* szArtSel;
	szArtSel = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* szArtSelector;
	szArtSelector = new wxBoxSizer(wxVERTICAL);

	szArtSelector->SetMinSize(wxSize(150,-1));
	m_staticText37 = new wxStaticText(pageArt,wxID_ANY,wxT("Art selection:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText37->Wrap(-1);
	szArtSelector->Add(m_staticText37,0,wxTOP|wxRIGHT|wxLEFT,5);

	lboxArt = new wxListBox(pageArt,wxID_LB_ART,wxDefaultPosition,wxDefaultSize,0,NULL,wxLB_ALWAYS_SB);
	szArtSelector->Add(lboxArt,1,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);


	szArtSel->Add(szArtSelector,0,wxEXPAND,5);

	m_staticline12 = new wxStaticLine(pageArt,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_VERTICAL);
	szArtSel->Add(m_staticline12,0,wxEXPAND|wxTOP|wxBOTTOM,5);

	wxBoxSizer* szInfo;
	szInfo = new wxBoxSizer(wxVERTICAL);

	m_staticText38 = new wxStaticText(pageArt,wxID_ANY,wxT("Unit description:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText38->Wrap(-1);
	szInfo->Add(m_staticText38,0,wxTOP|wxRIGHT|wxLEFT,5);

	txtInfo = new wxTextCtrl(pageArt,wxID_TXT_INFO,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_LEFT|wxTE_MULTILINE|wxTE_WORDWRAP);
	szInfo->Add(txtInfo,1,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);


	szArtSel->Add(szInfo,1,wxEXPAND,5);


	bSizer35->Add(szArtSel,1,wxEXPAND,5);


	pageArt->SetSizer(bSizer35);
	pageArt->Layout();
	bSizer35->Fit(pageArt);
	pages->AddPage(pageArt,wxT("Unit art"),false);
	pageGrp = new wxPanel(pages,wxID_PAGE_GRP,wxDefaultPosition,wxDefaultSize,wxTAB_TRAVERSAL);
	wxBoxSizer* bSizer36;
	bSizer36 = new wxBoxSizer(wxVERTICAL);

	m_staticText42 = new wxStaticText(pageGrp,wxID_ANY,wxT("Sprite render:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText42->Wrap(-1);
	bSizer36->Add(m_staticText42,0,wxTOP|wxRIGHT|wxLEFT,5);

	grp_canvas = new wxPanel(pageGrp,wxID_CANVAS_GRP,wxDefaultPosition,wxDefaultSize,wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL);
	bSizer36->Add(grp_canvas,1,wxEXPAND | wxALL,5);

	m_staticline14 = new wxStaticLine(pageGrp,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_HORIZONTAL);
	bSizer36->Add(m_staticline14,0,wxEXPAND | wxALL,5);

	m_staticText43 = new wxStaticText(pageGrp,wxID_ANY,wxT("Graphics type:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText43->Wrap(-1);
	bSizer36->Add(m_staticText43,0,wxTOP|wxRIGHT|wxLEFT,5);

	wxArrayString chbGrpTypeChoices;
	chbGrpType = new wxChoice(pageGrp,wxID_CHB_GRP_TYPE,wxDefaultPosition,wxDefaultSize,chbGrpTypeChoices,0);
	chbGrpType->SetSelection(0);
	bSizer36->Add(chbGrpType,0,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);

	wxBoxSizer* bSizer37;
	bSizer37 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer38;
	bSizer38 = new wxBoxSizer(wxVERTICAL);

	txtGrpZim = new wxStaticText(pageGrp,wxID_TXT_GRP_AZIM,wxT("Azimuth:"),wxDefaultPosition,wxDefaultSize,0);
	txtGrpZim->Wrap(-1);
	bSizer38->Add(txtGrpZim,0,wxRIGHT|wxLEFT,5);

	slideAzim = new wxSlider(pageGrp,wxID_SLIDE_AZIM,50,0,100,wxDefaultPosition,wxDefaultSize,wxSL_AUTOTICKS|wxSL_BOTTOM|wxSL_HORIZONTAL|wxSL_MIN_MAX_LABELS|wxSL_VALUE_LABEL);
	bSizer38->Add(slideAzim,0,wxALL|wxEXPAND,5);


	bSizer37->Add(bSizer38,1,wxEXPAND,5);

	m_staticline15 = new wxStaticLine(pageGrp,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_VERTICAL);
	bSizer37->Add(m_staticline15,0,wxEXPAND | wxALL,5);

	wxBoxSizer* bSizer39;
	bSizer39 = new wxBoxSizer(wxVERTICAL);

	txtGrpSlope = new wxStaticText(pageGrp,wxID_TXT_GRP_SLOPE,wxT("Slope:"),wxDefaultPosition,wxDefaultSize,0);
	txtGrpSlope->Wrap(-1);
	bSizer39->Add(txtGrpSlope,0,wxRIGHT|wxLEFT,5);

	slideSlope = new wxSlider(pageGrp,wxID_SLIDE_SLOPE,50,0,100,wxDefaultPosition,wxDefaultSize,wxSL_AUTOTICKS|wxSL_BOTTOM|wxSL_HORIZONTAL|wxSL_MIN_MAX_LABELS|wxSL_VALUE_LABEL);
	bSizer39->Add(slideSlope,0,wxALL|wxEXPAND,5);


	bSizer37->Add(bSizer39,1,wxEXPAND,5);

	m_staticline16 = new wxStaticLine(pageGrp,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_VERTICAL);
	bSizer37->Add(m_staticline16,0,wxEXPAND | wxALL,5);

	wxBoxSizer* bSizer40;
	bSizer40 = new wxBoxSizer(wxVERTICAL);

	m_staticText46 = new wxStaticText(pageGrp,wxID_ANY,wxT("Frame:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText46->Wrap(-1);
	bSizer40->Add(m_staticText46,0,wxRIGHT|wxLEFT,5);

	slideFrame = new wxSlider(pageGrp,wxID_SLIDE_FRAME,50,0,100,wxDefaultPosition,wxDefaultSize,wxSL_AUTOTICKS|wxSL_BOTTOM|wxSL_HORIZONTAL|wxSL_MIN_MAX_LABELS|wxSL_VALUE_LABEL);
	bSizer40->Add(slideFrame,0,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);


	bSizer37->Add(bSizer40,1,wxEXPAND,5);


	bSizer36->Add(bSizer37,0,wxEXPAND,5);

	m_staticline17 = new wxStaticLine(pageGrp,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_HORIZONTAL);
	bSizer36->Add(m_staticline17,0,wxEXPAND | wxALL,5);

	wxBoxSizer* bSizer41;
	bSizer41 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer42;
	bSizer42 = new wxBoxSizer(wxVERTICAL);

	txtGrpZoom = new wxStaticText(pageGrp,wxID_ANY,wxT("Zoom:"),wxDefaultPosition,wxDefaultSize,0);
	txtGrpZoom->Wrap(-1);
	bSizer42->Add(txtGrpZoom,0,wxTOP|wxRIGHT|wxLEFT,5);

	slideGrpZoom = new wxSlider(pageGrp,wxID_SLIDE_GRP_ZOOM,1,1,4,wxDefaultPosition,wxDefaultSize,wxSL_AUTOTICKS|wxSL_BOTTOM|wxSL_HORIZONTAL|wxSL_LABELS|wxSL_MIN_MAX_LABELS);
	bSizer42->Add(slideGrpZoom,0,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);

	txtGrpGamma = new wxStaticText(pageGrp,wxID_ANY,wxT("Gamma correction:"),wxDefaultPosition,wxDefaultSize,0);
	txtGrpGamma->Wrap(-1);
	bSizer42->Add(txtGrpGamma,0,wxTOP|wxRIGHT|wxLEFT,5);

	slideGrpGamma = new wxSlider(pageGrp,wxID_SLIDE_GRP_GAMMA,50,0,100,wxDefaultPosition,wxDefaultSize,wxSL_AUTOTICKS|wxSL_BOTTOM|wxSL_HORIZONTAL);
	bSizer42->Add(slideGrpGamma,0,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);


	bSizer41->Add(bSizer42,1,wxEXPAND,5);

	m_staticline18 = new wxStaticLine(pageGrp,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_VERTICAL);
	bSizer41->Add(m_staticline18,0,wxEXPAND | wxALL,5);

	wxBoxSizer* bSizer43;
	bSizer43 = new wxBoxSizer(wxVERTICAL);

	cbGrpOrigin = new wxCheckBox(pageGrp,wxID_CB_GRP_ORG,wxT("Show origin"),wxDefaultPosition,wxDefaultSize,0);
	bSizer43->Add(cbGrpOrigin,0,wxALL,5);

	cbGrpFireOrg = new wxCheckBox(pageGrp,wxID_CB_GRP_FIRE_ORG,wxT("Show fire origin"),wxDefaultPosition,wxDefaultSize,0);
	bSizer43->Add(cbGrpFireOrg,0,wxBOTTOM|wxRIGHT|wxLEFT,5);

	cbGrpFireMean = new wxCheckBox(pageGrp,wxID_CB_GRP_FIRE_ORG_MEAN,wxT("Show fire origins mean"),wxDefaultPosition,wxDefaultSize,0);
	bSizer43->Add(cbGrpFireMean,0,wxBOTTOM|wxRIGHT|wxLEFT,5);

	cbGrpFireCenter = new wxCheckBox(pageGrp,wxID_CB_GRP_FIRE_CENTER,wxT("Show turret origin"),wxDefaultPosition,wxDefaultSize,0);
	bSizer43->Add(cbGrpFireCenter,0,wxBOTTOM|wxRIGHT|wxLEFT,5);


	bSizer41->Add(bSizer43,1,wxEXPAND,5);


	bSizer36->Add(bSizer41,0,wxEXPAND,5);


	pageGrp->SetSizer(bSizer36);
	pageGrp->Layout();
	bSizer36->Fit(pageGrp);
	pages->AddPage(pageGrp,wxT("Unit graphics"),true);

	szArt->Add(pages,1,wxEXPAND | wxALL,5);


	szMain->Add(szArt,1,wxEXPAND,5);

	m_listBox11 = new wxListBox(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,0,NULL,wxLB_ALWAYS_SB);
	m_listBox11->SetMinSize(wxSize(200,-1));

	szMain->Add(m_listBox11,0,wxALL|wxEXPAND,5);


	this->SetSizer(szMain);
	this->Layout();

	this->Centre(wxBOTH);

	// === AUTO GENERATED END ===

	// unit ID is read only
	spinID->Enable(false);

	m_spell_data = NULL;
	m_unit = NULL;
	m_update = false;
	
	// close
	Bind(wxEVT_CLOSE_WINDOW,&FormUnits::OnClose,this,this->m_windowId);
	Bind(wxEVT_MENU,&FormUnits::OnSaveAuxClick,this,wxID_MM_SAVE_AUX);
	Bind(wxEVT_MENU,&FormUnits::OnCloseClick,this,wxID_MM_EXIT);
	Bind(wxEVT_MENU,&FormUnits::OnCloseClick,this,wxID_MM_SET);

	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormUnits::OnSelectUnit,this,wxID_LB_UNITS);
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormUnits::OnSelectArt,this,wxID_LB_ART);

	// icon canvas stuff
	icon_canvas->SetDoubleBuffered(true);
	icon_canvas->Bind(wxEVT_PAINT,&FormUnits::OnPaintIcon,this,wxID_CANVAS_ICON);

	// art canvas stuff
	art_canvas->SetDoubleBuffered(true);
	art_canvas->Bind(wxEVT_PAINT,&FormUnits::OnPaintArt,this,wxID_CANVAS_ART);
	
	// graphics canvas stuff
	grp_canvas->SetDoubleBuffered(true);
	grp_canvas->Bind(wxEVT_PAINT,&FormUnits::OnPaintGrp,this,wxID_CANVAS_GRP);
	grp_canvas->Bind(wxEVT_LEFT_DOWN,&FormUnits::OnGrpCanvasLMouseDown,this,wxID_CANVAS_GRP);
	grp_canvas->Bind(wxEVT_MOTION,&FormUnits::OnGrpCanvasMouseMove,this,wxID_CANVAS_GRP);
	grp_canvas->Bind(wxEVT_ENTER_WINDOW,&FormUnits::OnGrpCanvasMouseInOut,this,wxID_CANVAS_GRP);
	grp_canvas->Bind(wxEVT_LEAVE_WINDOW,&FormUnits::OnGrpCanvasMouseInOut,this,wxID_CANVAS_GRP);
	grp_canvas->SetCursor(*wxCROSS_CURSOR);

	// nothing to copy
	m_grp_copy_buffer.res = NULL;

	slideAzim->SetTickFreq(1);
	slideSlope->SetTickFreq(1);
	slideFrame->SetTickFreq(1);
	slideGrpGamma->SetRange(50,200);
	slideGrpGamma->SetValue(150);
	slideGrpGamma->SetTickFreq(10);	

	Bind(wxEVT_MENU,&FormUnits::OnCopyGrpOriginsClick,this,wxID_MM_COPY_GRP_ORG);
	Bind(wxEVT_MENU,&FormUnits::OnCopyGrpOriginsClick,this,wxID_MM_PASTE_GRP_ORG);
		
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormUnits::OnGrpResChange,this,wxID_CHB_GRP_TYPE);
	
	Bind(wxEVT_COMMAND_SLIDER_UPDATED,&FormUnits::OnChangeGrpFrame,this,wxID_SLIDE_AZIM);
	Bind(wxEVT_COMMAND_SLIDER_UPDATED,&FormUnits::OnChangeGrpFrame,this,wxID_SLIDE_SLOPE);
	Bind(wxEVT_COMMAND_SLIDER_UPDATED,&FormUnits::OnChangeGrpFrame,this,wxID_SLIDE_FRAME);
	Bind(wxEVT_COMMAND_SLIDER_UPDATED,&FormUnits::OnChangeGrpFrame,this,wxID_SLIDE_GRP_GAMMA);
	Bind(wxEVT_COMMAND_SLIDER_UPDATED,&FormUnits::OnChangeGrpFrame,this,wxID_SLIDE_GRP_ZOOM);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormUnits::OnChangeGrpFrame,this,wxID_CB_GRP_ORG);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormUnits::OnChangeGrpFrame,this,wxID_CB_GRP_FIRE_ORG);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormUnits::OnChangeGrpFrame,this,wxID_CB_GRP_FIRE_CENTER);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormUnits::OnChangeGrpFrame,this,wxID_CB_GRP_FIRE_ORG_MEAN);

	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormUnits::OnSelectSpecUnit,this,wxID_UNIT_TYPE);


	

	

}

FormUnits::~FormUnits()
{
}

void FormUnits::OnClose(wxCloseEvent& ev)
{
	wxPostEvent(GetParent(),ev);
	ev.Skip();
	Destroy();
}
// close form
void FormUnits::OnCloseClick(wxCommandEvent& event)
{
	m_update = (event.GetId() == wxID_MM_SET) && m_unit;
	if(m_update && lboxUnits->GetSelection() >= 0)
	{
		// update unit record (if attached)		
		m_unit->map->HaltUnitRanging(true);
		m_unit->MorphUnit(m_spell_data->units->GetUnit(lboxUnits->GetSelection()));		
		m_unit->was_moved = true;
		EditUnit();
		m_unit->map->AssignUnitID(m_unit);
		m_unit->map->ResumeUnitRanging(false);
	}
	else if(m_unit && m_new_unit)
	{
		// cancel - remove temp new unit
		delete m_unit;
		m_unit = NULL;
		m_update = false;
	}

	Close();
}

// check if unit shall be updated or not?
bool FormUnits::DoUpdateUnit()
{
	return(m_update);
}

// check if unit shall be added to map?
MapUnit *FormUnits::DoAddUnit()
{
	if(m_update && m_new_unit && m_unit)
		return(m_unit);
	else
		return(NULL);
}

// set spellcross data ref
void FormUnits::SetSpellData(SpellData* spelldata)
{
	m_spell_data = spelldata;

	// fill units list
	lboxUnits->Freeze();
	lboxUnits->Clear();
	for(auto & unit : m_spell_data->units->GetUnits())
		lboxUnits->Append(string_format("#%02d: %s ",lboxUnits->GetCount(),unit->name));
	lboxUnits->Thaw();
	if(lboxUnits->GetCount())
		lboxUnits->Select(0);

	SelectUnit();
}

// set spellcross map unit ref (or null to deselect unit)
void FormUnits::SetMapUnit(MapUnit *unit, SpellMap* map)
{
	m_new_unit = false;
	if(!unit && map)
	{
		// no map unit link provided - make new unit
		unit = new MapUnit(map);
		
		unit->unit = m_spell_data->units->GetUnit(unit->type_id);
		unit->is_event = true;
		unit->behave = MapUnitType::Values::NormalUnit;
		unit->spec_type = MapUnitType::Values::MissionUnit;
		unit->ResetHealth();

		// try assign new unit ID
		map->AssignUnitID(unit);

		m_new_unit = true;
	}
	
	m_unit = unit;
	SelectUnit(unit);
}

// on unit selection change
void FormUnits::OnSelectUnit(wxCommandEvent& event)
{
	SelectUnit();	
}

// on unit selection change
void FormUnits::OnSelectSpecUnit(wxCommandEvent& event)
{
	if(m_new_unit)
	{
		EditUnit();
		SelectUnit();
	}
}

// fill form stuff when unit selected
void FormUnits::SelectUnit(MapUnit *unit)
{
	if(unit)
	{
		// search map unit in list and select it:
		for(int uid = 0; uid < m_spell_data->units->Count(); uid++)
		{
			auto *item = m_spell_data->units->GetUnit(uid);
			if(item == unit->unit)
			{
				lboxUnits->Select(uid);
				break;
			}
		}
	}
	// get unit selection:
	if(!lboxUnits->GetCount() || lboxUnits->GetSelection() < 0)
		return;
	auto sel = lboxUnits->GetSelection();
	auto *unit_rec = m_spell_data->units->GetUnit(sel);

	// show some properties
	props->Freeze();
	props->Clear();
	props->Append(new wxStringProperty(wxT("Sight"),wxT(""),string_format("%d",unit_rec->sdir)));
	props->Append(new wxStringProperty(wxT("Move range"),wxT(""),string_format("%d",unit_rec->apw)));
	props->Append(new wxStringProperty(wxT("Defence"),wxT(""),string_format("%d",unit_rec->defence)));		
	props->Append(new wxStringProperty(wxT("Light attack"),wxT(""),string_format("%d",unit_rec->attack_light)));
	props->Append(new wxStringProperty(wxT("Armor attack"),wxT(""),string_format("%d",unit_rec->attack_armored)));
	props->Append(new wxStringProperty(wxT("Air attack"),wxT(""),string_format("%d",unit_rec->attack_air)));
	props->Append(new wxStringProperty(wxT("Objects attack"),wxT(""),string_format("%d",unit_rec->attack_objects)));
	props->Append(new wxStringProperty(wxT("Man count"),wxT(""),string_format("%d",unit_rec->cnt)));
	props->Thaw();	

	// show unit ID
	if(m_unit)
		spinID->SetValue(m_unit->id);

	// set health control
	int health = unit_rec->GetHP();
	if(m_unit)
		health = m_unit->man;
	spinHealth->SetRange(1, unit_rec->cnt);
	spinHealth->SetValue(health);

	// XP points
	int xp = 0;
	if(m_unit)
		xp = m_unit->experience_init;
	slideXP->SetValue(xp);

	// show map unit name
	string name = "";
	if(m_unit)
		name = m_unit->name;
	txtName->SetValue(name);
		
	// unit behaviour
	if(m_unit)
	{		
		if(m_unit->is_event)
		{
			// no beahave when event-unit
			chUnitBehave->Clear();
			chUnitBehave->Enable(false);

			// unit spec type list
			std::vector<MapUnitType::Values> spec_list ={MapUnitType::Values::EnemyUnit, MapUnitType::Values::ArmyUnit, MapUnitType::Values::MissionUnit, MapUnitType::Values::SpecUnit};
			chUnitType->Clear();
			if(m_new_unit)
				chUnitType->Append("Non-event EnemyUnit");
			for(auto spec: spec_list)
				chUnitType->Append(MapUnitType(spec).GetString());
			auto bid = chUnitType->FindString(m_unit->spec_type.GetString());
			if(bid >= 0)
			{
				chUnitType->Enable(true);
				chUnitType->Select(bid);
			}
		}
		else
		{
			// show unit behaviour
			std::vector<MapUnitType::Values> beh_list ={MapUnitType::Values::NormalUnit, MapUnitType::Values::PatrolUnit, MapUnitType::Values::WaitForContact, MapUnitType::Values::ToughDefence};
			chUnitBehave->Clear();
			for(auto beh: beh_list)
				chUnitBehave->Append(MapUnitType(beh).GetString());
			auto bid = chUnitBehave->FindString(m_unit->behave.GetString());
			if(bid >= 0)
			{
				chUnitBehave->Enable(true);
				chUnitBehave->Select(bid);
			}

			if(!m_new_unit)
			{
				chUnitType->Select(0);
				chUnitType->Enable(false);
			}
			else
			{
				chUnitType->Select(0);
				chUnitType->Enable(true);
			}
		}		
	}
	else
	{
		chUnitBehave->Clear();
		chUnitBehave->Enable(false);
		chUnitType->Clear();
		chUnitType->Enable(false);
	}


	// fill art list
	lboxArt->Freeze();
	lboxArt->Clear();
	for(auto & art : unit_rec->GetArtList(m_spell_data->info))
		lboxArt->Append(art);
	lboxArt->Thaw();
	if(lboxArt->GetCount())
		lboxArt->Select(0);

	// paint icon
	icon_canvas->Refresh();
	
	// paint art
	art_canvas->Refresh();

	// show info text
	WriteInfo();

	
	// fill unit graphics selector
	chbGrpType->Freeze();
	chbGrpType->Clear();
	for(int gid = 0; gid <= GRP_DIE; gid++)
	{
		FSU_resource *res = NULL;
		string title = "";
		if(gid == GRP_BASE)
		{
			res = unit_rec->gr_base;
			if(unit_rec->hasTurret())
				title = "Unit base";
			else
				title = "Unit";
		}
		else if(gid == GRP_AUX && unit_rec->gr_aux)
		{
			res =unit_rec->gr_aux;
			title = "Turret";
		}
		else if(gid == GRP_LIGHT && unit_rec->gr_attack_light)
		{
			res = unit_rec->gr_attack_light;
			title = "Light attack";
		}
		else if(gid == GRP_ARMOR && unit_rec->gr_attack_armor)
		{
			res = unit_rec->gr_attack_armor;
			title = "Armor attack";
		}
		else if(gid == GRP_AIR && unit_rec->gr_attack_air)
		{
			res = unit_rec->gr_attack_air;
			title = "Air attack";
		}
		else if(gid == GRP_ACTION && unit_rec->gr_action)
		{
			res = unit_rec->gr_action;
			title = "Special action";
		}
		else if(gid == GRP_DIE && unit_rec->gr_die)
		{
			res = unit_rec->gr_die;
			title = "Die animation";
		}
		else
			continue;
	
		for(int cid = 0; cid < 2; cid++)
		{
			TGrpRes rec;
			rec.res = res;
			rec.type = gid;
			rec.flag = 0;

			string title2;
			if(cid == 0 && res->stat.azimuths)
			{
				title2 = title + " (static)";
				rec.flag = GRP_STATIC;
			}
			else if(cid == 1 && (res->anim.azimuths || res->anim.slopes))
			{
				title2 = title + " (animation)";
				rec.flag = GRP_ANIMATION;
			}
			else
				continue;

			auto *cdata = new TGrpRes(rec);
			chbGrpType->Append(title2,cdata);
		}
	}
	chbGrpType->Thaw();
	if(chbGrpType->GetCount() && chbGrpType->GetSelection() < 0 || chbGrpType->GetSelection() >= chbGrpType->GetCount())
		chbGrpType->Select(0);
	ChangeGrpResource();
}

// edit unit params
void FormUnits::EditUnit()
{
	if(!m_unit)
		return;
	
	// update spec type selection
	auto utid = chUnitType->GetSelection();
	if(utid >= 0)
	{
		MapUnitType spec_type;
		spec_type = chUnitType->GetString(utid);
		m_unit->spec_type = spec_type;
		if(spec_type == MapUnitType::Values::Unknown)
		{
			m_unit->is_enemy = true;
			m_unit->is_event = false;
		}
		if(m_unit->spec_type == MapUnitType::Values::SpecUnit || m_unit->spec_type == MapUnitType::Values::MissionUnit || m_unit->spec_type == MapUnitType::Values::ArmyUnit)
		{
			m_unit->is_enemy = false;
			m_unit->is_event = true;
		}
		if(m_unit->spec_type == MapUnitType::Values::EnemyUnit)
		{
			m_unit->is_enemy = true;
			m_unit->is_event = true;
		}
	}

	// update unit behaviour selection
	auto beid = chUnitBehave->GetSelection();
	if(beid >= 0)
	{
		MapUnitType behave;
		behave = chUnitBehave->GetString(beid);
		if(behave != MapUnitType::Values::Unknown && !m_unit->is_event)
			m_unit->behave = behave;
	}

	// set experience
	m_unit->experience_init = slideXP->GetValue();
	m_unit->InitExperience(m_unit->experience_init);

	// other params
	m_unit->ResetAP();
	m_unit->morale = 100;
	m_unit->man = spinHealth->GetValue();
	if(m_unit->name.compare(txtName->GetValue()) != 0)
		m_unit->name = txtName->GetValue();

}


// show unit info
void FormUnits::WriteInfo()
{
	// get art selection:
	if(!lboxArt->GetCount() || lboxArt->GetSelection() < 0)
		return;
	auto sel = lboxArt->GetSelection();
	string art_name = lboxArt->GetString(sel).ToStdString();

	vector<string> langs = {"CZ","ENG"};
	for(auto & lang : langs)
	{
		// info text file
		string art_info_name = art_name + "." + lang;
		uint8_t* txt_buf;
		int txt_size;
		if(m_spell_data->info->GetFile(art_info_name.c_str(),&txt_buf,&txt_size))
			continue;
		char *text = new char[txt_size+1];
		memset(text,'\0',txt_size+1);
		memcpy(text, txt_buf, txt_size);		

		// convert codepage
		wstring info;
		if(lang == "CZ")
			info = char2wstringCP895(text);
		else
			info = char2wstring(text);
		delete[] text;

		txtInfo->SetValue(info);		
		break;
	}
}

// on paint icon
void FormUnits::OnPaintIcon(wxPaintEvent& event)
{
	// get unit selection:
	if(!lboxUnits->GetCount() || lboxUnits->GetSelection() < 0)
		return;
	auto sel = lboxUnits->GetSelection();
	auto* unit_rec = m_spell_data->units->GetUnit(sel);
	
	// render icon
	wxBitmap* bmp = unit_rec->icon_glyph->Render(icon_canvas->GetClientSize().GetWidth(),icon_canvas->GetClientSize().GetHeight());
	wxPaintDC pdc(icon_canvas);
	pdc.DrawBitmap(*bmp,wxPoint(0,0));
	delete bmp;
}

// on art selection change
void FormUnits::OnSelectArt(wxCommandEvent& event)
{
	art_canvas->Refresh();
}

// on paint unit art
void FormUnits::OnPaintArt(wxPaintEvent& event)
{
	// get art selection:
	if(!lboxArt->GetCount() || lboxArt->GetSelection() < 0)
		return;
	auto sel = lboxArt->GetSelection();
	string art_name = lboxArt->GetString(sel).ToStdString();
	
	// graphics content
	string art_lz_name = art_name + ".LZ";
	string art_pal_name = art_name + ".PAL";

	// load LZ graphics file
	uint8_t *lz_buf;
	int lz_size;
	if(m_spell_data->info->GetFile(art_lz_name.c_str(), &lz_buf, &lz_size))
		return;

	// decode LZ to RAW image
	uint8_t *data;
	int len;
	LZWexpand *lzw = new LZWexpand(1048576);
	if(lzw->Decode(lz_buf,&lz_buf[lz_size],&data,&len))
	{
		delete lzw;
		return;
	}
	delete lzw;
	uint8_t *end = &data[len];

	// load palette
	uint8_t *art_pal;
	int art_pal_size;
	if(m_spell_data->info->GetFile(art_pal_name.c_str(),&art_pal,&art_pal_size))
		return;
	auto terr = m_spell_data->GetTerrain(0);
	uint8_t pal[256][3];
	memcpy((void*)pal,(void*)terr->pal,256*3);
	memcpy((void*)&pal[0][0],(void*)art_pal,art_pal_size);

	
	// input data dimensions
	int x_size = 437;
	int y_size = len/x_size;

	// make redner buffer
	wxBitmap bmp(art_canvas->GetClientSize(),24);
	int surf_x = bmp.GetWidth();
	int surf_y = bmp.GetHeight();

	if(x_size <= surf_x && y_size <= surf_y)
	{
		// image should fit to surface
		int x_ofs = (surf_x - x_size)/2;
		int y_ofs = (surf_y - y_size)/2;

		// render 24bit RGB data to raw bmp buffer
		uint8_t* buf = data;
		wxNativePixelData pdata(bmp);
		wxNativePixelData::Iterator p(pdata);
		for(int y = 0; y < surf_y; y++)
		{
			uint8_t* scan = p.m_ptr;
			for(int x = 0; x < surf_x; x++)
			{
				int is_visible = y >= y_ofs && y < y_ofs+y_size && x >= x_ofs && x < x_ofs+x_size && data < end;
				if(is_visible && *buf)
				{
					*scan++ = pal[*buf][2];
					*scan++ = pal[*buf][1];
					*scan++ = pal[*buf][0];
					buf++;
				}
				else
				{
					uint8_t checkers = (!(x&32) == !(y&32))?0x88:0xAA;
					*scan++ = checkers;
					*scan++ = checkers;
					*scan++ = checkers;
					if(is_visible)
						buf++;
				}
			}
			p.OffsetY(pdata,1);
		}		
	}
	// loose raw data
	delete[] data;

	// blit to screen
	wxPaintDC pdc(art_canvas);
	pdc.DrawBitmap(bmp,wxPoint(0,0));
	
}






void FormUnits::OnPaintGrp(wxPaintEvent& event)
{
	if(chbGrpType->IsEmpty() || chbGrpType->GetSelection() < 0)
		return;
	auto sel = chbGrpType->GetSelection();
	TGrpRes* grp = (TGrpRes*)chbGrpType->GetClientObject(sel);

	auto terr = m_spell_data->GetTerrain(0);

	int azim = slideAzim->GetValue();
	int slope = slideSlope->GetValue();
	int frame = slideFrame->GetValue();
	int zoom = slideGrpZoom->GetValue();

	// get sprite
	FSU_sprite *spr;
	FSU_resource::Txy *fire_pos = NULL;
	FSU_resource::Txy* fire_center = NULL;
	FSU_resource::Txy fire_mean;
	if(grp->flag == GRP_STATIC)
	{		
		spr = grp->res->stat.lists[slope][azim];
		fire_pos = &grp->res->stat.fire_origin[slope][azim];
		fire_center = &grp->res->stat.fire_center[slope];
		fire_mean = grp->res->GetStatFireOriginMean(slope);
	}
	else
	{
		if(grp->res->anim.azimuths)
		{
			spr = grp->res->anim.lists[azim][frame];		
			fire_pos = &grp->res->anim.fire_origin[azim];
		}
		else
		{
			spr = grp->res->anim.lists[slope][frame];
			fire_pos = &grp->res->anim.fire_origin[slope];
		}
		fire_center = &grp->res->anim.fire_center;
		fire_mean = grp->res->GetAnimFireOriginMean();
	}
	
	wxBitmap bmp(grp_canvas->GetClientSize().GetWidth(),grp_canvas->GetClientSize().GetHeight(),24);
	int surf_x = bmp.GetWidth();
	int surf_y = bmp.GetHeight();

	// background surface
	if(surf_x != m_grp_x || surf_y != m_grp_y)
	{
		m_grpbuf.clear();		
		m_grp_x = surf_x;
		m_grp_y = surf_y;
	}
	m_grpbuf.assign(surf_x*surf_y,250);
	uint8_t *buf = &m_grpbuf[0];
	uint8_t* buf_end = &buf[surf_x*surf_y];

	// black frame
	memset(&buf[0],254,surf_x);
	memset(&buf[(surf_y-1)*surf_x],254,surf_x);
	for(int y = 0; y < surf_y; y++)
	{
		buf[y*surf_x + 0] = 254;
		buf[y*surf_x + surf_x-1] = 254;
	}

	

	// render unit
	int org_x = surf_x/2 - (80)*zoom/2;
	int org_y = surf_y/2 - (48)*zoom/2;
	spr->Render(buf,buf_end, org_x-0, org_y, surf_x,terr->filter.darkpal,NULL,zoom);

	/*vector<int> hist(256,0);
	for(auto & pix : buffer)
		hist[pix]++;

	m_listBox11->Freeze();
	m_listBox11->Clear();
	for(int k = 0; k < 256; k++)
	{
		if(hist[k] == 1)
			m_listBox11->Append(string_format("#%03d - %d",k, hist[k]));
	}
	m_listBox11->Thaw();*/

	// render origin lines
	if(cbGrpOrigin->IsChecked())
	{
		memset(&buf[(org_y+zoom/2)*surf_x],232,surf_x);
		for(int y = 0; y < surf_y;y++)
			buf[y*surf_x + org_x+zoom/2] = 232;
	}

	// render fire origin
	if(cbGrpFireOrg->IsChecked() && fire_pos)
	{
		int pos_x = org_x + fire_pos->x*zoom+zoom/2;
		int pos_y = org_y + fire_pos->y*zoom+zoom/2;
		for(int k = -3; k <= +3; k++)
		{
			buf[(pos_y+k)*surf_x + pos_x+0] = 0xFD;
			buf[(pos_y+0)*surf_x + pos_x+k] = 0xFD;
		}
		//149
	}

	// render fire center
	if(cbGrpFireCenter->IsChecked() && fire_pos)
	{
		int pos_x = org_x + fire_center->x*zoom+zoom/2;
		int pos_y = org_y + fire_center->y*zoom+zoom/2;
		for(int k = -3; k <= +3; k++)
		{
			buf[(pos_y+3-abs(k))*surf_x + pos_x+k] = 0xFD;
			buf[(pos_y-3+abs(k))*surf_x + pos_x+k] = 0xFD;
		}
		for(int k = -2; k <= +2; k++)
		{
			buf[(pos_y+2-abs(k))*surf_x + pos_x+k] = 0xFC;
			buf[(pos_y-2+abs(k))*surf_x + pos_x+k] = 0xFC;
		}
		//149
	}

	// render fire center
	if(cbGrpFireMean->IsChecked() && fire_pos)
	{
		int pos_x = org_x + fire_mean.x*zoom+zoom/2;
		int pos_y = org_y + fire_mean.y*zoom+zoom/2;
		for(int k = -3; k <= +3; k++)
		{
			buf[(pos_y+3-abs(k))*surf_x + pos_x+k] = 149;
			buf[(pos_y-3+abs(k))*surf_x + pos_x+k] = 149;
		}
		for(int k = -2; k <= +2; k++)
		{
			buf[(pos_y+2-abs(k))*surf_x + pos_x+k] = 0xFC;
			buf[(pos_y-2+abs(k))*surf_x + pos_x+k] = 0xFC;
		}
		//149
	}

	// apply gamma
	uint8_t pal[256][3];
	memcpy((void*)pal,(void*)terr->pal,256*3);
	apply_gamma((uint8_t*)pal, 0.01*slideGrpGamma->GetValue());

	// render 24bit RGB data to raw bmp buffer
	wxNativePixelData pdata(bmp);
	wxNativePixelData::Iterator p(pdata);
	for(int y = 0; y < surf_y; y++)
	{
		uint8_t* scan = p.m_ptr;
		for(int x = 0; x < surf_x; x++)
		{
			*scan++ = pal[*buf][2];
			*scan++ = pal[*buf][1];
			*scan++ = pal[*buf][0];
			buf++;
		}
		p.OffsetY(pdata,1);
	}

	// blit to screen
	wxPaintDC pdc(grp_canvas);
	pdc.DrawBitmap(bmp,wxPoint(0,0));

}

void FormUnits::OnGrpResChange(wxCommandEvent& event)
{
	ChangeGrpResource();
}
void FormUnits::ChangeGrpResource()
{
	if(chbGrpType->IsEmpty() || chbGrpType->GetSelection() < 0)
		return;
	auto sel = chbGrpType->GetSelection();	
	TGrpRes* grp = (TGrpRes*)chbGrpType->GetClientObject(sel);

	if(grp->flag == GRP_STATIC)
	{
		slideAzim->Enable();
		slideAzim->SetRange(0,grp->res->stat.azimuths-1);
		slideSlope->Enable();
		slideSlope->SetRange(0,grp->res->stat.slopes-1);
		slideFrame->Disable();
	}
	else
	{
		slideFrame->Enable();
		slideFrame->SetRange(0,grp->res->anim.frames-1);
		if(grp->res->anim.azimuths)
		{
			slideAzim->Enable();
			slideAzim->SetRange(0,grp->res->anim.azimuths-1);
			slideSlope->Disable();
		}
		else
		{
			slideSlope->Enable();
			slideSlope->SetRange(0,grp->res->anim.slopes-1);
			slideAzim->Disable();
		}
		
	}
	grp_canvas->Refresh();
}

void FormUnits::OnChangeGrpFrame(wxCommandEvent& event)
{
	if(chbGrpType->IsEmpty() || chbGrpType->GetSelection() < 0)
		return;
	auto sel = chbGrpType->GetSelection();
	TGrpRes* grp = (TGrpRes*)chbGrpType->GetClientObject(sel);
	int azimuths;
	if(grp->flag == GRP_STATIC)
		azimuths = grp->res->stat.azimuths;
	else
		azimuths = grp->res->anim.azimuths;

	txtGrpGamma->SetLabelText(string_format("Gamma correction (%0.2f):", 0.01*slideGrpGamma->GetValue()));
	txtGrpSlope->SetLabelText(string_format("Slope '%c':",slideSlope->GetValue()+'A'));
	if(azimuths)
		txtGrpZim->SetLabelText(string_format("Azimuth (%d deg):",(90-360*slideAzim->GetValue()/azimuths) % 360));
	else
		txtGrpZim->SetLabelText("Azimuth:");

	grp_canvas->Refresh();
}

void FormUnits::OnGrpCanvasLMouseDown(wxMouseEvent& event)
{
	OnGrpCanvasMouseMove(event);
}

void FormUnits::OnGrpCanvasMouseMove(wxMouseEvent& event)
{
	int zoom = slideGrpZoom->GetValue();
	
	int surf_x = m_grp_x;
	int surf_y = m_grp_y;
	int org_x = surf_x/2 - (80)*zoom/2;
	int org_y = surf_y/2 - (48)*zoom/2;
	int x_abs = event.GetX();
	int y_abs = event.GetY();
	int x_pos = (x_abs - org_x)/zoom - zoom/2;
	int y_pos = (y_abs - org_y)/zoom - zoom/2;

	// show position
	int color = 0;
	if(x_abs <= surf_x && y_abs <= surf_y)
		color = (unsigned)m_grpbuf[x_abs + y_abs*surf_x];
	string txt = string_format("x = %d, y = %d, color = %d (0x%02X)", x_pos, y_pos, color);
	SetStatusText(txt);

	// update fire origin
	if(!event.LeftIsDown())
		return;
	
	if(chbGrpType->IsEmpty() || chbGrpType->GetSelection() < 0)
		return;
	auto sel = chbGrpType->GetSelection();
	TGrpRes* grp = (TGrpRes*)chbGrpType->GetClientObject(sel);

	

	if(grp->flag == GRP_STATIC)
	{
		int azim = slideAzim->GetValue();
		int slope = slideSlope->GetValue();

		if(event.ControlDown() && cbGrpFireCenter->IsChecked())
		{
			// set fire center (turret mount or so)
			auto& org = grp->res->stat.fire_center[slope];
			org.x = x_pos;
			org.y = y_pos;
		}
		else if(cbGrpFireOrg->IsChecked())
		{
			// set fire origin (turret end or so)
			auto &org = grp->res->stat.fire_origin[slope][azim];
			org.x = x_pos;
			org.y = y_pos;
		}

		grp_canvas->Refresh();
	}
	else if(grp->flag == GRP_ANIMATION && grp->res->anim.azimuths)
	{
		int azim = slideAzim->GetValue();

		if(event.ControlDown() && cbGrpFireCenter->IsChecked())
		{
			// set fire center (turret mount or so)
			auto& org = grp->res->anim.fire_center;
			org.x = x_pos;
			org.y = y_pos;
		}
		else if(cbGrpFireOrg->IsChecked())
		{
			// set fire origin (turret end or so)
			auto& org = grp->res->anim.fire_origin[azim];
			org.x = x_pos;
			org.y = y_pos;
		}

		grp_canvas->Refresh();
	}

}

// mouse enter/leave graphics canvas
void FormUnits::OnGrpCanvasMouseInOut(wxMouseEvent& event)
{
	/*if(event.GetEventType() == wxEVT_ENTER_WINDOW)
	{
		grp_canvas->SetCursor(
	}*/
}

// copy/paste fire origins from one unit to other
void FormUnits::OnCopyGrpOriginsClick(wxCommandEvent& event)
{
	if(event.GetId() == wxID_MM_COPY_GRP_ORG)
	{
		if(chbGrpType->IsEmpty() || chbGrpType->GetSelection() < 0)
			return;
		auto sel = chbGrpType->GetSelection();
		m_grp_copy_buffer = *(TGrpRes*)chbGrpType->GetClientObject(sel);
	}
	else if(event.GetId() == wxID_MM_PASTE_GRP_ORG)
	{
		if(!m_grp_copy_buffer.res)
			return;
		
		if(chbGrpType->IsEmpty() || chbGrpType->GetSelection() < 0)
			return;
		auto sel = chbGrpType->GetSelection();
		auto *grp = (TGrpRes*)chbGrpType->GetClientObject(sel);

		if(grp->type != m_grp_copy_buffer.type || grp->flag != m_grp_copy_buffer.flag)
			return;

		if(grp->flag == GRP_STATIC)
		{
			if(grp->res->stat.azimuths != m_grp_copy_buffer.res->stat.azimuths || grp->res->stat.slopes != m_grp_copy_buffer.res->stat.slopes)
				return;
			for(int k = 0; k < grp->res->stat.slopes; k++)
			{
				grp->res->stat.fire_center[k] = m_grp_copy_buffer.res->stat.fire_center[k];
				grp->res->stat.fire_origin[k] = m_grp_copy_buffer.res->stat.fire_origin[k];
			}
		}
		else if(grp->flag == GRP_ANIMATION)
		{
			if(grp->res->anim.azimuths != m_grp_copy_buffer.res->anim.azimuths)
				return;
			grp->res->anim.fire_center = m_grp_copy_buffer.res->anim.fire_center;
			grp->res->anim.fire_origin = m_grp_copy_buffer.res->anim.fire_origin;
		}

		grp_canvas->Refresh();		
	}
}

// save aux data for FSU archive (fire origins, etc.)
void FormUnits::OnSaveAuxClick(wxCommandEvent& event)
{
	// split path to folder and file
	std::filesystem::path last_path = m_spell_data->units_fsu->GetAuxDataPath();
	wstring dir = last_path.parent_path(); dir += wstring(L"\\");
	wstring name = last_path.filename();

	// show open dialog
	wxFileDialog saveFileDialog(this,_("Save FSU auxiliary data"),dir,name,"FSA (*.fsa)|*.fsa",wxFD_SAVE);
	if(saveFileDialog.ShowModal() == wxID_CANCEL)
		return;
	wstring path = wstring(saveFileDialog.GetPath().ToStdWstring());

	// try save aux data
	m_spell_data->units_fsu->SaveAuxData(path);
}