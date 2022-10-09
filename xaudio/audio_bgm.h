#pragma once
#include "sndfmt.h"

#define XABGM_BUFFER_SIZE		4096

enum XABGM_Types
{
	XABGM_TYPE_NONE,
	XABGM_TYPE_WAV,
	XABGM_TYPE_OGG
};

enum XaBgm_Status
{
	XBGMST_NOTHING,
	XBGMST_PLAYING,
	XBGMST_STOPPED,
};

// simple callback override for signaling
class BGMCallback : public IXAudio2VoiceCallback
{
public:
	BGMCallback() :
		hEndEvent(CreateEventA(nullptr, FALSE, FALSE, nullptr))
	{
	}
	~BGMCallback()
	{
		CloseHandle(hEndEvent);
	}

	virtual void _stdcall OnStreamEnd() {}
	virtual void _stdcall OnVoiceProcessingPassEnd() {}
	virtual void _stdcall OnVoiceProcessingPassStart(UINT32 SamplesRequired) {}
	virtual void _stdcall OnBufferEnd(void* pBufferContext);
	virtual void _stdcall OnBufferStart(void* pBufferContext) {}
	virtual void _stdcall OnLoopEnd(void* pBufferContext) {}
	virtual void _stdcall OnVoiceError(void* pBufferContext, HRESULT Error) {}

	HANDLE hEndEvent;
};

class XAudioBGM
{
public:
	XAudioBGM();
	~XAudioBGM();

	void Release();

	void Open(IXAudio2* pXA, const char* filename);

	// playback
	void Start(int mode);
	void Stop();
	void Rewind();
	// attributes
	int GetVolume() { return volume; }
	void SetVolume(int vol);
	void SetPan(float pan);
	// status
	int IsPlaying() { return status == XBGMST_PLAYING ? 1 : 0; }
	// filling
	void Fill(bool start = true);
	// threading
	static DWORD WINAPI thread(LPVOID context);
	void Update();

	HANDLE hThread;
	BYTE buffer[2][XABGM_BUFFER_SIZE];
	// XAudio2 objects
	IXAudio2SourceVoice* voice;
	XAUDIO2_BUFFER xa_buf;
	BGMCallback cb;
	BYTE type, index,
		vol_change : 1,
		pan_change : 1;
	float mat[4], panning;
	int volume, chans;
	// stream parser
	AudioStream* str;
	int status;
	size_t sent;
};