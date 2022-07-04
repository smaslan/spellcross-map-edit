//=============================================================================
// Spellcross generic graphics stuff.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2022, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#undef _HAS_STD_BYTE
#define _HAS_STD_BYTE 0

#include "spell_graphics.h"
#include "sprites.h"
#include "other.h"

#include "wx/dcgraph.h"
#include "wx/dcbuffer.h"
#include <wx/rawbmp.h>


SpellGraphics::SpellGraphics()
{
	vector<wxCursor*> curs ={cur_pointer, cur_wait, cur_select, cur_question, cur_move, cur_attack_down, cur_attack_up, cur_attack_up_down};
	for(auto& cur : curs)
		cur = NULL;
}

SpellGraphics::~SpellGraphics()
{
	items.clear();
	for(auto & pnm : pnms)
		delete pnm;
	pnms.clear();

	vector<wxCursor*> curs = {cur_pointer, cur_wait, cur_select, cur_question, cur_move, cur_attack_down, cur_attack_up, cur_attack_up_down};
	for(auto & cur : curs)
		if(cur)
			delete cur;
}

// load raw bitmap of known size
int SpellGraphics::AddRaw(uint8_t* data,int dlen,int x_size,int y_size,char *name,uint8_t pal[][3],int with_ext,int fix_black)
{	
	// make new record
	items.emplace_back();	
	auto &grp = items.back();	
	
	// store name (loose extension)
	if(with_ext)
		strcpy(grp.name,name);
	else
		strcpy_noext(grp.name,name);

	// copy pixel data	
	grp.pixels.resize(dlen);
	memcpy(&grp.pixels[0],data,dlen);
	
	// convert #0 color to solid black?
	if(fix_black)
		for(auto & pix : grp.pixels)
			if(pix == 0)
				pix = 254;

	// store size
	grp.x_size = x_size;
	grp.y_size = y_size;
	grp.x_ofs = 0;
	grp.y_ofs = 0;
	grp.pal = pal;

	return(0);
}

// load ICO/PNM file (same as for PNM animations frames)
int SpellGraphics::AddICO(uint8_t* data,int dlen,char* name,uint8_t pal[][3])
{
	// make new record
	items.emplace_back();
	auto& grp = items.back();

	// store name (loose extension)
	strcpy_noext(grp.name,name);

	// end of input data
	uint8_t *data_end = &data[dlen];
	
	// vertilcal offset of first line
	if(data + 1 >= data_end)
		return(1);
	int y_ofs = *(int16_t*)data; data += 2;

	// minimum left offset of all lines
	if(data + 1 >= data_end)
		return(1);
	int x_ofs = *(uint16_t*)data; data += 2;

	// maximum line len
	if(data + 1 >= data_end)
		return(1);
	int x_size = *(uint16_t*)data; data += 2;

	// lines count
	if(data >= data_end)
		return(1);
	int y_size = *data++;

	// shring x-size to minimum possible by leaving out left offset
	//x_size -= x_ofs;

	// reserve and clear pixel buffer
	grp.pixels.assign(x_size*y_size,0);

	// for each icon line:
	for(int y = 0; y < y_size; y++)
	{
		// get chunk offset
		int line_x_ofs = -x_ofs;

		// pixel buffer line start
		uint8_t *pix = &grp.pixels[y*x_size];

		// copy all line chunks:
		if(data >= data_end)
			return(1);
		while(*data != 0xFF)
		{
			// get chunk left offset and size
			if(data + 2 > data_end)
				return(1);
			line_x_ofs += *data++;
			int chunk_size = *data++;			
			// copy chunk
			if(data + chunk_size > data_end)
				return(1);
			for(int x = 0; x < chunk_size; x++)
				if(data[x] == 0x00)
					pix[line_x_ofs+x] = 254; // convert 0 to other black
				else
					pix[line_x_ofs+x] = data[x];
			//memcpy(&pix[line_x_ofs], data, chunk_size);
			data += chunk_size;
			line_x_ofs += chunk_size;
			if(data >= data_end)
				return(1);
		}
		data++;		
	}

	// store size
	grp.x_size = x_size;
	grp.y_size = y_size;
	grp.x_ofs = x_ofs;
	grp.y_ofs = y_ofs;
	grp.pal = pal;

	return(0);
}

