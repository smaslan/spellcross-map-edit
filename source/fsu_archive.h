//=============================================================================
// class FSUarchive - Loading Spellcross FSU units graphics resource archive.
// It loads all resources from FSU, decompresses LZW compressed sprites
// and stores the sprites to lists. IT can take some time to decode'em as there 
// are a lot of them (>20000).
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#ifndef fsu_archiveH
#define fsu_archiveH
//---------------------------------------------------------------------------
#include "LZ_spell.h"
#include <vector>
#include <fstream>

class FSU_sprite
{
public:
	char name[4];
	int y_size;
	int y_ofs;
	// used range of pixels
	int x_min;
	int x_max;
	// sprite data
	uint8_t* data;

	FSU_sprite();
	~FSU_sprite();
	void Render(uint8_t* buffer, uint8_t* buf_end, int buf_x_pos, int buf_y_pos, int buf_x_size, uint8_t* shadow_filter,uint8_t* filter=NULL);
};

class FSU_resource
{
public:
	// unit tag name
	char name[6];
	// unit data offset in FSU
	uint32_t offset;
	// total sprites count
	uint16_t tot_sprites;
	// sprite list
	std::vector<FSU_sprite*> list;

	// --- sorted sprites groups (standing, moving, animation, ...)
	struct {
		// slopes available (only for tanks, otherwise 1)
		int slopes;
		// azimuths available
		int azimuths;
		// standing unit (flesh ones) or tanks (no animations) [slope][azimuth]
		FSU_sprite** lists['M' - 'A' + 1];
	} stat;
	
	struct {
		// frames count
		int frames;
		// azimuths count
		int azimuths;
		// lists of sprites [azimuth][frame]
		FSU_sprite*** lists;
	} anim;	

	FSU_resource();
	~FSU_resource();
};


class FSUarchive{
	private:

		// LZW decoder
		//LZWexpand *delz;
		// graphics resource list
		std::vector<FSU_resource*> list;		
		
		int LoadResource(uint8_t* data, int rid, FSU_resource* res, LZWexpand* delz);
		int LoadResourceGroup(uint8_t* data, int first, int count, int step, std::vector<FSU_resource*> *list);

	public:
		int xmin;
		int xmax;
		int ymin;
		int ymax;

		/*int FillItemsList(TListBox *lb);
		int LoadUnit(int uid);
		int GetImageName(int fid,char **name);
		int DrawBmpImage(int fid,Graphics::TBitmap *bmp,int zoom,uchar bcid,uchar scid);
		int FSUarchive::DrawImage(int fid,TImage *img,int zoom,uchar bcid,uchar scid);*/

		FSUarchive(std::wstring &path);
		~FSUarchive();
		FSU_resource* GetResource(char* name);
};


#endif

