//=============================================================================
// Spellcross video data handling routines.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2022, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================

#include "spell_video.h"
#include "other.h"

#include <fstream>
#include <filesystem>


SpellCAN::SpellCAN(std::wstring path)
{
	// try open file
	ifstream fr(path.c_str(),ios::in | ios::binary | ios::ate);
	if(!fr.is_open())
		throw runtime_error("Cannot open CAN file!");

	// read to local buffer and close
	streampos flen = fr.tellg();
	fr.seekg(0);
	vector<uint8_t> data;
	data.resize(flen);
	fr.read((char*)data.data(),flen);
	fr.close();

	if(Decode(data.data(), data.size()))
	{
		throw runtime_error("Decoding CAN video file failed!");
	}	
}

int SpellCAN::Decode(uint8_t* data,int len)
{
	uint8_t *p = data;
	uint8_t *pend = &data[len];
	
	// read frames count
	if(&p[4] > pend)
	{
		// fail
		return(1);
	}
	int frames_count = *(uint32_t*)p; p += sizeof(uint32_t);

	// some crazy stuff here...
	// looks like 68x 256-byte tables of god know what, possibly some palette indices???

	// read frame resolution
	p = &data[0x4408];
	if(&p[12] > pend)
	{
		// fail
		return(1);
	}
	x_size = *(uint32_t*)p; p += sizeof(uint32_t);
	y_size = *(uint32_t*)p; p += sizeof(uint32_t);	
	int something = *(uint32_t*)p; p += sizeof(uint32_t);

	// first audio block
		
	// audio frame size 1/15s of 11025Hz
	int audio_frames_size = 735;
	
	// audio frames count per block (first block is 44 frames)
	int audio_frames = 44;

	// read frames
	while(p < pend)
	{
		int audio_block_size = audio_frames_size*audio_frames;
		if(&p[audio_block_size] > pend)
		{
			// fail
			return(1);
		}
		
		// copy audio block
		int audio_pos = audio.size();
		audio.resize(audio_pos + audio_block_size);
		memcpy(&audio[audio_pos], p, audio_block_size);
		p += audio_block_size;
		
		// next audio block will be one frame only
		audio_frames = 1;

		// get frame
		if(&p[4] > pend)
		{
			// fail
			return(1);
		}
		int video_frame_size = *(uint32_t*)p; p += sizeof(uint32_t);
		if(&p[video_frame_size] > pend)
		{
			// fail
			return(1);
		}

		// store to frame buffer
		video.emplace_back();
		video.back().resize(video_frame_size);
		memcpy(video.back().data(), p,video_frame_size);
		p += video_frame_size;
	}

	return(0);
}

int SpellCAN::GetFramesCount()
{
	return(video.size());
}

int SpellCAN::SaveFrames(std::wstring folder_path)
{
	for(int fid = 0; fid < video.size(); fid++)
	{
		auto frame = &video[fid];

		// frame path
		wstring path = std::filesystem::path(folder_path) / std::filesystem::path(string_format("frame_%04d.raw",fid));

		// store frame file
		ofstream fw(path,ios::out | ios::binary);
		if(!fw.is_open())
			return(1);
		fw.write((char*)frame->data(),frame->size());
		fw.close();		
	}

	// store frame file
	wstring path = std::filesystem::path(folder_path) / std::filesystem::path("audio.raw");
	ofstream fw(path,ios::out | ios::binary);
	if(!fw.is_open())
		return(1);
	fw.write((char*)audio.data(),audio.size());
	fw.close();

	return(0);
}