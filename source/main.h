#ifndef MAIN_H
#define MAIN_H

#include "simpleini.h"
#include "spellcross.h"
#include "map.h"

// draw canvas for map
class mapCanvas : public wxPanel
{
    wxBitmap m_buffer;
    SpellMap* spell_map;
    TScroll* scroll;
    wxFrame *parent;

public:
    mapCanvas(wxFrame* parent, SpellMap* spell_map,TScroll* scroll);

    void OnPaint(wxPaintEvent& event);
    //void OnResize(wxSizeEvent& event);

    void OnMouseDown(wxMouseEvent& event);
    void OnMouseUp(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnMouseLeave(wxMouseEvent& event);
    void OnMouseWheel(wxMouseEvent& event);
    void OnKeyDown(wxKeyEvent& event);

    // some useful events
    /*
     void mouseMoved(wxMouseEvent& event);
     void mouseDown(wxMouseEvent& event);
     void mouseWheelMoved(wxMouseEvent& event);
     void mouseReleased(wxMouseEvent& event);
     void rightClick(wxMouseEvent& event);
     void mouseLeftWindow(wxMouseEvent& event);
     void keyPressed(wxKeyEvent& event);
     void keyReleased(wxKeyEvent& event);
     */

     //DECLARE_EVENT_TABLE()
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

private:
    void OnViewLayer(wxCommandEvent& event);
    void OnOpenMap(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnPaint(wxPaintEvent& event);
    void OnResize(wxSizeEvent& event);

    void OnTimer(wxTimerEvent& event);

    /*void OnMouseDown(wxMouseEvent& event);
    void OnMouseUp(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnMouseLeave(wxMouseEvent& event);*/

    SpellMap* spell_map;
    SpellData* spell_data;

    //wxBitmap m_buffer;
    wxTimer m_timer;
    TScroll scroll;
};

// GUI elements
enum
{
    ID_OpenMap = 101,
    ID_ViewTer = 201,
    ID_ViewObj = 202,
    ID_ViewAnm = 203,
    ID_ViewPnm = 204,
    ID_ViewUnt = 205,
    ID_ViewStTa = 206
};


#endif