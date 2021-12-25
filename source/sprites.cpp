//=============================================================================
// Spellcross sprite data handling routines.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#include "sprites.h"
#include "other.h"
#include "fs_archive.h"
#include "fsu_archive.h"
#include "map_types.h"

#include "simpleini.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <filesystem>

#include "wx/dcgraph.h"
#include "wx/dcbuffer.h"
#include <wx/rawbmp.h>

using namespace std;




//=============================================================================
// class Sprite 
//=============================================================================

// make empty sprite
Sprite::Sprite()
{
	name[0] = '\0';
	x_ofs = 0;
	y_ofs = 0;
	x_size = 0;
	y_size = 0;
	has_transp = 0;
	land_type = 0;
	data = NULL;
	index = 0;

	flags = 0;
	for(int k = 0; k < 4; k++)
		edge_class[k] = Sprite::CLASS_GENERIC;
	shading = 0;
	spec_class = Sprite::SPEC_CLASS_NONE;

	tool_class = 0;
	tool_group = 0;
}

// destroy sprite
Sprite::~Sprite()
{
	name[0] = '\0';
	if (data)
		delete[] data;
	for(int k = 0;k<4;k++)
		for(int m = 'A'; m <= 'M'; m++)
			quad[k][m-'A'].clear();
}
// check if sprite row quad mask has transparencies or it is just last quad
int Sprite::MaskHasTransp(uint8_t *mask)
{
	return(mask[0] == 0xFF ||
		!((mask[1] == 0x00 && mask[2] == 0x00 && mask[3] == 0xFF) ||
		  (mask[1] == 0x00 && mask[2] == 0xFF && mask[3] == 0xFF) ||
		  (mask[1] == 0xFF && mask[2] == 0xFF && mask[3] == 0xFF)));
}
// get tile slope letter
char Sprite::GetSlope()
{
	return(name[2]);
}
// set sprite index
void Sprite::SetIndex(int id)
{
	index = id;
}
// get sprite index
int Sprite::GetIndex()
{
	return(index);
}


// decode sprite from raw FS archive data, return bytes consumed
// ###todo: check input memory overrun?
int Sprite::Decode(uint8_t* src, char* name)
{
	// store data start
	uint8_t* source_start = src;
	
	// set width
	x_size = 80;

	// get heigth
	y_size = *src;
	src += 2;

	// get land type
	land_type = *src;
	src += 2;

	// get vertical offset
	y_ofs = (int)*(__int32*)src;
	src += 4;

	// allocate sprite data (maximum possible value)
	//data = new byte[(x_size + sizeof(int)*2) * 256];
	uint8_t*buf = new uint8_t[(x_size + sizeof(int) * 2) * 256];
	uint8_t*pdata = buf;


	//unsigned char* mp = mem;
	int i,j,k;

	// no transparencies yet
	has_transp = 0;

	// for each sprite line
	for (i = 0; i < y_size; i++)
	{
		unsigned char* quad = pdata + sizeof(int)*2;
		int oo;
		int nn;
		int	ss;

		// get line offset
		oo = *src++;
		src++;
		// get full blocks count
		nn = *src++;
		// get transparent blocks count
		ss = *src++;

		// read full blocks into temp
		std::memcpy((void*)quad, (void*)src, nn * 4);
		quad += nn * 4;
		src += nn * 4;

		// read transparent blocks into temp
		for (j = 0; j < ss; j++)
		{
			unsigned char mask[4];

			// copy chunk
			std::memcpy((void*)quad, (void*)src, 4);
			quad += 4;

			// generate transparency mark
			for (k = 0; k < 4; k++)
				mask[k] = (*src++ != 0x00) ? 0x00 : 0xFF;

			// check for mark validity
			if (memcmp((void*)src, (void*)mask, 4) != 0)
				return(1);

			// test if there is transparency
			if (MaskHasTransp(mask) || ss > 1)
				has_transp = 1;

			// skip mark
			src += 4;
		}

		// full line len (in pixels now)
		nn = nn * 4 + ss * 4;

		// line data start
		quad = pdata + sizeof(int)*2;

		// detect real len (loose transparent garbage at end of line)
		for (j = nn - 1; j >= 0; j--)
			if (quad[j] != 0x00)
				break;
		nn = j + 1;

		// store decoded line offset
		*(int*)&pdata[0] = oo;
		// store decoded line len
		*(int*)&pdata[sizeof(int)] = nn;

		// line decoding done
		pdata += nn + 2*sizeof(int);
	}

	// sprite data total len
	int len = pdata - buf;

	// store data to sprite record
	this->data = new uint8_t[len];
	std::memcpy((void*)this->data, (void*)buf, len);
	delete[] buf;

	// store sprite name
	strcpy_s(this->name, sizeof(this->name), name);

	// return bytes consumed from the source
	return(src - source_start);
}




// render sprite to target buffer, buffer is sprite origin, x_size is buffer width
void Sprite::Render(uint8_t* buffer,uint8_t* buf_end,int buf_x,int buf_y,int x_size)
{
	Render(buffer, buf_end, buf_x, buf_y, x_size, NULL);
}
void Sprite::Render(uint8_t* buffer, uint8_t* buf_end, int buf_x, int buf_y, int x_size, uint8_t *filter)
{
	// ###todo: optimize for no transparency mode?

	// initial buffer write position
	uint8_t* dest = &buffer[buf_x + (buf_y + y_ofs)*x_size];
	if (dest < buffer)
		return;

	// sprite source data
	uint8_t* data = this->data;

	// render each line:
	if(filter)
	{
		// with filter
		for (int y = 0; y < y_size; y++)
		{
			// line x-offset
			int offset = *(int*)data; data += sizeof(int);
			// pixels count
			int count = *(int*)data; data += sizeof(int);

			// copy line to buffer
			uint8_t* scan = &dest[offset];
			if (scan + count > buf_end)
				break;
			for (int x = 0; x < count; x++)
			{
				if (*data)
					*scan = filter[*data];
				data++;
				scan++;
			}
			// move to next buffer line
			dest += x_size;
		}
	}
	else
	{
		// no filter
		for(int y = 0; y < y_size; y++)
		{
			// line x-offset
			int offset = *(int*)data; data += sizeof(int);
			// pixels count
			int count = *(int*)data; data += sizeof(int);

			// copy line to buffer
			uint8_t* scan = &dest[offset];
			if(scan + count > buf_end)
				break;
			for(int x = 0; x < count; x++)
			{
				if(*data)
					*scan = *data;
				data++;
				scan++;
			}
			// move to next buffer line
			dest += x_size;
		}
	}
}

// render sprite to EMPTY bitmap (it will set its format and size)
wxBitmap *Sprite::Render(uint8_t *pal, double gamma)
{
	// allocate render buffer for indexed image
	int surf_x = x_size;
	int surf_y = 0 + y_size + 0;
	uint8_t* buf = new uint8_t[surf_x*surf_y];
	uint8_t* buf_end = &buf[surf_x*surf_y];
	// render origin
	int org_x = 0;
	int org_y = 0;

	// make local bitmap buffer
	wxBitmap *bmp = new wxBitmap(surf_x,surf_y,32);
	bmp->UseAlpha(true);

	// clear background
	std::memset((void*)buf,230,surf_x*surf_y);

	// render tile aligned to left
	this->Render(buf, buf_end, -this->x_ofs,-this->y_ofs, surf_x);
	
	// apply gamma correction to palette:		
	// male local copy of palette	
	uint8_t gamma_pal[256][3];
	std::memcpy((void*)gamma_pal,(void*)pal,3 * 256);

	// apply gamma correction (this should be maybe optimized out of here?
	for(int k = 0; k < 256; k++)
		for(int c = 0; c < 3; c++)
			gamma_pal[k][c] = (uint8_t)(pow((double)gamma_pal[k][c] / 255.0,1.0 / gamma) * 255.0);
	

	// render 24bit RGB data to raw bmp buffer
	typedef wxPixelData<wxBitmap,wxAlphaPixelFormat> PixelData;
	PixelData data(*bmp);
	PixelData::Iterator p(data);
	for(int y = 0; y < surf_y; ++y)
	{
		uint8_t* scan = p.m_ptr;
		uint8_t* src = &buf[y*surf_x];
		for(int x = 0; x < surf_x; x++)
		{
			*scan++ = gamma_pal[*src][2];
			*scan++ = gamma_pal[*src][1];
			*scan++ = gamma_pal[*src][0];
			*scan++ = (*src != 230)*255;
			src++;
		}
		p.OffsetY(data,1);
	}

	// loose local buffer
	delete[] buf;

	return(bmp);

}


// get vertices pair of edge 0-3 (Q1 - Q4)
int Sprite::GetTileEdge(int edge,TFxyz* vert)
{
	if(edge < 0 || edge > 3)
		return(1);

	// get vertices
	TFxyz tvert[4];
	GetTileModel(tvert, NULL, NULL);

	// return edge vertices in CCW order
	vert[0] = tvert[edge];
	edge++;
	if(edge > 3)
		edge = 0;
	vert[1] = tvert[edge];

	return(0);
}

