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
#include <wx/stdpaths.h>
#include <wx/filedlg.h>

///////////////////////////////////////////////////////////////////////////

FormPalView::FormPalView(wxWindow* parent,SpellData* spell_data,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style) : wxFrame(parent,id,title,pos,size,style)
{
	this->spell_data = spell_data;
	spell_map = NULL;

	// === AUTO GENERATED START ===	

	this->SetSizeHints(wxSize(950,300),wxDefaultSize);
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

	wxBoxSizer* szFilter;
	szFilter = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* szRed;
	szRed = new wxBoxSizer(wxVERTICAL);

	txtFilterRed = new wxStaticText(this,wxID_ANY,wxT("Red:"),wxDefaultPosition,wxDefaultSize,0);
	txtFilterRed->Wrap(-1);
	szRed->Add(txtFilterRed,0,wxTOP|wxRIGHT|wxLEFT,5);

	slideRed = new wxSlider(this,wxID_ANY,50,0,100,wxDefaultPosition,wxDefaultSize,wxSL_AUTOTICKS|wxSL_HORIZONTAL);
	szRed->Add(slideRed,0,wxEXPAND|wxTOP|wxBOTTOM,5);


	szFilter->Add(szRed,1,wxEXPAND,5);

	wxBoxSizer* szGreen;
	szGreen = new wxBoxSizer(wxVERTICAL);

	txtFilterGreen = new wxStaticText(this,wxID_ANY,wxT("Green:"),wxDefaultPosition,wxDefaultSize,0);
	txtFilterGreen->Wrap(-1);
	szGreen->Add(txtFilterGreen,0,wxTOP|wxRIGHT|wxLEFT,5);

	slideGreen = new wxSlider(this,wxID_ANY,50,0,100,wxDefaultPosition,wxDefaultSize,wxSL_AUTOTICKS|wxSL_HORIZONTAL);
	szGreen->Add(slideGreen,0,wxEXPAND|wxTOP|wxBOTTOM,5);


	szFilter->Add(szGreen,1,wxEXPAND,5);

	wxBoxSizer* szBlue;
	szBlue = new wxBoxSizer(wxVERTICAL);

	txtFilterBlue = new wxStaticText(this,wxID_ANY,wxT("Blue:"),wxDefaultPosition,wxDefaultSize,0);
	txtFilterBlue->Wrap(-1);
	szBlue->Add(txtFilterBlue,0,wxTOP|wxRIGHT|wxLEFT,5);

	slideBlue = new wxSlider(this,wxID_ANY,50,0,100,wxDefaultPosition,wxDefaultSize,wxSL_AUTOTICKS|wxSL_HORIZONTAL);
	szBlue->Add(slideBlue,0,wxEXPAND|wxTOP|wxBOTTOM,5);


	szFilter->Add(szBlue,1,wxEXPAND,5);


	szCanvas->Add(szFilter,0,wxEXPAND,5);


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
	ListFilters();

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

	Bind(wxEVT_COMMAND_SLIDER_UPDATED,&FormPalView::OnChangeFilterRGB,this);

	// defaulf filter
	filter = NULL;

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
		if(map && map->terrain && map->terrain->name.compare(spell_data->GetTerrain(k)->name) == 0)
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


void FormPalView::ListFilters()
{
	auto terr = FindTerrain();
	
	// loose old list
	for(int k = mmFilter->GetMenuItemCount(); k-- > 0;)
		mmFilter->Delete(FILTER_ID0 + k);
	filter = NULL;

	// make list of filters
	for(int k = 0; k < terr->filter.list.size(); k++)
	{
		auto filter = terr->filter.list[k];
		mmFilter->Append(FILTER_ID0 + k, wxString(filter->name),wxEmptyString,wxITEM_RADIO);
		Bind(wxEVT_MENU,&FormPalView::OnFilterChange,this,FILTER_ID0 + k);
	}
	if(terr->filter.list.size())
		mmFilter->Check(FILTER_ID0, true);

	mmFilter->Append(wxID_ANY,wxEmptyString,wxEmptyString,wxITEM_SEPARATOR);
	mmFilter->Append(FILTER_ID0 + terr->filter.list.size(),wxString("Save New Filter"),wxEmptyString,wxITEM_NORMAL);
	Bind(wxEVT_MENU,&FormPalView::OnSaveFilterFile,this,FILTER_ID0 + terr->filter.list.size());
}

// save current temp filter
void FormPalView::OnSaveFilterFile(wxCommandEvent& event)
{
	if(!filter)
		return;
	
	// split path to folder and file
	std::filesystem::path last_path = wxStandardPaths::Get().GetExecutablePath().ToStdWstring();
	wstring dir = last_path.parent_path() / L"";
	wstring name = last_path.filename();

	// show save dialog
	wxFileDialog saveFileDialog(this,_("Save Spellcross filter file"),dir,name,"Filter file (*.pal)|*.pal",wxFD_SAVE);
	if(saveFileDialog.ShowModal() == wxID_CANCEL)
		return;
	wstring path = wstring(saveFileDialog.GetPath().ToStdWstring());

	// try save
	filter->SaveFilter(path);	
}

void FormPalView::OnFilterChange(wxCommandEvent& event)
{	
	auto terr = FindTerrain();
	int filter_id = event.GetId() - FILTER_ID0;
	filter = terr->filter.list[filter_id];	
	canvas->Refresh();
}

void FormPalView::OnChangeFilterRGB(wxCommandEvent& event)
{
	auto terr = FindTerrain();
	auto filter = terr->filter.GetTempFilter();		
	if(filter)
		filter->SetFilter(&terr->pal[0][0], "New Filter *",(double)slideRed->GetValue()*0.02,(double)slideGreen->GetValue()*0.02,(double)slideBlue->GetValue()*0.02);
	canvas->Refresh();
}


void FormPalView::OnTerrainChange(wxCommandEvent& event)
{
	ListFilters();
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
		uint8_t* fil = NULL;
		if(filter)
			fil = filter->filter;
		terrain->RenderPalette(bmp, fil, relative_time);
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
		uint8_t* fil = NULL;
		if(filter)
			fil = filter->filter;
		int color = terrain->RenderPaletteColor(bmp, canvas->GetClientSize().GetWidth(),sel_pos_x,fil);
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
