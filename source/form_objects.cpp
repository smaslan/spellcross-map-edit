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
	
	this->SetSizeHints(wxDefaultSize,wxDefaultSize);
	this->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENU));

	m_statusBar2 = this->CreateStatusBar(1,wxSTB_SIZEGRIP,wxID_ANY);
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer(wxVERTICAL);

	htmlList = new wxSimpleHtmlListBox(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,0);
	bSizer6->Add(htmlList,1,wxALL|wxEXPAND,5);


	this->SetSizer(bSizer6);
	this->Layout();
	m_menubar2 = new wxMenuBar(0);
	m_menu4 = new wxMenu();
	wxMenuItem* btnSaveList;
	btnSaveList = new wxMenuItem(m_menu4,wxID_MM_SAVE_OBJECTS,wxString(wxT("Save objects")),wxEmptyString,wxITEM_NORMAL);
	m_menu4->Append(btnSaveList);

	m_menu4->AppendSeparator();

	wxMenuItem* btnClose;
	btnClose = new wxMenuItem(m_menu4,wxID_MM_CLOSE,wxString(wxT("Close")),wxEmptyString,wxITEM_NORMAL);
	m_menu4->Append(btnClose);

	m_menubar2->Append(m_menu4,wxT("File"));

	mnuTerr = new wxMenu();
	m_menubar2->Append(mnuTerr,wxT("Terrain"));

	this->SetMenuBar(m_menubar2);


	this->Centre(wxBOTH);

	// === AUTO GENERATED END ===


	// generate terrain menu content
	for(int k = 0;k<spell_data->GetTerrainCount();k++)
	{
		Terrain* terr = spell_data->GetTerrain(k);
		mnuTerr->Append(TERR_ID0 + k,terr->name,wxEmptyString,wxITEM_RADIO);
		Bind(wxEVT_MENU,&FormObjects::OnTerrainChange,this,TERR_ID0 + k);
	}

	// save object list
	Bind(wxEVT_MENU,&FormObjects::OnSaveObjects,this,wxID_MM_SAVE_OBJECTS);

	htmlList->SetSelectionBackground(wxColor(128,128,255));

	// default map
	SetMap(NULL);
}

FormObjects::~FormObjects()
{
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

	htmlList->Clear();
	htmlList->Hide();
	int count = terr->GetObjectsCount();
	for(int k = 0; k < count; k++)
	{
		SpellObject* obj = terr->GetObject(k);
		std::wstring html = L"<p align=center><img src=\"" + /*appPath.ToStdWstring() + L"\\" +*/ obj->GetGlyphPath() + L"\"><br>" + char2wstring(obj->GetDescription().c_str()) + L"</p>";
		htmlList->Append(html);
	}
	htmlList->Show();
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
	std::filesystem::path last_path = terrain->GetSpriteObjectsPath();
	wstring dir = last_path.parent_path(); dir += wstring(L"\\");
	wstring name = last_path.filename();

	// show save dialog
	wxFileDialog saveFileDialog(this,_("Save Spellcross Objects List File"),dir,name,"Objects list file (*.sol)|*.sol",
		wxFD_SAVE);
	if(saveFileDialog.ShowModal() == wxID_CANCEL)
		return;
	wstring path = wstring(saveFileDialog.GetPath().ToStdWstring());

	// save file
	terrain->SaveObjects(path);
}