// returns 4 vertices of tile and/or max two faces [vert_count, vert_1, vert_2, ..., vert_count, vert_1, vert_2, ...]
void Sprite::GetTileModel(TFxyz* vert, int* face, int* face_count)
{
	// step elevation
	const double el = TILE_ELEVATION;

	// tile Z-elev of vertices (CCW order from 0deg)
	const double vz[13][4] = {
		{0.0, 0.0, 0.0, 0.0},
		{+el, +el, +el, 0.0},
		{0.0, +el, 0.0, 0.0},
		{0.0, +el, +el, 0.0},
		{0.0, 0.0, +el, 0.0},
		{0.0, +el, +el, +el},
		{0.0, 0.0, +el, +el},
		{0.0, 0.0, 0.0, +el},
		{+el, 0.0, +el, +el},
		{+el, 0.0, 0.0, +el},
		{+el, 0.0, 0.0, 0.0},
		{+el, +el, 0.0, +el},
		{+el, +el, 0.0, 0.0} };

	// x,y of edges
	const double x[4] = { +40,   0, -40,   0 };
	const double y[4] = { 0,   +40,   0, -40 };

	// safety scaleup so there are no gaps between tiles due to int rounding (messy solution)
	const double scale = 1.02;

	// tile type
	char tile = this->name[2] - 'A';

	// for each vertex:
	if (vert)
	{
		for (int v = 0; v < 4; v++)
		{

			vert[v].x = x[v]*scale;
			vert[v].y = y[v]*scale;
			vert[v].z = vz[tile][v];
		}
	}

	const int fa[13][7] = {
		{1, 0,1,2,3, 0,0}, /* A */
		{2, 0,1,3, 1,2,3}, /* B */
		{2, 0,1,3, 1,2,3}, /* C */
		{1, 0,1,2,3, 0,0}, /* D */
		{2, 0,1,2, 0,2,3}, /* E */
		{2, 0,1,2, 0,2,3}, /* F */
		{1, 0,1,2,3, 0,0}, /* G */
		{2, 0,1,3, 1,2,3}, /* H */
		{2, 0,1,3, 1,2,3}, /* I */
		{1, 0,1,2,3, 0,0}, /* J */
		{2, 0,1,2, 0,2,3}, /* K */
		{2, 0,1,2, 0,2,3}, /* L */
		{1, 0,1,2,3, 0,0}  /* M */ };

	// return faces
	if (face && face_count)
	{
		if (fa[tile][0] == 1)
		{
			// single face
			*face++ = 4;
			std::memcpy((void*)face, (void*)&fa[tile][1], 4*sizeof(int));
			*face_count = 1;
		}
		else
		{
			// two faces
			*face++ = 3;
			std::memcpy((void*)face, (void*)&fa[tile][1], 3 * sizeof(int));
			face += 3;
			*face++ = 3;
			std::memcpy((void*)face, (void*)&fa[tile][4], 3 * sizeof(int));
			*face_count = 2;
		}
	}	
}

// transform vertex by spellcross view angle, returns projected x,y
TFxyz Sprite::ProjectVertex(TFxyz *vert)
{
	// sin/cos(54)
	/*const double sina = 0.80902;
	const double cosa = 0.58778;*/
	const static double sina = std::sin(PROJECTION_ANGLE / 180.0 * 3.1415);
	const static double cosa = std::cos(PROJECTION_ANGLE / 180.0 * 3.1415);
	
	// projected y of x,y point at given tile
	TFxyz xy;	
	xy.x = vert->x;
	xy.y = vert->z * cosa + vert->y * sina;
	return(xy);
}


// Bresneham algorithm (based on GitHub/bert algorithm)
void plot_line(uint8_t* buffer, uint8_t* buf_end, int buf_x, int buf_y, int x_size, uint8_t color, int x0, int y0, int x1, int y1)
{
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2; /* error value e_xy */

	for (;;) {  /* loop */				
		uint8_t *pix = &buffer[buf_x + x0 + (buf_y + y0) * x_size];
		if (pix >= buffer && pix < buf_end)
			*pix = color;
		if (x0 == x1 && y0 == y1) break;
		e2 = 2 * err;
		if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
		if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
	}
}

// render wireframe of tile
void Sprite::RenderTileWires(uint8_t* buffer, uint8_t* buf_end, int buf_x, int buf_y, int x_size, uint8_t color)
{
	// get tile model
	TFxyz vert[4];
	int face_data[4 + 4];
	int face_n;
	this->GetTileModel(vert, face_data, &face_n);

	// make view projection
	TFxyz ve2d[4];
	for (int k = 0; k < 4; k++)
		ve2d[k] = ProjectVertex(&vert[k]);
	// aling to sprite origin
	int y_ref = ve2d[1].y;
	for (int k = 0; k < 4; k++)
	{
		ve2d[k].x += 40;
		ve2d[k].y = -(ve2d[k].y - y_ref) + this->y_ofs;
	}

	
	// for each face:
	int* face = face_data;
	for (int fid = 0; fid < face_n; fid++)
	{
		// face size (vertices)
		int count = *face++;
		
		// for each wire
		for (int k = 0; k < count; k++)
		{
			// vertex indices for both ends of wire
			int wa = face[k];
			int wb = (k == count - 1) ? (face[0]) : (face[k + 1]);

			// plot line
			plot_line(buffer, buf_end, buf_x, buf_y, x_size, color, ve2d[wa].x, ve2d[wa].y, ve2d[wb].x, ve2d[wb].y);
		}
		// next face
		face += count;
	}


}

// get Z-axis (elevation) of x,y position within sprite, when sprite rhomb has axis sizez +-40
// note: very simplified method, better would be calculate actual intersections with sprite polygons
double Sprite::GetTileZ(double x, double y)
{
	// tile slope
	char tile = name[2];
	
	// get tile vertices
	TFxyz vert[4];
	this->GetTileModel(vert, NULL, NULL);

	// protect div by zero
	if (fabs(x) < 0.001)
		x += 0.002;
	
	// tile edge points
	double Ax, Ay, Az;
	double Bx, By, Bz;
	// tile edge line: b*x+d
	double b, d;
	if (x >= 0.0 && y >= 0.0)
	{
		// Q1
		b = -1.0;
		d = vert[1].y;
		Ax = vert[0].x;
		Ay = vert[0].y;
		Az = vert[0].z;
		Bx = vert[1].x;
		By = vert[1].y;
		Bz = vert[1].z;
	}
	else if (x < 0.0 && y >= 0.0)
	{
		// Q2:
		b = 1.0;
		d = vert[1].y;
		Ax = vert[1].x;
		Ay = vert[1].y;
		Az = vert[1].z;
		Bx = vert[2].x;
		By = vert[2].y;
		Bz = vert[2].z;
	}
	else if (x < 0.0 && y < 0.0)
	{
		// Q3:
		b = -1.0;
		d = vert[3].y;
		Ax = vert[2].x;
		Ay = vert[2].y;
		Az = vert[2].z;
		Bx = vert[3].x;
		By = vert[3].y;
		Bz = vert[3].z;
	}
	else
	{
		// Q4:
		b = 1.0;
		d = vert[3].y;
		Ax = vert[3].x;
		Ay = vert[3].y;
		Az = vert[3].z;
		Bx = vert[0].x;
		By = vert[0].y;
		Bz = vert[0].z;
	}
	
	// position ray line: a*x+c (c=0)
	double a = y / x;

	// AB size (only x,y)
	double AB2 = (Ax - Bx) * (Ax - Bx) + (Ay - By) * (Ay - By);

	// ray intersection with tile edge
	double Cx = d / (a - b);
	double Cy = a * Cx;		
	double Cz = sqrt(((Cx - Ax) * (Cx - Ax) + (Cy - Ay) * (Cy - Ay)) / AB2) * (Bz - Az) + Az;

	// center elevation
	double Dz = (tile == 'A')?(0.0):(0.5*TILE_ELEVATION);

	// finally Z elevation
	double z = sqrt((x * x + y * y) / (Cx * Cx + Cy * Cy)) * (Cz - Dz) + Dz;

	return(z);
}

// get projected y-offset of point x,y position on tile surface
double Sprite::GetTileProjY(double x, double y)
{
	// absolute point Z
	TFxyz vert;
	vert.x = x;
	vert.y = y;
	vert.z = this->GetTileZ(x, y);

	// make projection
	vert = this->ProjectVertex(&vert);
	
	// return y-position only
	return(vert.y);
}












//=============================================================================
// class AnimL1 - Layer 1 sprite animations
//=============================================================================
AnimL1::AnimL1()
{
	name[0] = '\0';
	frames.clear();
}

AnimL1::~AnimL1()
{
	name[0] = '\0';
	// loose frames
	for (unsigned k = 0; k < frames.size(); k++)
		delete frames[k];
	frames.clear();
}

// decode animation file from buffer
int AnimL1::Decode(uint8_t* data, char* name)
{
	// get frames count
	int count = *data++;

	// relative frame data offsets in data
	__int32 *frame_data_offsets = (__int32*)data;
	
	// --- for each frame:
	for (int fid = 0; fid < count; fid++)
	{
		// make new sprite
		Sprite* frame = new Sprite();
		frames.push_back(frame);
		
		// decode sprite
		int len = frames.back()->Decode(&data[*frame_data_offsets++], name);
		if (!len)
		{
			// failed
			return(1);
		}
	}

	// store dimesions to anim record
	x_size = frames.front()->x_size;
	y_size = frames.front()->y_size;
	y_ofs = frames.front()->y_ofs;

	// store animation name
	strcpy_s(this->name, sizeof(this->name), name);

	return(0);
}





//=============================================================================
// class AnimL1 - Layer 1 sprite animations
//=============================================================================
AnimPNM::AnimPNM()
{
	name[0] = '\0';	
	frames.clear();
}

AnimPNM::~AnimPNM()
{
	name[0] = '\0';
	// loose frames
	for (unsigned k = 0; k < frames.size(); k++)
		delete frames[k];
	frames.clear();
}

