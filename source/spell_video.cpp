//=============================================================================
// Spellcross video data handling routines.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2022, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================

#include "spell_video.h"
#include "other.h"
#include "LZ_spell.h"
#include "riff_loader.h"

#include <fstream>
#include <filesystem>


SpellVideo::SpellVideo(std::wstring path)
{
	// try open file
	ifstream fr(path.c_str(),ios::in | ios::binary | ios::ate);
	if(!fr.is_open())
		throw runtime_error("Cannot open video file!");

	// read to local buffer and close
	streampos flen = fr.tellg();
	fr.seekg(0);
	vector<uint8_t> data;
	data.resize(flen);
	fr.read((char*)data.data(),flen);
	fr.close();

	if(std::filesystem::path(path).extension().compare(".CAN") == 0 && DecodeCAN(data.data(), data.size()))
	{
		throw runtime_error("Decoding CAN video file failed!");
	}	
	else if(std::filesystem::path(path).extension().compare(".DPK") == 0 && DecodeDPK(data.data(),data.size()))
	{
		throw runtime_error("Decoding DPK video file failed!");
	}
	else if(std::filesystem::path(path).extension().compare(".DP2") == 0)
	{		
		// try open complement wave file
		std::wstring wave_path = std::filesystem::path(path).replace_extension("").wstring();
		ifstream frw(wave_path.c_str(),ios::in | ios::binary | ios::ate);
		if(!frw.is_open())
			throw runtime_error("Cannot open complement wave file!");

		// read to local buffer and close
		streampos wave_flen = frw.tellg();
		frw.seekg(0);
		vector<uint8_t> wave_data;
		wave_data.resize(wave_flen);
		frw.read((char*)wave_data.data(),wave_flen);
		frw.close();

		// try decode
		if(DecodeDP2(data.data(), data.size(), wave_data.data(), wave_data.size()))
		{
			throw runtime_error("Decoding DP2 video file failed!");
		}
	}
}

int SpellVideo::DecodeCAN(uint8_t* data,int len)
{
	type = SpellVideoType::CAN;

	uint8_t *p = data;
	uint8_t *pend = &data[len];
	
	// frame rate (estimate from smallest audio chunk)
	fps = 15.0;
		
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

	// init audio stream data
	audio.samples = 0;
	audio.fs = 11025;
	audio.channels = 1;
	std::strcpy(audio.name,"stream");

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
		audio.data.resize(audio.samples + audio_block_size);
		for(int k = 0; k < audio_block_size; k++)
			audio.data[audio.samples++] = (int16_t)((((uint16_t)*p++)<<8) - (uint16_t)0x8000u);
		
		
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
		frames.emplace_back();
		frames.back().resize(video_frame_size);
		memcpy(frames.back().data(), p,video_frame_size);
		p += video_frame_size;
	}

	return(0);
}

