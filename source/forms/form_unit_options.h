#pragma once

#include "spellcross.h"
#include <wx/window.h>
#include <wx/panel.h>
#include <string>

class FormUnitOpts
{
private:
    SpellData* m_spelldata;    
    std::function<void(int)> m_sel_cb;
    vector<uint8_t> m_back_buff;
    wxBitmap* m_bmp_btn[2];
    vector<string> m_labels;
    vector<int> m_actions;
    int m_action;

    wxWindow* form;
    wxPanel* buttons[2];

    string info_label;

    void OnClose(wxCloseEvent& ev);
    void OnPaintTab(wxPaintEvent& event);
    void OnPaintButton(wxPaintEvent& event);
    void OnButtonMouseHover(wxMouseEvent& event);
    void OnButtonClick(wxMouseEvent& event);

    static constexpr int ID_BTN = 1000;

public:
    FormUnitOpts(wxPanel* parent,wxWindowID win_id,wxPoint position,SpellData* spelldata,int flags,std::function<void(int)> selection_cb);
    ~FormUnitOpts();
    void ResultCallback();        

};

