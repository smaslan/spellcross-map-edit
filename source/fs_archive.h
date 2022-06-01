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

using namespace std;

class FSarchive
{
	public:
		FSarchive(wstring &path);
		~FSarchive();
		void Append(wstring& path);
		int GetFile(const char* name, uint8_t** data, int* size);
		int GetFile(string &name,uint8_t** data,int* size);
		int GetFile(int id, uint8_t** data, int* size, char** name=NULL);		
		inline string GetFile(const char* name);
		string GetFile(string& name);
		int Count();
		const char *GetFileName(int id);


	private:
		vector<uint8_t*> data;
		vector<char*> names;
		vector<uint32_t> sizes;
};

