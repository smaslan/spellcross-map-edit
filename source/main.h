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

#include <wx/ribbon/buttonbar.h>
#include <wx/ribbon/panel.h>
#include <wx/ribbon/page.h>
#include <wx/ribbon/control.h>
#include <wx/ribbon/art.h>
#include <wx/ribbon/bar.h>


// main
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



// main form
class MyFrame : public wxFrame
{
public:
    MyFrame(SpellMap* map,SpellData* spelldata);      
    void StatusStringCallback(std::string info);

    void CreateHUDbuttons();

private:
    void OnViewLayer(wxCommandEvent& event);
    void OnOpenMap(wxCommandEvent& event);
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
    void OnViewObjects(wxCommandEvent& event);
    void OnViewPal(wxCommandEvent& event);
    void OnViewGrRes(wxCommandEvent& event);
    void OnEditUnit(wxCommandEvent& event);
    void OnEditEvent(wxCommandEvent& event);
    void OnViewVideo(wxCommandEvent& event);
    void OnViewMidi(wxCommandEvent& event);
    void OnViewVoxZ(wxCommandEvent& event);
    void OnViewMiniMap(wxCommandEvent& event);
    void OnUpdateTileContext(wxCommandEvent& event);
    void OnUpdateTileContextMaps(wxCommandEvent& event);    
    void OnSelectAll(wxCommandEvent& event);
    void OnDeselectAll(wxCommandEvent& event);
    void OnInvalidateSelection(wxCommandEvent& event);
    void OnCreateNewObject(wxCommandEvent& event);
    void OnMoveUnit(wxCommandEvent& event);

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
    wxBitmap m_buffer;
    
    wxPanel* canvas;
    wxMenu* menuView;
    
    wxBoxSizer* sizer;
    wxRibbonBar* ribbonBar = NULL;
    
    SpellMap* spell_map;
    SpellData* spell_data;

    SpellTool spell_tool;

    // last target selection
    MapXY select_pos;
    MapUnit* cur_unit;
    MapUnit* sel_unit;
    int inUnitOptions() {return(form_unit_opts != NULL || form_message != NULL);};

    void ShowMessage(SpellTextRec *message, bool is_yesno, std::function<void(bool)> exit_cb=NULL);
    bool CheckMessageState();

    
    wxTimer m_timer;
    //TScroll scroll;

    FormSprite* form_sprites;
    FormObjects* form_objects;
    FormTools* form_tools;
    FormPalView* form_pal;
    FormGResView* form_gres;
    FormUnits* form_units;
    FormEvent* form_events;
    FormVideo* form_videos;
    FormUnitOpts *form_unit_opts;
    FormMsgBox *form_message;
    FormVideoBox* form_video_box;
    FormMapOptions *form_map_options;
    FormMIDI *form_midi;
    FormMiniMap *form_minimap;
    FormMapUnits *form_units_list;
    
    void OnPaintHUDbutton(wxPaintEvent& event);
    void OnHUDbuttonsMouseEnter(wxMouseEvent& event);
    void OnHUDbuttonsLeave(wxMouseEvent& event);
    void OnHUDbuttonsClick(wxMouseEvent& event);
    vector<wxPanel*> hud_buttons;    

    enum
    {
        ID_MAIN_WIN = 2000,
        ID_OBJECTS_WIN,
        ID_SPRITES_WIN,
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

    enum
    {
        ID_POP_ADD_MISSIONSTART = 2100,
        ID_POP_REM_MISSIONSTART,
        ID_POP_ADD_SEEUNIT,
        ID_POP_REM_SEEUNIT,
        ID_POP_EDIT_EVENT,
        ID_POP_EDIT_UNIT
    };

    // maximum size of minimap panel
    static constexpr int MAX_MINIMAP_X = 1000;
    static constexpr int MAX_MINIMAP_Y = 400;

};


// gamma form
class FormGamma : public wxDialog
{
public:
    FormGamma(wxFrame* parent,SpellMap* map);    
private:    
    void OnChangeGamma(wxCommandEvent& event);
    SpellMap* spell_map;
    wxSlider* slider;
};


// GUI elements
enum
{
    ID_OpenMap = 100,
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
    ID_ViewVoxZ,
    ID_ViewMiniMap,
    ID_ExportVoxZ,
    ID_SetGamma,
    ID_ViewSprites,
    ID_ViewPal,
    ID_ViewGRes,
    ID_EditUnit,
    ID_EditEvent,
    ID_UpdateSprContext,
    ID_SaveSprContext,
    ID_ViewObjects,
    ID_ViewTools,
    ID_SelectAll,
    ID_DeselectAll,
    ID_InvalidateSel,
    ID_CreateNewObject,
    ID_MoveUnit,
    ID_UpdateSprContextMaps,
    ID_ViewVideo,
    ID_ViewMIDI
};


#endif