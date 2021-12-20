#ifndef MAIN_H
#define MAIN_H

#include "simpleini.h"
#include "spellcross.h"
#include "map.h"
#include "form_objects.h"

#include <wx/ribbon/buttonbar.h>
#include <wx/ribbon/panel.h>
#include <wx/ribbon/page.h>
#include <wx/ribbon/control.h>
#include <wx/ribbon/art.h>
#include <wx/ribbon/bar.h>


// draw canvas for map
class mapCanvas : public wxPanel
{
private:   
    SpellMap* spell_map;
    TScroll* scroll;
    wxBitmap m_buffer;
    wxFrame *parent;
    SpellTool *spell_tool;

public:
    mapCanvas(wxFrame* parent, SpellMap* spell_map,TScroll* scroll);
    void SetToolRef(SpellTool* spell_tool);

    void OnPaint(wxPaintEvent& event);
    void OnMouseDown(wxMouseEvent& event);
    void OnMouseUp(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnMouseLeave(wxMouseEvent& event);
    void OnMouseEnter(wxMouseEvent& event);
    void OnMouseWheel(wxMouseEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnLMouseDown(wxMouseEvent& event); 

    wxBitmap *GetRednerBuffer();
    
};


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

    void OnPaint(wxPaintEvent& event);
    void OnResize(wxSizeEvent& event);

    void OnTimer(wxTimerEvent& event);

    void OnSetGamma(wxCommandEvent& event);
    void OnViewSprites(wxCommandEvent& event);
    void OnViewObjects(wxCommandEvent& event);
    void OnViewObjectsClose(wxWindowDestroyEvent& ev);
    void OnUpdateTileContext(wxCommandEvent& event);
    void OnUpdateTileContextMaps(wxCommandEvent& event);    
    void OnSelectAll(wxCommandEvent& event);
    void OnDeselectAll(wxCommandEvent& event);
    void OnInvalidateSelection(wxCommandEvent& event);
    void OnCreateNewObject(wxCommandEvent& event);

    void OnToolBtnClick(wxRibbonButtonBarEvent& event);
    void OnToolPageClick(wxRibbonBarEvent& event);
    SpellTool GetToolSelection();

    

    mapCanvas* canvas;
    
    wxRibbonBar* ribbonBar;
    

    /*void OnMouseDown(wxMouseEvent& event);
    void OnMouseUp(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnMouseLeave(wxMouseEvent& event);*/

    SpellMap* spell_map;
    SpellData* spell_data;

    SpellTool spell_tool;

    wxTimer m_timer;
    TScroll scroll;

    FormObjects* form_objects;

    enum
    {
        ID_MAIN_WIN = 2000,
        ID_OBJECTS_WIN,
        ID_SPRITES_WIN
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
    ID_SelectAll,
    ID_DeselectAll,
    ID_InvalidateSel,
    ID_CreateNewObject,
    ID_UpdateSprContextMaps
};


#endif