///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_objects.h"
#include "form_edit_toolset.h"
#include "sprites.h"
#include "other.h"
#include "wx_other.h"

#include <filesystem>
#include <regex>
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

	treeCtrlClasses = new wxTreeCtrl(this,wxID_TRC_CLASSES,wxDefaultPosition,wxDefaultSize,wxTR_DEFAULT_STYLE|wxTR_EDIT_LABELS|wxTR_HIDE_ROOT|wxTR_SINGLE|wxALWAYS_SHOW_SB|wxVSCROLL);
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


	this->SetSizer(szrMain);
	this->Layout();
	m_menubar2 = new wxMenuBar(0);
	mnuFile = new wxMenu();
	wxMenuItem* btnSaveList;
	btnSaveList = new wxMenuItem(mnuFile,wxID_MM_SAVE_OBJECTS,wxString(wxT("Save objects")),wxEmptyString,wxITEM_NORMAL);
	mnuFile->Append(btnSaveList);

	mnuFile->AppendSeparator();

	wxMenuItem* btnClose;
	btnClose = new wxMenuItem(mnuFile,wxID_MM_CLOSE,wxString(wxT("Close")) + wxT('\t') + wxT("Escape"),wxEmptyString,wxITEM_NORMAL);
	mnuFile->Append(btnClose);

	m_menubar2->Append(mnuFile,wxT("File"));

	mnuTerr = new wxMenu();
	m_menubar2->Append(mnuTerr,wxT("Terrain"));

	mnuEdit = new wxMenu();
	wxMenuItem* mmRemove;
	mmRemove = new wxMenuItem(mnuEdit,wxID_MM_REMOVE,wxString(wxT("Delete")) + wxT('\t') + wxT("Delete"),wxEmptyString,wxITEM_NORMAL);
	mnuEdit->Append(mmRemove);

	wxMenuItem* mmRename;
	mmRename = new wxMenuItem(mnuEdit,wxID_MM_RENAME,wxString(wxT("Rename")) + wxT('\t') + wxT("Ctrl+R"),wxEmptyString,wxITEM_NORMAL);
	mnuEdit->Append(mmRename);

	wxMenuItem* mmNewClass;
	mmNewClass = new wxMenuItem(mnuEdit,wxID_MM_NEW_CLASS,wxString(wxT("New toolset")) + wxT('\t') + wxT("Ctrl+N"),wxEmptyString,wxITEM_NORMAL);
	mnuEdit->Append(mmNewClass);

	wxMenuItem* mmNewTool;
	mmNewTool = new wxMenuItem(mnuEdit,wxID_MM_NEW_TOOL,wxString(wxT("New tool")) + wxT('\t') + wxT("Ctrl+T"),wxEmptyString,wxITEM_NORMAL);
	mnuEdit->Append(mmNewTool);

	wxMenuItem* mmEditToolset;
	mmEditToolset = new wxMenuItem(mnuEdit,wxID_MM_EDIT_TOOLSET,wxString(wxT("Edit toolset")) + wxT('\t') + wxT("Ctrl+E"),wxEmptyString,wxITEM_NORMAL);
	mnuEdit->Append(mmEditToolset);

	m_menubar2->Append(mnuEdit,wxT("Edit"));

	this->SetMenuBar(m_menubar2);


	this->Centre(wxBOTH);

	// === AUTO GENERATED END ===

	
	//treeCtrlClasses->image
	
	//slideGamma->SetTickFreq(100);
	// set icon
	wxIcon appIcon;
	appIcon.LoadFile("IDI_ICON2",wxBITMAP_TYPE_ICO_RESOURCE);
	if(appIcon.IsOk())
		SetIcon(appIcon);

	//wxInitAllImageHandlers();
	imlist = new wxImageList(16,16);
	wxBitmap img;
	img.LoadFile("IDI_MULTI",wxBITMAP_TYPE_BMP_RESOURCE);
	if(img.IsOk())
		imlist->Add(img);
	img.LoadFile("IDI_SINGLE",wxBITMAP_TYPE_BMP_RESOURCE);
	if(img.IsOk())
		imlist->Add(img);
	img.LoadFile("IDI_FOLDER",wxBITMAP_TYPE_BMP_RESOURCE);
	if(img.IsOk())
		imlist->Add(img);
	img.LoadFile("IDI_FOLDER_OPEN",wxBITMAP_TYPE_BMP_RESOURCE);
	if(img.IsOk())
		imlist->Add(img);
	treeCtrlClasses->SetImageList(imlist);

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
	Bind(wxEVT_MENU,&FormObjects::OnNewClass,this,wxID_MM_NEW_CLASS);
	Bind(wxEVT_MENU,&FormObjects::OnNewTool,this,wxID_MM_NEW_TOOL);
	Bind(wxEVT_MENU,&FormObjects::OnEditToolset,this,wxID_MM_EDIT_TOOLSET);


	// canvas stuff:
	canvas->SetDoubleBuffered(true);
	canvas->Bind(wxEVT_PAINT,&FormObjects::OnPaintCanvas,this);

	// object tool classes stuff:	
	FillToolsClasses();

	Bind(wxEVT_TREE_SEL_CHANGED,&FormObjects::OnTreeSelectionChanged,this,wxID_TRC_CLASSES);
	Bind(wxEVT_TREE_BEGIN_DRAG,&FormObjects::OnTreeClassBeginDrag,this,wxID_TRC_CLASSES);
	Bind(wxEVT_TREE_END_DRAG,&FormObjects::OnTreeClassEndDrag,this,wxID_TRC_CLASSES);
	Bind(wxEVT_TREE_BEGIN_LABEL_EDIT,&FormObjects::OnTreeClassBeginLabelEdit,this,wxID_TRC_CLASSES);
	Bind(wxEVT_TREE_END_LABEL_EDIT,&FormObjects::OnTreeClassEndLabelEdit,this,wxID_TRC_CLASSES);
	Bind(wxEVT_TREE_ITEM_MENU,&FormObjects::OnTreeClassMenu,this,wxID_TRC_CLASSES);

	// default map
	SetMap(NULL);
}

