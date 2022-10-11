#pragma once

#if USE_LIBVORBIS
#include "ogg\include\vorbis\vorbisfile.h"
#else
#include "ogg\minivorbis.h"
#endif

#include "..\file_ex.h"

// ======================================
// Base streaming interface
// ======================================
class AudioStream
{
public:
	AudioStream();
	virtual ~AudioStream() {}

	// opens a handle to the stream
	virtual int  open(const char* filename) { return 1; }
	// kills the stream processing
	virtual void close() {}
	// fills a buffer with audio samples
	virtual void fill(BYTE* dst, DWORD size) {}
	// fills a buffer with all audio samples the waveform has
	virtual void fill2(BYTE* dst) {}
	// return number of bytes required to hold the whole waveform
	virtual size_t bytes() { return 0; }
	//
	virtual void rewind() {}

	void setlooping(int loops) { this->loops = loops; }

	__inline DWORD Frequency() { return freq; }
	__inline DWORD Channels() { return chans; }
	__inline DWORD Blocks() { return ssize; }

protected:
	__inline size_t tosamples(size_t bytes) { return bytes / ssize / chans; }
	__inline size_t tobytes(size_t samples) { return samples * ssize * chans; }
	__inline size_t samples_to_time(size_t samples) { return samples / (freq); }

	DWORD loops : 1, looped : 1,
		padding : 30;
	size_t loop_start,
		loop_end;
	DWORD freq,	// frequency
		chans,	// audio channels
		ssize;	// sample size
};

// ======================================
// OGG streaming interface
// ======================================
class AudioStreamOgg : public AudioStream
{
public:
	AudioStreamOgg();

	virtual int  open(const char* filename);
	virtual void close();
	virtual void fill(BYTE* dst, DWORD size);
	virtual void fill2(BYTE* dst);
	virtual size_t bytes();
	virtual void rewind();

private:
	int get_comment_val(const char* name);
	void read_frames(BYTE* dst, size_t size);
	void read(BYTE* dst, size_t size);
	void seek(size_t pos);

	FileHandle fp;
	OggVorbis_File vorbis;
	int section;
	BYTE* buffer;
	size_t buffer_pos;
};
