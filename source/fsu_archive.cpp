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
#include "fsu_archive.h"
#include "LZ_spell.h"

#include "cstdint"
#include <fstream>
#include <vector>
#include <stdexcept>

#include <thread>

using namespace std;

//---------------------------------------------------------------------------
// load FS archive
//---------------------------------------------------------------------------
FSUarchive::FSUarchive(std::wstring &path)
{
	// try open file
	ifstream fr(path, ios::in | ios::binary | ios::ate);
	
	// read to local buffer and close
	streampos flen = fr.tellg();
	fr.seekg(0);
	uint8_t *data = new uint8_t[(unsigned)flen];
	fr.read((char*)data, flen);	
	fr.close();

	// read total graphic resources count
	uint32_t units_count = *(uint32_t*)&data[4];
	
	
	// --- multithreaded load (was too slow):
	// max cores
	int cores = thread::hardware_concurrency();
	thread** threads = new thread*[cores];
	vector<FSU_resource*> *lists = new vector<FSU_resource*>[cores];

	// load/decode:
	for (int k = 0; k < cores; k++)
	{
		// try to load batch
		threads[k] = new thread(&FSUarchive::LoadResourceGroup, this, data, k, units_count, cores, &lists[k]);
	}
	// wait for threads
	for (int k = 0; k < cores; k++)
	{
		// try to load batch
		threads[k]->join();
		delete threads[k];
	}
	delete[] threads;
	// merge results
	for (int k = 0; k < cores; k++)
	{
		for (unsigned rid = 0; rid < (lists[k]).size(); rid++)
		{			
			list.push_back(lists[k][rid]);
		}
		lists[k].clear();		
	}
	delete[] lists;

	// clear FSU file buffer
	delete[] data;			
}

//---------------------------------------------------------------------------
// Close FS archive
//---------------------------------------------------------------------------
FSUarchive::~FSUarchive()
{
	for (unsigned k = 0; k < list.size(); k++)
		delete list[k];
	list.clear();
}

//---------------------------------------------------------------------------
// Thread that loads multiple resources
//---------------------------------------------------------------------------
int FSUarchive::LoadResourceGroup(uint8_t* data, int first, int count, int step, std::vector<FSU_resource*> *list)
{
	// read total graphic resources count
	uint32_t max_count = *(uint32_t*)&data[4];

	// initialize LZW decoder
	// note: this object should not be destroyed for each sprite otherwise ti will be slow as hell!
	LZWexpand* delz = new LZWexpand(100000);

	for (unsigned k = first; k < max_count; k += step)
	{
		// make new unit record
		FSU_resource* res = new FSU_resource();

		// try decode
		LoadResource(data, k, res, delz);

		// add to list
		list->push_back(res);

		if (list->size() >= (unsigned)count)
			break;
	}

	// clear LZW decoder
	delete delz;

	return(0);
}

