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

#ifndef MAIN_H
#define MAIN_H

#include "simpleini.h"
#include "spellcross.h"
#include "map.h"
#include "spell_hud_buttons.h"
#include "spell_texts.h"
#include "forms/form_loader.h"
#include "forms/form_objects.h"
#include "forms/form_tools.h"
#include "forms/form_sprite_view.h"
#include "forms/form_anm.h"
#include "forms/form_pal_view.h"
#include "forms/form_gr_view.h"
#include "forms/form_units.h"
#include "forms/form_events.h"
#include "forms/form_new_object.h"
#include "forms/form_unit_options.h"
#include "forms/form_message_box.h"
#include "forms/form_map_options.h"
#include "forms/form_video.h"
#include "forms/form_video_box.h"
#include "forms/form_midi.h"
#include "forms/form_minimap.h"
#include "forms/form_map_units_list.h"
#include "forms/form_mission_params.h"
#include "forms/form_about.h"
#include "forms/form_sound.h"
#include "forms/form_flags.h"

#include <wx/ribbon/buttonbar.h>
#include <wx/ribbon/panel.h>
#include <wx/ribbon/page.h>
#include <wx/ribbon/control.h>
#include <wx/ribbon/art.h>
#include <wx/ribbon/bar.h>

// app entry point class
class MyApp : public wxApp
{
private:
    CSimpleIniA ini;
    SpellMap* spell_map;
    SpellData* spell_data;   
public:
    virtual bool OnInit();
    virtual int OnExit();
};



// gamma correction form
class FormGamma : public wxDialog
{
public:
    FormGamma(wxFrame* parent,SpellMap* map,wxWindowID id);
private:
    void OnChangeGamma(wxCommandEvent& event);
    void OnClose(wxCloseEvent& ev);
    void OnExit(wxKeyEvent& event);
    SpellMap* spell_map;
    wxSlider* slider;
};


