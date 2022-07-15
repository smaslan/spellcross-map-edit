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
#include <filesystem>

using namespace std;

// load entire archive file
FSarchive::FSarchive(wstring &path, int options)
{
	m_lzw = NULL;
	m_fs_name = std::filesystem::path(path).filename().string();
	Append(path, options);
}

// load more data to FS archive object
void FSarchive::Append(wstring& path,int options)
{
	// try to open FS archive
	ifstream fr(path,ios::in | ios::binary);
	if(!fr)
		throw runtime_error(string_format("Cannot open \"%ls\"!",path.c_str()));

	// init LZW decoder?
	if(options & Options::DELZ_ALL && !m_lzw)
	{
		m_lzw = new LZWexpand(1000000);
		if(!m_lzw)
			throw runtime_error("Cannot create LZW decoder!");
	}

	// get count of files in archive
	uint32_t count;
	fr.read((char*)&count,sizeof(uint32_t));

	// rellocate file list
	m_files.reserve(m_files.size() + count);

	for(int i = 0; i < count; i++)
	{	
		// make file record
		FSfile *file = new FSfile();
		
		//read file name
		char name[14];
		std::memset(name,'\0',14);
		fr.read(name,13);
		file->name = name;
		
		// get file offset
		uint32_t afofs;
		fr.read((char*)&afofs,sizeof(uint32_t));

		// get file len
		uint32_t aflen;
		fr.read((char*)&aflen,sizeof(uint32_t));

		// copy file data to buffer		
		file->data.resize(aflen);
		streampos next = fr.tellg();
		fr.seekg(afofs);
		fr.read((char*)file->data.data(),aflen);
		fr.seekg(next);

		// optional deLZ of known types
		if(m_lzw)
		{
			std::string ext = std::filesystem::path(name).extension().string();
			if(options & Options::DELZ_LZ && ext.compare(".LZ") == 0 
				|| options & Options::DELZ_LZ0 && ext.compare(".LZ0") == 0)
			{				
				auto &data = m_lzw->Decode(&file->data[0],&file->data[0] + aflen);
				if(data.empty())
				{
					delete file;
					throw std::runtime_error(string_format("DeLZ of file \"%s\" failed!",name));
				}
				file->data = data;				
			}
		}

		// store file data to list
		m_files.push_back(file);
	}

	// close file
	fr.close();
}


// clear archive data
FSarchive::~FSarchive()
{
	for(auto & file : m_files)
		delete file;
	m_files.clear();

	if(m_lzw)
		delete m_lzw;
}

// return files count
int FSarchive::Count()
{
	return((int)m_files.size());
}

// return all files
std::vector<FSarchive::FSfile*> &FSarchive::GetFiles()
{
	return(m_files);
}

// get file by name
int FSarchive::GetFile(string &name,uint8_t** data,int* size)
{
	return(GetFile(name.c_str(), data, size));
}
int FSarchive::GetFile(const char* name, uint8_t** data, int* size)
{
	for(auto & file : m_files)
	{
		if (_strcmpi(file->name.c_str(), name) == 0)
		{
			if(data)
				*data = file->data.data();
			if(size)
				*size = file->data.size();
			return(0);
		}
	}
	return(1);
}

// get file by name
string FSarchive::GetFile(string &name)
{
	return(GetFile(name.c_str()));
}
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
int FSarchive::GetFile(int id, uint8_t** data, int* size, const char **name)
{	
	if (id >= m_files.size())
		return(1);
	if (data)
		*data = m_files[id]->data.data();
	if (size)
		*size = m_files[id]->data.size();
	if (name)
		*name = m_files[id]->name.c_str();
	return(0);
}

// get name of file index
const char* FSarchive::GetFileName(int id)
{
	if(id >= m_files.size())
		return("");
	return(m_files[id]->name.c_str());
}

// get name of FS archive
std::string FSarchive::GetFSname(bool with_extension)
{
	if(with_extension)
		return(m_fs_name);
	else
		return(std::filesystem::path(m_fs_name).replace_extension().string());
}