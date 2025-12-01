///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_anm.h"

///////////////////////////////////////////////////////////////////////////

FormANM::FormANM(wxWindow* parent,SpellData* spell_data,bool is_pnm,wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{		
	// === AUTO GENERATED STUFF STARTS HERE ===

	this->SetSizeHints(wxSize(700,500),wxDefaultSize);
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENU));

	m_menubar10 = new wxMenuBar(0);
	m_menu19 = new wxMenu();
	wxMenuItem* mmSelect;
	mmSelect = new wxMenuItem(m_menu19,wxID_MM_SELECT,wxString(wxT("Select and Close")) + wxT('\t') + wxT("Enter"),wxEmptyString,wxITEM_NORMAL);
	m_menu19->Append(mmSelect);

	wxMenuItem* mmClose;
	mmClose = new wxMenuItem(m_menu19,wxID_MM_CLOSE,wxString(wxT("Close")) + wxT('\t') + wxT("Esc"),wxEmptyString,wxITEM_NORMAL);
	m_menu19->Append(mmClose);

	m_menubar10->Append(m_menu19,wxT("File"));

	mmTerrain = new wxMenu();
	m_menubar10->Append(mmTerrain,wxT("Terrain"));

	this->SetMenuBar(m_menubar10);

	wxBoxSizer* bSizer77;
	bSizer77 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer78;
	bSizer78 = new wxBoxSizer(wxVERTICAL);

	txtResources = new wxStaticText(this,wxID_TXT_RESOURCES,wxT("ANM resources:"),wxDefaultPosition,wxDefaultSize,0);
	txtResources->Wrap(-1);
	bSizer78->Add(txtResources,0,wxLEFT|wxRIGHT|wxTOP,5);

	lbList = new wxListBox(this,wxID_LB_LIST,wxDefaultPosition,wxSize(150,-1),0,NULL,wxLB_ALWAYS_SB);
	bSizer78->Add(lbList,1,wxBOTTOM|wxLEFT|wxRIGHT,5);


	bSizer77->Add(bSizer78,0,wxEXPAND,5);

	wxBoxSizer* bSizer79;
	bSizer79 = new wxBoxSizer(wxVERTICAL);

	txtFrames = new wxStaticText(this,wxID_TXT_FRAMES,wxT("Frames:"),wxDefaultPosition,wxDefaultSize,0);
	txtFrames->Wrap(-1);
	bSizer79->Add(txtFrames,0,wxLEFT|wxRIGHT|wxTOP,5);

	lbFrames = new wxListBox(this,wxID_LB_FRAMES,wxDefaultPosition,wxSize(150,-1),0,NULL,wxLB_ALWAYS_SB);
	bSizer79->Add(lbFrames,1,wxBOTTOM|wxLEFT|wxRIGHT,5);

	cbAnimate = new wxCheckBox(this,wxID_CB_ANIM,wxT("Animate"),wxDefaultPosition,wxDefaultSize,0);
	bSizer79->Add(cbAnimate,0,wxALL,5);


	bSizer77->Add(bSizer79,0,wxEXPAND,5);

	wxBoxSizer* bSizer80;
	bSizer80 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer(wxVERTICAL);


	bSizer5->Add(0,0,0,wxALL|wxEXPAND,4);

	wxStaticBoxSizer* sizerCanvas;
	sizerCanvas = new wxStaticBoxSizer(new wxStaticBox(this,wxID_ANY,wxT(" Frame view: ")),wxVERTICAL);

	canvas = new wxPanel(sizerCanvas->GetStaticBox(),wxID_CANVAS,wxDefaultPosition,wxDefaultSize,wxTAB_TRAVERSAL);
	sizerCanvas->Add(canvas,1,wxALL|wxEXPAND,5);


	bSizer5->Add(sizerCanvas,1,wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT,5);

	wxBoxSizer* sizeOffsets;
	sizeOffsets = new wxBoxSizer(wxHORIZONTAL);

	m_staticText88 = new wxStaticText(this,wxID_ANY,wxT("X offset:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText88->Wrap(-1);
	sizeOffsets->Add(m_staticText88,0,wxALIGN_CENTER_VERTICAL|wxALL,5);

	spinXofs = new wxSpinCtrl(this,wxID_SPIN_XOFS,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxSP_ARROW_KEYS,0,10,0);
	sizeOffsets->Add(spinXofs,1,wxALL,5);

	m_staticText89 = new wxStaticText(this,wxID_ANY,wxT("Y offset:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText89->Wrap(-1);
	sizeOffsets->Add(m_staticText89,0,wxALIGN_CENTER_VERTICAL|wxALL,5);

	spinYofs = new wxSpinCtrl(this,wxID_SPIN_YOFS,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxSP_ARROW_KEYS,0,10,0);
	sizeOffsets->Add(spinYofs,1,wxALL,5);


	bSizer5->Add(sizeOffsets,0,wxEXPAND,5);

	txtGamma = new wxStaticText(this,wxID_TXT_GAMMA,wxT("Set gamma correction:"),wxDefaultPosition,wxDefaultSize,0);
	txtGamma->Wrap(-1);
	bSizer5->Add(txtGamma,0,wxLEFT|wxTOP,5);

	slideGamma = new wxSlider(this,wxID_SLIDE_GAMMA,1300,500,2000,wxDefaultPosition,wxDefaultSize,wxSL_HORIZONTAL);
	bSizer5->Add(slideGamma,0,wxEXPAND|wxLEFT|wxRIGHT|wxTOP,1);

	cbZoom = new wxCheckBox(this,wxID_CB_ZOOM,wxT("Zoom 2x"),wxDefaultPosition,wxDefaultSize,0);
	bSizer5->Add(cbZoom,0,wxALL,5);


	bSizer80->Add(bSizer5,1,wxEXPAND,5);


	bSizer77->Add(bSizer80,1,wxEXPAND,5);


	this->SetSizer(bSizer77);
	this->Layout();

	this->Centre(wxBOTH);

	// === AUTO GENERATED STUFF STARTS HERE ===

	// set icon
	wxIcon appIcon;
	appIcon.LoadFile("IDI_ICON_SPR",wxBITMAP_TYPE_ICO_RESOURCE);
	if(appIcon.IsOk())
		SetIcon(appIcon);

	// back ref to spellcross data
	m_spell_data = spell_data;

	// no anim selected
	m_terrain = NULL;
	m_anim = NULL;
	m_pnm = NULL;
	m_anm_was_set = false;
	m_is_pnm = is_pnm;

	
	Bind(wxEVT_CLOSE_WINDOW,&FormANM::OnClose,this,this->m_windowId);
	Bind(wxEVT_MENU,&FormANM::OnCloseClick,this,wxID_MM_CLOSE);
	Bind(wxEVT_MENU,&FormANM::OnSelectClick,this,wxID_MM_SELECT);
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormANM::OnSelectANM,this,wxID_LB_LIST);
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormANM::OnSelectFrame,this,wxID_LB_FRAMES);
	canvas->Bind(wxEVT_PAINT,&FormANM::OnCanvasRepaint,this);
	Bind(wxEVT_COMMAND_SLIDER_UPDATED,&FormANM::OnChangeGamma,this,wxID_SLIDE_GAMMA);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormANM::OnChangeZoom,this,wxID_CB_ZOOM);

	if(m_is_pnm)
	{
		SetTitle("Objects layer animations PNM");
		txtResources->SetLabelText("PNM resources:");
		spinXofs->Enable(true);
		spinYofs->Enable(true);
		spinXofs->SetRange(-100,+100);
		spinYofs->SetRange(-100,+100);
	}
	else
	{
		SetTitle("Terrain layer animations ANM");
		txtResources->SetLabelText("ANM resources:");
		spinXofs->Enable(false);
		spinYofs->Enable(false);
	}

	// generate terrain menu content
	for(int k = 0; k < m_spell_data->GetTerrainCount(); k++)
	{
		Terrain* terr = spell_data->GetTerrain(k);
		mmTerrain->Append(TERR_ID0 + k,terr->name,wxEmptyString,wxITEM_RADIO);
		Bind(wxEVT_MENU,&FormANM::OnTerrainChange,this,TERR_ID0 + k);
	}
	
	// default 2x zoom
	cbZoom->SetValue(true);

	// default gamma 1.50
	slideGamma->SetValue(1500);

	// default animate
	cbAnimate->SetValue(true);

	// stops flickering
	canvas->SetDoubleBuffered(true);

	// select default terrain
	SelectTerrain();
	SelectANM();
	canvas->Refresh();

	// make animation timer
	m_timer.SetOwner(this);
	this->Connect(wxEVT_TIMER,wxTimerEventHandler(FormANM::OnTimer),NULL,this);
	m_timer.Start(100);
}

