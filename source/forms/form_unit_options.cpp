#include "form_unit_options.h"
#include "map.h"

#include <wx/rawbmp.h>


FormUnitOpts::FormUnitOpts(wxPanel *parent, wxWindowID win_id, wxPoint position, SpellData* spelldata, int flags,std::function<void(int)> selection_cb)
{
    m_spelldata = spelldata;
    m_sel_cb = selection_cb;    
     
    
    // find desired buttons
    vector<SpellGraphicItem*> glyphs;
    if(flags & SpellMap::UNIT_OPT_UPPER)
    {
        glyphs.push_back(m_spelldata->gres.wm_sel_upper);
        m_labels.push_back("Upper");
        m_actions.push_back(SpellMap::UNIT_OPT_UPPER);
    }
    if(flags & SpellMap::UNIT_OPT_LOWER)
    {
        glyphs.push_back(m_spelldata->gres.wm_sel_lower);
        m_labels.push_back("Lower");
        m_actions.push_back(SpellMap::UNIT_OPT_LOWER);
    }
    if(flags & SpellMap::UNIT_OPT_SELECT)
    {
        glyphs.push_back(m_spelldata->gres.wm_sel_select);
        m_labels.push_back("Select");
        m_actions.push_back(SpellMap::UNIT_OPT_SELECT);
    }
    if(flags & SpellMap::UNIT_OPT_ATTACK)
    {
        glyphs.push_back(m_spelldata->gres.wm_sel_attack);
        m_labels.push_back("Attack");
        m_actions.push_back(SpellMap::UNIT_OPT_ATTACK);
    }
    if(flags & SpellMap::UNIT_OPT_MOVE)
    {
        glyphs.push_back(m_spelldata->gres.wm_sel_move);
        m_labels.push_back("Move");
        m_actions.push_back(SpellMap::UNIT_OPT_MOVE);
    }
    if(glyphs.size() != 2)
    {
        // fail: not all buttons defined or more??
        m_action = m_actions[0];
        wxCloseEvent ev(wxEVT_CLOSE_WINDOW,win_id);
        wxPostEvent(parent,ev);
        return;
    }

    // make panel background
    auto tab = m_spelldata->gres.wm_sel_tab;
    m_back_buff.assign(tab->x_size*tab->y_size,0x00);
    uint8_t* buf = m_back_buff.data();
    m_spelldata->gres.wm_sel_tab->Render(buf,&buf[tab->x_size*tab->y_size],tab->x_size,0,0);

    // make new form
    wxSize size ={tab->x_size, tab->y_size};
    form = new wxWindow(parent,win_id,position,size,wxBORDER_NONE);
    form->SetDoubleBuffered(true);

    // no info label yet
    info_label = "";
       
    for(int k = 0; k < 2; k++)
    {
        m_bmp_btn[k] = glyphs[k]->Render();
        buttons[k] = new wxPanel(form, ID_BTN + k, wxPoint(6 + k*28,4), wxSize(m_bmp_btn[k]->GetWidth(),m_bmp_btn[k]->GetHeight()));
        buttons[k]->SetBackgroundStyle(wxBG_STYLE_PAINT);
        buttons[k]->SetClientData(&m_labels[k]);
        buttons[k]->Bind(wxEVT_PAINT,&FormUnitOpts::OnPaintButton,this,ID_BTN + k);
        buttons[k]->Bind(wxEVT_LEAVE_WINDOW,&FormUnitOpts::OnButtonMouseHover,this,ID_BTN + k);
        buttons[k]->Bind(wxEVT_ENTER_WINDOW,&FormUnitOpts::OnButtonMouseHover,this,ID_BTN + k);
        buttons[k]->Bind(wxEVT_LEFT_DOWN,&FormUnitOpts::OnButtonClick,this,ID_BTN + k);
    }
    
    form->Bind(wxEVT_CLOSE_WINDOW,&FormUnitOpts::OnClose,this);
    form->Bind(wxEVT_PAINT,&FormUnitOpts::OnPaintTab,this);
}

FormUnitOpts::~FormUnitOpts()
{    
    delete m_bmp_btn[0];
    delete m_bmp_btn[1];
}

void FormUnitOpts::OnClose(wxCloseEvent& ev)
{
    wxPostEvent(form->GetParent(),ev);
    form->Destroy();
}

void FormUnitOpts::OnPaintTab(wxPaintEvent& event)
{        
    // make background
    auto back = m_back_buff;
    uint8_t *buf = back.data();
    
    // render info label
    int x_size = m_spelldata->gres.wm_sel_tab->x_size;
    int y_size = m_spelldata->gres.wm_sel_tab->y_size;
    m_spelldata->font->Render(buf,&buf[back.size()],x_size,3,29,54,13,info_label,0xFF,0xFE,SpellFont::FontShadow::RIGHT_DOWN);
    
    // render 24bit RGB data to raw bmp buffer
    uint8_t (*pal)[3] = m_spelldata->gres.wm_sel_tab->pal;
    wxBitmap bmp(x_size,y_size,24);    
    wxNativePixelData pdata(bmp);
    wxNativePixelData::Iterator p(pdata);
    for(int y = 0; y < y_size; y++)
    {
        uint8_t* scan = p.m_ptr;
        for(int x = 0; x < x_size; x++)
        {
            *scan++ = pal[*buf][2];
            *scan++ = pal[*buf][1];
            *scan++ = pal[*buf][0];
            buf++;
        }
        p.OffsetY(pdata,1);
    }
    
    wxPaintDC pdc(form);
    pdc.DrawBitmap(bmp,wxPoint(0,0));
}

void FormUnitOpts::OnPaintButton(wxPaintEvent& event)
{
    auto bmp = m_bmp_btn[event.GetId() - ID_BTN];
    auto pan = (wxPanel*)event.GetEventObject();
    wxPaintDC pdc(pan);
    pdc.DrawBitmap(*bmp,wxPoint(0,0));
}

void FormUnitOpts::OnButtonMouseHover(wxMouseEvent& event)
{
    if(event.GetEventType() == wxEVT_ENTER_WINDOW)
    {
        auto pan = (wxPanel*)event.GetEventObject();
        info_label = *(string*)pan->GetClientData();
    }
    else
        info_label = "";
    
    // repaint form
    form->Refresh();
}

void FormUnitOpts::OnButtonClick(wxMouseEvent& event)
{
    m_action = m_actions[event.GetId() - ID_BTN];
    form->Close();
}

void FormUnitOpts::ResultCallback()
{
    m_sel_cb(m_action);
}