// load cursor files format: [x,y,data]
int SpellGraphics::AddCUR(uint8_t* data,int dlen,char* name,uint8_t pal[][3])
{
	if(dlen < 2)
		return(1);

	// end of input data
	uint8_t* data_end = &data[dlen];

	// get size
	int x_size = (unsigned)*data++;
	int y_size = (unsigned)*data++;
	if(&data[x_size*y_size] > data_end)
		return(1);

	// make new record
	items.emplace_back();
	auto& grp = items.back();
	grp.x_size = x_size;
	grp.y_size = y_size;
	grp.x_ofs = 0;
	grp.y_ofs = 0;
	grp.pal = pal;

	// copy image data
	grp.pixels.resize(x_size*y_size);
	memcpy(&grp.pixels[0],data,x_size*y_size);

	// store name (loose extension)
	strcpy_s(grp.name,sizeof(grp.name),name);

	return(0);
}

// make "round" LED indicator for mission hud
int SpellGraphics::AddLED(int color,const char* name,uint8_t pal[][3])
{
	// make item
	items.emplace_back();
	auto& grp = items.back();

	// store name (loose extension)
	strcpy(grp.name,name);

	// make size
	grp.x_size = 4;
	grp.y_size = 4;
	grp.x_ofs = 0;
	grp.y_ofs = 0;
	grp.pal = pal;

	// make LED
	grp.pixels.assign(4*4,0);
	memset(grp.GetPixels(0,1),color,2);
	memset(grp.GetPixels(1,0),color,4);
	memset(grp.GetPixels(2,0),color,4);
	memset(grp.GetPixels(3,1),color,2);

	return(0);
}


// get item data
SpellGraphicItem *SpellGraphics::GetResource(int index)
{	
	if(index >= 0 && index < items.size())
		return(&items[index]);
	else
		return(NULL);
}
SpellGraphicItem *SpellGraphics::GetResource(const char* name)
{
	for(int k = 0; k < items.size(); k++)
		if(strcmp(name, items[k].name) == 0)
			return(&items[k]);
	return(NULL);
}
// get items count
int SpellGraphics::Count()
{
	return(items.size());
}

// return pointer to pixel data
uint8_t* SpellGraphicItem::GetPixels(int y,int x)
{
	return(&pixels[y*x_size + x]);
}

// render image to buffer position with range checking and transparency
int SpellGraphicItem::Render(uint8_t* buf,uint8_t* buf_end,int buf_x_size,int x_pos,int y_pos,int in_black,int* y_buffer)
{	
	int *y_buf = NULL;
	int y_buf_pos = -1;
	for(int y = 0; y < y_size; y++)
	{
		uint8_t* pic = &buf[x_pos + this->x_ofs + (y_pos+this->y_ofs+y)*buf_x_size];
		if(y_pos < 0)
			continue;
		if(pic >= buf_end)
			continue;
		if(y_buffer)
		{
			y_buf = &y_buffer[x_pos + this->x_ofs];
			y_buf_pos = y_pos+this->y_ofs+y;
		}

		uint8_t* src = GetPixels(y);
		int x_sz = x_size;
		if(x_pos < 0)
		{
			pic += -x_pos;
			src += -x_pos;
			x_sz -= -x_pos;
			if(y_buffer)
				y_buf += -x_pos;
		}
		x_sz = min(buf_x_size - max(x_pos,0), x_sz);
		
		if(in_black)
		{
			for(int x = 0; x < x_sz; x++)
			{
				if(*src && *src != 0xFE)
					*pic = 0xFE;
				src++;
				pic++;
			}
		}
		else
		{
			for(int x = 0; x < x_sz; x++)
			{
				if(*src)
					*pic = *src;
				if(*src && y_buf && *y_buf < 0)
					*y_buf = y_buf_pos;
				if(y_buf)
					y_buf++;
				src++;
				pic++;
			}
		}
	}
	return(0);
}

