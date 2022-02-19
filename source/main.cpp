// wxWidgets "Hello World" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "wx/dcgraph.h"
#include "wx/dcbuffer.h"
#include <wx/rawbmp.h>
#include <wx/timer.h>
#include <wx/filedlg.h>
#include "wx/slider.h"

#include <filesystem>
#include <codecvt>
#include <tuple>
#include <string>

#include "main.h"
#include "other.h"
#include "simpleini.h"
#include "spellcross.h"
#include "map.h"





wxIMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
    // load config.ini
    ini.SetUnicode();
    ini.LoadFile("config.ini");

    // spellcross data root path
    wstring spelldata_path = char2wstring(ini.GetValue("SPELCROS","spell_path",""));
    // special data folder
    wstring spec_folder = char2wstring(ini.GetValue("DATA","spec_data_path",""));

    // try load spellcross data
    spell_data = new SpellData(spelldata_path, spec_folder);

    // for each terrain load tile context
    for(int k = 0; k < spell_data->GetTerrainCount(); k++)
    {
        // get terrain
        Terrain *terr = spell_data->GetTerrain(k);
        // make INI section
        string sec_name = "TERRAIN::" + string(terr->name);
        
        // try to load context
        wstring cont_path = char2wstring(ini.GetValue(sec_name.c_str(), "context_path", ""));        
        terr->InitSpriteContext(cont_path);        
    }
          

    // load some map
    wstring map_path = char2wstring(ini.GetValue("STATE","last_map",""));
    spell_map = new SpellMap();
    spell_map->Load(map_path,spell_data);
    spell_map->SetGamma(1.3);

    
    // for saving PNG file (among other stuff)
    wxInitAllImageHandlers();
        
    
    // --- run main form    
    // main window frame
    MyFrame* frame = new MyFrame(spell_map, spell_data);
    // show main frame
    frame->Show(true);
    return(true);
}
int MyApp::OnExit()
{
    // store last path
    ini.SetValue("STATE","last_map",wstring2string(spell_map->GetTopPath()).c_str());
    // save INI
    ini.SaveFile("config.ini");

    // loose map
    delete spell_map;

    // loose spell data
    delete spell_data;

    return(0);
}


