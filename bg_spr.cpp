#include <stdafx.h>
#include "game.h"

void BgSprAnim(int id, int w, int h, CTim* ptr)
{
	if (sprt_ent[id].enabled)
	{
		sprt_ent[id].width = w;
		sprt_ent[id].height = h;
		sprt_ent[id].tim = ptr;
		//sprt_ent[id].bmp = ptr[(h - 1) * w];
		sprt_ent[id].field_37 = 0;
		sprt_ent[id].frame_id = 0xffff;
		SetSprIsBg(id, 1);
		sprt_ent[id].field_41 = 0;
		sprt_ent[id].is_busy = 0;
	}
}

void BgSprPos(int id, int x, int y, int flag)
{
	if (sprt_ent[id].enabled)
	{
		sprt_ent[id].x3 = x;
		sprt_ent[id].y3 = y;
		sprt_ent[id].x0 = x;
		sprt_ent[id].y0 = y;
		sprt_ent[id].flag1 = flag;
	}
}

void EntryBmpSprite(int id, __int16 x, __int16 y, __int16 flag, __int16 w, __int16 h, CTim* ptr, DWORD a8, WORD is_abs)
{
	if (id <= 20)
	{
		sprt_ent[id].id2 = id;
		sprt_ent[id].enabled = 1;
		sprt_ent[id].field_32 = a8;
		sprt_ent[id].is_abs = is_abs;
		BgSprPos(id, x, y, flag);
		BgSprAnim(id, w, h, ptr);
		Vm_spr_lmt(id, -1, -1);
		sprt_ent[id].SetList();
	}
}
