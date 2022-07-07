#include "form_map_options.h"
#include "map.h"

#include <wx/rawbmp.h>


FormMapOptions::FormMapOptions(wxPanel* parent,wxWindowID win_id,SpellMap* spell_map)
{
    m_spell_map = spell_map;
    m_spelldata = spell_map->spelldata;    
    
    // bacground frame
    auto grp_frame = m_spelldata->gres.wm_map_opt_frame;   

    // panel dimensiones
    int x_size = grp_frame->x_size;
    int y_size = grp_frame->y_size;
   
    // make new form in center of parent
    wxPoint pos = {(parent->GetSize().x - x_size)/2, (parent->GetSize().y - y_size)/2};
    wxSize size ={x_size, y_size};
    form = new wxPanel(parent,win_id,pos,size,wxBORDER_NONE|wxFRAME_FLOAT_ON_PARENT|wxBG_STYLE_PAINT);
    form->SetDoubleBuffered(true);
    
    // make load/save buttons
    auto btn_glyph = m_spelldata->gres.wm_map_opt_btn_disabled;
    int btn_y_pos = 284;
    vector<int> btn_x_list = {34,121};
    vector<int> btn_wx_id = {wxID_BTN_SAVE, wxID_BTN_LOAD};
    m_buttons.reserve(2); // allocate all now to keep element addresses!
    for(int k = 0; k < 2; k++)
    {
        m_buttons.emplace_back();
        m_buttons.back().action_id = btn_wx_id[k];
        m_buttons.back().is_down = false;
        m_buttons.back().is_hover = false;
        m_buttons.back().panel = new wxPanel(form,wxID_ANY,wxPoint(btn_x_list[k],btn_y_pos),wxSize(btn_glyph->x_size,btn_glyph->y_size),wxBG_STYLE_PAINT);
        m_buttons.back().panel->SetDoubleBuffered(true);
        m_buttons.back().panel->Bind(wxEVT_PAINT,&FormMapOptions::OnPaintButton,this);
        m_buttons.back().panel->Bind(wxEVT_LEAVE_WINDOW,&FormMapOptions::OnButtonMouseHover,this);
        m_buttons.back().panel->Bind(wxEVT_ENTER_WINDOW,&FormMapOptions::OnButtonMouseHover,this);
        m_buttons.back().panel->Bind(wxEVT_LEFT_DOWN,&FormMapOptions::OnButtonClick,this);
        m_buttons.back().panel->Bind(wxEVT_LEFT_UP,&FormMapOptions::OnButtonClick,this);
        m_buttons.back().panel->SetClientData((void*)&m_buttons.back());
    }

    // gamma correction scrollbar (pos=32,133 sz=132,16)
    wxScrollBar *scroll_gamma = new wxScrollBar(form,wxID_SCROLL_GAMMA,wxPoint(32,133),wxSize(133,15),wxSB_HORIZONTAL|wxSB_FLAT);
    scroll_gamma->SetBackgroundColour(wxColor(90,90,90));
    scroll_gamma->Bind(wxEVT_SCROLL_THUMBTRACK,&FormMapOptions::OnScroll,this,wxID_SCROLL_GAMMA);
    scroll_gamma->Bind(wxEVT_SCROLL_LINEUP,&FormMapOptions::OnScroll,this,wxID_SCROLL_GAMMA);
    scroll_gamma->Bind(wxEVT_SCROLL_LINEDOWN,&FormMapOptions::OnScroll,this,wxID_SCROLL_GAMMA);
    scroll_gamma->SetRange(20-7+1);
    int gamma = min((int)(m_spell_map->GetGamma()*10.0-7.0),20-7);
    scroll_gamma->SetThumbPosition(gamma);

    // music volume scrollbar (pos=32,182 sz=132,16)
    wxScrollBar* scroll_music = new wxScrollBar(form,wxID_SCROLL_MUSIC,wxPoint(32,182),wxSize(133,15),wxSB_HORIZONTAL|wxSB_FLAT);
    scroll_music->SetBackgroundColour(wxColor(90,90,90));
    scroll_music->Bind(wxEVT_SCROLL_THUMBTRACK,&FormMapOptions::OnScroll,this,wxID_SCROLL_MUSIC);
    scroll_music->Bind(wxEVT_SCROLL_LINEUP,&FormMapOptions::OnScroll,this,wxID_SCROLL_MUSIC);
    scroll_music->Bind(wxEVT_SCROLL_LINEDOWN,&FormMapOptions::OnScroll,this,wxID_SCROLL_MUSIC);
    scroll_music->SetRange(51);
    double music_volume = m_spelldata->midi->GetVolume();
    scroll_music->SetThumbPosition(music_volume*50);

    // sound volume scrollbar (pos=32,232 sz=132,16)
    wxScrollBar* scroll_sound = new wxScrollBar(form,wxID_SCROLL_SOUND,wxPoint(32,232),wxSize(133,15),wxSB_HORIZONTAL|wxSB_FLAT);
    scroll_sound->SetBackgroundColour(wxColor(90,90,90));
    scroll_sound->Bind(wxEVT_SCROLL_THUMBTRACK,&FormMapOptions::OnScroll,this,wxID_SCROLL_SOUND);
    scroll_sound->Bind(wxEVT_SCROLL_LINEUP,&FormMapOptions::OnScroll,this,wxID_SCROLL_SOUND);
    scroll_sound->Bind(wxEVT_SCROLL_LINEDOWN,&FormMapOptions::OnScroll,this,wxID_SCROLL_SOUND);
    scroll_sound->Bind(wxEVT_SCROLL_PAGEUP,&FormMapOptions::OnScroll,this,wxID_SCROLL_SOUND);
    scroll_sound->Bind(wxEVT_SCROLL_PAGEDOWN,&FormMapOptions::OnScroll,this,wxID_SCROLL_SOUND);
    scroll_sound->SetRange(51);
    double sound_volume = m_spell_map->spelldata->sounds->channels->GetVolume();
    scroll_sound->SetThumbPosition(sound_volume*50);
    

    form->Show();    
    
    form->Bind(wxEVT_CLOSE_WINDOW,&FormMapOptions::OnClose,this,win_id);
    form->Bind(wxEVT_PAINT,&FormMapOptions::OnPaintTab,this,win_id);
    form->Bind(wxEVT_KEY_UP,&FormMapOptions::OnKeyPress,this,win_id);
    form->Bind(wxEVT_LEAVE_WINDOW,&FormMapOptions::OnLeaveWin,this,win_id);
    form->Bind(wxEVT_LEFT_DOWN,&FormMapOptions::OnWinClick,this,win_id);
    form->Bind(wxEVT_LEFT_UP,&FormMapOptions::OnWinClick,this,win_id);
    form->Bind(wxEVT_MOTION,&FormMapOptions::OnWinMouseMove,this,win_id);
        
}