// destructor
FormANM::~FormANM()
{
}

// on form close
void FormANM::OnClose(wxCloseEvent& ev)
{
	wxPostEvent(GetParent(),ev);
	ev.Skip();
	Destroy();
}

// close button
void FormANM::OnCloseClick(wxCommandEvent& event)
{
	// no exit anim selection
	m_terrain = NULL;
	m_anim = NULL;
	Close();
}

// select + close button
void FormANM::OnSelectClick(wxCommandEvent& event)
{
	// exit anim selection
	Close();
}

// set viewer to given terrain and animation
void FormANM::SetANM(Terrain *terr, AnimL1* anm)
{
	SetTerrain(terr);
	if(!anm)
		return;	
	auto anm_id = lbList->FindString(anm->name);
	if(anm_id >= 0)
		lbList->Select(anm_id);
	SelectANM();
	m_anm_was_set = true;
}

// set viewer to given terrain and animation
void FormANM::SetPNM(Terrain* terr,AnimPNM* pnm,int x_ofs,int y_ofs)
{
	SetTerrain(terr);
	if(!pnm)
		return;
	auto anm_id = lbList->FindString(pnm->name);
	if(anm_id >= 0)
		lbList->Select(anm_id);

	// init PNM position offset
	spinXofs->SetValue(x_ofs);
	spinYofs->SetValue(y_ofs);

	SelectANM();
	m_anm_was_set = true;
}

