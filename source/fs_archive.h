//=============================================================================
// class FSarchive - Loading Spellcross FS archive data to memory.
// Works only for *.FS files, but not *.FSU file!
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include "LZ_spell.h"

class FSarchive
{
	public:

		class Options
		{
		public:
			static constexpr int NONE = 0x00;
			static constexpr int DELZ_LZ = 0x01;
			static constexpr int DELZ_LZ0 = 0x02;
			static constexpr int DELZ_ALL = DELZ_LZ | DELZ_LZ0;
		};

		class FSfile
		{
		public:
			std::string name;
			std::vector<uint8_t> data;
		};		

		FSarchive(std::wstring &path, int options=Options::NONE);
		void Append(std::wstring& path,int options=Options::NONE);
		~FSarchive();		
		std::vector<FSfile*> &GetFiles();
		int GetFile(const char* name, uint8_t** data, int* size);
		int GetFile(std::string &name,uint8_t** data,int* size);
		int GetFile(int id, uint8_t** data, int* size, const char** name=NULL);		
		inline std::string GetFile(const char* name);
		std::string GetFile(std::string& name);
		int Count();
		const char *GetFileName(int id);
		std::string GetFSname(bool with_extension=true);

	private:			
		
		std::string m_fs_name;
		LZWexpand* m_lzw;
		std::vector<FSfile*> m_files;
};

