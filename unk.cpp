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
	//vm_index3[7] = 8 * LOWORD(sprt_dat[0].type0) + 4;
	//vm_index3[8] = sprt_dat[0].type;
	vm_index3[9] = sprt_ent[1].x0;
	vm_index3[10] = sprt_ent[1].y0;
	vm_index3[18] = (WORD)sprt_ent[1].flag1;
	//vm_index3[11] = 8 * LOWORD(sprt_dat[1].type0) + 4;
	//vm_index3[12] = sprt_dat[1].type;
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
	//sub_40245E();
	DrawQSprites();
	//SwapBuffer(&prog.render_bmp);
	//sub_403252();
}

// ========================================================
static int sx = 1;
static int sy = 10000;
static int sz = 3000;

long lrand()
{
	int r, s;
	unsigned long ans;

	/*
	if( arg )
		{
		sx = 1;
		sy = 10000;
		sz = 3000;
		}
	*/

	/*  sx = sx * 171 mod 30269 */
	r = sx / 177;
	s = sx - 177 * r;
	sx = 171 * s - 2 * r;
	if (sx < 0)
		sx += 30269;


	/* sy = sy * 172 mod 30307 */
	r = sy / 176;
	s = sy - 176 * r;
	sy = 172 * s - 35 * r;
	if (sy < 0)
		sy += 30307;

	/* sz = 170 * sz mod 30323 */
	r = sz / 178;
	s = sz - 178 * r;
	sz = 170 * s - 63 * r;
	if (sz < 0)
		sz += 30323;

	ans = sx * sy * sz;
	return(ans);
}

int __cdecl _totalsec(int a1, int a2, int a3, int a4, int a5, int a6)
{
	int v7; // esi
	int v8; // edi
	int v9; // ebx
	BYTE* v10; // eax
	int v11; // edi
	int v12; // eax
	int v13; // eax
	int v14; // [esp+Ch] [ebp-4h]
	int v15; // [esp+24h] [ebp+14h]
	int v16; // [esp+24h] [ebp+14h]
	int v17; // [esp+28h] [ebp+18h]
	int v18; // [esp+28h] [ebp+18h]
	int v19; // [esp+2Ch] [ebp+1Ch]

	static BYTE byte_422818[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	static DWORD dword_422824[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };

	if (a1 < 70 || a1 > 138)
		return -1;
	v17 = a6 / 60 + a5;
	v19 = a6 % 60;
	v15 = v17 / 60 + a4;
	v18 = v17 % 60;
	v7 = v15 / 24 + a3;
	v16 = v15 % 24;
	v8 = a2 / 12 + a1;
	v9 = a2 % 12;
	v10 = &byte_422818[a2 % 12];
	while (v7 >= (char)*v10)
	{
		if ((v8 & 3) != 0 || v9 != 1)
		{
			v7 -= (char)*v10;
			++v9;
			++v10;
		}
		else
		{
			if (v7 <= 28)
				break;
			v7 -= 29;
			v9 = 2;
			++v10;
		}
		if (v9 >= 12)
		{
			v9 -= 12;
			v10 -= 12;
			++v8;
		}
	}
	v11 = v8 - 70;
	v12 = v11 + 2;
	if (v11 + 2 < 0)
		v12 = v11 + 5;
	v13 = v12 >> 2;
	if ((((BYTE)v11 + 70) & 3) == 0 && v9 < 2)
		--v13;
	v14 = 4294934896/*dword_422944*/ + v19 + 60 * v18 + 3600 * v16 + 86400 * (v7 + dword_422824[v9] + 365 * v11 + v13);
	//if (dword_422948 && _isDST(v16, v7, v9 + 1, v11))
	//	v14 -= 3600;
	if (v14 <= 0)
		return -1;
	else
		return v14;
}

int __cdecl _statcvt_i64(DWORD* a1)
{
	int result; // eax
	struct _SYSTEMTIME SystemTime; // [esp+4h] [ebp-10h] BYREF

	GetLocalTime(&SystemTime);
	result = _totalsec(
		SystemTime.wYear - 1900,
		SystemTime.wMonth - 1,
		SystemTime.wDay - 1,
		SystemTime.wHour,
		SystemTime.wMinute,
		SystemTime.wSecond);
	if (a1)
		*a1 = result;
	return result;
}

LARGE_INTEGER rand_seed;

void _srand(DWORD seed)
{
	rand_seed.QuadPart = seed;
}

long _rand()
{
	rand_seed.LowPart = 22695477 * rand_seed.LowPart + 1;
	return HIWORD(rand_seed.u.LowPart) & 0x7FFF;
}

long _lrand()
{
	LONG HighPart; // eax
	DWORD ten; // et1
	LARGE_INTEGER r; // rax

	HighPart = rand_seed.HighPart;
	if (rand_seed.HighPart)
		HighPart = 20021 * rand_seed.HighPart;
	ten = HighPart + 5540 * rand_seed.LowPart;
	r.QuadPart = 20021i64 * rand_seed.LowPart;
	r.HighPart += ten;
	rand_seed.QuadPart = r.QuadPart + 1;
	return ((unsigned __int64)(r.QuadPart + 1) >> 32) & 0x7FFFFFFF;
}
