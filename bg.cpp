#include <stdafx.h>
#include "game.h"

void GameDrawRects(VM* vm, int rect_no)
{
	RenderTile(GETX(vm->rects[rect_no].x), GETY(vm->rects[rect_no].y), vm->rects[rect_no].w, vm->rects[rect_no].h, (BYTE)vm->r, (BYTE)vm->g, (BYTE)vm->b);
}

void Game_BgDispTrnRect(VM* vm)
{
	Game_BgDispTrn(vm,
		vm->rects[vm->rect_index].x,
		vm->rects[vm->rect_index].y,
		vm->rects[vm->rect_index].w,
		vm->rects[vm->rect_index].h,
		vm->rects[vm->rect_index].rw1,
		vm->rects[vm->rect_index].rw2,
		vm->rect_index,
		0,
		0);
}

void Game_BgDispTrn_1(VM* vm)
{
	if (vm->field_28A2)
	{
		if (vm->field_28A6)
		{
			--vm->field_28A6;
		}
		else
		{
			Game_BgDispTrn_0(vm);
			vm->field_28A6 = vm->field_28A4;
			--vm->field_28A2;
		}
	}
}

void Game_BgDispTrn_0(VM* vm)
{
	__int16 v1; // dx
	int v2; // cf
	__int16 v3; // dx
	__int16 v4; // dx
	__int16 v5; // dx

	v1 = vm->field_28AA;
	v2 = v1 == 0;
	v3 = v1 - 1;
	if (v2)
	{
		vm->rects[vm->rect_index].rw2 -= vm->rx;
		if (vm->rects[vm->rect_index].rw2 < 0)
			vm->rects[vm->rect_index].rw2 = vm->bank_no[vm->rect_index]->h - 1;
	}
	else
	{
		v4 = v3 - 3;
		if (!v4)
		{
			vm->rects[vm->rect_index].rw1 += vm->rx;
			if (vm->rects[vm->rect_index].rw1 >= vm->bank_no[vm->rect_index]->w)
				vm->rects[vm->rect_index].rw1 = 0;
			Game_BgDispTrnRect(vm);
		}
		v5 = v4 - 4;
		if (!v5)
		{
			vm->rects[vm->rect_index].rw2 += vm->rx;
			if (vm->rects[vm->rect_index].rw2 >= vm->bank_no[vm->rect_index]->h)
				vm->rects[vm->rect_index].rw2 = 0;
			Game_BgDispTrnRect(vm);
		}
		if (v5 == 4)
		{
			vm->rects[vm->rect_index].rw1 -= vm->rx;
			if (vm->rects[vm->rect_index].rw1 > 0x8000u)
				vm->rects[vm->rect_index].rw1 = vm->bank_no[vm->rect_index]->w - 1;
			Game_BgDispTrnRect(vm);
		}
	}
}

void Game_RedrawAll(VM* vm)
{
	if (vm->field_28B8[vm_data.vm_evt_pos])
	{
		if (vm->field_28CC[vm_data.vm_evt_pos])
		{
			--vm->field_28CC[vm_data.vm_evt_pos];
		}
		else
		{
			vm->rects[vm->rect_xy_index[vm_data.vm_evt_pos]].x = vm->field_28E0[vm_data.vm_evt_pos];
			vm->rects[vm->rect_xy_index[vm_data.vm_evt_pos]].y = vm->field_28E8[vm_data.vm_evt_pos + 1];
			Game_RedrawScene(vm);
			if (vm->field_28FE[vm_data.vm_evt_pos] + vm->field_28CC[vm_data.vm_evt_pos + 5] <= ++vm->rect_xy_index[vm_data.vm_evt_pos])
			{
				vm->rect_xy_index[vm_data.vm_evt_pos] = vm->field_28CC[vm_data.vm_evt_pos + 5];
				--vm->field_28B8[vm_data.vm_evt_pos];
			}
			vm->field_28CC[vm_data.vm_evt_pos] = vm->field_28B8[vm_data.vm_evt_pos + 5];
		}
	}
}

