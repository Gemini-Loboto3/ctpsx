//#include <stdafx.h>
#include <windows.h>
#include <vector>
#include <intrin.h>
#include <mmreg.h>

#define USE_LIBVORBIS		1

#if USE_LIBVORBIS
#pragma comment(lib, "ogg/lib/libogg_static.lib")
#pragma comment(lib, "ogg/lib/libvorbis_static.lib")
#pragma comment(lib, "ogg/lib/libvorbisfile_static.lib")
#else
#define OGG_IMPL
#define VORBIS_IMPL
#endif

#include "sndfmt.h"

#define memalign(x, y) ((x + (y - 1)) & ~(y - 1))

__inline void memset16(uint16_t* dst, uint16_t value, size_t size)
{
	for (size_t i = 0; i < size; i++) *dst++ = value;
}

//////////////////////////////////////////////////
// base
AudioStream::AudioStream() : loops(false),
	looped(0),
	loop_start(0),
	loop_end(0),
	freq(0),
	chans(0),
	ssize(2)
{}

//////////////////////////////////////////////////
// OGG
static size_t ogg_fread(void* dst, size_t size, size_t count, void* fp)
{
	auto f = (FileMemory*)fp;
	return f->Read(dst, size * count);
}

static int ogg_fseek(void* fp, ogg_int64_t pos, int mode)
{
	auto f = (FileMemory*)fp;
	switch (mode)
	{
	case SEEK_SET: f->Seek(pos, FileMemory::SeekSet); break;
	case SEEK_CUR: f->Seek(pos, FileMemory::SeekCur); break;
	case SEEK_END: f->Seek(pos, FileMemory::SeekEnd); break;
	}
	
	return 0;
}

static int ogg_fclose(void* fp)
{
	auto f = (FileMemory*)fp;
	f->Close();

	return 0;
}

static long ogg_ftell(void* fp)
{
	auto f = (FileMemory*)fp;
	return f->Tell();
}

static ov_callbacks OV_CALLBACKS_MEM =
{
	ogg_fread,
	ogg_fseek,
	ogg_fclose,
	ogg_ftell
};

AudioStreamOgg::AudioStreamOgg() :
	vorbis{ 0 },
	section(0),
	buffer(nullptr),
	buffer_pos(0)
{
}

int AudioStreamOgg::open(const char* filename)
{
	if (!fp.Open((void*)filename, 0))
		return 0;
	if (ov_open_callbacks(&fp, &vorbis, nullptr, 0, OV_CALLBACKS_MEM) != 0)
	{
		fp.Close();
		return 0;
	}

	auto seq_ptr = ov_info(&vorbis, -1);
	freq = seq_ptr->rate;
	chans = seq_ptr->channels;
	ssize = seq_ptr->channels * 16 / 8;
	// default looping values
	loops = 0;
	loop_start = 0;
	loop_end = tobytes((size_t)ov_pcm_total(&vorbis, 0));
	// allocate a buffer large enough for looping
	buffer = new BYTE[loop_end + 8192];
	buffer_pos = 0;
	section = 0;
	// looping stuff
	auto ret = get_comment_val("Loops");
	if (ret == 1)
	{
		loops = 1;
		loop_start = get_comment_val("LoopStart");
		loop_end = get_comment_val("LoopEnd");
	}

	return 1;
}

void AudioStreamOgg::close()
{
	if (buffer)
	{
		delete[] buffer;
		buffer = nullptr;
	}
	ov_clear(&vorbis);
}

size_t AudioStreamOgg::bytes()
{
	return loop_end;
}

void AudioStreamOgg::fill(BYTE* dst, DWORD size)
{
	// doesn't loop and already past waveform position
	if (!loops && looped)
	{
		// fill with silence
		memset(dst, 0, size);
		return;
	}

	auto cur_pos = buffer_pos;

//#if _DEBUG
//	auto cur_sec = samples_to_time(tosamples(cur_pos));
//	auto tot_sec = samples_to_time(tosamples(loop_end));
//	printf("\rOgg pos %02d:%02d/%02d:%02d (%d)\r", cur_sec / 60, cur_sec % 60, tot_sec / 60, tot_sec % 60, loops);
//#endif

	if (cur_pos + size >= loop_end)
	{
		int rest = loop_end - cur_pos;
		// read whatever is at the end
		read(dst, rest);
		// signal that we're looping from now on
		looped = 1;
		if (loops)
		{
			seek(loop_start);
			read(&dst[rest], size - rest);
		}
		else
		{
			// fill non looping with blanks
			memset(&dst[rest], 0, size - rest);
		}
	}
	else read(dst, size);
}

void AudioStreamOgg::fill2(BYTE* dst)
{
	fill(dst, loop_end);
}

int AudioStreamOgg::get_comment_val(const char* name)
{
	auto comment = vorbis_comment_query(vorbis.vc, name, 0);
	if (comment) return atoi(comment);

	return 0;
}

void AudioStreamOgg::read_frames(BYTE* dst, size_t size)
{
	size_t total_read = 0;
	// cache as much as necessary
	do
	{
		// decode a frame
		auto r = ov_read(&vorbis, (char*)&dst[total_read], size - total_read, false, 2, true, &section);
		switch (r)
		{
			// various read errors, these shouldn't even occur
		case OV_HOLE:
		case OV_EBADLINK:
		case OV_EINVAL:
			throw 0;
			// EOF, force leaving
		case 0:
			looped = 1;
			//memset(&dst[total_read], 0, size - total_read);
			return;
		}
		total_read += r;
	} while (total_read < size);
}

void AudioStreamOgg::read(BYTE* dst, size_t size)
{
	// read from ogg if we're not done looping
	if (looped == 0)
		read_frames(&buffer[buffer_pos], size);
	// cache the current frames
	memcpy(dst, &buffer[buffer_pos], size);
	buffer_pos += size;
}

void AudioStreamOgg::seek(size_t pos) { buffer_pos = pos; }

void AudioStreamOgg::rewind()
{
	seek(0);
}
