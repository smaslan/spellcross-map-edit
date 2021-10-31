//=============================================================================
// Unsorted Spellcross data handling routines.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#include "spellcross.h"
#include "fs_archive.h"
#include "fsu_archive.h"
#include "spell_units.h"

#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <regex>

#include <format>

using namespace std;


//=============================================================================
// class SpellDEF - DEF files parser
//=============================================================================
SpellDefCmd::SpellDefCmd(std::string command, std::string params, std::string comment)
{
	name = command;
	comment = comment;

	std::regex regexz(",");
	parameters = new vector<std::string>(std::sregex_token_iterator(params.begin(), params.end(), regexz, -1), std::sregex_token_iterator());
}
SpellDefCmd::~SpellDefCmd()
{	
	delete parameters;
}


// construct from file
SpellDEF::SpellDEF(const wchar_t* path)
{
	data = NULL;

	HANDLE fr = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (fr == INVALID_HANDLE_VALUE)
	{
		// opening failed
		throw std::runtime_error("DEF file not found!");
	}
	// get its size
	DWORD flen = GetFileSize(fr, NULL);
	// allocate memory for archive
	data = new char[flen];
	if (data == NULL)
	{
		// allocation failed
		CloseHandle(fr);
		throw std::runtime_error("DEF file memory allocation failed!");
	}
	// read data
	DWORD read;
	ReadFile(fr, (void*)data, flen, &read, NULL);
	if (flen != read)
	{
		// failed
		delete(data);
		CloseHandle(fr);
		throw std::runtime_error("DEF file reading failed!");
	}
	// closse file
	CloseHandle(fr);
}
// construct from data buffer
SpellDEF::SpellDEF(byte* data, int size)
{
	this->data = new char[size];
	if (this->data == NULL)
	{
		// allocation failed
		throw std::runtime_error("DEF file memory allocation failed!");
	}
	std::memcpy((void*)this->data, (void*)data, size);
}
// destroy
SpellDEF::~SpellDEF()
{
	if (data)
		delete[] data;
	data = NULL;
}

// returns vector of commands in given section
vector<SpellDefCmd*> SpellDEF::GetSection(std::string section)
{
	vector<SpellDefCmd*> cmd_list;
	
	std::string data = std::string(this->data);
	
	// get first occurence of key
	data = data.substr(data.find(section.c_str()));
	
	if(section.find("(") != string::npos)
		section = section.replace(section.find("("), 1, "\\(");
	if (section.find(")") != string::npos)
		section = section.replace(section.find(")"), 1, "\\)");
	
	// look for entire section
	regex secexp(".*" + section + " +\\{([^\\}]+)");
	smatch match;
	std::regex_search(data, match, secexp);

	if (match.size() < 2)
	{
		// failed
		return(cmd_list);
	}

	// found
	std::string sectxt = match[1];
		
	while (1)
	{
		regex cmdexp("(.*\\r*\\n*) *([a-zA-Z]+)\\(([^\\)]+)\\)\\r*?\\n*?");
		std::regex_search(sectxt, match, cmdexp);

		if (match.size() != 4)
		{
			return(cmd_list);
		}
		
		// parse command data
		SpellDefCmd* cmd = new SpellDefCmd(match[2], match[3], match[1]);		
		// store to list
		cmd_list.push_back(cmd);

		// continue
		sectxt = match.suffix();
	}

	return(cmd_list);

}






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
int Sprite::MaskHasTransp(byte *mask)
{
	return(mask[0] == 0xFF ||
		!((mask[1] == 0x00 && mask[2] == 0x00 && mask[3] == 0xFF) ||
		  (mask[1] == 0x00 && mask[2] == 0xFF && mask[3] == 0xFF) ||
		  (mask[1] == 0xFF && mask[2] == 0xFF && mask[3] == 0xFF)));
}


// decode sprite from raw FS archive data, return bytes consumed
// ###todo: check input memory overrun?
int Sprite::Decode(byte* src, char* name)
{
	// store data start
	byte* source_start = src;
	
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
	byte *buf = new byte[(x_size + sizeof(int) * 2) * 256];
	byte *pdata = buf;


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
	this->data = new byte[len];
	std::memcpy((void*)this->data, (void*)buf, len);
	delete[] buf;

	// store sprite name
	strcpy(this->name, name);

	// return bytes consumed from the source
	return(src - source_start);
}




