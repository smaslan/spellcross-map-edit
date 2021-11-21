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

#include "form_sprite_view.h"





wxIMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
    // load config.ini
    ini.SetUnicode();
    ini.LoadFile("config.ini");

    // spellcross data root path
    wstring spelldata_path = char2wstring(ini.GetValue("SPELCROS","spell_path",""));
    // try load spellcross data
    spell_data = new SpellData(spelldata_path);

    // special data folder
    wstring spec_folder = char2wstring(ini.GetValue("DATA","spec_tiles_path",""));
    // load special sprites
    spell_data->LoadSpecialLand(spec_folder);

    // load some map
    wstring map_path = char2wstring(ini.GetValue("STATE","last_map",""));
    spell_map = new SpellMap();
    spell_map->Load(map_path,spell_data);
    spell_map->SetGamma(1.3);
        
    
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


MyFrame::MyFrame(SpellMap* map, SpellData* spelldata):wxFrame(NULL, wxID_ANY, "Spellcross Map Editor", wxDefaultPosition, wxSize(1024,768))
{
    // store local reference to initial map and data
    spell_map = map;
    spell_data = spelldata;

    // view scroller
    scroll.Reset();
        
    // File menu
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_OpenMap, "&Open Map\tCtrl-O", "Open new Spellcross map file.");
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
    
    // tools
    wxMenu* menuTools = new wxMenu;
    menuTools->Append(ID_ViewSprites,"Sprites viewer","",wxITEM_NORMAL);
    menuTools->Append(ID_SetGamma,"","",wxITEM_SEPARATOR);
    menuTools->Append(ID_UpdateSprContext, "Update tile context","",wxITEM_NORMAL);
    
        
    // Help menu
    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    
    // Main menu
    wxMenuBar* menuBar = new wxMenuBar;    
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuEdit,"&Edit");
    menuBar->Append(menuView, "&View");
    menuBar->Append(menuTools,"&Tools");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);
    
    CreateStatusBar(6);
    const int ss_w[] = {50,50,70,100,100,100};
    SetStatusWidths(6,ss_w);
    SetStatusText("");
      

    this->SetDoubleBuffered(1);

    m_timer.SetOwner(this);
    this->Connect(wxEVT_TIMER,wxTimerEventHandler(MyFrame::OnTimer),NULL,this);
    m_timer.Start(100);


    // main sizer 
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    // make and attach render canvas
    mapCanvas* canvas = new mapCanvas(this,spell_map,&scroll);
    sizer->Add(canvas,1,wxEXPAND|wxALL,1);
    this->SetSizer(sizer);
    this->SetAutoLayout(true);

    
    Bind(wxEVT_MENU, &MyFrame::OnOpenMap, this, ID_OpenMap);
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
    Bind(wxEVT_MENU,&MyFrame::OnUpdateTileContext,this,ID_UpdateSprContext);

    Bind(wxEVT_MENU,&MyFrame::OnSelectAll,this,ID_SelectAll);
    Bind(wxEVT_MENU,&MyFrame::OnDeselectAll,this,ID_DeselectAll);
    
   
    
}


mapCanvas::mapCanvas(wxFrame* parent,SpellMap* spell_map,TScroll* scroll) :wxPanel(parent)
{
    this->spell_map = spell_map;
    this->scroll = scroll;
    this->parent = parent;

    Bind(wxEVT_PAINT,&mapCanvas::OnPaint,this);
    //Bind(wxEVT_SIZING,&myCanvas::OnResize,this);

    Bind(wxEVT_RIGHT_DOWN,&mapCanvas::OnMouseDown,this);
    Bind(wxEVT_RIGHT_UP,&mapCanvas::OnMouseUp,this);
    Bind(wxEVT_MOTION,&mapCanvas::OnMouseMove,this);
    Bind(wxEVT_LEAVE_WINDOW,&mapCanvas::OnMouseLeave,this);
    Bind(wxEVT_MOUSEWHEEL,&mapCanvas::OnMouseWheel,this);    
    Bind(wxEVT_KEY_DOWN,&mapCanvas::OnKeyDown,this);
}



