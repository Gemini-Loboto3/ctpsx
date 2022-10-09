#pragma once

#define FRAMES_PER_SECOND	(60)
#define TICKS_PER_FRAME		(1)
#define TICKS_PER_SECOND	(TICKS_PER_FRAME * FRAMES_PER_SECOND)

#include <pshpack8.h>
class FrameLimiter
{
public:
	FrameLimiter() :
		turbo_id(0),
		turbo(1.0),
		TIME_Frequency(0.),
		TIME_Ticks(0.),
		turbo_tbl{ 1.0, 0.5, 1.0 / 3.0 }
	{ }

	bool Init();
	DWORD Sync(int VSync_rate);
	DWORD Sync();
	void NextSpeed();
	void PrevSpeed();
	void FixVoiceSpeed();

	__inline int    GetSpeed()    { return turbo_id; }
	__inline double GetSpeedFps() { return turbo; }

private:
	//DWORD TID;
	//int Timer_th;
	double TIME_Frequency,
		TIME_Ticks;
	double turbo,
		turbo_tbl[3];
	int turbo_id;

	void Ticks();
};
#include <poppack.h>
