#include "form_minimap.h"
#include "fs_archive.h"
#include "other.h"


#include <wx/rawbmp.h>
#include <wx/msgdlg.h>
#include <filesystem>
#include <stdexcept>
#include <algorithm>


FormMiniMap::FormMiniMap(wxPanel* parent,wxWindowID win_id,SpellData* spell_data,TMiniMap &minimap)
{
    m_spelldata = spell_data;
    m_minimap = minimap;

    int xs = m_minimap.bmp->GetWidth();
    int ys = m_minimap.bmp->GetHeight();
    if(xs > MAX_MINIMAP_X || ys >= MAX_MINIMAP_Y)
    {
        // resize to fit
        wxImage img = m_minimap.bmp->ConvertToImage();
        delete m_minimap.bmp;
        double scale = min((double)MAX_MINIMAP_X/xs,(double)MAX_MINIMAP_Y/ys);
        xs = (int)(scale*xs);
        ys = (int)(scale*ys);
        img.Rescale(xs,ys,wxIMAGE_QUALITY_HIGH);
        m_minimap.bmp = new wxBitmap(img);
    }
        
    // get frame size
    int surf_x = m_minimap.bmp->GetWidth();
    int surf_y = m_minimap.bmp->GetHeight();
    
    // frame border graphics
    auto corn = m_spelldata->gres.wm_frame_corner;
    auto horz = m_spelldata->gres.wm_frame_horz;
    auto vert = m_spelldata->gres.wm_frame_vert;
    
    // desired panel width
    int x_size = surf_x + 2*corn->x_size;
    int y_size = surf_y + 2*corn->y_size;

    // render frame in indexed colors    
    vector<uint8_t> buf(x_size*y_size,0x00);
    int pos_x = corn->x_size;
    do {
        horz->Render(&buf[0],&buf[0] + x_size*y_size,x_size,pos_x,0);
        horz->Render(&buf[0],&buf[0] + x_size*y_size,x_size,pos_x,y_size - horz->y_size);
        pos_x += horz->x_size;
    } while(pos_x < x_size);
    int pos_y = 10;
    do {
        vert->Render(&buf[0],&buf[0] + x_size*y_size,x_size,0,pos_y);
        vert->Render(&buf[0],&buf[0] + x_size*y_size,x_size,x_size - corn->x_size,pos_y);
        pos_y += vert->y_size;
    } while(pos_y < y_size);
    corn->Render(&buf[0],&buf[0] + x_size*y_size,x_size,0,0);
    corn->Render(&buf[0],&buf[0] + x_size*y_size,x_size,x_size - corn->x_size,0);
    corn->Render(&buf[0],&buf[0] + x_size*y_size,x_size,0,y_size - corn->y_size);
    corn->Render(&buf[0],&buf[0] + x_size*y_size,x_size,x_size - corn->x_size,y_size - corn->y_size);
    
    // convert frame to RGB frame buffer
    m_frame = new wxBitmap(x_size, y_size, 24); 
    uint8_t *fptr = &buf[0];
    uint8_t (*pal)[3] = horz->pal;
    wxNativePixelData fdata(*m_frame);
    wxNativePixelData::Iterator f(fdata);
    wxNativePixelData pdata(*m_minimap.bmp);
    wxNativePixelData::Iterator p(pdata);
    for(int y = 0; y < y_size; y++)
    {
        uint8_t* scan = f.m_ptr;
        uint8_t* psrc = p.m_ptr;
        for(int x = 0; x < x_size; x++)
        {
            if(*fptr)
            {
                *scan++ = pal[*fptr][2];
                *scan++ = pal[*fptr][1];
                *scan++ = pal[*fptr][0];
            }
            else
            {
                *scan++ = *psrc++;
                *scan++ = *psrc++;
                *scan++ = *psrc++;
            }
            fptr++;
        }        
        f.OffsetY(fdata,1);
        if(y > corn->y_size)
            p.OffsetY(pdata,1);
    }
    buf.clear();
    delete m_minimap.bmp;
    m_minimap.bmp = NULL;
        
           
    // make new form
    wxPoint pos = {(parent->GetSize().x - x_size)/2, (parent->GetSize().y - y_size)/2};
    wxSize size ={x_size, y_size};
    form = new wxWindow(parent,win_id,pos,size,wxBORDER_NONE|wxSTAY_ON_TOP|wxBG_STYLE_PAINT);
    form->SetDoubleBuffered(true);
    form->Show();
             
    form->Bind(wxEVT_CLOSE_WINDOW,&FormMiniMap::OnClose,this,win_id);
    form->Bind(wxEVT_PAINT,&FormMiniMap::OnPaintTab,this,win_id);
    form->Bind(wxEVT_KEY_UP,&FormMiniMap::OnKeyPress,this,win_id);
    form->Bind(wxEVT_LEAVE_WINDOW,&FormMiniMap::OnLeaveWin,this,win_id);
    form->Bind(wxEVT_LEFT_DOWN,&FormMiniMap::OnWinClick,this,win_id);
    form->Bind(wxEVT_LEFT_UP,&FormMiniMap::OnWinClick,this,win_id);
    form->Bind(wxEVT_MOTION,&FormMiniMap::OnWinClick,this,win_id);
    
    // go focus, without this it won't respond to key press!
    form->SetFocus();    
}