// build toolset title from name and description
std::string FormObjects::MakeToolsetTitle(std::string name,std::string desc)
{
	if(name.compare(desc) == 0 || desc.empty())
		return(name);
	return(name + ": " + desc);
}
// build toolset title from toolset id
std::string FormObjects::GetToolsetTitle(int toolset_class_id)
{
	auto terr = FindTerrain();
	return(MakeToolsetTitle(terr->GetToolSetName(toolset_class_id),terr->GetToolSetTitle(toolset_class_id)));
}
// rename toolset from single string "name: title"
void FormObjects::RenameToolset(int toolset_class_id,std::string title)
{
	regex secexp("([^:]*)[:\\s]*(.*)");
	smatch match;
	std::regex_search(title,match,secexp);
	if(match.size() != 3)
	{
		// failed
		return;
	}
	std::string name = match[1];
	title = match[2];
	if(title.empty())
		title = name;
	auto terr = FindTerrain();
	terr->SetToolSetName(toolset_class_id,name);
	terr->SetToolSetTitle(toolset_class_id,title);
}

// new toolset class
void FormObjects::OnNewClass(wxCommandEvent& evt)
{
	auto terr = FindTerrain();
	std::string name = "New toolset";
	terr->AddToolSet(name,name);
	auto toolset_id = terr->GetToolSetID(name);

	auto root_id = treeCtrlClasses->GetRootItem();
	auto new_id = treeCtrlClasses->AppendItem(root_id,GetToolsetTitle(toolset_id),Icons::FOLDER,Icons::FOLDER_OPEN,(wxTreeItemData*)new TreeNode(toolset_id+1));
	treeCtrlClasses->SelectItem(new_id);
	if(!treeCtrlClasses->IsVisible(new_id))
		treeCtrlClasses->EnsureVisible(new_id);
	treeCtrlClasses->EditLabel(new_id);
}

