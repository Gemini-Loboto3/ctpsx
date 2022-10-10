#include <stdafx.h>
#include "game.h"
#include "vm.h"

void sub_4017BB()
{
	prog.field_13C |= vm_index5[7];
	vm_index5[7] = 0;
}

void sub_4017D6()
{
	vm_index3[0] = ((4 * ~prog.field_1B6) | 3) & (5 * prog.click_bits);
	prog.field_1B6 = prog.click_bits;
	GetCursorPos((LPPOINT)&prog.field_1B8);
	ScreenToClient(prog.hWnd, (LPPOINT)&prog.field_1B8);
	vm_index3[1] = prog.field_1B8;
	vm_index3[2] = prog.field_1BC;
	vm_index3[3] = prog.key_is_down;
	vm_index3[4] = prog.field_1C8;
	vm_index3[5] = sprt_ent[0].x0;
	vm_index3[6] = sprt_ent[0].y0;
	vm_index3[17] = (WORD)sprt_ent[0].flag1;
	vm_index3[7] = 8 * LOWORD(sprt_dat[0].type0) + 4;
	vm_index3[8] = (WORD)sprt_dat[0].type;
	vm_index3[9] = sprt_ent[1].x0;
	vm_index3[10] = sprt_ent[1].y0;
	vm_index3[18] = (WORD)sprt_ent[1].flag1;
	vm_index3[11] = 8 * LOWORD(sprt_dat[1].type0) + 4;
	vm_index3[12] = (WORD)sprt_dat[1].type;
	vm_index3[19] = prog.screen_x;
	vm_index3[20] = prog.screen_y;
	vm_index3[21] = prog.pal_obj.type;
}

void sub_41259E()
{
	vm_index3[14] = 0;
	if (vm_index4[60])
		vm_index3[14] = 2;
	if (vm_index4[61])
		vm_index3[14] |= 1u;
}

int sub_403304(int a1)
{
	return vm_index7[a1] != 0xFFFF && vm_index2[vm_index7[a1]];
}

void __cdecl sub_405F2E()
{
	RECT rcDst; // [esp+0h] [ebp-14h] BYREF

	for (int i = 0; i < 30; ++i)
	{
		if (sub_403304(i))
		{
			rcDst.left = prog.render_rect.left + vm_rects[i].left - prog.screen_x;
			rcDst.right = prog.render_rect.left + vm_rects[i].right - prog.screen_x;
			rcDst.top = prog.render_rect.top + vm_rects[i].top - prog.screen_y;
			rcDst.bottom = prog.render_rect.top + vm_rects[i].bottom - prog.screen_y;
			if (IntersectRect(&rcDst, &prog.render_rect, &rcDst))
				RenderTile(rcDst.left, rcDst.top, rcDst.right - rcDst.left, rcDst.bottom - rcDst.top, 0, 0xff, 0xff);
		}
	}
}

void sub_405ED3()
{
	prog.field_140 = 1;
	TMapRender(&tmap);
	Vm_all_spr_disp();
	if (prog.field_134)
		sub_405F2E();
}

void DrawQSprites()
{
	if (prog.screen_on)
	{
		sub_405ED3();
		ItemRender();
	}
}

void Render_frame()
{
	sub_40245E();
	DrawQSprites();
	SwapBuffer();
	sub_403252();
}

// ========================================================
int __cdecl _totalsec(int year, int month, int day, int hour, int minute, int second)
{
	int _seconds, _days, _hours,
		_min_ex,
		_days_base, _days_month,
		_hour_ex,
		_month_src, _moth_pos,
		_odd;
	BYTE* pdays; // eax
	int time;

	static BYTE days_in_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	static WORD days_to_month[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };

	if (year < 70 || year > 138)
		return -1;
	_min_ex = second / 60 + minute;
	_seconds = second % 60;
	_hour_ex = _min_ex / 60 + hour;
	_hours = _min_ex % 60;
	_days_base = _hour_ex / 24 + day;
	_days = _hour_ex % 24;
	_month_src = month / 12 + year;
	_days_month = month % 12;
	pdays = &days_in_month[month % 12];
	while (_days_base >= (char)*pdays)
	{
		if ((_month_src & 3) != 0 || _days_month != 1)
		{
			_days_base -= (char)*pdays;
			++_days_month;
			++pdays;
		}
		else
		{
			if (_days_base <= 28)
				break;
			_days_base -= 29;
			_days_month = 2;
			++pdays;
		}
		if (_days_month >= 12)
		{
			_days_month -= 12;
			pdays -= 12;
			++_month_src;
		}
	}
	_moth_pos = _month_src - 70;
	_odd = _moth_pos + 2;
	if (_moth_pos + 2 < 0)
		_odd = _moth_pos + 5;
	_odd >>= 2;
	if (((_moth_pos + 70) & 3) == 0 && _days_month < 2)
		--_odd;
	time = _seconds + 60 * _hours + 3600 * _days + 86400 * (_days_base + days_to_month[_days_month] + 365 * _moth_pos + _odd) - 32400;
	if (time <= 0)
		return -1;
	else
		return time;
}

int _statcvt_i64(DWORD* a1)
{
	int ret;
	SYSTEMTIME SystemTime;

	GetLocalTime(&SystemTime);
	ret = _totalsec(
		SystemTime.wYear - 1900,
		SystemTime.wMonth - 1,
		SystemTime.wDay - 1,
		SystemTime.wHour,
		SystemTime.wMinute,
		SystemTime.wSecond);
	if (a1)
		*a1 = ret;
	return ret;
}

LARGE_INTEGER rand_seed;

void _srand(DWORD seed)
{
	rand_seed.QuadPart = seed;
}

long _rand()
{
	rand_seed.LowPart = 0x15A4E35 * rand_seed.LowPart + 1;
	return (rand_seed.u.LowPart >> 16) & 0x7FFF;
}

long _lrand()
{
	DWORD hi, temp;
	LARGE_INTEGER r;

	hi = rand_seed.HighPart;
	if (rand_seed.HighPart)
		hi = 20021 * rand_seed.HighPart;
	temp = hi + 5540 * rand_seed.LowPart;
	r.QuadPart = 20021 * rand_seed.LowPart;
	r.HighPart += temp;
	rand_seed.QuadPart = r.QuadPart + 1;
	return r.LowPart & 0x7FFFFFFF;
}