MyFrame::MyFrame(SpellMap* map, SpellData* spelldata):wxFrame(NULL, wxID_ANY, "Spellcross Map Editor", wxDefaultPosition, wxSize(1600,1000))
{
    // store local reference to initial map and data
    spell_map = map;
    spell_data = spelldata;

    // subforms
    form_objects = NULL;

    // view scroller
    scroll.Reset();
        
    // File menu
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_OpenMap, "&Open Map\tCtrl-O", "Open new Spellcross map file.");
    menuFile->Append(ID_NewMap,"&Ceate new Map\tCtrl-N","Create new map.");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    
    // View menu
    wxMenu* menuView = new wxMenu;
    menuView->Append(ID_ViewTer,"Layer 1: Terrain\tF1","",wxITEM_CHECK);
    menuView->FindItem(ID_ViewTer)->Check(true);
    menuView->Append(ID_ViewObj,"Layer 2: Objects\tF2","",wxITEM_CHECK);
    menuView->FindItem(ID_ViewObj)->Check(true);
    menuView->Append(ID_ViewAnm,"Layer 3: Tile anmiations\tF3","",wxITEM_CHECK);
    menuView->FindItem(ID_ViewAnm)->Check(true);
    menuView->Append(ID_ViewPnm,"Layer 4: Sprite animations\tF4","",wxITEM_CHECK);
    menuView->FindItem(ID_ViewPnm)->Check(true);
    menuView->Append(ID_ViewUnt,"Layer 5: Units\tF5","",wxITEM_CHECK);
    menuView->FindItem(ID_ViewUnt)->Check(true);
    menuView->Append(ID_ViewStTa,"Layer 6: Start/Target\tF6","",wxITEM_CHECK);
    menuView->FindItem(ID_ViewStTa)->Check(true);
    menuView->Append(wxID_ANY,"","",wxITEM_SEPARATOR);
    menuView->Append(ID_SetGamma,"Set gamma","",wxITEM_NORMAL);

    // edit menu
    wxMenu* menuEdit = new wxMenu;
    menuEdit->Append(ID_SelectAll,"Select all tiles\tCtrl+A","",wxITEM_NORMAL);
    menuEdit->Append(ID_DeselectAll,"Deselect all tiles\tCtrl+Shift+A","",wxITEM_NORMAL);
    menuEdit->Append(wxID_ANY,"","",wxITEM_SEPARATOR);
    menuEdit->Append(ID_InvalidateSel,"Invalidate selection\tCtrl+I","",wxITEM_NORMAL);
    menuEdit->Append(wxID_ANY,"","",wxITEM_SEPARATOR);
    menuEdit->Append(ID_CreateNewObject,"Create new object\tCtrl+Shift+O","",wxITEM_NORMAL);
    menuEdit->Append(wxID_ANY,"","",wxITEM_SEPARATOR);
    menuEdit->Append(ID_MoveUnit,"Move unit\tCtrl+M","",wxITEM_NORMAL);

    
    // tools
    wxMenu* menuTools = new wxMenu;
    menuTools->Append(ID_ViewSprites,"Sprites editor","",wxITEM_NORMAL);
    menuTools->Append(ID_ViewObjects,"Objects editor","",wxITEM_NORMAL);
    menuTools->Append(ID_ViewTools, "Tools editor", "", wxITEM_NORMAL);
    menuTools->Append(ID_ViewPal,"Palette viewer","",wxITEM_NORMAL);
    menuTools->Append(ID_ViewGRes,"Graphics viewer","",wxITEM_NORMAL);
    menuTools->Append(ID_SetGamma,"","",wxITEM_SEPARATOR);
    menuTools->Append(ID_UpdateSprContext, "Update tile context from map","",wxITEM_NORMAL);
    menuTools->Append(ID_UpdateSprContextMaps,"Update tile context from ALL maps","",wxITEM_NORMAL);
    
        
    // Help menu
    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    
    // Main menu
    wxMenuBar* menuBar = new wxMenuBar;    
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuEdit, "&Edit");
    menuBar->Append(menuView, "&View");
    menuBar->Append(menuTools,"&Tools");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);
    
    CreateStatusBar(8);
    const int ss_w[] = {45,45,45,60,100,100,100,-1};
    SetStatusWidths(8,ss_w);
    SetStatusText("");
      

    m_timer.SetOwner(this);
    this->Connect(wxEVT_TIMER,wxTimerEventHandler(MyFrame::OnTimer),NULL,this);
    m_timer.Start(100);


    // main sizer 
    sizer = new wxBoxSizer(wxVERTICAL); 
    this->SetSizeHints(wxDefaultSize,wxDefaultSize);

    // toolset ribbon
    //ribbonBar = new wxRibbonBar(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxRIBBON_BAR_DEFAULT_STYLE);
    //ribbonBar->SetArtProvider(new wxRibbonDefaultArtProvider);
    ribbonBar = NULL;
    LoadToolsetRibbon();
    //sizer->Add(ribbonBar,0,wxALL|wxEXPAND,2);
    

    Bind(wxEVT_RIBBONBUTTONBAR_CLICKED,&MyFrame::OnToolBtnClick,this);
    Bind(wxEVT_RIBBONBAR_PAGE_CHANGED,&MyFrame::OnToolPageClick,this);

    // make and attach render canvas
    canvas = new wxPanel(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxTAB_TRAVERSAL);
    sizer->Add(canvas,1,wxEXPAND|wxALL,1);
    canvas->SetBackgroundStyle(wxBG_STYLE_PAINT);
    canvas->SetDoubleBuffered(true);
    

    canvas->Bind(wxEVT_PAINT,&MyFrame::OnPaintCanvas,this);
    canvas->Bind(wxEVT_RIGHT_DOWN,&MyFrame::OnCanvasMouseDown,this);
    canvas->Bind(wxEVT_RIGHT_UP,&MyFrame::OnCanvasMouseUp,this);
    canvas->Bind(wxEVT_MOTION,&MyFrame::OnCanvasMouseMove,this);
    canvas->Bind(wxEVT_LEAVE_WINDOW,&MyFrame::OnCanvasMouseLeave,this);
    canvas->Bind(wxEVT_ENTER_WINDOW,&MyFrame::OnCanvasMouseEnter,this);
    canvas->Bind(wxEVT_MOUSEWHEEL,&MyFrame::OnCanvasMouseWheel,this);
    canvas->Bind(wxEVT_KEY_DOWN,&MyFrame::OnCanvasKeyDown,this);
    canvas->Bind(wxEVT_LEFT_DOWN,&MyFrame::OnCanvasLMouseDown,this);




    this->SetSizer(sizer);    
    this->SetAutoLayout(true);
    this->Layout();

    Bind(wxEVT_CLOSE_WINDOW, &MyFrame::OnClose, this);
    
    Bind(wxEVT_MENU, &MyFrame::OnOpenMap, this, ID_OpenMap);
    Bind(wxEVT_MENU,&MyFrame::OnNewMap,this,ID_NewMap);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);

    Bind(wxEVT_MENU,&MyFrame::OnViewLayer,this,ID_ViewTer);
    Bind(wxEVT_MENU,&MyFrame::OnViewLayer,this,ID_ViewObj);
    Bind(wxEVT_MENU,&MyFrame::OnViewLayer,this,ID_ViewAnm);
    Bind(wxEVT_MENU,&MyFrame::OnViewLayer,this,ID_ViewPnm);
    Bind(wxEVT_MENU,&MyFrame::OnViewLayer,this,ID_ViewUnt);
    Bind(wxEVT_MENU,&MyFrame::OnViewLayer,this,ID_ViewStTa);

    Bind(wxEVT_MENU,&MyFrame::OnSetGamma,this,ID_SetGamma);
    Bind(wxEVT_MENU,&MyFrame::OnViewSprites,this,ID_ViewSprites);
    Bind(wxEVT_MENU,&MyFrame::OnViewObjects,this,ID_ViewObjects);
    Bind(wxEVT_MENU,&MyFrame::OnViewTools, this, ID_ViewTools);
    Bind(wxEVT_MENU,&MyFrame::OnViewPal,this,ID_ViewPal);
    Bind(wxEVT_MENU,&MyFrame::OnViewGrRes,this,ID_ViewGRes);
    Bind(wxEVT_MENU,&MyFrame::OnUpdateTileContext,this,ID_UpdateSprContext);
    Bind(wxEVT_MENU,&MyFrame::OnUpdateTileContextMaps,this,ID_UpdateSprContextMaps);

    Bind(wxEVT_MENU,&MyFrame::OnSelectAll,this,ID_SelectAll);
    Bind(wxEVT_MENU,&MyFrame::OnDeselectAll,this,ID_DeselectAll);
    Bind(wxEVT_MENU,&MyFrame::OnInvalidateSelection,this,ID_InvalidateSel);
    Bind(wxEVT_MENU,&MyFrame::OnCreateNewObject,this,ID_CreateNewObject);
    Bind(wxEVT_MENU,&MyFrame::OnMoveUnit,this,ID_MoveUnit);


    // main sizer 
    /*auto sizer2 = new wxBoxSizer(wxVERTICAL);

    wxButton *btnOk = new wxButton(canvas,wxID_ANY,wxT("TEST"),wxDefaultPosition,wxDefaultSize,0);
    sizer2->Add(btnOk,0,wxALL,5);

    canvas->SetSizer(sizer2);
    canvas->SetAutoLayout(true);
    canvas->Layout();*/

    
    
}
// on form close
void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}
// about message
void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("Experimental Spellcross map editor, V1.0\n(c) Stanislav Maslan, s.maslan@seznam.cz","About Spellcross Map Editor",wxOK | wxICON_INFORMATION);
}
// callback function to write status messages from within the spellcross routines:
// usage: make and pass callback pointer using: bind(&MyFrame::StatusStringCallback,this,placeholders::_1)
// spellcross function example:
// void whatever_function(std::function<void(std::string)> status_cb)
// {
//   status_cb("Some message");
// }
void MyFrame::StatusStringCallback(std::string info)
{
    SetStatusText(info,7);
}