void Game_BgDispTrn(VM* vm, int x, int y, int w, int h, __int16 srcx, __int16 srcy, WORD id, int type, int mode)
{
	if (vm->bank_no[id])
	{
		if (type == 1)
			Vm_pal_set(0, 0, 0, 0, 0);
		if (w == 0)
			w = vm->bank_no[id]->w;
		if (h == 0)
			h = vm->bank_no[id]->h;
		Render_bank(x, y, w, h, srcx, srcy, vm->bank_no[id], mode);
		vm->_id = id;
		vm->_x = x;
		vm->_y = y;
		vm->_w = w;
		vm->_h = h;
		vm->rects[id].x = x;
		vm->rects[id].y = y;
		vm->rects[id].w = w;
		vm->rects[id].h = h;
		vm->rects[id].rw1 = srcx;
		vm->rects[id].rw2 = srcy;
	}
	else printf("Bank %d doesn't exist!\n", id);
}

void GameDrawFromRects(VM* vm, int rect_no)
{
	RenderTile(GETX(vm->rects[rect_no].x),
		GETY(vm->rects[rect_no].y),
		vm->rects[rect_no].w, vm->rects[rect_no].h, (BYTE)vm->r, (BYTE)vm->g, (BYTE)vm->b);
}

void Game_RedrawScene(VM* vm)
{
	if (vm->field_2908[vm_data.vm_evt_pos])
		GameDrawFromRects(vm, vm->rect_xy_index[vm_data.vm_evt_pos]);
	if (vm->field_2926[vm_data.vm_evt_pos])
	{
		if (vm->bank_no[vm->rect_xy_index[vm_data.vm_evt_pos]])
		{
			vm->bank_spr_id[vm->ent_index[vm_data.vm_evt_pos]] = vm->rect_xy_index[vm_data.vm_evt_pos];
			EntryBmpSprite(vm->ent_index[vm_data.vm_evt_pos],
				vm->rects[vm->rect_xy_index[vm_data.vm_evt_pos]].x,
				vm->rects[vm->rect_xy_index[vm_data.vm_evt_pos]].y,
				*((WORD*)&vm->field_28F4 + vm_data.vm_evt_pos),
				vm->bank_no[vm->rect_xy_index[vm_data.vm_evt_pos]]->w,
				vm->bank_no[vm->rect_xy_index[vm_data.vm_evt_pos]]->h,
				vm->bank_no[vm->rect_xy_index[vm_data.vm_evt_pos]],
				vm->field_2912[vm_data.vm_evt_pos],
				0);
		}
	}
	else
	{
		Game_BgDispTrn(
			vm,
			vm->rects[vm->rect_xy_index[vm_data.vm_evt_pos]].x,
			vm->rects[vm->rect_xy_index[vm_data.vm_evt_pos]].y,
			vm->rects[vm->rect_xy_index[vm_data.vm_evt_pos]].w,
			vm->rects[vm->rect_xy_index[vm_data.vm_evt_pos]].h,
			vm->rects[vm->rect_xy_index[vm_data.vm_evt_pos]].rw1,
			vm->rects[vm->rect_xy_index[vm_data.vm_evt_pos]].rw2,
			vm->rect_xy_index[vm_data.vm_evt_pos],
			0,
			0);
	}
}

void Render_bank(int x, int y, int w, int h, int srcx, int srcy, CTexture* bank, int mode)
{
	//BmpGetRect(&prog.render_bmp, &rcSrc2);
	//result = intersectRect(&dst_rect, &rcSrc2);
	//if (result)
	{
		int _x = GetX(x, w);
		RenderRect(bank, _x, GETY(y), bank->w, bank->h, 0, 0, 0xff, 0xff, 0xff);
	}
}
