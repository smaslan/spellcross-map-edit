//=============================================================================
// Spellcross Map Editor
// ----------------------------------------------------------------------------
// Top level functions, wxWidgets GUI.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021-2025, Stanislav Maslan, s.maslan@seznam.cz
// url: https://github.com/smaslan/spellcross-map-edit
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
// 
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
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

#include "main.h"
#include "other.h"
#include "simpleini.h"
#include "spellcross.h"
#include "map.h"




wxIMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
    // for saving PNG file (among other stuff)
    wxInitAllImageHandlers();
    
    // load config.ini
    ini.SetUnicode();
    ini.LoadFile("config.ini");

    // --- try load Spellcross data
    FormLoader* form_loader = new FormLoader(NULL,spell_data,L"config.ini");
    bool data_ok = form_loader->ShowModal();
    delete form_loader;
    if(!data_ok)
    {
        wxMessageBox("Loading Spellcross data failed!\nPossibly incorrect game paths in ''config.ini''?","Error",wxICON_ERROR);
        return(false);
    }

    // --- load some map
    wstring map_path = char2wstring(ini.GetValue("STATE","last_map",""));
    spell_map = new SpellMap();
    if(spell_map->Load(map_path,spell_data))
        wxMessageBox(string_format("Loading Spellcross map file failed with error:\n%s",spell_map->GetLastError().c_str()),"Error",wxICON_ERROR);
    spell_map->SetGamma(1.3);

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
    MainFrame* frame = new MainFrame(spell_map, spell_data);
    frame->SetSize(win_x_size,win_y_size);
    if(win_maximize)
        frame->Maximize();
    frame->Center();
    // show main frame
    frame->Show(true);
    return(true);
}
int MyApp::OnExit()
{
    // store last path
    ini.SetValue("STATE","last_map",wstring2string(spell_map->GetTopPath()).c_str());

    // store sound/midi volumes
    ini.SetLongValue("STATE", "sound_volume", 100.0*spell_data->sounds->channels->GetVolume());
    ini.SetLongValue("STATE", "music_volume", 100.0*spell_data->midi->GetVolume());

    // save INI
    ini.SaveFile("config.ini");

    // loose map
    delete spell_map;

    // loose spell data
    delete spell_data;

    return(0);
}