// decode animation file from buffer
int AnimPNM::Decode(uint8_t* data, char* name)
{
	// get frames count
	int count = *data++;

	// list of frame offsets
	uint32_t* list = (uint32_t*)data;

	// frames limits
	int xmin = 256;
	int xmax = 0;
	int ymin = 256;
	int ymax = -256;
		
	// --- for each frame:
	for (int fid = 0; fid < count; fid++)
	{
		// frame data start
		uint8_t* pnm = &data[*list++];

		// vertilcal offset
		int y_ofs = *(int16_t*)pnm; pnm += 2;

		// minimum left offset of all lines
		int x_ofs = *(uint16_t*)pnm; pnm += 2;

		// maximum line len
		int x_size = *(uint16_t*)pnm; pnm += 2;

		// lines count
		int y_size = *pnm++;

		// maximum estimate of image data size
		// ###todo: add checking, not safe!
		int max_data = y_size * (2*sizeof(int) + x_size);

		// make local image buffer
		uint8_t* buf = new uint8_t[max_data];
		uint8_t* pic = buf;

		// for each line:
		for (int y = 0; y < y_size; y++)
		{
			// scanline buffer
			uint8_t line[256];
			// clear line
			std::memset((void*)line, 0, sizeof(line));

			// this line offset in result image
			int line_offset = *pnm;

			// running chunk offset
			int chunk_ofs = 0;

			// scan line (it consist of any count of isolated chunks of pixels):
			while (*pnm != 0xFF)
			{
				// repeat while not end of scan line

				// chunk offset
				int cof = (int)*pnm++;

				// chunk len
				int col = (int)*pnm++;

				// update running chunk offset
				chunk_ofs += cof;

				// copy chunk to scanline
				std::memcpy((void*)(line + chunk_ofs), (void*)pnm, col);
				pnm += col;
				chunk_ofs += col;
			}
			// skip end of line mark
			pnm++;

			// store line data
			if (chunk_ofs == 0)
			{
				// empty line
				*(int*)pic = 0; pic += sizeof(int);
				*(int*)pic = 0; pic += sizeof(int);
			}
			else
			{
				// not empty line
				// store line offset
				*(int*)pic = line_offset;
				pic += sizeof(int);

				// store its len
				*(int*)pic = chunk_ofs - line_offset;
				pic += sizeof(int);

				// store its data
				std::memcpy((void*)pic, (void*)(line + line_offset), chunk_ofs - line_offset);
				pic += (chunk_ofs - line_offset);
			}

		}

		// update frames limits
		if (x_ofs < xmin)
			xmin = x_ofs;
		if ((x_ofs + x_size) > xmax)
			xmax = (x_ofs + x_size);

		if ((y_ofs + y_size) > ymax)
			ymax = y_ofs + y_size;
		if (y_ofs < ymin)
			ymin = y_ofs;

		// make sprite object for this frame
		Sprite *spr = new Sprite();
		// copy frame pixel data
		spr->data = new uint8_t[pic - buf];
		std::memcpy((void*)spr->data, (void*)buf, pic - buf);
		
		// store name
		strcpy_s(spr->name, sizeof(spr->name), name);

		// store frame geometry
		spr->has_transp = 1;
		spr->land_type = 0;
		spr->x_size = x_size;
		spr->y_size = y_size;
		spr->x_ofs = x_ofs;
		spr->y_ofs = y_ofs;

		// store frame to list
		frames.push_back(spr);
		
		// loose local frame buffer
		delete[] buf;

	}

	// store global PNM parameters
	this->x_min = xmin;
	this->x_max = xmax;
	this->y_min = ymin;
	this->y_max = ymax;

	// store animation name
	strcpy_s(this->name, sizeof(this->name), name);

	return(0);
}





// reserve buffer for tile context (for speedup)
int Sprite::ReserveContext(int quadrant, char tile, int size)
{
	if(quadrant < 0 || quadrant >= 4 || tile < 'A' || tile > 'M')
		return(1);
	quad[quadrant][tile-'A'].reserve(size);
	return(0);
}
// clear context list for given quadrant
int Sprite::ClearContext(int quadrant)
{
	if(quadrant < 0 || quadrant >= 4)
		return(1);
	for(char tile = 'A'; tile <= 'M'; tile++)
		quad[quadrant][tile-'A'].clear();
	return(0);
}
// add tile to context
int Sprite::AddContext(int quadrant, Sprite* sprite,bool no_check)
{
	if(quadrant < 0 || quadrant > 4)
		return(1);

	// ge ttile slope
	int tile = sprite->GetSlope() - 'A';

	// leave if the context sprite is already there
	if(!no_check)
	{
		for(auto const & cont : quad[quadrant][tile])
			if(cont == sprite)
				return(1);
	}

	// add sprite to context
	quad[quadrant][tile].push_back(sprite);

	return(0);
}
// get size of context list for quadrant
int Sprite::GetContextCount(int quadrant, char tile)
{
	if(quadrant < 0 || quadrant > 4 || tile < 'A' || tile > 'M')
		return(0);
	return(quad[quadrant][tile-'A'].size());
}
// get size of context list for quadrant (all tile types)
int Sprite::GetContextCount(int quadrant)
{
	if(quadrant < 0 || quadrant > 4)
		return(0);
	int count = 0;
	for(char tile = 'A'; tile <= 'M'; tile++)
		count += quad[quadrant][tile-'A'].size();
	return(count);
}
// get n-th sprite context tile for given tile quadrant (for given tile slope)
Sprite * Sprite::GetContext(int quadrant, char tile, int index)
{
	if(quadrant < 0 || quadrant > 4 || tile < 'A' || tile > 'M' || index < 0 || index >= quad[quadrant][tile-'A'].size())
		return(NULL);
	// return sprite pointer
	return(quad[quadrant][tile-'A'][index]);
}
// get n-th sprite context tile for given tile quadrant (all tile slopes together sequentially)
Sprite* Sprite::GetContext(int quadrant,int index)
{
	if(quadrant < 0 || quadrant > 4 || index < 0)
		return(NULL);
	int id = 0;
	for(int tid = 'A'; tid <= 'M'; tid++)
	{
		if(index < (id + quad[quadrant][tid-'A'].size()))
			return(quad[quadrant][tid-'A'][index - id]);
		id += quad[quadrant][tid-'A'].size();
	}
	return(NULL);
}
// remove list of context entries
void Sprite::RemoveContext(int quadrant,char tile,vector<int>& list)
{
	if(quadrant < 0 || quadrant > 4 || tile < 'A' || tile > 'M')
		return;

	// quite ineffective remover
	std::sort(list.begin(), list.end());
	for(int k = list.size()-1; k >= 0; k--)
		quad[quadrant][tile-'A'].erase(quad[quadrant][tile-'A'].begin() + list[k]);
}

// check context match with given sprite
int Sprite::CheckContext(int quadrant,Sprite* sprite)
{
	for(auto const cont : quad[quadrant][sprite->GetSlope()-'A'])
		if(cont == sprite)
			return(1);
	return(0);
}
// get tile flags
uint32_t Sprite::GetFlags()
{
	return(flags);
}
// set tile flags
uint32_t Sprite::SetFlags(uint32_t new_flags)
{
	flags = new_flags;
	return(flags);
}
// set tile glyph flags
uint32_t Sprite::SetGlyphFlags(uint32_t new_flags)
{
	flags2 = new_flags;
	return(flags2);
}
// get tile glyph flags
uint32_t Sprite::GetGlyphFlags()
{
	return(flags2);
}

// set edge/corner shading flags (overrides alll preexisting flags)
uint32_t Sprite::SetShadingFlags(uint32_t new_flags)
{
	shading = new_flags;
	return(shading);
}
// get edge/corner shading flags
uint32_t Sprite::GetShadingFlags()
{
	return(shading);
}
// set edge/corner shading flags mask (0 means the shading does not matter)
uint32_t Sprite::SetShadingMask(uint32_t mask)
{
	shading = (shading & 0x00FF) | (mask << 8);
	return(shading);
}
// get edge/corner shading mask
uint32_t Sprite::GetShadingMask()
{
	return((shading >> 8) & 0x00FF);
}
// is tile side shaded?
bool Sprite::isSideShaded(int side)
{
	return(!!(shading & (Sprite::SHADING_SIDE_Q1 << side)));
}
// is tile corner shaded?
bool Sprite::isCornerShaded(int corner)
{
	return(!!(shading & (Sprite::SHADING_CORNER_Q1 << corner)));
}
// is tile side shaded?
bool Sprite::isSideShadedMask(int side)
{
	return(!!((shading >> 8) & (Sprite::SHADING_SIDE_Q1 << side)));
}

// set edge class
void Sprite::SetEdgeClass(int edge, uint32_t new_class)
{
	if(edge < 0 || edge > 3)
		return;
	edge_class[edge] = new_class;
}
// get edge class
uint32_t Sprite::GetEdgeClass(int edge)
{
	if(edge < 0 || edge > 3)
		return(Sprite::CLASS_GENERIC);
	return(edge_class[edge]);
}
// get special tile classes count
int Sprite::GetSpecClassCount()
{
	return(SPEC_CLASS_MAX+1);
}
// get special tile class name string
std::string Sprite::GetSpecClassName(int id)
{
	switch(id)
	{
		case SPEC_CLASS_NONE: return("None");
		case SPEC_CLASS_GRAVE: return("Graves");
		case SPEC_CLASS_RUBBLE1: return("Light rubble");
		case SPEC_CLASS_RUBBLE2: return("Average rubble");
		case SPEC_CLASS_RUBBLE3: return("Dense rubble");		
		case SPEC_CLASS_ROCK: return("Rock");
		case SPEC_CLASS_FIREPLACE: return("Fireplace");
		case SPEC_CLASS_SCAR: return("Terrain scars");
		case SPEC_CLASS_OBJECT: return("Object/house");		
		default: return("N/A");
	}
}
// set special class
void Sprite::SetSpecClass(uint32_t class_id)
{
	spec_class = class_id;
}
// get special tile class
uint32_t Sprite::GetSpecClass()
{
	return(spec_class);
}
// tools class/group clasification
void Sprite::SetToolClass(int id)
{
	tool_class = id;
}
void Sprite::SetToolClassGroup(int id)
{
	tool_group = id;
}
int Sprite::GetToolClass()
{
	return(tool_class);
}
int Sprite::GetToolClassGroup()
{
	return(tool_group);
}



//=============================================================================
// class Terrain
//=============================================================================

Terrain::Terrain()
{
	name[0] = '\0';	
	sprites.clear();
	anms.clear();
	last_gamma = 0.0;
	context_path = L"";
}

Terrain::~Terrain()
{
	name[0] = '\0';
	
	// destruct each sprite element
	for (unsigned k = 0; k < sprites.size(); k++)
		delete sprites[k];
	sprites.clear();

	// destruct each sprite element
	for (unsigned k = 0; k < anms.size(); k++)
		delete anms[k];
	anms.clear();

	// destruct each sprite element
	for(unsigned k = 0; k < pnms.size(); k++)
		delete pnms[k];
	pnms.clear();

	// destruct each object element
	for(unsigned k = 0; k < objects.size(); k++)
		delete objects[k];
	objects.clear();

	// clear tools
	for(unsigned k = 0; k < tools.size(); k++)
		delete tools[k];
	tools.clear();
}