void MyFrame::OnTimer(wxTimerEvent& event)
{
    if(spell_map->Tick())
    {
        Refresh();
    }
}
void MyFrame::OnResize(wxSizeEvent& event)
{
    Refresh();
}

void mapCanvas::OnPaint(wxPaintEvent& event)
{
    int x_size = GetClientSize().x;
    int y_size = GetClientSize().y;
    if (!m_buffer.IsOk() || m_buffer.GetSize() != GetClientSize())
    {
        m_buffer = wxBitmap(x_size, y_size, 24);
    }

    wxPaintDC pdc(this);

    //wxStopWatch sw;
    spell_map->Render(m_buffer, scroll);
    pdc.DrawBitmap(m_buffer,wxPoint(0,0));

    event.Skip();

    

    /*static int count = 0;
    count++;
    SetStatusText(wxString::Format(wxT("%d"),count),5);*/
    //SetStatusText(wxString::Format(wxT("%ld"),sw.Time()),5);
}
void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}
void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
        "About Hello World", wxOK | wxICON_INFORMATION);
}
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
void MyFrame::OnOpenMap(wxCommandEvent& event)
{
    // split path to folder and file
    std::filesystem::path last_path = spell_map->GetTopPath();
    wstring dir = last_path.parent_path();
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
    FormSprite *form = new FormSprite(this, spell_data);
    form->Show();
}
// update tiles context from map selection
void MyFrame::OnUpdateTileContext(wxCommandEvent& event)
{
    spell_map->BuildSpriteContext();
}



// --- scrolling control ---
void mapCanvas::OnMouseDown(wxMouseEvent& event)
{    
    scroll->SetRef(event.GetX(), event.GetY());
}
void mapCanvas::OnMouseLeave(wxMouseEvent& event)
{
    scroll->Idle();
}
void mapCanvas::OnMouseUp(wxMouseEvent& event)
{
    scroll->Idle();
}
void mapCanvas::OnMouseMove(wxMouseEvent& event)
{
    scroll->Move(event.GetX(), event.GetY());
    
    // update map selection
    //SpellMap* map = (*spell_map);
    MapXY mxy = spell_map->GetSelection(m_buffer,scroll);
    parent->SetStatusText(wxString::Format(wxT("x=%d"),mxy.x),0);
    parent->SetStatusText(wxString::Format(wxT("y=%d"),mxy.y),1);
    parent->SetStatusText(wxString::Format(wxT("xy=%d"),spell_map->ConvXY(mxy)),2);
    parent->SetStatusText(wxString::Format(wxT("L1: %s"),spell_map->GetL1tileName(m_buffer,scroll)),3);
    parent->SetStatusText(wxString::Format(wxT("L2: %s"),spell_map->GetL2tileName(m_buffer,scroll)),4);
    //int height, flags, code;
    auto [flags,height,code] = spell_map->GetTileFlags(m_buffer,scroll);
    parent->SetStatusText(wxString::Format(wxT("(0x%02X)"),code),5);
    
    Refresh();
}
void mapCanvas::OnMouseWheel(wxMouseEvent& event)
{
    scroll->ResizeSelection(event.GetWheelRotation()/event.GetWheelDelta());
    Refresh();
}
void mapCanvas::OnKeyDown(wxKeyEvent& event)
{
    if(event.ControlDown())
    {
        int key = event.GetKeyCode();

        // --- edit terrain elevation:
        int step = 0;
        if(key == WXK_PAGEUP)
            step++;
        else if(key == WXK_PAGEDOWN)
            step--;
        if(step != 0)
        {
            spell_map->EditElev(m_buffer, scroll, step);
            Refresh();
        }

        // --- edit selection:
        if(key == '=' || key == '+')
        {
            // add selection
            auto list = spell_map->GetSelections(m_buffer, scroll);
            spell_map->SelectTiles(list, SpellMap::SELECT_ADD);
        }
        else if(key == '-')
        {
            // clear selection
            auto list = spell_map->GetSelections(m_buffer,scroll);
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
