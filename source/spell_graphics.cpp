//=============================================================================
// Spellcross generic graphics stuff.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2022, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================

#include "spell_graphics.h"
#include "other.h"

SpellGraphics::SpellGraphics()
{
}

// load raw bitmap of known size
int SpellGraphics::AddRaw(uint8_t* data,int dlen,int x_size,int y_size,char *name)
{	
	// make new record
	items.emplace_back();	
	auto &grp = items.back();	
	
	// store name (loose extension)
	strcpy_noext(grp.name,name);

	// copy pixel data	
	grp.pixels.resize(dlen);
	memcpy(&grp.pixels[0],data,dlen);

	// store size
	grp.x_size = x_size;
	grp.y_size = y_size;
	grp.x_ofs = 0;
	grp.y_ofs = 0;

	return(0);
}

// load raw bitmap of known size
int SpellGraphics::AddICO(uint8_t* data,int dlen,char* name)
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
			memcpy(&pix[line_x_ofs], data, chunk_size);
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

	return(0);
}

// make "round" LED indicator for mission hud
int SpellGraphics::AddLED(int color,const char* name)
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
int SpellGraphicItem::Render(uint8_t* buf,uint8_t* buf_end,int buf_x_size,int x_pos,int y_pos)
{	
	for(int y = 0; y < y_size; y++)
	{
		uint8_t* pic = &buf[x_pos + this->x_ofs + (y_pos+this->y_ofs+y)*buf_x_size];
		if(y_pos < 0)
			continue;
		if(pic >= buf_end)
			continue;
		uint8_t* src = GetPixels(y);
		int x = 0;
		if(x_pos < 0)
		{
			pic += -x_pos;
			src += -x_pos;
			x = -x_pos;
		}
		for(; x < x_size; x++)
		{
			if(*src)
				*pic = *src;
			src++;
			pic++;
		}
	}
	return(0);
}




