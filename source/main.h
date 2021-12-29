#ifndef MAIN_H
#define MAIN_H

#include "simpleini.h"
#include "spellcross.h"
#include "map.h"
#include "form_objects.h"
#include "form_tools.h"
#include "form_sprite_view.h"

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

private:
    void OnViewLayer(wxCommandEvent& event);
    void OnOpenMap(wxCommandEvent& event);
    void OnNewMap(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnClose(wxCloseEvent& ev);

    void OnPaint(wxPaintEvent& event);
    void OnResize(wxSizeEvent& event);

    void OnTimer(wxTimerEvent& event);

    void OnSetGamma(wxCommandEvent& event);
    void OnViewTools(wxCommandEvent& event);
    //void OnViewToolsClose(wxWindowDestroyEvent& ev);
    void OnViewSprites(wxCommandEvent& event);
    void OnViewObjects(wxCommandEvent& event);
    //void OnViewObjectsClose(wxWindowDestroyEvent& ev);
    void OnUpdateTileContext(wxCommandEvent& event);
    void OnUpdateTileContextMaps(wxCommandEvent& event);    
    void OnSelectAll(wxCommandEvent& event);
    void OnDeselectAll(wxCommandEvent& event);
    void OnInvalidateSelection(wxCommandEvent& event);
    void OnCreateNewObject(wxCommandEvent& event);

    void OnToolBtnClick(wxRibbonButtonBarEvent& event);
    void OnToolPageClick(wxRibbonBarEvent& event);
    SpellTool GetToolSelection();
    void LoadToolsetRibbon(Terrain* terr = NULL);

    // canvas stuff:
    void OnPaintCanvas(wxPaintEvent& event);
    void OnCanvasMouseDown(wxMouseEvent& event);
    void OnCanvasMouseUp(wxMouseEvent& event);
    void OnCanvasMouseMove(wxMouseEvent& event);
    void OnCanvasMouseLeave(wxMouseEvent& event);
    void OnCanvasMouseEnter(wxMouseEvent& event);
    void OnCanvasMouseWheel(wxMouseEvent& event);
    void OnCanvasKeyDown(wxKeyEvent& event);
    void OnCanvasLMouseDown(wxMouseEvent& event);
    wxBitmap m_buffer;


    wxPanel* canvas;
    
    wxBoxSizer* sizer;
    wxRibbonBar* ribbonBar = NULL;
    

    SpellMap* spell_map;
    SpellData* spell_data;

    SpellTool spell_tool;

    
    wxTimer m_timer;
    TScroll scroll;

    FormSprite* form_sprites;
    FormObjects* form_objects;
    FormTools* form_tools;

    enum
    {
        ID_MAIN_WIN = 2000,
        ID_OBJECTS_WIN,
        ID_SPRITES_WIN,
        ID_TOOLS_WIN
    };

    static constexpr int ID_TOOL_BASE = 10000;
    static constexpr int ID_TOOL_CLASS_STEP = 100;
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
    ID_ViewTer,
    ID_ViewObj,
    ID_ViewAnm,
    ID_ViewPnm,
    ID_ViewUnt,
    ID_ViewStTa,
    ID_SetGamma,
    ID_ViewSprites,
    ID_UpdateSprContext,
    ID_SaveSprContext,
    ID_ViewObjects,
    ID_ViewTools,
    ID_SelectAll,
    ID_DeselectAll,
    ID_InvalidateSel,
    ID_CreateNewObject,
    ID_UpdateSprContextMaps
};


#endif