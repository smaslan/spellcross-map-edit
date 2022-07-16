#include "form_map_units_list.h"
#include "other.h"

#include <wx/rawbmp.h>
#include <wx/msgdlg.h>
#include <filesystem>
#include <stdexcept>
#include <algorithm>


FormMapUnits::FormMapUnits(wxPanel* parent,wxWindowID win_id,SpellData* spell_data,SpellMap* spell_map)
{
    canvas = parent;
    m_win_id = win_id;
    m_spelldata = spell_data;
    m_spellmap = spell_map;

    // make centered grid of panels
    grid = new wxGridSizer(1,1,10,10);
    sizer = new wxBoxSizer(wxVERTICAL);    
    sizer->AddStretchSpacer(1);
    sizer->Add(grid, 0,wxALL|wxALIGN_CENTER);
    sizer->AddStretchSpacer(1);
    sizer->AddSpacer(HUD_Y_GAP);
    canvas->SetSizer(sizer);
    canvas->Layout();
    canvas->Bind(wxEVT_SIZE, &FormMapUnits::OnResizeCanvas, this);
    canvas->Bind(wxEVT_KEY_UP,&FormMapUnits::OnCanvasKey,this);
    
    // initialize grid
    wxSizeEvent szrev;
    OnResizeCanvas(szrev);

    // darken map
    m_spellmap->SetRenderFilter(m_spellmap->terrain->filter.darker);
}

// cleanup
FormMapUnits::~FormMapUnits()
{               
    canvas->Unbind(wxEVT_SIZE,&FormMapUnits::OnResizeCanvas,this);
    canvas->Unbind(wxEVT_KEY_UP,&FormMapUnits::OnCanvasKey,this);

    grid->Clear(true);
    sizer->Clear(true);    
    canvas->SetSizer(NULL,true);    
}

// on key press
void FormMapUnits::OnCanvasKey(wxKeyEvent &ev)
{
    // map filte to idle
    m_spellmap->SetRenderFilter(NULL);

    // close list
    wxQueueEvent(canvas->GetParent(),new wxCloseEvent(wxEVT_CLOSE_WINDOW,m_win_id));
}

// on canvas resize
void FormMapUnits::OnResizeCanvas(wxSizeEvent& ev)
{
    // loose old grid stuff
    grid->Clear(true);

    // panel backgroud
    auto back = m_spelldata->gres.wm_map_units_list;

    // get canvas max size
    auto parent_rect = canvas->GetClientRect();
    int surf_x = parent_rect.width;
    int surf_y = parent_rect.height - HUD_Y_GAP;

    // max number of elements
    int cnt_x = floor(surf_x/(back->x_size + 10));
    int cnt_y = floor((surf_y)/(back->y_size + 10));    
    grid->SetCols(cnt_x);
    grid->SetRows(cnt_y);

    // generate panels
    // make list of desired units
    int count = 0;
    for(auto & unit : m_spellmap->units)
    {
        if(m_spellmap->isGameMode() && unit->is_enemy)
            continue;
        count++;
        if(count > cnt_x*cnt_y)
            break;        

        wxPanel* panel = new wxPanel(canvas,wxID_PANEL0+unit->id,wxDefaultPosition,wxSize(back->x_size,back->y_size),wxBORDER_NONE|wxFRAME_FLOAT_ON_PARENT|wxBG_STYLE_PAINT);
        panel->SetDoubleBuffered(true);
        panel->SetMaxSize(wxSize(back->x_size,back->y_size));
        panel->SetMaxClientSize(wxSize(back->x_size,back->y_size));
        grid->Add(panel,0,wxALL|wxALIGN_CENTER);

        panel->Bind(wxEVT_PAINT,&FormMapUnits::OnPaintUnit,this,wxID_PANEL0 + unit->id);
        panel->Bind(wxEVT_LEFT_DOWN,&FormMapUnits::OnUnitClick,this,wxID_PANEL0 + unit->id);
    }
    grid->Layout();

    canvas->Layout();
}

// paint unit panel
void FormMapUnits::OnPaintUnit(wxPaintEvent& event)
{              
    auto panel = (wxPanel*)event.GetEventObject();
    auto unit_id = event.GetId() - wxID_PANEL0;
    int surf_x = panel->GetClientRect().GetWidth();
    int surf_y = panel->GetClientRect().GetHeight();

    // get unit
    auto *unit = m_spellmap->GetUnit(unit_id);
    if(!unit)
        return;

    // make render buffer
    std::vector<uint8_t> buffer(surf_x*surf_y,0x00);
    uint8_t* buf = &buffer[0];
    uint8_t* buf_end =  &buf[buffer.size()];

    // render
    unit->RenderPreview(buf, buf_end, surf_x);

    // render 24bit RGB data to raw bmp buffer
    wxBitmap bmp(surf_x,surf_y,24);
    uint8_t (*pal)[3] = m_spellmap->pal;
    wxNativePixelData data(bmp);
    wxNativePixelData::Iterator p(data);
    for(int y = 0; y < surf_y; ++y)
    {
        uint8_t* scan = p.m_ptr;        
        for(int x = 0; x < surf_x; x++)
        {
            *scan++ = pal[*buf][2];
            *scan++ = pal[*buf][1];
            *scan++ = pal[*buf][0];
            buf++;
        }
        p.OffsetY(data,1);
    }

    // render map
    wxPaintDC pdc(panel);    
    pdc.DrawBitmap(bmp,wxPoint(0,0));
}

// on click unit panel
void FormMapUnits::OnUnitClick(wxMouseEvent& event)
{    
    // select unit
    auto unit_id = event.GetId() - wxID_PANEL0;
    auto* unit = m_spellmap->GetUnit(unit_id);
    if(unit)
    {
        m_spellmap->SelectUnit(unit);
        m_spellmap->ScrollToUnit(unit);
    }

    // map filte to idle
    m_spellmap->SetRenderFilter(NULL);

    // close list
    wxQueueEvent(canvas->GetParent(),new wxCloseEvent(wxEVT_CLOSE_WINDOW,m_win_id));
}


