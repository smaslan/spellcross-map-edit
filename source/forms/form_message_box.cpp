#include "form_message_box.h"
#include "map.h"

#include <wx/rawbmp.h>


FormMsgBox::FormMsgBox(wxPanel* parent,wxWindowID win_id,SpellData* spell_data,SpellMap* spell_map,SpellTextRec* text,SpellMsgOptions options,std::function<void(bool)> selection_cb)
{
    m_spelldata = spell_data;
    m_spell_map = spell_map;
    m_sel_cb = selection_cb;
    m_options = options;

    // frame border graphics
    auto corn = m_spelldata->gres.wm_frame_corner;
    auto horz = m_spelldata->gres.wm_frame_horz;
    auto vert = m_spelldata->gres.wm_frame_vert;

    // desired panel width
    int x_size = 450;
    
    // word wrap text
    int text_x_size = x_size - 4*corn->x_size;
    m_chunks = text->WordWrap(spell_data->font,text_x_size);
    int x_text = 0;
    int y_size = 0;
    for(auto & line : m_chunks)
    {
        y_size = max(y_size, line.pos_y); 
        x_text = max(x_text, line.size_x);
    }
    y_size += spell_data->font->GetHeight() + 4*corn->y_size;
    x_size -= max(text_x_size - x_text,0);


    if(options == SpellMsgOptions::YESNO)
        y_size += BTN_Y_GAP + BTN_Y;

    // make new form
    wxPoint pos = {(parent->GetSize().x - x_size)/2, (parent->GetSize().y - y_size)/2};
    wxSize size ={x_size, y_size};
    form = new wxWindow(parent,win_id,pos,size,wxBORDER_NONE|wxSTAY_ON_TOP|wxBG_STYLE_PAINT);
    form->SetDoubleBuffered(true);

    if(options == SpellMsgOptions::YESNO)
    {
        int btn_x[2] = {x_size/2 - BTN_X_GAP/2 - BTN_X, x_size/2 + BTN_X_GAP/2};
        int btn_y = y_size - corn->y_size - BTN_Y_GAP - BTN_Y;
        m_labels = {"YES", "NO"};
        m_results = {SpellMsgResult::YES, SpellMsgResult::NO};
        m_hovers = {false, false};
        
        for(int k = 0; k < 2; k++)
        {
            m_buttons[k] = new wxPanel(form,ID_BTN + k,wxPoint(btn_x[k],btn_y),wxSize(BTN_X,BTN_Y));
            m_buttons[k]->SetBackgroundStyle(wxBG_STYLE_PAINT);
            m_buttons[k]->SetClientData(&m_labels[k]);
            m_buttons[k]->Bind(wxEVT_PAINT,&FormMsgBox::OnPaintButton,this,ID_BTN + k);
            m_buttons[k]->Bind(wxEVT_LEAVE_WINDOW,&FormMsgBox::OnButtonMouseHover,this,ID_BTN + k);
            m_buttons[k]->Bind(wxEVT_ENTER_WINDOW,&FormMsgBox::OnButtonMouseHover,this,ID_BTN + k);
            m_buttons[k]->Bind(wxEVT_LEFT_UP,&FormMsgBox::OnButtonClick,this,ID_BTN + k);
        }
    }

    // default exit action
    m_result = SpellMsgResult::CLOSE;
    
    form->Bind(wxEVT_CLOSE_WINDOW,&FormMsgBox::OnClose,this,win_id);
    form->Bind(wxEVT_PAINT,&FormMsgBox::OnPaintTab,this,win_id);
    form->Bind(wxEVT_KEY_UP,&FormMsgBox::OnKeyPress,this,win_id);
    form->Bind(wxEVT_LEAVE_WINDOW,&FormMsgBox::OnLeaveWin,this,win_id);
    form->Bind(wxEVT_LEFT_DOWN,&FormMsgBox::OnWinClick,this,win_id);
    form->Bind(wxEVT_LEFT_UP,&FormMsgBox::OnWinClick,this,win_id);
    form->Bind(wxEVT_MOTION,&FormMsgBox::OnWinMouseMove,this,win_id);
    
    // play optional narration sound
    m_sound = NULL;
    if(text->audio)
    {
        m_sound = new SpellSound(m_spelldata->sounds->channels, text->audio);
        m_sound->Play();
    }

    form->SetFocus();
}

FormMsgBox::~FormMsgBox()
{   
    form->Destroy();
}

void FormMsgBox::OnClose(wxCloseEvent& ev)
{
    // optional kill narration sound playback
    if(m_sound)
    {
        m_sound->Stop();
        while(!m_sound->isDone())
            this_thread::sleep_for(10ms);
        delete m_sound;
    }

    // terminate (and send message to parent)
    form->DeletePendingEvents();
    wxQueueEvent(form->GetParent(),new wxCloseEvent(ev));    
    //form->Destroy();
    //ev.Skip();
}

