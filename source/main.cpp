//=============================================================================
// Spellcross Map Editor
// ----------------------------------------------------------------------------
// Top level functions, wxWidgets GUI.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021-2026, Stanislav Maslan, s.maslan@seznam.cz
// url: https://github.com/smaslan/spellcross-map-edit
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
// 
// For compilers that support precompilation, includes "wx/wx.h".
//#define wxMSVC_VERSION_ABI_COMPAT
#include <wx/wxprec.h>
//#include <wx/msw/wx.rc>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/dcgraph.h>
#include <wx/dcbuffer.h>
#include <wx/rawbmp.h>
#include <wx/timer.h>
#include <wx/filedlg.h>
#include <wx/slider.h>
#include <wx/stdpaths.h>

#include <filesystem>
#include <codecvt>
#include <tuple>
#include <string>
#include <chrono>
#include <future>

#include "resource.h"
#include "main.h"
#include "other.h"
#include "simpleini.h"
#include "spellcross.h"
#include "map.h"

// load history of paths from INI
std::filesystem::path iniLoadPathHist(CSimpleIniA &ini, std::string section, std::string key, std::vector<std::filesystem::path> &list, int count=10)
{
    std::filesystem::path main_path;
    list.clear();
    for(int k = 0; k < count; k++)
    {
        std::string hkey = key;
        if(k)
            hkey = string_format("%s<%d>",key.c_str(),k);
        std::filesystem::path path = char2wstring(ini.GetValue(section.c_str(), hkey.c_str(), ""));
        if(!k)
            main_path = path;
        if(path.empty() || !std::filesystem::exists(path))
            continue;
        list.push_back(path);
    }
    return(main_path);
}

// save history of paths to INI
void iniSavePathHist(CSimpleIniA& ini,std::string section,std::string key,std::filesystem::path path, std::vector<std::filesystem::path>& list,std::string comment="",int count=10)
{
    std::vector<std::filesystem::path> paths;
    for(auto &item: list)
        if(item.empty() || std::filesystem::exists(item))
            paths.push_back(item);
    // prepend no-path if not in history
    if(!paths.empty() && !paths[0].empty() && path.empty())
        paths.insert(paths.begin(), path);

    
    for(int k = 0; k < count; k++)
    {
        std::string hkey = key;
        if(k)
            hkey = string_format("%s<%d>",key.c_str(),k);
        std::filesystem::path path = "";
        if(k < paths.size())
            path = paths[k];        
        const char *com = NULL;
        if(!k)
            com = comment.c_str();
        ini.SetValue(section.c_str(), hkey.c_str(),wstring2string(path.wstring()).c_str(),com);
    }    
}

// spellcross data loader ui
int MainFrame::LoadSpellData(wxWindow *parent, SpellConfig &config, SpellData *&spell_data)
{
    while(true)
    {
        FormLoader form_loader(parent,spell_data,config,ID_LOADER_WIN);
        if(form_loader.ShowModal())
        {
            auto error = form_loader.GetExitMessage();

            // ###note: This is absolutely needed because RtAudio seems to messup with threading concurency model for main UI thread,
            // which results in file dialog calls (and god knows what else) to hangup? Maybe it will be fixed in new version of wxWidgets? 
            // Anyway, here we reset it back to original state before RtAudio channels were changed in spell_data destructor/constructor above.
            // It took me whole day to figure this out! :-)
            #ifdef __WXMSW__
                ::CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
            #endif

            // try again with different paths?
            wxMessageDialog dial(parent,string_format("Loading Spellcross data failed! Loader reports:\n%s\n\nSome data files are missing? Modify search paths?",error.c_str()),"Loading Spellcross data",wxYES_NO | wxYES_DEFAULT |wxICON_ERROR);
            if(dial.ShowModal() != wxID_YES)
            {             
                // failed and no retry
                return(1);
            }

            // show config dialogue and try again
            FormConfig form_config(parent,ID_CONFIG_WIN,&config);
            form_config.ShowModal();
            continue;
        }
        return(0);
    }
}

wxIMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
    spell_data = NULL;
    spell_map = NULL;

    // for saving PNG file (among other stuff)
    wxInitAllImageHandlers();

    // load config.ini
    ini.SetUnicode();
    ini.LoadFile("config.ini");

    // spellcross data path
    m_config.spell_path = iniLoadPathHist(ini, "SPELCROS", "spell_path",m_config.spell_path_hist);

    // spellcross CD data path
    m_config.spell_cd_path = iniLoadPathHist(ini,"SPELCROS","spellcd_path",m_config.spell_cd_path_hist);
    
    // spellcross mod data path
    m_config.spell_mod_path = iniLoadPathHist(ini,"SPELCROS","mod_path",m_config.spell_mod_path_hist);

    // hide warnings when loading maps?
    m_config.hide_map_warnings = ini.GetBoolValue("STATE","hide_map_load_warnings",false);

    // additional program data folder
    m_config.spec_data_path = char2wstring(ini.GetValue("DATA","spec_data_path",""));
    
    // some additional metadata for units.fsu
    m_config.units_aux_data_path = char2wstring(ini.GetValue("DATA","units_aux_data_path",""));

    // load list of context paths for each terrain
    std::list<CSimpleIniA::Entry> ini_sec_list;
    ini.GetAllSections(ini_sec_list);
    for(auto item: ini_sec_list)
    {
        auto tok = regexp_get(item.pItem,"TERRAIN::([^s]+)");
        if(tok.size() != 1)
            continue;
        std::string terrain_section = "TERRAIN::" + tok[0];
        std::filesystem::path cont_path = char2wstring(ini.GetValue(terrain_section.c_str(), "context_path"));
        m_config.context_path.insert({tok[0], cont_path});
    }

    // --- try load Spellcross data
    spell_data = new SpellData();
    if(MainFrame::LoadSpellData(NULL, m_config, spell_data))
    {        
        OnExit();
        return(false);
    }


    // last export path
    spell_data->export_path = char2wstring(ini.GetValue("STATE","export_path",""));

    // --- load some map
    wstring map_path = char2wstring(ini.GetValue("STATE","last_map",""));
    spell_map = new SpellMap();
    spell_map->hide_map_load_warnings = m_config.hide_map_warnings;
    if(!map_path.empty())
    {
        if(spell_map->Load(map_path,spell_data))
            wxMessageBox(string_format("Loading Spellcross map file failed with error:\n%s",spell_map->GetLastError().c_str()),"Error",wxICON_ERROR);
        else if(!m_config.hide_map_warnings && !spell_map->GetLastError().empty())
            wxMessageBox(string_format("Loading Spellcross map file ended with warning(s):\n%s",spell_map->GetLastError().c_str()),"Warning",wxICON_WARNING);
    }
    spell_map->SetGamma(ini.GetDoubleValue("STATE","gamme",1.3));

    // sound effects/midi volumes
    spell_data->sounds->channels->SetVolume(0.01*ini.GetLongValue("STATE","sound_volume",50));
    spell_data->midi->SetVolume(0.01*ini.GetLongValue("STATE","music_volume",100));

    // play default MIDI
    string midi_name = ini.GetValue("STATE","default_midi","");
    spell_data->midi->Play(midi_name);

    // default window size
    int win_x_size = ini.GetLongValue("STATE","win_x_size",1600);
    int win_y_size = ini.GetLongValue("STATE","win_y_size",1000);
    bool win_maximize = ini.GetBoolValue("STATE","win_maximize",false);

    

    

    // limit to screen size
    int disp_x_size;
    int disp_y_size;
    wxDisplaySize(&disp_x_size,&disp_y_size);
    win_x_size = min(win_x_size,disp_x_size);
    win_y_size = min(win_y_size,disp_y_size);
                
    // --- run main form    
    // main window frame
    MainFrame* frame = new MainFrame(&m_config, spell_map, spell_data);
    frame->SetSize(win_x_size,win_y_size);
    if(win_maximize)
        frame->Maximize();

    // set icon
    wxIcon appIcon;
    appIcon.LoadFile("IDI_ICON2",wxBITMAP_TYPE_ICO_RESOURCE);
    if(appIcon.IsOk())
        frame->SetIcon(appIcon);

    frame->Center();
    // show main frame
    frame->Show(true);
    return(true);
}
int MyApp::OnExit()
{
    if(spell_map)
    {        
        // store last path
        if(!spell_map->GetTopPath().empty())
            ini.SetValue("STATE","last_map",wstring2string(spell_map->GetTopPath()).c_str());
        
        // map warnings
        ini.SetBoolValue("STATE","hide_map_load_warnings",m_config.hide_map_warnings);

        // gamma correction
        ini.SetDoubleValue("STATE","gamme",spell_map->GetGamma());
    }

    // last export path
    if(spell_data)
        ini.SetValue("STATE","export_path",wstring2string(spell_data->export_path).c_str());
        
    // store sound/midi volumes
    if(spell_data->sounds)
        ini.SetLongValue("STATE", "sound_volume", 100.0*spell_data->sounds->channels->GetVolume());
    if(spell_data->midi)
        ini.SetLongValue("STATE", "music_volume", 100.0*spell_data->midi->GetVolume());            

    // save data paths
    iniSavePathHist(ini,"SPELCROS","spell_path",m_config.spell_path, m_config.spell_path_hist,"; spellcross installation data folder");
    iniSavePathHist(ini,"SPELCROS","spellcd_path",m_config.spell_cd_path, m_config.spell_cd_path_hist,"; spellcross CD copy data path");
    iniSavePathHist(ini,"SPELCROS","mod_path",m_config.spell_mod_path, m_config.spell_mod_path_hist,"; spellcross mod path (optional)");

    // save INI
    ini.SaveFile("config.ini");

    // loose map
    if(spell_map)
        delete spell_map;

    // loose spell data
    if(spell_data)
        delete spell_data;

    return(0);
}