// render outer glyph shape mask
int SpellGraphicItem::RenderMask(uint8_t* buf, uint8_t *buf_end)
{
	memset(buf,0xFFu,buf_end - buf);
	if(buf_end - buf < x_size*y_size)
		return(1);
	for(int y = 0; y < y_size; y++)
	{
		for(int x = 0; x < x_size; x++)
		{
			if(*GetPixels(y,x))
				break;
			buf[x + y*x_size] = 0x00u;
		}
		for(int x = x_size-1; x >= 0; x--)
		{
			if(*GetPixels(y,x))
				break;
			buf[x + y*x_size] = 0x00u;
		}
	}
	for(int x = 0; x < x_size; x++)
	{
		for(int y = 0; y < y_size; y++)
		{
			if(*GetPixels(y,x))
				break;
			buf[x + y*x_size] = 0x00u;
		}
		for(int y = y_size-1; y >= 0; y--)
		{
			if(*GetPixels(y,x))
				break;
			buf[x + y*x_size] = 0x00u;
		}
	}
		
	return(0);
}



// render glyph to bmp
wxBitmap* SpellGraphicItem::Render(bool transparent)
{
	return(Render(-1,-1,transparent));
}
wxBitmap *SpellGraphicItem::Render(int surf_x, int surf_y, bool transparent)
{
	if(surf_x < 0 || surf_y < 0)
	{
		surf_x = x_size;
		surf_y = y_size;
	}
	wxBitmap *bmp = new wxBitmap(surf_x,surf_y,(transparent)?32:24);
	bmp->UseAlpha(transparent);

	// leave if surface not big enough
	if(surf_x < x_size || surf_y < y_size)
		return(bmp);

	// source dat end
	uint8_t *end = &pixels[0] + x_size*y_size;

	// image should fit to surface
	int x_ofs = (surf_x - x_size)/2;
	int y_ofs = (surf_y - y_size)/2;

	if(transparent)
	{
		// render 32bit RGBA data to raw bmp buffer
		uint8_t* buf = &pixels[0];
		typedef wxPixelData<wxBitmap,wxAlphaPixelFormat> PixelData;
		PixelData data(*bmp);
		PixelData::Iterator p(data);
		for(int y = 0; y < surf_y; ++y)
		{
			uint8_t* scan = p.m_ptr;
			for(int x = 0; x < surf_x; x++)
			{
				int is_visible = y >= y_ofs && y < y_ofs+y_size && x >= x_ofs && x < x_ofs+x_size && buf < end;
				// visible area
				*scan++ = pal[*buf][2];
				*scan++ = pal[*buf][1];
				*scan++ = pal[*buf][0];
				*scan++ = (*buf != 0 && is_visible)*255; // alpha channel
				buf++;
			}
			p.OffsetY(data,1);
		}
	}
	else
	{
		// render 24bit RGB data to raw bmp buffer
		uint8_t* buf = &pixels[0];
		wxNativePixelData pdata(*bmp);
		wxNativePixelData::Iterator p(pdata);
		for(int y = 0; y < surf_y; y++)
		{
			uint8_t* scan = p.m_ptr;
			for(int x = 0; x < surf_x; x++)
			{
				int is_visible = y >= y_ofs && y < y_ofs+y_size && x >= x_ofs && x < x_ofs+x_size && buf < end;
				if(is_visible && *buf)
				{
					*scan++ = pal[*buf][2];
					*scan++ = pal[*buf][1];
					*scan++ = pal[*buf][0];
					buf++;
				}
				else
				{
					uint8_t checkers = (!(x&32) == !(y&32))?0x88:0xAA;
					*scan++ = checkers;
					*scan++ = checkers;
					*scan++ = checkers;
					if(is_visible)
						buf++;
				}
			}
			p.OffsetY(pdata,1);
		}
	}

	return(bmp);
}


