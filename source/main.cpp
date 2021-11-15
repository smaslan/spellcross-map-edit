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

#include <filesystem>
#include <codecvt>

#include "main.h"
#include "simpleini.h"
#include "spellcross.h"
#include "map.h"



std::wstring char2wstring(const char* str)
{
    //setup converter
    typedef std::codecvt_utf8<wchar_t> convert_type;
    std::wstring_convert<convert_type,wchar_t> converter;
    return converter.from_bytes(str);
}

// convert wstring to UTF-8 string
std::string wstring2string(const std::wstring& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
}





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


    // view scroller
    scroll.Reset();
    
    // --- run main form    
    // main window frame
    MyFrame* frame = new MyFrame(&spell_map, spell_data);
    // main sizer 
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    // render canvas
    mapCanvas* canvas = new mapCanvas(frame, spell_map, &scroll);
    sizer->Add(canvas,1,wxEXPAND|wxALL,1);
    // show main frame
    frame->SetSizer(sizer);
    frame->SetAutoLayout(true);
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


mapCanvas::mapCanvas(wxFrame* parent,SpellMap* spell_map,TScroll *scroll) :wxPanel(parent)
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
}


MyFrame::MyFrame(SpellMap** map, SpellData* spelldata):wxFrame(NULL, wxID_ANY, "Spellcross Map Editor", wxDefaultPosition, wxSize(800,600))
{
    // store local reference to initial map and data
    spell_map = map;
    spell_data = spelldata;
    
    // File menu
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_OpenMap, "&Open Map\tCtrl-O", "Open new Spellcross map file.");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    
    // View menu
    wxMenu* menuView = new wxMenu;
    menuView->Append(ID_ViewTer,"Layer 1: Terrain\tF1","",wxITEM_CHECK);
    menuView->Append(ID_ViewObj,"Layer 2: Objects\tF2","",wxITEM_CHECK);
    menuView->Append(ID_ViewAnm,"Layer 3: Tile anmiations\tF3","",wxITEM_CHECK);
    menuView->Append(ID_ViewPnm,"Layer 4: Sprite animations\tF4","",wxITEM_CHECK);
    menuView->Append(ID_ViewUnt,"Layer 5: Units\tF5","",wxITEM_CHECK);
    menuView->Append(ID_ViewStTa,"Layer 6: Start/Target\tF6","",wxITEM_CHECK);
    menuView->FindItem(ID_ViewTer)->Check(true);
    menuView->FindItem(ID_ViewObj)->Check(true);
    menuView->FindItem(ID_ViewAnm)->Check(true);
    menuView->FindItem(ID_ViewPnm)->Check(true);
    menuView->FindItem(ID_ViewUnt)->Check(true);
    menuView->FindItem(ID_ViewStTa)->Check(true);
    
    // Help menu
    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    
    // Main menu
    wxMenuBar* menuBar = new wxMenuBar;    
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuView, "&View");
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
    
    Bind(wxEVT_MENU, &MyFrame::OnOpenMap, this, ID_OpenMap);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);

    Bind(wxEVT_MENU,&MyFrame::OnViewLayer,this,ID_ViewTer);
    Bind(wxEVT_MENU,&MyFrame::OnViewLayer,this,ID_ViewObj);
    Bind(wxEVT_MENU,&MyFrame::OnViewLayer,this,ID_ViewAnm);
    Bind(wxEVT_MENU,&MyFrame::OnViewLayer,this,ID_ViewPnm);
    Bind(wxEVT_MENU,&MyFrame::OnViewLayer,this,ID_ViewUnt);
    Bind(wxEVT_MENU,&MyFrame::OnViewLayer,this,ID_ViewStTa);
    
    //Bind(wxEVT_PAINT, &MyFrame::OnPaint, this);
    //Bind(wxEVT_SIZING, &MyFrame::OnResize,this);
    //Bind(wxEVT_PAINT,&MyFrame::OnPaint,this);

    /*Bind(wxEVT_RIGHT_DOWN,&MyFrame::OnMouseDown,this);
    Bind(wxEVT_RIGHT_UP,&MyFrame::OnMouseUp,this);
    Bind(wxEVT_MOTION,&MyFrame::OnMouseMove,this);
    Bind(wxEVT_LEAVE_WINDOW,&MyFrame::OnMouseLeave,this);*/
    
}
void MyFrame::OnTimer(wxTimerEvent& event)
{
    if((*spell_map)->Tick())
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
    (*spell_map)->SetRender(wL1,wL2,wL3,wL4,wSS,wL5);
    Refresh();
}
void MyFrame::OnOpenMap(wxCommandEvent& event)
{
    std::filesystem::path last_path = (*spell_map)->GetTopPath();
    wstring dir = last_path.parent_path();
    wstring name = last_path.filename();
    
    wxFileDialog openFileDialog(this,_("Open Spellcross Map File"),dir,name,"Map script file (*.def)|*.def|Map data file (*.dta)|*.dta",
        wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if(openFileDialog.ShowModal() == wxID_CANCEL)
        return;
    wstring path = wstring(openFileDialog.GetPath().ToStdWstring());

    // loose old map
    delete (*spell_map);
    (*spell_map) = new SpellMap();
    // load new one
    (*spell_map)->Load(path, spell_data);
    // reset layers visibility
    (*spell_map)->SetGamma(1.30);
    OnViewLayer(event);
    // reset scroller
    scroll.Reset();
    
}


// --- scrolling control ---
void mapCanvas::OnMouseDown(wxMouseEvent& event)
{    
    scroll->xref = event.GetX();
    scroll->yref = event.GetY();
    scroll->state = 1;
    scroll->modified = 1;
}
void mapCanvas::OnMouseLeave(wxMouseEvent& event)
{
    scroll->state = 0;
    scroll->modified = 1;
}
void mapCanvas::OnMouseMove(wxMouseEvent& event)
{
    if(scroll->state)
    {
        scroll->dx -= (event.GetX() - scroll->xref);
        scroll->dy -= (event.GetY() - scroll->yref);
    }
    scroll->xref = event.GetX();
    scroll->yref = event.GetY();
    scroll->modified = 1;
    
    // update map selection
    //SpellMap* map = (*spell_map);
    MapXY mxy = spell_map->GetSelection(m_buffer,scroll);
    parent->SetStatusText(wxString::Format(wxT("x=%d"),mxy.x),0);
    parent->SetStatusText(wxString::Format(wxT("y=%d"),mxy.y),1);
    parent->SetStatusText(wxString::Format(wxT("xy=%d"),spell_map->ConvXY(mxy)),2);
    parent->SetStatusText(wxString::Format(wxT("L1: %s"),spell_map->GetL1tileName(m_buffer,scroll)),3);
    parent->SetStatusText(wxString::Format(wxT("L2: %s"),spell_map->GetL2tileName(m_buffer,scroll)),4);
    
    Refresh();
}
void mapCanvas::OnMouseUp(wxMouseEvent& event)
{
    scroll->state = 0;
    scroll->modified = 1;
}
