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
#include <wx/msgdlg.h>

///////////////////////////////////////////////////////////////////////////

FormObjects::FormObjects( wxWindow* parent,SpellData* spell_data,wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->spell_data = spell_data;
	spell_map = NULL;
	m_spell_obj = NULL;

	// === AUTO GENERATED START ===
	
	this->SetSizeHints(wxSize(800,400),wxDefaultSize);
	this->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
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

	treeCtrlClasses = new wxTreeCtrl(this,wxID_TRC_CLASSES,wxDefaultPosition,wxDefaultSize,wxTR_DEFAULT_STYLE|wxTR_EDIT_LABELS|wxALWAYS_SHOW_SB|wxVSCROLL);
	szrList->Add(treeCtrlClasses,1,wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT,5);


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
	wxMenuItem* mmRemove;
	mmRemove = new wxMenuItem(mnuEdit,wxID_MM_REMOVE,wxString(wxT("Delete object")) + wxT('\t') + wxT("Delete"),wxEmptyString,wxITEM_NORMAL);
	mnuEdit->Append(mmRemove);

	wxMenuItem* mmRename;
	mmRename = new wxMenuItem(mnuEdit,wxID_MM_RENAME,wxString(wxT("Rename object")) + wxT('\t') + wxT("Ctrl+R"),wxEmptyString,wxITEM_NORMAL);
	mnuEdit->Append(mmRename);

	m_menubar2->Append(mnuEdit,wxT("Edit"));

	this->SetMenuBar(m_menubar2);


	this->Centre(wxBOTH);

	// === AUTO GENERATED END ===

	
	//slideGamma->SetTickFreq(100);
	// set icon
	wxIcon appIcon;
	appIcon.LoadFile("IDI_ICON2",wxBITMAP_TYPE_ICO_RESOURCE);
	if(appIcon.IsOk())
		SetIcon(appIcon);


	// generate terrain menu content
	for(int k = 0;k<spell_data->GetTerrainCount();k++)
	{
		Terrain* terr = spell_data->GetTerrain(k);
		mnuTerr->Append(TERR_ID0 + k,terr->name,wxEmptyString,wxITEM_RADIO);
		Bind(wxEVT_MENU,&FormObjects::OnTerrainChange,this,TERR_ID0 + k);
	}

	Bind(wxEVT_CLOSE_WINDOW, &FormObjects::OnClose, this, this->m_windowId);

	// save object list
	Bind(wxEVT_MENU,&FormObjects::OnSaveObjects,this,wxID_MM_SAVE_OBJECTS);
	Bind(wxEVT_MENU, &FormObjects::OnCloseClick, this, wxID_MM_CLOSE);

	Bind(wxEVT_COMMAND_SLIDER_UPDATED,&FormObjects::OnChangeGamma,this,wxID_SLIDE_GAMMA);

	Bind(wxEVT_MENU,&FormObjects::OnRename,this,wxID_MM_RENAME);
	Bind(wxEVT_MENU,&FormObjects::OnRemove,this,wxID_MM_REMOVE);

	// canvas stuff:
	canvas->SetDoubleBuffered(true);
	canvas->Bind(wxEVT_PAINT,&FormObjects::OnPaintCanvas,this);

	// object tool classes stuff:	
	FillToolsClasses();
	// 
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormObjects::OnToolClassItemChange,this,wxID_CHB_GROUP);

	Bind(wxEVT_TREE_SEL_CHANGED,&FormObjects::OnTreeSelectionChanged,this,wxID_TRC_CLASSES);
	Bind(wxEVT_TREE_BEGIN_DRAG,&FormObjects::OnTreeClassBeginDrag,this,wxID_TRC_CLASSES);
	Bind(wxEVT_TREE_END_DRAG,&FormObjects::OnTreeClassEndDrag,this,wxID_TRC_CLASSES);
	Bind(wxEVT_TREE_BEGIN_LABEL_EDIT,&FormObjects::OnTreeClassBeginLabelEdit,this,wxID_TRC_CLASSES);
	Bind(wxEVT_TREE_END_LABEL_EDIT,&FormObjects::OnTreeClassEndLabelEdit,this,wxID_TRC_CLASSES);

	// default map
	SetMap(NULL);
}

// rename object by menu
void FormObjects::OnRename(wxCommandEvent &evt)
{
	auto item_id = treeCtrlClasses->GetSelection();
	if(!item_id)
		return;
	/*auto* obj = (TreeNode*)treeCtrlClasses->GetItemData(item_id);
	if(!obj || !obj->m_obj)
		return;*/
	treeCtrlClasses->EditLabel(item_id);
}