wxCursor* SpellGraphicItem::RenderCUR(bool is_grayscale)
{
	wxCursor* cursor;

	if(is_grayscale)
	{
		// grayscale mode (it works at least in Windows, dunno if in Linux/Mac...)

		auto bmp = Render(true);
		cursor = new wxCursor(bmp->ConvertToImage());
		delete bmp;

	}
	else
	{
		// use BW mode
		uint8_t bits[32][32/8];
		uint8_t mask[32][32/8];
		memset(bits, 0xFF, sizeof(bits));
		memset(mask, 0xFF, sizeof(mask));

		if(x_size > 32 || y_size > 32)
			return(NULL);

		uint8_t* buf = &pixels[0];
		for(int y = 0; y < y_size; ++y)
		{
			int x_pos = 0;
			int pix8pos = 0;
			for(int x = 0; x < x_size; x++)
			{
				uint8_t pix = pal[*buf][0];
				bits[y][x_pos] &= ~((uint8_t)(pix > 50u)<<pix8pos);
				mask[y][x_pos] &= ~((uint8_t)(*buf != 0)<<pix8pos);
				buf++;
				pix8pos++;
				if(pix8pos >= 8)
				{				
					pix8pos = 0;
					x_pos++;
				}
			}
		}
	

		#ifdef __WXMSW__
			wxBitmap bmp_data((char*)&bits[0][0],32,32);
			wxBitmap bmp_mask((char*)&mask[0][0],32,32);
			bmp_data.SetMask(new wxMask(bmp_mask));
			wxImage image = bmp_data.ConvertToImage();
			image.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X,0);
			image.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y,0);
			cursor = new wxCursor(image);
		#elif defined(__WXGTK__) or defined(__WXMOTIF__)
			cursor = new wxCursor(bmp_data,32,32,0,0,
				bmp_mask,wxWHITE,wxBLACK);
		#endif
	}
	
	return(cursor);
}


// render cursor file (with transparency)
wxCursor *SpellGraphics::RenderCUR(const char *name)
{
	auto resource = GetResource(name);
	if(!resource)
		return(NULL);
	return(resource->RenderCUR(true));
}


// add PNM to list
int SpellGraphics::AddPNM(uint8_t* data,int dlen,char* name)
{
	// insert new PNM object
	AnimPNM *pnm = new AnimPNM();
	pnms.push_back(pnm);	
	char temp_name[13];
	strcpy_noext(temp_name, name);
	return(pnms.back()->Decode(data,temp_name));
}

// obtain PNM file from loaded list by name
AnimPNM* SpellGraphics::GetPNM(const char* name)
{
	for(auto & pnm : pnms)
		if(strcmp(pnm->name,name) == 0)
			return(pnm);
	return(NULL);
}



// initialize projectile
SpellProjectile::SpellProjectile(SpellGraphicItem& glyph)
{	
	// identify projectile direction
	int direction = hex2num(glyph.name[2]);
	if(direction < 0 || direction > 15)
		throw std::exception("Invalid name of projection GFK file!");

	// init glyphs
	for(int k = 0; k < 16; k++)
		glyphs[k] = NULL;

	// store glyph
	glyphs[direction] = &glyph;
	
	// store name converted to "ST0*.GFK"
	strcpy_s(name,sizeof(name),glyph.name);
	name[2] = '0';
}

// insert projectile glyph to list
int SpellProjectile::Insert(SpellGraphicItem& glyph)
{
	// identify projectile direction
	int direction = hex2num(glyph.name[2]);
	if(direction < 0 || direction > 15)
		return(1);

	// store glyph
	glyphs[direction] = &glyph;
	return(0);
}

// check if projectile is complete (true)
int SpellProjectile::Check()
{
	for(int k = 0; k < 16; k++)
		if(!glyphs[k])
			return(0);
	return(1);
}

// get projectile by angle
SpellGraphicItem* SpellProjectile::GetGlyph(double angle)
{
	angle = remainder(angle,360.0);
	int azim = ((int)round((360.0 + 90.0 - angle)/360.0*(double)16.0)) % 16;
	return(glyphs[azim]);
}


// search projectile resources and order them per type
int SpellGraphics::SortProjectiles()
{
	// group projectiles:
	for(auto & gfk : items)
	{		
		if(wildcmp("ST?*.GFK",gfk.name))
		{
			// make base name
			char base_name[sizeof(gfk.name)];
			strcpy_s(base_name,sizeof(gfk.name),gfk.name);
			base_name[2] = '0';

			int found = false;
			for(auto& proj : projectiles)
			{
				if(strcmp(proj.name,base_name) == 0)
				{
					// insert to existing projectile set
					proj.Insert(gfk);
					found = true;
					break;
				}
			}
			if(!found)
			{
				// first glyph for projectile: make new
				projectiles.emplace_back(gfk);
			}
		}
	}
	for(auto& proj : projectiles)
	if(!proj.Check())
	{
		// porjectil incomplete
		return(1);
	}
	return(0);
}

// get projectile record by name or NULL
SpellProjectile* SpellGraphics::GetProjectile(char* name)
{
	for(auto & proj : projectiles)
		if(strcmp(proj.name, name) == 0)
			return(&proj);
	return(NULL);
}



