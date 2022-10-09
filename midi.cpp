#include <stdafx.h>
#include "xaudio/audio.h"

XAudio xa;

void LoadSf2()
{
	XAudio2_IsPresent();

	xa.Init();
}

void Midi_load(const char* filename)
{
	char fname[MAX_PATH], path[MAX_PATH];
	strcpy_s(fname, MAX_PATH, filename);
	*strrchr(fname, '.') = '\0';

	sprintf_s(path, MAX_PATH, "%s.OGG", fname);

	xa.SetBgm(path);
}

void Midi_unload()
{
	xa.bgm.Stop();
	xa.bgm.Release();
}
