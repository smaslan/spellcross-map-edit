//=============================================================================
// class LZ_spell - LZW decoder for Spellcross. It is almost standard LZW, 
// but there are few changes. It is quite nasty code, but seems to work.
// It keeps dictionary buffer and local output data buffer allocated so
// it can be reused for many files, which is essential for decoding FSU 
// archive with 20000+ sprites.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#include "LZ_spell.h"

#include <cstdint>
#include <stdexcept>
//---------------------------------------------------------------------------

// get u8 from stream
uint8_t gu8(uint8_t **p)
{
	uint8_t b = *((uint8_t*)(*p));
	(*p)++;
	return(b);
}

// get u16 from stream
uint16_t gu16(uint8_t**p)
{
	uint16_t w = *((uint16_t*)(*p));
	*p = (uint8_t*)(((uint16_t*)*p) + 1);
	return(w);
}

//---------------------------------------------------------------------------
// LZ dictionary - constructor
//---------------------------------------------------------------------------
LZWdct::LZWdct()
{
	data_used = 0;
	idict_len = 0;
	cdict_len = 0;
	data = new uint8_t[DMLEN * DMLEN];
}

int LZWdct::Load(uint8_t **src, uint8_t *end)
{
	int i;

	// check parameters
	if(!src || !end)
	{
		return(1);
	}
	
	// --- initial dictionary ---

	// dictionary len
	int len = gu16(src);

	// item len
	ibw = gu8(src);

	// get initial dictionary
	for (i = 0; i < len; i++)
	{
		data[i] = (len & 0x100) ? i : gu8(src);
		idict[i].len = 1;
		idict[i].ofs = i;		
	}
	idict_len = len;
	data_used = len;

	// set current dictionary
	LZWreinitDict();

	return(0);
}

//---------------------------------------------------------------------------
// copy initial dictionary to current one
//---------------------------------------------------------------------------
int LZWdct::LZWreinitDict()
{
	// copy initial to current dictionary
	std::memcpy((void*)cdict, (void*)idict, idict_len*sizeof(LZWdictElem));
	cdict_len = idict_len;
	data_used = idict_len;

	// set current bitstream width
	cbw = ibw;

	return(0);
}

//---------------------------------------------------------------------------
// LZ dictionary destructor
//---------------------------------------------------------------------------
LZWdct::~LZWdct(void)
{
	data_used = 0;
	idict_len = 0;
	cdict_len = 0;
	delete[] data;
}

//---------------------------------------------------------------------------
// LZ dictionary - add item
// Adds combination of item A with first byte from item B
//---------------------------------------------------------------------------
int LZWdct::ins(LZWdictElem *ita, LZWdictElem *itb)
{
	if(ita==NULL || itb==NULL)
		return(1);

	// copy A
	cdict[cdict_len].len = ita->len + 1;
	cdict[cdict_len].ofs = data_used;	
	std::memcpy((void*)&data[data_used], (void*)&data[ita->ofs], ita->len);
	data_used += ita->len;
	// copy first symbol of B
	data[data_used++] = data[itb->ofs];
	// update current dict size
	cdict_len++;

	// update stream width
	if (cdict_len + 1 >= (1 << cbw))
		cbw++;

	return(0);
}

//---------------------------------------------------------------------------
// LZ dictionary - is item clear code?
//---------------------------------------------------------------------------
int inline LZWdct::isclr(int code)
{
	// ###todo: this should be maybe derived from current dict instead of initial???
	return(code == idict_len);
}


//---------------------------------------------------------------------------
// LZ dictionary - get dictionary item or empty if error
//---------------------------------------------------------------------------
LZWdictElem *LZWdct::get(int code)
{
	if (code >= cdict_len)
	{
		// failed
		return(NULL);
	}
	
	// return poitner to data of requested item
	cdict[code].data = &data[cdict[code].ofs];
	return(&cdict[code]);	
}




//---------------------------------------------------------------------------
// LZW decoder
//---------------------------------------------------------------------------

// construct with initial output buffer size
LZWexpand::LZWexpand(int buf_size)
{
	// allocate initial output buffer (this will exist until class destroyed and grow whenever needed)
	this->buffer = new uint8_t[buf_size];
	this->buf_size = buf_size;
	this->buf_use = 0;
}

// close LZW decoder
LZWexpand::~LZWexpand()
{
	// loose local output buffer
	if (buffer)
		delete[] buffer;
	buf_size = 0;
	buf_use = 0;
}

