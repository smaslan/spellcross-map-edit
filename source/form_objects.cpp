///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_objects.h"
#include "sprites.h"
#include "other.h"

#include <filesystem>

///////////////////////////////////////////////////////////////////////////

FormObjects::FormObjects( wxWindow* parent,SpellData* spell_data,wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->spell_data = spell_data;
	spell_map = NULL;

	// === AUTO GENERATED START ===
	
	this->SetSizeHints(wxSize(1000,600),wxDefaultSize);
	//this->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENU));

	sbar = this->CreateStatusBar(1,wxSTB_SIZEGRIP,wxID_SB_MAIN);
	wxBoxSizer* szrMain;
	szrMain = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* szrList;
	szrList = new wxBoxSizer(wxVERTICAL);

	szrList->SetMinSize(wxSize(250,-1));
	m_staticText14 = new wxStaticText(this,wxID_ANY,wxT("Objects list:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText14->Wrap(-1);
	szrList->Add(m_staticText14,0,wxLEFT|wxTOP,5);

	lbObjects = new wxListBox(this,wxID_LB_OBJECTS,wxDefaultPosition,wxSize(100,-1),0,NULL,wxLB_ALWAYS_SB);
	szrList->Add(lbObjects,1,wxLEFT|wxRIGHT|wxEXPAND,5);


	szrMain->Add(szrList,0,wxEXPAND,5);

	m_staticline5 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_VERTICAL);
	szrMain->Add(m_staticline5,0,wxEXPAND | wxALL,5);

	wxBoxSizer* szrView;
	szrView = new wxBoxSizer(wxVERTICAL);

	m_staticText15 = new wxStaticText(this,wxID_ANY,wxT("Object preview:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText15->Wrap(-1);
	szrView->Add(m_staticText15,0,wxLEFT|wxRIGHT|wxTOP,5);

	canvas = new wxPanel(this,wxID_CANVAS,wxDefaultPosition,wxDefaultSize,wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL);
	szrView->Add(canvas,1,wxEXPAND | wxALL,5);

	m_staticline8 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_HORIZONTAL);
	szrView->Add(m_staticline8,0,wxEXPAND|wxTOP|wxRIGHT|wxLEFT,5);

	m_staticText19 = new wxStaticText(this,wxID_ANY,wxT("Object name (keep it short):"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText19->Wrap(-1);
	szrView->Add(m_staticText19,0,wxTOP|wxRIGHT|wxLEFT,5);

	txtName = new wxTextCtrl(this,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,0);
	szrView->Add(txtName,0,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);

	m_staticline9 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_HORIZONTAL);
	szrView->Add(m_staticline9,0,wxEXPAND|wxTOP|wxRIGHT|wxLEFT,5);

	txtGamma = new wxStaticText(this,wxID_TXT_GAMMA,wxT("Gamma correction:"),wxDefaultPosition,wxDefaultSize,0);
	txtGamma->Wrap(-1);
	szrView->Add(txtGamma,0,wxLEFT|wxRIGHT|wxTOP,5);

	slideGamma = new wxSlider(this,wxID_SLIDE_GAMMA,1300,500,2000,wxDefaultPosition,wxDefaultSize,wxSL_HORIZONTAL);
	szrView->Add(slideGamma,0,wxEXPAND|wxALL,1);


	szrMain->Add(szrView,1,wxEXPAND,5);

	m_staticline6 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_VERTICAL);
	szrMain->Add(m_staticline6,0,wxEXPAND | wxALL,5);

	wxBoxSizer* szrProps;
	szrProps = new wxBoxSizer(wxVERTICAL);

	szrProps->SetMinSize(wxSize(250,-1));
	m_staticText17 = new wxStaticText(this,wxID_ANY,wxT("Objects class:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText17->Wrap(-1);
	szrProps->Add(m_staticText17,0,wxLEFT|wxTOP,5);

	wxArrayString chbObjectClassChoices;
	chbObjectClass = new wxChoice(this,wxID_CHB_CLASS,wxDefaultPosition,wxDefaultSize,chbObjectClassChoices,0);
	chbObjectClass->SetSelection(0);
	szrProps->Add(chbObjectClass,0,wxEXPAND|wxLEFT|wxRIGHT,5);

	m_staticText18 = new wxStaticText(this,wxID_ANY,wxT("Objects group:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText18->Wrap(-1);
	szrProps->Add(m_staticText18,0,wxLEFT|wxTOP,5);

	wxArrayString chbObjectsGroupChoices;
	chbObjectsGroup = new wxChoice(this,wxID_CHB_GROUP,wxDefaultPosition,wxDefaultSize,chbObjectsGroupChoices,0);
	chbObjectsGroup->SetSelection(0);
	szrProps->Add(chbObjectsGroup,0,wxEXPAND|wxLEFT|wxRIGHT,5);


	szrMain->Add(szrProps,0,wxEXPAND,5);


	this->SetSizer(szrMain);
	this->Layout();
	m_menubar2 = new wxMenuBar(0);
	mnuFile = new wxMenu();
	wxMenuItem* btnSaveList;
	btnSaveList = new wxMenuItem(mnuFile,wxID_MM_SAVE_OBJECTS,wxString(wxT("Save objects")),wxEmptyString,wxITEM_NORMAL);
	mnuFile->Append(btnSaveList);

	mnuFile->AppendSeparator();

	wxMenuItem* btnClose;
	btnClose = new wxMenuItem(mnuFile,wxID_MM_CLOSE,wxString(wxT("Close")),wxEmptyString,wxITEM_NORMAL);
	mnuFile->Append(btnClose);

	m_menubar2->Append(mnuFile,wxT("File"));

	mnuTerr = new wxMenu();
	m_menubar2->Append(mnuTerr,wxT("Terrain"));

	mnuEdit = new wxMenu();
	wxMenuItem* btnNext;
	btnNext = new wxMenuItem(mnuEdit,wxID_MM_NEXT,wxString(wxT("Next item")) + wxT('\t') + wxT("Ctrl+]"),wxEmptyString,wxITEM_NORMAL);
	mnuEdit->Append(btnNext);

	wxMenuItem* btnPrev;
	btnPrev = new wxMenuItem(mnuEdit,wxID_MM_PREV,wxString(wxT("Previous item")) + wxT('\t') + wxT("Ctrl+["),wxEmptyString,wxITEM_NORMAL);
	mnuEdit->Append(btnPrev);

	m_menubar2->Append(mnuEdit,wxT("Edit"));

	this->SetMenuBar(m_menubar2);


	this->Centre(wxBOTH);

	// === AUTO GENERATED END ===
	
	//slideGamma->SetTickFreq(100);


	// generate terrain menu content
	for(int k = 0;k<spell_data->GetTerrainCount();k++)
	{
		Terrain* terr = spell_data->GetTerrain(k);
		mnuTerr->Append(TERR_ID0 + k,terr->name,wxEmptyString,wxITEM_RADIO);
		Bind(wxEVT_MENU,&FormObjects::OnTerrainChange,this,TERR_ID0 + k);
	}

	// save object list
	Bind(wxEVT_MENU,&FormObjects::OnSaveObjects,this,wxID_MM_SAVE_OBJECTS);

	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormObjects::OnSelectObject,this,wxID_LB_OBJECTS);
	Bind(wxEVT_MENU,&FormObjects::OnSelectObjectBtn,this,wxID_MM_NEXT);
	Bind(wxEVT_MENU,&FormObjects::OnSelectObjectBtn,this,wxID_MM_PREV);
	Bind(wxEVT_COMMAND_SLIDER_UPDATED,&FormObjects::OnChangeGamma,this,wxID_SLIDE_GAMMA);

	// canvas stuff:
	canvas->SetDoubleBuffered(true);
	canvas->Bind(wxEVT_PAINT,&FormObjects::OnPaintCanvas,this);


	// default map
	SetMap(NULL);
}

FormObjects::~FormObjects()
{
}

// on change object
void FormObjects::OnSelectObject(wxCommandEvent& event)
{
	canvas->Refresh();
}

void FormObjects::SetMap(SpellMap* map)
{
	spell_map = map;
	for(int k = 0; k < spell_data->GetTerrainCount(); k++)
	{
		Terrain* terr = spell_data->GetTerrain(k);
		wxString lab = terr->name;
		if(map && std::strcmp(map->terrain->name,spell_data->GetTerrain(k)->name) == 0)
		{
			lab = lab + " (active)";
		}
		mnuTerr->SetLabel(TERR_ID0 + k,lab);
	}

	SelectTerrain();
}

// find terrain selected
Terrain* FormObjects::FindTerrain()
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



// call when terrain is selected
void FormObjects::SelectTerrain()
{	
	Terrain *terr = FindTerrain();		

	/*#include <wx/stdpaths.h>
	wxFileName f(wxStandardPaths::Get().GetExecutablePath());
	wxString appPath(f.GetPath());*/

	lbObjects->Clear();
	int count = terr->GetObjectsCount();
	for(int k = 0; k < count; k++)
	{
		SpellObject* obj = terr->GetObject(k);
		lbObjects->Append(obj->GetDescription());
	}
	if(lbObjects->GetCount())
		lbObjects->Select(0);
	
	//wxEVT_COMMAND_LISTBOX_SELECTED
	wxCommandEvent event(wxID_LB_OBJECTS,wxEVT_COMMAND_LISTBOX_SELECTED);
	OnSelectObject(event);
}

// change gamma
void FormObjects::OnChangeGamma(wxCommandEvent& event)
{
	double gamma = 0.001*(double)slideGamma->GetValue();
	txtGamma->SetLabel(wxString::Format(wxT("Gamma correction (%#0.2f):"),gamma));
	canvas->Refresh();
}

void FormObjects::OnTerrainChange(wxCommandEvent& event)
{
	SelectTerrain();
}

// save current terrain's objects list
void FormObjects::OnSaveObjects(wxCommandEvent& event)
{
	// get this terrain
	Terrain* terrain = FindTerrain();

	// split path to folder and file
	std::filesystem::path last_path = terrain->GetSpriteContextPath();
	wstring dir = last_path.parent_path(); dir += wstring(L"\\");
	wstring name = last_path.filename();

	// show save dialog
	wxFileDialog saveFileDialog(this,_("Save Spellcross terrain context file"),dir,name,"Context file (*.con)|*.con",wxFD_SAVE);
	if(saveFileDialog.ShowModal() == wxID_CANCEL)
		return;
	wstring path = wstring(saveFileDialog.GetPath().ToStdWstring());

	// save file
	terrain->SaveSpriteContext(path);
}

// select object by shortcuts
void FormObjects::OnSelectObjectBtn(wxCommandEvent& event)
{
	if(event.GetId() == wxID_MM_NEXT)
	{
		if(lbObjects->GetSelection() < lbObjects->GetCount()-1)
			lbObjects->Select(lbObjects->GetSelection()+1);
	}
	else if(event.GetId() == wxID_MM_PREV)
	{
		if(lbObjects->GetSelection() > 0)
			lbObjects->Select(lbObjects->GetSelection()-1);
	}
	OnSelectObject(event);
}

// render preview
void FormObjects::OnPaintCanvas(wxPaintEvent& event)
{	
	// make render buffer
	wxBitmap bmp(canvas->GetClientSize(),24);

	if(lbObjects->GetCount() && lbObjects->GetSelection() >= 0)
	{					
		// get this terrain
		Terrain* terrain = FindTerrain();
		if(terrain)
		{
			// get selected object
			auto obj = terrain->GetObject(lbObjects->GetSelection());
			if(obj)
			{
				// render preview
				double gamma = 0.001*(double)slideGamma->GetValue();
				obj->RenderPreview(bmp, gamma);
			}
		}
	}
	
	// blit to screen
	wxPaintDC pdc(canvas);
	pdc.DrawBitmap(bmp,wxPoint(0,0));
}