//---------------------------------------------------------------------------
// Load item content into list
//---------------------------------------------------------------------------
int FSUarchive::LoadResource(uint8_t *data, int rid, FSU_resource *res, LZWexpand* delz)
{
	int i,j,k;

	uint8_t* head = &data[8 + rid * (5 + 4)];

	// get unit name
	memset((void*)res->name, '\0', sizeof(res->name));
	for (int c = 0; c < 5; c++)
	{
		char sym = *head++;
		
		if (sym != '_') /* loose suffix symbols '_' */
			res->name[c] = sym;
	}

	// get unit sprite list offset in FSU data
	res->offset = *(uint32_t*)head;

	// go to unit sprites list
	uint8_t* plist = &data[res->offset];

	// read total sprites count
	res->tot_sprites = *(uint16_t*)plist; plist += sizeof(uint16_t);
			
	// for each sprite
	xmin  =65536;
	xmax = 0;
	ymin = 65536;
	ymax = 0;
	for (i = 0; i < res->tot_sprites; i++)
	{
		// make new sprite
		FSU_sprite* sprite = new FSU_sprite();

		// get sprite name
		std::memset((void*)sprite->name, '\0', sizeof(sprite->name));
		memcpy((void*)sprite->name, (void*)plist, 3);
		plist += 3;
		
		// get sprite data offset in FSU
		uint32_t start = *(uint32_t*)plist; plist += sizeof(uint32_t);

		// get sprite data len
		uint16_t len = *(uint16_t*)plist; plist += sizeof(uint16_t);

		// load compressed sprite data
		uint8_t* src = &data[start];
		// sprite data end
		uint8_t* srce = src + len;
			

		// try to decode sprite archive
		int slen = 0;
		uint8_t* spr;
		int err = delz->Decode(src, srce, &spr, &slen);
		if (err)
		{
			// something has fucked while decompressing
			delete sprite;
			delete delz;
			return(1);
		}

		// sprite data local pointer
		uint8_t* sd = spr;
		
		// === decode sprite ===
		// get sprite heigth
		sprite->y_size = *(uint16_t*)sd; sd += sizeof(uint16_t);

		// get sprite offset
		sprite->y_ofs = (int)*(int16_t*)sd; sd += sizeof(int16_t);
		if(sprite->y_ofs < ymin)
			ymin = sprite->y_ofs;
		
		// sprite data end
		uint8_t *se = spr + slen;

		// alloc decoded image data (maximum possible size)
		uint8_t *img_data = new uint8_t[slen*2];
		uint8_t *img = img_data;

		// for each line
		int lxmax = 0;
		int lxmin = 256;
		for (j = 0; j < sprite->y_size; j++)
		{
			// check for EOF (don't know why, but some are shorter that they should be)
			if(sd >= (se-4))
			{
				*(int*)img = 0; img += sizeof(int);
				*(int*)img = 0; img += sizeof(int);
				continue;
			}

			// get line offset
			int ofs = (int)*(int16_t*)sd; sd += sizeof(int16_t);
			*(int*)img = ofs; img += sizeof(int);
			
			// reserve line line item
			uint8_t* llen = img; img += sizeof(int);

			// get full chunks count
			int ful = *sd++;

			// get partial chunks count
			int par = *sd++;

			// detect minimal offset
			if((ful || par) && ofs<lxmin)
				lxmin = ofs;

			// read full parts
			std::memcpy((void*)img, (void*)sd, ful*4);
			img += ful*4;
			sd += ful*4;
			uint8_t *md = sd+4;

			// read partial chunks
			uint8_t *imge = img;
			for (k = 0; k < par; k++)
			{
				int m;
				for (m = 0; m < 4; m++)
				{
					if(md[m])
						*img++ = 0x00;
					else
					{
						*img++ = sd[m];
						imge = img;
					}
				}
				sd += 8;
				md += 8;
			}
			img = imge;

			// check real line len
			int xlen = (int)(imge - (llen + sizeof(int)));

			// store real line len
			*(int*)llen = xlen;

			// detect maximal line len
			if(ofs + xlen>lxmax)
				lxmax = ofs + xlen;
		}

		// loose LZ decoder output buffer
		delete[] spr;

		// global ymax
		if(sprite->y_ofs + sprite->y_size > ymax)
			ymax = sprite->y_ofs + sprite->y_size;

		// store x limits
		sprite->x_min = lxmin;
		sprite->x_max = lxmax;

		// global xmin
		if(lxmin < xmin)
			xmin = lxmin;

		// global xmax
		if(lxmax > xmax)
			xmax = lxmax;

		// store image
		int img_data_len = (int)(img - img_data);
		sprite->data = new uint8_t[img_data_len];
		std::memcpy((void*)sprite->data, (void*)img_data, img_data_len);

		// loose temp image buffer
		delete[] img_data;		

		// store new sprite
		res->list.push_back(sprite);
	}

	/*if(strcmp(res->name,"SRANG") == 0)
		i++;*/
	
	
	// ====== analyze sprite names ======
	for (i = 0; i < res->tot_sprites; i++)
		if(res->list[i]->name[0] == 'F')
			break;
	if(i != res->tot_sprites)
	{
		// === two separate groups: movement+statical ===

		// sprites in the static group (name starts with F__) - azimuths
		res->stat.azimuths = res->tot_sprites - i;
		// single slope only
		res->stat.slopes = 1;
		// make list of standing sprites, one for each azimuth		
		res->stat.lists[0] = new FSU_sprite*[res->stat.azimuths];
		memcpy((void*)res->stat.lists[0], (void*)&res->list[i], res->stat.azimuths * sizeof(FSU_sprite*));
		

		// --- movement group ---
		if(i > 0)
		{
			// count azimuths
			char ffc = res->list[0]->name[1];
			for (j = 0; j < i; j++)
				if(res->list[j]->name[1] != ffc)
					break;
			
			// azimuths count
			res->anim.azimuths = j;
			// frames count
			res->anim.frames = i / j;
			// make list of azimuths
			res->anim.lists = new FSU_sprite ** [res->anim.azimuths];
			// for each azimuth:
			for (int azid = 0; azid < res->anim.azimuths; azid++)
			{
				res->anim.lists[azid] = new FSU_sprite * [res->anim.frames];
				for (int k=0; k < res->anim.frames; k++)
					res->anim.lists[azid][k] = res->list[azid + k*res->anim.azimuths];
			}
		}
		else
		{
			// no movement images			
			res->anim.azimuths = 0;
			res->anim.frames = 0;
			res->anim.lists = NULL;
		}
	}
	else
	{
		// === single image group ===
		
		/*int azims[13];
		memset(azims,0,13*sizeof(int));
		for(i = 0; i < res->tot_sprites; i++)
			if(res->list[0]->name[1] != '0')*/		
		for (i = 0; i < res->tot_sprites; i++)
			if(res->list[i]->name[1] != '0')
				break;
		if(i != res->tot_sprites)
		{
			// --- it's animation ---
		
			// count azimuths
			char ffc = res->list[0]->name[1];
			for (j = 0; j < i; j++)
				if(res->list[j]->name[1] != ffc)
					break;

			// azimuths count
			res->anim.azimuths = j;
			// frames count
			res->anim.frames = res->tot_sprites / j;
			// make list of azimuths
			res->anim.lists = new FSU_sprite * *[res->anim.azimuths];
			// for each azimuth:
			for (int azid = 0; azid < res->anim.azimuths; azid++)
			{
				res->anim.lists[azid] = new FSU_sprite * [res->anim.frames];
				for (int k = 0; k < res->anim.frames; k++)
					res->anim.lists[azid][k] = res->list[azid + k*res->anim.azimuths];
			}

		}
		else
		{
			// it's mechanical unit (has multiple terrain slope variants)

			// count azimuths
			char ffc = res->list[0]->name[0];
			for(j=0;j<i;j++)
				if(res->list[j]->name[0] != ffc)
					break;

			// sprites in the static group (name starts with F__) - azimuths
			res->stat.azimuths = j;
			// single slope only
			res->stat.slopes = res->tot_sprites / j;
			// make list of standing sprites, one for each azimuth		
			for (int sid = 0; sid < res->stat.slopes; sid++)
			{
				res->stat.lists[sid] = new FSU_sprite * [res->stat.azimuths];
				memcpy((void*)res->stat.lists[sid], (void*)&res->list[sid* res->stat.azimuths], res->stat.azimuths*sizeof(FSU_sprite*));
			}
		}
	}
	
	return(0);
}

