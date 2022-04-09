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
#include "sprites.h"
#include "LZ_spell.h"
#include "other.h"

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
			for(int k = 0; k < ful*4; k++)
			{
				if(sd[k])
					img[k] = sd[k];
				else
					img[k] = 254; // swap 0 to other black because of transparencies
			}			
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
						if(sd[m])
							*img++ = sd[m];
						else
							*img++ = 254; // swap 0 to other black because of transparencies
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
		// make default fire azimuths
		res->stat.fire_origin[0].assign(res->stat.azimuths,{0,0});
		

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
			// slopes count
			res->anim.slopes = 0;
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
			// make default fire azimuths
			res->anim.fire_origin.assign(res->anim.azimuths,{0,0});
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
		
		for (i = 0; i < res->tot_sprites; i++)
			if(res->list[i]->name[1] != '0')
				break;

		if(strcmp(res->name,"DPEKJ") == 0)
		{
			// --- animation with single azimuth, but more slopes (hell cavallery)

			// azimuths count
			res->anim.azimuths = 0;
			// slopes count
			res->anim.slopes = 13;
			// frames count
			res->anim.frames = res->tot_sprites / 13;
			// make list of azimuths
			res->anim.lists = new FSU_sprite * *[res->anim.slopes];
			// for each azimuth:
			for(int azid = 0; azid < res->anim.slopes; azid++)
			{
				res->anim.lists[azid] = new FSU_sprite *[res->anim.frames];
				for(int k = 0; k < res->anim.frames; k++)
					res->anim.lists[azid][k] = res->list[k + azid*res->anim.frames];
			}
			// make default fire azimuths
			res->anim.fire_origin.assign(res->anim.slopes,{0,0});
		}
		else if(i != res->tot_sprites)
		{
			// --- it's animation ---
		
			// count azimuths
			char ffc = res->list[0]->name[1];
			for (j = 0; j < i; j++)
				if(res->list[j]->name[1] != ffc)
					break;

			// azimuths count
			res->anim.azimuths = j;
			// slopes count
			res->anim.slopes = 0;
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
			// make default fire azimuths
			res->anim.fire_origin.assign(res->anim.azimuths,{0,0});			
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
				// make default fire azimuths
				res->stat.fire_origin[sid].assign(res->stat.azimuths,{0,0});
			}
		}
	}
	
	return(0);
}

// get graphic resource by name
FSU_resource *FSUarchive::GetResource(const char* name)
{
	for (unsigned k = 0; k < list.size(); k++)
		if (_strcmpi(list[k]->name, name) == 0)
			return(list[k]);
	return(NULL);
}

// get count of resources
int FSUarchive::GetCount()
{
	return(list.size());
}

// save auxiliary data asociated to FSU archive
int FSUarchive::SaveAuxData(std::wstring path)
{
	// create file
	ofstream fw(path,ios::out | ios::binary);
	if(!fw.is_open())
		return(1);

	// store last path
	aux_data_path = path;

	// store version string
	const char ver[] = "SpellFSUcontextV1.0";
	fw.write(ver,sizeof(ver));

	// store resource count
	ostream_write_i32(fw,GetCount());

	// store list of resource names
	for(auto & res : list)
		ostream_write_string(fw,res->name);
	
	// store resouce data for each:
	for(auto& res : list)
	{
		// static data:
		auto &stat = res->stat;
		ostream_write_i32(fw,stat.slopes);
		for(int sid = 0; sid < stat.slopes; sid++)
		{
			auto &forg = stat.fire_origin[sid];
			ostream_write_i32(fw,forg.size());
			for(auto & aorg : forg)
			{
				ostream_write_i32(fw,aorg.x);
				ostream_write_i32(fw,aorg.y);
			}
			ostream_write_i32(fw,stat.fire_center[sid].x);
			ostream_write_i32(fw,stat.fire_center[sid].y);
		}

		// animation data:
		auto& anim = res->anim;
		ostream_write_i32(fw,anim.fire_origin.size());
		for(auto& aorg : anim.fire_origin)
		{
			ostream_write_i32(fw,aorg.x);
			ostream_write_i32(fw,aorg.y);
		}
		ostream_write_i32(fw,anim.fire_center.x);
		ostream_write_i32(fw,anim.fire_center.y);
	}

	// close file
	fw.close();

	return(0);
}

