#include <stdafx.h>
#include <stdlib.h>
#include "timer.h"

bool FrameLimiter::Init()
{
	LARGE_INTEGER frequency;

	if (!QueryPerformanceFrequency(&frequency))
		return false;

	TIME_Frequency = (double)frequency.QuadPart / (double)TICKS_PER_SECOND;
	Ticks();
	return true;
}

void FrameLimiter::Ticks()
{
	LARGE_INTEGER counter;

	QueryPerformanceCounter(&counter);
	TIME_Ticks = (double)counter.QuadPart / TIME_Frequency;
}

DWORD FrameLimiter::Sync(int VSync_rate)
{
	static double mult[] =
	{
		1,		// 60 fps
		-1,		// uncapped
		2,		// 30 fps
		4,		// 15 fps
		4,		// 7.5 fps?
		4		// 3.75 fps?
	};

	DWORD lastTicks, currentTicks;
	LARGE_INTEGER counter;

	QueryPerformanceCounter(&counter);
	lastTicks = (DWORD)TIME_Ticks;
	TIME_Ticks = (double)counter.QuadPart / (TIME_Frequency * mult[VSync_rate] * turbo);
	currentTicks = (DWORD)TIME_Ticks;

	return (currentTicks > lastTicks) ? currentTicks - lastTicks : 0;
}

DWORD FrameLimiter::Sync()
{
	DWORD lastTicks, currentTicks;
	LARGE_INTEGER counter;

	QueryPerformanceCounter(&counter);
	lastTicks = (DWORD)TIME_Ticks;
	TIME_Ticks = (double)counter.QuadPart / (TIME_Frequency * 2);
	currentTicks = (DWORD)TIME_Ticks;

	return (currentTicks > lastTicks) ? currentTicks - lastTicks : 0;
}

void FrameLimiter::NextSpeed()
{
	turbo_id++;
	if (turbo_id >= _countof(turbo_tbl))
		turbo_id = _countof(turbo_tbl) - 1;
	turbo = turbo_tbl[turbo_id];
}

void FrameLimiter::PrevSpeed()
{
	turbo_id--;
	if (turbo_id < 0)
		turbo_id = 0;
	turbo = turbo_tbl[turbo_id];
}

void FrameLimiter::FixVoiceSpeed()
{
	turbo_id = 1;
	turbo = turbo_tbl[1];
}
