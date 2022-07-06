#include "form_video_box.h"
#include "fs_archive.h"
#include "other.h"


#include <wx/rawbmp.h>
#include <wx/msgdlg.h>
#include <filesystem>
#include <stdexcept>

// load video and make form
FormVideoBox::FormVideoBox(wxPanel* parent,wxWindowID win_id,SpellData* spell_data,std::string name,int zoom)
{
    m_spelldata = spell_data;

    // build target archive path
    std::wstring archive_path;
    if(std::filesystem::path(name).extension().compare(".DP2") == 0)
    {
        // in CZE version, DP2 video loops are placed in SPEAKER.FS
        archive_path = std::filesystem::path(m_spelldata->cd_data_path) / std::filesystem::path("SPEAKER.FS");
        if(!std::filesystem::exists(archive_path))
            archive_path = std::filesystem::path(m_spelldata->data_path) / std::filesystem::path("SPEAKER.FS");
    }
    else
    {
        // all other vide stuff should be in MOVIE.FS
        archive_path = std::filesystem::path(m_spelldata->cd_data_path) / std::filesystem::path("MOVIE.FS");
    }
    if(!std::filesystem::exists(archive_path))
    {
        // archive not found
        throw runtime_error(string_format("Video file \"%s\" not found in \"%ls\" archive.", name, archive_path));
    }
        
        
    try {
        
        // load archive
        FSarchive fs(archive_path);
        
        // try get video file
        m_video = new SpellVideo(&fs, name);

    }catch(const runtime_error& error) {
        throw runtime_error(string_format("Loading video file \"%s\" failed (%s)!",name,error.what()));
    }
        
    // get frame size
    auto [frame_x,frame_y] = m_video->GetResolution();
    m_zoom = zoom;
    
    // no sound yet
    m_sound = new SpellSound(m_spelldata->sounds->channels, m_video->GetSound());
    m_frame_id = -1;
    
    // frame border graphics
    auto corn = m_spelldata->gres.wm_frame_corner;
    auto horz = m_spelldata->gres.wm_frame_horz;
    auto vert = m_spelldata->gres.wm_frame_vert;
    
    // desired panel width
    int x_size = frame_x*m_zoom + 2*corn->x_size;
    int y_size = frame_y*m_zoom + 2*corn->y_size;

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
    m_frame = new wxBitmap(x_size,y_size,24);        
    uint8_t* ptr = &buf[0];
    uint8_t (*pal)[3] = horz->pal;
    wxNativePixelData pdata(*m_frame);
    wxNativePixelData::Iterator p(pdata);
    for(int y = 0; y < y_size; y++)
    {
        uint8_t* scan = p.m_ptr;
        for(int x = 0; x < x_size; x++)
        {
            *scan++ = pal[*ptr][2];
            *scan++ = pal[*ptr][1];
            *scan++ = pal[*ptr][0];
            ptr++;
        }
        p.OffsetY(pdata,1);
    }
    buf.clear();
        
           
    // make new form
    wxPoint pos = {(parent->GetSize().x - x_size)/2, (parent->GetSize().y - y_size)/2};
    wxSize size ={x_size, y_size};
    form = new wxWindow(parent,win_id,pos,size,wxBORDER_NONE|wxSTAY_ON_TOP|wxBG_STYLE_PAINT);
    form->SetDoubleBuffered(true);
             
    form->Bind(wxEVT_CLOSE_WINDOW,&FormVideoBox::OnClose,this,win_id);
    form->Bind(wxEVT_PAINT,&FormVideoBox::OnPaintTab,this,win_id);
    form->Bind(wxEVT_KEY_UP,&FormVideoBox::OnKeyPress,this,win_id);
    form->Bind(wxEVT_LEAVE_WINDOW,&FormVideoBox::OnLeaveWin,this,win_id);
    form->Bind(wxEVT_LEFT_DOWN,&FormVideoBox::OnWinClick,this,win_id);
    form->Bind(wxEVT_LEFT_UP,&FormVideoBox::OnWinClick,this,win_id);
    form->Bind(wxEVT_MOTION,&FormVideoBox::OnWinMouseMove,this,win_id);
    
    // new audio frame callback
    form->Bind(wxEVT_THREAD,&FormVideoBox::OnNewAudioFrame,this);

    // start playback
    m_sound->Play(false,false,bind(&FormVideoBox::cbNewAudioFrame,this),0.010);

    form->SetFocus();
}

