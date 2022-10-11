// ========================================
// Streamable audio data
// ----------------------------------------
// Takes input data of any type and tries
// to stream in fix-sized chunks.
// ========================================

#include <stdafx.h>
#include <vector>
#include "audio.h"
#include "audio_bgm.h"

XAudioBGM::XAudioBGM() :
	hThread(CreateThread(nullptr, 0, thread, this, CREATE_SUSPENDED, nullptr)),
	buffer{ 0 },
	voice(nullptr),
	xa_buf{ 0 },
	type(XABGM_TYPE_NONE),
	index(0),
	sent(0),
	str(nullptr),
	status(XBGMST_NOTHING),
	mat{ 0 },
	panning(0.5f),
	volume(127),
	vol_change(0),
	pan_change(0),
	chans(1)
{
}

XAudioBGM::~XAudioBGM()
{
	Release();
	if(hThread)
		CloseHandle(hThread);
}

void XAudioBGM::Open(IXAudio2* pXA, const char *filename)
{
	try
	{
		type = XABGM_TYPE_OGG;
		str = new AudioStreamOgg;

		if (str == nullptr)
			throw 1;
		if (str->open(filename) == 0)
			throw 2;

		index = 0;
		sent = 0;
		status = XBGMST_STOPPED;
		// settings for voice based on this stream
		WAVEFORMATEX fmt;
		fmt.cbSize = sizeof(fmt);
		fmt.wFormatTag = WAVE_FORMAT_PCM;
		fmt.nChannels = (WORD)str->Channels();
		fmt.nBlockAlign = fmt.nChannels * 16 / 8;
		fmt.nSamplesPerSec = str->Frequency();
		fmt.nAvgBytesPerSec = fmt.nSamplesPerSec * fmt.nBlockAlign;
		fmt.wBitsPerSample = 16;
		// prepare xaudio2 double buffer data
		ZeroMemory(&xa_buf, sizeof(xa_buf));
		xa_buf.Flags = XAUDIO2_END_OF_STREAM;
		xa_buf.AudioBytes = XABGM_BUFFER_SIZE;
		xa_buf.pContext = this;
		xa_buf.PlayLength = XABGM_BUFFER_SIZE / fmt.nBlockAlign;

		vol_change = 0;
		pan_change = 0;
		panning = 0.5f;
		chans = str->Channels();
		// generate a suitable voice for this stream
		pXA->CreateSourceVoice(&voice, &fmt, 0, 2.f, &cb);
	}
	catch (...)
	{
	//	LPCSTR errors[] =
	//	{
	//		"Unsupported audio format.",
	//		"No stream open.",
	//		"Couldn't open stream."
	//	};

	//	//MessageBoxX(nullptr, errors[error], __FUNCTION__, MB_ICONERROR);
	}
}

void XAudioBGM::Release()
{
	if (voice)
	{
		Stop();
		voice->DestroyVoice();
		voice = nullptr;
	}

	if (str)
	{
		str->close();
		delete str;
		str = nullptr;
	}

	type = XABGM_TYPE_NONE;
	status = XBGMST_NOTHING;
}

void XAudioBGM::Start(int mode)
{
	if (voice)
	{
		if (status == XBGMST_PLAYING)
			return;

		// set the looping flag
		str->setlooping(mode ? 1 : 0);

		// first pass
		if (sent == 0)
		{
			// do some filling
			Fill(false);
			Fill(false);
			// prevent buffer flip glitch
			ResetEvent(cb.hEndEvent);
		}

		// kick playback
		ResumeThread(hThread);
		voice->Start();
		status = XBGMST_PLAYING;
	}
}

void XAudioBGM::Stop()
{
	if (voice)
	{
		if (status != XBGMST_PLAYING)
			return;
		SuspendThread(hThread);
		voice->Stop();
		status = XBGMST_STOPPED;
	}
}

void XAudioBGM::Rewind()
{
	if (str)
	{
		str->rewind();
		// remove queued buffers
		if (voice)
			voice->FlushSourceBuffers();
		// make sure we send packets whenever the audio restarts
		sent = 0;
	}
}

void XAudioBGM::SetVolume(int vol)
{
	volume = vol;
	if (voice)
	{
		auto volf = vol / 127.f;// XAudioConvertVolume(vol);
		voice->SetVolume(volf);
	}
	else vol_change = 1;
}

void XAudioBGM::SetPan(float pan)
{
	panning = pan;

	if (voice)
	{
		if (chans == 2)
		{
			mat[1] = mat[2] = 0.f;
			mat[0] = 0.5f - panning / 2.f;
			mat[3] = 0.5f + panning / 2.f;
		}
		else
		{
			mat[0] = 0.5f - panning / 2.f;
			mat[1] = 0.5f + panning / 2.f;
		}
		voice->SetOutputMatrix(nullptr, chans, 2, mat);
	}
	else pan_change = 1;
}

void XAudioBGM::Fill(bool start)
{
	XAudio2Lock();
	// do buffer filling
	xa_buf.pAudioData = buffer[index];
	str->fill(buffer[index], XABGM_BUFFER_SIZE);
	index ^= 1;
	sent++;
	XAudio2Unlock();

	if (vol_change)
	{
		vol_change = 0;
		voice->SetVolume(volume / 127.f/*XAudioConvertVolume(volume)*/);
	}
	if (pan_change)
	{
		pan_change = 0;
		if (chans == 2)
		{
			mat[1] = mat[2] = 0.f;
			mat[0] = 0.5f - panning / 2.f;
			mat[3] = 0.5f + panning / 2.f;
		}
		else
		{
			mat[0] = 0.5f - panning / 2.f;
			mat[1] = 0.5f + panning / 2.f;
		}
		voice->SetOutputMatrix(nullptr, chans, 2, mat);
	}

	// keep the loop going
	voice->SubmitSourceBuffer(&xa_buf);

	if(start)
		voice->Start();
}

DWORD WINAPI XAudioBGM::thread(LPVOID context)
{
	XAudioBGM* ths = (XAudioBGM*)context;

	while (1)
		ths->Update();

	return 0;
}

void XAudioBGM::Update()
{
	if (status != XBGMST_PLAYING)
	{
		Sleep(20);	// cheese a bit the looping whenever it's not playing
		return;
	}
	// wait until one buffer is done playing
	WaitForSingleObject(cb.hEndEvent, INFINITE);
	// queue another buffer
	Fill();
}

void BGMCallback::OnBufferEnd(void* pBufferContext)
{
	// signal to thread that this buffer is processed
	SetEvent(hEndEvent);
}
