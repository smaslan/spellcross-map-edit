
#include "spell_hud_buttons.h"

#include "wx/dcgraph.h"
#include "wx/dcbuffer.h"
#include <wx/rawbmp.h>

SpellBtnHUD::~SpellBtnHUD()
{
	if(bmp_idle)
		delete[] bmp_idle;
	if(bmp_hover)
		delete[] bmp_hover;
	if(bmp_press)
		delete[] bmp_press;
	bmp_idle = NULL;
	bmp_press = NULL;
	bmp_hover = NULL;
}

// set button to temporary invalid (button record will be just placeholder)
void SpellBtnHUD::Invalidate()
{
	this->~SpellBtnHUD();
}
int SpellBtnHUD::IsValid()
{
	return(bmp_idle && bmp_press &&	bmp_hover);
}

// is position of this object identical to given?
int SpellBtnHUD::PositionMatch(int x_pos,int y_pos,int x_size,int y_size)
{
	return(x_pos == this->x_pos && y_pos == this->y_pos && x_size == this->x_size && y_size == this->y_size);
}

// make new button object
int SpellBtnHUD::Make(int x_pos,int y_pos,int x_size,int y_size,
	uint8_t* pal,uint8_t* pic_idle,uint8_t* pic_hover,uint8_t* pic_press,
	int action_id,std::function<void(void)> cb_press,std::function<void(void)> cb_hover)
{
	bmp_idle = NULL;
	bmp_press = NULL;
	bmp_hover = NULL;

	wx_id = 0;

	this->action_id = action_id;
	this->cb_press = cb_press;
	this->cb_hover = cb_hover;
	this->x_pos = x_pos;
	this->y_pos = y_pos;
	this->x_size = x_size;
	this->y_size = y_size;

	uint8_t* picz[3] ={pic_idle, pic_hover, pic_press};
	wxBitmap** bmpz[3] ={&bmp_idle, &bmp_hover, &bmp_press};
	for(int pid = 0; pid < 3; pid++)
	{
		uint8_t* buf = picz[pid];

		// make local bitmap buffer
		wxBitmap* bmp = new wxBitmap(x_size,y_size,32);
		bmp->UseAlpha(true);

		// render 24bit RGB data to raw bmp buffer
		typedef wxPixelData<wxBitmap,wxAlphaPixelFormat> PixelData;
		PixelData data(*bmp);
		PixelData::Iterator p(data);
		for(int y = 0; y < y_size; ++y)
		{
			uint8_t* scan = p.m_ptr;
			uint8_t* src = &buf[y*x_size];
			for(int x = 0; x < x_size; x++)
			{
				*scan++ = pal[(unsigned)*src*3+2];
				*scan++ = pal[(unsigned)*src*3+1];
				*scan++ = pal[(unsigned)*src*3+0];
				*scan++ = (*src != 0)*255;
				src++;
			}
			p.OffsetY(data,1);
		}

		*bmpz[pid] = bmp;
	}
	return(0);
}

SpellBtnHUD::SpellBtnHUD(int x_pos,int y_pos,int x_size,int y_size,
	uint8_t* pal,uint8_t* pic_idle,uint8_t* pic_hover,uint8_t* pic_press,
	int action_id, std::function<void(void)> cb_press,std::function<void(void)> cb_hover)
{	
	Make(x_pos, y_pos, x_size, y_size,
		pal, pic_idle, pic_hover, pic_press,
		action_id, cb_press, cb_hover);
	// clear states but not in Make()
	is_hover = false;
	is_press = false;
}

	