// Main panel init
MainFrame::MainFrame(SpellMap* map, SpellData* spelldata):wxFrame(NULL, wxID_ANY, "Spellcross Map Editor", wxDefaultPosition, wxSize(1600,1000))
{
    // store local reference to initial map and data
    spell_map = map;
    spell_data = spelldata;

    // subforms
    form_objects = NULL;
    form_sprites = NULL;
    form_gres = NULL;
    form_units = NULL;
    form_pal = NULL;
    form_tools = NULL;
    form_unit_opts = NULL;
    form_message = NULL;
    form_map_options = NULL;
    form_videos = NULL;
    form_video_box = NULL;
    form_midi = NULL;
    form_minimap = NULL;
    form_units_list = NULL;
        
    // File menu
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_OpenMap, "&Open Map\tCtrl-O", "Open new Spellcross map file.");
    menuFile->Append(ID_SaveMap,"&Save Map\tCtrl-S","Save Spellcross map file(s).");
    menuFile->Append(ID_SaveDTA,"&Save DTA map file","Save Spellcross map DTA file.");
    menuFile->Append(ID_SaveDEF,"&Save DEF map file","Save Spellcross map DEF file.");
    menuFile->Append(ID_NewMap,"&Ceate new Map\tCtrl-N","Create new map.");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

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
    menuView->Append(ID_ViewUnt,"Layer 5: Units\tF5","",wxITEM_CHECK);
    menuView->FindItem(ID_ViewUnt)->Check(true);
    menuView->Append(ID_ViewStTa,"Layer 6: Start/Target\tF6","",wxITEM_CHECK);
    menuView->FindItem(ID_ViewStTa)->Check(true);
    menuView->Append(ID_ViewSoundLoops,"Layer 7: Sound loops\tF7","",wxITEM_CHECK);
    menuView->FindItem(ID_ViewSoundLoops)->Check(false);
    menuView->Append(ID_ViewSounds,"Layer 8: Sounds\tF8","",wxITEM_CHECK);
    menuView->FindItem(ID_ViewSounds)->Check(false);
    menuView->Append(ID_ViewEvents,"Show events\tF9","",wxITEM_CHECK);
    menuView->FindItem(ID_ViewEvents)->Check(false);    
    menuView->Append(ID_HighlighObj,"Highlight objects\tF10","",wxITEM_CHECK);
    menuView->FindItem(ID_HighlighObj)->Check(false);
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
    wxMenu* menuEdit = new wxMenu;
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
    menuEdit->Append(ID_InvalidateSel,"Invalidate selection\tCtrl+I","",wxITEM_NORMAL);
    menuEdit->Append(ID_DeleteSel,"Delete stuff\tCtrl+Delete","",wxITEM_NORMAL);
    menuEdit->Append(wxID_ANY,"","",wxITEM_SEPARATOR);
    menuEdit->Append(ID_ElevUp,"Elevate terrain\tCtrl+PageUp","",wxITEM_NORMAL);
    menuEdit->Append(ID_ElevDown,"Lower terrain\tCtrl+PageDown","",wxITEM_NORMAL);
    menuEdit->Append(wxID_ANY,"","",wxITEM_SEPARATOR);
    menuEdit->Append(ID_CreateNewObject,"Create new object\tCtrl+Shift+O","",wxITEM_NORMAL);
    menuEdit->Append(wxID_ANY,"","",wxITEM_SEPARATOR);
    menuEdit->Append(ID_AddUnit,"Add unit\tCtrl+Shift+U","",wxITEM_NORMAL);


    
    // tools
    wxMenu* menuTools = new wxMenu;
    menuTools->Append(ID_ViewSprites,"Sprites viewer","",wxITEM_NORMAL);
    menuTools->Append(ID_ViewAnms,"Animations (ANM) viewer","",wxITEM_NORMAL);
    menuTools->Append(ID_ViewPnms,"Animations (PNM) viewer","",wxITEM_NORMAL);
    menuTools->Append(ID_SoundsViewer,"Sounds viewer","",wxITEM_NORMAL);
    menuTools->Append(ID_ViewObjects,"Objects editor","",wxITEM_NORMAL);
    menuTools->Append(ID_ViewTools, "Tools editor", "", wxITEM_NORMAL);
    menuTools->Append(ID_ViewPal,"Palette viewer","",wxITEM_NORMAL);
    menuTools->Append(ID_ViewGRes,"Graphics viewer","",wxITEM_NORMAL);
    menuTools->Append(ID_EditUnit,"Units viewer/editor\tCtrl+U","",wxITEM_NORMAL);
    menuTools->Append(ID_EditEvent,"Event viewer/editor\tCtrl+E","",wxITEM_NORMAL);
    menuTools->Append(ID_ViewVideo,"Video viewer","",wxITEM_NORMAL);
    menuTools->Append(ID_ViewMIDI,"MIDI player","",wxITEM_NORMAL);
    menuTools->Append(ID_EditTileFlags,"Edit tile flags\tCtrl+F","",wxITEM_NORMAL);
    menuTools->Append(wxID_ANY,"","",wxITEM_SEPARATOR);
    menuTools->Append(ID_ViewMiniMap,"View mini-map","",wxITEM_NORMAL);
    menuTools->Append(ID_ViewVoxZ,"View Z-map","",wxITEM_NORMAL);
    menuTools->Append(ID_ExportVoxZ,"Export Z-map","",wxITEM_NORMAL);
    menuTools->Append(wxID_ANY,"","",wxITEM_SEPARATOR);
    menuTools->Append(ID_UpdateSprContext, "Update tile context from map","",wxITEM_NORMAL);
    menuTools->Append(ID_UpdateSprContextMaps,"Update tile context from ALL maps","",wxITEM_NORMAL);
    
        
    // Help menu
    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    
    // Main menu
    wxMenuBar* menuBar = new wxMenuBar;    
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuGame, "&Game");
    menuBar->Append(menuEdit, "&Edit");
    menuBar->Append(menuView, "&View");
    menuBar->Append(menuTools,"&Tools");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);
    
    CreateStatusBar(8);
    const int ss_w[] = {45,45,45,60,100,100,100,-1};
    SetStatusWidths(8,ss_w);
    SetStatusText("");
      
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
    //canvas->Bind(wxEVT_LEFT_DCLICK,&MainFrame::OnCanvasLMouseDown,this);
    canvas->Bind(wxEVT_THREAD,&MainFrame::OnThreadCanvas,this);
    

    this->SetSizer(sizer);    
    this->SetAutoLayout(true);
    this->Layout();

    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);
    
    Bind(wxEVT_MENU,&MainFrame::OnOpenMap,this,ID_OpenMap);
    Bind(wxEVT_MENU,&MainFrame::OnSaveMap,this,ID_SaveMap);
    Bind(wxEVT_MENU,&MainFrame::OnSaveDTA,this,ID_SaveDTA);
    Bind(wxEVT_MENU,&MainFrame::OnSaveDEF,this,ID_SaveDEF);
    Bind(wxEVT_MENU,&MainFrame::OnNewMap,this,ID_NewMap);
    Bind(wxEVT_MENU,&MainFrame::OnAbout, this, wxID_ABOUT);
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
    Bind(wxEVT_MENU,&MainFrame::OnViewLayer,this,ID_ViewHUD);
    Bind(wxEVT_MENU,&MainFrame::OnViewLayer,this,ID_ViewSounds);
    Bind(wxEVT_MENU,&MainFrame::OnViewLayer,this,ID_ViewSoundLoops);
    Bind(wxEVT_MENU,&MainFrame::OnViewLayer,this,ID_ViewEvents);
    Bind(wxEVT_MENU,&MainFrame::OnViewLayer,this,ID_HighlighObj);

    Bind(wxEVT_MENU,&MainFrame::OnSetGamma,this,ID_SetGamma);
    Bind(wxEVT_MENU,&MainFrame::OnViewSprites,this,ID_ViewSprites);
    Bind(wxEVT_MENU,&MainFrame::OnViewAnms,this,ID_ViewAnms);
    Bind(wxEVT_MENU,&MainFrame::OnViewPnms,this,ID_ViewPnms);
    Bind(wxEVT_MENU,&MainFrame::OnViewSounds,this,ID_SoundsViewer);
    Bind(wxEVT_MENU,&MainFrame::OnViewObjects,this,ID_ViewObjects);
    Bind(wxEVT_MENU,&MainFrame::OnViewTools, this, ID_ViewTools);
    Bind(wxEVT_MENU,&MainFrame::OnViewPal,this,ID_ViewPal);
    Bind(wxEVT_MENU,&MainFrame::OnViewGrRes,this,ID_ViewGRes);
    Bind(wxEVT_MENU,&MainFrame::OnEditUnit,this,ID_EditUnit);
    Bind(wxEVT_MENU,&MainFrame::OnEditEvent,this,ID_EditEvent);
    Bind(wxEVT_MENU,&MainFrame::OnViewVideo,this,ID_ViewVideo);
    Bind(wxEVT_MENU,&MainFrame::OnViewMidi,this,ID_ViewMIDI);
    Bind(wxEVT_MENU,&MainFrame::OnTileFlags,this,ID_EditTileFlags);
    Bind(wxEVT_MENU,&MainFrame::OnViewVoxZ,this,ID_ViewVoxZ);
    Bind(wxEVT_MENU,&MainFrame::OnViewVoxZ,this,ID_ExportVoxZ);
    Bind(wxEVT_MENU,&MainFrame::OnViewMiniMap,this,ID_ViewMiniMap);
    Bind(wxEVT_MENU,&MainFrame::OnUnitViewDebug,this,ID_UnitViewDbg);
    Bind(wxEVT_MENU,&MainFrame::OnUpdateTileContext,this,ID_UpdateSprContext);
    Bind(wxEVT_MENU,&MainFrame::OnUpdateTileContextMaps,this,ID_UpdateSprContextMaps);
    
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
    
    // main sizer 
    /*auto sizer2 = new wxBoxSizer(wxVERTICAL);

    wxButton *btnOk = new wxButton(canvas,wxID_ANY,wxT("TEST"),wxDefaultPosition,wxDefaultSize,0);
    sizer2->Add(btnOk,0,wxALL,5);

    canvas->SetSizer(sizer2);
    canvas->SetAutoLayout(true);
    canvas->Layout();*/

    //SetCursor(spelldata->gres.GetResource("DOJAZD.CUR")->Render(true)->ConvertToImage());
    
    //SetCursor(*spelldata->gres.ico_attack_up_down);

    /*SpellTextRec text("Experimental text message", SpellLang::CZE);
    ShowMessage(&text,true);*/
    
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
// callback function to write status messages from within the spellcross routines:
// usage: make and pass callback pointer using: bind(&MainFrame::StatusStringCallback,this,placeholders::_1)
// spellcross function example:
// void whatever_function(std::function<void(std::string)> status_cb)
// {
//   status_cb("Some message");
// }
void MainFrame::StatusStringCallback(std::string info)
{
    SetStatusText(info,7);
}