// get graphic resource by name
FSU_resource *FSUarchive::GetResource(char* name)
{
	for (unsigned k = 0; k < list.size(); k++)
		if (_strcmpi(list[k]->name, name) == 0)
			return(list[k]);
	return(NULL);
}


// single resource constructor
FSU_resource::FSU_resource()
{
	// clearing only pointe rlists, but not the sprites themselves!
	for (int k = 0; k < sizeof(stat.lists) / sizeof(FSU_sprite*); k++)
		stat.lists[k] = NULL;
	stat.azimuths = 0;
	stat.slopes = 0;
	anim.azimuths = 0;
	anim.frames = 0;	
	anim.lists = NULL;
}
// single resource cleaner
FSU_resource::~FSU_resource()
{
	// clear static unit lists
	for (int k = 0; k < sizeof(stat.lists) / sizeof(FSU_sprite*); k++)
		if(stat.lists[k])
			delete[] stat.lists[k];	
	stat.azimuths = 0;
	stat.slopes = 0;
	
	// clear unit animation lists
	if (anim.lists)
	{
		for (int k = 0; k < anim.azimuths; k++)
			if (anim.lists[k])
				delete[] anim.lists[k];
		delete[] anim.lists;
	}
	anim.azimuths = 0;
	anim.frames = 0;

	// clear actual sprite data
	for(unsigned k = 0; k < list.size(); k++)
		delete list[k];
	list.clear();
}

// get azimuth index for fiven angle
int FSU_resource::GetAnimAzim(double angle)
{
	angle = remainder(angle, 360.0);
	return ((int)round((360.0 + 90.0 - angle)/360.0*(double)anim.azimuths)) % anim.azimuths;	
}
// get azimuth index for fiven angle
int FSU_resource::GetStaticAzim(double angle)
{
	angle = remainder(angle,360.0);
	return ((int)round((360.0 + 90.0 - angle)/360.0*(double)stat.azimuths)) % stat.azimuths;
}



// FSU sprite constructor
FSU_sprite::FSU_sprite()
{
	name[0] = '\0';
	data = NULL;
}
// FSU sprite desctructor
FSU_sprite::~FSU_sprite()
{
	if(data)
		delete[] data;
}