// Main application frame
class MainFrame : public wxFrame
{
public:
    MainFrame(SpellMap* map,SpellData* spelldata);      
    void StatusStringCallback(std::string info);
    void CreateHUDbuttons();

private:
    void OnViewLayer(wxCommandEvent& event);
    void OnOpenMap(wxCommandEvent& event);
    void OnSaveMap(wxCommandEvent& event);
    void OnSaveDTA(wxCommandEvent& event);
    void OnSaveDEF(wxCommandEvent& event);
    void OnNewMap(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnClose(wxCloseEvent& ev);     

    void OnSwitchGameMode(wxCommandEvent& event);
    void OnResetUnitView(wxCommandEvent& event);
    void OnSelectUnitView(wxCommandEvent& event);

    void OnPaint(wxPaintEvent& event);
    void OnResize(wxSizeEvent& event);

    void OnTimer(wxTimerEvent& event);

    void OnSetGamma(wxCommandEvent& event);
    void OnViewTools(wxCommandEvent& event);
    void OnViewSprites(wxCommandEvent& event);
    void OnViewAnms(wxCommandEvent& event);
    void OnViewPnms(wxCommandEvent& event);
    void OnViewSounds(wxCommandEvent& event);
    void OnViewObjects(wxCommandEvent& event);
    void OnViewPal(wxCommandEvent& event);
    void OnViewGrRes(wxCommandEvent& event);
    void OnEditUnit(wxCommandEvent& event);
    void OnEditEvent(wxCommandEvent& event);
    void OnViewVideo(wxCommandEvent& event);
    void OnViewMidi(wxCommandEvent& event);
    void OnTileFlags(wxCommandEvent& event);
    void OnViewVoxZ(wxCommandEvent& event);
    void OnViewMiniMap(wxCommandEvent& event);
    void OnUnitViewDebug(wxCommandEvent& event);
    void OnUpdateTileContext(wxCommandEvent& event);
    void OnUpdateTileContextMaps(wxCommandEvent& event);    
    void OnSelectAll(wxCommandEvent& event);
    void OnDeselectAll(wxCommandEvent& event);
    void OnSelectDeselect(wxCommandEvent& event);
    void OnCopyBuf(wxCommandEvent& event);
    void OnPasteBuf(wxCommandEvent& event);
    void OnClearBuf(wxCommandEvent& event);
    void OnDeleteSel(wxCommandEvent& event);
    void OnChangeElevation(wxCommandEvent& event);
    void OnInvalidateSelection(wxCommandEvent& event);
    void OnCreateNewObject(wxCommandEvent& event);
    void OnAddUnit(wxCommandEvent& event);
    void OnEditMissionParams(wxCommandEvent& event);

    void OnToolBtnClick(wxRibbonButtonBarEvent& event);
    void OnToolPageClick(wxRibbonBarEvent& event);
    SpellTool GetToolSelection();
    void LoadToolsetRibbon(Terrain* terr = NULL);

    // canvas stuff:
    void OnThreadCanvas(wxThreadEvent& event);
    void OnPaintCanvas(wxPaintEvent& event);
    void OnCanvasRMouse(wxMouseEvent& event);
    void OnCanvasMouseMove(wxMouseEvent& event);
    void OnCanvasMouseLeave(wxMouseEvent& event);
    void OnCanvasMouseEnter(wxMouseEvent& event);
    void OnCanvasMouseWheel(wxMouseEvent& event);
    void OnCanvasKeyDown(wxKeyEvent& event);
    void OnCanvasLMouseDown(wxMouseEvent& event);
    void OnCanvasPopupSelect(wxCommandEvent& event);
    void OnUnitClick_cb(int action);
    
    // map render buffer
    wxBitmap m_buffer;
    
    wxPanel* canvas;
    wxMenu* menuView;    
    wxBoxSizer* sizer;
    wxRibbonBar* ribbonBar = NULL;
    
    // spellcross data object
    SpellData* spell_data;
    // spellcross map object
    SpellMap* spell_map;
    // last selected tile pos
    MapXY spell_pos;
    // selected edit tool
    SpellTool spell_tool;

    // last target selection
    MapXY select_pos;
    MapUnit* cur_unit;
    MapUnit* sel_unit;
    int inUnitOptions() {return(form_unit_opts != NULL || form_message != NULL);};
    int inSubForm() {return(
        form_gamma != NULL ||
        form_sprites != NULL ||
        form_anms != NULL ||
        form_sounds != NULL ||
        form_objects != NULL ||
        form_tools != NULL ||
        form_pal != NULL ||
        form_gres != NULL ||
        form_units != NULL ||
        form_events != NULL ||
        form_videos != NULL ||
        form_unit_opts != NULL ||
        form_message != NULL ||
        form_video_box != NULL ||
        form_map_options != NULL ||
        form_midi != NULL ||
        form_minimap != NULL ||
        form_units_list != NULL); };

    void ShowMessage(SpellTextRec *message, bool is_yesno, std::function<void(bool)> exit_cb=NULL);
    bool CheckMessageState();

    // tick timer
    wxTimer m_timer;

    // sub-forms
    FormGamma* form_gamma = NULL;
    FormSprite* form_sprites = NULL;
    FormANM* form_anms = NULL;
    FormObjects* form_objects = NULL;
    FormTools* form_tools = NULL;
    FormPalView* form_pal = NULL;
    FormGResView* form_gres = NULL;
    FormUnits* form_units = NULL;
    FormEvent* form_events = NULL;
    FormVideo* form_videos = NULL;
    FormUnitOpts *form_unit_opts = NULL;
    FormMsgBox *form_message = NULL;
    FormVideoBox* form_video_box = NULL;
    FormMapOptions *form_map_options = NULL;
    FormMIDI *form_midi = NULL;
    FormMiniMap *form_minimap = NULL;
    FormMapUnits *form_units_list = NULL;
    FormSound *form_sounds = NULL;
    
    // spellcross HUD interface stuff
    void OnPaintHUDbutton(wxPaintEvent& event);
    void OnHUDbuttonsMouseEnter(wxMouseEvent& event);
    void OnHUDbuttonsLeave(wxMouseEvent& event);
    void OnHUDbuttonsClick(wxMouseEvent& event);
    vector<wxPanel*> hud_buttons;    

    // sub-forms IDs
    enum
    {
        ID_MAIN_WIN = 2000,
        ID_GAMMA_WIN,
        ID_OBJECTS_WIN,
        ID_SPRITES_WIN,
        ID_ANM_WIN,
        ID_SOUNDS_WIN,
        ID_TOOLS_WIN,
        ID_PAL_WIN,
        ID_GRES_WIN,
        ID_UNITS_WIN,
        ID_EVENT_WIN,
        ID_MINIMAP_WIN,
        ID_UNIT_MODE_WIN,
        ID_MSG_WIN,
        ID_VIDEO_BOX_WIN,
        ID_MAP_OPT_WIN,
        ID_VIDEO_WIN,
        ID_MIDI_WIN,
        ID_MAP_UNITS_WIN
    };
    static constexpr int ID_HUD_BASE = 3000;
    static constexpr int ID_TOOL_BASE = 10000;
    static constexpr int ID_TOOL_CLASS_STEP = 100;

    // map edit popup menu items
    enum
    {
        ID_POP_ADD_MISSIONSTART = 2100,
        ID_POP_REM_MISSIONSTART,
        ID_POP_ADD_SEEUNIT,
        ID_POP_REM_SEEUNIT,
        ID_POP_EDIT_EVENT,
        ID_POP_ANOTHER_EVENT,
        ID_POP_EDIT_UNIT,
        ID_POP_EDIT_OBJ,
        ID_POP_REM_OBJ,
        ID_POP_EDIT_ANM,
        ID_POP_REM_ANM,
        ID_POP_EDIT_PNM,
        ID_POP_REM_PNM,
        ID_POP_EDIT_SOUND,
        ID_POP_REM_SOUND
    };

    // maximum size of minimap panel
    static constexpr int MAX_MINIMAP_X = 1000;
    static constexpr int MAX_MINIMAP_Y = 400;

};


// GUI elements
enum
{
    ID_OpenMap = 100,
    ID_SaveMap,
    ID_SaveMapAs,
    ID_SaveDTA,
    ID_SaveDEF,
    ID_NewMap,
    ID_mmGameMode,
    ID_mmResetViewMap,
    ID_mmUnitViewMode,
    ID_Canvas,
    ID_ViewTer,
    ID_ViewObj,
    ID_ViewAnm,
    ID_ViewPnm,
    ID_ViewUnt,
    ID_ViewStTa,
    ID_ViewHUD,
    ID_ViewSounds,
    ID_ViewSoundLoops,
    ID_ViewEvents,
    ID_HighlighObj,
    ID_ViewVoxZ,
    ID_ViewMiniMap,
    ID_ExportVoxZ,
    ID_SetGamma,
    ID_ViewSprites,
    ID_ViewAnms,
    ID_ViewPnms,
    ID_SoundsViewer,
    ID_ViewPal,
    ID_ViewGRes,
    ID_UnitViewDbg,
    ID_EditMissionParams,
    ID_EditUnit,
    ID_EditEvent,
    ID_UpdateSprContext,
    ID_SaveSprContext,
    ID_ViewObjects,
    ID_ViewTools,
    ID_SelectLay1,
    ID_SelectLay2,
    ID_SelectLayANM,
    ID_SelectAll,
    ID_DeselectAll,
    ID_SelectDeselect,
    ID_InvalidateSel,
    ID_DeleteSel,
    ID_ElevUp,
    ID_ElevDown,
    ID_CopyBuf,
    ID_CutBuf,
    ID_PasteBuf,
    ID_ClearBuf,
    ID_PlaceStart,
    ID_PlaceExit,
    ID_CreateNewObject,
    ID_AddUnit,
    ID_UpdateSprContextMaps,
    ID_ViewVideo,
    ID_ViewMIDI,
    ID_EditTileFlags
};


#endif