// new tool within class
void FormObjects::OnNewTool(wxCommandEvent& evt)
{
	auto item_id = treeCtrlClasses->GetSelection();
	auto* obj = (TreeNode*)treeCtrlClasses->GetItemData(item_id);
	if(!obj)
		return; // nope in root
	auto parent_id = treeCtrlClasses->GetItemParent(item_id);
	auto* par_obj = (TreeNode*)treeCtrlClasses->GetItemData(parent_id);
	int class_id = 0;
	if(obj->m_obj && par_obj && par_obj->m_class_id)
		class_id = par_obj->m_class_id;
	else if(obj->m_class_id)
		class_id = obj->m_class_id;
	else
		return;
	if(obj->m_tool_id <= 0)
		parent_id = item_id;
		
	auto terr = FindTerrain();
	std::string name = "New tool";
	terr->AddToolSetItem(class_id-1,name);
	auto tool_class_id = terr->GetToolSetItem(class_id-1,name);
	auto tool_id = treeCtrlClasses->AppendItem(parent_id,name,Icons::MULTI,-1,(wxTreeItemData*)new TreeNode(class_id,tool_class_id+1));
	treeCtrlClasses->SelectItem(tool_id);
	FillToolsClasses();
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
	if(!item_id.IsOk())
		return;
	auto* obj = (TreeNode*)treeCtrlClasses->GetItemData(item_id);
	if(!obj)
		return;
	if(!obj->m_obj && obj->m_class_id < 1)
		return;
	auto terr = FindTerrain();
	if(obj->m_obj)
	{
		// removing object
		terr->RemoveObject(terr->FindObject(obj->m_obj));
		//FillToolsClasses();
		treeCtrlClasses->Delete(item_id);
	}
	else if(obj->m_tool_id)
	{
		// remove tool
		terr->RemoveToolSetItem(obj->m_class_id-1,obj->m_tool_id-1);
		FillToolsClasses();
	}
	else
	{
		// removing toolset class
		terr->RemoveToolSet(obj->m_class_id - 1);
		FillToolsClasses();
	}
	SortItems();
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
	if(!obj->m_obj && obj->m_class_id < 1)
	{
		// no edit unassigned class name
		evt.Veto();
		return;
	}
}
void FormObjects::OnTreeClassEndLabelEdit(wxTreeEvent& evt)
{
	std::string text = evt.GetLabel().ToStdString();
	if(text.empty())
	{
		evt.Veto();
		return;
	}
	auto item_id = evt.GetItem();
	auto* obj = (TreeNode*)treeCtrlClasses->GetItemData(item_id);
	if(!obj)
	{
		// no edit root
		evt.Veto();
		return;
	}
	if(!obj->m_obj && obj->m_tool_id)
	{
		// rename tool name
		auto terr = FindTerrain();
		terr->RenameToolSetItem(obj->m_class_id-1,text,obj->m_tool_id-1);
		return;
	}
	if(!obj->m_obj)
	{
		// edit class name
		RenameToolset(obj->m_class_id - 1,text);
		treeCtrlClasses->SetItemText(item_id,GetToolsetTitle(obj->m_class_id - 1));
		evt.Veto();
		return;
	}
	// edit tool item name
	obj->m_obj->SetDescription(text);
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
	if(!target_item.IsOk())
		return;
	if(treeCtrlClasses->GetRootItem() == target_item)
		return; // cannot move to root
	auto target_obj = (TreeNode*)treeCtrlClasses->GetItemData(target_item);
	auto parent_node = treeCtrlClasses->GetItemParent(target_item);
	if(!obj->m_obj && obj->m_tool_id < 1)
	{
		// moving toolset class
		if(treeCtrlClasses->GetRootItem() == parent_node && target_obj->m_class_id > 0)
		{
			// target is class node		
			auto terr = FindTerrain();
			terr->MoveToolSet(obj->m_class_id-1,target_obj->m_class_id-1,true);
			treeCtrlClasses->SelectItem(m_drag_item);
			FillToolsClasses();
			treeCtrlClasses->Refresh();
			evt.Veto();
		}
	}
	else if(obj->m_tool_id)
	{
		// moving objects group
		if(target_obj->m_class_id == obj->m_class_id)
		{
			// within same toolset
			auto terr = FindTerrain();
			terr->MoveToolSetItem(obj->m_class_id-1,obj->m_tool_id-1,target_obj->m_tool_id-1,true);
			treeCtrlClasses->SelectItem(m_drag_item);
			FillToolsClasses();
			treeCtrlClasses->Refresh();
			evt.Veto();
		}
	}
	else
	{
		// moving tool item
		wxTreeItemId nid;
		int grp_id = 0;
		if(treeCtrlClasses->GetRootItem() == parent_node)
		{
			// target is class node: place it as single object at the end of the objects list
			nid = treeCtrlClasses->AppendItem(target_item,treeCtrlClasses->GetItemText(m_drag_item),Icons::SINGLE,-1,(wxTreeItemData*)new TreeNode(obj->m_obj));
		}
		else if(target_obj && target_obj->m_obj)
		{
			// target is object: insert at position
			grp_id = target_obj->m_obj->GetToolClassGroup();
			nid = treeCtrlClasses->InsertItem(parent_node,treeCtrlClasses->GetPrevSibling(target_item),treeCtrlClasses->GetItemText(m_drag_item),Icons::SINGLE,-1,(wxTreeItemData*)new TreeNode(obj->m_obj));
		}
		else if(target_obj && target_obj->m_tool_id > 0)
		{
			// target is objects group: append
			grp_id = target_obj->m_tool_id;
			nid = treeCtrlClasses->AppendItem(target_item,treeCtrlClasses->GetItemText(m_drag_item),Icons::SINGLE,-1,(wxTreeItemData*)new TreeNode(obj->m_obj));			
		}
		else
			return;
		treeCtrlClasses->SelectItem(nid);
		auto cls = (TreeNode*)treeCtrlClasses->GetItemData(treeCtrlClasses->GetItemParent(nid));
		obj->m_obj->SetToolClass(cls->m_class_id);
		obj->m_obj->SetToolClassGroup(grp_id);
		treeCtrlClasses->Delete(m_drag_item);
	}

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
	
	//FillToolItemsList();

	// is node
	m_spell_obj = obj->m_obj;
	canvas->Refresh();	
}