int Terrain::Load(wstring &path)
{	
	// store terrain tag name
	const wchar_t *tag = wcsrchr(path.c_str(), '\\');
	if (!tag)
	{
		return(1);
	}
	char temp[sizeof(this->name)];
	wcsrtombs(temp, &tag, sizeof(this->name), NULL);
	strcpy_s(this->name, sizeof(this->name), &temp[1]);
	char* pstr = strchr(this->name, '.');
	if (pstr)
		*pstr = '\0';

	// try to load FS archive
	FSarchive* fs;
	fs = new FSarchive(path);

	// --- read files from archive:
	int sprite_index = 0;
	int fcnt = 0;
	for (int i = 0; i < fs->Count(); i++)
	{		
		char* full_name;
		uint8_t* data;
		int size;
		
		// get file from archive
		fs->GetFile(i, &data, &size, &full_name);

		// local name copy
		char name[14];
		strcpy_s(name, sizeof(name), full_name);
				
		// split name and ext
		char *pstr = strrchr(name, '.');
		char* ext;
		if (pstr)
		{
			*pstr = '\0';
			ext = pstr + 1;
		}
		else
		{
			ext = &name[strlen(name) - 1];
		}
				
		// check extension
		if (ext)
		{
			// file with extension

			if (_strcmpi(ext, "DTA") == 0)
			{
				///////////////////
				///// Sprites /////
				///////////////////
				
				// add sprite list element
				Sprite* sprite = new Sprite();
				sprites.push_back(sprite);
				
				// try decode sprite data
				if (!sprite->Decode(data, name))
				{
					delete fs;
					return(1);
				}
				// set sprite index (linear unsorted)
				sprite->SetIndex(sprite_index++);
				
				//st->Caption = "Loading " + N_terr + ": " + IntToStr(fcnt) + " - " + AnsiString(name);
				fcnt++;
			}
			else if (_strcmpi(ext, "ANM") == 0)
			{
				/////////////////////////
				///// ANM animation /////
				/////////////////////////

				// add animation to list
				AnimL1* anim = new AnimL1();
				anms.push_back(anim);

				// try decode animation data
				if (anim->Decode(data, name))
				{
					delete fs;
					return(1);
				}

				//st->Caption = "Loading " + N_terr + ": " + IntToStr(fcnt) + " - " + AnsiString(name);
				fcnt++;
			}
			else if (_strcmpi(ext, "PNM") == 0)
			{
				/////////////////////////
				///// PNM animation /////
				/////////////////////////

				// add animation to list
				AnimPNM* pnm = new AnimPNM();
				pnms.push_back(pnm);

				// try decode animation data
				if (pnm->Decode(data, name))
				{
					delete fs;
					return(1);
				}

				//st->Caption = "Loading " + N_terr + ": " + IntToStr(fcnt) + " - " + AnsiString(name);
				fcnt++;
			}
			else if (_strcmpi(ext, "PAL") == 0)
			{
				if (size != 256)
				{
					///////////////////
					///// Palette /////
					///////////////////

					if (_strcmpi(full_name, "map.pal") == 0)
					{
						// "map.pal"
						std::memcpy((void*)&pal[0][0], data, 128*3);

						//st->Caption = "Loading " + N_terr + ": " + IntToStr(fcnt) + " - " + AnsiString(name);
						fcnt++;
					}
					else if (_strcmpi(full_name, "cycle.pal") == 0)
					{
						// "cycle.pal"
						std::memcpy((void*)&pal[240][0], data, 10*3);

						//st->Caption = "Loading " + N_terr + ": " + IntToStr(fcnt) + " - " + AnsiString(name);
						fcnt++;
					}
				}
				else
				{
					//////////////////
					///// Filter /////
					//////////////////
					
					// these are color reindexing filters, ie. 256 bytes represent new 256 colors, each points to some original color
					if (_strcmpi(full_name, "darkpal.pal"))
						memcpy((void*)filter.darkpal, (void*)data, 256);
					else if (_strcmpi(full_name, "darkpal2.pal"))
						memcpy((void*)filter.darkpal2, (void*)data, 256);
					else if (_strcmpi(full_name, "darker.pal"))
						memcpy((void*)filter.darker, (void*)data, 256);
					else if (_strcmpi(full_name, "bluepal.pal"))
						memcpy((void*)filter.bluepal, (void*)data, 256);
					else if (_strcmpi(full_name, "dbluepal.pal"))
						memcpy((void*)filter.dbluepal, (void*)data, 256);
					else if (_strcmpi(full_name, "redpal.pal"))
						memcpy((void*)filter.redpal, (void*)data, 256);

					//st->Caption = "Loading " + N_terr + ": " + IntToStr(fcnt) + " - " + AnsiString(name);
					fcnt++;
				}
			}
		}
	}

	// free archive
	delete fs;

	// initialize context list
	//wstring cont_path = L"";
	//InitSpriteContext(cont_path);
	
	//st->Caption = "Loading " + N_terr + ": " + IntToStr(fcnt) + " files in " + IntToStr(UN->memlen / 1024) + "kB ...";

	return(0);
	
}

// get sprite pointer by its name
Sprite* Terrain::GetSprite(const char* name)
{
	for (unsigned k = 0; k < this->sprites.size(); k++)
	{
		if (_strcmpi(this->sprites[k]->name, name) == 0)
			return(this->sprites[k]);
	}
	return(NULL);
}
// get sprite ID by its name
int Terrain::GetSpriteID(const char* name)
{
	for(unsigned k = 0; k < this->sprites.size(); k++)
	{
		if(_strcmpi(this->sprites[k]->name,name) == 0)
			return(k);
	}
	return(-1);
}
// get sprite pointer by index
Sprite* Terrain::GetSprite(int index)
{
	if(index >= sprites.size())
		return(NULL);
	return(sprites[index]);
}
// get sprites count
int Terrain::GetSpriteCount()
{
	return(sprites.size());
}
// get order index in terrain list of sprite by sprite pointer
int Terrain::GetSpriteID(Sprite* spr)
{
	/*for(int k = 0; k < sprites.size(); k++)
		if(sprites[k] == spr)
			return(k);*/
	if(spr)
		return(spr->GetIndex());
	// not found
	return(-1);
}

// get sprite by wildcard string
Sprite* Terrain::GetSpriteWild(const char* wild, WildMode mode)
{
	if(mode == FIRST)
	{
		// get first match
		for(unsigned k = 0; k < sprites.size(); k++)
			if(wildcmp(wild,sprites[k]->name))
				return(sprites[k]);
		return(NULL);
	}
	else
	{
		// get random match
		Sprite** list = new Sprite*[sprites.size()];
		Sprite *ret = NULL;
		int count = 0;
		for(unsigned k = 0; k < sprites.size(); k++)
			if(wildcmp(wild,sprites[k]->name))
				list[count++] = sprites[k];
		if(count)
			ret = list[rand()%count];
		delete[] list;
		return(ret);
	}
	
	return(NULL);
}