void MainFrame::OnClose(wxCloseEvent& ev)
{
    if(ev.GetId() == ID_GAMMA_WIN)
    {
        form_gamma->Destroy();
    }
    else if (ev.GetId() == ID_TOOLS_WIN)
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
                // edit existing map anim
                spell_map->SoundEdit(snd,snd_type,&spell_pos);
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
    else if(ev.GetId() == ID_MINIMAP_WIN)
    {
        delete form_minimap;
    }
    else if(ev.GetId() == ID_UNITS_WIN)
    {
        // unit editor closed
        auto new_unit = form_units->DoAddUnit();
        if(new_unit)
        {
            // add new unit to map
            spell_map->HaltUnitRanging(true);            
            new_unit->in_placement = true;
            new_unit->is_active = true;
            new_unit->ResetAP();
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
            spell_map->ResumeUnitRanging(false);
        }
        if(form_units->DoUpdateUnit())
        {
            // update current unit:
            spell_map->SortUnits();
            canvas->Refresh();
        }        
        form_units->Destroy();
    }
    else if(ev.GetId() == ID_EVENT_WIN)
    {
        // event editor closed        
        spell_map->SortUnits();
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
    if(!spell_map->IsLoaded())
        canvas->ClearBackground();
    else
    {
        wxPaintDC pdc(canvas);
        spell_map->Render(m_buffer,NULL,&spell_tool,bind(&MainFrame::CreateHUDbuttons,this));
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
    bool wSound = GetMenuBar()->FindItem(ID_ViewSounds)->IsChecked();
    bool wSoundLoop = GetMenuBar()->FindItem(ID_ViewSoundLoops)->IsChecked();
    bool wEvents = GetMenuBar()->FindItem(ID_ViewEvents)->IsChecked();
    bool wHobj = GetMenuBar()->FindItem(ID_HighlighObj)->IsChecked();
    spell_map->SetRender(wL1,wL2,wL3,wL4,wSS,wL5,wSound,wSoundLoop,wEvents,wHobj);
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
    // reset layers visibility
    spell_map->SetGamma(1.30);
    OnViewLayer(event);
    // reload toolset ribbon
    LoadToolsetRibbon();
    // repaint
    Refresh();
}

// save map data files
void MainFrame::OnSaveMap(wxCommandEvent& event)
{
    if(!spell_map || !spell_map->IsLoaded())
        return;

    OnSaveDTA(event);
    OnSaveDEF(event);    
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

    // show open dialog
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
    // create some map (###todo: set parameters by some menu)
    spell_map->Create(spell_data, "T11", 20,50);
    // reset layers visibility
    spell_map->SetGamma(1.30);
    OnViewLayer(event);
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

// update tiles context from all maps
void MainFrame::OnUpdateTileContextMaps(wxCommandEvent& event)
{   
    // split path to folder and file
    wstring dir = spell_data->spell_data_root + L"\\DATA\\COMMON\\";

    // show open dialog
    wxDirDialog fd = wxDirDialog(this, _("Select Spellcross COMMON folder"), dir, wxDD_DIR_MUST_EXIST);
    if(fd.ShowModal() == wxID_CANCEL)
        return;
    wstring path = wstring(fd.GetPath().ToStdWstring());
    
    // load map context (###todo: add terrain type selector?)
    spell_data->BuildSpriteContextOfMaps(path, "T11", bind(&MainFrame::StatusStringCallback,this,placeholders::_1));
}

// open tools editor
void MainFrame::OnViewTools(wxCommandEvent& event)
{
    if(!FindWindowById(ID_TOOLS_WIN))
    {
        form_tools = new FormTools(this, spell_data, ID_TOOLS_WIN);
        //form_tools->Connect(wxID_ANY, wxEVT_DESTROY, (wxObjectEventFunction)&MainFrame::OnViewToolsClose);
        form_tools->SetMap(spell_map);
        form_tools->Show();
    }
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
        // 
        // split path to folder and file
        std::filesystem::path last_path = wxStandardPaths::Get().GetExecutablePath().ToStdWstring();
        wstring dir = last_path.parent_path(); dir += wstring(L"\\");
        wstring name = last_path.filename();

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
        form_units->SetMapUnit(NULL, spell_map);
        form_units->Show();
    }


    /*auto *unit = spell_map->GetSelectedUnit();
    if(unit)
    {
        // start unit movement
        unit->in_placement = true;
    }*/
    
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
    }
    else if(menu_id == ID_POP_REM_MISSIONSTART)
    {
        // try remove the unit from MissionStart() event
        cur_unit->creator_event->ExtractUnit(cur_unit);
        spell_map->AddUnit(cur_unit);
        spell_map->SortUnits();
    }
    else if(menu_id == ID_POP_ADD_SEEUNIT)
    {
        // try add SeeUnit() event
        spell_map->events->AddSeeUnitEvent(cur_unit);
        spell_map->SortUnits();
    }
    else if(menu_id == ID_POP_ADD_DESTROY_UNIT)
    {
        // try add DestroyUnit() objective
        spell_map->events->AddUnitObjective(cur_unit,SpellMapEventRec::EvtTypes::EVT_DESTROY_UNIT);
        spell_map->SortUnits();
    }
    else if(menu_id == ID_POP_ADD_SAVE_UNIT)
    {
        // try add SaveUnit() objective
        spell_map->events->AddUnitObjective(cur_unit,SpellMapEventRec::EvtTypes::EVT_SAVE_UNIT);
        spell_map->SortUnits();
    }
    else if(menu_id == ID_POP_ADD_TRANSPORT_UNIT)
    {
        // try add TransportUnit() objective
        spell_map->events->AddUnitObjective(cur_unit,SpellMapEventRec::EvtTypes::EVT_TRANSPORT_UNIT);
        spell_map->SortUnits();
    }
    else if(menu_id == ID_POP_REM_SEEUNIT || menu_id == ID_POP_REM_DESTROY_UNIT || menu_id == ID_POP_REM_SAVE_UNIT || menu_id == ID_POP_REM_TRANSPORT_UNIT)
    {
        // try remove xxxUnit() events
        spell_map->SelectEvent(NULL);
        auto evt = cur_unit->GetTrigEvent({SpellMapEventRec::EvtTypes::EVT_SEE_UNIT,SpellMapEventRec::EvtTypes::EVT_DESTROY_UNIT,SpellMapEventRec::EvtTypes::EVT_SAVE_UNIT,SpellMapEventRec::EvtTypes::EVT_TRANSPORT_UNIT});
        spell_map->events->EraseEvent(evt);
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
    }
    else if(menu_id == ID_POP_REM_OBJ)
    {
        // remove object tile        
        spell_map->RemoveObj();
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
        spell_map->LockMap();
        spell_map->RemoveANM();
        spell_map->ReleaseMap();
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
                menu.Append(ID_POP_EDIT_TERR,"Edit terrain sprite");
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
    
    // get selected area (preference of persistent selection over cursor)
    std::vector<MapXY> list;
    list = spell_map->GetPersistSelections();
    if(list.empty())
        list = spell_map->GetSelections();

    spell_map->LockMap();
    spell_map->HaltUnitRanging();
    if(event.GetId() == ID_CutBuf)
        spell_map->CutBuffer(list,lay);
    else
        spell_map->CopyBuffer(list, lay);
    spell_map->ResumeUnitRanging();
    spell_map->ReleaseMap();
}
// clear map copy buffer
void MainFrame::OnClearBuf(wxCommandEvent& event)
{
    if(!spell_map->IsLoaded())
        return;
    // clear copy buffer and also clear tool
    spell_map->LockMap();
    spell_map->ClearBuffer();    
    wxRibbonBarEvent rev;
    OnToolPageClick(rev);        
    spell_map->ReleaseMap();
    Refresh();
}
// try place copy buffer to map
void MainFrame::OnPasteBuf(wxCommandEvent& event)
{
    if(!spell_map->IsLoaded())
        return;
    auto list = spell_map->GetSelections();
    if(list.empty())
        return;

    spell_map->LockMap();
    spell_map->PasteBuffer(spell_map->tiles,spell_map->L3,spell_map->L4,list);
    spell_map->ReleaseMap();
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
        spell_map->RemoveUnit(spell_map->GetCursorUnit(),true);
    }
    spell_map->ReleaseMap();
    Refresh();
}



// canvas left click
void MainFrame::OnCanvasLMouseDown(wxMouseEvent& event)
{
    if(!spell_map->IsLoaded())
        return;
    if(inUnitOptions())
        return;    
       
    // get selection
    auto xy_list = spell_map->GetSelections();
    
    if(event.LeftDown())
    {
        // LEFT DOWN event:

        if(spell_map->isCopyBufferFull() && xy_list.size() && xy_list[0].IsSelected())
        {
            // something in copy buffer
            spell_map->LockMap();
            spell_map->PasteBuffer(spell_map->tiles,spell_map->L3,spell_map->L4,xy_list);
            spell_map->ReleaseMap();
            Refresh();
        }
        else if(spell_tool.isActive() && xy_list.size() && xy_list[0].IsSelected())
        {
            // some tool selected: edit map class        
            spell_map->EditClass(xy_list, &spell_tool, bind(&MainFrame::StatusStringCallback,this,placeholders::_1));
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
                }
                else if(wEvents && sel_evt && sel_evt->position == select_pos)
                {
                    // move/place event
                    if(sel_unit)
                        sel_unit->in_placement = false;
                    sel_evt->in_placement = !sel_evt->in_placement;
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
                    if(sel_evt)
                        sel_evt->in_placement = false;
                    sel_unit->in_placement = !sel_unit->in_placement;
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

    spell_map->SetUnitRangeViewMode(SpellMap::UNIT_RANGE_NONE);
    spell_map->scroller.Idle();
}
// on canvas mouse move
void MainFrame::OnCanvasMouseMove(wxMouseEvent& event)
{
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

    canvas->Refresh();
    //event.Skip();
}
// on canvas wheel
void MainFrame::OnCanvasMouseWheel(wxMouseEvent& event)
{
    spell_map->scroller.ResizeSelection(event.GetWheelRotation()/event.GetWheelDelta());
    canvas->Refresh();
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
                    {
                        // tool is object
                        // unset tool (depreceted method)
                        spell_tool.Set();
                        // place tool to clipboard (new method)
                        spell_map->SetBuffer(obj);
                    }
                    else
                        spell_tool.Set(tid, iid); // tool is class
                }
            }
        }
    }
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
FormGamma::FormGamma(wxFrame* parent,SpellMap* map,wxWindowID id) :wxDialog(parent,wxID_ANY,"Gamma correction",wxDefaultPosition,wxSize(400,80),wxDEFAULT_FRAME_STYLE|wxSTAY_ON_TOP)
{
    // store local reference to initial map and data
    spell_map = map;

    // make slider
    slider = new wxSlider(this,wxID_ANY,1300,500,2000);
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(slider,1,wxEXPAND|wxALL);
    this->SetSizer(sizer);
    this->SetAutoLayout(true); 
    this->Center();
    this->Bind(wxEVT_CHAR_HOOK,&FormGamma::OnExit,this);
    
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
    if(event.GetKeyCode()==WXK_ESCAPE)
        this->Close();
    else
        event.Skip();
}