// edit toolset class properties
void FormObjects::OnEditToolset(wxCommandEvent& evt)
{
	auto item_id = treeCtrlClasses->GetSelection();
	if(!item_id.IsOk())
		return;
	auto* obj = (TreeNode*)treeCtrlClasses->GetItemData(item_id);
	auto terr = FindTerrain();
	auto form = new FormEditToolset(this,terr,obj->m_class_id-1);
	if(form->ShowModal() == wxID_OK)
	{
		// --- confirmed
	}
	delete form;
	FillToolsClasses();
}

// tool tree menu popup
void FormObjects::OnTreeClassMenu(wxTreeEvent& evt)
{
	auto item_id = evt.GetItem();
	if(!item_id.IsOk())
		return;
	treeCtrlClasses->SelectItem(item_id);
	auto* obj = (TreeNode*)treeCtrlClasses->GetItemData(item_id);

	wxMenu menu;// = new wxMenu();
	menu.SetClientData(item_id);
	if(obj)
		menu.Append(MNU_REMOVE,"Remove\tDelete");
	if(!obj->m_obj && obj->m_class_id > 0)
		menu.Append(MNU_NEW_TOOL,"New tool\tCtrl+T");
	menu.Append(MNU_NEW_TOOLSET,"New toolset\tCtrl+N");
	if(!obj->m_obj && obj->m_class_id > 0 && obj->m_tool_id <= 0)
		menu.Append(MNU_EDIT_TOOLSET,"Edit toolset parameters\tCtrl+E");
	menu.Connect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(FormObjects::OnTreeClassMenuClick),NULL,this);
	treeCtrlClasses->PopupMenu(&menu,evt.GetPoint());
}
// on tool tree menu click
void FormObjects::OnTreeClassMenuClick(wxCommandEvent& evt)
{
	auto menu_id = evt.GetId();
	auto menu = (wxMenu*)evt.GetEventObject();
	auto item_id = (wxTreeItemId)menu->GetClientData();
	if(menu_id == MNU_REMOVE)
	{
		wxCommandEvent event;
		OnRemove(event);
	}
	else if(menu_id == MNU_NEW_TOOL)
	{
		wxCommandEvent event;
		OnNewTool(event);
	}
	else if(menu_id == MNU_NEW_TOOLSET)
	{
		wxCommandEvent event;
		OnNewClass(event);
	}
	else if(menu_id == MNU_EDIT_TOOLSET)
	{
		wxCommandEvent event;
		OnEditToolset(event);
	}
}