// initialize sprite tiles context from file (optional)
int Terrain::InitSpriteContext(wstring &path)
{
	// total sprites count
	int spr_count = sprites.size();
	
	// leave if not path
	if(!path.length())
		return(1);
	
	ifstream fr(path,ios::in | ios::binary);
	if(!fr.is_open())
		return(1);

	// check version
	const char ver_ref[] = "SpellMapEditContextV1.0";
	char ver[sizeof(ver_ref)];
	fr.read(ver,sizeof(ver_ref));
	if(memcmp(ver, ver_ref, sizeof(ver_ref)))
	{
		fr.close();
		return(1);
	}

	
	// --- tool clasification:	

	// read tool classes count
	uint32_t tool_count;
	fr.read((char*)&tool_count,sizeof(uint32_t));
	// for each tool list:
	for(int tid = 0; tid < tool_count; tid++)
	{
		// get class name
		uint16_t len;
		char *temp;
		fr.read((char*)&len, sizeof(uint16_t));		
		temp = new char[len];
		fr.read(temp,len);
		std::string name(temp);
		delete[] temp;

		// get class title
		fr.read((char*)&len,sizeof(uint16_t));
		temp = new char[len];
		fr.read(temp,len);
		std::string title(temp);
		delete[] temp;

		// make tool class
		SpellToolsGroup* grp = new SpellToolsGroup(name,title);
		tools.push_back(grp);
						
		// read tool items count in class
		uint32_t items_count;
		fr.read((char*)&items_count,sizeof(uint32_t));		

		// for each item in tool class:
		vector<int> list;
		for(int k = 0; k < items_count; k++)
		{
			// read item name
			fr.read((char*)&len,sizeof(uint16_t));
			temp = new char[len];
			fr.read(temp,len);
			std::string tool_name(temp);
			delete[] temp;

			// add tool to class list
			grp->AddItem(tool_name);

		}
		
	}


	// get context tile list size
	uint32_t count;
	fr.read((char*)&count, sizeof(uint32_t));

	// read tile names
	//int *list = new int[count];
	vector<int> list;
	list.reserve(count);
	vector<Sprite*> spr_list;
	spr_list.reserve(count);
	for(int k = 0; k < count; k++)
	{
		// get tile name
		char tile_name[9];
		fr.read(tile_name, sizeof(tile_name));
		
		// try to find its index in terrain list
		Sprite *spr = GetSprite(tile_name);
		spr_list.push_back(spr);
		list.push_back(spr->GetIndex());
	}

	// for each tile in the list:
	for(int k = 0; k < count; k++)
	{
		// get reference tile		
		if(list[k] < 0)
			continue;

		for(int quid = 0; quid < 4; quid++)
		{
			// L1 context count
			uint32_t cont_count;
			fr.read((char*)&cont_count,sizeof(uint32_t));

			// reserve context buffer (faster adding tiles)
			/*for(int tid = 'A'; tid <= 'M'; tid++) // ###temp
				sprites[list[k]]->ReserveContext(quid, tid, cont_count);*/

			// for each context tile
			for(int sid = 0; sid < cont_count; sid++)
			{
				// get context tile index				
				uint16_t tile_id;
				fr.read((char*)&tile_id,sizeof(uint16_t));
				
				// reindex to terrain indices
				int terr_id = list[tile_id];
				if(terr_id < 0)
					continue;
											
				// add to context list (not check duplicates)
				sprites[list[k]]->AddContext(quid, sprites[terr_id], true);
			}
		}

		// load edge classes
		for(int quid = 0; quid < 4; quid++)
		{
			// read class
			uint8_t edge_class;
			fr.read((char*)&edge_class,sizeof(uint8_t));
			sprites[list[k]]->SetEdgeClass(quid, edge_class);
		}

		// load flags
		uint32_t flags;
		fr.read((char*)&flags,sizeof(uint32_t));
		sprites[list[k]]->SetFlags(flags);
		
		// load special flags
		fr.read((char*)&flags,sizeof(uint32_t));
		sprites[list[k]]->SetGlyphFlags(flags);

		// load special tile class
		uint32_t spec_class;
		fr.read((char*)&spec_class,sizeof(uint32_t));
		sprites[list[k]]->SetSpecClass(spec_class);

		// load shading flags		
		fr.read((char*)&flags,sizeof(uint32_t));
		sprites[list[k]]->SetShadingFlags(flags & 0x00FF);
		sprites[list[k]]->SetShadingMask((flags >> 8) & 0x00FF);

		// load tools class id
		uint32_t tool_class;
		fr.read((char*)&tool_class,sizeof(uint32_t));
		
		// load tools class item id
		uint32_t tool_class_item;
		fr.read((char*)&tool_class_item,sizeof(uint32_t));
		
		// set class data
		sprites[list[k]]->SetToolClass(tool_class);
		sprites[list[k]]->SetToolClassGroup(tool_class_item);
	}


	// get terrain name
	/*uint32_t terr_len;
	char terr_name[MAX_STR+1];
	fr.read((char*)&terr_len,sizeof(uint32_t));
	if(terr_len > MAX_STR)
	{
		fr.close();
		return(1);
	}
	fr.read(terr_name,terr_len);
	if(std::strcmp(terr_name,name))
	{
		// wrong terrain!
		fr.close();
		return(1);
	}*/

	// get objects count
	int obj_count;
	fr.read((char*)&obj_count,sizeof(uint32_t));

	// clear object list
	objects.clear();
	objects.reserve(obj_count);

	// --- for each object:
	for(int k = 0; k < obj_count;k++)
	{
		// read object data
		SpellObject* obj = new SpellObject(fr,spr_list,(uint8_t*)pal);
		objects.push_back(obj);
	}

	// close context file
	fr.close();

	// store load path expanded to absolute
	context_path = std::filesystem::absolute(path).wstring();

	// select tiles to be used as class type glyphs
	UpdateTileGlyphs();

	return(0);
}
// save terrain sprites context to file
int Terrain::SaveSpriteContext(wstring& path)
{
	// create file
	ofstream fw(path,ios::out | ios::binary);
	if(!fw.is_open())
		return(1);

	// store last path
	context_path = path;

	// store version string
	const char ver[] = "SpellMapEditContextV1.0";
	fw.write(ver, sizeof(ver));

	// --- tool clasification:
	// store tool classes count
	uint32_t tool_count = GetToolsCount();
	fw.write((char*)&tool_count,sizeof(uint32_t));
	// for each tool list:
	for(int tid = 0; tid < tool_count; tid++)
	{
		// get tool set
		SpellToolsGroup *tool = GetToolSet(tid);
		
		// store tool set name
		string name = tool->GetClassName();
		uint16_t len = name.size() + 1;
		fw.write((char*)&len, sizeof(uint16_t));
		fw.write(name.c_str(), len);

		// store tool set name
		string title = tool->GetClassTitle();
		len = title.size() + 1;
		fw.write((char*)&len,sizeof(uint16_t));
		fw.write(title.c_str(),len);
		
		// store class items count
		uint32_t items_count = tool->GetCount();
		fw.write((char*)&items_count,sizeof(uint32_t));

		// store class item names:
		for(int k = 0; k < items_count; k++)
		{
			// store item name
			string name = tool->GetItem(k);
			uint16_t len = name.size() + 1;
			fw.write((char*)&len,sizeof(uint16_t));
			fw.write(name.c_str(),len);
		}
	}


	// store sprite count
	uint32_t count = sprites.size();
	fw.write((char*)&count, sizeof(uint32_t));

	// store sprite name list, following code will work with indexes corresponding to this list
	for(int k = 0; k < count;k++)
		fw.write(sprites[k]->name, sizeof(sprites[k]->name));

	// --- for each sprite:
	for(int k = 0; k < count;k++)
	{
		// for each tile side:
		for(int quid = 0; quid < 4; quid++)
		{
			// L1 context count
			uint32_t cont_count = sprites[k]->GetContextCount(quid);
			fw.write((char*)&cont_count,sizeof(uint32_t));

			// for each context tile
			for(int sid = 0; sid < cont_count; sid++)
			{
				// get context tile index
				Sprite *sprite = sprites[k]->GetContext(quid, sid);
				// store index
				uint16_t tile_id = sprite->GetIndex();
				fw.write((char*)&tile_id,sizeof(uint16_t));
			}

		}
		// store edge classes
		for(int quid = 0; quid < 4; quid++)
		{
			uint8_t edge_class = sprites[k]->GetEdgeClass(quid);
			fw.write((char*)&edge_class,sizeof(uint8_t));
		}

		// store flags
		uint32_t flags = sprites[k]->GetFlags();
		fw.write((char*)&flags,sizeof(uint32_t));
		
		// store special flags
		flags = sprites[k]->GetGlyphFlags();
		fw.write((char*)&flags,sizeof(uint32_t));
		
		// store special tile class
		uint32_t spec_class = sprites[k]->GetSpecClass();
		fw.write((char*)&spec_class,sizeof(uint32_t));
		
		// store shading flags and masks
		uint32_t shade_flags = sprites[k]->GetShadingFlags() | (sprites[k]->GetShadingMask() << 8);
		fw.write((char*)&shade_flags,sizeof(uint32_t));

		// store tool class
		uint32_t tool_class = sprites[k]->GetToolClass();
		fw.write((char*)&tool_class,sizeof(uint32_t));

		// store tool class item
		uint32_t tool_class_item = sprites[k]->GetToolClassGroup();
		fw.write((char*)&tool_class_item,sizeof(uint32_t));				
	}


	// store objects count
	uint32_t obj_count = objects.size();
	fw.write((char*)&obj_count,sizeof(uint32_t));

	// for each object:
	for(auto const & obj : objects)
	{
		// write object data
		obj->WriteToFile(fw);
	}


	// close file
	fw.close();

	return(0);
}
// return last context path
wstring& Terrain::GetSpriteContextPath()
{
	return(context_path);
}
// clear sprite context (only the Q1-Q4 tiles context, but not the flags of course!)
void Terrain::ClearSpriteContext()
{
	for(auto const & spr : sprites)
	{
		// clear all quadrants
		for(int k = 0; k < 4; k++)
			spr->ClearContext(k);
	}
}
// auto build context based on tile edge data
int Terrain::UpdateSpriteContext(std::function<void(std::string)> status_cb)
{
	// for each sprite:
	for(auto const & ref : sprites)
	{	
		// debug
		/*if(ref->name[0] != 'P' && ref->name[1] != 'L')
			continue;
		ref->ClearContext(0);
		ref->ClearContext(1);
		ref->ClearContext(2);
		ref->ClearContext(3);*/

		string status = string_format("Processing sprite #%d: %s", ref->GetIndex(), ref->name);
		if(status_cb)
			status_cb(status);
		
		// check possible neighbors:
		for(auto const & neig: sprites)
		{
						
			/*if(neig->name[0] != 'P' && neig->name[1] != 'L')
				continue;*/

			// repeat for each tile side:
			bool match = 1;
			for(int eid = 0; eid < 4; eid++)
			{
				// skip generic classes
				if(ref->GetEdgeClass(eid) == Sprite::CLASS_GENERIC)
					continue;

				int eid2 = eid + 2;
				if(eid2 >= 4)
					eid2 -= 4;
				// skip no matching edge classes
				if(ref->GetEdgeClass(eid) != neig->GetEdgeClass(eid2))
					continue;

				// get ref tile edge vertices
				TFxyz refv[2];
				ref->GetTileEdge(eid, refv);
				
				// get dut tile edge vertices
				TFxyz dutv[2];
				neig->GetTileEdge(eid2,dutv);

				// skip if not matching tile types (based on tile model, ###todo: optimize by strict tile slope lists?)
				bool same_level = (refv[0].z == dutv[1].z) && (refv[1].z == dutv[0].z);
				bool diff_level = (dutv[1].z - refv[0].z) == (dutv[0].z - refv[1].z);
				if(!same_level && !diff_level)
					continue;
				
				// ref tile side shading
				bool ref_is_shaded = ref->isSideShaded(eid) || !ref->isSideShadedMask(eid);
				bool ref_not_shaded = !ref->isSideShaded(eid) || !ref->isSideShadedMask(eid);
				// neighboring tile side shading
				bool neig_is_shaded = neig->isSideShaded(eid2) || !neig->isSideShadedMask(eid2);
				bool neig_not_shaded = !neig->isSideShaded(eid2) || !neig->isSideShadedMask(eid2);

				// check shading matches				
				// It should work now except there are errors in spellcross maps itself (sometimes
				// missing correct shading combinations, so violating them in some cases)
				// ###todo: implement corner flags checking (not that visible, so screw it for now)
				// ###note: there are likely few invalid tile combinations (sharp edges) but it should not make any troubles
				bool is_sharp = 0;
				bool can_shade = 0;
				char slp = ref->GetSlope();
				char nslp = neig->GetSlope();
				bool match = false;
				switch(slp)
				{
					case 'A':
						if(nslp == 'A')
							match = ref_not_shaded && neig_not_shaded;
						else if(nslp == 'B' || nslp == 'F' || nslp == 'I' || nslp == 'L')
							match = ref_is_shaded; // note: this is valid because of fail in spellcross tiles design
						else if(nslp == 'C' || nslp == 'E' || nslp == 'H' || nslp == 'K')
							match = ref_not_shaded && neig_is_shaded;
						else if((nslp == 'D' && eid == 1) || (nslp == 'G' && eid == 2) || (nslp == 'J' && eid == 3) || (nslp == 'M' && eid == 0))
							match = ref_not_shaded && neig_is_shaded;
						else
							match = ref_is_shaded && neig_is_shaded;
						break;					
					
					case 'B':
						can_shade = (eid == 0 || eid == 1);
						is_sharp = nslp == 'J' || nslp == 'G' || nslp == 'I' || nslp == 'L' || nslp == 'F';
						goto BFIL;
					case 'F':
						can_shade = (eid == 1 || eid == 2);
						is_sharp = nslp == 'J' || nslp == 'M' || nslp == 'B' || nslp == 'I' || nslp == 'L';
						goto BFIL;
					case 'I':
						can_shade = (eid == 2 || eid == 3);
						is_sharp = nslp == 'D' || nslp == 'M' || nslp == 'B' || nslp == 'F' || nslp == 'L';
						goto BFIL;
					case 'L':
						can_shade = (eid == 3 || eid == 0);
						is_sharp = nslp == 'D' || nslp == 'G' || nslp == 'B' || nslp == 'F' || nslp == 'I';
					BFIL:
						if(!can_shade)
							match = neig_not_shaded; // not shadeable side: do not allow neighbor shading
						else if(nslp == 'A')
							match = neig_is_shaded; // note: spellcross fail, allowing neighbor when at least 'A' is shaded
						else if(is_sharp)
							match = ref_is_shaded && neig_is_shaded; // sharp edges: both shaded only
						else
							match = (neig_is_shaded && ref_is_shaded) || (neig_not_shaded && ref_not_shaded);
						break;
					
					case 'C':
						can_shade = (eid == 2 || eid == 3);
						is_sharp = nslp == 'G' || nslp == 'J' || nslp == 'E' || nslp == 'H' || nslp == 'K';
						goto CEHK;
					case 'E':
						can_shade = (eid == 3 || eid == 0);
						is_sharp = nslp == 'M' || nslp == 'J' || nslp == 'C' || nslp == 'H' || nslp == 'K';
						goto CEHK;
					case 'H':
						can_shade = (eid == 0 || eid == 1);
						is_sharp = nslp == 'M' || nslp == 'D' || nslp == 'C' || nslp == 'E' || nslp == 'K';
						goto CEHK;
					case 'K':						
						can_shade = (eid == 1 || eid == 2);
						is_sharp = nslp == 'G' || nslp == 'D' || nslp == 'C' || nslp == 'E' || nslp == 'H';
					CEHK:
						if(!can_shade)
							match = neig_not_shaded; // not shadeable side: do not allow neighbor shading
						else if(nslp == 'A')
							match = neig_not_shaded && ref_is_shaded;
						else if(is_sharp)
							match = ref_is_shaded && neig_is_shaded; // sharp edges: both shaded only
						else
							match = neig_not_shaded && ref_not_shaded; // all others must not be shaded
						break;

					case 'D':
						can_shade = (eid == 1 || eid == 3);
						is_sharp = nslp == 'J' || nslp == 'H' || nslp == 'K' || nslp == 'I' || nslp == 'L' ||(eid == 1 && nslp == 'A');
						goto DGJM;
					case 'G':
						can_shade = (eid == 0 || eid == 2);
						is_sharp = nslp == 'M' || nslp == 'C' || nslp == 'K' || nslp == 'B' || nslp == 'L' || (eid == 2 && nslp == 'A');
						goto DGJM;
					case 'J':
						can_shade = (eid == 1 || eid == 3);
						is_sharp = nslp == 'D' || nslp == 'C' || nslp == 'E' || nslp == 'B' || nslp == 'F' || (eid == 3 && nslp == 'A');
						goto DGJM;
					case 'M':
						can_shade = (eid == 0 || eid == 2);
						is_sharp = nslp == 'G' || nslp == 'H' || nslp == 'E' || nslp == 'F' || nslp == 'I' || (eid == 0 && nslp == 'A');
					DGJM:
						if(!can_shade)
							match = neig_not_shaded; // not shadeable side: do not allow neighbor shading
						else if(is_sharp)
							match = ref_is_shaded && neig_is_shaded; // sharp edges: both shaded only
						else if(nslp == 'A')
							match = neig_not_shaded && ref_is_shaded; // bottom 'A' tile: shading only this tile						
						else
							match = neig_not_shaded && ref_not_shaded; // all others must not be shaded
						break;
					
					default:
						match = false;
				}
				if(!match)
					continue;

				// edge class match - add sprite to context list
				ref->AddContext(eid, neig);
			}
		}
	}

	// remove context data that are not allowed (e.g. two parallel roads or so)
	// for each sprite:
	for(auto const& ref : sprites)
	{						
		string status = string_format("Removing context for sprite #%d: %s",ref->GetIndex(),ref->name);
		if(status_cb)
			status_cb(status);

		/*if(strcmp(ref->name, "CGA0_120") == 0)
			status_cb("test");*/


		uint32_t road_mask = Sprite::IS_ROAD | Sprite::IS_DIRT_ROAD | Sprite::IS_MUD_PATH;
		if(ref->GetFlags() & road_mask)
		{
			uint32_t road_type = ref->GetFlags() & road_mask;

			for(int qid = 0; qid < 4; qid++)
			{
				// matching neighbor edge
				int qid180 = (qid + 2) & 3;
				
				// to remove list
				vector<int> rem;
				rem.clear();

				// mark invalids
				bool ref_edge_generic = ref->GetEdgeClass(qid) == Sprite::CLASS_GENERIC;
				for(int k = 0; k < ref->GetContextCount(qid, 'A'); k++)
				{
					Sprite *cont = ref->GetContext(qid, 'A', k);

					if(!(cont->GetFlags() & road_type))
						continue;
										
					bool cont_edge_generic = cont->GetEdgeClass(qid180) == Sprite::CLASS_GENERIC;
					if(!ref_edge_generic && !cont_edge_generic)
						rem.push_back(k);
				}

				// remove list of invalids
				ref->RemoveContext(qid, 'A', rem);								
			}
		}
	}


	return(0);
}
// auto set edge/corner shading flags from known sprite types (PLx*)
int Terrain::InitSpriteContextShading()
{
	for(int k = 0; k < sprites.size(); k++)
	{		
		Sprite *spr = sprites[k];
		if(wildcmp("PL???_??",spr->name))
		{
			// PL[s][c][e]_[nn]
			//  s - slope
			//  c - corner shading
			//  e - edge shading
			//  nn - alternative index
			Sprite* cont = sprites[k];

			const uint32_t edge_list[] = {
				Sprite::SHADING_SIDE_Q4, Sprite::SHADING_SIDE_Q1, Sprite::SHADING_SIDE_Q2, Sprite::SHADING_SIDE_Q3};
			const uint32_t corn_list[] ={
				Sprite::SHADING_CORNER_Q4, Sprite::SHADING_CORNER_Q1, Sprite::SHADING_CORNER_Q2, Sprite::SHADING_CORNER_Q3};
			
			// decode flags
			uint32_t flags = 0x00;
			uint32_t mask = 0x00;
			for(int f = 0; f < 4; f++)
			{
				mask |= edge_list[f];
				if(hex2num(spr->name[3]) & (1<<f))
					flags |= edge_list[f];
			}
			for(int f = 0; f < 4; f++)
			{
				mask |= corn_list[f];
				if(hex2num(spr->name[4]) & (1<<f))
					flags |= corn_list[f];
			}

			// assign to context
			cont->SetShadingFlags(flags);
			cont->SetShadingMask(mask);

			// also set edge classes as they are all the same
			if(_strcmpi(this->name,"T11") == 0 || _strcmpi(this->name,"DEVAST") == 0)
			{
				// for T11/DEVAST the default layer is grass
				for(int e = 0; e < 4; e++)
					cont->SetEdgeClass(e,Sprite::CLASS_GRASS);
				cont->SetFlags(Sprite::IS_GRASS);
			}
			else if(_strcmpi(this->name,"PUST") == 0)
			{
				// for PUST the default layer is sand
				for(int e = 0; e < 4; e++)
					cont->SetEdgeClass(e,Sprite::CLASS_SAND);
				cont->SetFlags(Sprite::IS_SAND);
			}			
		}
	}
	return(0);
}