// remove object by menu
void FormObjects::OnRemove(wxCommandEvent& evt)
{
	auto item_id = treeCtrlClasses->GetSelection();
	if(!item_id)
		return;
	auto* obj = (TreeNode*)treeCtrlClasses->GetItemData(item_id);
	if(!obj || !obj->m_obj)
		return;
	auto terr = FindTerrain();
	terr->RemoveObject(terr->FindObject(obj->m_obj));
	treeCtrlClasses->Delete(item_id);	
}

void FormObjects::OnTreeClassBeginLabelEdit(wxTreeEvent& evt)
{
	auto* obj = (TreeNode*)treeCtrlClasses->GetItemData(evt.GetItem());
	if(!obj)
	{
		// no edit root
		evt.Veto();
		return;
	}
	if(!obj->m_obj)
	{
		// no edit class name
		evt.Veto();
		return;
	}
}
void FormObjects::OnTreeClassEndLabelEdit(wxTreeEvent& evt)
{
	wxString text = evt.GetLabel();
	auto* obj = (TreeNode*)treeCtrlClasses->GetItemData(evt.GetItem());
	if(!obj)
	{
		// no edit root
		evt.Veto();
		return;
	}
	if(!obj->m_obj)
	{
		// no edit class name
		evt.Veto();
		return;
	}
	obj->m_obj->SetDescription(text.ToStdString());
}


void FormObjects::OnTreeClassBeginDrag(wxTreeEvent& evt)
{
	m_drag_item = evt.GetItem();	
	auto* obj = (TreeNode*)treeCtrlClasses->GetItemData(m_drag_item);
	if(!obj)
		m_drag_item.Unset();
	evt.Allow();
}

void FormObjects::OnTreeClassEndDrag(wxTreeEvent& evt)
{
	if(!m_drag_item)
		return;
	auto obj = (TreeNode*)treeCtrlClasses->GetItemData(m_drag_item);
	auto target_item = evt.GetItem();
	if(treeCtrlClasses->GetRootItem() == target_item)
		return; // cannot move to root
	auto parent_node = treeCtrlClasses->GetItemParent(target_item);
	wxTreeItemId nid;
	if(treeCtrlClasses->GetRootItem() == parent_node)
	{
		// target is class node		
		nid = treeCtrlClasses->AppendItem(target_item,treeCtrlClasses->GetItemText(m_drag_item),-1,-1,(wxTreeItemData*)new TreeNode(obj->m_obj));
		treeCtrlClasses->SelectItem(nid);
	}
	else
	{
		// target is class item		
		nid = treeCtrlClasses->InsertItem(parent_node,treeCtrlClasses->GetPrevSibling(target_item),treeCtrlClasses->GetItemText(m_drag_item),-1,-1,(wxTreeItemData*)new TreeNode(obj->m_obj));
		treeCtrlClasses->SelectItem(nid);
	}
	auto cls = (TreeNode*)treeCtrlClasses->GetItemData(treeCtrlClasses->GetItemParent(nid));	
	obj->m_obj->SetToolClass(cls->m_class_id);
	treeCtrlClasses->Delete(m_drag_item);

	SortItems();
}

void FormObjects::OnTreeSelectionChanged(wxTreeEvent& evt)
{
	wxTreeItemId selectedNode = evt.GetItem();
	auto *obj = (TreeNode*)treeCtrlClasses->GetItemData(selectedNode);
	if(!obj)
	{
		// root
		evt.Skip();
		return; 
	}
	
	FillToolItemsList();

	// is node
	m_spell_obj = obj->m_obj;
	canvas->Refresh();	
}

// sort objects per classe (call before leaving)
void FormObjects::SortItems()
{
	auto terr = FindTerrain();	

	// for each class:
	int new_obj_id = 0;
	auto root_id = treeCtrlClasses->GetRootItem();
	wxTreeItemIdValue cookie;
	auto class_id = treeCtrlClasses->GetFirstChild(root_id,cookie);
	for(int cid = 0; cid <= terr->GetToolsCount(); cid++)
	{
		if(!class_id.IsOk())
			break;
		
		wxTreeItemIdValue item_cookie;
		auto item_id = treeCtrlClasses->GetFirstChild(class_id,item_cookie);
		while(item_id.IsOk())
		{
			TreeNode *data = (TreeNode*)treeCtrlClasses->GetItemData(item_id);
			if(!data)
				continue;
			
			auto obj_id = terr->FindObject(data->m_obj);
			if(obj_id < 0)
				continue;
			terr->MoveObject(new_obj_id,obj_id);
			new_obj_id++;
			
			item_id = treeCtrlClasses->GetNextChild(class_id,item_cookie);
		}
		
		class_id = treeCtrlClasses->GetNextChild(root_id,cookie);
	}
}