// cleanup
FormMiniMap::~FormMiniMap()
{               
    // loose temp frame
    delete m_frame;
    m_frame = NULL;

    form->Destroy();
}


void FormMiniMap::OnClose(wxCloseEvent& ev)
{       
    // terminate (and send message to parent)
    form->DeletePendingEvents();
    wxQueueEvent(form->GetParent(),new wxCloseEvent(ev));
    //wxQueueEvent(form->GetParent(),new wxCloseEvent(ev));    
}

void FormMiniMap::OnPaintTab(wxPaintEvent& event)
{              
    // frame border graphics
    auto corn = m_spelldata->gres.wm_frame_corner;
    
    wxBitmap* bmp = m_frame;
    // render map
    wxPaintDC pdc(form);
    pdc.DrawBitmap(*bmp,wxPoint(0,0));
    // render selection:
    int bmp_x = bmp->GetWidth() - 2*corn->x_size;
    int bmp_y = bmp->GetHeight() - 2*corn->y_size;
    //auto [surf_x,surf_y] = m_minimap.map->GetMapSurfaceSize();
    /*int surf_x = m_buffer.GetWidth();
    int surf_y = m_buffer.GetHeight();*/
    auto [surf_x,surf_y] = m_minimap.scroll->GetSurface();
    auto [scroll_x,scroll_y] = m_minimap.scroll->GetScroll();
    scroll_x -= m_minimap.source_x_ofs;
    scroll_y -= m_minimap.source_y_ofs;
    wxPoint c1 = wxPoint(corn->x_size + scroll_x*bmp_x/m_minimap.source_x,corn->y_size + scroll_y*bmp_y/m_minimap.source_y);
    wxPoint c2 = wxPoint(min(corn->x_size + (scroll_x+surf_x)*bmp_x/m_minimap.source_x,bmp->GetWidth()-corn->x_size), min(corn->y_size + (scroll_y+surf_y)*bmp_y/m_minimap.source_y,bmp->GetHeight()-corn->y_size));
    pdc.SetBrush(wxBrush(wxColor(0,0,0),wxBRUSHSTYLE_TRANSPARENT));
    pdc.SetPen(wxPen(wxColor(255,0,0),3));
    pdc.DrawRectangle(c1.x,c1.y,c2.x-c1.x,c2.y-c1.y);
    event.Skip();
    
}

void FormMiniMap::OnWinClick(wxMouseEvent& event)
{    
    if(!event.LeftIsDown() && event.GetEventType() != wxEVT_LEFT_UP)
        return;

    // frame border graphics
    auto corn = m_spelldata->gres.wm_frame_corner;    // get bitmap to render

    wxBitmap* bmp = m_frame;
    // current position
    int bmp_x = bmp->GetWidth() - 2*corn->x_size;
    int bmp_y = bmp->GetHeight() - 2*corn->y_size;
    //auto [map_x,map_y] = spell_map->GetMapSurfaceSize();
    auto [surf_x,surf_y] = m_minimap.scroll->GetSurface();
    /*int surf_x = m_buffer.GetWidth();
    int surf_y = m_buffer.GetHeight();*/
    int point_x = event.GetX() + corn->x_size;
    int point_y = event.GetY() + corn->x_size;
    // set new pos
    int pos_x = point_x*m_minimap.source_x/bmp_x - surf_x/2 + m_minimap.source_x_ofs;
    int pos_y = point_y*m_minimap.source_y/bmp_y - surf_y/2 + m_minimap.source_y_ofs;
    m_minimap.scroll->SetPos(pos_x,pos_y);
    m_minimap.scroll->CheckScroll(m_minimap.source_x_ofs,m_minimap.source_y_ofs,m_minimap.source_x - surf_x + m_minimap.source_x_ofs,m_minimap.source_y - surf_y + m_minimap.source_y_ofs);
    // repaint map
    form->Refresh();
    wxQueueEvent(form->GetParent(),new wxThreadEvent());

    //canvas->Refresh();
}
void FormMiniMap::OnWinMouseMove(wxMouseEvent& event)
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

void FormMiniMap::OnKeyPress(wxKeyEvent& event)
{
    // terminate
    if(event.GetKeyCode() == WXK_ESCAPE)
        form->Close();
}

void FormMiniMap::OnLeaveWin(wxMouseEvent& event)
{
    // keep focus
    form->SetFocus();
}

