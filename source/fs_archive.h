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

class FSarchive
{
	public:
		FSarchive(std::wstring &path);
		~FSarchive();
		void Append(std::wstring& path);
		int GetFile(const char* name, uint8_t** data, int* size);
		int GetFile(std::string &name,uint8_t** data,int* size);
		int GetFile(int id, uint8_t** data, int* size, char** name=NULL);		
		inline std::string GetFile(const char* name);
		std::string GetFile(std::string& name);
		int Count();
		const char *GetFileName(int id);
		std::string GetFSname(bool with_extension=true);

	private:
		std::vector<uint8_t*> data;
		std::vector<char*> names;
		std::vector<uint32_t> sizes;
		std::string fs_name;
};

