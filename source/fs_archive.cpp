//=============================================================================
// class FSarchive - Loading Spellcross FS archive data to memory.
// Works only for *.FS files, but not *.FSU file!
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================

#include "fs_archive.h"
#include "other.h"
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// load entire archive file
FSarchive::FSarchive(wstring &path)
{
	names.clear();
	data.clear();
	sizes.clear();
	Append(path);
}

// load more data to FS archive object
void FSarchive::Append(wstring& path)
{
	// try to open FS archive
	ifstream fr(path,ios::in | ios::binary);
	if(!fr)
		throw runtime_error(string_format("Cannot open \"%s\"!",path));

	// get count of files in archive
	uint32_t count;
	fr.read((char*)&count,sizeof(uint32_t));

	for(int i = 0; i < count; i++)
	{
		//read file name
		char* name = new char[14];
		memset(name,'\0',14);
		fr.read(name,13);

		// get file offset
		uint32_t afofs;
		fr.read((char*)&afofs,sizeof(uint32_t));

		// get file len
		uint32_t aflen;
		fr.read((char*)&aflen,sizeof(uint32_t));

		// copy file data to new buffer
		uint8_t* file_data = new uint8_t[aflen];
		streampos next = fr.tellg();
		fr.seekg(afofs);
		fr.read((char*)file_data,aflen);
		fr.seekg(next);

		// store file name to list
		this->names.push_back(name);

		// store file data to list
		this->data.push_back(file_data);

		// store file size
		this->sizes.push_back(aflen);
	}

	// close file
	fr.close();
}


// clear archive data
FSarchive::~FSarchive()
{
	for (int k = 0; k < names.size(); k++)
		delete[] names[k];
	names.clear();
	
	for (int k = 0; k < data.size(); k++)
		delete[] data[k];
	data.clear();

	sizes.clear();
}

// return files count
int FSarchive::Count()
{
	return((int)data.size());
}

// get file by name
int FSarchive::GetFile(const char* name, uint8_t** data, int* size)
{
	for (int k = 0; k < names.size(); k++)
	{
		if (_strcmpi(names[k], name) == 0)
		{
			if(data)
				*data = this->data[k];
			if(size)
				*size = this->sizes[k];
			return(0);
		}
	}
	return(1);
}

// get file by name
string FSarchive::GetFile(const char* name)
{
	string text;
	uint8_t* data;
	int size;
	GetFile(name, &data, &size);
	text.resize(size);
	memcpy(&text[0],data,size);
	return(text);
}

// get file by order index (zero based)
int FSarchive::GetFile(int id, uint8_t** data, int* size, char **name)
{	
	if (id >= this->data.size())
		return(1);
	if (data)
		*data = this->data[id];
	if (size)
		*size = this->sizes[id];
	if (name)
		*name = this->names[id];
	return(0);
}

// get name of file index
const char* FSarchive::GetFileName(int id)
{
	if(id >= this->data.size())
		return("");
	return(names[id]);
}