FormMapOptions::~FormMapOptions()
{   
    form->Destroy();
}

void FormMapOptions::OnClose(wxCloseEvent& ev)
{    
    // terminate (and send message to parent)
    form->DeletePendingEvents();    
    wxPostEvent(form->GetParent(),ev);    
}

// handle scroll bars
void FormMapOptions::OnScroll(wxScrollEvent& event)
{
    auto scroll = (wxScrollBar*)event.GetEventObject();
    if(event.GetId() == wxID_SCROLL_GAMMA)
    {
        // update gamma correction        
        double gamma = 0.1*(double)scroll->GetThumbPosition() + 0.7;
        m_spell_map->SetGamma(gamma);
        form->Refresh();
    }
    else if(event.GetId() == wxID_SCROLL_MUSIC)
    {
        // update music volume        
        double volume = 0.02*(double)scroll->GetThumbPosition();
        m_spelldata->midi->SetVolume(volume);
        form->Refresh();
    }
    else if(event.GetId() == wxID_SCROLL_SOUND)
    {
        // update sound volume
        double volume = 0.02*(double)scroll->GetThumbPosition();
        m_spelldata->sounds->channels->SetVolume(volume);
        form->Refresh();
    }
    
}

void FormMapOptions::OnPaintTab(wxPaintEvent& event)
{        
    // bacground frame
    auto grp_frame = m_spelldata->gres.wm_map_opt_frame;
    
    // make local frame buffer
    int x_size = form->GetSize().x;
    int y_size = form->GetSize().y;
    
    // make render buffer
    if(m_buffer.empty() || m_buffer.size() != x_size*y_size)
    {
        m_buffer.reserve(x_size*y_size);
        m_mask.assign(x_size*y_size,0xFF);
        grp_frame->RenderMask(m_mask.data(),m_mask.data() + x_size*y_size);
    }
    m_buffer.assign(x_size*y_size,0x00);    
    uint8_t *buf = m_buffer.data();

    // get current map render surface behind this window
    int x_pos = form->GetPosition().x;
    int y_pos = form->GetPosition().y;
    m_spell_map->GetRender(buf, x_size, y_size, x_pos, y_pos);

    // make semi transparent back
    for(int k = 0; k < x_size*y_size; k++)
        if(m_mask[k])
            buf[k] = m_spell_map->terrain->filter.darker[buf[k]];

    // render back frame
    grp_frame->Render(buf,&buf[x_size*y_size],x_size,0,0);

    // move label pos=61,16 sz=87,16
    m_spelldata->font->Render(buf,&buf[x_size*y_size],x_size,61,16, 87,16, "MOVEMENT",229,254,SpellFont::DIAG3);
    // alinace label pos=16,36 sz=82,16
    m_spelldata->font->Render(buf,&buf[x_size*y_size],x_size,16,36,82,16,"Aliance",229,254,SpellFont::DIAG3);
    // os label pos=102,36 sz=82,16
    m_spelldata->font->Render(buf,&buf[x_size*y_size],x_size,102,36,82,16,"Other Side",229,254,SpellFont::DIAG3);

    // render saves count 88,287  24,18
    string str_saves = string_format("%d",m_spell_map->saves->canSave());
    m_spelldata->font->Render(buf,&buf[x_size*y_size],x_size, 89,287, 24,18, str_saves,253,254,SpellFont::DIAG);

    // render gamma correcion label pos=32,113 sz=132,16
    string str_gamma = string_format("GAMMA (%#.1f)",m_spell_map->GetGamma());
    m_spelldata->font->Render(buf,&buf[x_size*y_size],x_size,32,113,132,16,str_gamma,232,254,SpellFont::DIAG3);

    // render music volume label pos=32,162 sz=132,16
    double mus_volume = m_spelldata->midi->GetVolume();
    string str_mus_volume = string_format("MUSIC (%.0f%%)",(mus_volume*100.0));
    m_spelldata->font->Render(buf,&buf[x_size*y_size],x_size,32,162,132,16,str_mus_volume,232,254,SpellFont::DIAG3);

    // render sound volume label pos=32,212 sz=132,16
    double snd_volume = m_spelldata->sounds->channels->GetVolume();
    string str_snd_volume = string_format("SOUNDS (%.0f%%)",(snd_volume*100.0));
    m_spelldata->font->Render(buf,&buf[x_size*y_size],x_size,32,212,132,16,str_snd_volume,232,254,SpellFont::DIAG3);

    // pos=32,262 sz=51,15
    m_spelldata->font->Render(buf,&buf[x_size*y_size],x_size,32,262,51,15,"Save",232,254,SpellFont::DIAG3);
    // pos=118,262 sz=51,15
    m_spelldata->font->Render(buf,&buf[x_size*y_size],x_size,118,262,51,15,"Load",232,254,SpellFont::DIAG3);

    // objectives heading label pos=195,66 sz=215,16
    m_spelldata->font->Render(buf,&buf[x_size*y_size],x_size,195,66,215,16,"MISSION OBJECTIVES",229,254,SpellFont::DIAG3);    
    // objectives area pos=203,90, y_step = 20, sz=198,16
    int obj_y_pos = 90;
    auto &evt_list = m_spell_map->events->GetEvents();
    for(auto & evt : evt_list)
    {
        if(!evt->is_objective)
            continue;
        
        uint8_t color = (evt->isDone())?232:253;
        string obj_label = wstring2stringCP895(evt->label) +" " + ((evt->isDone())?"\x7B":"\x7C");
        m_spelldata->font->Render(buf,&buf[x_size*y_size],x_size,203,obj_y_pos,198,16,obj_label,color,254,SpellFont::DIAG3);
        
        obj_y_pos += 20;
    }


    // blit window
    uint8_t* pal = m_spell_map->GetPalette();
    uint8_t* ptr = buf;
    wxBitmap bmp(x_size,y_size,24);
    wxNativePixelData pdata(bmp);
    wxNativePixelData::Iterator p(pdata);
    for(int y = 0; y < y_size; y++)
    {
        uint8_t* scan = p.m_ptr;
        for(int x = 0; x < x_size; x++)
        {
            *scan++ = pal[*ptr*3+2];
            *scan++ = pal[*ptr*3+1];
            *scan++ = pal[*ptr*3+0];
            ptr++;
        }
        p.OffsetY(pdata,1);
    }    
    wxPaintDC pdc(form);
    pdc.DrawBitmap(bmp,wxPoint(0,0));

}