FormObjects::~FormObjects()
{
}

void FormObjects::OnClose(wxCloseEvent& ev)
{
	//SortItems();
	wxPostEvent(GetParent(), ev);
	ev.Skip();
	Destroy();
}

// close form
void FormObjects::OnCloseClick(wxCommandEvent& event)
{
	Close();
}


// set current map poitner
void FormObjects::SetMap(SpellMap* map)
{
	spell_map = map;
	for(int k = 0; k < spell_data->GetTerrainCount(); k++)
	{
		Terrain* terr = spell_data->GetTerrain(k);
		wxString lab = terr->name;		
		if(map && map->terrain && map->terrain->name.compare(spell_data->GetTerrain(k)->name) == 0)
			lab = lab + " (active)";
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
	m_spell_obj = NULL;
	
	FillToolsClasses();
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


// render preview
void FormObjects::OnPaintCanvas(wxPaintEvent& event)
{	
	// make render buffer
	wxBitmap bmp(canvas->GetClientSize(),24);

	if(m_spell_obj)
	{					
		// get this terrain
		Terrain* terrain = FindTerrain();
		if(terrain)
		{
			// check object exist (just in case)
			if(std::find(terrain->objects.begin(),terrain->objects.end(),m_spell_obj) != terrain->objects.end())
			{
				// render preview
				double gamma = 0.001*(double)slideGamma->GetValue();
				m_spell_obj->RenderPreview(bmp, gamma);
			}
		}
	}
	
	// blit to screen
	wxPaintDC pdc(canvas);
	pdc.DrawBitmap(bmp,wxPoint(0,0));
}



// fills tool class menu
void FormObjects::FillToolsClasses()
{
	// get this terrain
	Terrain* terr = FindTerrain();

	treeCtrlClasses->DeleteAllItems();
	auto rid = treeCtrlClasses->AddRoot("Classes");	
	for(int k = 0; k <= terr->GetToolsCount(); k++)
	{
		wxTreeItemId cid;
		if(k == 0)
			cid = treeCtrlClasses->AppendItem(rid,"<Not assigned>",-1,-1,(wxTreeItemData*)new TreeNode(k));
		else
			cid = treeCtrlClasses->AppendItem(rid,terr->GetToolSetName(k - 1),-1,-1,(wxTreeItemData*)new TreeNode(k));		
				
		for(int oid = 0; oid < terr->GetObjectsCount(); oid++)
		{			
			auto obj = terr->objects[oid];
			if(obj->GetToolClass() == k)
				treeCtrlClasses->AppendItem(cid,obj->GetDescription(),-1,-1,(wxTreeItemData*)new TreeNode(obj));
		}
	}
	treeCtrlClasses->Expand(rid);	

}

// fill tool class items menu
void FormObjects::FillToolItemsList()
{
	// get this terrain
	Terrain* terr = FindTerrain();

	// add neutral item
	chbObjectsGroup->Clear();
	chbObjectsGroup->Append("None");
	chbObjectsGroup->Select(0);

	if(m_spell_obj)
	{
		
		int class_id = m_spell_obj->GetToolClass();
		if(class_id)
		{
			// make list of existing classes
			//SpellToolsGroup* grp = terr->GetToolSet(class_id - 1);

			// fill the list
			for(auto const & str : terr->GetToolSetItems(class_id - 1))
				chbObjectsGroup->Append(str);

			int item_id = m_spell_obj->GetToolClassGroup();
			chbObjectsGroup->Select(item_id);
		}

	}
}


// on change tool class item selection
void FormObjects::OnToolClassItemChange(wxCommandEvent& event)
{
	// get this terrain
	Terrain* terr = FindTerrain();

	if(m_spell_obj)
	{		
		// new tool class for sprite
		int class_id = m_spell_obj->GetToolClass();
		if(class_id)
		{
			// update tool class item selection
			int item_id = chbObjectsGroup->GetSelection();
			m_spell_obj->SetToolClassGroup(item_id);
		}
		else
			m_spell_obj->SetToolClassGroup(0); // invalidate item id if no tool class selected (should not happen)

		// refresh selectors
		//UpdateToolClassesView();
	}
}