//---------------------------------------------------------------------------
// LZ - get word form bitstream
// returns -1 if data end reached
//---------------------------------------------------------------------------
int LZWexpand::GetWord(uint8_t** dsrc, uint8_t* dend)
{
	const uint32_t mskl[12] = { 0x80000000u,0xC0000000u,0xE0000000u,0xF0000000u,
							    0xF8000000u,0xFC000000u,0xFE000000u,0xFF000000u,
							    0xFF800000u,0xFFC00000u,0xFFE00000u,0xFFF00000u };

	// reset bitstream
	if (!dsrc)
	{
		bofs = 7;
		return(0);
	}

	// get word from stream
	uint32_t msk = mskl[dct.cbw - 1] >> (7 - bofs);
	union {
		uint32_t u32;
		uint8_t u8[4];
	}dwrd;
	uint8_t* wdat = (uint8_t*)(*dsrc);
	// bswap	
	dwrd.u8[3] = *wdat++;
	dwrd.u8[2] = *wdat++;
	dwrd.u8[1] = *wdat++;
	dwrd.u8[0] = *wdat++;
	// mask and align to LSB
	dwrd.u32 = (dwrd.u32 & msk) >> (25 + bofs - dct.cbw);

	//new offset
	bofs -= dct.cbw;
	while (bofs < 0)
	{
		if (*dsrc >= dend - 1)
			return(-1);
		bofs += 8;
		(*dsrc)++;
	}

	return(dwrd.u32);
}


// decode LZW data
int LZWexpand::Decode(uint8_t*dsrc, uint8_t *dend,uint8_t **dest,int *dlen)
{
	LZWdictElem *cwrd = NULL;
	LZWdictElem pwrd;
	int ccd;

	// missing some parameters?
	if(!dsrc || !dend || !dest || !dlen)
		return(1);

	// invalid parameter values?
	if(dsrc>=dend)
		return(1);

	// no output data yet
	*dlen = 0;
	*dest = NULL;

	// reset local output buffer
	buf_use = 0;

	// load dictionary (clears the old one)
	dct.Load(&dsrc, dend);

	// init bitstream
	GetWord(NULL, NULL);
		
	// and start decoding
	pwrd.len = 0;
	while(dsrc < dend)
	{
		// get item from stream
		ccd = GetWord(&dsrc,dend);
		if(ccd < 0)
		{
			// finished
			break;
		}

		// clear dictionary?
		if(dct.isclr(ccd))
		{
			// yeah, clear dictionary

			// store previous chunk into local memory buffer first (because it
			// can't be lost with dictionary)
			if(pwrd.len > CMLEN)
			{
				// too long - leave (this should never happen but if so ...)
				return(3);
			}
			if(pwrd.len != 0)
				std::memcpy((void*)temp_chunk,(void*)pwrd.data,pwrd.len);
			pwrd.data = temp_chunk;

			// now clear dictionary
			dct.LZWreinitDict();

			// and cotinue
			continue;
		}

		// update dictionary?
		if(ccd < dct.cdict_len)
		{
			// yeah - append chunk

			// get new item
			cwrd = dct.get(ccd);

			// combine it with previous item and add into dictionary
			if(dct.ins(&pwrd,cwrd))
			{
				// inserting failed
				return(5);
			}
		}
		else if(ccd == dct.cdict_len)
		{
			// yeah - duplicate chunk

			// duplicate previous chunk and add into dictionary
			if(dct.ins(&pwrd,&pwrd))
			{
				// inserting failed
				return(5);
			}

			// get new item from dictionary
			cwrd = dct.get(ccd);
		}
		else
		{
			// failed - invalid dictionary entry			
			return(4);
		}

		// write chunk to local output buffer:
		if (buf_use + cwrd->len > buf_size)
		{
			// not enough memory, expand buffer
			int new_buf_size = buf_size + 100000;
			uint8_t* new_buf = new uint8_t[new_buf_size];			
			std::memcpy((void*)new_buf, (void*)buffer, buf_size);
			delete[] buffer;
			buffer = new_buf;
			buf_size = new_buf_size;			
		}
		// copy data to local buffer
		std::memcpy((void*)&buffer[buf_use], (void*)cwrd->data, cwrd->len);
		buf_use += cwrd->len;

		// current word to previous word
		pwrd = *cwrd;
	}

	// make and copy local data to output buffer
	*dlen = buf_use;
	*dest = new uint8_t[buf_use];
	std::memcpy((void*)*dest, (void*)buffer, buf_use);

	return(0);
}

