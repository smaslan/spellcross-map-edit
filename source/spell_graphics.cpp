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
#include "spellcross.h"
#include "sprites.h"
#include "LZ_spell.h"
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
	for(auto &item: items)
		delete item;
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
int SpellGraphics::AddRaw(uint8_t* data,int dlen,int x_size,int y_size,const char *name,SpellPalette *pal,int is_solid)
{	
	// auto y-size?
	if(x_size && !y_size)
		y_size = dlen/x_size;

	// bad size
	if(x_size*y_size > dlen)
		return(1);

	// fix size (crop size to actual dims, because LZ sometimes contains extra byte for whatever reason)
	if(x_size && y_size)
		dlen = x_size*y_size;
	
	// make new record
	auto grp = new SpellGraphicItem();
	items.emplace_back(grp);
	
	
	// store name
	grp->full_name = name;
	grp->name = std::filesystem::path(name).stem().string();
	
	// copy pixel data 
	grp->pixels.resize(dlen);
	memcpy(&grp->pixels[0],data,dlen);
	
	// convert #0 color to solid black?
	grp->is_transparent = !is_solid;
	/*if(fix_black)
		for(auto & pix : grp.pixels)
			if(pix == 0)
				pix = 254;*/
	
	// link to external palette	
	grp->palette = pal;
	grp->pal = (uint8_t(*)[3])pal->m_pal.data();
	
	// store size
	grp->x_size = x_size;
	grp->y_size = y_size;
	grp->x_ofs = 0;
	grp->y_ofs = 0;
	

	return(0);
}

// load ICO/PNM file (same as for PNM animations frames)
int SpellGraphics::AddICO(uint8_t* data,int dlen,const char* name,SpellPalette *pal)
{
	// make new record
	auto grp = new SpellGraphicItem();
	items.emplace_back(grp);

	// store name (loose extension)
	grp->full_name = name;
	grp->name = std::filesystem::path(name).stem().string();
	//strcpy_noext(grp.name,name);

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
	grp->pixels.assign(x_size*y_size,0);

	// for each icon line:
	for(int y = 0; y < y_size; y++)
	{
		// get chunk offset
		int line_x_ofs = -x_ofs;

		// pixel buffer line start
		uint8_t *pix = &grp->pixels[y*x_size];

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
				/*if(data[x] == 0x00)
					pix[line_x_ofs+x] = 254; // convert 0 to other black
				else*/
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
	grp->x_size = x_size;
	grp->y_size = y_size;
	grp->x_ofs = x_ofs;
	grp->y_ofs = y_ofs;
	grp->is_transparent = true;
	
	// link to external palette	
	grp->palette = pal;
	grp->pal = (uint8_t(*)[3])pal->m_pal.data();

	return(0);
}

// load cursor files format: [x,y,data]
int SpellGraphics::AddCUR(uint8_t* data,int dlen,const char* name,SpellPalette* pal)
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
	auto grp = new SpellGraphicItem();
	items.emplace_back(grp);
	grp->x_size = x_size;
	grp->y_size = y_size;
	grp->x_ofs = 0;
	grp->y_ofs = 0;
	grp->is_transparent = true;
	
	// link to external palette	
	grp->palette = pal;
	grp->pal = (uint8_t(*)[3])pal->m_pal.data();

	// copy image data
	grp->pixels.resize(x_size*y_size);
	memcpy(&grp->pixels[0],data,x_size*y_size);

	// store name (loose extension)
	grp->full_name = name;
	grp->name = std::filesystem::path(name).stem().string();

	return(0);
}

// make "round" LED indicator for mission hud
int SpellGraphics::AddLED(int color,const char* name,SpellPalette* pal)
{
	// make item
	auto grp = new SpellGraphicItem();
	items.emplace_back(grp);

	// store name (loose extension)
	grp->full_name = name;
	grp->name = name;

	// make size
	grp->x_size = 4;
	grp->y_size = 4;
	grp->x_ofs = 0;
	grp->y_ofs = 0;
	grp->is_transparent = true;

	// link to external palette	
	grp->palette = pal;
	grp->pal = (uint8_t(*)[3])pal->m_pal.data();

	// make LED
	grp->pixels.assign(4*4,0);
	memset(grp->GetPixels(0,1),color,2);
	memset(grp->GetPixels(1,0),color,4);
	memset(grp->GetPixels(2,0),color,4);
	memset(grp->GetPixels(3,1),color,2);

	return(0);
}