// cleanup
FormVideoBox::~FormVideoBox()
{           
    // optional kill narration sound playback
    StopPlayback();
    
    // loose video data
    delete m_video;
    m_video = NULL;
    
    // loose temp frame
    delete m_frame;
    m_frame = NULL;

    form->Destroy();
}

// callback when new audio frame is submitted (time critical!), called from another thread, so no touchy to GUI here!
void FormVideoBox::cbNewAudioFrame(void)
{
    wxThreadEvent* evt = new wxThreadEvent();
    wxQueueEvent(form,evt);
}

// on new audio frame (GUI thread event)
void FormVideoBox::OnNewAudioFrame(wxThreadEvent& event)
{
    if(!m_sound || !m_video)
        return;

    // get playback ime
    double time = m_sound->GetPlaybackTime();

    // update video frame
    int new_frame = m_video->GetFrameID(time);
    if(m_frame_id != new_frame)
    {
        m_frame_id = new_frame;
        form->Refresh();
    }

    if(m_sound->isDone())
        form->Close();
}

// stop audio playback
void FormVideoBox::StopPlayback()
{
    if(m_sound)
    {
        m_sound->Stop(3.0);
        delete m_sound;
    }
    m_sound = NULL;
}


void FormVideoBox::OnClose(wxCloseEvent& ev)
{       
    // terminate (and send message to parent)
    form->DeletePendingEvents();
    wxQueueEvent(form->GetParent(),new wxCloseEvent(ev));    
}

void FormVideoBox::OnPaintTab(wxPaintEvent& event)
{           
    // make local frame buffer
    int x_size = form->GetSize().x;
    int y_size = form->GetSize().y;
    
    // frame border graphics
    auto corn = m_spelldata->gres.wm_frame_corner;
    auto horz = m_spelldata->gres.wm_frame_horz;
    auto vert = m_spelldata->gres.wm_frame_vert;
    
    if(m_frame && m_video && m_video->GetFramesCount() && m_frame_id >= 0 && m_frame_id < m_video->GetFramesCount())
    {
        if(m_video->isCAN())
            return;

        // get frame size
        auto [frame_x,frame_y] = m_video->GetResolution();

        // render 24bit RGB data to raw bmp buffer		
        uint8_t *buf = m_video->GetFrame(m_frame_id);
        uint8_t *pal = m_video->GetPalette();
        wxNativePixelData pdata(*m_frame);
        wxNativePixelData::Iterator p(pdata);
        p.OffsetY(pdata,corn->y_size);
        int y_chunk = 0;
        for(int y = 0; y < frame_y*m_zoom; y++)
        {
            uint8_t* scan = &p.m_ptr[3*corn->x_size];
            int x_chunk = 0;
            for(int x = 0; x < frame_x*m_zoom; x++)
            {
                uint8_t pix = *buf;
                if(++x_chunk >= m_zoom)
                {
                    buf++;
                    x_chunk = 0;
                }
                *scan++ = pal[pix*3 + 2];
                *scan++ = pal[pix*3 + 1];
                *scan++ = pal[pix*3 + 0];
            }
            if(++y_chunk < m_zoom)
                buf -= frame_x;
            else
                y_chunk = 0;
            p.OffsetY(pdata,1);
        }

    }
   
    // blit image to screen
    wxPaintDC pdc(form);
    pdc.DrawBitmap(*m_frame,wxPoint(0,0));
}

void FormVideoBox::OnWinClick(wxMouseEvent& event)
{    
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
void FormVideoBox::OnWinMouseMove(wxMouseEvent& event)
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

void FormVideoBox::OnKeyPress(wxKeyEvent& event)
{
    // terminate
    if(event.GetKeyCode() == WXK_ESCAPE)
        form->Close();
}

void FormVideoBox::OnLeaveWin(wxMouseEvent& event)
{
    // keep focus
    form->SetFocus();
}

