#pragma once

#include "wx/dcbuffer.h"

class SpellBtnHUD
{
public:
	wxBitmap* bmp_idle;
	wxBitmap* bmp_press;
	wxBitmap* bmp_hover;
	int x_pos;
	int y_pos;
	int x_size;
	int y_size;
	int wx_id;	
	int is_hover;
	int is_press;
	int action_id;
	std::function<void(void)> cb_press;
	std::function<void(void)> cb_hover;

	~SpellBtnHUD();
	SpellBtnHUD(int x_pos, int y_pos, int x_size, int y_size, 
		uint8_t *pal, uint8_t *pic_idle,uint8_t* pic_hover,uint8_t *pic_press,
		int action_id=0, std::function<void(void)> cb_press=NULL,std::function<void(void)> cb_hover=NULL);
};