void MyFrame::OnClose(wxCloseEvent& ev)
{
    if (ev.GetId() == ID_TOOLS_WIN)
    {
        form_tools->Destroy();
        LoadToolsetRibbon();
    }
    else if (ev.GetId() == ID_OBJECTS_WIN)
    {
        form_objects->Destroy();
        LoadToolsetRibbon();
    }
    else if (ev.GetId() == ID_SPRITES_WIN)
    {
        form_sprites->Destroy();
        LoadToolsetRibbon();
    }
    else if(ev.GetId() == ID_PAL_WIN)
    {
        form_pal->Destroy();
    }
    else if(ev.GetId() == ID_GRES_WIN)
    {
        form_gres->Destroy();
    }
    else
        ev.Skip();
}







// map animation periodic refresh tick
void MyFrame::OnTimer(wxTimerEvent& event)
{
    if(spell_map->Tick())
    {
        canvas->Refresh();
    }
}
// on main panel resizing
void MyFrame::OnResize(wxSizeEvent& event)
{
    Refresh();
}

// render canvas repaint event
void MyFrame::OnPaintCanvas(wxPaintEvent& event)
{
    // make buffer
    if(!m_buffer.IsOk() || m_buffer.GetSize() != canvas->GetClientSize())
        m_buffer = wxBitmap(canvas->GetClientSize(),24);

    // render map
    wxPaintDC pdc(canvas);
    spell_map->Render(m_buffer,&scroll,&spell_tool,bind(&MyFrame::CreateHUDbuttons,this));
    pdc.DrawBitmap(m_buffer,wxPoint(0,0));

    event.Skip();
}