void FormMsgBox::OnPaintTab(wxPaintEvent& event)
{        
    // make local frame buffer
    int x_size = form->GetSize().x;
    int y_size = form->GetSize().y;
    uint8_t *buf = new uint8_t[x_size*y_size];

    // get current map render surface behind this window
    int x_pos = form->GetPosition().x;
    int y_pos = form->GetPosition().y;
    m_spell_map->GetRender(buf, x_size, y_size, x_pos, y_pos);

    // make semi transparent back
    for(int k = 0; k < x_size*y_size; k++)
        buf[k] = m_spell_map->terrain->filter.darkpal[buf[k]];


    // frame border graphics
    auto corn = m_spelldata->gres.wm_frame_corner;
    auto horz = m_spelldata->gres.wm_frame_horz;
    auto vert = m_spelldata->gres.wm_frame_vert;

    // render text chunks
    int text_y_ofs = 2*corn->y_size;
    for(auto & line : m_chunks)
        m_spelldata->font->Render(buf, &buf[x_size*y_size], x_size, line.pos_x + 4*corn->x_size/2, line.pos_y + text_y_ofs, line.text, 0xFF);      

    // render frame
    int pos_x = corn->x_size;
    do {
        horz->Render(buf,&buf[x_size*y_size],x_size,pos_x,0);
        horz->Render(buf,&buf[x_size*y_size],x_size,pos_x,y_size - horz->y_size);
        pos_x += horz->x_size;
    }while(pos_x < x_size);
    int pos_y = 10;
    do {
        vert->Render(buf,&buf[x_size*y_size],x_size,0,pos_y);
        vert->Render(buf,&buf[x_size*y_size],x_size,x_size - corn->x_size,pos_y);
        pos_y += vert->y_size;
    }while(pos_y < y_size);
    corn->Render(buf,&buf[x_size*y_size],x_size,0,0);
    corn->Render(buf,&buf[x_size*y_size],x_size,x_size - corn->x_size,0);
    corn->Render(buf,&buf[x_size*y_size],x_size,0,y_size - corn->y_size);
    corn->Render(buf,&buf[x_size*y_size],x_size,x_size - corn->x_size,y_size - corn->y_size);


    // blit window
    uint8_t* pal = (uint8_t*)m_spell_map->terrain->pal;
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
    delete[] buf;      
}

void FormMsgBox::OnWinClick(wxMouseEvent& event)
{    
    if(!m_in_move && m_options == SpellMsgOptions::NONE)
        form->Close();

    if(event.LeftDown())
    {
        //form->CaptureMouse();
        m_click_pos = event.GetPosition() + form->GetPosition();
        m_in_move = true;
    }
    else if(event.LeftUp())
    {
        //form->ReleaseMouse();
        m_in_move = false;
    }    
}
void FormMsgBox::OnWinMouseMove(wxMouseEvent& event)
{
    if(m_in_move)
    {
        auto new_pos = event.GetPosition() + form->GetPosition();
        auto delta = new_pos - m_click_pos;
        form->SetPosition(form->GetPosition() + delta);
        m_click_pos = new_pos;
        form->Refresh();
    }
}

void FormMsgBox::OnKeyPress(wxKeyEvent& event)
{
    // terminate
    if(event.GetKeyCode() == WXK_ESCAPE)
        form->Close();

    if(m_options == SpellMsgOptions::NONE && (event.GetKeyCode() == WXK_RETURN || event.GetKeyCode() == WXK_NUMPAD_ENTER))
        form->Close();
}

void FormMsgBox::OnLeaveWin(wxMouseEvent& event)
{
    // keep focus
    form->SetFocus();
}

void FormMsgBox::OnPaintButton(wxPaintEvent& event)
{
    auto btn = (wxPanel*)event.GetEventObject();
    int is_hover = m_hovers[event.GetId() - ID_BTN];
    
    // make local frame buffer
    int x_size = btn->GetSize().x;
    int y_size = btn->GetSize().y;
    uint8_t* buf = new uint8_t[x_size*y_size];

    // get current map render surface behind this window
    int x_pos = btn->GetPosition().x + form->GetPosition().x;
    int y_pos = btn->GetPosition().y + form->GetPosition().y;
    m_spell_map->GetRender(buf,x_size,y_size,x_pos,y_pos);

    // make semi transparent back
    if(is_hover)
        std::memset(buf, 231, x_size*y_size);
    else
    {
        uint8_t* filter = m_spell_map->terrain->filter.darkpal;;
        for(int k = 0; k < x_size*y_size; k++)
            buf[k] = filter[buf[k]];
    }

    // draw frame
    memset(&buf[0],0xFF,x_size);
    memset(&buf[(y_size-1)*x_size],0xFF,x_size);
    for(int y = 0; y < y_size; y++)
    {
        buf[y*x_size] = 0xFF;
        buf[y*x_size + x_size-1] = 0xFF;
    }

    // draw button label
    auto label = (string*)btn->GetClientData();
    m_spelldata->font->Render(buf,&buf[x_size*y_size],x_size,0,0,x_size,y_size,*label,(is_hover)?254:0xFF,(is_hover)?-1:0xFE,(is_hover)?(SpellFont::FontShadow::NONE):(SpellFont::FontShadow::RIGHT_DOWN));
      
    // blit window
    uint8_t* pal = (uint8_t*)m_spell_map->terrain->pal;
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
    delete[] buf;
}

void FormMsgBox::OnButtonClick(wxMouseEvent& event)
{    
    m_result = m_results[event.GetId() - ID_BTN];
    form->Close();
}

void FormMsgBox::OnButtonMouseHover(wxMouseEvent& event)
{
    m_hovers[event.GetId() - ID_BTN] = event.GetEventType() == wxEVT_ENTER_WINDOW;
    auto btn = (wxPanel*)event.GetEventObject();
    btn->Refresh();
}

void FormMsgBox::ResultCallback()
{
    if(m_sel_cb)
        m_sel_cb(m_result == SpellMsgResult::YES);
}

