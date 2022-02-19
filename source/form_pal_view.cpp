///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_pal_view.h"
#include "sprites.h"
#include "other.h"

//#include <filesystem>

///////////////////////////////////////////////////////////////////////////

FormPalView::FormPalView(wxWindow* parent,SpellData* spell_data,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style) : wxFrame(parent,id,title,pos,size,style)
{
	this->spell_data = spell_data;
	spell_map = NULL;

	// === AUTO GENERATED START ===	

	this->SetSizeHints(wxSize(300,200),wxDefaultSize);
	this->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENU));

	mMenu = new wxMenuBar(0);
	mmFile = new wxMenu();
	wxMenuItem* mmClose;
	mmClose = new wxMenuItem(mmFile,wxID_MM_CLOSE,wxString(wxT("Close")),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmClose);

	mMenu->Append(mmFile,wxT("File"));

	mmTerrain = new wxMenu();
	mMenu->Append(mmTerrain,wxT("Terrain"));

	mmFilter = new wxMenu();
	mMenu->Append(mmFilter,wxT("Filter"));

	this->SetMenuBar(mMenu);

	wxBoxSizer* szCanvas;
	szCanvas = new wxBoxSizer(wxVERTICAL);

	m_staticText28 = new wxStaticText(this,wxID_ANY,wxT("Palette:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText28->Wrap(-1);
	szCanvas->Add(m_staticText28,0,wxTOP|wxRIGHT|wxLEFT,5);

	canvas = new wxPanel(this,wxID_CANVAS,wxDefaultPosition,wxDefaultSize,wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL);
	szCanvas->Add(canvas,1,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);

	m_staticText29 = new wxStaticText(this,wxID_ANY,wxT("Selected color:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText29->Wrap(-1);
	szCanvas->Add(m_staticText29,0,wxTOP|wxRIGHT|wxLEFT,5);

	color = new wxPanel(this,wxID_COLOR,wxDefaultPosition,wxSize(-1,20),wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL);
	color->SetMaxSize(wxSize(-1,20));

	szCanvas->Add(color,1,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);


	this->SetSizer(szCanvas);
	this->Layout();
	sbar = this->CreateStatusBar(1,wxSTB_SIZEGRIP,wxID_STAT_BAR);
	timer.SetOwner(this,wxID_TIMER);

	this->Centre(wxBOTH);
	

	// === AUTO GENERATED END ===
	

	// generate terrain menu content
	for(int k = 0;k<spell_data->GetTerrainCount();k++)
	{
		Terrain* terr = spell_data->GetTerrain(k);
		mmTerrain->Append(TERR_ID0 + k,terr->name,wxEmptyString,wxITEM_RADIO);
		Bind(wxEVT_MENU,&FormPalView::OnTerrainChange,this,TERR_ID0 + k);
	}

	Bind(wxEVT_CLOSE_WINDOW, &FormPalView::OnClose, this, this->m_windowId);
	Bind(wxEVT_MENU,&FormPalView::OnCloseClick,this,wxID_MM_CLOSE);
	
	sel_pos_x = -1;
	canvas->Bind(wxEVT_MOTION,&FormPalView::OnCanvasMouseMove,this);
	canvas->Bind(wxEVT_LEAVE_WINDOW,&FormPalView::OnCanvasMouseLeave,this);

	// canvas stuff:
	canvas->SetDoubleBuffered(true);
	canvas->Bind(wxEVT_PAINT,&FormPalView::OnPaintCanvas,this);
	color->SetDoubleBuffered(true);
	color->Bind(wxEVT_PAINT,&FormPalView::OnPaintColor,this);

	Connect(wxEVT_TIMER,wxTimerEventHandler(FormPalView::OnTimer),NULL,this);
	relative_time = 0;
	timer.Start(200);

	// default map
	SetMap(NULL);
}

FormPalView::~FormPalView()
{
}

void FormPalView::OnClose(wxCloseEvent& ev)
{
	wxPostEvent(GetParent(), ev);
	ev.Skip();
	Destroy();
}

// close form
void FormPalView::OnCloseClick(wxCommandEvent& event)
{
	Close();
}


// set current map poitner
void FormPalView::SetMap(SpellMap* map)
{
	spell_map = map;
	for(int k = 0; k < spell_data->GetTerrainCount(); k++)
	{
		Terrain* terr = spell_data->GetTerrain(k);
		wxString lab = terr->name;
		if(map && std::strcmp(map->terrain->name,spell_data->GetTerrain(k)->name) == 0)
			lab = lab + " (active)";
		mmTerrain->SetLabel(TERR_ID0 + k,lab);
	}

	canvas->Refresh();
}

// find terrain selected
Terrain* FormPalView::FindTerrain()
{
	for(int k = 0;k<spell_data->GetTerrainCount();k++)
	{
		if(GetMenuBar()->FindItem(TERR_ID0 + k)->IsChecked())
		{
			// found selection
			Terrain* terr = spell_data->GetTerrain(k);
			return(terr);
		}
	}
	return(NULL);
}



void FormPalView::OnTerrainChange(wxCommandEvent& event)
{
	canvas->Refresh();
}



// render preview
void FormPalView::OnPaintCanvas(wxPaintEvent& event)
{	
	// make render buffer
	wxBitmap bmp(canvas->GetClientSize(),24);

	// get this terrain
	Terrain* terrain = FindTerrain();
	if(terrain)
	{
		// render palette
		terrain->RenderPalette(bmp, relative_time);
	}
	
	// blit to screen
	wxPaintDC pdc(canvas);
	pdc.DrawBitmap(bmp,wxPoint(0,0));
}


void FormPalView::OnCanvasMouseMove(wxMouseEvent& event)
{
	sel_pos_x = event.GetX();
	color->Refresh();
}
void FormPalView::OnCanvasMouseLeave(wxMouseEvent& event)
{
	sel_pos_x = -1;
	color->Refresh();
}

// render color preview
void FormPalView::OnPaintColor(wxPaintEvent& event)
{
	// make render buffer
	wxBitmap bmp(color->GetClientSize(),24);

	wxString state = "";

	// get this terrain
	Terrain* terrain = FindTerrain();
	if(terrain)
	{
		// render palette
		int color = terrain->RenderPaletteColor(bmp, canvas->GetClientSize().GetWidth(),sel_pos_x);		
		if(color >= 0)
			state = string_format("Color = #%d (0x%02X), R = %d, G = %d, B = %d, RGB = 0x%02X%02X%02X", color, color,
				terrain->pal[color][0],terrain->pal[color][1],terrain->pal[color][2],
				terrain->pal[color][0],terrain->pal[color][1],terrain->pal[color][2]);
	}

	// blit to screen
	wxPaintDC pdc(color);
	pdc.DrawBitmap(bmp,wxPoint(0,0));
	
	SetStatusText(state);
}


// map animation periodic refresh tick
void FormPalView::OnTimer(wxTimerEvent& event)
{
	relative_time++;
	canvas->Refresh();
	color->Refresh();
}