// get selected terrain
Terrain* FormANM::GetSelectedTerrain()
{
	return(m_terrain);
}

// get selected animation
AnimL1* FormANM::GetSelectedAnim()
{
	return(m_anim);
}

// get selected PNM animation
AnimPNM* FormANM::GetSelectedPNM()
{
	return(m_pnm);
}

// get PNM animation xy-offsets
std::tuple<int,int> FormANM::GetPNMoffset()
{	
	return(std::tuple(spinXofs->GetValue(),spinYofs->GetValue()));
}

// was animation set by ::SetANM()?
bool FormANM::WasAnmSet()
{
	return(m_anm_was_set);
}

// was PNM animation?
bool FormANM::wasPNM()
{
	return(m_is_pnm);
}



// change terrain click
void FormANM::OnTerrainChange(wxCommandEvent& event)
{
	SelectTerrain();
	SelectANM();
	canvas->Refresh();
}

// set terrain
void FormANM::SetTerrain(Terrain *terr)
{
	m_terrain = terr;
	if(!terr)
		return;
	for(int k = 0; k < m_spell_data->GetTerrainCount(); k++)
		GetMenuBar()->FindItem(TERR_ID0 + k)->Check(GetMenuBar()->FindItem(TERR_ID0 + k)->GetItemLabel().compare(terr->name) == 0);	
	SelectTerrain();
}

// select terrain from menu
Terrain* FormANM::FindTerrain()
{
	for(int k = 0; k < m_spell_data->GetTerrainCount(); k++)
	{
		if(GetMenuBar()->FindItem(TERR_ID0 + k)->IsChecked())
		{
			// found selection
			m_terrain = m_spell_data->GetTerrain(k);
			return(m_terrain);
		}
	}
	m_terrain = NULL;
	return(m_terrain);
}

// select new terrain
void FormANM::SelectTerrain()
{
	// no anim selection
	m_anim = NULL;
	
	// loose old sprites list
	lbList->Clear();

	// found selection
	Terrain* terr = FindTerrain();
	if(!terr)
		return;

	// update window title
	if(m_is_pnm)
		SetTitle(wxString::Format("Objects layer animations viewer (%s)",terr->name));
	else
		SetTitle(wxString::Format("Terrain layer animations viewer (%s)",terr->name));

	// make list of animations
	lbList->Freeze();
	if(m_is_pnm)
	{
		for(auto& pnm: terr->pnms)
			lbList->Append(pnm->name);
	}
	else
	{
		for(auto &anm: terr->anms)
			lbList->Append(anm->name);
	}

	// select default
	if(lbList->GetCount())
		lbList->Select(0);
	else
		lbList->Select(-1);
	lbList->Thaw();	
}