void FormMapOptions::OnWinClick(wxMouseEvent& event)
{        
    if(event.LeftDown())
    {
        //form->CaptureMouse();
        m_click_pos = event.GetPosition() + form->GetPosition();
        m_in_move = true;
        m_was_moved = false;
    }
    else if(event.LeftUp())
    {
        //form->ReleaseMouse();

        if(!m_was_moved)
            form->Close();

        m_in_move = false;
    }    
}
void FormMapOptions::OnWinMouseMove(wxMouseEvent& event)
{
    if(m_in_move)
    {
        auto new_pos = event.GetPosition() + form->GetPosition();
        auto delta = new_pos - m_click_pos;
        form->SetPosition(form->GetPosition() + delta);
        m_click_pos = new_pos;
        m_was_moved = true;
        form->Refresh();
    }
}

void FormMapOptions::OnKeyPress(wxKeyEvent& event)
{
    // terminate
    if(event.GetKeyCode() == WXK_ESCAPE)
        form->Close();

    if(event.GetKeyCode() == WXK_RETURN || event.GetKeyCode() == WXK_NUMPAD_ENTER)
        form->Close();
}

void FormMapOptions::OnLeaveWin(wxMouseEvent& event)
{
    // keep focus
    form->SetFocus();
}

void FormMapOptions::OnPaintButton(wxPaintEvent& event)
{
    auto btn = (wxPanel*)event.GetEventObject();
    auto btn_rec = (FormMapOptionsBtn*)btn->GetClientData();
    
    // make local frame buffer
    int x_size = btn->GetSize().x;
    int y_size = btn->GetSize().y;
    vector<uint8_t> buffer(x_size*y_size);
    uint8_t *buf = buffer.data();
    
    // get current map render surface behind this window
    int x_pos = btn->GetPosition().x + form->GetPosition().x;
    int y_pos = btn->GetPosition().y + form->GetPosition().y;
    m_spell_map->GetRender(buf,x_size,y_size,x_pos,y_pos);

    // make black back because the buttons have transparencies where they should not for whatever reason
    memset(buf,254,x_size*y_size);

    // select glyph
    SpellGraphicItem *btn_glyph;
    if(btn_rec->is_down)
        btn_glyph = m_spelldata->gres.wm_map_opt_btn_down;
    else if(btn_rec->is_hover)
        btn_glyph = m_spelldata->gres.wm_map_opt_btn_hover;
    else
        btn_glyph = m_spelldata->gres.wm_map_opt_btn_idle;

    // render back frame
    btn_glyph->Render(buf,&buf[x_size*y_size],x_size,0,0);
      
    // blit window
    uint8_t* pal = (uint8_t*)m_spell_map->GetPalette();
    uint8_t* ptr = buf;
    wxBitmap bmp(x_size,y_size,24);
    wxNativePixelData pdata(bmp);
    wxNativePixelData::Iterator p(pdata);
    for(int y = 0; y < y_size; y++)
    {
        uint8_t* scan = p.m_ptr;
        for(int x = 0; x < x_size; x++)
        {
            *scan++ = pal[*ptr*3+2];
            *scan++ = pal[*ptr*3+1];
            *scan++ = pal[*ptr*3+0];
            ptr++;
        }
        p.OffsetY(pdata,1);
    }
    wxPaintDC pdc(btn);
    pdc.DrawBitmap(bmp,wxPoint(0,0));
}