int SpellVideo::DecodeDPK(uint8_t* data,int len)
{
	type = SpellVideoType::DPK;

	uint8_t* p = data;
	uint8_t* pend = &data[len];

	// frame rate (estimate from smallest audio chunk)
	fps = 15.0;

	// read frames count
	if(&p[4] > pend)
	{
		// fail
		return(1);
	}
	int frames_count = *(uint32_t*)p; p += sizeof(uint32_t);

	// read palette data
	if(&p[256*3] > pend)
	{
		// fail
		return(1);
	}
	std::memcpy(&pal[0][0], p, 256*3);
	p += 256*3;
	
	// read frame resolution
	if(&p[8] > pend)
	{
		// fail
		return(1);
	}
	x_size = *(uint32_t*)p; p += sizeof(uint32_t);
	y_size = *(uint32_t*)p; p += sizeof(uint32_t);

	// make temp frame buffer holding current frame data
	vector<uint8_t> frame(x_size*y_size, 0x00);

	// initialize LZW decoder
	LZWexpand delz(1000000);

	// first audio block
	int audio_frame_size = 2*735;
	int audio_frames_count = 23;
	
	// init audio stream data
	audio.samples = 0;
	audio.fs = 11025;
	audio.channels = 1;
	std::strcpy(audio.name, "stream");
	
	
	// read frames
	while(p < pend)
	{
		// read audio samples till end of block detected
		
		int audio_block_size = audio_frame_size*audio_frames_count;
		if(&p[audio_block_size] > pend)
		{
			// fail
			return(1);
		}		
		
		// copy audio block
		uint16_t *smpl = (uint16_t*)p;		
		audio.data.resize(audio.samples + audio_block_size/2);
		for(int k = 0; k < audio_block_size/2; k++)
			audio.data[audio.samples++] = (int16_t)(*smpl++ - (uint16_t)0x8000u);
		p += audio_block_size;
		
		// next audio block will be one frame only
		audio_frames_count = 1;
					
		// get video frame data size		
		if(&p[4] > pend)
		{
			// failed
			return(1);
		}
		int video_chunk_size = *(uint32_t*)p; p += sizeof(uint32_t);
		if(&p[video_chunk_size] > pend)
		{
			// fail
			return(1);
		}
		if(!video_chunk_size)
		{
			// duplicate last frame
			frames.emplace_back(frame);
			continue;
		}

		// try deLZ
		uint8_t *deltas = NULL;
		int deltas_size;
		delz.Decode(p, &p[video_chunk_size], &deltas, &deltas_size);
		if(!deltas || deltas_size < 20)
		{
			// failed
			return(1);
		}
		p += video_chunk_size;
		uint8_t *dptr = deltas;
		uint8_t* dend = &deltas[deltas_size];
		
		// modified area range
		int min_x = *(uint16_t*)dptr; dptr += sizeof(uint16_t);
		int max_x = *(uint16_t*)dptr; dptr += sizeof(uint16_t);
		int min_y = *(uint16_t*)dptr; dptr += sizeof(uint16_t);
		int max_y = *(uint16_t*)dptr; dptr += sizeof(uint16_t);

		// frame data pointer
		uint8_t *fptr = frame.data();
		uint8_t* fend = &fptr[frame.size()];

		while(true)
		{
			// get pixels gap
			if(&dptr[4] > dend)
			{
				// failed
				delete deltas;
				return(1);
			}
			uint32_t gap_size = *(uint32_t*)dptr; dptr += sizeof(uint32_t);			
			// end mark?
			if(gap_size == 0xFFFFFFFFu)
				break;
			
			// get chunk size
			if(&dptr[4] > dend)
			{
				// failed
				delete deltas;
				return(1);
			}
			int chunk_size = *(uint32_t*)dptr; dptr += sizeof(uint32_t);
			if(!chunk_size)
				continue;

			// copy data chunk to current frame
			fptr += gap_size;
			if(&fptr[chunk_size] > fend || &dptr[chunk_size] > dend)
			{
				// failed
				delete deltas;
				return(1);
			}
			std::memcpy(fptr, dptr, chunk_size);
			dptr += chunk_size;
			fptr += chunk_size;			
		}

		// store to frame buffer
		frames.emplace_back(frame);
		
		delete deltas;
	}

	return(0);
}