// save auxiliary data asociated to FSU archive
int FSUarchive::LoadAuxData(std::wstring path)
{
	// create file
	ifstream fr(path,ios::in | ios::binary);
	if(!fr.is_open())
		return(1);

	// store last path
	aux_data_path = path;

	// check version
	const char ver_ref[] = "SpellFSUcontextV1.0";
	char ver[sizeof(ver_ref)];
	fr.read(ver,sizeof(ver_ref));
	if(memcmp(ver,ver_ref,sizeof(ver_ref)))
	{
		fr.close();
		return(1);
	}

	// get aux resouce count
	uint32_t count = istream_read_u32(fr);

	// load list of resources
	vector<string> names;
	for(int uid = 0; uid < count; uid++)
	{
		// resource name
		string name = istream_read_string(fr);
		names.push_back(name);
	}

	// load aux data for each unit graphic resource
	for(auto & rname : names)
	{
		// try to find matching resource
		auto *res = GetResource(rname.c_str());

		// load statics
		int stat_slopes = istream_read_i32(fr);
		for(int sid = 0; sid < stat_slopes; sid++)
		{
			int stat_azims = istream_read_i32(fr);			
			for(int aid = 0; aid < stat_azims; aid++)
			{
				int x = istream_read_i32(fr);
				int y = istream_read_i32(fr);
				if(res && sid < res->stat.slopes && aid < res->stat.azimuths)
					res->stat.fire_origin[sid][aid] = FSU_resource::Txy(x,y);				
			}
			int x = istream_read_i32(fr);
			int y = istream_read_i32(fr);
			if(res && sid < res->stat.slopes)
				res->stat.fire_center[sid] = FSU_resource::Txy(x,y);
		}

		// load animations
		uint32_t anim_azims = istream_read_i32(fr);
		for(int aid = 0; aid < anim_azims; aid++)
		{
			int x = istream_read_i32(fr);
			int y = istream_read_i32(fr);
			if(res && aid < res->anim.azimuths)
				res->anim.fire_origin[aid] = FSU_resource::Txy(x,y);
		}		
		int x = istream_read_i32(fr);
		int y = istream_read_i32(fr);
		if(res)
			res->anim.fire_center = FSU_resource::Txy(x,y);
	}

	// close file
	fr.close();

	return(0);
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
		for(int k = 0; k < anim.slopes; k++)
			if(anim.lists[k])
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
	if(!anim.azimuths)
		return(0);
	int azim = round((90.0 - angle)/360.0*(double)anim.azimuths);
	while(azim < 0)
		azim += anim.azimuths;
	azim = azim % anim.azimuths;
	return(azim);
}
// get azimuth angle (deg for fiven azimuth index
double FSU_resource::GetAnimAngle(int azim)
{
	if(azim < 0 || azim >= anim.azimuths)
		return(0.0);
	double angle = 90.0 - (double)azim*360.0/(double)anim.azimuths;
	return(angle);
}
// get azimuth index for fiven angle
int FSU_resource::GetStaticAzim(double angle)
{
	if(!stat.azimuths)
		return(0);
	int azim = round((90.0 - angle)/360.0*(double)stat.azimuths);
	while(azim < 0)
		azim += stat.azimuths;
	azim = azim % stat.azimuths;
	return(azim);
}
// get azimuth angle (deg for fiven azimuth index
double FSU_resource::GetStaticAngle(int azim)
{
	if(azim < 0 || azim >= stat.azimuths)
		return(0.0);	
	double angle = 90.0 - (double)azim*360.0/(double)stat.azimuths;
	return(angle);
}

// get azimuth index for fiven angle using fire angles
int FSU_resource::GetStaticFireAzim(int slope,double angle)
{
	if(stat.slopes == 1)
		slope = 0;
	auto &origins = stat.fire_origin[slope];
	double angle_min = 1e9;
	int azimuth_id = 0;
	auto xy0 = GetStatFireOriginMean(slope);
	for(int aid = 0; aid < stat.azimuths; aid++)
	{
		auto &org = origins[aid];
		double dx = org.x - xy0.x;
		double dy = org.y - xy0.y;
		double da = abs(atan2(-dy,dx)*180.0/M_PI - angle);
		if(da < angle_min)
		{
			angle_min = da;
			azimuth_id = aid;
		}
	}
	return(azimuth_id);
}
// get azimuth index for fiven angle using fire angles
int FSU_resource::GetAnimFireAzim(double angle)
{
	auto& origins = anim.fire_origin;
	double angle_min = 1e9;
	int azimuth_id = 0;
	auto xy0 = GetAnimFireOriginMean();
	for(int aid = 0; aid < anim.azimuths; aid++)
	{
		auto& org = origins[aid];
		double dx = org.x - xy0.x;
		double dy = org.y - xy0.y;
		double da = abs(atan2(-dy,dx)*180.0/M_PI - angle);
		if(da < angle_min)
		{
			angle_min = da;
			azimuth_id = aid;
		}
	}
	return(azimuth_id);
}

// get mean origin of firing
FSU_resource::Txy FSU_resource::GetStatFireOriginMean(int slope)
{
	if(slope >= stat.slopes)
		return(Txy(0,0));
	int x = 0;
	int y = 0;
	for(auto & org : stat.fire_origin[slope])
	{
		x += org.x;
		y += org.y;
	}
	x = x/stat.azimuths;
	y = y/stat.azimuths;		
	return(Txy(x,y));
}

// get mean origin of firing
FSU_resource::Txy FSU_resource::GetAnimFireOriginMean()
{
	int x = 0;
	int y = 0;
	int count = 0;
	for(auto& org : anim.fire_origin)
	{
		x += org.x;
		y += org.y;
		count++;
	}
	x = x/count;
	y = y/count;
	return(Txy(x,y));
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
void FSU_sprite::Render(uint8_t* buffer, uint8_t* buf_end, int buf_x_pos, int buf_y_pos, int buf_x_size,uint8_t* shadow_filter,uint8_t* filter,int zoom)
{
	// source data
	uint8_t* data = this->data;

	// initial xy-position
	uint8_t *dest = &buffer[buf_x_pos + (buf_y_pos + (y_ofs - 128)*zoom) * buf_x_size];
	// check valid write range
	if (dest < buffer)
		return;
		
	for (int y = 0; y < y_size; y++)
	{
		// line x-offset
		int offset = *(int*)data; data += sizeof(int);
		// pixels count
		int count = *(int*)data; data += sizeof(int);

		uint8_t *line_data = data;
		for(int z = 0; z < zoom; z++)
		{
			// init line data position
			data = line_data;
			// initial pixel in destination buffer
			uint8_t* scan = &dest[offset*zoom];
			// check safe range
			if (scan + count*zoom > buf_end)
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
					for(int zx = 0; zx < zoom; zx++)
					{
						if(*data == 0xFD && shadow_filter)
						{
							// shadow it is - convert original color using filter
							*scan = shadow_filter[*scan];
						}
						else if (*data)
						{
							// normal visible pixel
							*scan = *data;
						}
						scan++;
					}
					data++;
				}
			}
			// move to next buffer line
			dest += buf_x_size;
		}		
	}
}

