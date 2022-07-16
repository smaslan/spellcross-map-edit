#pragma once

#include "spellcross.h"
#include "map.h"
#include <wx/frame.h>
#include <wx/window.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/sizer.h>
#include <string>


class FormMapUnits
{
public:

    FormMapUnits(wxPanel* parent,wxWindowID win_id,SpellData* spell_data,SpellMap *spell_map);
    ~FormMapUnits();
    MapUnit* GetSelectedUnit();

private:
    SpellData* m_spelldata;
    SpellMap* m_spellmap;
    wxWindowID m_win_id;
    MapUnit *m_unit;
    
    wxPanel *canvas;
    wxSizer* sizer;
    wxGridSizer* grid;
    std::vector<wxPanel*> panels;

    void OnResizeCanvas(wxSizeEvent& ev);
    void OnCanvasKey(wxKeyEvent& ev);    
    void OnPaintUnit(wxPaintEvent& event);    
    void OnUnitClick(wxMouseEvent& event);    
    void OnUnitHover(wxMouseEvent& event);

    static constexpr int PANEL_SPACING = 15;
    static constexpr int TOP_Y_GAP = 20;
    static constexpr int HUD_Y_GAP = 120;
    static constexpr int wxID_PANEL0 = 1000;

};

