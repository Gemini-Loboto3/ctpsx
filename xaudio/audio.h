#pragma once
#include "xaudio/include/xaudio2redist.h"
#include <vector>
#include "audio_bgm.h"

/////////////////////////////////
enum XSampleType
{
	XST_DOOR,
	XST_ARMS,
	XST_ROOM,
	XST_ENEMY,
	XST_CORE,
	XST_BGM,
	XST_SBGM,
	XST_XA,
	XST_COUNT
};

class XAudio;
class XSample;
class XStream;
class VoiceCallback;
class XVoice;
class XVoicePool;

bool XAudio2_IsPresent();
float XAudioConvertVolume(int volume);
void XAudio2Lock();
void XAudio2Unlock();

/////////////////////////////////
// XAudio engine
class XAudio
{
public:
	XAudio() :
		pXA(nullptr),
		pMaster(nullptr)
	{
	}

	XAudioBGM bgm;

	void Init();
	void Shutdown();

	void SetBgm(const char* filename);

	IXAudio2* pXA;
	IXAudio2MasteringVoice* pMaster;
};
