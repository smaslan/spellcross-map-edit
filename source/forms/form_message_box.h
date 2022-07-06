#pragma once

#include "spellcross.h"
#include "map.h"
#include <wx/frame.h>
#include <wx/window.h>
#include <wx/panel.h>
#include <string>

class FormMsgBox
{
public:

    enum class SpellMsgOptions { NONE,YESNO };
    enum class SpellMsgResult { CLOSE,NO,YES };

    FormMsgBox(wxPanel* parent,wxWindowID win_id,SpellData* spell_data,SpellMap* spell_map,SpellTextRec* text,SpellMsgOptions options=SpellMsgOptions::NONE,std::function<void(bool)> selection_cb=NULL);
    ~FormMsgBox();
    void ResultCallback();

private:
    SpellData* m_spelldata;    
    SpellMap* m_spell_map;
    std::function<void(int)> m_sel_cb;
    SpellMsgOptions m_options;

    SpellTextChunks m_chunks;
    
    SpellSound* m_sound;

    wxWindow* form;
    wxPanel *m_buttons[2];
    vector<string> m_labels;
    vector<SpellMsgResult> m_results;
    SpellMsgResult m_result;

    wxPoint m_click_pos;
    int m_in_move;

    void OnClose(wxCloseEvent& ev);
    void OnPaintTab(wxPaintEvent& event);
    void OnKeyPress(wxKeyEvent& event);
    void OnLeaveWin(wxMouseEvent& event);
    void OnWinClick(wxMouseEvent& event);
    
    void OnPaintButton(wxPaintEvent& event);
    void OnButtonMouseHover(wxMouseEvent& event);
    void OnButtonClick(wxMouseEvent& event);
    void OnWinMouseMove(wxMouseEvent& event);
    

    static constexpr int ID_BTN = 1000;

    static constexpr int BTN_X_GAP = 30;
    static constexpr int BTN_Y_GAP = 10;
    static constexpr int BTN_Y = 30;
    static constexpr int BTN_X = 60;


};