// render sprite to target buffer, buffer is sprite origin, x_size is buffer width
void Sprite::Render(byte* buffer, byte* buf_end, int buf_x, int buf_y, int x_size)
{
	// ###todo: optimize for no transparency mode?

	// initial buffer write position
	uint8_t* dest = &buffer[buf_x + (buf_y + y_ofs)*x_size];
	if (dest < buffer)
		return;

	// sprite source data
	uint8_t* data = this->data;

	// render each line:
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
				*scan = *data;
			data++;
			scan++;
		}
		// move to next buffer line
		dest += x_size;
	}
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
	for (int k = 0; k < frames.size(); k++)
		delete frames[k];
	frames.clear();
}

// decode animation file from buffer
int AnimL1::Decode(byte* data, char* name)
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
	strcpy(this->name, name);

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
	for (int k = 0; k < frames.size(); k++)
		delete frames[k];
	frames.clear();
}

// decode animation file from buffer
int AnimPNM::Decode(byte* data, char* name)
{
	// get frames count
	int count = *data++;

	// list of frame offsets
	DWORD* list = (DWORD*)data;

	// frames limits
	int xmin = 256;
	int xmax = 0;
	int ymin = 256;
	int ymax = -256;
		
	// --- for each frame:
	for (int fid = 0; fid < count; fid++)
	{
		// frame data start
		byte* pnm = &data[*list++];

		// vertilcal offset
		int y_ofs = *(__int16*)pnm; pnm += 2;

		// minimum left offset of all lines
		int x_ofs = *(WORD*)pnm; pnm += 2;

		// maximum line len
		int x_size = *(WORD*)pnm; pnm += 2;

		// lines count
		int y_size = *pnm++;

		// maximum image data size
		int max_data = y_size * (2*sizeof(int) + x_size);

		// make local image buffer
		byte* buf = new byte[max_data];
		byte* pic = buf;

		// for each line:
		for (int y = 0; y < y_size; y++)
		{
			// scanline buffer
			byte line[256];
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
				*(DWORD*)pic = 0; pic += 4;
				*(DWORD*)pic = 0; pic += 4;
			}
			else
			{
				// not empty line
				// store line offset
				*(DWORD*)pic = line_offset;
				pic += 4;

				// store its len
				*(DWORD*)pic = chunk_ofs - line_offset;
				pic += 4;

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
		spr->data = new byte[pic - buf];
		std::memcpy((void*)spr->data, (void*)buf, pic - buf);
		
		// store name
		std::strcpy(spr->name, name);

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
	strcpy(this->name, name);

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
	for (int k = 0; k < sprites.size(); k++)
		delete sprites[k];
	// clear list of sprites
	sprites.clear();

	// destruct each sprite element
	for (int k = 0; k < anms.size(); k++)
		delete anms[k];
	// clear list of sprites
	anms.clear();
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
	strcpy(this->name, &temp[1]);
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
		byte* data;
		int size;
		
		// get file from archive
		fs->GetFile(i, &data, &size, &full_name);

		// local name copy
		char name[14];
		strcpy(name, full_name);
				
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

			if (strcmpi(ext, "DTA") == 0)
			{
				///////////////////
				///// Sprites /////
				///////////////////
				
				// add sprite list element
				Sprite* sprite = new Sprite();
				sprites.push_back(sprite);
				
				// try decode sprite data
				if (!sprites.back()->Decode(data, name))
				{
					delete fs;
					return(1);
				}
				
				//st->Caption = "Loading " + N_terr + ": " + IntToStr(fcnt) + " - " + AnsiString(name);
				fcnt++;
			}
			else if (strcmpi(ext, "ANM") == 0)
			{
				/////////////////////////
				///// ANM animation /////
				/////////////////////////

				// add animation to list
				AnimL1* anim = new AnimL1();
				anms.push_back(anim);

				// try decode animation data
				if (anms.back()->Decode(data, name))
				{
					delete fs;
					return(1);
				}

				//st->Caption = "Loading " + N_terr + ": " + IntToStr(fcnt) + " - " + AnsiString(name);
				fcnt++;
			}
			else if (strcmpi(ext, "PNM") == 0)
			{
				/////////////////////////
				///// PNM animation /////
				/////////////////////////

				// add animation to list
				AnimPNM* pnm = new AnimPNM();
				pnms.push_back(pnm);

				// try decode animation data
				if (pnms.back()->Decode(data, name))
				{
					delete fs;
					return(1);
				}

				//st->Caption = "Loading " + N_terr + ": " + IntToStr(fcnt) + " - " + AnsiString(name);
				fcnt++;
			}
			else if (strcmpi(ext, "PAL") == 0)
			{
				if (size != 256)
				{
					///////////////////
					///// Palette /////
					///////////////////

					if (strcmpi(full_name, "map.pal") == 0)
					{
						// "map.pal"
						std::memcpy((void*)&pal[0][0], data, 128*3);

						//st->Caption = "Loading " + N_terr + ": " + IntToStr(fcnt) + " - " + AnsiString(name);
						fcnt++;
					}
					else if (strcmpi(full_name, "cycle.pal") == 0)
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
Sprite* Terrain::GetSprite(char* name)
{
	for (int k = 0; k < this->sprites.size(); k++)
	{
		if (strcmpi(this->sprites[k]->name, name) == 0)
			return(this->sprites[k]);
	}
	return(NULL);
}

// get L1 animation (ANM) pointer by its name
AnimL1* Terrain::GetANM(char* name)
{
	for (int k = 0; k < this->anms.size(); k++)
	{
		if (strcmpi(this->anms[k]->name, name) == 0)
			return(this->anms[k]);
	}
	return(NULL);
}

// get L4 animation (PNM) pointer by its name
AnimPNM* Terrain::GetPNM(char* name)
{
	for (int k = 0; k < this->pnms.size(); k++)
	{
		if (strcmpi(this->pnms[k]->name, name) == 0)
			return(this->pnms[k]);
	}
	return(NULL);
}




//=============================================================================
// class SpellData
//=============================================================================

SpellData::SpellData(wstring &data_path)
{
	// load terrains
	vector<wchar_t*> terrain_list({L"\\T11.FS",
		                           L"\\PUST.FS",
		                           L"\\DEVAST.FS"});
	for (int k = 0; k < terrain_list.size(); k++)
	{		
		// terrain archive path
		wstring path = data_path + terrain_list[k];
						
		// load terrain
		Terrain* new_terrain = new Terrain;
		new_terrain->Load(path);

		// store to list
		terrain.push_back(new_terrain);		
	}


	// load FSU data
	wstring path = data_path + L"\\UNITS.FSU";
	units_fsu = new FSUarchive(path);


	// load COMMON.FS
	FSarchive *common;
	path = data_path + L"\\COMMON.FS";
	common = new FSarchive(path);
	byte* data;
	int size;
	
	// load UNITS.PAL palette chunk and merge with terrain palette chunk
	if (common->GetFile("UNITS.PAL", &data, &size))
	{
		delete common;
		throw runtime_error("UNITS.PAL not found in COMMON.FS!");
	}
	for (int k = 0; k < this->terrain.size(); k++)
		std::memcpy((void*)&this->terrain[k]->pal[128][0], (void*)data, 96 * 3);
	
	// load SYSTEM.PAL palette chunk and merge with terrain palette chunk
	if (common->GetFile("SYSTEM.PAL", &data, &size))
	{
		delete common;
		throw runtime_error("SYSTEM.PAL not found in COMMON.FS!");
	}
	for (int k = 0; k < this->terrain.size(); k++)
	{
		byte temp[10][3];
		std::memcpy((void*)temp, (void*)&this->terrain[k]->pal[240][0], 10 * 3);
		std::memcpy((void*)&this->terrain[k]->pal[224][0], (void*)data, 32 * 3);
		std::memcpy((void*)&this->terrain[k]->pal[240][0], (void*)temp, 10 * 3);
	}

	// load JEDNOTKY.DEF units definition file
	if (common->GetFile("JEDNOTKY.DEF", &data, &size))
	{
		delete common;
		throw runtime_error("JEDNOTKY.DEF not found in COMMON.FS!");
	}
	units = new SpellUnits(data, size, units_fsu);

	// close common.fs
	delete common;
	
}

SpellData::~SpellData()
{
	// destroy terrain data
	for (int k = 0; k < terrain.size(); k++)
		delete terrain[k];
	terrain.clear();
	// clear units list
	delete units;
}


// Load special land images (solid A-M, edge A-M, selection A-M)
int SpellData::LoadSpecialLand(wstring &spec_folder)
{

	// groups to load
	wchar_t* files[] = { L"\\solid.fs", L"\\edge.fs", L"\\grid.fs", L"\\select.fs" };
	Sprite* lists[] = { special.solid, special.edge, special.grid, special.select };
	
	// for each group:
	for (int g = 0; g < sizeof(files)/sizeof(wchar_t*); g++)
	{
		// try to load special archive
		wstring path = spec_folder + files[g];
		FSarchive *fs = new FSarchive(path);

		// for each slope:
		char name[6];
		strcpy(name, "_.BMP");
		for (char sid = 'A'; sid <= 'M'; sid++)
		{
			name[0] = sid;
			byte* pic;
			int size;
			if (fs->GetFile(name, &pic, &size))
			{
				// not found?
				delete[] fs;
				return(1);
			}

			// target sprite object
			Sprite* spr = &lists[g][sid - 'A'];

			// store some name (because of sprite type letter)
			std::memset(spr->name, '\0', sizeof(spr->name));
			spr->name[0] = '_';
			spr->name[1] = '_';
			spr->name[2] = sid;


			// --- very crude BMP decoder:
			// get width
			int x = *((DWORD*)&pic[0x12]);

			// get heigth
			int y = *((DWORD*)&pic[0x16]);

			// get graphics data start
			int fofs = *((DWORD*)&pic[0x0A]);

			// store x, y
			spr->x_size = x;
			spr->y_size = y;
			
			// vertical offset (derived from original sprites)
			if (sid == 'B' || sid == 'D' || sid == 'F' || sid == 'L' || sid == 'M')
				spr->y_ofs = -18;
			else if (sid == 'C')
				spr->y_ofs = -17;
			else
				spr->y_ofs = 0;

			// no transparencies
			spr->has_transp = 0;

			// allocate image buffer
			spr->data = new byte[(80 + sizeof(int) * 2) * 200];

			// recode bmp->Sprite
			int pp1 = 0;
			for (int j = 0; j < y; j++)
			{
				// get line
				int pp2 = 0;
				byte ss = 0;
				int oo = 0;
				byte line[80];
				for (int k = 0; k < x; k++)
				{
					if (pic[fofs + (y - 1) * x - j * x + k] == 0x07u && ss == 0)
						oo++;
					else
					{
						ss = 1;
						line[pp2++] = pic[fofs + (y - 1) * x - j * x + k];
					}
				}

				// check end transparency
				int m;
				for (m = pp2 - 1; m >= 0; m--)
				{
					if (line[m] != 0x07)
						break;
				}				

				// store offset
				std::memset((void*)&spr->data[pp1], 0x00, sizeof(int) * 2);
				spr->data[pp1] = oo;
				pp1 += sizeof(int);

				// store line len
				spr->data[pp1] = m + 1;
				pp1 += sizeof(int);

				// check for transparencies
				if (memchr(line, 0x07, m + 1) != NULL && !spr->has_transp)
					spr->has_transp = 1;

				// convert colors
				for (int n = 0; n < m + 1; n++)
				{
					ss = 0x00;
					switch (line[n])
					{
						case 0xFFu: ss = 0xFEu; break;
						case 0x00u: ss = 0xFFu; break;
						case 0xF9u: ss = 0xC1u; break;
					}
					line[n] = ss;
				}

				// store line data
				std::memcpy((void*)&spr->data[pp1], (void*)line, m + 1);
				pp1 += m + 1;				
			}

			// ###todo: convert this whole stuff to load alrady converted sprite data! 
			// ###todo: add destructor for these sprites somewhere?
		}

		// loose archive
		delete fs;
	}


	return(0);
}

// get terrain pointer by terrain name or return NULL
Terrain* SpellData::GetTerrain(char* name)
{
	for (int k = 0; k < this->terrain.size(); k++)
		if (strcmpi(this->terrain[k]->name, name) == 0)
			return(this->terrain[k]);
	return(NULL);
}
