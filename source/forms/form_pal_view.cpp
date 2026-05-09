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
#include <wx/msgdlg.h>

///////////////////////////////////////////////////////////////////////////

FormPalView::FormPalView(wxWindow* parent,SpellData* spell_data,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style) : wxFrame(parent,id,title,pos,size,style)
{
	this->spell_data = spell_data;
	spell_map = NULL;

	// === AUTO GENERATED START ===	
	// <wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormPalView' on 2026-05-07 19:54:31
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
	

	// </wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormPalView' on 2026-05-07 19:54:31
	// === AUTO GENERATED END ===
	
	// set icon
	wxIcon appIcon;
	appIcon.LoadFile("IDI_ICON_PAL",wxBITMAP_TYPE_ICO_RESOURCE);
	if(appIcon.IsOk())
		SetIcon(appIcon);

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

	Bind(wxEVT_COMMAND_TEXT_UPDATED,&FormPalView::OnChangePalListFilter,this,wxID_TXT_FILTER);
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormPalView::OnSelectPalette,this,wxID_LIST_PAL);

	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormPalView::OnExportPalInfo,this,wxID_MM_SAVE_PALINFO);

	

	// defaulf filter
	filter = NULL;

	// default map
	SetMap(NULL);

	m_pal = NULL;
	FillPalettes();
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

// fill list of palettes
void FormPalView::FillPalettes()
{
	auto wild = txtNameFilter->GetValue().ToStdString();
	listPal->Freeze();
	listPal->Clear();
	for(auto &item: spell_data->pal_list)
	{
		if(wildcmp(wild.c_str(), item->m_name.c_str()))
			listPal->Append(item->m_name);
	}
	listPal->Thaw();
	if(!listPal->IsEmpty())
		listPal->Select(0);
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
	while(mmFilter->GetMenuItemCount())
	{
		auto item = mmFilter->FindItemByPosition(mmFilter->GetMenuItemCount() - 1);
		mmFilter->Delete(item);
	}
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

// on change palette list filter
void FormPalView::OnChangePalListFilter(wxCommandEvent& event)
{
	FillPalettes();
}

// on select palette
void FormPalView::OnSelectPalette(wxCommandEvent& event)
{
	canvas->Refresh();
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

// on export palette
void FormPalView::OnExportPalInfo(wxCommandEvent& event)
{
	// get palette name
	std::string pal_name = "";
	int sel = listPal->GetSelection();
	if(sel >= 0)
		pal_name = listPal->GetString(sel);
	auto pal = spell_data->GetPalette(pal_name);
	if(!pal)
	{
		wxMessageDialog msg(NULL,string_format("Cannot export composed palette MAP! Select other palette."),"Exportinf palette info",wxOK| wxICON_EXCLAMATION);
		msg.ShowModal();
		return;
	}

	auto name = std::filesystem::path(pal->m_name).stem().wstring();

	// show save dialog
	wxFileDialog saveFileDialog(this,_("Export palette info"),spell_data->export_path,name,"Spellcross palette info (*.palinfo)|*.palinfo",
		wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if(saveFileDialog.ShowModal() == wxID_CANCEL)
		return;
	auto path = std::filesystem::path(saveFileDialog.GetPath().ToStdWstring());
	spell_data->export_path = saveFileDialog.GetDirectory().ToStdWstring();

	// save palette info
	pal->SaveInfo(path);	
}


// render preview
void FormPalView::OnPaintCanvas(wxPaintEvent& event)
{	
	// make render buffer
	wxBitmap bmp(canvas->GetClientSize(),24);

	// get palette name
	std::string pal_name = "MAP";
	int sel = listPal->GetSelection();
	if(sel >= 0)
		pal_name = listPal->GetString(sel);
	m_pal = spell_data->GetPalette(pal_name);

	if(pal_name == "MAP")
	{
		// basic map palette from terrain
		Terrain* terrain = FindTerrain();
		if(terrain)
		{		
			// render palette
			uint8_t* fil = NULL;
			if(filter)
				fil = filter->filter;
			terrain->RenderPalette(bmp, fil, relative_time);
		}
	}
	else
	{
		// other palette from list
		if(!m_pal)
			return;
		m_pal->Render(bmp);
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

	if(!m_pal || m_pal->m_name == "MAP")
	{
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
	}
	else
	{
		// other palette from list
		int color = m_pal->RenderPaletteColor(bmp,canvas->GetClientSize().GetWidth(),sel_pos_x);
		if(color >= 0)
		{
			uint8_t(*pal)[3] = (uint8_t(*)[3])m_pal->m_pal.data();
			state = string_format("Color = #%d (0x%02X), R = %d, G = %d, B = %d, RGB = 0x%02X%02X%02X",color,color,
				pal[color][0],pal[color][1],pal[color][2],
				pal[color][0],pal[color][1],pal[color][2]);
		}
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
