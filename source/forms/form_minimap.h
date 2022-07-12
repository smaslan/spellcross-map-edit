#pragma once

#include "spellcross.h"
#include "map.h"
#include <wx/frame.h>
#include <wx/window.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <string>

class TMiniMap {
public:
    wxBitmap* bmp;
    TScroll *scroll;
    SpellMap *map;
    int source_x_ofs;
    int source_y_ofs;
    int source_x;
    int source_y;
};

class FormMiniMap
{
public:

    FormMiniMap(wxPanel* parent,wxWindowID win_id,SpellData* spell_data,TMiniMap &minimap);
    ~FormMiniMap();

private:
    SpellData* m_spelldata;
    wxBitmap* m_frame;
    TMiniMap m_minimap;

    wxWindow* form;    
    
    wxPoint m_click_pos;
    int m_in_move;

    void OnClose(wxCloseEvent& ev);
    void OnPaintTab(wxPaintEvent& event);    
    void OnKeyPress(wxKeyEvent& event);
    void OnLeaveWin(wxMouseEvent& event);
    void OnWinClick(wxMouseEvent& event);    
    void OnWinMouseMove(wxMouseEvent& event);    

    // maximum size of minimap panel
    static constexpr int MAX_MINIMAP_X = 1000;
    static constexpr int MAX_MINIMAP_Y = 400;
};