// Main panel init
MainFrame::MainFrame(SpellConfig* config, SpellMap *&map, SpellData *&spelldata):
    wxFrame(NULL, wxID_ANY, "Spellcross Map Editor", wxDefaultPosition, wxSize(1600,1000)),
    spell_data(spelldata),
    spell_map(map)
{
    // store local reference to initial map and data
    m_spell_config = config;

    // subforms
    form_gamma = NULL;
    form_sprites = NULL;
    form_anms = NULL;
    form_objects = NULL;
    form_pal = NULL;
    form_gres = NULL;
    form_units = NULL;
    form_events = NULL;
    form_videos = NULL;
    form_unit_opts = NULL;
    form_message = NULL;
    form_video_box = NULL;
    form_map_options = NULL;
    form_midi = NULL;
    form_minimap = NULL;
    form_units_list = NULL;
    form_sounds = NULL;
        
    // File menu
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_OpenMap, "&Open Map\tCtrl-O", "Open new Spellcross map file.");
    menuFile->Append(ID_SaveMap,"&Save Map\tCtrl-S","Save Spellcross map file(s).");
    menuFile->Append(ID_SaveMapAs,"&Save Map As","Save Spellcross map file(s).");
    menuFile->Append(ID_SaveDTA,"&Save DTA map file","Save Spellcross map DTA file.");
    menuFile->Append(ID_SaveDEF,"&Save DEF map file","Save Spellcross map DEF file.");
    menuFile->Append(ID_NewMap,"&Ceate new Map\tCtrl-N","Create new map.");
    menuFile->AppendSeparator();
    menuFile->Append(ID_Config,"Configuration","Open new Spellcross Map Editor configuration.");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    AssignSVGresourceToMenu(menuFile,ID_OpenMap,"IDR_OPEN3");
    AssignSVGresourceToMenu(menuFile,ID_SaveMap,"IDR_SAVE");
    AssignSVGresourceToMenu(menuFile,ID_SaveMapAs,"IDR_SAVE");
    AssignSVGresourceToMenu(menuFile,ID_SaveDTA,"IDR_SAVE");
    AssignSVGresourceToMenu(menuFile,ID_SaveDEF,"IDR_SAVE");
    AssignSVGresourceToMenu(menuFile,ID_NewMap,"IDR_NEW");
    AssignSVGresourceToMenu(menuFile,ID_Config,"IDR_SETUP");
    AssignSVGresourceToMenu(menuFile,wxID_EXIT,"IDR_CLOSE");
    

    // Game menu
    wxMenu* menuGame = new wxMenu;
    menuGame->Append(ID_mmGameMode,"Game mode\tCtrl-G","Switch game mode",wxITEM_CHECK);
    menuGame->AppendSeparator();
    menuGame->Append(ID_mmResetViewMap,"Reset view map","");
    menuGame->Append(ID_mmUnitViewMode,"View unit move/attack range\tSpace","");
    
    
    // View menu
    menuView = new wxMenu;
    menuView->Append(ID_ViewTer,"Layer 1: Terrain\tF1","",wxITEM_CHECK);
    menuView->FindItem(ID_ViewTer)->Check(true);
    menuView->Append(ID_ViewObj,"Layer 2: Objects\tF2","",wxITEM_CHECK);
    menuView->FindItem(ID_ViewObj)->Check(true);
    menuView->Append(ID_ViewAnm,"Layer 3: Tile animations\tF3","",wxITEM_CHECK);
    menuView->FindItem(ID_ViewAnm)->Check(true);
    menuView->Append(ID_ViewPnm,"Layer 4: Sprite animations\tF4","",wxITEM_CHECK);
    menuView->FindItem(ID_ViewPnm)->Check(true);    
    menuView->Append(ID_ViewStartCounterAttack,"Layer 5+6: Counter attack positions\tF6","",wxITEM_CHECK);
    menuView->FindItem(ID_ViewStartCounterAttack)->Check(false);
    menuView->Append(ID_ViewSoundLoops,"Layer 7: Sound loops\tF7","",wxITEM_CHECK);
    menuView->FindItem(ID_ViewSoundLoops)->Check(false);
    menuView->Append(ID_ViewSounds,"Layer 8: Sounds\tF8","",wxITEM_CHECK);
    menuView->FindItem(ID_ViewSounds)->Check(false);
    menuView->Append(ID_ViewUnt,"Units","",wxITEM_CHECK);
    menuView->FindItem(ID_ViewUnt)->Check(true);
    menuView->Append(ID_ViewStTa,"Start/Escape/Target\tF5","",wxITEM_CHECK);
    menuView->FindItem(ID_ViewStTa)->Check(true);
    menuView->Append(ID_ViewEvents,"Show events\tF9","",wxITEM_CHECK);
    menuView->FindItem(ID_ViewEvents)->Check(false);    
    menuView->Append(ID_HighlighObj,"Highlight objects\tF10","",wxITEM_CHECK);
    menuView->FindItem(ID_HighlighObj)->Check(false);    
    menuView->Append(ID_ShowDebug,"Show debug\tF11","",wxITEM_CHECK);
    menuView->FindItem(ID_ShowDebug)->Check(false);
    menuView->Append(ID_ViewHUD,"Show mission HUD panel\tCtrl+H","",wxITEM_CHECK);
    menuView->FindItem(ID_ViewHUD)->Check(spell_map->GetHUDstate());
    menuView->Append(ID_UnitViewDbg,"Enable unit view debug mode\tCtrl+D","",wxITEM_CHECK);    
    menuView->Append(wxID_ANY,"","",wxITEM_SEPARATOR);
    menuView->Append(ID_SetGamma,"Set gamma","",wxITEM_NORMAL);

    // Layer selection submenu for copy/paste editor
    wxMenu* menuLayer = new wxMenu;
    menuLayer->Append(ID_SelectLay1,"Layer 1 - Terrain\tCtrl+F1","",wxITEM_CHECK);
    menuLayer->FindItem(ID_SelectLay1)->Check(true);
    menuLayer->Append(ID_SelectLay2,"Layer 2 - Objects\tCtrl+F2","",wxITEM_CHECK);
    menuLayer->FindItem(ID_SelectLay2)->Check(true);
    menuLayer->Append(ID_SelectLayANM,"Layer 3 - ANM animations\tCtrl+F3","",wxITEM_CHECK);
    menuLayer->FindItem(ID_SelectLayANM)->Check(true);
    menuLayer->Append(ID_SelectLayPNM,"Layer 4 - PNM animations\tCtrl+F4","",wxITEM_CHECK);
    menuLayer->FindItem(ID_SelectLayPNM)->Check(true);
    // edit menu
    menuEdit = new wxMenu;
    menuEdit->Append(ID_HistoryUndo,"Undo\tCtrl+Z","",wxITEM_NORMAL);
    menuEdit->Append(ID_HistoryRedo,"Redo\tCtrl+Y","",wxITEM_NORMAL);
    menuEdit->Append(wxID_ANY,"","",wxITEM_SEPARATOR);
    menuEdit->Append(ID_EditMissionParams,"Edit mission parameters","",wxITEM_NORMAL);
    menuEdit->Append(wxID_ANY,"","",wxITEM_SEPARATOR);
    menuEdit->AppendSubMenu(menuLayer,"Select layer(s)","");
    menuEdit->Append(ID_SelectAll,"Select all tiles\tCtrl+A","",wxITEM_NORMAL);
    menuEdit->Append(ID_DeselectAll,"Deselect all tiles\tCtrl+Shift+A","",wxITEM_NORMAL);
    menuEdit->Append(ID_SelectDeselect,"Select/deselect tiles\tCtrl+Insert","",wxITEM_NORMAL);
    menuEdit->Append(wxID_ANY,"","",wxITEM_SEPARATOR);
    menuEdit->Append(ID_CopyBuf,"Copy selection to buffer\tCtrl+C","",wxITEM_NORMAL);
    menuEdit->Append(ID_CutBuf,"Cut selection to buffer\tCtrl+X","",wxITEM_NORMAL);
    menuEdit->Append(ID_PasteBuf,"Paste from buffer\tCtrl+V","",wxITEM_NORMAL);
    menuEdit->Append(ID_ClearBuf,"Clear buffer\tESC","",wxITEM_NORMAL);
    menuEdit->Append(wxID_ANY,"","",wxITEM_SEPARATOR);
    menuEdit->Append(ID_Dummy,"Paste from tool\tLeft Click","",wxITEM_NORMAL);
    menuEdit->Append(ID_Dummy,"Paste from tool and randomize\tCtrl+Left Click","",wxITEM_NORMAL);
    menuEdit->Append(ID_Dummy,"Paste from tool with auto map\tShift+Left Click","",wxITEM_NORMAL);
    menuEdit->Append(ID_Dummy,"Change tool size\tWheel","",wxITEM_NORMAL);
    menuEdit->Append(ID_Dummy,"Cycle tool items\tCtrl+Wheel","",wxITEM_NORMAL);
    menuEdit->Append(wxID_ANY,"","",wxITEM_SEPARATOR);
    menuEdit->Append(ID_InvalidateSel,"Invalidate selection\tCtrl+I","",wxITEM_NORMAL);
    menuEdit->Append(ID_DeleteSel,"Delete stuff\tShift+Delete","",wxITEM_NORMAL);
    menuEdit->Append(wxID_ANY,"","",wxITEM_SEPARATOR);
    menuEdit->Append(ID_ElevUp,"Elevate terrain\tCtrl+PageUp","",wxITEM_NORMAL);
    menuEdit->Append(ID_ElevDown,"Lower terrain\tCtrl+PageDown","",wxITEM_NORMAL);
    menuEdit->Append(wxID_ANY,"","",wxITEM_SEPARATOR);
    menuEdit->Append(ID_CreateNewObject,"Create new object\tCtrl+Shift+O","",wxITEM_NORMAL);
    menuEdit->Append(wxID_ANY,"","",wxITEM_SEPARATOR);
    menuEdit->Append(ID_AddUnit,"Add unit\tCtrl+Shift+U","",wxITEM_NORMAL);
    AssignSVGresourceToMenu(menuEdit,ID_HistoryUndo,"IDR_UNDO");
    AssignSVGresourceToMenu(menuEdit,ID_HistoryRedo,"IDR_REDO");
    AssignSVGresourceToMenu(menuEdit,ID_EditMissionParams,"IDR_EDIT");
    AssignSVGresourceToMenu(menuEdit,ID_CopyBuf,"IDR_COPY");
    AssignSVGresourceToMenu(menuEdit,ID_CutBuf,"IDR_CUT");
    AssignSVGresourceToMenu(menuEdit,ID_PasteBuf,"IDR_PASTE");
    AssignSVGresourceToMenu(menuEdit,ID_ClearBuf,"IDR_CLEAR");    
    AssignSVGresourceToMenu(menuEdit,ID_InvalidateSel,"IDR_CLEAR");
    AssignSVGresourceToMenu(menuEdit,ID_DeleteSel,"IDR_CLEAR");
    AssignSVGresourceToMenu(menuEdit,ID_CreateNewObject,"IDR_NEW");
    AssignSVGresourceToMenu(menuEdit,ID_ElevUp,"IDR_UP");
    AssignSVGresourceToMenu(menuEdit,ID_ElevDown,"IDR_DOWN");


    
    // tools
    wxMenu* menuTools = new wxMenu;
    menuTools->Append(ID_ViewSprites,"Sprites viewer","",wxITEM_NORMAL);
    menuTools->Append(ID_ViewAnms,"Animations (ANM) viewer","",wxITEM_NORMAL);
    menuTools->Append(ID_ViewPnms,"Animations (PNM) viewer","",wxITEM_NORMAL);
    menuTools->Append(ID_SoundsViewer,"Sounds viewer","",wxITEM_NORMAL);
    menuTools->Append(ID_ViewPal,"Palette viewer","",wxITEM_NORMAL);
    menuTools->Append(ID_ViewGRes,"Graphics viewer","",wxITEM_NORMAL);
    menuTools->Append(ID_EncodeGRes,"Graphics endoder","",wxITEM_NORMAL);
    menuTools->Append(ID_TextEdit,"Text view/editor","",wxITEM_NORMAL);
    menuTools->Append(ID_TextEditRaw,"Raw text view/editor","",wxITEM_NORMAL);
    menuTools->Append(ID_EditUnit,"Units viewer/editor\tCtrl+U","",wxITEM_NORMAL);
    menuTools->Append(ID_EditEvent,"Event viewer/editor\tCtrl+E","",wxITEM_NORMAL);
    menuTools->Append(ID_ViewVideo,"Video viewer","",wxITEM_NORMAL);
    menuTools->Append(ID_ViewMIDI,"MIDI player","",wxITEM_NORMAL);
    menuTools->Append(wxID_ANY,"","",wxITEM_SEPARATOR);
    menuTools->Append(ID_ViewObjects,"Objects editor","",wxITEM_NORMAL);
    menuTools->Append(ID_EditTileFlags,"Edit tile flags\tCtrl+F","",wxITEM_NORMAL);
    menuTools->Append(wxID_ANY,"","",wxITEM_SEPARATOR);
    menuTools->Append(ID_ViewMiniMap,"View mini-map","",wxITEM_NORMAL);
    menuTools->Append(ID_ViewVoxZ,"View Z-map","",wxITEM_NORMAL);
    menuTools->Append(ID_ExportVoxZ,"Export Z-map","",wxITEM_NORMAL);
    menuTools->Append(ID_ExportMapImg,"Export map render","",wxITEM_NORMAL);
    menuTools->Append(ID_ExportMapsImg,"Export batch map renders","",wxITEM_NORMAL);
    menuTools->Append(wxID_ANY,"","",wxITEM_SEPARATOR);
    menuTools->Append(ID_UpdateSprContext, "Update tile context from this map","",wxITEM_NORMAL);
    menuTools->Append(ID_UpdateSprContextMaps,"Update tile context from ALL maps","",wxITEM_NORMAL);
    menuTools->Append(ID_GenDMAobjects,"Generate DMAx_xxx objects from this map","",wxITEM_NORMAL);
    menuTools->Append(ID_GenDMAobjectsMaps,"Generate DMAx_xxx objects from ALL maps","",wxITEM_NORMAL);
    AssignSVGresourceToMenu(menuTools,ID_ViewSprites,"IDR_LAY_SPRITE");
    AssignSVGresourceToMenu(menuTools,ID_ViewAnms,"IDR_LAY_ANM");
    AssignSVGresourceToMenu(menuTools,ID_ViewPnms,"IDR_LAY_PNM");
    AssignSVGresourceToMenu(menuTools,ID_SoundsViewer,"IDR_SPEAKER");
    AssignSVGresourceToMenu(menuTools,ID_ViewPal,"IDR_LAY_PAL");
    AssignSVGresourceToMenu(menuTools,ID_ViewGRes,"IDR_IMAGE_VIEW");
    AssignSVGresourceToMenu(menuTools,ID_EncodeGRes,"IDR_IMAGE_EDIT");
    AssignSVGresourceToMenu(menuTools,ID_TextEdit,"IDR_LAY_TEXT");
    AssignSVGresourceToMenu(menuTools,ID_TextEditRaw,"IDR_LAY_TEXT_RAW");
    AssignSVGresourceToMenu(menuTools,ID_EditUnit,"IDR_LAY_UNIT");
    AssignSVGresourceToMenu(menuTools,ID_EditEvent,"IDR_EVENT_TIME");
    AssignSVGresourceToMenu(menuTools,ID_ViewVideo,"IDR_VIDEO");
    AssignSVGresourceToMenu(menuTools,ID_ViewMIDI,"IDR_MUSIC");
    
        
    // Help menu
    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    AssignSVGresourceToMenu(menuHelp,wxID_ABOUT,"IDR_INFO");
    
    // Main menu
    wxMenuBar* menuBar = new wxMenuBar;    
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuGame, "&Game");
    menuBar->Append(menuEdit, "&Edit");
    menuBar->Append(menuView, "&View");
    menuBar->Append(menuTools,"&Tools");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);
    
    
    std::vector<int> sbar_w = {45,45,45,60,100,100,100,350,120,-1};
    CreateStatusBar(sbar_w.size());
    SetStatusWidths(sbar_w.size(),sbar_w.data());
    SetStatusText("");
    UpdateMapStatus();
      
    // tick timer
    m_timer.SetOwner(this);
    this->Connect(wxEVT_TIMER,wxTimerEventHandler(MainFrame::OnTimer),NULL,this);
    m_timer.Start(10);

    // main sizer 
    sizer = new wxBoxSizer(wxVERTICAL); 
    this->SetSizeHints(wxDefaultSize,wxDefaultSize);

    // toolset ribbon
    //ribbonBar = new wxRibbonBar(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxRIBBON_BAR_DEFAULT_STYLE);
    //ribbonBar->SetArtProvider(new wxRibbonDefaultArtProvider);
    ribbonBar = NULL;
    LoadToolsetRibbon();
    //sizer->Add(ribbonBar,0,wxALL|wxEXPAND,2);
    
    Bind(wxEVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED,&MainFrame::OnToolBtnDropClick,this);
    Bind(wxEVT_RIBBONBUTTONBAR_CLICKED,&MainFrame::OnToolBtnClick,this);
    Bind(wxEVT_RIBBONBAR_PAGE_CHANGED,&MainFrame::OnToolPageClick,this);

    // make and attach render canvas
    canvas = new wxPanel(this,ID_Canvas,wxDefaultPosition,wxDefaultSize,wxTAB_TRAVERSAL);
    sizer->Add(canvas,1,wxEXPAND|wxALL,1);
    canvas->SetBackgroundStyle(wxBG_STYLE_PAINT);
    canvas->SetDoubleBuffered(true);
    

    canvas->Bind(wxEVT_CLOSE_WINDOW,&MainFrame::OnClose,this);
    canvas->Bind(wxEVT_PAINT,&MainFrame::OnPaintCanvas,this);
    canvas->Bind(wxEVT_RIGHT_DOWN,&MainFrame::OnCanvasRMouse,this);
    canvas->Bind(wxEVT_RIGHT_UP,&MainFrame::OnCanvasRMouse,this);
    
    

    canvas->Bind(wxEVT_MOTION,&MainFrame::OnCanvasMouseMove,this);
    canvas->Bind(wxEVT_LEAVE_WINDOW,&MainFrame::OnCanvasMouseLeave,this);
    canvas->Bind(wxEVT_ENTER_WINDOW,&MainFrame::OnCanvasMouseEnter,this);
    canvas->Bind(wxEVT_MOUSEWHEEL,&MainFrame::OnCanvasMouseWheel,this);
    canvas->Bind(wxEVT_KEY_DOWN,&MainFrame::OnCanvasKeyDown,this);
    canvas->Bind(wxEVT_LEFT_DOWN,&MainFrame::OnCanvasLMouseDown,this);
    canvas->Bind(wxEVT_LEFT_UP,&MainFrame::OnCanvasLMouseUp,this);

    //canvas->Bind(wxEVT_LEFT_DCLICK,&MainFrame::OnCanvasLMouseDown,this);
    canvas->Bind(wxEVT_THREAD,&MainFrame::OnThreadCanvas,this);
    

    this->SetSizer(sizer);    
    this->SetAutoLayout(true);
    this->Layout();

    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);
    
    Bind(wxEVT_MENU,&MainFrame::OnOpenMap,this,ID_OpenMap);
    Bind(wxEVT_MENU,&MainFrame::OnSaveMap,this,ID_SaveMap);
    Bind(wxEVT_MENU,&MainFrame::OnSaveMap,this,ID_SaveMapAs);
    Bind(wxEVT_MENU,&MainFrame::OnSaveDTA,this,ID_SaveDTA);
    Bind(wxEVT_MENU,&MainFrame::OnSaveDEF,this,ID_SaveDEF);
    Bind(wxEVT_MENU,&MainFrame::OnNewMap,this,ID_NewMap);
    Bind(wxEVT_MENU,&MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU,&MainFrame::OnConfig,this,ID_Config);
    Bind(wxEVT_MENU,&MainFrame::OnExit, this, wxID_EXIT);

    Bind(wxEVT_MENU,&MainFrame::OnSwitchGameMode,this,ID_mmGameMode);
    Bind(wxEVT_MENU,&MainFrame::OnResetUnitView,this,ID_mmResetViewMap);
    Bind(wxEVT_MENU,&MainFrame::OnSelectUnitView,this,ID_mmUnitViewMode);

    Bind(wxEVT_MENU,&MainFrame::OnViewLayer,this,ID_ViewTer);
    Bind(wxEVT_MENU,&MainFrame::OnViewLayer,this,ID_ViewObj);
    Bind(wxEVT_MENU,&MainFrame::OnViewLayer,this,ID_ViewAnm);
    Bind(wxEVT_MENU,&MainFrame::OnViewLayer,this,ID_ViewPnm);
    Bind(wxEVT_MENU,&MainFrame::OnViewLayer,this,ID_ViewUnt);
    Bind(wxEVT_MENU,&MainFrame::OnViewLayer,this,ID_ViewStTa);
    Bind(wxEVT_MENU,&MainFrame::OnViewLayer,this,ID_ViewStartCounterAttack);
    Bind(wxEVT_MENU,&MainFrame::OnViewLayer,this,ID_ViewHUD);
    Bind(wxEVT_MENU,&MainFrame::OnViewLayer,this,ID_ViewSounds);
    Bind(wxEVT_MENU,&MainFrame::OnViewLayer,this,ID_ViewSoundLoops);
    Bind(wxEVT_MENU,&MainFrame::OnViewLayer,this,ID_ViewEvents);
    Bind(wxEVT_MENU,&MainFrame::OnViewLayer,this,ID_HighlighObj);
    Bind(wxEVT_MENU,&MainFrame::OnViewLayer,this,ID_ShowDebug);

    Bind(wxEVT_MENU,&MainFrame::OnSetGamma,this,ID_SetGamma);
    Bind(wxEVT_MENU,&MainFrame::OnViewSprites,this,ID_ViewSprites);
    Bind(wxEVT_MENU,&MainFrame::OnViewAnms,this,ID_ViewAnms);
    Bind(wxEVT_MENU,&MainFrame::OnViewPnms,this,ID_ViewPnms);
    Bind(wxEVT_MENU,&MainFrame::OnViewSounds,this,ID_SoundsViewer);
    Bind(wxEVT_MENU,&MainFrame::OnViewObjects,this,ID_ViewObjects);
    Bind(wxEVT_MENU,&MainFrame::OnViewPal,this,ID_ViewPal);
    Bind(wxEVT_MENU,&MainFrame::OnViewGrRes,this,ID_ViewGRes);
    Bind(wxEVT_MENU,&MainFrame::OnEncodeGrRes,this,ID_EncodeGRes);
    Bind(wxEVT_MENU,&MainFrame::OnTextEdit,this,ID_TextEdit);
    Bind(wxEVT_MENU,&MainFrame::OnTextEditRaw,this,ID_TextEditRaw);
    Bind(wxEVT_MENU,&MainFrame::OnEditUnit,this,ID_EditUnit);
    Bind(wxEVT_MENU,&MainFrame::OnEditEvent,this,ID_EditEvent);
    Bind(wxEVT_MENU,&MainFrame::OnViewVideo,this,ID_ViewVideo);
    Bind(wxEVT_MENU,&MainFrame::OnViewMidi,this,ID_ViewMIDI);
    Bind(wxEVT_MENU,&MainFrame::OnTileFlags,this,ID_EditTileFlags);
    Bind(wxEVT_MENU,&MainFrame::OnViewVoxZ,this,ID_ViewVoxZ);
    Bind(wxEVT_MENU,&MainFrame::OnViewVoxZ,this,ID_ExportVoxZ);
    Bind(wxEVT_MENU,&MainFrame::OnExportMapRender,this,ID_ExportMapImg);    
    Bind(wxEVT_MENU,&MainFrame::OnExportAllMapsRender,this,ID_ExportMapsImg);    

    Bind(wxEVT_MENU,&MainFrame::OnViewMiniMap,this,ID_ViewMiniMap);
    Bind(wxEVT_MENU,&MainFrame::OnUnitViewDebug,this,ID_UnitViewDbg);
    Bind(wxEVT_MENU,&MainFrame::OnUpdateTileContext,this,ID_UpdateSprContext);
    Bind(wxEVT_MENU,&MainFrame::OnUpdateTileContextMaps,this,ID_UpdateSprContextMaps);
    Bind(wxEVT_MENU,&MainFrame::OnGenDMAobjects,this,ID_GenDMAobjects);
    Bind(wxEVT_MENU,&MainFrame::OnGenDMAobjectsMaps,this,ID_GenDMAobjectsMaps);
    
    Bind(wxEVT_MENU,&MainFrame::OnHistory,this,ID_HistoryUndo);
    Bind(wxEVT_MENU,&MainFrame::OnHistory,this,ID_HistoryRedo);
    Bind(wxEVT_MENU,&MainFrame::OnEditMissionParams,this,ID_EditMissionParams);
    Bind(wxEVT_MENU,&MainFrame::OnCopyBuf,this,ID_CopyBuf);
    Bind(wxEVT_MENU,&MainFrame::OnCopyBuf,this,ID_CutBuf);
    Bind(wxEVT_MENU,&MainFrame::OnPasteBuf,this,ID_PasteBuf);
    Bind(wxEVT_MENU,&MainFrame::OnClearBuf,this,ID_ClearBuf);
    Bind(wxEVT_MENU,&MainFrame::OnChangeElevation,this,ID_ElevUp);
    Bind(wxEVT_MENU,&MainFrame::OnChangeElevation,this,ID_ElevDown);
    Bind(wxEVT_MENU,&MainFrame::OnSelectAll,this,ID_SelectAll);
    Bind(wxEVT_MENU,&MainFrame::OnDeselectAll,this,ID_DeselectAll);
    Bind(wxEVT_MENU,&MainFrame::OnSelectDeselect,this,ID_SelectDeselect);
    Bind(wxEVT_MENU,&MainFrame::OnInvalidateSelection,this,ID_InvalidateSel);
    Bind(wxEVT_MENU,&MainFrame::OnDeleteSel,this,ID_DeleteSel);
    Bind(wxEVT_MENU,&MainFrame::OnCreateNewObject,this,ID_CreateNewObject);
    Bind(wxEVT_MENU,&MainFrame::OnAddUnit,this,ID_AddUnit);

    spell_map->SetMessageInterface(bind(&MainFrame::ShowMessage,this,placeholders::_1,placeholders::_2,placeholders::_3), bind(&MainFrame::CheckMessageState,this));    
    
    HistoryCheck();

    // main sizer 
    /*auto sizer2 = new wxBoxSizer(wxVERTICAL);

    wxButton *btnOk = new wxButton(canvas,wxID_ANY,wxT("TEST"),wxDefaultPosition,wxDefaultSize,0);
    sizer2->Add(btnOk,0,wxALL,5);

    canvas->SetSizer(sizer2);
    canvas->SetAutoLayout(true);
    canvas->Layout();*/

    //SetCursor(spelldata->gres.GetResource("DOJAZD.CUR")->Render(true)->ConvertToImage());
    
    //SetCursor(*spelldata->gres.ico_attack_up_down);

    //SpellTextRec text("Experimental text message", SpellLang::CZE);
    //ShowMessage(spell_data->texts->GetText("E06_0009"),false);
    
}
// on form close
void MainFrame::OnExit(wxCommandEvent& event)
{
    spell_map->Close();
    Close(true);
}
// about message
void MainFrame::OnAbout(wxCommandEvent& event)
{
    auto form = new FormAbout(this);
    if(form->ShowModal() == wxID_OK)
    {
        // --- confirmed
    }
    delete form;
}