// make list of tile glyphs from tile class flags (call before first map rendering)
int Terrain::UpdateTileGlyphs()
{
	for(int sid = 'A'; sid <= 'M'; sid++)
	{
		glyphs[sid - 'A'].clear();
		for(int k = 0; k < sprites.size(); k++)
		{
			Sprite *spr = sprites[k];
			if(spr->isClassGlyph() && spr->GetSlope() == sid)
				glyphs[sid - 'A'].push_back(spr);
		}
	}
	return(0);
}
// try to find sprite glyph matching input sprite class flags
Sprite *Terrain::GetTileGlyph(Sprite *sprite, uint32_t flags)
{
	if(!flags)
		flags = sprite->GetFlags();
	vector<Sprite *> list = glyphs[sprite->GetSlope() - 'A'];
	for(int k = 0; k < list.size(); k++)
		if(list[k]->GetFlags() == flags)
			return(list[k]);
	return(NULL);
}



// render sprite(s) to buffer for sprite preview
int Terrain::RenderPreview(wxBitmap& bmp, int count, int *tiles, int flags, double gamma)
{
	if(count > 5)
		return(1);

	// zoom mode
	int zoom = (flags&Terrain::RENDER_ZOOMX2)?2:1;
	
	// allocate render buffer for indexed image
	int surf_x = bmp.GetWidth();
	int surf_y = bmp.GetHeight();
	uint8_t *buf = new uint8_t[surf_x*surf_y];
	uint8_t *buf_end = &buf[surf_x*surf_y];

	// clear background
	std::memset((void*)buf, 230, surf_x*surf_y);

	// multiple tiles mode origins (cener, then ccw from Q1 to Q4)
	const int org_x[5] = {0,+40,-40,-40,+40};
	const int org_y[5] = {0,-24,-24,+24,+24};

	if(count >= 1)
	{
		int xs, ys;
		Sprite* spr_ref = NULL;
		for(int tid = 0;tid < count;tid++)
		{
			int sid = tiles[tid];
			if(sid < 0)
				continue;
			Sprite *spr = sprites[sid];
			
			// center
			int elev = 0;
			if(tid == 0)
			{
				xs = spr->x_size;
				ys = spr->y_size + spr->y_ofs;
				spr_ref = spr;
			}
			else if(spr_ref)
			{
				// get refence tile edge
				TFxyz vref[2];
				spr_ref->GetTileEdge(tid-1, vref);
				
				// get neighbor edge				
				int edge_x = tid - 1 + 2;
				if(edge_x > 3)
					edge_x -= 4;
				TFxyz nref[2];
				spr->GetTileEdge(edge_x,nref);

				// align neighbor so edge elevation matches center tile
				if(nref[1].z > vref[0].z || nref[0].z > vref[1].z)
					elev--;
				else if(nref[1].z < vref[0].z || nref[0].z < vref[1].z)
					elev++;
			}

			int ref_x = (surf_x/zoom - xs)/2 + org_x[tid];
			int ref_y = (surf_y/zoom - ys)/2 + org_y[tid] - elev*18;

			// render tile
			spr->Render(buf, buf_end, ref_x, ref_y,surf_x);
		}
	}

	// apply gamma correction to palette:
	if(gamma != last_gamma)
	{
		// store last gamma to prevent recalculatiom in each frame
		last_gamma = gamma;

		// male local copy of palette	
		std::memcpy((void*)gamma_pal,(void*)pal,3 * 256);

		// apply gamma correction (this should be maybe optimized out of here?
		for(int k = 0; k < 256; k++)
			for(int c = 0; c < 3; c++)
				gamma_pal[k][c] = (uint8_t)(pow((double)pal[k][c] / 255.0,1.0 / gamma) * 255.0);
	}

	// render 24bit RGB data to raw bmp buffer
	wxNativePixelData data(bmp);
	wxNativePixelData::Iterator p(data);
	for(int y = 0; y < surf_y; ++y)
	{
		uint8_t* scan = p.m_ptr;
		uint8_t* src = &buf[y/zoom*surf_x];
		for(int x = 0; x < surf_x; x++)
		{
			*scan++ = gamma_pal[*src][2];
			*scan++ = gamma_pal[*src][1];
			*scan++ = gamma_pal[*src][0];
			src += (zoom == 1)?1:(x&1);
		}
		p.OffsetY(data,1);
	}

	// loose local buffer
	delete[] buf;

	return(0);
}



