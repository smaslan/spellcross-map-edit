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

	SpellPalette(SpellPalette &pal,std::vector<std::string> chunks={});
	SpellPalette(std::string name="empty");
	void Clear();
	int Insert(uint8_t *data,std::string name,int offset,int count);
	int Insert(std::vector<uint8_t> &data,std::string name="",int offset=0,int count=0);
	int Insert(std::wstring path,int offset=0,std::string used="");
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
	std::tuple<int,int> GetUserRange(int start=0);
	std::string GetUserRangeString(bool add_zero=true);

};