// run configuration
void MainFrame::OnConfig(wxCommandEvent& event)
{
    // close map
    std::filesystem::path map_path = "";
    if(spell_map->IsLoaded())
    {
        wxMessageDialog dlg(this, "Opened map will be closed and all Spellcross data will be reloaded! Continue?", "Spellcross configuration", wxYES_NO | wxNO_DEFAULT | wxICON_WARNING);
        if(dlg.ShowModal() != wxID_YES)
            return;
        map_path = spell_map->GetTopPath();
    }
    spell_map->Close();

    // show config dialogue
    FormConfig form_config(this,ID_CONFIG_WIN,m_spell_config);
    form_config.ShowModal();

    // loose old spellcross data
    spell_data->Cleanup();
        
    // try reload spellcross data
    if(LoadSpellData(this, *m_spell_config,spell_data))
        Close();
    
    // ###note: This is absolutely needed because RtAudio seems to messup with threading concurency model for main UI thread,
    // which results in file dialog calls (and god knows what else) to hangup? Maybe it will be fixed in new version of wxWidgets? 
    // Anyway, here we reset it back to original state before RtAudio channels were changed in spell_data destructor/constructor above.
    // It took me whole day to figure this out! :-)
#ifdef __WXMSW__
    ::CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
#endif
    
    // try reload map
    if(!map_path.empty())
        spell_map->Load(map_path, spell_data);
    canvas->Refresh();
}


// callback function to write status messages from within the spellcross routines:
// usage: make and pass callback pointer using: bind(&MainFrame::StatusStringCallback,this,placeholders::_1)
// spellcross function example:
// void whatever_function(std::function<void(std::string)> status_cb)
// {
//   status_cb("Some message");
// }
void MainFrame::StatusStringCallback(std::string info)
{
    SetStatusTextLast(info);
}
// set status text to last status field
void MainFrame::SetStatusTextLast(std::string text)
{
    SetStatusText(text,GetStatusBar()->GetFieldsCount() - 1);
}
// set status to units field
void MainFrame::SetStatusTextUnits(std::string text)
{
    SetStatusText(text,7);
}
void MainFrame::SetStatusTextUnits(SpellMap *map)
{
    if(!map)
        map = spell_map;
    if(!map || !map->IsLoaded())
    {
        SetStatusTextUnits("");
        return;
    }
    auto &events = map->events->GetEvents();
    int start = 0;
    int spawn = 0;
    for(auto &evt: events)
    {
        if(evt->isMissionStart())
            start += evt->units.size();
        else
            spawn += evt->units.size();
    }
    SetStatusTextUnits(string_format("Units: {MissionStart: %d, Static-enemy: %d, Spawn: %d}", start, map->units.size(), spawn));
}
// set status to events field
void MainFrame::SetStatusTextEvents(std::string text)
{
    SetStatusText(text,8);
}
void MainFrame::SetStatusTextEvents(SpellMap* map)
{
    if(!map)
        map = spell_map;
    if(!map || !map->IsLoaded())
    {
        SetStatusTextUnits("");
        return;
    }
    auto& events = map->events->GetEvents();    
    SetStatusTextEvents(string_format("Events: %d",events.size()));
}
// update map status (units, events, etc.)
void MainFrame::UpdateMapStatus(SpellMap* map)
{
    SetStatusTextUnits(map);
    SetStatusTextEvents(map);
}



void MainFrame::OnClose(wxCloseEvent& ev)
{
    if(ev.GetId() == ID_GAMMA_WIN)
    {
        form_gamma->Destroy();
    }
    else if (ev.GetId() == ID_OBJECTS_WIN)
    {
        form_objects->Destroy();
        LoadToolsetRibbon();
    }
    else if (ev.GetId() == ID_SPRITES_WIN)
    {
        // on close sprite editor
        Terrain *terr = form_sprites->GetSelectedTerrain();
        Sprite *spr = form_sprites->GetSelectedSprite();        
        bool was_edit = form_sprites->wasSet();
        form_sprites->Destroy();
        LoadToolsetRibbon();

        if(spell_map && spell_map->IsLoaded() && spell_map->terrain == terr && spr)
        {
            if(was_edit)
            {
                // edit existing sprite
                spell_map->EditTileSprite(spr,&spell_pos);
                HistoryPush();
            }
            else
            {
                // some sprite selected - place to clipboard
                spell_map->SetBuffer(spr);
            }
        }
    }
    else if(ev.GetId() == ID_ANM_WIN)
    {
        // on close ANM viewer
        Terrain* terr = form_anms->GetSelectedTerrain();
        AnimL1* anm = form_anms->GetSelectedAnim();
        AnimPNM* pnm = form_anms->GetSelectedPNM();
        bool was_edit = form_anms->WasAnmSet();
        bool was_pnm =  form_anms->wasPNM();
        auto [x_ofs,y_ofs] = form_anms->GetPNMoffset();
        form_anms->Destroy();

        if(spell_map && spell_map->IsLoaded() && spell_map->terrain == terr)
        {
            if(anm)
            {
                if(was_edit)
                {
                    // edit existing map anim
                    spell_map->PlaceANM(&spell_pos,anm);
                    HistoryPush();
                }
                else
                {
                    // some anim selected - place to clipboard
                    spell_map->SetBuffer(anm);
                }
            }
            if(pnm)
            {
                if(was_edit)
                {
                    // edit existing map anim
                    spell_map->PlacePNM(&spell_pos,pnm,x_ofs,y_ofs);
                    HistoryPush();
                }
                else
                {
                    // some anim selected - place to clipboard
                    spell_map->SetBuffer(pnm,x_ofs,y_ofs);
                }
            }

        }
    }
    else if(ev.GetId() == ID_SOUNDS_WIN)
    {
        // on close sounds viewer        
        SpellSample *snd = form_sounds->GetSelectedSound();
        auto snd_type = form_sounds->GetMapSoundType();
        auto was_edit = form_sounds->WasSoundSet();
        form_sounds->Destroy();

        if(spell_map && spell_map->IsLoaded() && snd)
        {
            if(was_edit)
            {
                // edit existing sound
                spell_map->SoundEdit(snd,snd_type,&spell_pos);
                HistoryPush();
            }
            else
            {
                // add new sound
                auto map_sound = spell_map->SoundAdd(snd, snd_type, &spell_pos);
                if(map_sound)
                {
                    spell_map->SoundSelect(map_sound);
                    map_sound->in_placement = true;                    
                }
            }
        }                
    }
    else if(ev.GetId() == ID_PAL_WIN)
    {
        form_pal->Destroy();
    }
    else if(ev.GetId() == ID_GRES_WIN)
    {
        form_gres->Destroy();
    }
    else if(ev.GetId() == ID_GRES_ENCODE_WIN)
    {
        form_gres_encoder->Destroy();
    }
    else if(ev.GetId() == ID_TEXT_EDIT_WIN)
    {
        form_text_edit->Destroy();
    }
    else if(ev.GetId() == ID_TEXT_EDIT_RAW_WIN)
    {
        form_text_edit_raw->Destroy();
    }
    else if(ev.GetId() == ID_MINIMAP_WIN)
    {
        delete form_minimap;
        form_minimap = NULL;
    }
    else if(ev.GetId() == ID_UNITS_WIN)
    {
        // unit editor closed
        auto new_unit = form_units->DoAddUnit();
        if(new_unit)
        {
            // add new unit to map
            new_unit->in_placement = true;
            new_unit->is_active = true;
            new_unit->ResetAP();
            new_unit->not_placed_yet = true;
            auto pos = spell_map->GetSelection();
            if(pos.IsSelected())
                new_unit->coor = pos;
            else
                new_unit->coor = MapXY(0,0);
            if(new_unit->is_event)
            {
                // event unit - place to MissionStart
                spell_map->events->AddMissionStartUnit(new_unit);
            }
            else
            {
                // normal unit - place to map
                spell_map->AddUnit(new_unit);
            }
            spell_map->SelectUnit(new_unit);
        }
        if(form_units->DoUpdateUnit())
        {
            // update current unit:
            spell_map->SortUnits();
            HistoryPush();
            canvas->Refresh();
        }        
        form_units->Destroy();
    }
    else if(ev.GetId() == ID_EVENT_WIN)
    {
        // event editor closed        
        spell_map->SortUnits();
        HistoryPush(); // ###todo: this should be done only if something changed?
        canvas->Refresh();
        form_events->Destroy();
    }
    else if(ev.GetId() == ID_VIDEO_WIN)
    {
        // video viwer closed        
        form_videos->Destroy();
    }
    else if(ev.GetId() == ID_MIDI_WIN)
    {
        // midi player closed        
        form_midi->Destroy();
    }
    else if(ev.GetId() == ID_UNIT_MODE_WIN)
    {
        // unit multi-action menu
        form_unit_opts->ResultCallback(); // exec result callback (calling it from here to have in this thread)
        delete form_unit_opts;
        form_unit_opts = NULL;
    }
    else if(ev.GetId() == ID_MSG_WIN && form_message)
    {
        // unit multi-action menu
        form_message->ResultCallback(); // exec result callback (calling it from here to have in this thread)
        delete form_message;
        form_message = NULL;
    }
    else if(ev.GetId() == ID_VIDEO_BOX_WIN && form_video_box)
    {
        // unit multi-action menu
        //form_video_box->ResultCallback(); // exec result callback (calling it from here to have in this thread)
        delete form_video_box;
        form_video_box = NULL;
    }
    else if(ev.GetId() == ID_MAP_OPT_WIN && form_map_options)
    {
        // unit multi-action menu
        //form_map_options->ResultCallback(); // exec result callback (calling it from here to have in this thread)
        delete form_map_options;
        form_map_options = NULL;
    }
    else if(ev.GetId() == ID_MAP_UNITS_WIN && form_units_list)
    {
        // map unit list selection        
        auto *unit = form_units_list->GetSelectedUnit();
        if(unit)
            spell_map->SelectUnit(unit,true);
        
        delete form_units_list;
        form_units_list = NULL;
    }
    else
        ev.Skip();

    UpdateMapStatus();
}


// on switch game mode
void MainFrame::OnSwitchGameMode(wxCommandEvent& event)
{
    if(!spell_map->IsLoaded())
        return;

    auto is_game = GetMenuBar()->FindItem(ID_mmGameMode)->IsChecked();
    spell_map->SetGameMode(is_game);
    canvas->Refresh();
    if(is_game)
    {
        // switch to game mode
        ribbonBar->HidePanels();        
        menuView->FindItem(ID_ViewSoundLoops)->Check(false);
        menuView->FindItem(ID_ViewSounds)->Check(false);
        menuView->FindItem(ID_ViewEvents)->Check(false);
        OnViewLayer(event);

        // reset map
        spell_map->saves->Clear();
        spell_map->events->ResetEvents();
        spell_map->saves->SaveInitial();        
        // exec initial events
        spell_map->MissionStartEvent();
        // reset units view/attack ranges
        spell_map->unit_view->ClearEvents();
        spell_map->unit_view->ClearUnitsView(SpellMap::ViewRange::ClearMode::RESET);
        spell_map->unit_view->AddUnitsView();
    }
    else
    {
        // switch to editor mode
        spell_map->saves->LoadInitial();
        spell_map->ResetUnitEvents();
    }
}

