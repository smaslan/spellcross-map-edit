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
FSarchive::FSarchive(wstring path, int options)
{
	m_lzw = NULL;
	m_fs_name = std::filesystem::path(path).filename().string();
	Append(path, options);
}

// load more data to FS archive object
void FSarchive::Append(wstring path,int options)
{
	// try to open FS archive
	m_file_path = path;
	ifstream fr(path,ios::in | ios::binary);
	if(!fr)
		throw runtime_error(string_format("Cannot open \"%ls\"!",path.c_str()));

	// init LZW decoder?
	m_options = options;
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

		// file position and size on FS
		file->file_size = aflen;
		file->file_pos = afofs;

		if(options & FSarchive::Options::NO_LOAD)
		{
			// no load data mode - just store file params:
			file->is_loaded = false;
		}
		else
		{
			// load data mode:
			file->is_loaded = true;
			
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
		}

		// store file data to list
		m_files.push_back(file);
	}

	// close file
	fr.close();
}

// load file data for given file record if not loaded before
int FSarchive::LoadFile(FSfile *fsfile)
{
	if(fsfile->is_loaded)
		return(0);

	// try open FS archive
	ifstream fr(m_file_path,ios::in | ios::binary);
	if(!fr)
		return(1);

	// load file data to buffer
	fsfile->data.resize(fsfile->file_size);	
	fr.seekg(fsfile->file_pos);
	fr.read((char*)fsfile->data.data(),fsfile->file_size);
	fr.close();
	
	// optional deLZ of known types
	if(m_lzw)
	{
		std::string ext = std::filesystem::path(fsfile->name).extension().string();
		if(m_options & Options::DELZ_LZ && ext.compare(".LZ") == 0
			|| m_options & Options::DELZ_LZ0 && ext.compare(".LZ0") == 0)
		{
			auto& data = m_lzw->Decode(&fsfile->data[0],&fsfile->data[0] + fsfile->file_size);
			if(data.empty())
				return(1);
			fsfile->data = data;
		}
	}

	// mark file as loaded
	fsfile->is_loaded = true;

	return(0);
}

// free loaded file data (will be reloaded on next access)
int FSarchive::FreeFileData(FSfile* fsfile)
{
	if(!fsfile)
		return(1);
	fsfile->data.clear();
	fsfile->is_loaded = false;
	return(0);
}

// free loaded file data (will be reloaded on next access)
int FSarchive::FreeFileData(std::string name)
{
	return(FreeFileData(name.c_str()));
}
// free loaded file data (will be reloaded on next access)
int FSarchive::FreeFileData(const char *name)
{
	for(auto& file : m_files)
	{
		if(_strcmpi(file->name.c_str(),name) == 0)
		{
			FreeFileData(file);
			return(0);
		}
	}
	return(1);
}
// free loaded file data (will be reloaded on next access)
int FSarchive::FreeFileData(int id)
{
	if(id < 0 || id >= m_files.size())
		return(1);
	return(FreeFileData(m_files[id]));
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
			if(!file->is_loaded)
			{
				// load file data first
				if(LoadFile(file))
					return(1);
			}
			
			// file loaded - just return
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
	if(GetFile(name, &data, &size))
		return(text);
	text.resize(size);
	memcpy(&text[0],data,size);
	return(text);
}

// get file by order index (zero based)
int FSarchive::GetFile(int id, uint8_t** data, int* size, const char **name)
{	
	if (id >= m_files.size())
		return(1);
	if(LoadFile(m_files[id]))
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

// get wildcard filtered file names
std::vector<std::string> FSarchive::GetFileNames(std::string wild)
{
	std::vector<std::string> list;
	for(auto &file: m_files)
		if(wildcmp(wild.c_str(),file->name.c_str()))
			list.push_back(file->name);
	return(list);
}