void MyFrame::CreateHUDbuttons()
{
    // mark all buttons as unused
    for(auto & pan : hud_buttons)
    {
        pan->SetClientData(0);
    }

    // create new ones
    int button_id = ID_HUD_BASE;
    for(auto & btn : *spell_map->GetHUDbuttons())
    {        
        // try to find existing button
        int skip = false;
        for(auto& pan : hud_buttons)
        {
            if(pan->GetPosition().x == btn->x_pos && pan->GetPosition().y == btn->y_pos)
            {
                btn->wx_id = button_id++;
                pan->SetId(btn->wx_id);
                pan->SetClientData((void*)1);
                pan->Refresh();
                skip = true;
                break;
            }
        }
        if(skip)
            continue;
        
        btn->wx_id = button_id++;
        wxPanel *wx_btn = new wxPanel(canvas,btn->wx_id,wxPoint(btn->x_pos, btn->y_pos),wxSize(btn->x_size, btn->y_size));
        wx_btn->SetClientData((void*)1);
        wx_btn->SetWindowStyle(wxTRANSPARENT_WINDOW);       
        wx_btn->SetBackgroundStyle(wxBG_STYLE_PAINT);
        wx_btn->SetDoubleBuffered(true);        
        wx_btn->Bind(wxEVT_PAINT,&MyFrame::OnPaintHUDbutton,this);
        wx_btn->Bind(wxEVT_LEAVE_WINDOW,&MyFrame::OnHUDbuttonsLeave,this);
        wx_btn->Bind(wxEVT_ENTER_WINDOW,&MyFrame::OnHUDbuttonsMouseEnter,this);
        wx_btn->Bind(wxEVT_LEFT_DOWN,&MyFrame::OnHUDbuttonsClick,this);
        wx_btn->Bind(wxEVT_LEFT_UP,&MyFrame::OnHUDbuttonsClick,this);
        hud_buttons.push_back(wx_btn);
    }

    // loose old unused buttons   
    for(int pid = hud_buttons.size()-1; pid >= 0; pid--)
    {
        if(!hud_buttons[pid]->GetClientData())
        {
            hud_buttons[pid]->Destroy();
            hud_buttons.erase(hud_buttons.begin() + pid);
        }
    }
}
void MyFrame::OnPaintHUDbutton(wxPaintEvent& event)
{
    wxPanel* pan = (wxPanel*)event.GetEventObject();
    auto* btn = spell_map->GetHUDbutton(pan->GetId());
    if(btn)
    {                
        wxPaintDC pdc(pan);
        if(btn->is_press)
            pdc.DrawBitmap(*btn->bmp_press,wxPoint(0,0));
        else if(btn->is_hover)
            pdc.DrawBitmap(*btn->bmp_hover,wxPoint(0,0));
        else
            pdc.DrawBitmap(*btn->bmp_idle,wxPoint(0,0));
    }
    event.Skip();
}
void MyFrame::OnHUDbuttonsMouseEnter(wxMouseEvent& event)
{
    wxPanel* pan = (wxPanel*)event.GetEventObject();
    auto* btn = spell_map->GetHUDbutton(pan->GetId());
    if(btn)
    {
        btn->is_hover = true;
        pan->Refresh();
        // click event callback?
        if(btn->is_hover && btn->cb_hover)
            btn->cb_hover();
    }
}
void MyFrame::OnHUDbuttonsLeave(wxMouseEvent& event)
{
    wxPanel* pan = (wxPanel*)event.GetEventObject();
    auto* btn = spell_map->GetHUDbutton(pan->GetId());
    if(btn)
    {
        btn->is_hover = false;
        pan->Refresh();
    }
}
void MyFrame::OnHUDbuttonsClick(wxMouseEvent& event)
{
    wxPanel* pan = (wxPanel*)event.GetEventObject();
    auto* btn = spell_map->GetHUDbutton(pan->GetId());
    if(btn)
    {
        btn->is_press = (event.GetEventType() == wxEVT_LEFT_DOWN);        
        pan->Refresh();
        // click event callback?
        if(!btn->is_press && btn->cb_press)
            btn->cb_press();
    }
}



// on change of map layer view
void MyFrame::OnViewLayer(wxCommandEvent& event)
{
    bool wL1 = GetMenuBar()->FindItem(ID_ViewTer)->IsChecked();
    bool wL2 = GetMenuBar()->FindItem(ID_ViewObj)->IsChecked();
    bool wL3 = GetMenuBar()->FindItem(ID_ViewAnm)->IsChecked();
    bool wL4 = GetMenuBar()->FindItem(ID_ViewPnm)->IsChecked();
    bool wL5 = GetMenuBar()->FindItem(ID_ViewUnt)->IsChecked();
    bool wSS = GetMenuBar()->FindItem(ID_ViewStTa)->IsChecked();
    spell_map->SetRender(wL1,wL2,wL3,wL4,wSS,wL5);
    Refresh();
}