// on reset view range in game mode
void MainFrame::OnResetUnitView(wxCommandEvent& event)
{
    if(!spell_map->IsLoaded())
        return;

    spell_map->unit_view->ClearUnitsView(SpellMap::ViewRange::ClearMode::RESET);
    spell_map->InvalidateUnitsView();
    canvas->Refresh();
}

// cycle unit range view modes
void MainFrame::OnSelectUnitView(wxCommandEvent& event)
{
    if(!spell_map->IsLoaded())
        return;

    if(spell_map->isGameMode())
    {
        // game mode unti range view selection
        spell_map->SetUnitRangeViewMode(SpellMap::UNIT_RANGE_INCREMENT);
        canvas->Refresh();
    }
}




// map animation periodic refresh tick
void MainFrame::OnTimer(wxTimerEvent& event)
{
    if(!spell_data)
        return;
    if(!spell_map || !canvas)
        return;
    if(!spell_map->IsLoaded())
        return;
    if(spell_map->Tick())
        canvas->Refresh();
}
// on main panel resizing
void MainFrame::OnResize(wxSizeEvent& event)
{
    if(spell_map)
        spell_map->scroller.SetSurface(canvas->GetClientSize().GetWidth(),canvas->GetClientSize().GetHeight());
    Refresh();
}

// render canvas repaint event
void MainFrame::OnThreadCanvas(wxThreadEvent& event)
{
    canvas->Refresh();
}
void MainFrame::OnPaintCanvas(wxPaintEvent& event)
{           
    // make buffer
    if(!m_buffer.IsOk() || m_buffer.GetSize() != canvas->GetClientSize())
        m_buffer = wxBitmap(canvas->GetClientSize(),24);
    
    // render map    
    if(!spell_data || !spell_map->IsLoaded())
        canvas->ClearBackground();
    else
    {
        wxPaintDC pdc(canvas);
        /*int frames = 100;
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();        
        for(int k = 0; k < frames; k++)*/
            spell_map->Render(m_buffer,NULL,&spell_tool,bind(&MainFrame::CreateHUDbuttons,this));
        /*std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        float time = 1e-6*std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        SetStatusText(wxString::Format(wxT("%.0f fps"),frames/time),7);*/
        pdc.DrawBitmap(m_buffer,wxPoint(0,0));
    }
       

    event.Skip();
}


void MainFrame::CreateHUDbuttons()
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
        if(!btn->IsValid())
            continue;

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
        wx_btn->Bind(wxEVT_PAINT,&MainFrame::OnPaintHUDbutton,this);
        wx_btn->Bind(wxEVT_LEAVE_WINDOW,&MainFrame::OnHUDbuttonsLeave,this);
        wx_btn->Bind(wxEVT_ENTER_WINDOW,&MainFrame::OnHUDbuttonsMouseEnter,this);
        wx_btn->Bind(wxEVT_LEFT_DOWN,&MainFrame::OnHUDbuttonsClick,this);
        wx_btn->Bind(wxEVT_LEFT_UP,&MainFrame::OnHUDbuttonsClick,this);
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
void MainFrame::OnPaintHUDbutton(wxPaintEvent& event)
{
    wxPanel* pan = (wxPanel*)event.GetEventObject();
    auto* btn = spell_map->GetHUDbutton(pan->GetId());
    if(btn)
    {                
        wxPaintDC pdc(pan);        
        if(btn->is_press && !btn->is_disabled)
            pdc.DrawBitmap(*btn->bmp_press,wxPoint(0,0));
        else if(btn->is_hover && !btn->is_disabled)
            pdc.DrawBitmap(*btn->bmp_hover,wxPoint(0,0));
        else
            pdc.DrawBitmap(*btn->bmp_idle,wxPoint(0,0));
    }
    event.Skip();
}
void MainFrame::OnHUDbuttonsMouseEnter(wxMouseEvent& event)
{
    if(!spell_data)
        return;
    wxPanel* pan = (wxPanel*)event.GetEventObject();
    auto* btn = spell_map->GetHUDbutton(pan->GetId());
    if(btn)
    {
        btn->is_hover = true;
        pan->Refresh();
        // click event callback?
        if(btn->is_hover && btn->cb_hover)
            btn->cb_hover();
        if(!btn->is_disabled)
        {
            // play hover sound            
            auto *hover_sound = new SpellSound(*spell_data->sounds->aux_samples.btn_hover);
            hover_sound->Play(true);

            /*std::thread snd(&SpellSound::PlayAsync,hover_sound);
            snd.detach();*/

            //std::async(std::launch::async,&SpellSound::PlayAsync,hover_sound);
            
            /*chrono::steady_clock::time_point ref_time = std::chrono::high_resolution_clock::now();            
            hover_sound->Play(true);
            auto now_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now_time - ref_time).count();
            StatusStringCallback(string_format("%d",duration));*/
            
        }
    }

    // default game cursor
    SetCursor(*spell_data->gres.cur_pointer);
}
void MainFrame::OnHUDbuttonsLeave(wxMouseEvent& event)
{
    wxPanel* pan = (wxPanel*)event.GetEventObject();
    auto* btn = spell_map->GetHUDbutton(pan->GetId());
    if(btn)
    {
        btn->is_hover = false;
        pan->Refresh();
    }
}
void MainFrame::OnHUDbuttonsClick(wxMouseEvent& event)
{
    if(!spell_data)
        return;
    wxPanel* pan = (wxPanel*)event.GetEventObject();
    auto* btn = spell_map->GetHUDbutton(pan->GetId());
    if(btn && !btn->is_disabled)
    {
        btn->is_press = (event.GetEventType() == wxEVT_LEFT_DOWN);        
        pan->Refresh();
        // play click sound        
        if(!btn->is_press && !btn->is_disabled)
        {
            auto* click_sound = new SpellSound(*spell_data->sounds->aux_samples.btn_end_turn);
            click_sound->Play(true);
        }
        // click event callback?
        if(!btn->is_press && btn->cb_press)
            btn->cb_press();
        if(btn->is_press)
            return;
        if(btn->action_id == SpellMap::HUD_ACTION_MINIMAP)
        {
            // show minimap
            wxCommandEvent cmd(wxEVT_MENU);
            OnViewMiniMap(cmd);
        }
        if(btn->action_id == SpellMap::HUD_ACTION_UNITS && !form_units_list)
        {
            // show units list            
            form_units_list = new FormMapUnits(canvas,ID_MAP_UNITS_WIN, spell_data, spell_map);
        }
        if(btn->action_id == SpellMap::HUD_ACTION_MAP_OPTIONS && !form_map_options)
        {
            // show map options
            form_map_options = new FormMapOptions(canvas,ID_MAP_OPT_WIN,spell_map);
        }
    }
}



// on change of map layer view
void MainFrame::OnViewLayer(wxCommandEvent& event)
{
    
    bool wL1 = GetMenuBar()->FindItem(ID_ViewTer)->IsChecked();
    bool wL2 = GetMenuBar()->FindItem(ID_ViewObj)->IsChecked();
    bool wL3 = GetMenuBar()->FindItem(ID_ViewAnm)->IsChecked();
    bool wL4 = GetMenuBar()->FindItem(ID_ViewPnm)->IsChecked();
    bool wL5 = GetMenuBar()->FindItem(ID_ViewUnt)->IsChecked();
    bool wSS = GetMenuBar()->FindItem(ID_ViewStTa)->IsChecked();
    bool wCounterStart = GetMenuBar()->FindItem(ID_ViewStartCounterAttack)->IsChecked();    
    bool wSound = GetMenuBar()->FindItem(ID_ViewSounds)->IsChecked();
    bool wSoundLoop = GetMenuBar()->FindItem(ID_ViewSoundLoops)->IsChecked();
    bool wEvents = GetMenuBar()->FindItem(ID_ViewEvents)->IsChecked();
    bool wHobj = GetMenuBar()->FindItem(ID_HighlighObj)->IsChecked();
    bool wDebug = GetMenuBar()->FindItem(ID_ShowDebug)->IsChecked();
    spell_map->SetRender(wL1,wL2,wL3,wL4,wSS,wCounterStart,wL5,wSound,wSoundLoop,wEvents,wHobj,wDebug);
    bool hud = GetMenuBar()->FindItem(ID_ViewHUD)->IsChecked();
    spell_map->SetHUDstate(hud);
    Refresh();
    
}

// enable disable unit view debug mode
void MainFrame::OnUnitViewDebug(wxCommandEvent& event)
{    
    if(!spell_map->IsLoaded())
        return;

    spell_map->SetUnitsViewDebugMode(GetMenuBar()->FindItem(ID_UnitViewDbg)->IsChecked());
    auto unit = spell_map->GetSelectedUnit();
    spell_map->unit_view->AddUnitView(unit,
        spell_map->isUnitsViewDebugMode()?(SpellMap::ViewRange::ClearMode::HIDE):(SpellMap::ViewRange::ClearMode::NONE));
}

// open new map
void MainFrame::OnOpenMap(wxCommandEvent& event)
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
    if(spell_map->Load(path, spell_data))
        wxMessageBox(string_format("Loading Spellcross map file failed with error:\n%s",spell_map->GetLastError().c_str()),"Error",wxICON_ERROR);
    else if(!spell_map->hide_map_load_warnings && !spell_map->GetLastError().empty())
        wxMessageBox(string_format("Loading Spellcross map file ended with warning(s):\n%s",spell_map->GetLastError().c_str()),"Warning",wxICON_WARNING);
    
    // reset layers visibility
    //spell_map->SetGamma(1.30);
    OnViewLayer(event);
    // reload toolset ribbon
    LoadToolsetRibbon();
    // repaint
    Refresh();

    HistoryCheck();
    UpdateMapStatus();
}

// save map data files
void MainFrame::OnSaveMap(wxCommandEvent& event)
{
    if(!spell_map || !spell_map->IsLoaded())
        return;
    
    bool save_as = spell_map->map_path.empty() || spell_map->def_path.empty() || event.GetId() == ID_SaveMapAs;
    if(save_as)
    {
        // save as
        OnSaveDTA(event);
        OnSaveDEF(event);
    }
    else
    {
        // save
        wxMessageDialog dlg(this, "Overwrire map files (DTA and DEF)?", "Save map file", wxYES_NO| wxYES_DEFAULT);
        if(dlg.ShowModal() != wxID_YES)
            return;
        spell_map->SaveDTA(spell_map->map_path);
        spell_map->SaveDEF(spell_map->def_path);
    }    
}

