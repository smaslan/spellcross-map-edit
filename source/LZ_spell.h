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
#ifndef LZ_spellH
#define LZ_spellH
//---------------------------------------------------------------------------
#include <cstdint>
#include <vector>

// max dictionary len
#define DMLEN 4096
// max chunk len
#define CMLEN 4096


class LZWdictElem
{
public:
	int len;
	int ofs;
	uint8_t *data;
};

class LZWdct
{
	// initial dictionary
	LZWdictElem idict[DMLEN];
	int idict_len;
	// initial stream bitwidth
	int ibw;

	// maximum dictionary data size
	uint8_t *data;
	int data_used;
	
	public:
		LZWdct();
		~LZWdct();
		int Load(uint8_t** src, uint8_t* end);		
		int LZWreinitDict();

		int inline isclr(int code);

		int ins(LZWdictElem* ita, LZWdictElem* itb);
		LZWdictElem *get(int code);

		// current dictionary
		LZWdictElem cdict[DMLEN];
		int cdict_len;
		// current stream bitwidth
		int cbw;
};

class LZWexpand
{
private:
	// local output buffer
	std::vector<uint8_t> buffer;
	int output_size;

	// dictionary
	LZWdct dct;

	// bit stream bit offset
	int bofs;

	// static stuff to safe time
	uint8_t temp_chunk[CMLEN];

	int GetWord(uint8_t** dsrc, uint8_t* dend);

	int DecodeCore(uint8_t* dsrc,uint8_t* dend);

public:

	LZWexpand(int buf_size);
	~LZWexpand();
	
	int Decode(uint8_t* dsrc, uint8_t* dend, uint8_t** dest, int* dlen);
	std::vector<uint8_t> &Decode(uint8_t* dsrc,uint8_t* dend);

};




#endif
