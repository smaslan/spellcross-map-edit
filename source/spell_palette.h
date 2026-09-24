//=============================================================================
// Spellcross color palette related stuff.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2026, Stanislav Maslan, s.maslan@seznam.cz
// url: https://github.com/smaslan/spellcross-map-edit
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#pragma once

#include "cstdint"
#include <vector>
#include <string>
#include <filesystem>
#include <type_traits>

#include "graphics.h"

class wxBitmap;

// palette record
class SpellPalette
{
private:
	void SortChunks();

public:
	std::string m_name;
	std::vector<uint8_t> m_pal;
	std::vector<uint8_t> m_used;
	std::vector<uint8_t> m_used_user;

	class Chunk{
	public:
		std::string name;
		int offset;
		int size;
		bool isWithin(uint8_t pixel);
		bool isWithin(std::vector<uint8_t> &pixel);
	};
	std::vector<Chunk> m_chunks;

	// in-place gamma correction for count rgb tripplets
	template <typename TRGB>
	static void Gamma(double gamma,TRGB *rgb,int count=1)
	{
		if constexpr(std::is_same_v<TRGB,ImgQuantize::Pixel>)
		{
			auto col = static_cast<ImgQuantize::Pixel*>(rgb);
			for(int k = 0; k < count; k++)
			{
				col[k].r = (pow((double)col[k].r/255.0, 1.0/gamma)*255.0);
				col[k].g = (pow((double)col[k].g/255.0, 1.0/gamma)*255.0);
				col[k].b = (pow((double)col[k].b/255.0, 1.0/gamma)*255.0);
			}
		}
		else
		{
			for(int k = 0; k < 3*count; k++)
				rgb[k] = (pow((double)rgb[k] / 255.0,1.0/gamma)*255.0);
		}
	}
	
	// in-place color saturation correction count rgb tripplets
	template <typename TRGB>
	static void Saturation(double sat,TRGB *rgb,int count=1)
	{
		if constexpr(std::is_same_v<TRGB,ImgQuantize::Pixel>)
		{
			auto col = static_cast<ImgQuantize::Pixel*>(rgb);
			for(int k = 0; k < count; k++)
			{
				double luma = 0.2126*col[k].r + 0.7152*col[k].g + 0.0722*col[k].b;
				col[k].r = (std::clamp(luma + sat*(col[k].r - luma),0.0,255.0));
				col[k].g = (std::clamp(luma + sat*(col[k].g - luma),0.0,255.0));
				col[k].b = (std::clamp(luma + sat*(col[k].b - luma),0.0,255.0));
			}
		}
		else
		{
			for(int k = 0; k < count; k++)
			{
				double luma = 0.2126*rgb[3*k + 0] + 0.7152*rgb[3*k + 1] + 0.0722*rgb[3*k + 2];
				for(int c = 0; c < 3; c++)
					rgb[3*k + c] = (std::clamp(luma + sat*(rgb[3*k + c] - luma),0.0,255.0));
			}
		}
	}

	SpellPalette(SpellPalette &pal,std::vector<std::string> chunks={});
	SpellPalette(std::string name="empty");
	void Clear();
	int Insert(uint8_t *data,std::string name,int offset,int count);
	int Insert(std::vector<uint8_t> &data,std::string name="",int offset=0,int count=0);
	int Insert(std::filesystem::path path,int offset=0,std::string used="");
	int InsertBlank(std::string name,int offset,int size,std::string color_str_filter="");
	int AssignColors(std::vector<ImgQuantize::Pixel> &colors, std::vector<uint8_t> mask={});
	int GetUsedCount();
	std::tuple<int,int> GetRange(int start=0);
	std::string GetRangeString(bool add_zero=true,std::vector<std::string> chunk_names={});
	int Save(std::filesystem::path path);
	int SaveChunks(std::filesystem::path directory_path);
	int LoadInfo(std::filesystem::path path);
	int SaveInfo(std::filesystem::path path,std::vector<std::string> chunks={});
	int Render(wxBitmap& bmp);
	int RenderPaletteColor(wxBitmap& bmp,int x_size,int x_pos,uint8_t* filter=NULL);
	uint8_t (*GetPal())[3];
	Chunk *GetChunk(std::string name);
	std::vector<std::string> GetChunkNames();
	std::vector<std::string> GetChunkNames(std::vector<uint8_t> &pixels);
	bool CheckPixels(std::vector<uint8_t>& pixels, std::vector<std::string> chunk_names={});
	
	// to mark used color by user (not used for exporting)
	int ClearUserRange();
	int AddUserRange(std::string chunk_name);
	int AddUserRange(std::vector<std::string> &chunk_names);
	int AddUserRange(std::vector<uint8_t> &pixels);
	int AddUserRangeStr(std::string range_string);
	std::tuple<int,int> GetUserRange(int start=0);
	std::string GetUserRangeString(bool add_zero=true);
	bool isInUserRange(std::string chunk_name);

	static std::vector<uint8_t> ParseRangeStr(std::string range_string,int count=256);
};