// save map DTA file
void MainFrame::OnSaveDTA(wxCommandEvent& event)
{
    if(!spell_map || !spell_map->IsLoaded())
        return;

    // split path to folder and file    
    std::filesystem::path last_path = spell_map->map_path;
    wstring dir = last_path.parent_path(); dir += wstring(L"\\");
    wstring name = last_path.filename();

    // show save dialog
    wxFileDialog saveFileDialog(this,_("Save Spellcross Map DTA file"),dir,name,"Map data file (*.dta)|*.dta",
        wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if(saveFileDialog.ShowModal() == wxID_CANCEL)
        return;
    wstring path = wstring(saveFileDialog.GetPath().ToStdWstring());

    if(spell_map->SaveDTA(path))
        wxMessageBox(string_format("Saving Spellcross map DTA file failed with error:\n%s",spell_map->GetLastError().c_str()),"Error",wxICON_ERROR);
}

// save map DEF file
void MainFrame::OnSaveDEF(wxCommandEvent& event)
{
    if(!spell_map || !spell_map->IsLoaded())
        return;

    // split path to folder and file    
    std::filesystem::path last_path = spell_map->def_path;
    wstring dir = last_path.parent_path(); dir += wstring(L"\\");
    wstring name = last_path.filename();

    // show open dialog
    wxFileDialog saveFileDialog(this,_("Save Spellcross Map DEF file"),dir,name,"Map script file (*.def)|*.def",
        wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if(saveFileDialog.ShowModal() == wxID_CANCEL)
        return;
    wstring path = wstring(saveFileDialog.GetPath().ToStdWstring());

    if(spell_map->SaveDEF(path))
        wxMessageBox(string_format("Saving Spellcross map DEF file failed with error:\n%s",spell_map->GetLastError().c_str()),"Error",wxICON_ERROR);
}

// create new map
void MainFrame::OnNewMap(wxCommandEvent& event)
{
    if(!spell_data)
        return;

    FormNewMap form(this, spell_data,ID_NEW_MAP);
    if(form.ShowModal() != wxID_OK)
        return;
    
    // create some map
    auto [xx,yy] = form.GetSize();
    auto ee = form.GetElev();
    spell_map->Create(spell_data, form.GetTerrain().c_str(), xx,yy, ee);
    // reset layers visibility
    //spell_map->SetGamma(1.30);
    OnViewLayer(event);
    // reload toolset ribbon
    LoadToolsetRibbon();

    HistoryCheck();
    UpdateMapStatus();
}

// set gamma correction
void MainFrame::OnSetGamma(wxCommandEvent& event)
{
    if(!FindWindowById(ID_GAMMA_WIN))
    {
        form_gamma = new FormGamma(this,spell_map,ID_GAMMA_WIN);
        form_gamma->Show();
    } 
}

// open sprite viewer
void MainFrame::OnViewSprites(wxCommandEvent& event)
{    
    if(!FindWindowById(ID_SPRITES_WIN))
    {
        form_sprites = new FormSprite(this, spell_data, ID_SPRITES_WIN);
        if(spell_map)
            form_sprites->SetSprite(spell_map->terrain);
        form_sprites->Show();
    }
}

// open ANM viewer
void MainFrame::OnViewAnms(wxCommandEvent& event)
{
    if(!FindWindowById(ID_ANM_WIN))
    {
        form_anms = new FormANM(this,spell_data,false,ID_ANM_WIN);
        form_anms->Show();
    }
}

// open PNM viewer
void MainFrame::OnViewPnms(wxCommandEvent& event)
{
    if(!FindWindowById(ID_ANM_WIN))
    {
        form_anms = new FormANM(this,spell_data,true,ID_ANM_WIN);
        form_anms->Show();
    }
}

// open sounds viewer
void MainFrame::OnViewSounds(wxCommandEvent& event)
{
    if(!FindWindowById(ID_SOUNDS_WIN))
    {
        form_sounds = new FormSound(this,spell_data,ID_SOUNDS_WIN);
        form_sounds->Show();
    }
}

// update tiles context from map selection
void MainFrame::OnUpdateTileContext(wxCommandEvent& event)
{
    if(!spell_map->IsLoaded())
        return;

    spell_map->BuildSpriteContext();
}

// generate DMAx_xxx tiles objects from this map
void MainFrame::OnGenDMAobjects(wxCommandEvent& event)
{
    if(!spell_map->IsLoaded())
        return;
    spell_map->BuildHouseObjects();
    LoadToolsetRibbon();
}

// generate DMAx_xxx tiles objects from all maps
void MainFrame::OnGenDMAobjectsMaps(wxCommandEvent& event)
{
    FormTerrain form(this,spell_data,ID_TARRAIN_WIN);
    if(form.ShowModal() != wxID_OK)
        return;
    // --- confirmed
    auto terr_name = form.GetTerrain();
        
    // default path
    wstring dir = std::filesystem::path(spell_data->spell_data_root) / std::filesystem::path("data") / std::filesystem::path("common");
 
    // show open dialog
    wxDirDialog fd = wxDirDialog(this,_("Select Spellcross COMMON folder"),dir,wxDD_DIR_MUST_EXIST);
    if(fd.ShowModal() == wxID_CANCEL)
        return;
    wstring path = wstring(fd.GetPath().ToStdWstring());

    // load map context
    spell_data->BuildHouseObjectsOfMaps(path,terr_name,bind(&MainFrame::StatusStringCallback,this,placeholders::_1));

    LoadToolsetRibbon();
}

// update tiles context from all maps
void MainFrame::OnUpdateTileContextMaps(wxCommandEvent& event)
{   
    FormTerrain form(this,spell_data,ID_TARRAIN_WIN);
    if(form.ShowModal() != wxID_OK)
        return;
    // --- confirmed
    auto terr_name = form.GetTerrain();

    // default path
    wstring dir = std::filesystem::path(spell_data->spell_data_root) / std::filesystem::path("data") / std::filesystem::path("common");

    // show open dialog
    wxDirDialog fd = wxDirDialog(this, _("Select Spellcross COMMON folder"), dir, wxDD_DIR_MUST_EXIST);
    if(fd.ShowModal() == wxID_CANCEL)
        return;
    wstring path = wstring(fd.GetPath().ToStdWstring());
    
    // load map context
    spell_data->BuildSpriteContextOfMaps(path,terr_name, bind(&MainFrame::StatusStringCallback,this,placeholders::_1));
}


// open objects viewer
void MainFrame::OnViewObjects(wxCommandEvent& event)
{
    if(!FindWindowById(ID_OBJECTS_WIN))
    {
        form_objects = new FormObjects(this,spell_data,ID_OBJECTS_WIN);
        //form_objects->Connect(wxID_ANY,wxEVT_DESTROY,(wxObjectEventFunction)&MainFrame::OnViewObjectsClose);
        form_objects->SetMap(spell_map);
        form_objects->Show();
    }
}

// open palette viewer
void MainFrame::OnViewPal(wxCommandEvent& event)
{
    if(!FindWindowById(ID_PAL_WIN))
    {
        form_pal = new FormPalView(this,spell_data,ID_PAL_WIN);
        form_pal->SetMap(spell_map);
        form_pal->Show();
    }
}

// open graphics viewer
void MainFrame::OnViewGrRes(wxCommandEvent& event)
{
    if(!FindWindowById(ID_GRES_WIN))
    {
        form_gres = new FormGResView(this,spell_data,ID_GRES_WIN);
        form_gres->Show();
    }
}

// open graphics encoder
void MainFrame::OnEncodeGrRes(wxCommandEvent& event)
{
    if(!FindWindowById(ID_GRES_ENCODE_WIN))
    {
        form_gres_encoder = new FormGResEncoder(this,spell_data,ID_GRES_ENCODE_WIN);
        form_gres_encoder->Show();
    }
}

// open text view/editor
void MainFrame::OnTextEdit(wxCommandEvent& event)
{
    if(!FindWindowById(ID_TEXT_EDIT_WIN))
    {
        form_text_edit = new FormText(this,spell_data,ID_TEXT_EDIT_WIN);
        form_text_edit->Show();
    }
}

// open raw text view/editor
void MainFrame::OnTextEditRaw(wxCommandEvent& event)
{
    if(!FindWindowById(ID_TEXT_EDIT_RAW_WIN))
    {
        form_text_edit_raw = new FormTextEdit(this,ID_TEXT_EDIT_RAW_WIN);
        form_text_edit_raw->Show();
    }
}


// open units viewer/editor
void MainFrame::OnEditUnit(wxCommandEvent& event)
{
    if(!FindWindowById(ID_UNITS_WIN))
    {
        form_units = new FormUnits(this,ID_UNITS_WIN);
        form_units->SetSpellData(spell_data);
        form_units->SetMapUnit(spell_map->GetSelectedUnit());
        form_units->Show();
    }
}

// open events viewer/editor
void MainFrame::OnEditEvent(wxCommandEvent& event)
{
    if(!spell_map->IsLoaded())
        return;
    if(!FindWindowById(ID_EVENT_WIN))
    {
        form_events = new FormEvent(this,spell_data,ID_EVENT_WIN);
        form_events->SetMap(spell_map);
        form_events->Show();
    }
}

// open video viewer
void MainFrame::OnViewVideo(wxCommandEvent& event)
{
    /*if(!FindWindowById(ID_VIDEO_BOX_WIN))
    {
        form_video_box = new FormVideoBox(canvas,ID_VIDEO_BOX_WIN,spell_data,"LEVEL10.DPK");
    }*/
    if(!FindWindowById(ID_VIDEO_WIN))
    {
        form_videos = new FormVideo(this,spell_data,ID_VIDEO_WIN);
        form_videos->Show();
    }
}

// open MIDI player
void MainFrame::OnViewMidi(wxCommandEvent& event)
{
    if(!FindWindowById(ID_MIDI_WIN))
    {
        form_midi = new FormMIDI(this,spell_data,ID_MIDI_WIN);
        form_midi->Show();
    }
}

// tile flags editor
void MainFrame::OnTileFlags(wxCommandEvent& event)
{
    if(!spell_map || !spell_map->IsLoaded())
        return;
    auto flags = spell_map->GetFlags(spell_map->GetSelection());
    auto form = new FormFlags(this,spell_map->terrain,flags);
    if(form->ShowModal() == wxID_OK)
    {
        // --- confirmed
        flags = form->GetSelectedFlags();
        
        // set to all selected tiles
        auto posz = spell_map->GetSelections();
        spell_map->SetFlags(posz, flags);
    }
    delete form;
}





// export voxel map elevation raster
void MainFrame::OnViewVoxZ(wxCommandEvent& event)
{    
    if(!spell_map->IsLoaded())
        return;

    wxBitmap* bmp = spell_map->unit_view->ExportUnitsViewZmap();
    if(event.GetId() == ID_ViewVoxZ)
    {
        // view only:
        auto [map_x,map_y] = spell_map->GetMapSurfaceSize();
        
        // ceate panel
        TMiniMap minimap ={bmp, &spell_map->scroller, spell_map, 0, 0, map_x, map_y};
        form_minimap = new FormMiniMap(canvas,ID_MINIMAP_WIN,spell_data,minimap);
    }
    else if(event.GetId() == ID_ExportVoxZ)
    {
        // export to file:

        // split path to folder and file
        std::wstring dir = std::filesystem::path(GetExecutableDir());
        wstring name = L"zmap.png";

        // show open dialog
        wxFileDialog saveFileDialog(this,_("Save voxel map elevation"),dir,name,"PNG file (*.png)|*.png",wxFD_SAVE);
        if(saveFileDialog.ShowModal() == wxID_CANCEL)
            return;
        wstring path = wstring(saveFileDialog.GetPath().ToStdWstring());

        // expor as PNG
        bmp->SaveFile(path,wxBITMAP_TYPE_PNG);
        delete bmp;
    }
    
}

// view minimap
void MainFrame::OnViewMiniMap(wxCommandEvent& event)
{
    if(!spell_map->IsLoaded())
        return;
    
    // make local scroll object with zero scroll state
    TScroll scrl;
    scrl.Reset();
    
    // obtain redner surface range
    auto [pic_x,pic_y] = spell_map->GetMapSurfaceSize();
    int hud_state = spell_map->SetHUDstate(false);
    wxBitmap* buf = new wxBitmap(1,1,24);
    scrl.SetPos(0,0);
    //scrl.SetSurface(pic_x,pic_y);
    scrl.SetSurface(1,1);
    spell_map->RenderPrepare(&scrl);
    auto [x1,y1] = scrl.GetScroll();
    scrl.SetPos(pic_x,pic_y);
    spell_map->RenderPrepare(&scrl);
    auto [x2,y2] = scrl.GetScroll();
    delete buf;
    
    // make local render buffer for entire map size    
    buf = new wxBitmap(x2-x1, y2-y1, 24);
    scrl.SetSurface(x2-x1,y2-y1);
    scrl.SetPos(0,0);
    spell_map->Render(*buf, &scrl);
    spell_map->SetHUDstate(hud_state);
    scrl.SetSurface(canvas->GetClientSize().GetWidth(),canvas->GetClientSize().GetHeight());
    
    // ceate panel
    TMiniMap minimap = {buf, &spell_map->scroller, spell_map, x1, y1, x2-x1, y2-y1};
    form_minimap = new FormMiniMap(canvas,ID_MINIMAP_WIN,spell_data,minimap);

}


// export current map render
void MainFrame::OnExportMapRender(wxCommandEvent& event)
{
    if(!spell_map->IsLoaded())
        return;

    // save dialogue
    auto map_path = std::filesystem::path(spell_map->GetTopPath());
    auto png_name = std::wstring(map_path.stem().concat(".png"));
    wxFileDialog saveFileDialog(this,_("Save map render image"),spell_data->export_path,png_name,"PNG file (*.png)|*.png",wxFD_SAVE);
    if(saveFileDialog.ShowModal() == wxID_CANCEL)
        return;
    auto png_path = saveFileDialog.GetPath();

    // make local scroll object with zero scroll state
    TScroll scrl;
    scrl.Reset();

    // obtain redner surface range
    auto [pic_x,pic_y] = spell_map->GetMapSurfaceSize();
    int hud_state = spell_map->SetHUDstate(false);
    wxBitmap* buf = new wxBitmap(1,1,24);
    scrl.SetPos(0,0);
    //scrl.SetSurface(pic_x,pic_y);
    scrl.SetSurface(1,1);
    spell_map->RenderPrepare(&scrl);
    auto [x1,y1] = scrl.GetScroll();
    scrl.SetPos(pic_x,pic_y);
    spell_map->RenderPrepare(&scrl);
    auto [x2,y2] = scrl.GetScroll();
    delete buf;

    // make local render buffer for entire map size    
    buf = new wxBitmap(x2-x1,y2-y1,24);
    scrl.SetSurface(x2-x1,y2-y1);
    scrl.SetPos(0,0);
    spell_map->Render(*buf,&scrl);
    spell_map->SetHUDstate(hud_state);

    // try save
    buf->SaveFile(png_path,wxBITMAP_TYPE_PNG);
    delete buf;        
}


// export all maps renders
void MainFrame::OnExportAllMapsRender(wxCommandEvent& event)
{
    auto src_dir = std::filesystem::path(spell_map->GetTopPath()).parent_path().wstring();

    // get source maps
    wxFileDialog openMapsDialogue(this, "Select map files to render (multiple files allowed)", src_dir, "M*.def", "Map DEF files (*.def)|*.def", wxFD_OPEN|wxFD_MULTIPLE|wxFD_FILE_MUST_EXIST);
    if(openMapsDialogue.ShowModal() == wxID_CANCEL)
        return;
    wxArrayString maps_list;
    openMapsDialogue.GetFilenames(maps_list);
    
    // target directory dialogue
    wxDirDialog saveDialog(this,_("Select export path for renders"),spell_data->export_path,wxDD_DIR_MUST_EXIST);
    if(saveDialog.ShowModal() == wxID_CANCEL)
        return;
    auto export_dir = std::filesystem::path(saveDialog.GetPath().ToStdWstring());
    // rather ask for overwrite
    wxMessageDialog overDial(this, "Image files in target directory can be overwriten! Continue?", "Exporting map renders", wxICON_EXCLAMATION|wxYES_NO|wxYES_DEFAULT);
    if(overDial.ShowModal() != wxID_YES)
        return;
    spell_data->export_path = export_dir;


    bool wL1 = GetMenuBar()->FindItem(ID_ViewTer)->IsChecked();
    bool wL2 = GetMenuBar()->FindItem(ID_ViewObj)->IsChecked();
    bool wL3 = GetMenuBar()->FindItem(ID_ViewAnm)->IsChecked();
    bool wL4 = GetMenuBar()->FindItem(ID_ViewPnm)->IsChecked();
    bool wL5 = GetMenuBar()->FindItem(ID_ViewUnt)->IsChecked();
    bool wSS = GetMenuBar()->FindItem(ID_ViewStTa)->IsChecked();
    bool wSound = GetMenuBar()->FindItem(ID_ViewSounds)->IsChecked();
    bool wSoundLoop = GetMenuBar()->FindItem(ID_ViewSoundLoops)->IsChecked();
    bool wEvents = GetMenuBar()->FindItem(ID_ViewEvents)->IsChecked();
    bool wHobj = GetMenuBar()->FindItem(ID_HighlighObj)->IsChecked();
    bool wDebug = GetMenuBar()->FindItem(ID_ShowDebug)->IsChecked();

    // for each map file
    for(int fid = 0; fid < maps_list.Count(); fid++)
    {        
        auto map_path = maps_list[fid];        
        SetStatusTextLast(string_format("Exporting map %d of %d: \"%ls\"...",fid+1,maps_list.Count(),map_path.ToStdWstring().c_str()));

        // try load map
        SpellMap map;
        map.hide_map_load_warnings = true;
        if(map.Load(std::filesystem::path(map_path.ToStdWstring()),spell_data))
        {
            wxMessageDialog dlg(this,string_format("Loading map file \"%ls\" failed! Error message:\n%s\n\nContinue?",map_path.ToStdWstring().c_str(), map.GetLastError().c_str()), "Export map render error",wxYES_NO|wxYES_DEFAULT|wxICON_ERROR);
            if(dlg.ShowModal() != wxID_YES)
                break;
            continue;
        }

        // set current view format
        map.SetRender(wL1,wL2,wL3,wL4,wSS,false,wL5,wSound,wSoundLoop,wEvents,wHobj,wDebug);

        // make local scroll object with zero scroll state
        TScroll scrl;
        scrl.Reset();
        map.RenderPrepare(&scrl);
        map.SetHUDstate(false);
        map.SetGamma(spell_map->GetGamma());
        auto [pic_x,pic_y] = map.GetMapSurfaceSize();
        // render full scale image
        auto buf = new wxBitmap(pic_x,pic_y,24);
        scrl.SetSurface(pic_x,pic_y);
        scrl.SetPos(0,0);
        map.Render(*buf,&scrl);

        int x_crop_left = 100;
        int y_crop_top = 250;
        int x_crop_right = 200;
        int y_crop_bot = 400;
        // try save
        auto png_path = export_dir / std::filesystem::path(map_path.ToStdWstring()).stem().concat(".png");
        buf->GetSubBitmap(wxRect(x_crop_left, y_crop_top, pic_x - x_crop_left - x_crop_right, pic_y - y_crop_top - y_crop_bot)).SaveFile(png_path.wstring(),wxBITMAP_TYPE_PNG);
        delete buf;
    }
    SetStatusTextLast(string_format("Exporting %d maps done!",maps_list.Count()));
}




// edit mission parameters
void MainFrame::OnEditMissionParams(wxCommandEvent& event)
{
    if(!spell_data || !spell_map)
        return;
    FormMissionParams* form = new FormMissionParams(this,spell_data,spell_map);
    if(form->ShowModal() == wxID_OK)
    {
        // --- confirmed
    }

    // destroy form
    delete form;
}


// create new object
void MainFrame::OnCreateNewObject(wxCommandEvent& event)
{
    if(!spell_map->IsLoaded())
        return;

    FormNewObject form(this,spell_map->terrain);
    if(form.ShowModal() == wxID_OK)
    {
        // --- confirmed

        // get object descriptions
        std::string description = form.GetDescription();
        int class_id = form.GetClass();

        // get layers mask
        SpellMap::Layers lay;
        lay.lay1 = GetMenuBar()->FindItem(ID_SelectLay1)->IsChecked();
        lay.lay2 = GetMenuBar()->FindItem(ID_SelectLay2)->IsChecked();
        lay.anm = GetMenuBar()->FindItem(ID_SelectLayANM)->IsChecked();
        lay.pnm = GetMenuBar()->FindItem(ID_SelectLayPNM)->IsChecked();

        auto posxy = spell_map->GetPersistSelections();
        auto L1_list = spell_map->GetL1sprites(posxy);
        auto L2_list = spell_map->GetL2sprites(posxy);
        auto flag_list = spell_map->GetFlags(posxy);
        auto pnm_list = spell_map->GetPNMs(posxy);
        if(!lay.lay1)
            for(auto& spr: L1_list)
                spr = NULL;
        if(!lay.lay2)
            for(auto& spr: L2_list)
                spr = NULL;
        if(!lay.pnm)
            pnm_list.clear();
        
        // add object to list
        auto obj =spell_map->terrain->AddObject(posxy,L1_list,L2_list,flag_list,pnm_list,(uint8_t*)spell_map->terrain->pal,description);
        obj->SetToolClass(class_id);
                
        // clear selection
        spell_map->SelectTiles(SpellMap::SELECT_CLEAR);

        // refresh tools list
        LoadToolsetRibbon(spell_map->terrain);
    }
    
    // destroy form
    //delete form;    
}

// add new unit
void MainFrame::OnAddUnit(wxCommandEvent& event)
{
    if(!spell_map->IsLoaded())
        return;

    if(!FindWindowById(ID_UNITS_WIN))
    {
        // make new unit        
        /*MapUnit *new_unit = spell_map->CreateUnit();
        spell_map->SelectUnit(new_unit);
        new_unit->in_placement = true;
        new_unit->is_active = true;*/               

        form_units = new FormUnits(this,ID_UNITS_WIN);
        form_units->SetSpellData(spell_data);
        form_units->SetMapUnit(NULL, spell_map, &m_spell_unit_template);
        form_units->Show();
    }


    /*auto *unit = spell_map->GetSelectedUnit();
    if(unit)
    {
        // start unit movement
        unit->in_placement = true;
    }*/
    
}


// on history undo/redo
void MainFrame::OnHistory(wxCommandEvent& event)
{
    if(!spell_map->IsLoaded())
        return;
    spell_map->HistoryPop(event.GetId() == ID_HistoryRedo);
    HistoryCheck();
    canvas->Refresh();
}
// update history undo/redo buttons state
void MainFrame::HistoryCheck()
{
    bool has_undo = false;
    bool has_redo = false;
    if(spell_map->IsLoaded())
    {
        has_undo = spell_map->HistoryCanPop(false);
        has_redo = spell_map->HistoryCanPop(true);
    }

    auto mm = menuEdit->FindItem(ID_HistoryUndo);
    if(mm)
        mm->Enable(has_undo);
    mm = menuEdit->FindItem(ID_HistoryRedo);
    if(mm)
        mm->Enable(has_redo);
}
// push map state to history
void MainFrame::HistoryPush()
{
    if(!spell_map->IsLoaded())
        return;
    spell_map->HistoryPush();
    HistoryCheck();
}



// unit popup menu
void MainFrame::OnCanvasPopupSelect(wxCommandEvent& event)
{
    if(!spell_map->IsLoaded())
        return;

    auto menu_id = event.GetId();
    auto menu = (wxMenu*)event.GetEventObject();
    auto cur_unit = (MapUnit*)menu->GetClientData();
    if(menu_id >= ID_POP_ADD_MISSIONSTART && menu_id <= ID_POP_ADD_MISSIONSTART_MAX)
    {
        // try add unit to MissionStart() event
        int probab = menu_id - ID_POP_ADD_MISSIONSTART;
        spell_map->ExtractUnit(cur_unit);
        spell_map->events->AddMissionStartUnit(cur_unit,probab);
        spell_map->SortUnits();
        HistoryPush();
    }
    else if(menu_id == ID_POP_REM_MISSIONSTART)
    {
        // try remove the unit from MissionStart() event
        cur_unit->creator_event->ExtractUnit(cur_unit);        
        spell_map->AddUnit(cur_unit);
        spell_map->SortUnits();
        HistoryPush();
    }
    else if(menu_id == ID_POP_ADD_SEEUNIT)
    {
        // try add SeeUnit() event
        spell_map->events->AddSeeUnitEvent(cur_unit);
        spell_map->SortUnits();
        HistoryPush();
    }
    else if(menu_id == ID_POP_ADD_DESTROY_UNIT)
    {
        // try add DestroyUnit() objective
        spell_map->events->AddUnitObjective(cur_unit,SpellMapEventRec::EvtTypes::EVT_DESTROY_UNIT);
        spell_map->SortUnits();
        HistoryPush();
    }
    else if(menu_id == ID_POP_ADD_SAVE_UNIT)
    {
        // try add SaveUnit() objective
        spell_map->events->AddUnitObjective(cur_unit,SpellMapEventRec::EvtTypes::EVT_SAVE_UNIT);
        spell_map->SortUnits();
        HistoryPush();
    }
    else if(menu_id == ID_POP_ADD_TRANSPORT_UNIT)
    {
        // try add TransportUnit() objective
        spell_map->events->AddUnitObjective(cur_unit,SpellMapEventRec::EvtTypes::EVT_TRANSPORT_UNIT);
        spell_map->SortUnits();
        HistoryPush();
    }
    else if(menu_id == ID_POP_REM_SEEUNIT || menu_id == ID_POP_REM_DESTROY_UNIT || menu_id == ID_POP_REM_SAVE_UNIT || menu_id == ID_POP_REM_TRANSPORT_UNIT)
    {
        // try remove xxxUnit() events
        std::map<int,SpellMapEventRec::EvtTypes> types = {{ID_POP_REM_SEEUNIT,SpellMapEventRec::EvtTypes::EVT_SEE_UNIT},{ID_POP_REM_DESTROY_UNIT,SpellMapEventRec::EvtTypes::EVT_DESTROY_UNIT},{ID_POP_REM_SAVE_UNIT,SpellMapEventRec::EvtTypes::EVT_SAVE_UNIT},{ID_POP_REM_TRANSPORT_UNIT,SpellMapEventRec::EvtTypes::EVT_TRANSPORT_UNIT}};
        auto evt = cur_unit->GetTrigEvent(types.at(menu_id));
        spell_map->events->EraseEvent(evt);
        HistoryPush();
    }
    else if(menu_id == ID_POP_ADD_SEE_PLACE)
    {
        // try create SeePlace event        
        spell_map->events->AddSeePlaceEvent(spell_pos);
        HistoryPush();
    }
    else if(menu_id == ID_POP_REM_SEE_PLACE)
    {
        // try remove SeePlace event        
        auto evt = spell_map->events->CheckEvent(SpellMapEventRec::EvtTypes::EVT_SEE_PLACE,&spell_pos);
        spell_map->events->EraseEvent(evt);
        HistoryPush();
    }
    else if(menu_id == ID_POP_ADD_SPAWN_UNIT)
    {
        // try add/remove unit to event
        auto cur_evt = spell_map->GetSelectEvent();
        spell_map->UpdateEventUnit(cur_evt,cur_unit);
        HistoryPush();
    }
    else if(menu_id == ID_POP_EDIT_EVENT)
    {
        // edit event        
        auto cur_evt = spell_map->GetSelectEvent();
        //spell_map->SelectEvent(cur_evt);
        OnEditEvent(event);
    }
    else if(menu_id == ID_POP_ANOTHER_EVENT)
    {
        // switch to another event at position
        auto cur_evt = spell_map->GetSelectEvent();
        spell_map->SelectEvent(spell_map->events->GetAnotherEvent(cur_evt));        
    }
    else if(menu_id == ID_POP_EDIT_UNIT)
    {
        // edit unit
        spell_map->SelectUnit(cur_unit);
        OnEditUnit(event);
    }
    else if(menu_id == ID_POP_REM_UNIT)
    {
        // remove unit
        spell_map->RemoveUnit(cur_unit,true);
        HistoryPush();
    }
    else if(menu_id == ID_POP_REM_OBJ)
    {
        // remove object tile        
        spell_map->RemoveObj();
        HistoryPush();
        Refresh();
    }
    else if(menu_id == ID_POP_EDIT_TERR)
    {
        // edit terrain tile
        if(!FindWindowById(ID_SPRITES_WIN))
        {
            spell_pos = spell_map->GetSelection();
            auto tile = spell_map->GetTile();
            if(tile)
            {
                form_sprites = new FormSprite(this,spell_data,ID_SPRITES_WIN);
                form_sprites->SetSprite(spell_map->terrain,tile->L1);
                form_sprites->Show();
            }
        }
    }
    else if(menu_id == ID_POP_EDIT_OBJ)
    {
        // edit obj tile
        if(!FindWindowById(ID_SPRITES_WIN))
        {
            spell_pos = spell_map->GetSelection();
            form_sprites = new FormSprite(this,spell_data,ID_SPRITES_WIN);
            auto spr = spell_map->CheckObj();
            form_sprites->SetSprite(spell_map->terrain,spr->L2);
            form_sprites->Show();
        }
    }
    else if(menu_id == ID_POP_REM_ANM)
    {
        // remove ANM tile
        spell_map->RemoveANM();
        HistoryPush();
    }
    else if(menu_id == ID_POP_EDIT_ANM)
    {
        // edit ANM tile
        if(!FindWindowById(ID_ANM_WIN))
        {
            spell_pos = spell_map->GetSelection();
            form_anms = new FormANM(this,spell_data,false,ID_ANM_WIN);
            form_anms->SetANM(spell_map->terrain, spell_map->CheckANM()->anim);
            form_anms->Show();
        }
    }
    else if(menu_id == ID_POP_REM_PNM)
    {
        // remove PNM animation
        spell_map->RemovePNM();
        HistoryPush();
    }
    else if(menu_id == ID_POP_EDIT_PNM)
    {
        // edit PNM animation
        if(!FindWindowById(ID_ANM_WIN))
        {
            spell_pos = spell_map->GetSelection();
            auto pnm = spell_map->CheckPNM();
            if(pnm)
            {
                form_anms = new FormANM(this,spell_data,true,ID_ANM_WIN);
                form_anms->SetPNM(spell_map->terrain,pnm->anim,pnm->x_ofs,pnm->y_ofs);
                form_anms->Show();
            }
        }
    }
    else if(menu_id == ID_POP_REM_SOUND)
    {
        // remove sound
        spell_map->SoundRemove();
        HistoryPush();
    }
    else if(menu_id == ID_POP_EDIT_SOUND)
    {
        // edit sound
        if(!FindWindowById(ID_SOUNDS_WIN))
        {            
            spell_pos = spell_map->GetSelection();
            form_sounds = new FormSound(this,spell_data,ID_SOUNDS_WIN);
            auto snd = spell_map->CheckSound();
            if(snd)
                form_sounds->SetSound(snd->GetName(), snd->GetType());
            form_sounds->Show();
        }
    }

    UpdateMapStatus();
}



// --- scrolling control ---
void MainFrame::OnCanvasRMouse(wxMouseEvent& event)
{    
    if(!spell_map->IsLoaded())
        return;
    if(inUnitOptions())
        return;
    
    if(event.RightDown())
        spell_map->scroller.SetRef(event.GetX(), event.GetY());
    else if(event.RightUp())
    {
        int was_moved = spell_map->scroller.Idle();
        if(!was_moved && !spell_map->isGameMode())
        {
            // --- editor mode popup menu stuff:
            auto cur_unit = spell_map->GetCursorUnit();
            auto cur_evt = spell_map->GetCursorEvent();
            auto sel_evt = spell_map->GetSelectEvent();
            auto cur_pos = spell_map->GetSelection();
            spell_pos = cur_pos;

            int wSounds = GetMenuBar()->FindItem(ID_ViewSounds)->IsChecked(); // ###todo: optimize?
            int wSoundLoops = GetMenuBar()->FindItem(ID_ViewSoundLoops)->IsChecked(); // ###todo: optimize?
            bool wSound = wSounds || wSoundLoops;
            MapSound::SoundType snd_type = MapSound::SoundType::BOTH;
            if(wSounds && wSoundLoops)
                snd_type = MapSound::SoundType::BOTH;
            else if(wSounds)
                snd_type = MapSound::SoundType::RANDOM;
            else if(wSoundLoops)
                snd_type = MapSound::SoundType::LOOP;

            wxMenu menu;
            menu.SetClientData(cur_unit);

            if(sel_evt && sel_evt->GetPosition() == cur_pos && spell_map->events->GetEventsCount(cur_pos) > 1)
            {
                menu.Append(ID_POP_ANOTHER_EVENT,"Switch to other event");
            }
            if((cur_unit && cur_unit->creator_event && cur_unit->creator_event->isMissionStart()) || cur_evt)
            {
                menu.Append(ID_POP_EDIT_EVENT,"Edit event");
            }
            if(cur_unit && cur_unit->creator_event && cur_unit->creator_event->isMissionStart())
            {                
                menu.Append(ID_POP_REM_MISSIONSTART,"Remove unit from MissionStart event");
            }
            if(cur_unit && (!cur_unit->creator_event || !cur_unit->creator_event->isMissionStart()))
            {
                auto list = spell_map->events->GetMissionStartEvent();
                if(list.size() > 1)
                {
                    wxMenu *sub_menu = new wxMenu();
                    sub_menu->SetClientData(cur_unit);
                    for(auto &evt: list)
                        sub_menu->Append(ID_POP_ADD_MISSIONSTART + evt->probability,string_format("MissionStart (p=%d%%)",evt->probability));
                    menu.AppendSubMenu(sub_menu,"Add unit to MissionStart event");
                }
                else
                {
                    menu.Append(ID_POP_ADD_MISSIONSTART+100,"Add unit to MissionStart event");
                }
            }
            if(cur_unit && !cur_unit->GetTrigEvent({SpellMapEventRec::EvtTypes::EVT_SAVE_UNIT,SpellMapEventRec::EvtTypes::EVT_TRANSPORT_UNIT,SpellMapEventRec::EvtTypes::EVT_DESTROY_UNIT}))
            {
                menu.Append(ID_POP_ADD_SAVE_UNIT,"Create SaveUnit objective");
                menu.Append(ID_POP_ADD_TRANSPORT_UNIT,"Create TransportUnit objective");
                menu.Append(ID_POP_ADD_DESTROY_UNIT,"Create DestroyUnit objective");
            }
            if(cur_unit && cur_unit->GetTrigEvent(SpellMapEventRec::EvtTypes::EVT_SAVE_UNIT))
            {
                menu.Append(ID_POP_REM_SAVE_UNIT,"Remove SaveUnit objective");
            }
            if(cur_unit && cur_unit->GetTrigEvent(SpellMapEventRec::EvtTypes::EVT_TRANSPORT_UNIT))
            {
                menu.Append(ID_POP_REM_TRANSPORT_UNIT,"Remove TransportUnit objective");
            }
            if(cur_unit && cur_unit->GetTrigEvent(SpellMapEventRec::EvtTypes::EVT_DESTROY_UNIT))
            {
                menu.Append(ID_POP_REM_DESTROY_UNIT,"Remove DestroyUnit objective");
            }
            if(cur_unit && !cur_unit->GetTrigEvent(SpellMapEventRec::EvtTypes::EVT_SEE_UNIT))
            {
                menu.Append(ID_POP_ADD_SEEUNIT,"Create SeeUnit event");
            }
            if(cur_unit && cur_unit->GetTrigEvent(SpellMapEventRec::EvtTypes::EVT_SEE_UNIT))
            {
                menu.Append(ID_POP_REM_SEEUNIT,"Remove SeeUnit event");
            }
            if(!spell_map->events->CheckEvent(SpellMapEventRec::EvtTypes::EVT_SEE_PLACE,&cur_pos))
            {
                menu.Append(ID_POP_ADD_SEE_PLACE,"Create SeePlace event");
            }
            if(spell_map->events->CheckEvent(SpellMapEventRec::EvtTypes::EVT_SEE_PLACE,&cur_pos))
            {
                menu.Append(ID_POP_REM_SEE_PLACE,"Remove SeePlace event");
            }
            if(cur_unit && sel_evt)
            {
                menu.Append(ID_POP_ADD_SPAWN_UNIT,"Add/remove unit to/from event spawn units list\tCtrl+Left Click");
            }
            if(cur_unit)
            {
                if(menu.GetMenuItemCount())
                    menu.AppendSeparator();
                menu.Append(ID_POP_EDIT_UNIT,"Edit unit");
                menu.Append(ID_POP_REM_UNIT,"Remove unit");
            }            
            if(GetMenuBar()->FindItem(ID_ViewTer)->IsChecked())
            {
                if(menu.GetMenuItemCount())
                    menu.AppendSeparator();
                auto item = menu.Append(ID_POP_EDIT_TERR,"Edit terrain sprite");
            }
            if(spell_map->CheckObj() && GetMenuBar()->FindItem(ID_ViewObj)->IsChecked())
            {
                if(menu.GetMenuItemCount())
                    menu.AppendSeparator();
                menu.Append(ID_POP_EDIT_OBJ,"Edit object");
                menu.Append(ID_POP_REM_OBJ,"Remove object");
            }
            if(spell_map->CheckANM() && GetMenuBar()->FindItem(ID_ViewAnm)->IsChecked())
            {
                if(menu.GetMenuItemCount())
                    menu.AppendSeparator();
                menu.Append(ID_POP_EDIT_ANM,"Edit ANM tile");
                menu.Append(ID_POP_REM_ANM,"Remove ANM tile");
            }
            if(spell_map->CheckPNM() && GetMenuBar()->FindItem(ID_ViewPnm)->IsChecked())
            {
                if(menu.GetMenuItemCount())
                    menu.AppendSeparator();
                menu.Append(ID_POP_EDIT_PNM,"Edit PNM tile");
                menu.Append(ID_POP_REM_PNM,"Remove PNM tile");
            }
            if(spell_map->CheckSound(NULL,snd_type) && wSound)
            {
                if(menu.GetMenuItemCount())
                    menu.AppendSeparator();
                menu.Append(ID_POP_EDIT_SOUND,"Edit sound");
                menu.Append(ID_POP_REM_SOUND,"Remove sound");
            }
                        
            
            if(menu.GetMenuItemCount())
            {
                menu.Connect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(MainFrame::OnCanvasPopupSelect),NULL,this);
                PopupMenu(&menu);
            }
        }
    }
    
    // unit view mode:
    int is_down = event.RightIsDown();
    if(is_down)
        spell_map->SetUnitRangeViewMode(SpellMap::UNIT_RANGE_MOVE);
    else
        spell_map->SetUnitRangeViewMode(SpellMap::UNIT_RANGE_NONE);
    canvas->Refresh();

}


// select all tiles
void MainFrame::OnSelectAll(wxCommandEvent& event)
{
    if(!spell_map->IsLoaded())
        return;
    spell_map->SelectTiles(SpellMap::SELECT_ADD);
}
// deselect all tiles
void MainFrame::OnDeselectAll(wxCommandEvent& event)
{
    if(!spell_map->IsLoaded())
        return;
    spell_map->SelectTiles(SpellMap::SELECT_CLEAR);
}
// select or deselect tiles
void MainFrame::OnSelectDeselect(wxCommandEvent& event)
{
    if(!spell_map->IsLoaded())
        return;
    // add/remove selection
    auto list = spell_map->GetSelections();
    spell_map->SelectTiles(list,SpellMap::SELECT_XOR);
}

// copy map selection to copy buffer
void MainFrame::OnCopyBuf(wxCommandEvent& event)
{
    if(!spell_map->IsLoaded())
        return;
    
    // get layers mask
    SpellMap::Layers lay;
    lay.lay1 = GetMenuBar()->FindItem(ID_SelectLay1)->IsChecked();
    lay.lay2 = GetMenuBar()->FindItem(ID_SelectLay2)->IsChecked();
    lay.anm = GetMenuBar()->FindItem(ID_SelectLayANM)->IsChecked();
    lay.pnm = GetMenuBar()->FindItem(ID_SelectLayPNM)->IsChecked();
    
    // get selected area (preference of persistent selection over cursor)
    std::vector<MapXY> list;
    list = spell_map->GetPersistSelections();
    if(list.empty())
        list = spell_map->GetSelections();
    
    if(event.GetId() == ID_CutBuf)
        spell_map->CutBuffer(list, lay);
    else
        spell_map->CopyBuffer(list, lay);
    
}
// clear map copy buffer
void MainFrame::OnClearBuf(wxCommandEvent& event)
{
    if(!spell_map->IsLoaded())
        return;
    // clear copy buffer and also clear tool
    spell_map->ClearBuffer();    
    wxRibbonBarEvent rev;
    OnToolPageClick(rev);

    auto* unit = spell_map->GetSelectedUnit();
    if(unit && unit->in_placement && unit->not_placed_yet)
    {
        // cancel unit in placement        
        spell_map->RemoveUnit(unit, true);
    }


    Refresh();
}
// try place copy buffer to map
void MainFrame::OnPasteBuf(wxCommandEvent& event)
{
    if(!spell_map->IsLoaded())
        return;    

    auto pos = spell_map->GetSelection();
    spell_map->PasteBuffer(spell_map->tiles,spell_map->anms,spell_map->pnms,spell_map->start,spell_map->escape,spell_map->target,spell_map->counter_attack_post_player,spell_map->counter_attack_post_enemy,pos);    
    HistoryPush();

    // optional cycling of tool items
    /*if(spell_tool.isTool())
        spell_map->SetBuffer(spell_tool,+1);*/

    Refresh();
}

// try place copy buffer to map
void MainFrame::OnChangeElevation(wxCommandEvent& event)
{
    if(!spell_map->IsLoaded())
        return;
    int step = 0;
    if(event.GetId() == ID_ElevUp)
        step++;
    else if(event.GetId() == ID_ElevDown)
        step--;
    if(step != 0)
    {
        spell_map->LockMap();
        spell_map->EditElev(step);
        spell_map->ReleaseMap();
        HistoryPush();
        Refresh();
    }
}

// invalidate map region (retexturing)
void MainFrame::OnInvalidateSelection(wxCommandEvent& event)
{
    if(!spell_map->IsLoaded())
        return;
    // get selected area (preference of persistent selection over cursor)
    std::vector<MapXY> list;    
    list = spell_map->GetPersistSelections();
    if(list.empty())
        list = spell_map->GetSelections();

    // invalidate region    
    spell_map->IvalidateTiles(list, bind(&MainFrame::StatusStringCallback,this,placeholders::_1));
}

// delete object or stuff
void MainFrame::OnDeleteSel(wxCommandEvent& event)
{
    if(!spell_map->IsLoaded())
        return;
    // get selected area (preference of persistent selection over cursor)
    std::vector<MapXY> list;
    list = spell_map->GetPersistSelections();
    if(list.empty())
        list = spell_map->GetSelections();    

    SpellMap::Layers layers;
    layers.lay1 = false;
    layers.lay2 = GetMenuBar()->FindItem(ID_ViewObj)->IsChecked();
    layers.anm = GetMenuBar()->FindItem(ID_ViewAnm)->IsChecked();
    layers.pnm = GetMenuBar()->FindItem(ID_ViewPnm)->IsChecked();
    spell_map->LockMap();
    int rem_count = spell_map->DeleteSelObjects(list,layers);
    if(!rem_count)
    {
        // nothing removed, maybe remove unit?
        rem_count = !spell_map->RemoveUnit(spell_map->GetCursorUnit(),true);
    }
    spell_map->ReleaseMap();
    if(rem_count)
        HistoryPush();
    Refresh();
}



// canvas left click
void MainFrame::OnCanvasLMouseDown(wxMouseEvent& event)
{
    if(!spell_map->IsLoaded())
        return;
    if(inUnitOptions())
        return;

    // close minimap when clicked outside
    if(form_minimap && form_minimap->form)
    {
        wxCloseEvent evt(wxEVT_CLOSE_WINDOW,ID_MINIMAP_WIN);
        wxQueueEvent(form_minimap->form,new wxCloseEvent(evt));
    }
       
    // get selection
    auto xy_list = spell_map->GetSelections();

    // store drag selection start
    m_drag_sel_start = spell_map->GetSelection();
    
    if(event.LeftDown())
    {
        // LEFT DOWN event:

        if(event.ShiftDown() && spell_tool.isActive() && xy_list.size() && xy_list[0].IsSelected())
        {
            // auto tile mapping            
            spell_map->EditClass(xy_list,&spell_tool,bind(&MainFrame::StatusStringCallback,this,placeholders::_1));
            HistoryPush();
        }
        else if(spell_map->isCopyBufferFull())
        {
            // something in copy buffer            
            auto pos = spell_map->GetSelection();
            spell_map->PasteBuffer(spell_map->tiles,spell_map->anms,spell_map->pnms,spell_map->start,spell_map->escape,spell_map->target,spell_map->counter_attack_post_player,spell_map->counter_attack_post_enemy,pos);
            // optional cycling of tool items
            if(event.ControlDown() && spell_tool.isTool())
                spell_map->SetBuffer(spell_tool,+1);
            HistoryPush();
            Refresh();
        }
        else
        {
            // try select/move stuff:
            int wPnms = GetMenuBar()->FindItem(ID_ViewPnm)->IsChecked(); // ###todo: optimize?
            int wAnms = GetMenuBar()->FindItem(ID_ViewAnm)->IsChecked(); // ###todo: optimize?
            int wEvents = GetMenuBar()->FindItem(ID_ViewEvents)->IsChecked(); // ###todo: optimize?
            int wSounds = GetMenuBar()->FindItem(ID_ViewSounds)->IsChecked(); // ###todo: optimize?
            int wSoundLoops = GetMenuBar()->FindItem(ID_ViewSoundLoops)->IsChecked(); // ###todo: optimize?
            bool wSound = wSounds || wSoundLoops;
            MapSound::SoundType snd_type = MapSound::SoundType::BOTH;
            if(wSounds && wSoundLoops)
                snd_type = MapSound::SoundType::BOTH;
            else if(wSounds)
                snd_type = MapSound::SoundType::RANDOM;
            else if(wSoundLoops)
                snd_type = MapSound::SoundType::LOOP;

            select_pos = spell_map->GetSelection(NULL);
            sel_unit = spell_map->GetSelectedUnit();
            cur_unit = spell_map->GetCursorUnit();
            auto cur_evt = spell_map->GetCursorEvent();
            auto sel_evt = spell_map->GetSelectEvent();            
            auto cur_sound = spell_map->CheckSound(NULL,snd_type);
            auto sel_sound = spell_map->SoundSelected();
            auto cur_pnm = spell_map->CheckPNM();
            auto sel_pnm = spell_map->SelectedPNM();
            auto cur_anm = spell_map->CheckANM();
            auto sel_anm = spell_map->SelectedANM();
            if(!spell_map->isGameMode())
            {
                if(wAnms && cur_anm && cur_anm == sel_anm)
                {
                    // move ANM
                    sel_anm->in_placement = !sel_anm->in_placement;
                    if(!sel_anm->in_placement)
                        HistoryPush();
                }
                else if(wAnms && cur_anm)
                {
                    // select ANM
                    spell_map->SelectANM(cur_anm);
                }
                else if(wPnms && cur_pnm && cur_pnm == sel_pnm)
                {
                    // move PNM
                    sel_pnm->in_placement = !sel_pnm->in_placement;
                    if(!sel_pnm->in_placement)
                        HistoryPush();
                }
                else if(wPnms && cur_pnm)
                {
                    // select PNM
                    spell_map->SelectPNM(cur_pnm);
                }
                else if(wSound && cur_sound && cur_sound == sel_sound)
                {
                    // move sound
                    if(sel_sound->in_placement)
                    {
                        // remap sound positions when mover released
                        spell_map->sounds->InitSounds();
                        spell_map->sounds->UpdateMaps();
                    }
                    sel_sound->in_placement = !sel_sound->in_placement;
                    if(!sel_sound->in_placement)
                        HistoryPush();
                }
                else if(wSound && cur_sound)
                {
                    // select sound
                    spell_map->SoundSelect(cur_sound);
                }
                else if(wEvents && sel_evt && cur_unit && event.ControlDown())
                {
                    // try add/remove unit to event
                    spell_map->UpdateEventUnit(sel_evt, cur_unit);
                    HistoryPush();
                }
                else if(wEvents && sel_evt && sel_evt->position == select_pos)
                {
                    // move/place event
                    if(sel_unit)
                        sel_unit->in_placement = false;
                    sel_evt->in_placement = !sel_evt->in_placement;
                    if(!sel_evt->in_placement)
                        HistoryPush();
                }
                else if(wEvents && cur_evt && !cur_evt->isMissionStart())
                {
                    // select event
                    spell_map->SelectEvent(cur_evt);
                }
                else if(wEvents && cur_unit && cur_unit->GetTrigEvent(SpellMapEventRec::EvtTypes::EVT_SEE_UNIT))
                {
                    // select SeeUnit() event
                    spell_map->SelectEvent(cur_unit->GetTrigEvent(SpellMapEventRec::EvtTypes::EVT_SEE_UNIT));
                }
                else if(cur_unit && cur_unit == sel_unit)
                {
                    // move/place unit
                    bool was_new_unit = sel_unit->not_placed_yet;

                    if(sel_evt)
                        sel_evt->in_placement = false;
                    if(sel_unit->in_placement)
                        sel_unit->not_placed_yet = false;
                    sel_unit->in_placement = !sel_unit->in_placement;

                    if(was_new_unit)
                    {
                        // was placement of new unit: fork unit and place again
                        auto new_unit = new MapUnit(*sel_unit);
                        new_unit->in_placement = true;
                        new_unit->is_active = true;
                        new_unit->ResetAP();
                        new_unit->not_placed_yet = true;
                        auto pos = spell_map->GetSelection();
                        if(pos.IsSelected())
                            new_unit->coor = pos;
                        else
                            new_unit->coor = MapXY(0,0);
                        if(new_unit->is_event)
                        {
                            // event unit - place to MissionStart
                            spell_map->events->AddMissionStartUnit(new_unit);
                        }
                        else
                        {
                            // normal unit - place to map
                            spell_map->AddUnit(new_unit);
                        }
                        spell_map->SelectUnit(new_unit);
                    }

                    if(!sel_unit->in_placement)
                        HistoryPush();
                }
                else if(cur_unit)
                {
                    // try select unit (if on cursor)
                    spell_map->SelectUnit(cur_unit);
                }
            }
            else
            {
                // game mode:
                int options = spell_map->GetUnitOptions();
                
                // reduce attack options if only one target is possible
                if(!!(options & SpellMap::UNIT_OPT_LOWER) != !!(options & SpellMap::UNIT_OPT_UPPER))
                    options = (options & ~(SpellMap::UNIT_OPT_LOWER | SpellMap::UNIT_OPT_LOWER)) | SpellMap::UNIT_OPT_ATTACK;

                if(options)
                {
                    // show optional menu (or directly call callback) to resolve options
                    wxPoint pos = event.GetPosition();
                    pos.x -= 15;
                    pos.y -= 15;
                    form_unit_opts = new FormUnitOpts(canvas,ID_UNIT_MODE_WIN,pos,spell_data,options,bind(&MainFrame::OnUnitClick_cb,this,placeholders::_1));
                }
            }
        }
    }

    UpdateMapStatus();
    canvas->Refresh();
}

// canvas left up (end of drag selection)
void MainFrame::OnCanvasLMouseUp(wxMouseEvent& event)
{
    auto m_drag_sel_end = spell_map->GetSelection();
    if(m_drag_sel_start == m_drag_sel_end)
        return;

    spell_map->scroller.SetDragSelectRange(m_drag_sel_start,m_drag_sel_end);

    auto xy_list = spell_map->GetSelections();
    if(spell_tool.isActive() && spell_tool.isTool() && xy_list.size() > 1 && xy_list[0].IsSelected() && spell_map->scroller.isDragSelect())
    {
        if(event.ShiftDown())
        {
            // auto tile mapping            
            spell_map->EditClass(xy_list,&spell_tool,bind(&MainFrame::StatusStringCallback,this,placeholders::_1));
            HistoryPush();
            Refresh();
        }
        else if(spell_map->isCopyBufferFull())
        {
            // something in copy buffer            
            //spell_map->SetBuffer(spell_tool);
            auto pos_list = spell_map->GetSelections();
            // remove reference item because it was already placed by mouse-down event
            if(!pos_list.empty())
                pos_list.erase(pos_list.end() - 1);
            spell_map->scroller.ResizeSelection(0);
            for(auto pos: pos_list)
            {
                if(event.ControlDown())
                    spell_map->SetBuffer(spell_tool,0);
                spell_map->PasteBuffer(spell_map->tiles,spell_map->anms,spell_map->pnms,spell_map->start,spell_map->escape,spell_map->target,spell_map->counter_attack_post_player,spell_map->counter_attack_post_enemy,pos,false);
            }
            //spell_map->SetBuffer(spell_tool);
            HistoryPush();
            Refresh();
        }
    }
    m_drag_sel_start.Clear();
    spell_map->scroller.ResizeSelection(0);

    UpdateMapStatus();
    canvas->Refresh();
}

// on canvas mouse enter
void MainFrame::OnCanvasMouseEnter(wxMouseEvent& event)
{
    if(inSubForm())
        return;

    canvas->SetFocus();
}
// on canvas mouse leave
void MainFrame::OnCanvasMouseLeave(wxMouseEvent& event)
{
    SetCursor(*wxSTANDARD_CURSOR);

    if(!spell_map->IsLoaded())
        return;
    if(inUnitOptions())
        return;

    m_drag_sel_start.Clear();
    spell_map->scroller.ResizeSelection(0);

    spell_map->SetUnitRangeViewMode(SpellMap::UNIT_RANGE_NONE);
    spell_map->scroller.Idle();
}
// on canvas mouse move
void MainFrame::OnCanvasMouseMove(wxMouseEvent& event)
{
    if(!spell_data)
        return;
    if(!spell_map->IsLoaded())
        return;
    if(inUnitOptions())
        return;

    

    static int last_in_hud = false;

    int hud_top = spell_map->GetHUDtop(event.GetX());
    if(event.GetY() >= hud_top)
    {
        // mouse in HUD area - kill scroll
        spell_map->SetUnitRangeViewMode(SpellMap::UNIT_RANGE_NONE);
        if(!last_in_hud)
            spell_map->InvalidateHUDbuttons();
        spell_map->scroller.Idle();
        last_in_hud = true;

        // invalidate cursor
        spell_map->ClearSelections();
        m_drag_sel_start.Clear();

        // default game cursor
        SetCursor(*spell_data->gres.cur_pointer);
    }
    else
    {
        spell_map->scroller.Move(event.GetX(),event.GetY());
        last_in_hud = false;

        // resolve cursor
        auto options = spell_map->GetUnitOptions();
        wxCursor* cur = spell_data->gres.cur_pointer;
        if(!spell_map->isGameMode())
            cur = spell_data->gres.cur_pointer;
        else if(!options)
            cur = spell_data->gres.cur_pointer;
        else if(options == SpellMap::UNIT_OPT_MOVE)
            cur = spell_data->gres.cur_move;
        else if(options == SpellMap::UNIT_OPT_SELECT)
            cur = spell_data->gres.cur_select;
        else if(options == SpellMap::UNIT_OPT_LOWER)
            cur = spell_data->gres.cur_attack_down;
        else if(options == SpellMap::UNIT_OPT_UPPER)
            cur = spell_data->gres.cur_attack_up;
        else if(options == (SpellMap::UNIT_OPT_UPPER | SpellMap::UNIT_OPT_LOWER))
            cur = spell_data->gres.cur_attack_up_down;
        else
            cur = spell_data->gres.cur_question;
        SetCursor(*cur);
    }


    // update map selection
    MapXY mxy = spell_map->GetSelection();
    int elev = spell_map->GetElevation();
    SetStatusText(wxString::Format(wxT("x=%d"),mxy.x),0);
    SetStatusText(wxString::Format(wxT("y=%d"),mxy.y),1);
    SetStatusText(wxString::Format(wxT("z=%d"),elev),2);
    SetStatusText(wxString::Format(wxT("xy=%d"),spell_map->ConvXY(mxy)),3);
    SetStatusText(wxString::Format(wxT("L1: %s"),spell_map->GetL1tileName()),4);
    SetStatusText(wxString::Format(wxT("L2: %s"),spell_map->GetL2tileName()),5);
    //int height, flags, code;
    auto [flags,height,code] = spell_map->GetTileFlags();
    SetStatusText(wxString::Format(wxT("(0x%02X)"),code),6);    

    auto sel_evt = spell_map->GetSelectEvent();
    auto* unit = spell_map->GetSelectedUnit();
    auto sel_sound = spell_map->SoundSelected();
    auto* sel_pnm = spell_map->SelectedPNM();
    auto* sel_anm = spell_map->SelectedANM();
    if(sel_anm && sel_anm->in_placement && mxy.IsSelected())
    {
        // change ANM position
        spell_map->MoveANM(sel_anm,mxy);
    }
    else if(sel_pnm && sel_pnm->in_placement && mxy.IsSelected())
    {
        // change PNM position
        spell_map->MovePNM(sel_pnm,mxy);
    }
    else if(sel_sound && sel_sound->in_placement && mxy.IsSelected())
    {
        // change sound position
        spell_map->SoundMove(sel_sound,mxy,false);
    }
    else if(sel_evt && sel_evt->in_placement && mxy.IsSelected())
    {
        // change event position
        sel_evt->position = mxy;
        spell_map->events->ResetEvents();
    }
    else if(unit && unit->in_placement && mxy.IsSelected())
    {
        // change unit position
        if(unit->coor != mxy)
            unit->was_moved = true;
        unit->coor = mxy;

        if(unit->was_moved)
            spell_map->unit_view->AddUnitView(unit,
                spell_map->isUnitsViewDebugMode()?(SpellMap::ViewRange::ClearMode::HIDE):(SpellMap::ViewRange::ClearMode::NONE));
    }


    if(event.LeftIsDown())
    {
        if(/*spell_map->scroller.GetSize() == 1 && */m_drag_sel_start.IsSelected() && m_drag_sel_start != mxy)
        {
            spell_map->scroller.SetDragSelectRange(m_drag_sel_start,mxy);
            //auto [da,db] = spell_map->scroller.GetDragSelectionSize();
            //SetStatusText(wxString::Format(wxT("da=%d db=%d"),da,db),7);
        }
    }

    canvas->Refresh();
    //event.Skip();
}
// on canvas wheel
void MainFrame::OnCanvasMouseWheel(wxMouseEvent& event)
{
    int delta = event.GetWheelRotation()/event.GetWheelDelta();
    if(event.ControlDown())
    {
        // with CTRL: cycle objects within tool
        if(spell_tool.isTool())
        {
            spell_map->SetBuffer(spell_tool,delta);
            canvas->Refresh();
        }
    }
    else
    {
        // no key: change selection size
        spell_map->scroller.ResizeSelection(delta);
        if(spell_tool.isTool())
            spell_map->SetBuffer(spell_tool,delta);
        canvas->Refresh();
    }
}
// on canvas key down
void MainFrame::OnCanvasKeyDown(wxKeyEvent& event)
{
    int key = event.GetKeyCode();
    if(event.ControlDown())
    {
    }
}


void MainFrame::OnUnitClick_cb(int option)
{
    if(option & SpellMap::UNIT_OPT_SELECT)
    {
        // select unit
        auto unit = spell_map->CanSelectUnit(select_pos);
        spell_map->SelectUnit(unit);
    }
    else if(option & SpellMap::UNIT_OPT_MOVE)
    {
        // move
        spell_map->MoveUnit(select_pos);
    }
    else if(option & SpellMap::UNIT_OPT_ATTACK || option & SpellMap::UNIT_OPT_LOWER || option & SpellMap::UNIT_OPT_UPPER)
    {
        // attack (unit or object)
        int is_upper = option & SpellMap::UNIT_OPT_UPPER;
        spell_map->Attack(select_pos, is_upper);
    }
}


//--------------------------------------------------------------------------------------------------------------------
// Message display stuff
//--------------------------------------------------------------------------------------------------------------------
// show message function wrapper
void MainFrame::ShowMessage(SpellTextRec *message,bool is_yesno,std::function<void(bool)> exit_cb)
{
    //auto text = spell_data->texts->GetText("u0101_07");
    form_message = new FormMsgBox(canvas, ID_MSG_WIN, spell_data, spell_map, message, (is_yesno)?(FormMsgBox::SpellMsgOptions::YESNO):(FormMsgBox::SpellMsgOptions::NONE), exit_cb);
}
// return true if some message still exist
bool MainFrame::CheckMessageState()
{
    return(form_message != NULL);    
}


//--------------------------------------------------------------------------------------------------------------------
// Tool bar stuff
//--------------------------------------------------------------------------------------------------------------------
// tool selected
void MainFrame::OnToolBtnClick(wxRibbonButtonBarEvent& event)
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
            if(id == btn_id)
            {            
                // this button (event caller):
                
                // some tool selected: setup tool pointer
                SpellObject *obj = (SpellObject*)btns->GetItemClientData(btns->GetItemById(btn_id));
                if(obj)
                {
                    // tool is object
                    // unset tool (depreceted method)
                    spell_tool.Set();
                    // place tool to clipboard (new method)
                    spell_map->SetBuffer(obj);
                }
                else
                {
                    spell_tool.Set(tid, iid); // tool is class
                    if(spell_tool.isTool())
                        spell_map->SetBuffer(spell_tool,0);
                }               
            }
        }
    }
}
// tool drop down selected
void MainFrame::OnToolBtnDropClick(wxRibbonButtonBarEvent& event)
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

            if(id == btn_id)
            {
                // this button (event caller):
                SpellObject* obj = (SpellObject*)btns->GetItemClientData(btns->GetItemById(btn_id));
                if(obj)
                {
                    // tool is object

                }
                else
                {
                    spell_tool.Set(tid,iid); // tool is class
                    
                    wxMenu menu;
                    //menu.SetClientObject(new SpellTool(spell_tool));
                    auto list = spell_map->terrain->GetToolSprites(spell_tool);
                    int item_id = 0;
                    for(auto item: list)
                    {
                        auto mmi = menu.Append(item_id++,item->name);
                        auto bmp = item->Render((uint8_t*)spell_map->terrain->pal,1.3,64,64,true);
                        auto img = bmp->ConvertToImage(); // this does something to alpha channel so it is correctly recognized by menu
                        mmi->SetBitmap(img);
                        delete bmp;
                    }
                    auto obj_list = spell_map->terrain->GetToolObjects(spell_tool);
                    for(auto item: obj_list)
                    {
                        auto mmi = menu.Append(item_id++,item->GetDescription());
                        auto bmp = item->RenderPreview(1.3,128,64,true);
                        auto img = bmp->ConvertToImage(); // this does something to alpha channel so it is correctly recognized by menu
                        mmi->SetBitmap(img);
                        delete bmp;
                    }

                    if(menu.GetMenuItemCount())
                    {
                        menu.Connect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(MainFrame::OnToolItemPopupSelect),NULL,this);
                        PopupMenu(&menu);
                    }
                    
                }
            }
        }
    }
}
// on tool item pupup selection
void MainFrame::OnToolItemPopupSelect(wxCommandEvent& event)
{
    if(!spell_map->IsLoaded())
        return;

    auto menu_id = event.GetId();
    auto menu = (wxMenu*)event.GetEventObject();
    if(!spell_tool.isActive() || !spell_tool.isTool())
        return;

    spell_map->SetBuffer(spell_tool,0,menu_id);
}

