///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_tools.h"

///////////////////////////////////////////////////////////////////////////

FormTools::FormTools( wxWindow* parent, SpellData* spelldata, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->spell_data = spelldata;
	this->spell_map = NULL;

	// ==== BEGIN AUTO GENERATED ====
	
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENU));

	sbar = this->CreateStatusBar(1, wxSTB_SIZEGRIP, wxID_ANY);
	wxBoxSizer* szrMain;
	szrMain = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* szrToolset;
	szrToolset = new wxBoxSizer(wxVERTICAL);

	m_staticText19 = new wxStaticText(this, wxID_ANY, wxT("Toolsets:"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText19->Wrap(-1);
	szrToolset->Add(m_staticText19, 0, wxLEFT | wxRIGHT | wxTOP, 5);

	lbToolset = new wxListBox(this, wxID_LB_TOOLSET, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB);
	szrToolset->Add(lbToolset, 1, wxEXPAND | wxRIGHT | wxLEFT, 5);

	m_staticText21 = new wxStaticText(this, wxID_ANY, wxT("Toolset name:"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText21->Wrap(-1);
	szrToolset->Add(m_staticText21, 0, wxTOP | wxRIGHT | wxLEFT, 5);

	txtNewToolset = new wxTextCtrl(this, wxID_TXT_NEW_TOOLSET, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	szrToolset->Add(txtNewToolset, 0, wxBOTTOM | wxRIGHT | wxLEFT | wxEXPAND, 5);

	m_staticText23 = new wxStaticText(this, wxID_ANY, wxT("Toolset title:"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText23->Wrap(-1);
	szrToolset->Add(m_staticText23, 0, wxRIGHT | wxLEFT, 5);

	txtToolsetTitle = new wxTextCtrl(this, wxID_TXT_TOOLSET_TITLE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	szrToolset->Add(txtToolsetTitle, 0, wxEXPAND | wxRIGHT | wxLEFT, 5);

	wxBoxSizer* szrScaling;
	szrScaling = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* szrScale;
	szrScale = new wxBoxSizer(wxVERTICAL);

	m_staticText26 = new wxStaticText(this, wxID_ANY, wxT("Glyph scaling:"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText26->Wrap(-1);
	szrScale->Add(m_staticText26, 0, wxTOP | wxRIGHT | wxLEFT, 5);

	wxArrayString chbScalingChoices;
	chbScaling = new wxChoice(this, wxID_CHB_SCALE, wxDefaultPosition, wxDefaultSize, chbScalingChoices, 0);
	chbScaling->SetSelection(0);
	szrScale->Add(chbScaling, 0, wxBOTTOM | wxRIGHT | wxLEFT | wxEXPAND, 5);


	szrScaling->Add(szrScale, 1, wxEXPAND, 5);

	wxBoxSizer* szrWidth;
	szrWidth = new wxBoxSizer(wxVERTICAL);

	m_staticText27 = new wxStaticText(this, wxID_ANY, wxT("Width:"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText27->Wrap(-1);
	szrWidth->Add(m_staticText27, 0, wxTOP | wxRIGHT | wxLEFT, 5);

	scWidth = new wxSpinCtrl(this, wxID_SC_WIDTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 32, 200, 80);
	scWidth->SetMinSize(wxSize(80, -1));
	scWidth->SetMaxSize(wxSize(80, -1));

	szrWidth->Add(scWidth, 0, wxBOTTOM | wxRIGHT | wxLEFT | wxEXPAND, 5);


	szrScaling->Add(szrWidth, 0, wxEXPAND, 5);

	wxBoxSizer* szrHeight;
	szrHeight = new wxBoxSizer(wxVERTICAL);

	m_staticText28 = new wxStaticText(this, wxID_ANY, wxT("Height:"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText28->Wrap(-1);
	szrHeight->Add(m_staticText28, 0, wxTOP | wxRIGHT | wxLEFT, 5);

	scHeight = new wxSpinCtrl(this, wxID_SC_HEIGHT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 32, 200, 80);
	scHeight->SetMinSize(wxSize(80, -1));
	scHeight->SetMaxSize(wxSize(80, -1));

	szrHeight->Add(scHeight, 0, wxBOTTOM | wxRIGHT | wxLEFT | wxEXPAND, 5);


	szrScaling->Add(szrHeight, 0, wxEXPAND, 5);


	szrToolset->Add(szrScaling, 0, wxEXPAND, 5);

	wxGridSizer* szrToolsetBtn;
	szrToolsetBtn = new wxGridSizer(0, 5, 0, 0);

	btnAddToolset = new wxButton(this, wxID_BTN_TOOLSET_NEW, wxT("New"), wxDefaultPosition, wxDefaultSize, 0);
	szrToolsetBtn->Add(btnAddToolset, 0, wxALL | wxEXPAND, 5);

	btnRenameToolset = new wxButton(this, wxID_BTN_TOOLSET_RENAME, wxT("Update"), wxDefaultPosition, wxDefaultSize, 0);
	szrToolsetBtn->Add(btnRenameToolset, 0, wxALL | wxEXPAND, 5);

	btnToolsetDel = new wxButton(this, wxID_BTN_TOOLSET_REM, wxT("Remove"), wxDefaultPosition, wxDefaultSize, 0);
	szrToolsetBtn->Add(btnToolsetDel, 0, wxALL | wxEXPAND, 5);

	btnToolsetUp = new wxButton(this, wxID_BTN_TOOLSET_UP, wxT("Up"), wxDefaultPosition, wxDefaultSize, 0);
	szrToolsetBtn->Add(btnToolsetUp, 0, wxALL | wxEXPAND, 5);

	btnToolsetDown = new wxButton(this, wxID_BTN_TOOLSET_DOWN, wxT("Down"), wxDefaultPosition, wxDefaultSize, 0);
	szrToolsetBtn->Add(btnToolsetDown, 0, wxALL | wxEXPAND, 5);


	szrToolset->Add(szrToolsetBtn, 0, wxEXPAND, 5);


	szrMain->Add(szrToolset, 1, wxEXPAND, 5);

	wxBoxSizer* szrTool;
	szrTool = new wxBoxSizer(wxVERTICAL);

	m_staticText20 = new wxStaticText(this, wxID_ANY, wxT("Tools:"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText20->Wrap(-1);
	szrTool->Add(m_staticText20, 0, wxTOP | wxRIGHT | wxLEFT, 5);

	lbTool = new wxListBox(this, wxID_LB_TOOL, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB);
	szrTool->Add(lbTool, 1, wxEXPAND | wxBOTTOM | wxRIGHT | wxLEFT, 5);

	m_staticText22 = new wxStaticText(this, wxID_ANY, wxT("Tool name:"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText22->Wrap(-1);
	szrTool->Add(m_staticText22, 0, wxTOP | wxRIGHT | wxLEFT, 5);

	txtNewTool = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	szrTool->Add(txtNewTool, 0, wxBOTTOM | wxRIGHT | wxLEFT | wxEXPAND, 5);

	wxGridSizer* srzToolBtn;
	srzToolBtn = new wxGridSizer(0, 5, 0, 0);

	btnNewTool = new wxButton(this, wxID_BTN_TOOL_NEW, wxT("New"), wxDefaultPosition, wxDefaultSize, 0);
	srzToolBtn->Add(btnNewTool, 0, wxALL | wxEXPAND, 5);

	btnRenameTool = new wxButton(this, wxID_BTN_TOOL_RENAME, wxT("Rename"), wxDefaultPosition, wxDefaultSize, 0);
	srzToolBtn->Add(btnRenameTool, 0, wxALL | wxEXPAND, 5);

	btnToolDel = new wxButton(this, wxID_BTN_TOOL_REM, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0);
	srzToolBtn->Add(btnToolDel, 0, wxALL | wxEXPAND, 5);

	btnToolUp = new wxButton(this, wxID_BTN_TOOL_UP, wxT("Up"), wxDefaultPosition, wxDefaultSize, 0);
	srzToolBtn->Add(btnToolUp, 0, wxALL | wxEXPAND, 5);

	btnToolDown = new wxButton(this, wxID_BTN_TOOL_DOWN, wxT("Down"), wxDefaultPosition, wxDefaultSize, 0);
	srzToolBtn->Add(btnToolDown, 0, wxALL | wxEXPAND, 5);


	szrTool->Add(srzToolBtn, 0, wxEXPAND, 5);


	szrMain->Add(szrTool, 1, wxEXPAND, 5);


	this->SetSizer(szrMain);
	this->Layout();
	mmBar = new wxMenuBar(0);
	mmFile = new wxMenu();
	wxMenuItem* mmSave;
	mmSave = new wxMenuItem(mmFile, wxID_MM_SAVE, wxString(wxT("Save list")) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL);
	mmFile->Append(mmSave);

	mmFile->AppendSeparator();

	wxMenuItem* mmClose;
	mmClose = new wxMenuItem(mmFile, wxID_MM_CLOSE, wxString(wxT("Close")), wxEmptyString, wxITEM_NORMAL);
	mmFile->Append(mmClose);

	mmBar->Append(mmFile, wxT("File"));

	mmTerrain = new wxMenu();
	mmBar->Append(mmTerrain, wxT("Terrain"));

	mmEdit = new wxMenu();
	wxMenuItem* mmClrAllToolset;
	mmClrAllToolset = new wxMenuItem(mmEdit, wxID_MM_CLR_TOOLSETS, wxString(wxT("Remove all toolsets")), wxEmptyString, wxITEM_NORMAL);
	mmEdit->Append(mmClrAllToolset);

	wxMenuItem* mmClrAllTools;
	mmClrAllTools = new wxMenuItem(mmEdit, wxID_CLR_TOOLS, wxString(wxT("Remove all tools")), wxEmptyString, wxITEM_NORMAL);
	mmEdit->Append(mmClrAllTools);

	mmBar->Append(mmEdit, wxT("Edit"));

	this->SetMenuBar(mmBar);


	this->Centre(wxBOTH);

	// ==== END AUTO GENERATED ====

	// fill terrain selector
	for (int k = 0; k < spell_data->GetTerrainCount(); k++)
	{
		Terrain* terr = spell_data->GetTerrain(k);
		mmTerrain->Append(TERR_ID0 + k, terr->name, wxEmptyString, wxITEM_RADIO);
		Bind(wxEVT_MENU, &FormTools::OnTerrainChange, this, TERR_ID0 + k);
	}
	SetMap(NULL);

	// fill glyph scaling selector
	chbScaling->Append("Mean aspect, fixed w/h");
	chbScaling->Append("Max size");
	chbScaling->Append("Max size, no zoom");


	Bind(wxEVT_CLOSE_WINDOW, &FormTools::OnClose, this, this->m_windowId);
	Bind(wxEVT_MENU, &FormTools::OnCloseClick, this, wxID_MM_CLOSE);
	Bind(wxEVT_MENU, &FormTools::OnSaveContext, this, wxID_MM_SAVE);

	Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &FormTools::OnToolsetSelect, this, wxID_LB_TOOLSET);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &FormTools::OnToolsetRename, this, wxID_BTN_TOOLSET_RENAME);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &FormTools::OnToolsetNew, this, wxID_BTN_TOOLSET_NEW);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &FormTools::OnToolsetRemove, this, wxID_BTN_TOOLSET_REM);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &FormTools::OnToolsetMove, this, wxID_BTN_TOOLSET_UP);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &FormTools::OnToolsetMove, this, wxID_BTN_TOOLSET_DOWN);

	Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &FormTools::OnToolSelect, this, wxID_LB_TOOL);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &FormTools::OnToolRename, this, wxID_BTN_TOOL_RENAME);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &FormTools::OnToolNew, this, wxID_BTN_TOOL_NEW);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &FormTools::OnToolRemove, this, wxID_BTN_TOOL_REM);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &FormTools::OnToolMove, this, wxID_BTN_TOOL_UP);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &FormTools::OnToolMove, this, wxID_BTN_TOOL_DOWN);
}

FormTools::~FormTools()
{
}

void FormTools::OnClose(wxCloseEvent& ev)
{
	wxPostEvent(GetParent(), ev);
	ev.Skip();
	Destroy();
}

// close form
void FormTools::OnCloseClick(wxCommandEvent& event)
{
	Close();
}

// save current terrain's constext
void FormTools::OnSaveContext(wxCommandEvent& event)
{
	// get this terrain
	Terrain* terrain = FindTerrain();

	// split path to folder and file
	std::filesystem::path last_path = terrain->GetSpriteContextPath();
	wstring dir = last_path.parent_path(); dir += wstring(L"\\");
	wstring name = last_path.filename();

	// show save dialog
	wxFileDialog saveFileDialog(this, _("Save Spellcross terrain context file"), dir, name, "Context file (*.con)|*.con", wxFD_SAVE);
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;
	wstring path = wstring(saveFileDialog.GetPath().ToStdWstring());

	// save file
	terrain->SaveSpriteContext(path);
}



// set current map poitner
void FormTools::SetMap(SpellMap* map)
{
	spell_map = map;
	for (int k = 0; k < spell_data->GetTerrainCount(); k++)
	{
		Terrain* terr = spell_data->GetTerrain(k);
		wxString lab = terr->name;
		if (map && map->terrain && map->terrain->name.compare(spell_data->GetTerrain(k)->name) == 0)
		{
			lab = lab + " (active)";
			mmTerrain->Check(TERR_ID0 + k,true);
		}
		mmTerrain->SetLabel(TERR_ID0 + k, lab);
	}
	
	SelectTerrain();
}

// find terrain selected
Terrain* FormTools::FindTerrain()
{
	for (int k = 0; k < spell_data->GetTerrainCount(); k++)
	{
		if (GetMenuBar()->FindItem(TERR_ID0 + k)->IsChecked())
		{
			// found selection
			Terrain* terr = spell_data->GetTerrain(k);
			return(terr);
		}
	}
	return(NULL);
}
// on terrain selection click
void FormTools::OnTerrainChange(wxCommandEvent& event)
{
	SelectTerrain();
}

// terrain change
void FormTools::SelectTerrain()
{
	auto terr = FindTerrain();
	if (!terr)
		return;

	// for each toolset:
	lbToolset->Freeze();
	lbToolset->Clear();
	for (int tid = 0; tid < terr->GetToolsCount(); tid++)
	{
		//auto toolset = terr->GetToolSet(tid);
		
		lbToolset->Append(terr->GetToolSetName(tid) + ": " + terr->GetToolSetTitle(tid));
	}
	lbToolset->Thaw();
	if(lbToolset->GetCount())
		lbToolset->Select(0);

	SelectToolset();
}

// on toolset selection click
void FormTools::OnToolsetSelect(wxCommandEvent& event)
{
	SelectToolset();
}

// selection of toolset
void FormTools::SelectToolset()
{
	// get terrain selection
	auto terr = FindTerrain();
	if (!terr)
		return;

	// get toolset
	txtNewToolset->Clear();
	txtToolsetTitle->Clear();
	lbTool->Clear();
	if (!lbToolset->GetCount() || lbToolset->GetSelection() < 0)
		return;
	//auto toolset = terr->GetToolSet(lbToolset->GetSelection());
	auto selid = lbToolset->GetSelection();
	
	// show name/title
	txtNewToolset->SetValue(terr->GetToolSetName(selid));
	txtToolsetTitle->SetValue(terr->GetToolSetTitle(selid));
	chbScaling->SetSelection(terr->GetToolSetGlyphScalingMode(selid));
	auto [w,h] = terr->GetToolSetGlyphScaling(selid);
	scWidth->SetValue(w);
	scHeight->SetValue(h);

	// for each toolset:
	lbTool->Freeze();
	for (auto const& str : terr->GetToolSetItems(selid))
		lbTool->Append(str);
	lbTool->Thaw();
	if (lbTool->GetCount())
		lbTool->Select(0);

	SelectTool();
}

// on toolset selection click
void FormTools::OnToolSelect(wxCommandEvent& event)
{
	SelectTool();
}

// tool selection
void FormTools::SelectTool()
{
	// get terrain selection
	auto terr = FindTerrain();
	if (!terr)
		return;

	txtNewTool->Clear();

	// get toolset
	if (!lbToolset->GetCount() || lbToolset->GetSelection() < 0)
		return;
	//auto toolset = terr->GetToolSet(lbToolset->GetSelection());
	auto selid = lbToolset->GetSelection();

	// get tool
	if (!lbTool->GetCount() || lbTool->GetSelection() < 0)
		return;
	txtNewTool->SetValue(terr->GetToolSetItem(selid, lbTool->GetSelection()));
}

// on toolset rename click
void FormTools::OnToolsetRename(wxCommandEvent& event)
{
	// get terrain selection
	auto terr = FindTerrain();
	if (!terr)
		return;

	// get toolset
	if (!lbToolset->GetCount() || lbToolset->GetSelection() < 0)
		return;
	//auto toolset = terr->GetToolSet(lbToolset->GetSelection());
	auto setid = lbToolset->GetSelection();

	// update names
	terr->SetToolSetName(setid, txtNewToolset->GetValue().ToStdString());
	terr->SetToolSetTitle(setid, txtToolsetTitle->GetValue().ToStdString());
	terr->SetToolSetGlyphScalingMode(setid, chbScaling->GetSelection());
	terr->SetToolSetGlyphScaling(setid, scWidth->GetValue(), scHeight->GetValue());

	auto selid = lbToolset->GetSelection();
	SelectTerrain();
	lbToolset->SetSelection(selid);
	SelectToolset();
}

// on toolset add new click
void FormTools::OnToolsetNew(wxCommandEvent& event)
{
	// get terrain selection
	auto terr = FindTerrain();
	if (!terr)
		return;

	// get toolset
	int selid = -1;
	if (lbToolset->GetCount() && lbToolset->GetSelection() >= 0)
		selid = lbToolset->GetSelection();
	
	// add new toolset
	terr->AddToolSet(txtNewToolset->GetValue().ToStdString(), txtToolsetTitle->GetValue().ToStdString(), selid);

	SelectTerrain();
	if(selid >= 0)
		lbToolset->SetSelection(selid);
	SelectToolset();
}

// on toolset add new click
void FormTools::OnToolsetRemove(wxCommandEvent& event)
{
	// get terrain selection
	auto terr = FindTerrain();
	if (!terr)
		return;

	// get toolset
	if (!lbToolset->GetCount() || lbToolset->GetSelection() < 0)
		return;
	auto selid = lbToolset->GetSelection();

	// add new toolset
	terr->RemoveToolSet(selid);

	SelectTerrain();
	if (lbToolset->GetCount())
		lbToolset->SetSelection(min(selid, (int)lbToolset->GetCount()-1));
	SelectToolset();
}

// on toolset add new click
void FormTools::OnToolsetMove(wxCommandEvent& event)
{
	// get terrain selection
	auto terr = FindTerrain();
	if (!terr)
		return;

	// get toolset
	if (!lbToolset->GetCount() || lbToolset->GetSelection() < 0)
		return;
	auto selid = lbToolset->GetSelection();

	if (event.GetId() == wxID_BTN_TOOLSET_UP && selid > 0)
	{
		terr->MoveToolSet(selid, selid - 1);
		selid--;
	}
	else if (event.GetId() == wxID_BTN_TOOLSET_DOWN && selid < lbToolset->GetCount() - 1)
	{
		terr->MoveToolSet(selid, selid + 1);
		selid++;
	}
	SelectTerrain();
	lbToolset->SetSelection(selid);
	SelectToolset();
}

// on new tool click
void FormTools::OnToolNew(wxCommandEvent& event)
{
	// get terrain selection
	auto terr = FindTerrain();
	if (!terr)
		return;

	// get toolset
	if (!lbToolset->GetCount() || lbToolset->GetSelection() < 0)
		return;
	auto setid = lbToolset->GetSelection();

	int selid = -1;
	if (lbTool->GetCount() && lbTool->GetSelection() >= 0)
		selid = lbTool->GetSelection();

	// update name
	terr->AddToolSetItem(setid, txtNewTool->GetValue().ToStdString(), selid);

	SelectToolset();
	lbTool->SetSelection(selid);
	SelectTool();
}

// on tool rename click
void FormTools::OnToolRename(wxCommandEvent& event)
{
	// get terrain selection
	auto terr = FindTerrain();
	if (!terr)
		return;

	// get toolset
	if (!lbToolset->GetCount() || lbToolset->GetSelection() < 0)
		return;
	//auto toolset = terr->GetToolSet(lbToolset->GetSelection());
	auto setid = lbToolset->GetSelection();

	if (!lbTool->GetCount() || lbTool->GetSelection() < 0)
		return;

	// update name
	terr->RenameToolSetItem(setid, txtNewTool->GetValue().ToStdString(), lbTool->GetSelection());

	auto selid = lbTool->GetSelection();
	SelectToolset();
	lbTool->SetSelection(selid);
	SelectTool();
}

// on tool remove click
void FormTools::OnToolRemove(wxCommandEvent& event)
{
	// get terrain selection
	auto terr = FindTerrain();
	if (!terr)
		return;

	// get toolset
	if (!lbToolset->GetCount() || lbToolset->GetSelection() < 0)
		return;
	auto setid = lbToolset->GetSelection();

	if (!lbTool->GetCount() || lbTool->GetSelection() < 0)
		return;

	// update name
	auto selid = lbTool->GetSelection();
	terr->RemoveToolSetItem(setid, selid);

	SelectToolset();
	if(terr->GetToolSetItemsCount(setid))
		lbTool->SetSelection(min(selid, terr->GetToolSetItemsCount(setid) - 1));
	SelectTool();
}

// on tool remove click
void FormTools::OnToolMove(wxCommandEvent& event)
{
	// get terrain selection
	auto terr = FindTerrain();
	if (!terr)
		return;

	// get toolset
	if (!lbToolset->GetCount() || lbToolset->GetSelection() < 0)
		return;
	//auto toolset = terr->GetToolSet(lbToolset->GetSelection());
	auto setid = lbToolset->GetSelection();

	if (!lbTool->GetCount() || lbTool->GetSelection() < 0)
		return;
	auto selid = lbTool->GetSelection();

	if (event.GetId() == wxID_BTN_TOOL_UP && selid > 0)
	{
		terr->MoveToolSetItem(setid, selid, selid - 1);
		selid--;
	}
	else if (event.GetId() == wxID_BTN_TOOL_DOWN && selid < lbTool->GetCount() - 1)
	{
		terr->MoveToolSetItem(setid, selid, selid + 1);
		selid++;
	}

	SelectToolset();
	lbTool->SetSelection(selid);
	SelectTool();
}