// render sprite to target buffer, buffer is sprite origin, x_size is buffer width
void FSU_sprite::Render(uint8_t* buffer, uint8_t* buf_end, int buf_x_pos, int buf_y_pos, int buf_x_size,uint8_t* shadow_filter,uint8_t* filter)
{
	// source data
	uint8_t* data = this->data;

	// initial xy-position
	uint8_t *dest = &buffer[buf_x_pos + (buf_y_pos + y_ofs - 128) * buf_x_size];
	// check valid write range
	if (dest < buffer)
		return;
		
	for (int y = 0; y < y_size; y++)
	{
		// line x-offset
		int offset = *(int*)data; data += sizeof(int);
		// pixels count
		int count = *(int*)data; data += sizeof(int);

		// initial pixel in destination buffer
		uint8_t* scan = &dest[offset];
		// check safe range
		if (scan + count > buf_end)
			break;
		// render line
		if(filter)
		{
			for(int x = 0; x < count; x++)
			{
				if(*data == 0xFD && shadow_filter)
				{
					// shadow it is - convert original color using filter
					*scan = filter[shadow_filter[*scan]];
				}
				else if(*data)
				{
					// normal visible pixel
					*scan = filter[*data];
				}
				data++;
				scan++;
			}
		}
		else
		{
			for (int x = 0; x < count; x++)
			{
				if (*data == 0xFD && shadow_filter)
				{
					// shadow it is - convert original color using filter
					*scan = shadow_filter[*scan];
				}
				else if (*data)
				{
					// normal visible pixel
					*scan = *data;
				}
				data++;
				scan++;
			}
		}
		// move to next buffer line
		dest += buf_x_size;
	}
}


//---------------------------------------------------------------------------
// get image name
//---------------------------------------------------------------------------
/*int FSUarchive::GetImageName(int fid,char **name)
{
	if(!ulst || fid>=imgs || !name)
		return(1);

	// return poitner
	*name=ulst[fid].name;

	return(0);
}*/

//---------------------------------------------------------------------------
// Draw item image bitmap
//---------------------------------------------------------------------------
/*int FSUarchive::DrawBmpImage(int fid,Graphics::TBitmap *bmp,int zoom,uchar bcid,uchar scid)
{
	int i,j,k;


	if(!ulst || fid>=imgs || !bmp)
		return(1);

	// sprite to draw
	TUSpr *spr=&ulst[fid];

	// sprite data
	uchar *sd=sprd+spr->dpos;

	// vertical offset
	int vofs=spr->yofs-ymin;

	// set bmp size if not defined
	if(!bmp->Width)
		bmp->Width=(xmax-xmin)*(zoom+1);
	if(!bmp->Height)
		bmp->Height=(ymax-ymin)*(zoom+1);
	int xs=bmp->Width;
	int ys=bmp->Height;

	// clear bitmap
	for(i=0;i<ys;i++)
		memset((void*)bmp->ScanLine[i],bcid,xs);

	// draw to dest bitmap
	for(i=vofs;i<vofs+spr->ys;i++)
	{
		if(i*(zoom+1)>=ys)
			break;

		// line pointer
		uchar *sp=(uchar*)bmp->ScanLine[i*(zoom+1)+0];
		uchar *spe=sp+xs;

		// get line offset
		uchar ofs=*sd++;
		ofs-=xmin;
		sp+=(unsigned int)ofs*(zoom+1);

		// get line len
		uchar len=*sd++;

		// write line
		for(j=0;j<len;j++)
		{
			register uchar pix=*sd++;
			if(sp<spe)
			{
				if(pix==0xFD)
					*sp=scid;
				else if(pix!=0xFE)
					*sp=pix;
				if(zoom)
					sp[1]=*sp++;
				sp++;
			}
		}

		if(zoom && i*(zoom+1)+1<ys)
			memcpy((void*)bmp->ScanLine[i*(zoom+1)+1],(void*)bmp->ScanLine[i*(zoom+1)+0],xs);
	}

	return(0);
}

//---------------------------------------------------------------------------
// Draw item image into TImage
//---------------------------------------------------------------------------
int FSUarchive::DrawImage(int fid,TImage *img,int zoom,uchar bcid,uchar scid)
{
	Graphics::TBitmap *bmp=img->Picture->Bitmap;

	// set size to image
	bmp->Width=img->Width;
	bmp->Height=img->Height;

	// draw into it's bitmap
	return(DrawBmpImage(fid,bmp,zoom,bcid,scid));
}*/
