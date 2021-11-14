//=============================================================================
// Spellcross sprite data handling routines.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#include "sprites.h"
#include "fs_archive.h"
#include "fsu_archive.h"

#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>

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
}

// destroy sprite
Sprite::~Sprite()
{
	name[0] = '\0';
	if (data)
		delete[] data;
}

// check if sprite row quad mask has transparencies or it is just last quad
int Sprite::MaskHasTransp(uint8_t *mask)
{
	return(mask[0] == 0xFF ||
		!((mask[1] == 0x00 && mask[2] == 0x00 && mask[3] == 0xFF) ||
		  (mask[1] == 0x00 && mask[2] == 0xFF && mask[3] == 0xFF) ||
		  (mask[1] == 0xFF && mask[2] == 0xFF && mask[3] == 0xFF)));
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












//=============================================================================
// class Terrain
//=============================================================================

Terrain::Terrain()
{
	name[0] = '\0';	
	sprites.clear();
	anms.clear();
}

Terrain::~Terrain()
{
	name[0] = '\0';
	
	// destruct each sprite element
	for (unsigned k = 0; k < sprites.size(); k++)
		delete sprites[k];
	// clear list of sprites
	sprites.clear();

	// destruct each sprite element
	for (unsigned k = 0; k < anms.size(); k++)
		delete anms[k];
	// clear list of sprites
	anms.clear();

	// destruct each sprite element
	for(unsigned k = 0; k < pnms.size(); k++)
		delete pnms[k];
	// clear list of sprites
	pnms.clear();
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