// ANM resource selected
void FormANM::SelectANM()
{
	lbFrames->Clear();

	if(!m_terrain)
		return;

	// get selection
	int sel = lbList->GetSelection();
	if(sel < 0)
		return;
	auto name = lbList->GetString(sel).ToStdString();
	
	
	if(m_is_pnm)
	{
		// try get pnm
		m_pnm = m_terrain->GetPNM(name);
		if(!m_pnm)
			return;
		m_anim = NULL;

		// fill frames list
		lbFrames->Freeze();
		for(int k = 0; k < m_pnm->frames.size(); k++)
			lbFrames->Append(wxString::Format("Frame #%02d",k));
		if(lbFrames->GetCount())
			lbFrames->Select(0);
		lbFrames->Thaw();
	}
	else
	{
		// try get anim
		m_anim = m_terrain->GetANM(name);
		if(!m_anim)
			return;
		m_pnm = NULL;

		// fill frames list
		lbFrames->Freeze();
		for(int k = 0; k < m_anim->frames.size(); k++)
			lbFrames->Append(wxString::Format("Frame #%02d",k));
		if(lbFrames->GetCount())
			lbFrames->Select(0);
		lbFrames->Thaw();

		// clear offsets
		spinXofs->SetValue(0);
		spinYofs->SetValue(0);
	}

	
}

// on select ANM resource
void FormANM::OnSelectANM(wxCommandEvent& event)
{
	SelectANM();
	canvas->Refresh();
}

// select frame (render)
void FormANM::OnCanvasRepaint(wxPaintEvent& event)
{
	// get terrain
	FindTerrain();
	if(!m_terrain)
		return;
	
	// select frame
	int frame_id = lbFrames->GetSelection();
	Sprite *frame = NULL;
	if(m_is_pnm)
	{
		if(!m_pnm)
			return;
		if(frame_id < 0 || frame_id >= m_pnm->frames.size())
			return;	
		frame = m_pnm->frames[frame_id];
	}
	else
	{
		if(!m_anim)
			return;
		if(frame_id < 0 || frame_id >= m_anim->frames.size())
			return;
		frame = m_anim->frames[frame_id];
	}

	// allocate buffer
	int x_size = canvas->GetClientSize().x;
	int y_size = canvas->GetClientSize().y;
	wxBitmap m_buffer(x_size,y_size,24);
	wxPaintDC pdc(canvas);
		
	// render flags
	int flags = cbZoom->IsChecked()?(Terrain::RENDER_ZOOMX2):0;

	// get gamma
	double gamma = 0.001*(double)slideGamma->GetValue();

	// render tile to buffer
	if(m_is_pnm)
		m_terrain->RenderPNMpreview(m_buffer,frame,flags,gamma);
	else
		m_terrain->RenderSpritePreview(m_buffer,frame,flags,gamma);

	// blit to screend
	pdc.DrawBitmap(m_buffer,wxPoint(0,0));
	event.Skip();
}

// on select ANM frame
void FormANM::OnSelectFrame(wxCommandEvent& event)
{
	canvas->Refresh();
}

// map animation periodic refresh tick
void FormANM::OnTimer(wxTimerEvent& event)
{
	if(!cbAnimate->IsChecked())
		return;
	if(!lbFrames->GetCount())
		return;

	// cycle frames
	int frame_id = lbFrames->GetSelection();
	frame_id++;
	if(frame_id >= lbFrames->GetCount())
		frame_id = 0;
	lbFrames->Select(frame_id);
	
	// repaint
	canvas->Refresh();
}

// change zoom
void FormANM::OnChangeZoom(wxCommandEvent& event)
{
	canvas->Refresh();
}

// change gamma
void FormANM::OnChangeGamma(wxCommandEvent& event)
{
	double gamma = 0.001*(double)slideGamma->GetValue();
	txtGamma->SetLabel(wxString::Format(wxT("Gamma correction (%#0.2f):"),gamma));
	canvas->Refresh();
}