// get item data
SpellGraphicItem *SpellGraphics::GetResource(int index)
{	
	if(index >= 0 && index < items.size())
		return(items[index]);
	else
		return(NULL);
}
SpellGraphicItem *SpellGraphics::GetResource(const char* name)
{
	for(int k = 0; k < items.size(); k++)
		if(items[k]->full_name.compare(name) == 0 || items[k]->name.compare(name) == 0)
			return(items[k]);
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

// save info file 
int SpellGraphicItem::ExportInfo(wstring path, wstring image_name)
{
	std::string info = string_format("// Spellcross graphics resource meta file (autogenerated by Spellcross Map Editor)\n");
	info += string_format("name:: %s\n",full_name.c_str());
	info += string_format("image:: %ls\n",image_name.c_str());
	info += string_format("pixels:: %d\n",pixels.size());
	info += string_format("xsize:: %d\n",x_size);
	info += string_format("ysize:: %d\n",y_size);
	info += string_format("transparent:: %d\n",is_transparent);
	
	auto pal_name = std::filesystem::path(palette->m_name).stem().concat(".palinfo").string();
	info += string_format("palette:: %s\n",pal_name.c_str());
	
	info += string_format("colors:: ");	
	info += palette->GetRangeString();
	info += string_format("\n");

	// try open file
	ofstream fw(path,ios::out | ios::trunc);
	if(!fw.is_open())
		return(1);
	fw.write(info.c_str(), info.size());
	fw.close();

	return(0);
}

// export resource to file
int SpellGraphicItem::Export(std::wstring path)
{
	auto ext = std::filesystem::path(path).extension().string();

	if(iequals(ext,".LZ"))
	{
		// LZ files

		// try compress
		std::vector<uint8_t> lz_data;
		LZspell *lz = new LZspell(pixels.data(), pixels.size(), lz_data);
		delete lz;
		if(lz_data.empty())
			return(1);

		// try store file
		ofstreamext fw(path,ios::out | ios::binary | ios::trunc);
		if(!fw.is_open())
			return(1);
		fw.write((const char*)lz_data.data(), lz_data.size());
		fw.close();
		
	}
	else
	{
		// other formats not implemented yet
		return(1);
	}

	return(0);
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
				if(*src || !is_transparent)
					*pic = *src;
				if((*src || !is_transparent) && y_buf && *y_buf < 0)
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
wxBitmap* SpellGraphicItem::Render(bool transparent,bool invert)
{
	return(Render(-1,-1,transparent,invert));
}
wxBitmap *SpellGraphicItem::Render(int surf_x, int surf_y, bool transparent, bool invert)
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
	uint8_t *end = pixels.data() + pixels.size();

	// image should fit to surface
	int x_ofs = (surf_x - x_size)/2;
	int y_ofs = (surf_y - y_size)/2;

	// make temp palette
	uint8_t tpal[256][3];
	if(invert)
		for(int k = 0; k < 256; k++)
		{
			tpal[k][0] = 255u - pal[k][0];
			tpal[k][1] = 255u - pal[k][1];
			tpal[k][2] = 255u - pal[k][2];
		}
	else
		std::memcpy(tpal, pal, 256*3);

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
				if(buf < end)
				{
					*scan++ = tpal[*buf][2];
					*scan++ = tpal[*buf][1];
					*scan++ = tpal[*buf][0];
				}
				else
				{
					*scan++ = 0;
					*scan++ = 0;
					*scan++ = 0;
				}
				*scan++ = ((*buf != 0 || !is_transparent) && is_visible)*255; // alpha channel
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
				if(is_visible && (*buf || !is_transparent))
				{
					*scan++ = tpal[*buf][2];
					*scan++ = tpal[*buf][1];
					*scan++ = tpal[*buf][0];
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

// clear resource
void SpellGraphicItem::Clear()
{
	name = "";
	full_name = "";
	palette = NULL;
	pal = NULL;
	pixels.clear();
	x_size = 0;
	y_size = 0;
	x_ofs = 0;
	y_ofs = 0;
}



// encode bitmap to graphic resouce
int SpellGraphicItem::Encode(wxBitmap& bmp,std::string name,SpellPalette* target_pal,int dither_dist)
{
	is_transparent = bmp.HasAlpha();

	// store name
	full_name = name;
	this->name = std::filesystem::path(name).stem().string();

	// store size
	x_size = bmp.GetWidth();
	y_size = bmp.GetHeight();
	x_ofs = 0;
	y_ofs = 0;

	// clear pixel data	
	pixels.assign(x_size*y_size,0);

	// link to external palette	
	palette = target_pal;
	pal = (uint8_t(*)[3])palette->m_pal.data();

	// make target palette
	std::vector<int[3]> ipal(256);
	for(int k = 0; k < 256; k++)
	{
		ipal[k][0] = pal[k][0];
		ipal[k][1] = pal[k][1];
		ipal[k][2] = pal[k][2];
	}
	uint8_t *mask = palette->m_used.data();

	class Pixel {
	public:
		int r;
		int g;
		int b;
		int a;
	};
	std::vector<Pixel> pix(x_size*y_size);

	if(is_transparent)
	{
		// scan 32bit RGBA bmp data to buffer
		typedef wxPixelData<wxBitmap,wxAlphaPixelFormat> PixelData;
		PixelData data(bmp);
		PixelData::Iterator p(data);
		int id = 0;
		for(int y = 0; y < y_size; ++y)
		{
			uint8_t* scan = p.m_ptr;
			for(int x = 0; x < x_size; x++)
			{
				pix[id].b = *scan++;
				pix[id].g = *scan++;
				pix[id].r = *scan++;
				pix[id].a = *scan++;
				id++;
			}
			p.OffsetY(data,1);
		}
	}
	else
	{
		// scan 24bit RGB bmp data to buffer
		wxNativePixelData data(bmp);
		wxNativePixelData::Iterator p(data);
		int depth = bmp.GetDepth();
		if(depth != 24)
			return(1);
		int id = 0;
		for(int y = 0; y < y_size; ++y)
		{
			uint8_t* scan = p.m_ptr;
			for(int x = 0; x < x_size; x++)
			{
				pix[id].b = *scan++;
				pix[id].g = *scan++;
				pix[id].r = *scan++;
				pix[id].a = 255;
				id++;
			}
			p.OffsetY(data,1);
		}
	}

	// encode
	for(int p = 0; p < pix.size(); p++)
	{
		int rr = pix[p].r;
		int gg = pix[p].g;
		int bb = pix[p].b;
		int aa = pix[p].a;

		int min_dist = 256*256*3;
		int min_dist2 = 256*256*3;
		int min_id = -1;
		int min_id2 = -1;
		for(int k = 1; k < 256; k++)
		{
			if(!mask[k])
				continue;
			int dist = (ipal[k][0] - rr)*(ipal[k][0] - rr) + (ipal[k][1] - gg)*(ipal[k][1] - gg) + (ipal[k][2] - bb)*(ipal[k][2] - bb);
			if(dist < min_dist)
			{
				min_id2 = min_id;
				min_dist2 = min_dist;
				min_id = k;
			}
			min_dist = min(min_dist,dist);
		}
		if(min_dist > dither_dist*dither_dist)
		{
			// dithering mode
			min_dist = std::sqrt(min_dist);
			min_dist2 = std::sqrt(min_dist2);
			if(std::rand() % (min_dist + min_dist2) < min_dist)
				min_id = min_id2;
		}
		if(min_id < 0)
			min_id = 0;

		if(aa && !(rr == 0 && gg == 0 && bb == 0))
			pixels[p] = min_id;
		else
			pixels[p] = 0;
	}

	return(1);
}


// render cursor
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
int SpellGraphics::AddPNM(uint8_t* data,int dlen,const char* name)
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


// get used colors range string
std::string SpellGraphicItem::GetColorRangeString()
{
	// has zero index?
	bool has_zero = std::find(pixels.begin(),pixels.end(),0) != pixels.end();
	
	uint8_t pix_min = 255;
	uint8_t pix_max = 0;
	for(auto &pix: pixels)
	{
		if(pix)
			pix_min = min(pix_min,pix);
		if(pix)
			pix_max = max(pix_max,pix);
	}
	
	std::string str = "";
	if(has_zero && pix_min <= 1)
		pix_min = 0;
	if(has_zero && pix_min > 1)
		str += "0, ";
	if(pix_min != pix_max)
		str += string_format("%d-%d",pix_min,pix_max);
	else
		str += string_format("%d",pix_min);
	return(str);
}


// initialize projectile
SpellProjectile::SpellProjectile(SpellGraphicItem *glyph)
{	
	// identify projectile direction
	int direction = hex2num(glyph->name[2]);
	if(direction < 0 || direction > 15)
		throw std::exception("Invalid name of projection GFK file!");

	// init glyphs
	for(int k = 0; k < 16; k++)
		glyphs[k] = NULL;

	// store glyph
	glyphs[direction] = glyph;
	
	// store name converted to "ST0*.GFK"
	strcpy_s(name,sizeof(name),glyph->full_name.c_str());
	name[2] = '0';
}

// insert projectile glyph to list
int SpellProjectile::Insert(SpellGraphicItem *glyph)
{
	// identify projectile direction
	int direction = hex2num(glyph->name[2]);
	if(direction < 0 || direction > 15)
		return(1);

	// store glyph
	glyphs[direction] = glyph;
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
		if(wildcmp("ST?*.GFK",gfk->name.c_str()))
		{
			// make base name
			std::string base_name = gfk->full_name;
			if(base_name.size() < 2)
				return(1);
			base_name[2] = '0';
			
			int found = false;
			for(auto& proj : projectiles)
			{
				if(base_name.compare(proj.name) == 0)
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