// open new map
void MyFrame::OnOpenMap(wxCommandEvent& event)
{
    // split path to folder and file
    std::filesystem::path last_path = spell_map->GetTopPath();
    wstring dir = last_path.parent_path(); dir += wstring(L"\\");
    wstring name = last_path.filename();
    
    // show open dialog
    wxFileDialog openFileDialog(this,_("Open Spellcross Map File"),dir,name,"Map script file (*.def)|*.def|Map data file (*.dta)|*.dta",
        wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if(openFileDialog.ShowModal() == wxID_CANCEL)
        return;
    wstring path = wstring(openFileDialog.GetPath().ToStdWstring());

    // load new one
    spell_map->Load(path, spell_data);
    // reset layers visibility
    spell_map->SetGamma(1.30);
    OnViewLayer(event);
    // reset scroller
    scroll.Reset();
    // reload toolset ribbon
    LoadToolsetRibbon();
}

// create new map
void MyFrame::OnNewMap(wxCommandEvent& event)
{
    // create some map (###todo: set parameters by some menu)
    spell_map->Create(spell_data, "T11", 20,50);
    // reset layers visibility
    spell_map->SetGamma(1.30);
    OnViewLayer(event);
    // reset scroller
    scroll.Reset();
}

// set gamma correction
void MyFrame::OnSetGamma(wxCommandEvent& event)
{
    FormGamma *gamma_form = new FormGamma(this, spell_map);
    gamma_form->Show(true);
}

// open sprite viewer
void MyFrame::OnViewSprites(wxCommandEvent& event)
{
    if(!FindWindowById(ID_SPRITES_WIN))
    {
        form_sprites = new FormSprite(this, spell_data, ID_SPRITES_WIN);
        form_sprites->Show();
    }
}

// update tiles context from map selection
void MyFrame::OnUpdateTileContext(wxCommandEvent& event)
{
    spell_map->BuildSpriteContext();
}

// update tiles context from all maps
void MyFrame::OnUpdateTileContextMaps(wxCommandEvent& event)
{   
    // split path to folder and file
    wstring dir = spell_data->spell_data_root + L"\\DATA\\COMMON\\";

    // show open dialog
    wxDirDialog fd = wxDirDialog(this, _("Select Spellcross COMMON folder"), dir, wxDD_DIR_MUST_EXIST);
    if(fd.ShowModal() == wxID_CANCEL)
        return;
    wstring path = wstring(fd.GetPath().ToStdWstring());
    
    // load map context (###todo: add terrain type selector?)
    spell_data->BuildSpriteContextOfMaps(path, "T11", bind(&MyFrame::StatusStringCallback,this,placeholders::_1));
}


// open tools editor
void MyFrame::OnViewTools(wxCommandEvent& event)
{
    if (!FindWindowById(ID_TOOLS_WIN))
    {
        form_tools = new FormTools(this, spell_data, ID_TOOLS_WIN);
        //form_tools->Connect(wxID_ANY, wxEVT_DESTROY, (wxObjectEventFunction)&MyFrame::OnViewToolsClose);
        form_tools->SetMap(spell_map);
        form_tools->Show();
    }
}

// open objects viewer
void MyFrame::OnViewObjects(wxCommandEvent& event)
{
    if(!FindWindowById(ID_OBJECTS_WIN))
    {
        form_objects = new FormObjects(this,spell_data,ID_OBJECTS_WIN);
        //form_objects->Connect(wxID_ANY,wxEVT_DESTROY,(wxObjectEventFunction)&MyFrame::OnViewObjectsClose);
        form_objects->SetMap(spell_map);
        form_objects->Show();
    }
}

// open palette viewer
void MyFrame::OnViewPal(wxCommandEvent& event)
{
    if(!FindWindowById(ID_PAL_WIN))
    {
        form_pal = new FormPalView(this,spell_data,ID_PAL_WIN);
        form_pal->SetMap(spell_map);
        form_pal->Show();
    }
}

// open graphics viewer
void MyFrame::OnViewGrRes(wxCommandEvent& event)
{
    if(!FindWindowById(ID_GRES_WIN))
    {
        form_gres = new FormGResView(this,spell_data,ID_GRES_WIN);
        form_gres->Show();
    }
}


// create new object
void MyFrame::OnCreateNewObject(wxCommandEvent& event)
{
    FormNewObject* form = new FormNewObject(this,spell_map->terrain);
    if(form->ShowModal() == wxID_OK)
    {
        // --- confirmed

        // get selection    
        auto xy_list = spell_map->GetPersistSelections();
        auto L1_spr_list = spell_map->GetL1sprites(xy_list);
        auto L2_spr_list = spell_map->GetL2sprites(xy_list);
        auto flag_list = spell_map->GetFlags(xy_list);

        // get object descriptions
        std::string description = form->GetDescription();
        
        // add object to list
        spell_map->terrain->AddObject(xy_list, L1_spr_list, L2_spr_list, flag_list, (uint8_t*)spell_map->terrain->pal, description);
                
        // clear selection
        spell_map->SelectTiles(SpellMap::SELECT_CLEAR);
    }
    
    // destroy form
    delete form;    
}

// move unit (if selected)
void MyFrame::OnMoveUnit(wxCommandEvent& event)
{
    auto *unit = spell_map->GetSelectedUnit();
    if(unit)
    {
        // start unti movement
        unit->in_placement = true;
    }
}




// --- scrolling control ---
void MyFrame::OnCanvasMouseDown(wxMouseEvent& event)
{    
    scroll.SetRef(event.GetX(), event.GetY());
}
void MyFrame::OnCanvasMouseEnter(wxMouseEvent& event)
{
    canvas->SetFocus();
}
void MyFrame::OnCanvasMouseLeave(wxMouseEvent& event)
{
    scroll.Idle();
}
void MyFrame::OnCanvasMouseUp(wxMouseEvent& event)
{
    scroll.Idle();
}
void MyFrame::OnCanvasMouseMove(wxMouseEvent& event)
{
    scroll.Move(event.GetX(), event.GetY());
    
    // update map selection
    MapXY mxy = spell_map->GetSelection(m_buffer,&scroll);
    int elev = spell_map->GetElevation(m_buffer,&scroll);
    SetStatusText(wxString::Format(wxT("x=%d"),mxy.x),0);
    SetStatusText(wxString::Format(wxT("y=%d"),mxy.y),1);    
    SetStatusText(wxString::Format(wxT("z=%d"),elev),2);
    SetStatusText(wxString::Format(wxT("xy=%d"),spell_map->ConvXY(mxy)),3);
    SetStatusText(wxString::Format(wxT("L1: %s"),spell_map->GetL1tileName(m_buffer,&scroll)),4);
    SetStatusText(wxString::Format(wxT("L2: %s"),spell_map->GetL2tileName(m_buffer,&scroll)),5);
    //int height, flags, code;
    auto [flags,height,code] = spell_map->GetTileFlags(m_buffer,&scroll);
    SetStatusText(wxString::Format(wxT("(0x%02X)"),code),6);

    auto* unit = spell_map->GetSelectedUnit();
    if(unit && unit->in_placement && mxy.IsSelected())
    {
        // change unit position
        unit->coor = mxy;
        // sort units before rendering
        spell_map->SortUnits();

        spell_map->ClearUnitsView(true);
        spell_map->AddUnitView(unit);
    }
    
    canvas->Refresh();
}
void MyFrame::OnCanvasMouseWheel(wxMouseEvent& event)
{
    scroll.ResizeSelection(event.GetWheelRotation()/event.GetWheelDelta());
    canvas->Refresh();
}
void MyFrame::OnCanvasKeyDown(wxKeyEvent& event)
{
    if(event.ControlDown())
    {
        int key = event.GetKeyCode();
        if(key == WXK_CONTROL)
            return;

        // --- edit terrain elevation:
        int step = 0;
        if(key == WXK_PAGEUP)
            step++;
        else if(key == WXK_PAGEDOWN)
            step--;
        if(step != 0)
        {
            spell_map->EditElev(m_buffer, &scroll, step);
            Refresh();
        }

        // --- edit selection:
        if(key == '=' || key == '+')
        {
            // add selection
            auto list = spell_map->GetSelections(m_buffer, &scroll);
            spell_map->SelectTiles(list, SpellMap::SELECT_ADD);
        }
        else if(key == '-')
        {
            // clear selection
            auto list = spell_map->GetSelections(m_buffer, &scroll);
            spell_map->SelectTiles(list,SpellMap::SELECT_CLEAR);
        }
    }    
}
// select all tiles
void MyFrame::OnSelectAll(wxCommandEvent& event)
{
    spell_map->SelectTiles(SpellMap::SELECT_ADD);
}
void MyFrame::OnDeselectAll(wxCommandEvent& event)
{
    spell_map->SelectTiles(SpellMap::SELECT_CLEAR);
}
// invalidate map region (retexturing)
void MyFrame::OnInvalidateSelection(wxCommandEvent& event)
{
    // get selected area (preference of persistent selection over cursor)
    std::vector<MapXY> list;    
    list = spell_map->GetPersistSelections();
    if(list.empty())
        list = spell_map->GetSelections(m_buffer,&scroll);

    // invalidate region    
    spell_map->IvalidateTiles(list, bind(&MyFrame::StatusStringCallback,this,placeholders::_1));
}


// tool edit click
void MyFrame::OnCanvasLMouseDown(wxMouseEvent& event)
{
    // get selection
    auto xy_list = spell_map->GetSelections(m_buffer, &scroll);

    if(spell_tool.isActive() && xy_list.size() && xy_list[0].IsSelected())
    {
        // something selected: edit map class        
        spell_map->EditClass(xy_list, &spell_tool, bind(&MyFrame::StatusStringCallback,this,placeholders::_1));
    }
    else
    {
        // try select unit:

        auto* cur_unit = spell_map->GetCursorUnit(m_buffer,&scroll);
        auto* sel_unit = spell_map->GetSelectedUnit();
        if(cur_unit && cur_unit == sel_unit)
        {
            // move/place unit
            sel_unit->in_placement = !sel_unit->in_placement;
        }
        else
        {
            // try select unit (if on cursor)
            auto* unit = spell_map->GetCursorUnit(m_buffer, &scroll);
            spell_map->SelectUnit(unit);
        }
    }

    canvas->Refresh();
}

// tool selected
void MyFrame::OnToolBtnClick(wxRibbonButtonBarEvent& event)
{
    // get button id
    int id = event.GetId();

    // no tool selection
    spell_tool.Set();

    if(!spell_map->IsLoaded())
        return;
    if(!ribbonBar)
        return;

    int tool_id = (id - ID_TOOL_BASE)/ID_TOOL_CLASS_STEP;
    int item_id = (id - ID_TOOL_BASE)%ID_TOOL_CLASS_STEP;

    // very schmutzig way to deselect all other tool buttons
    for(int tid = 0; tid < ribbonBar->GetPageCount(); tid++)
    {        
        // get button bar
        wxRibbonPage* page = ribbonBar->GetPage(tid);
        auto wlist = page->GetChildren();
        if(!wlist.size())
            continue;
        wxRibbonPanel* panel = (wxRibbonPanel*)wlist[0];
        auto clist = panel->GetChildren();
        if(!clist.size())
            continue;
        wxRibbonButtonBar* btns = (wxRibbonButtonBar*)clist[0];

        // for each button:
        for(int iid = 0; iid < btns->GetButtonCount(); iid++)
        {
            int btn_id = ID_TOOL_BASE + tid*ID_TOOL_CLASS_STEP + iid;            
            
            if(id != btn_id)
                btns->ToggleButton(btn_id, false);
            else
            {            
                // this button (event caller):
                if(event.IsChecked())
                {
                    // some tool selected: setup tool pointer
                    SpellObject *obj = (SpellObject*)btns->GetItemClientData(btns->GetItemById(btn_id));
                    if(obj)
                        spell_tool.Set(obj); // tool is object
                    else
                        spell_tool.Set(tid, iid); // tool is class
                }
            }
        }
    }
}
void MyFrame::OnToolPageClick(wxRibbonBarEvent& event)
{
    // no tool selection
    spell_tool.Set();

    if(!ribbonBar)
        return;

    // very schmutzig way to deselect all other tool buttons
    for(int tid = 0; tid < ribbonBar->GetPageCount(); tid++)
    {
        wxRibbonPage* page = ribbonBar->GetPage(tid);
        auto wlist = page->GetChildren();
        if(!wlist.size())
            continue;
        wxRibbonPanel* panel = (wxRibbonPanel*)wlist[0];
        auto clist = panel->GetChildren();
        if(!clist.size())
            continue;
        wxRibbonButtonBar* btns = (wxRibbonButtonBar*)clist[0];
        // for each button:
        for(int iid = 0; iid < btns->GetButtonCount(); iid++)
        {
            int btn_id = ID_TOOL_BASE + tid*ID_TOOL_CLASS_STEP + iid;
            btns->ToggleButton(btn_id,false);
        }
    }
}

// fill toolset ribbon
void MyFrame::LoadToolsetRibbon(Terrain *terr)
{    
    // clear old ribbon
    if (ribbonBar)
        ribbonBar->Destroy();
    ribbonBar = new wxRibbonBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxRIBBON_BAR_DEFAULT_STYLE);
    ribbonBar->SetArtProvider(new wxRibbonDefaultArtProvider);
    
    if (!terr && spell_map)
        terr = spell_map->terrain;
    if (terr)
    {
        // for each toolset:
        for (int tool_id = 0; tool_id < terr->GetToolsCount(); tool_id++)
        {
            string toolset_name = terr->GetToolSetName(tool_id);
            string toolset_title = terr->GetToolSetTitle(tool_id);

            // for each tool in toolset:
            wxRibbonPage* ribPage = new wxRibbonPage(ribbonBar, wxID_ANY, toolset_name, wxNullBitmap, 0);
            wxRibbonPanel* ribPanel = new wxRibbonPanel(ribPage, wxID_ANY, toolset_title, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE | wxRIBBON_PANEL_NO_AUTO_MINIMISE | wxRIBBON_PANEL_FLEXIBLE);
            wxRibbonButtonBar* ribBtns = new wxRibbonButtonBar(ribPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
            
            // obtain all tools (glyphs) parameters
            vector<std::string> titles;
            vector<int> index;
            vector<std::tuple<int, int>> size;
            vector<SpellObject*> objects;
            int item_id = 0;
            for (; item_id < terr->GetToolSetItemsCount(tool_id); item_id++)
            {
                titles.push_back(terr->GetToolSetItem(tool_id, item_id));
                index.push_back(ID_TOOL_BASE + tool_id * ID_TOOL_CLASS_STEP + item_id);
                size.push_back(terr->GetToolSetItemImageSize(tool_id, item_id));
                objects.push_back(NULL);
            }
            for (auto const& obj : terr->GetObjects())
            {
                if (obj->GetToolClass() != tool_id + 1)
                    continue;
                if (obj->GetToolClassGroup() != 0)
                    continue;
                titles.push_back(obj->GetDescription());
                index.push_back(ID_TOOL_BASE + tool_id * ID_TOOL_CLASS_STEP + item_id);
                size.push_back(obj->GetGlyphSize());
                objects.push_back(obj);
                item_id++;
            }
            // get mean aspect ratio
            double aspect = 0;
            int x_max = 0;
            int y_max = 0;
            for (int k = 0; k < size.size(); k++)
            {
                auto [x, y] = size[k];
                if (!objects[k])
                {
                    x_max = max(x_max, x);
                    y_max = max(y_max, y);
                }
                aspect += (double)x / (double)y;
            }
            aspect *= (1.0 / size.size());
            
            // glyph size
            int x_size = -1;
            int y_size = -1;
            if (terr->GetToolSetGlyphScalingMode(tool_id) == SpellToolsGroup::SCALE_MEAN)
            {
                auto [x,y] = terr->GetToolSetGlyphScaling(tool_id);
                x_size = x;
                y_size = (int)((double)x_size / aspect);
            }
            else
            {
                x_size = x_max;
                y_size = y_max;
            }
            // build buttons:
            for (int k = 0; k < size.size(); k++)
            {
                // render glyph
                wxBitmap *bmp=NULL;                
                if (objects[k])
                    bmp = objects[k]->RenderPreview(1.30, x_size, y_size);
                else
                    bmp = terr->RenderToolSetItemImage(tool_id, k, 1.30, x_size, y_size);
                // make button
                auto btn = ribBtns->AddButton(index[k], titles[k], *bmp, wxEmptyString, wxRIBBON_BUTTON_TOGGLE);
                // include object pointer if it's object
                ribBtns->SetItemClientData(btn,objects[k]);
                delete bmp;
            }

            ribBtns->Realize();
        }
    }

    // update ribbon with new stuff
    ribbonBar->Realize();
    sizer->Insert(0,ribbonBar, 0, wxALL | wxEXPAND, 2);
    sizer->Layout();
}



//--------------------------------------------------------------------------------------------------------------------
// Set Gamma Dialog
//--------------------------------------------------------------------------------------------------------------------
FormGamma::FormGamma(wxFrame* parent,SpellMap* map) :wxDialog(parent,wxID_ANY,"Gamma correction",wxDefaultPosition,wxSize(400,50),wxDEFAULT_FRAME_STYLE|wxSTAY_ON_TOP)
{
    // store local reference to initial map and data
    spell_map = map;

    // make slider
    slider = new wxSlider(this,wxID_ANY,1300,500,2000);
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(slider,1,wxEXPAND|wxALL);
    this->SetSizer(sizer);
    this->SetAutoLayout(true); 
    
    Bind(wxEVT_COMMAND_SLIDER_UPDATED,&FormGamma::OnChangeGamma, this);
}
void FormGamma::OnChangeGamma(wxCommandEvent& event)
{
    double gamma = 0.001*(double)slider->GetValue();
    SetTitle(wxString::Format(wxT("Gamma correction = %#0.2f"),gamma));
    spell_map->SetGamma(gamma);
}
