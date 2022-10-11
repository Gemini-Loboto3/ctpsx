#include <stdafx.h>
#include "audio.h"

/////////////////////////////////////
float fsnd_tbl[] =
{
	0.000000f, 0.000037f, 0.000161f, 0.000378f, 0.000693f, 0.001108f, 0.001626f, 0.002249f,
	0.002978f, 0.003815f, 0.004762f, 0.005820f, 0.006989f, 0.008270f, 0.009666f, 0.011176f,
	0.012801f, 0.014542f, 0.016400f, 0.018376f, 0.020470f, 0.022683f, 0.025015f, 0.027467f,
	0.030040f, 0.032734f, 0.035550f, 0.038487f, 0.041548f, 0.044731f, 0.048038f, 0.051469f,
	0.055024f, 0.058704f, 0.062509f, 0.066439f, 0.070496f, 0.074679f, 0.078989f, 0.083425f,
	0.087989f, 0.092681f, 0.097501f, 0.102449f, 0.107526f, 0.112731f, 0.118066f, 0.123531f,
	0.129125f, 0.134850f, 0.140705f, 0.146691f, 0.152807f, 0.159055f, 0.165435f, 0.171946f,
	0.178589f, 0.185364f, 0.192272f, 0.199313f, 0.206486f, 0.213793f, 0.221233f, 0.228807f,
	0.236515f, 0.244356f, 0.252333f, 0.260443f, 0.268688f, 0.277069f, 0.285584f, 0.294235f,
	0.303021f, 0.311943f, 0.321001f, 0.330195f, 0.339526f, 0.348993f, 0.358596f, 0.368337f,
	0.378214f, 0.388229f, 0.398381f, 0.408671f, 0.419098f, 0.429664f, 0.440367f, 0.451209f,
	0.462189f, 0.473308f, 0.484566f, 0.495962f, 0.507498f, 0.519172f, 0.530987f, 0.542940f,
	0.555034f, 0.567267f, 0.579641f, 0.592154f, 0.604808f, 0.617602f, 0.630537f, 0.643612f,
	0.656829f, 0.670186f, 0.683685f, 0.697325f, 0.711106f, 0.725029f, 0.739094f, 0.753300f,
	0.767649f, 0.782139f, 0.796772f, 0.811547f, 0.826465f, 0.841525f, 0.856728f, 0.872074f,
	0.887563f, 0.903195f, 0.918970f, 0.934889f, 0.950951f, 0.967157f, 0.983507f, 1.000000f
};

float XAudioConvertVolume(int volume)
{
	// no gain... no pain?
	if (volume <= -1 || volume > 127)
		volume = 127;

	return fsnd_tbl[volume];
}

CRITICAL_SECTION xa_cs;

void XAudio2Lock()
{
	EnterCriticalSection(&xa_cs);
}

void XAudio2Unlock()
{
	LeaveCriticalSection(&xa_cs);
}

HMODULE xdll = nullptr;
HRESULT (__stdcall *XAudio2CreateEx)(_Outptr_ IXAudio2** ppXAudio2, UINT32 Flags, XAUDIO2_PROCESSOR XAudio2Processor);

bool XAudio2_IsPresent()
{
	// -----------------------------------------------
	// new XAudio2 2.9 loader which doesn't rely on libs
#if _DEBUG
	xdll = LoadLibraryA("xaudio2_9d.dll");
#else
	xdll = LoadLibraryA("xaudio2_9.dll");
#endif
	if (!xdll)
	{
		wchar_t path[MAX_PATH];
		CopyMemory(path + GetSystemDirectoryW(path, MAX_PATH - 14), "\\xaudio2_9.dll", 15 * 2);
		xdll = LoadLibraryW(path);
		if(!xdll)
			return false;
	}

	XAudio2CreateEx = (HRESULT (__stdcall *)(IXAudio2**, UINT32, XAUDIO2_PROCESSOR))GetProcAddress(xdll, "XAudio2Create");
	if (!XAudio2CreateEx)
	{
		FreeLibrary(xdll);
		return false;
	}
	// -----------------------------------------------

	bool ret;
	IXAudio2* pXAudio2;

	if (FAILED(XAudio2CreateEx(&pXAudio2, 0, 0)))
		ret = false;
	else
	{
		ret = true;
		pXAudio2->Release();
	}

	return ret;
}

/////////////////////////////////////
void XAudio::Init()
{
	if (pXA) return;	// already initialized

#if defined(_DEBUG)
	UINT32 flags = XAUDIO2_DEBUG_ENGINE;
#else
	UINT32 flags = 0;
#endif

	if (FAILED(XAudio2CreateEx(&pXA, flags, 0)))
		return;

#if defined(_DEBUG)
	// To see the trace output, you need to view ETW logs for this application:
	//    Go to Control Panel, Administrative Tools, Event Viewer.
	//    View->Show Analytic and Debug Logs.
	//    Applications and Services Logs / Microsoft / Windows / XAudio2. 
	//    Right click on Microsoft Windows XAudio2 debug logging, Properties, then Enable Logging, and hit OK 
	XAUDIO2_DEBUG_CONFIGURATION debug = { 0 };
	debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
	debug.BreakMask = XAUDIO2_LOG_ERRORS;
	pXA->SetDebugConfiguration(&debug, 0);
#endif

	pXA->CreateMasteringVoice(&pMaster, 2, 44100);

	InitializeCriticalSection(&xa_cs);
}

void XAudio::Shutdown()
{
	if (pMaster)
	{
		pMaster->DestroyVoice();
		pMaster = nullptr;
	}

	if (pXA)
	{
		pXA->Release();
		pXA = nullptr;
		CoUninitialize();
	}

	FreeLibrary(xdll);
	DeleteCriticalSection(&xa_cs);
}

void XAudio::SetBgm(const char* filename)
{
	bgm.Open(pXA, filename);
	bgm.Start(0);
}
