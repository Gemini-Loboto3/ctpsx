#define _CRT_SECURE_NO_WARNINGS

#include <stdafx.h>
#include "game.h"

int Sound_get_pan(int x)
{
	if (x <= 0) return -16;
	if (x >= 480) return 16;

	int pan = 24 * (x + 10) / 480 - 12;
	if (pan < -12 || pan > 12)
		return 0;
	return pan;
}

void Sound_set_vol(const char* name, int vol)
{

}

void Sound_set_pan(const char* name, int pan)
{

}

void Sound_play(const char* name, int loops, int pan, int vol)
{

}

void Sound_stop(const char* str)
{
	auto ret = _strupr((char*)str);
	if (!strcmp(ret, "ALL"))
	{
		for (int i = 0; i < 6; ++i)
			Sound_clear(i);
	}
	else
	{
		for (int j = 0; j < 6; ++j)
		{
			//if (pDS_buf_tbl[j])
			{
				//if (!strcmp(ret, DS_buf_names[j]))
				//	return DSoundRelIndex(j) == 0;
			}
		}
	}
}

int Sound_clear(int index)
{
	return 0;
}
