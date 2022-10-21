#include <stdafx.h>
#include <time.h>
#include "game.h"
#include "vm.h"

void sub_4017BB()
{
	prog.click_on_item |= vm_index5[7];
	vm_index5[7] = 0;
}

void Vm_update_state()
{
	vm_index3[0]  = ((4 * ~prog.click_old) | 3) & (5 * prog.click_bits);
	prog.click_old = prog.click_bits;
	//GetCursorPos(&prog.mousePT);
	prog.mousePT.x = mouseX;
	prog.mousePT.y = mouseY;
	//ScreenToClient(prog.hWnd, &prog.mousePT);
	vm_index3[1]  = (WORD)prog.mousePT.x;
	vm_index3[2]  = (WORD)prog.mousePT.y;
	vm_index3[3]  = prog.key_is_down;
	vm_index3[4]  = prog.in_key;
	vm_index3[5]  = sprt_ent[0].x0;
	vm_index3[6]  = sprt_ent[0].y0;
	vm_index3[7]  = WORD(8 * ai_ent[0].type0 + 4);
	vm_index3[8]  = (WORD)ai_ent[0].type;
	vm_index3[9]  = sprt_ent[1].x0;
	vm_index3[10] = sprt_ent[1].y0;
	vm_index3[11] = WORD(8 * ai_ent[1].type0 + 4);
	vm_index3[12] = (WORD)ai_ent[1].type;
	vm_index3[17] = (WORD)sprt_ent[0].flag1;
	vm_index3[18] = (WORD)sprt_ent[1].flag1;
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
		vm_index3[14] |= 1;
}

int sub_403304(int a1)
{
	return vm_index6[a1 + 500] != 0xFFFF && vm_index2[vm_index6[a1 + 500]];
}

void TriggerDebug()
{
	CRect rtrg; // [esp+0h] [ebp-14h] BYREF

	for (int i = 0; i < 30; ++i)
	{
		if (sub_403304(i))
		{
			rtrg.left = prog.render_rect.left + vm_rects[i].left - prog.screen_x;
			rtrg.right = prog.render_rect.left + vm_rects[i].right - prog.screen_x;
			rtrg.top = prog.render_rect.top + vm_rects[i].top - prog.screen_y;
			rtrg.bottom = prog.render_rect.top + vm_rects[i].bottom - prog.screen_y;
			if (intersectRect(&prog.render_rect, &rtrg))
				RenderTile(GETX(rtrg.left), GETY(rtrg.top), rtrg.W() / 2, rtrg.H() / 2, 0, 0xff, 0xff, 0x80);
		}
	}
}

void sub_405ED3()
{
	prog.field_140 = 1;
	TMapRender(&tmap);
	Vm_all_spr_disp();
	if (prog.trigger_debug)
		TriggerDebug();
}

void DrawQSprites()
{
	if (prog.screen_on)
	{
		sub_405ED3();
		ItemRender();
	}
}

// ========================================================
// BORLAND randomizer
// ========================================================

static LARGE_INTEGER rand_seed;

LONGLONG _statcvt_i64()
{
	return (LONGLONG)time(nullptr);
}

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