int SpellVideo::DecodeDP2(uint8_t* dp2,int dp2_len, uint8_t *riff, int riff_len)
{
	type = SpellVideoType::DP2;

	uint8_t* p = dp2;
	uint8_t* pend = &dp2[dp2_len];

	// frame rate (estimate from smallest audio chunk)
	fps = 15.0;

	// read frames count
	if(&p[4] > pend)
	{
		// fail
		return(1);
	}
	int frames_count = *(uint32_t*)p; p += sizeof(uint32_t);

	// read palette data
	if(&p[256*3] > pend)
	{
		// fail
		return(1);
	}
	std::memcpy(&pal[0][0],p,256*3);
	p += 256*3;

	// read frame resolution
	if(&p[8] > pend)
	{
		// fail
		return(1);
	}
	x_size = *(uint32_t*)p; p += sizeof(uint32_t);
	y_size = *(uint32_t*)p; p += sizeof(uint32_t);

	// make temp frame buffer holding current frame data
	vector<uint8_t> frame(x_size*y_size,0x00);

	// initialize LZW decoder
	LZWexpand delz(1000000);

	// read frames
	while(p < pend)
	{
		// read audio samples till end of block detected

		// get video frame data size		
		if(&p[4] > pend)
		{
			// failed
			return(1);
		}
		int video_chunk_size = *(uint32_t*)p; p += sizeof(uint32_t);
		if(&p[video_chunk_size] > pend)
		{
			// fail
			return(1);
		}
		if(!video_chunk_size)
		{
			// duplicate last frame
			frames.emplace_back(frame);
			continue;
		}

		// try deLZ
		uint8_t* deltas = NULL;
		int deltas_size;
		delz.Decode(p,&p[video_chunk_size],&deltas,&deltas_size);
		if(!deltas || deltas_size < 20)
		{
			// failed
			return(1);
		}
		p += video_chunk_size;
		uint8_t* dptr = deltas;
		uint8_t* dend = &deltas[deltas_size];

		// modified area range
		int min_x = *(uint16_t*)dptr; dptr += sizeof(uint16_t);
		int max_x = *(uint16_t*)dptr; dptr += sizeof(uint16_t);
		int min_y = *(uint16_t*)dptr; dptr += sizeof(uint16_t);
		int max_y = *(uint16_t*)dptr; dptr += sizeof(uint16_t);

		// frame data pointer
		uint8_t* fptr = frame.data();
		uint8_t* fend = &fptr[frame.size()];

		while(true)
		{
			// get pixels gap
			if(&dptr[4] > dend)
			{
				// failed
				delete deltas;
				return(1);
			}
			uint32_t gap_size = *(uint32_t*)dptr; dptr += sizeof(uint32_t);
			// end mark?
			if(gap_size == 0xFFFFFFFFu)
				break;

			// get chunk size
			if(&dptr[4] > dend)
			{
				// failed
				delete deltas;
				return(1);
			}
			int chunk_size = *(uint32_t*)dptr; dptr += sizeof(uint32_t);
			if(!chunk_size)
				continue;

			// copy data chunk to current frame
			fptr += gap_size;
			if(&fptr[chunk_size] > fend || &dptr[chunk_size] > dend)
			{
				// failed
				delete deltas;
				return(1);
			}
			std::memcpy(fptr,dptr,chunk_size);
			dptr += chunk_size;
			fptr += chunk_size;
		}

		// store to frame buffer
		frames.emplace_back(frame);

		delete deltas;
	}

	// load complementary audio data			
	try {
		// try parse RIFF WAVE
		RIFF riff_cont(riff,riff_len);
		
		// store PCM data
		std::strcpy(audio.name,"stream");
		audio.channels = riff_cont.channels;
		audio.fs = riff_cont.fs;
		audio.samples = riff_cont.samples;
		riff_cont.ConvertPCM(audio.data);
	}
	catch(const runtime_error& error) {
		return(1);
	}

	return(0);
}

std::tuple<int,int> SpellVideo::GetResolution()
{
	return std::tuple(x_size,y_size);
}

int SpellVideo::GetFramesCount()
{
	if(isDP2())
		return((int)(fps*audio.samples/audio.fs));
	return(frames.size());
}

uint8_t* SpellVideo::GetFrame(int id)
{
	if(id < 0)
		return(NULL);
	if(isDP2())
		id = mod(id, frames.size()); // loop frames for DP2 format
	else if(id >= frames.size())
		return(NULL);
	return(frames[id].data());
}

// get frame matching timestamp
int SpellVideo::GetFrameID(double time)
{
	if(frames.empty())
		return(-1);
	int id;
	if(isDP2())
		id = max((int)(time*fps),0);
	else
		id = min(max((int)(time*fps), 0), (int)frames.size()-1);	
	return(id);
}

uint8_t* SpellVideo::GetPalette()
{
	return(&pal[0][0]);
}

SpellSample* SpellVideo::GetSound()
{
	return(&audio);
}

int SpellVideo::SaveFrames(std::wstring folder_path)
{
	for(int fid = 0; fid < frames.size(); fid++)
	{
		auto frame = &frames[fid];

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
	fw.write((char*)audio.data.data(),audio.samples*sizeof(int16_t));
	fw.close();

	return(0);
}