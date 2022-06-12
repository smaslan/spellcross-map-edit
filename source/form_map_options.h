#pragma once

#include "map.h"
#include <wx/frame.h>
#include <wx/window.h>
#include <wx/panel.h>
#include <string>
#include <wx/scrolbar.h>
#include <wx/event.h>

class FormMapOptionsBtn
{
public:    
    wxPanel *panel;
    int action_id;
    int is_hover;
    int is_down;
};

class FormMapOptions
{
public:

    FormMapOptions(wxPanel* parent,wxWindowID win_id,SpellMap* spell_map);
    ~FormMapOptions();
    void ResultCallback();

private:
    SpellData* m_spelldata;    
    SpellMap* m_spell_map;

    vector<uint8_t> m_buffer;
    vector<uint8_t> m_mask;

    wxPanel* form;
    vector<FormMapOptionsBtn> m_buttons;
    vector<string> m_labels;

    wxPoint m_click_pos;
    int m_in_move;
    int m_was_moved;

    void OnClose(wxCloseEvent& ev);
    void OnPaintTab(wxPaintEvent& event);
    void OnKeyPress(wxKeyEvent& event);
    void OnLeaveWin(wxMouseEvent& event);
    void OnWinClick(wxMouseEvent& event);
    
    void OnPaintButton(wxPaintEvent& event);
    void OnButtonMouseHover(wxMouseEvent& event);
    void OnButtonClick(wxMouseEvent& event);
    void OnWinMouseMove(wxMouseEvent& event);  

    void OnScroll(wxScrollEvent& event);

    enum
    {
        wxID_SCROLL_GAMMA = 1000,
        wxID_SCROLL_MUSIC,
        wxID_SCROLL_SOUND,
        wxID_BTN_SAVE,
        wxID_BTN_LOAD
    };

};