// tool page selected
void MainFrame::OnToolPageClick(wxRibbonBarEvent& event)
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
void MainFrame::LoadToolsetRibbon(Terrain *terr)
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
            vector<bool> multi;
            int item_id = 0;
            for (; item_id < terr->GetToolSetItemsCount(tool_id); item_id++)
            {
                SpellTool spell_tool;
                spell_tool.Set(tool_id,item_id);
                int has_multi = terr->GetToolSprites(spell_tool).size() + terr->GetToolObjects(spell_tool).size();
                titles.push_back(terr->GetToolSetItem(tool_id, item_id)/* + ((has_multi > 1)?" [+]":"")*/);
                multi.push_back(has_multi > 1);
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
                multi.push_back(false);
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
                //auto btn = ribBtns->AddButton(index[k], titles[k], *bmp, wxEmptyString, wxRIBBON_BUTTON_TOGGLE);
                auto btn = ribBtns->AddButton(index[k],titles[k],*bmp,wxEmptyString,multi[k]?wxRIBBON_BUTTON_HYBRID:wxRIBBON_BUTTON_NORMAL);
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
FormGamma::FormGamma(wxFrame* parent,SpellMap* map,wxWindowID id) :wxDialog(parent,wxID_ANY,"Gamma correction",wxDefaultPosition,wxSize(400,80),wxDEFAULT_FRAME_STYLE|wxSTAY_ON_TOP)
{
    // store local reference to initial map and data
    spell_map = map;

    // make slider
    slider = new wxSlider(this,wxID_ANY,spell_map->GetGamma()*1000,500,2000);
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(slider,1,wxEXPAND|wxALL);
    this->SetSizer(sizer);
    this->SetAutoLayout(true); 
    this->Center();
    this->Bind(wxEVT_CHAR_HOOK,&FormGamma::OnExit,this);
    SetMinSize(wxSize(400,-1));
    this->Fit();
    
    Bind(wxEVT_COMMAND_SLIDER_UPDATED,&FormGamma::OnChangeGamma, this);
    Bind(wxEVT_CLOSE_WINDOW,&FormGamma::OnClose,this,id);

}
void FormGamma::OnChangeGamma(wxCommandEvent& event)
{
    double gamma = 0.001*(double)slider->GetValue();
    SetTitle(wxString::Format(wxT("Gamma correction = %#0.2f"),gamma));
    spell_map->SetGamma(gamma);
}
void FormGamma::OnClose(wxCloseEvent& ev)
{
    wxPostEvent(GetParent(),ev);
    ev.Skip();
    Destroy();
}
void FormGamma::OnExit(wxKeyEvent& event)
{
    if(event.GetKeyCode()==WXK_ESCAPE || event.GetKeyCode()==WXK_RETURN || event.GetKeyCode()==WXK_NUMPAD_ENTER)
        this->Close();
    else
        event.Skip();
}


//--------------------------------------------------------------------------------------------------------------------
// Select terrain Dialog
//--------------------------------------------------------------------------------------------------------------------
FormTerrain::FormTerrain(wxFrame* parent,SpellData* data,wxWindowID id) :wxDialog(parent,wxID_ANY,"Terrain selection",wxDefaultPosition,wxSize(300,150),wxDEFAULT_FRAME_STYLE|wxSTAY_ON_TOP)
{
    // store local reference to initial map and data
    m_spell_data = data;

    // make slider
    wxStaticText *txt = new wxStaticText(this, wxID_ANY, "Select terrain type:");    
    terr_choice = new wxChoice(this, wxID_TERR_CHB);
    btn_ok = new wxButton(this, wxID_OK_BTN,"OK");
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(txt,0,wxEXPAND|wxTOP|wxLEFT|wxRIGHT,5);
    sizer->Add(terr_choice,0,wxEXPAND|wxBOTTOM|wxLEFT|wxRIGHT,5);
    sizer->Add(btn_ok,0,wxEXPAND|wxALL,5);
    this->SetSizer(sizer);
    this->SetAutoLayout(true);
    this->Center();
    SetMinSize(wxSize(300,-1));
    this->Fit();

    // assign button shortcuts
    std::vector<wxAcceleratorEntry> entries;
    entries.emplace_back(wxACCEL_NORMAL,WXK_RETURN,wxID_OK_BTN);
    entries.emplace_back(wxACCEL_NORMAL,WXK_NUMPAD_ENTER,wxID_OK_BTN);
    wxAcceleratorTable accel(entries.size(),entries.data());
    this->SetAcceleratorTable(accel);

    this->Bind(wxEVT_CHAR_HOOK,&FormTerrain::OnExit,this);

    Bind(wxEVT_CLOSE_WINDOW,&FormTerrain::OnClose,this,id);
    Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormTerrain::OnOK,this,wxID_OK_BTN);

    for(int k = 0; k < m_spell_data->GetTerrainCount(); k++)
        terr_choice->Append(m_spell_data->GetTerrain(k)->name);
    terr_choice->Select(0);
}

void FormTerrain::OnClose(wxCloseEvent& ev)
{
}
void FormTerrain::OnOK(wxCommandEvent& ev)
{
    EndModal(wxID_OK);
}
std::string FormTerrain::GetTerrain()
{
    return(terr_choice->GetStringSelection().ToStdString());
}
void FormTerrain::OnExit(wxKeyEvent& event)
{
    if(event.GetKeyCode()==WXK_RETURN)
        EndModal(wxID_OK);
    else if(event.GetKeyCode()==WXK_ESCAPE)
        EndModal(wxID_CANCEL);
}