// get L1 animation (ANM) pointer by its name
AnimL1* Terrain::GetANM(const char* name)
{
	for (unsigned k = 0; k < this->anms.size(); k++)
	{
		if (_strcmpi(this->anms[k]->name, name) == 0)
			return(this->anms[k]);
	}
	return(NULL);
}

// get L4 animation (PNM) pointer by its name
AnimPNM* Terrain::GetPNM(const char* name)
{
	for (unsigned k = 0; k < this->pnms.size(); k++)
	{
		if (_strcmpi(this->pnms[k]->name, name) == 0)
			return(this->pnms[k]);
	}
	return(NULL);
}







//=============================================================================
// Spellcross map objects stuff
//=============================================================================
// make object
SpellObject::SpellObject(vector<MapXY> xy,vector<Sprite*> L1_list,vector<Sprite*> L2_list,vector<uint8_t> flag_list,uint8_t* palette,std::string desc)
{
	sprite_pos.clear();
	L1_sprites.clear();
	L2_sprites.clear();
	flags.clear();
	description = desc;	
	last_gamma = 0.0;
	if(palette)
		std::memcpy((void*)pal, (void*)palette, 256*3);
	pic = NULL;
	surf_x = 0;
	surf_y = 0;
	tool_class = 0;
	tool_group = 0;

	// find reference tile (bottom right)
	MapXY ref(1<<30, 1<<30);	
	for(int k = 0; k < xy.size(); k++)
	{
		ref.x = min(xy[k].x, ref.x);
		ref.y = min(xy[k].y, ref.y);
	}		
	// ref tile is on even tile
	bool y_is_even = !(ref.y & 1);

	// store tile list	
	MapXY ref2(1<<30, 1<<30);
	for(int k = 0; k < xy.size(); k++)
	{
		// relative tile positions
		MapXY pos;
		pos.x = xy[k].x - ref.x;
		pos.y = xy[k].y - ref.y;
		if(!y_is_even && (xy[k].y&1))
			pos.x--; // aligning zig-zag tile x-offsets
		sprite_pos.push_back(pos);
		ref2.x = min(pos.x, ref2.x);
		ref2.y = min(pos.y, ref2.y);

		// tile sprite pointers
		L1_sprites.push_back(L1_list[k]);
		L2_sprites.push_back(L2_list[k]);
		flags.push_back(flag_list[k]);
	}
	// align from 0,0 (lazy solution)
	for(int k = 0; k < sprite_pos.size(); k++)
		sprite_pos[k].x -= ref2.x;

	// render object glyph to internal buffer
	RenderObjectGlyph();
}
// clear objectg stuff
SpellObject::~SpellObject()
{
	if(pic)
		delete[] pic;
}

// render glyph to internal indexed buffer
int SpellObject::RenderObjectGlyph()
{
	if(!sprite_pos.size())
		return(1);

	// get dimensions for rendering
	int x_size = 0;
	int y_size = 0;
	int x_ref = 1<<30;
	for(int k = 0; k < sprite_pos.size(); k++)
	{
		x_ref = min(sprite_pos[k].x*80 + ((sprite_pos[k].y & 1)?0:40),x_ref);
		x_size = max(sprite_pos[k].x*80 + ((sprite_pos[k].y & 1)?0:40)+80,x_size);
		y_size = max(sprite_pos[k].y*24 + 48,y_size);
	}
	x_ref = -x_ref;
	x_size += x_ref;	

	// allocate render buffer for indexed image
	surf_x = x_size;
	surf_y = 0 + y_size + 0;		
	// render origin
	int org_x = x_ref;
	int org_y = 0;

	// (re)allocate image buffer
	if(pic)
		delete[] pic;
	pic = new uint8_t[surf_x*surf_y];
	pic_end = &pic[surf_x*surf_y];

	// clear background
	std::memset((void*)pic,230,surf_x*surf_y);

	// L1 render:
	for(int tid = 0;tid < sprite_pos.size(); tid++)
	{
		// tile position in surface
		int sx = sprite_pos[tid].x;
		int sy = sprite_pos[tid].y;
		int ref_x = org_x + sx*80 + ((sy&1)?0:40);
		int ref_y = org_y + sy*24;

		// render tile
		Sprite* spr = L1_sprites[tid];
		spr->Render(pic,pic_end,ref_x,ref_y,surf_x);
	}
	// L2 render:
	for(int tid = 0;tid < sprite_pos.size(); tid++)
	{		
		Sprite* spr = L2_sprites[tid];
		if(!spr)
			continue;

		// tile position in surface
		int sx = sprite_pos[tid].x;
		int sy = sprite_pos[tid].y;
		int ref_x = org_x + sx*80 + ((sy&1)?0:40);
		int ref_y = org_y + sy*24;

		// apply L1 y-offset
		Sprite* L1 = L1_sprites[tid];
		sy += L1->y_ofs;

		// render tile		
		spr->Render(pic,pic_end,ref_x,ref_y,surf_x);
	}

	return(0);
}

// render preview to widgets bitmap
int SpellObject::RenderPreview(wxBitmap &bmp,double gamma)
{
	if(!pic)
		return(1);
	
	// apply gamma correction to palette:
	if(gamma != last_gamma)
	{
		// store last gamma to prevent recalculatiom in each frame
		last_gamma = gamma;

		// male local copy of palette	
		std::memcpy((void*)gamma_pal,(void*)pal,3 * 256);

		// apply gamma correction (this should be maybe optimized out of here?
		for(int k = 0; k < 256; k++)
			for(int c = 0; c < 3; c++)
				gamma_pal[k][c] = (uint8_t)(pow((double)pal[k][c] / 255.0,1.0 / gamma) * 255.0);
	}

	// setp bitmap
	//wxBitmap bmp(surf_x,surf_y,32);

	int bmp_xsz = bmp.GetWidth();
	int bmp_ysz = bmp.GetHeight();
	int x_ofs = (surf_x - bmp_xsz)/2;
	int y_ofs = (surf_y - bmp_ysz)/2;

	if(bmp.HasAlpha() && bmp.GetDepth() == 32)
	{
		// render 32bit RGBA data to raw bmp buffer
		typedef wxPixelData<wxBitmap,wxAlphaPixelFormat> PixelData;
		PixelData data(bmp);
		PixelData::Iterator p(data);
		for(int y = 0; y < bmp_ysz; ++y)
		{
			uint8_t* scan = p.m_ptr;
			uint8_t* src = &pic[(y + y_ofs)*surf_x + x_ofs];			
			for(int x = 0; x < bmp_xsz; x++)
			{
				if((x + x_ofs) < 0 || (x + x_ofs) >= surf_x || (y + y_ofs) < 0 || (y + y_ofs) >= surf_y)
				{
					// invalid source area: render blank
					*scan++ = 0;
					*scan++ = 0;
					*scan++ = 0;
					*scan++ = 0; // alpha channel
				}
				else
				{
					// visible area
					*scan++ = gamma_pal[*src][2];
					*scan++ = gamma_pal[*src][1];
					*scan++ = gamma_pal[*src][0];
					*scan++ = (*src != 230)*255; // alpha channel
				}
				src++;
			}
			p.OffsetY(data,1);
		}
	}
	else if(!bmp.HasAlpha() && bmp.GetDepth() == 24)
	{
		// render 24bit RGB data to raw bmp buffer
		/*typedef wxPixelData<wxBitmap,wxPixelFormat<unsigned char,24,wxNativePixelFormat::RED,wxNativePixelFormat::GREEN,wxNativePixelFormat::BLUE>> PixelData;
		PixelData data(bmp);
		PixelData::Iterator p(data);*/
		wxNativePixelData data(bmp);
		wxNativePixelData::Iterator p(data);
		for(int y = 0; y < bmp_ysz; ++y)
		{
			uint8_t* scan = p.m_ptr;
			uint8_t* src = &pic[(y + y_ofs)*surf_x + x_ofs];
			for(int x = 0; x < bmp_xsz; x++)
			{
				if((x + x_ofs) < 0 || (x + x_ofs) >= surf_x || (y + y_ofs) < 0 || (y + y_ofs) >= surf_y)
				{
					// invalid source area: render blank
					*scan++ = gamma_pal[230][2];
					*scan++ = gamma_pal[230][1];
					*scan++ = gamma_pal[230][0];
				}
				else
				{
					// visible area
					*scan++ = gamma_pal[*src][2];
					*scan++ = gamma_pal[*src][1];
					*scan++ = gamma_pal[*src][0];
				}
				src++;
			}
			p.OffsetY(data,1);
		}
	}


	return(0);
}

// get object description
std::string SpellObject::GetDescription()
{
	return(description);
}