FormObjects::~FormObjects()
{
	delete imlist;
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
		if(map && spell_data->GetTerrain(k) == map->terrain)
		{
			// found selection
			GetMenuBar()->FindItem(TERR_ID0 + k)->Check(true);
			break;
		}
		
		/*Terrain* terr = spell_data->GetTerrain(k);
		wxString lab = terr->name;		
		if(map && map->terrain && map->terrain->name.compare(spell_data->GetTerrain(k)->name) == 0)
			lab = lab + " (active)";
		mnuTerr->SetLabel(TERR_ID0 + k,lab);*/
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




class TreeCtrlState{
public:
	int level;
	bool state;
	bool sel;
	std::string name;
};




// fills tool class menu
void FormObjects::FillToolsClasses()
{
	// get this terrain
	Terrain* terr = FindTerrain();
	
	// remember last expand states
	wxTreeLister lister(treeCtrlClasses);

	treeCtrlClasses->DeleteAllItems();
	auto root_id = treeCtrlClasses->AddRoot("Classes",Icons::FOLDER,Icons::FOLDER_OPEN);	
	for(int k = 0; k <= terr->GetToolsCount(); k++)
	{
		wxTreeItemId cid;
		if(k == 0)
			cid = treeCtrlClasses->AppendItem(root_id,"<Not assigned>",Icons::FOLDER,Icons::FOLDER_OPEN,(wxTreeItemData*)new TreeNode(k));
		else
			cid = treeCtrlClasses->AppendItem(root_id,GetToolsetTitle(k - 1),Icons::FOLDER,Icons::FOLDER_OPEN,(wxTreeItemData*)new TreeNode(k));
		
		for(int tid = 0; tid < terr->GetToolSetItemsCount(k-1); tid++)
		{
			auto name = terr->GetToolSetItem(k-1,tid);
			auto group_id = treeCtrlClasses->AppendItem(cid,name,Icons::MULTI,-1,(wxTreeItemData*)new TreeNode(k,tid + 1));
			
			for(int oid = 0; oid < terr->GetObjectsCount(); oid++)
			{
				auto obj = terr->objects[oid];
				if(obj->GetToolClass() == k && obj->GetToolClassGroup() == tid + 1)
					treeCtrlClasses->AppendItem(group_id,obj->GetDescription(),Icons::SINGLE,-1,(wxTreeItemData*)new TreeNode(obj));
			}
		}

		for(int oid = 0; oid < terr->GetObjectsCount(); oid++)
		{			
			auto obj = terr->objects[oid];
			if(obj->GetToolClass() == k && obj->GetToolClassGroup() == 0)
				treeCtrlClasses->AppendItem(cid,obj->GetDescription(),Icons::SINGLE,-1,(wxTreeItemData*)new TreeNode(obj));
		}
	}
	// try restore last expand states
	lister.treeCtrlSetStates(treeCtrlClasses);
}


// sort objects per classe (call before leaving)
void FormObjects::SortItems()
{
	auto terr = FindTerrain();

	// for each class:
	int new_obj_id = 0;
	auto root_id = treeCtrlClasses->GetRootItem();

	// for each toolset class:
	wxTreeItemIdValue cookie;
	auto class_id = treeCtrlClasses->GetFirstChild(root_id,cookie);
	while(true)
	{
		if(!class_id.IsOk())
			break;

		// for each tool/object:
		wxTreeItemIdValue item_cookie;
		auto item_id = treeCtrlClasses->GetFirstChild(class_id,item_cookie);
		while(item_id.IsOk())
		{
			TreeNode* data = (TreeNode*)treeCtrlClasses->GetItemData(item_id);
			if(!data)
			{
				item_id = treeCtrlClasses->GetNextChild(class_id,item_cookie);
				continue;
			}

			if(data->m_tool_id)
			{
				// tool
				wxTreeItemIdValue tool_cookie;
				auto tool_id = treeCtrlClasses->GetFirstChild(item_id,tool_cookie);
				while(tool_id.IsOk())
				{
					TreeNode* data = (TreeNode*)treeCtrlClasses->GetItemData(tool_id);
					if(data->m_obj)
					{
						auto obj_id = terr->FindObject(data->m_obj);
						if(obj_id >= 0)
						{
							terr->MoveObject(new_obj_id,obj_id);
							new_obj_id++;
						}
					}
					tool_id = treeCtrlClasses->GetNextChild(item_id,tool_cookie);
				}
			}
			else if(data->m_obj)
			{
				// object
				auto obj_id = terr->FindObject(data->m_obj);
				if(obj_id >= 0)
				{
					terr->MoveObject(new_obj_id,obj_id);
					new_obj_id++;
				}
			}

			item_id = treeCtrlClasses->GetNextChild(class_id,item_cookie);
		}

		class_id = treeCtrlClasses->GetNextChild(root_id,cookie);
	}
}