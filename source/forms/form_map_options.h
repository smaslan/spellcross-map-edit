#pragma once

#include <wx/frame.h>
#include <wx/window.h>
#include <wx/panel.h>
#include <wx/scrolbar.h>
#include <wx/event.h>

#include "map.h"
#include "forms/form_midi.h"
#include <string>

class FormMapOptionsBtn
{
public:    
    wxPanel *panel;
    int action_id;
    int is_hover;
    int is_down;
    std::string text;

    FormMapOptionsBtn()
    {
        is_hover = false;
        is_down = false;
        text = "";
    }
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

    FormMIDI *form_midi;

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
        wxID_BTN_LOAD,
        wxID_BTN_MIDI,
        wxID_WINDOW_MIDI
    };

};