void FormMapOptions::OnButtonClick(wxMouseEvent& event)
{    
    auto btn = (wxPanel*)event.GetEventObject();
    auto btn_rec = (FormMapOptionsBtn*)btn->GetClientData();

    if(event.GetEventType() == wxEVT_LEFT_UP)
    {
        btn_rec->is_down = false;
    }
    else if(event.GetEventType() == wxEVT_LEFT_DOWN)
    {
        btn_rec->is_down = true;             

        if(btn_rec->action_id == wxID_BTN_SAVE && m_spell_map->saves->canSave())
            m_spell_map->saves->Save();
        else if(btn_rec->action_id == wxID_BTN_LOAD && m_spell_map->saves->canLoad())
        {
            m_spell_map->saves->Load();
            form->Close();
        }

        
    }
    btn->Refresh();
}

void FormMapOptions::OnButtonMouseHover(wxMouseEvent& event)
{
    auto btn = (wxPanel*)event.GetEventObject();
    auto btn_rec = (FormMapOptionsBtn*)btn->GetClientData();
    
    if(event.GetEventType() == wxEVT_LEAVE_WINDOW)
    {
        btn_rec->is_hover = false;
        btn_rec->is_down = false;
    }
    else if(event.GetEventType() == wxEVT_ENTER_WINDOW)
    {
        btn_rec->is_hover = true;
    }
    btn->Refresh();
}

void FormMapOptions::ResultCallback()
{
}