// write object to a file (for saving objects list)
int SpellObject::WriteToFile(ofstream &fw)
{
	// write description string
	uint32_t desc_len = description.size()+1;
	fw.write((char*)&desc_len,sizeof(uint32_t));
	fw.write(description.c_str(),desc_len);

	// store tool class
	uint32_t tool_class = this->tool_class;
	fw.write((char*)&tool_class,sizeof(uint32_t));

	// store tool class item
	uint32_t tool_class_item = this->tool_group;
	fw.write((char*)&tool_class_item,sizeof(uint32_t));


	// write tiles count
	uint32_t tile_count = L1_sprites.size();
	fw.write((char*)&tile_count,sizeof(uint32_t));

	// for each tile:
	for(int k = 0;k < tile_count; k++)
	{	
		// write L1 sprite index
		uint32_t id1 = L1_sprites[k]->GetIndex();
		fw.write((char*)&id1,sizeof(uint32_t));

		// write L2 sprite index
		uint32_t id2 = (L2_sprites[k])?(L2_sprites[k]->GetIndex()):(-1);
		fw.write((char*)&id2,sizeof(uint32_t));

		// write flags
		uint8_t flag = flags[k];
		fw.write((char*)&flag,sizeof(uint8_t));

		// write tile relative position [x,y]
		uint32_t x_pos = sprite_pos[k].x;
		uint32_t y_pos = sprite_pos[k].y;
		fw.write((char*)&x_pos,sizeof(uint32_t));
		fw.write((char*)&y_pos,sizeof(uint32_t));
	}

	// --- write image glyph data
	if(!pic)
	{
		// render glyph if not done yet
		RenderObjectGlyph();
	}

	// write pic size
	uint32_t pic_x = surf_x;
	fw.write((char*)&pic_x,sizeof(uint32_t));
	uint32_t pic_y = surf_y;
	fw.write((char*)&pic_y,sizeof(uint32_t));

	// write palette
	fw.write((char*)pal,3*256);

	// write image data
	fw.write((char*)pic,surf_x*surf_y);

	return(0);
}

// create object to a file
SpellObject::SpellObject(ifstream& fr,vector<Sprite*> &sprite_list, uint8_t* palette)
{
	sprite_pos.clear();
	L1_sprites.clear();
	L2_sprites.clear();
	description = "";
	last_gamma = 0.0;
	if(palette)
		std::memcpy((void*)pal,(void*)palette,256*3);
	pic = NULL;
	surf_x = 0;
	surf_y = 0;
	tool_class = 0;
	tool_group = 0;

	char *temp;
	uint32_t len;

	// read description string	
	fr.read((char*)&len,sizeof(uint32_t));
	temp = new char[len];
	fr.read(temp,len);
	description = string(temp);
	delete[] temp;

	// load tools class id
	uint32_t tool_class;
	fr.read((char*)&tool_class,sizeof(uint32_t));
	this->tool_class = tool_class;

	// load tools class item id
	uint32_t tool_class_item;
	fr.read((char*)&tool_class_item,sizeof(uint32_t));
	this->tool_group = tool_class_item;
	
	// read tiles count
	uint32_t tile_count;
	fr.read((char*)&tile_count,sizeof(uint32_t));
	
	L1_sprites.reserve(tile_count);
	L2_sprites.reserve(tile_count);
	flags.reserve(tile_count);
	sprite_pos.reserve(tile_count);
	
	// for each tile:
	for(int k = 0;k < tile_count; k++)
	{
		// read L1 sprite index
		uint32_t id1;
		fr.read((char*)&id1,sizeof(uint32_t));
		if(id1 >= sprite_list.size())
			return;		
		L1_sprites.push_back(sprite_list[id1]);

		// read L2 sprite index
		uint32_t id2;
		fr.read((char*)&id2,sizeof(uint32_t));
		if(id2 == 0xFFFFFFFFu)
			L2_sprites.push_back(NULL);
		else if(id2 >= sprite_list.size())
			return;
		else
		L2_sprites.push_back(sprite_list[id2]);		

		// read flags
		uint32_t flag;
		fr.read((char*)&flag,sizeof(uint8_t));
		flags.push_back(flag);
		
		// read tile relative position [x,y]
		uint32_t x_pos;
		uint32_t y_pos;
		fr.read((char*)&x_pos,sizeof(uint32_t));
		fr.read((char*)&y_pos,sizeof(uint32_t));
		MapXY pxy(x_pos, y_pos);
		sprite_pos.push_back(pxy);
	}

	// read pic size
	uint32_t pic_x;
	fr.read((char*)&pic_x,sizeof(uint32_t));
	surf_x = pic_x;
	uint32_t pic_y;
	fr.read((char*)&pic_y,sizeof(uint32_t));
	surf_y = pic_y;

	// read palette
	fr.read((char*)pal,3*256);

	// read image data
	pic = new uint8_t[pic_x*pic_y];
	pic_end = &pic[pic_x*pic_y];
	fr.read((char*)pic,surf_x*surf_y);

}


// add object to list of object
int Terrain::AddObject(vector<MapXY> xy,vector<Sprite*> L1_list,vector<Sprite*> L2_list,vector<uint8_t> flag_list,uint8_t* palette,std::string desc)
{
	// create object
	SpellObject *obj = new SpellObject(xy,L1_list,L2_list,flag_list,(uint8_t*)pal,desc);

	// generate glyph
	//obj->GenerateImage(glyph_path);

	// add to list
	objects.push_back(obj);

	return(0);
}
// get objects count
int Terrain::GetObjectsCount()
{
	return(objects.size());
}
// get object from list
SpellObject* Terrain::GetObject(int id)
{
	if(id >= objects.size())
		return(NULL);
	return(objects[id]);
}
// tools class/group clasification
void SpellObject::SetToolClass(int id)
{
	tool_class = id;
}
void SpellObject::SetToolClassGroup(int id)
{
	tool_group = id;
}
int SpellObject::GetToolClass()
{
	return(tool_class);
}
int SpellObject::GetToolClassGroup()
{
	return(tool_group);
}













SpellToolsGroup::SpellToolsGroup(string& name,string& title)
{
	items.clear();
	this->name = name;
	this->title = title;
}
int SpellToolsGroup::AddItem(string item, int position)
{
	if (position < 0)
		items.push_back(item);
	else if (position >= 0 && position < items.size())
		items.insert(items.begin() + position, item);
	else
		return(1);
	return(0);
}
int SpellToolsGroup::RenameItem(string item, int position)
{
	if (position < 0 || position >= items.size())
		return(1);
	items[position] = item;
	return(0);
}
int SpellToolsGroup::RemoveItem(int position)
{
	if (position < 0 || position >= items.size())
		return(1);
	items.erase(items.begin() + position);
	return(0);
}
int SpellToolsGroup::MoveItem(int posa, int posb)
{
	if(posa < 0 || posa >= items.size() || posb < 0 || posb >= items.size())
		return(1);
	auto temp = items[posa];
	items[posa] = items[posb];
	items[posb] = temp;
	return(0);
}
int SpellToolsGroup::GetCount()
{
	return(items.size());
}
string SpellToolsGroup::GetItem(int id)
{
	if(id < items.size())
		return(items[id]);
	return("");
}
string &SpellToolsGroup::GetClassName()
{	
	return(name);
}
string& SpellToolsGroup::GetClassTitle()
{
	return(title);
}
void SpellToolsGroup::SetClassName(string name)
{
	this->name = name;
}
void SpellToolsGroup::SetClassTitle(string title)
{
	this->title = title;
}
int SpellToolsGroup::GetItemID(const char* item_name)
{
	for(int k = 0; k < items.size(); k++)
		if(items[k].compare(item_name) == 0)
			return(k);
	return(-1);
}





// get tool classes count
int Terrain::GetToolsCount()
{
	return(tools.size());
}
// add new tool
int Terrain::AddToolSet(string name, string title, int position)
{
	SpellToolsGroup *toolset = new SpellToolsGroup(name, title);
	
	if (position < 0)
		tools.push_back(toolset);
	else if (position >= 0 && position < tools.size())
		tools.insert(tools.begin() + position, toolset);
	else
		return(1);

	for (auto const& spr : sprites)
	{
		auto tid = spr->GetToolClass();
		if (tid > position)
			spr->SetToolClass(tid + 1);
	}

	return(0);
}
// remove tool
int Terrain::RemoveToolSet(int position)
{
	if (position < 0 || position >= tools.size())
		return(1);
	delete tools[position];
	tools.erase(tools.begin() + position);
	for (auto const& spr : sprites)
	{
		if (spr->GetToolClass() == position + 1)
			spr->SetToolClass(0);
		if (spr->GetToolClass() > position)
			spr->SetToolClass(spr->GetToolClass() - 1);
	}
	return(0);
}
// move tool from to position
int Terrain::MoveToolSet(int posa, int posb)
{
	if (posa < 0 || posa >= tools.size() || posb < 0 || posb >= tools.size())
		return(1);
	
	auto temp = tools[posa];
	tools[posa] = tools[posb];
	tools[posb] = temp;

	for (auto const& spr : sprites)
	{
		auto tid = spr->GetToolClass();
		if (tid == posa + 1)
			spr->SetToolClass(posb + 1);
		else if(tid == posb + 1)
			spr->SetToolClass(posa + 1);
	}

	return(0);
}
// get tools class
SpellToolsGroup* Terrain::GetToolSet(int id)
{
	if(id < tools.size())
		return(tools[id]);
	return(NULL);
}
SpellToolsGroup* Terrain::GetToolSet(string &name)
{
	for(auto const &id : tools)
		if(id->GetClassName().compare(name) == 0)
			return(id);
	return(NULL);
}
// get tool class id
int Terrain::GetToolSetID(string& name)
{
	return(GetToolSetID(name.c_str()));
}
int Terrain::GetToolSetID(const char *name)
{
	for(int k = 0; k < tools.size(); k++)
		if(tools[k]->GetClassName().compare(name) == 0)
			return(k);
	return(-1);
}
// render tool item glyph image (if available in sprite context)
wxBitmap* Terrain::RenderToolSetItemImage(int tool_id,int item_id,double gamma)
{
	// find sprite that matches the desired classes and is marked as tool item glyph
	for(auto const &sid : sprites)
	{
		if( sid->GetToolClass() == tool_id + 1 &&
			sid->GetToolClassGroup() == item_id + 1 &&
			(sid->GetGlyphFlags() & Sprite::IS_TOOL_ITEM_GLYPH))
		{
			// mathing sprite found: render
			return(sid->Render((uint8_t*)pal, gamma));
		}
	}
	// not found - render blank:	
	wxBitmap* bmp = new wxBitmap(48,48,32);
	bmp->UseAlpha(true);
	return(bmp);
}
