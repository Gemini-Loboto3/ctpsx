#include <stdafx.h>
#include <time.h>
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
LONGLONG _statcvt_i64()
{
	return (LONGLONG)time(nullptr);
}

LARGE_INTEGER rand_seed;

void _srand(LONGLONG seed)
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
