#include <stdafx.h>
#include "game.h"

void GameDrawRects(VM* game, unsigned __int16 rect_no)
{
	RenderTile(GETX(game->rects[rect_no].x), GETY(game->rects[rect_no].y), game->rects[rect_no].w, game->rects[rect_no].h, (BYTE)game->r, (BYTE)game->g, (BYTE)game->b);
}

void Game_BgDispTrnRect(VM* a1)
{
	Game_BgDispTrn(a1,
		a1->rects[a1->rect_index].x,
		a1->rects[a1->rect_index].y,
		a1->rects[a1->rect_index].w,
		a1->rects[a1->rect_index].h,
		a1->rects[a1->rect_index].rw1,
		a1->rects[a1->rect_index].rw2,
		a1->rect_index,
		0,
		0);
}

void Game_BgDispTrn_1(VM* game)
{
	if (game->field_28A2)
	{
		if (game->field_28A6)
		{
			--game->field_28A6;
		}
		else
		{
			Game_BgDispTrn_0(game);
			game->field_28A6 = game->field_28A4;
			--game->field_28A2;
		}
	}
}

void Game_BgDispTrn_0(VM* a1)
{
	__int16 v1; // dx
	int v2; // cf
	__int16 v3; // dx
	__int16 v4; // dx
	__int16 v5; // dx

	v1 = a1->field_28AA;
	v2 = v1 == 0;
	v3 = v1 - 1;
	if (v2)
	{
		a1->rects[a1->rect_index].rw2 -= a1->rx;
		if (a1->rects[a1->rect_index].rw2 < 0)
			a1->rects[a1->rect_index].rw2 = a1->bank_no[a1->rect_index]->pix_h - 1;
	}
	else
	{
		v4 = v3 - 3;
		if (!v4)
		{
			a1->rects[a1->rect_index].rw1 += a1->rx;
			if (a1->rects[a1->rect_index].rw1 >= a1->bank_no[a1->rect_index]->real_w)
				a1->rects[a1->rect_index].rw1 = 0;
			Game_BgDispTrnRect(a1);
		}
		v5 = v4 - 4;
		if (!v5)
		{
			a1->rects[a1->rect_index].rw2 += a1->rx;
			if (a1->rects[a1->rect_index].rw2 >= a1->bank_no[a1->rect_index]->pix_h)
				a1->rects[a1->rect_index].rw2 = 0;
			Game_BgDispTrnRect(a1);
		}
		if (v5 == 4)
		{
			a1->rects[a1->rect_index].rw1 -= a1->rx;
			if (a1->rects[a1->rect_index].rw1 > 0x8000u)
				a1->rects[a1->rect_index].rw1 = a1->bank_no[a1->rect_index]->real_w - 1;
			Game_BgDispTrnRect(a1);
		}
	}
}

void Game_RedrawAll(VM* g)
{
	if (g->field_28B8[vm_evt_pos])
	{
		if (g->field_28CC[vm_evt_pos])
		{
			--g->field_28CC[vm_evt_pos];
		}
		else
		{
			g->rects[g->rect_xy_index[vm_evt_pos]].x = g->field_28E0[vm_evt_pos];
			g->rects[g->rect_xy_index[vm_evt_pos]].y = g->field_28E8[vm_evt_pos + 1];
			Game_RedrawScene(g);
			if (g->field_28FE[vm_evt_pos] + g->field_28CC[vm_evt_pos + 5] <= ++g->rect_xy_index[vm_evt_pos])
			{
				g->rect_xy_index[vm_evt_pos] = g->field_28CC[vm_evt_pos + 5];
				--g->field_28B8[vm_evt_pos];
			}
			g->field_28CC[vm_evt_pos] = g->field_28B8[vm_evt_pos + 5];
		}
	}
}

void Game_BgDispTrn(VM* game, int x, int y, int w, int h, __int16 a6, __int16 a7, WORD id, int type, int a10)
{
	if (game->bank_no[id])
	{
		if (type == 1)
			Vm_pal_set(0, 0, 0, 0, 0);
		if (!w)
			w = game->bank_no[id]->real_w;
		if (!h)
			h = game->bank_no[id]->pix_h;
		Render_bank(x, y, w, h, a6, a7, game->bank_no[id], a10);
		game->_id = id;
		game->_x = x;
		game->_y = y;
		game->_w = w;
		game->_h = h;
		game->rects[id].x = x;
		game->rects[id].y = y;
		game->rects[id].w = w;
		game->rects[id].h = h;
		game->rects[id].rw1 = a6;
		game->rects[id].rw2 = a7;
	}
	else printf("Bank %d doesn't exist!\n", id);
}

void GameDrawFromRects(VM* game, unsigned __int16 rect_no)
{
	RenderTile(GETX(game->rects[rect_no].x),
		GETY(game->rects[rect_no].y),
		game->rects[rect_no].w, game->rects[rect_no].h, (BYTE)game->r, (BYTE)game->g, (BYTE)game->b);
}

void Game_RedrawScene(VM* a1)
{
	if (a1->field_2908[vm_evt_pos])
		GameDrawFromRects(a1, a1->rect_xy_index[vm_evt_pos]);
	if (a1->field_2926[vm_evt_pos])
	{
		if (a1->bank_no[a1->rect_xy_index[vm_evt_pos]])
		{
			a1->bank_spr_id[a1->ent_index[vm_evt_pos]] = a1->rect_xy_index[vm_evt_pos];
			EntryBmpSprite(a1->ent_index[vm_evt_pos],
				a1->rects[a1->rect_xy_index[vm_evt_pos]].x,
				a1->rects[a1->rect_xy_index[vm_evt_pos]].y,
				*((WORD*)&a1->field_28F4 + vm_evt_pos),
				a1->bank_no[a1->rect_xy_index[vm_evt_pos]]->real_w,
				a1->bank_no[a1->rect_xy_index[vm_evt_pos]]->pix_h,
				a1->bank_no[a1->rect_xy_index[vm_evt_pos]],
				a1->field_2912[vm_evt_pos],
				0);
		}
	}
	else
	{
		Game_BgDispTrn(
			a1,
			a1->rects[a1->rect_xy_index[vm_evt_pos]].x,
			a1->rects[a1->rect_xy_index[vm_evt_pos]].y,
			a1->rects[a1->rect_xy_index[vm_evt_pos]].w,
			a1->rects[a1->rect_xy_index[vm_evt_pos]].h,
			a1->rects[a1->rect_xy_index[vm_evt_pos]].rw1,
			a1->rects[a1->rect_xy_index[vm_evt_pos]].rw2,
			a1->rect_xy_index[vm_evt_pos],
			0,
			0);
	}
}

void Render_bank(int x, int y, int w, int h, int srcx, int srcy, CTim* bank, int mode)
{
	//BmpGetRect(&prog.render_bmp, &rcSrc2);
	//result = IntersectRect(&dst_rect, &dst_rect, &rcSrc2);
	//if (result)
	{
		int _x = GetX(x, w);
		RenderRect(bank, _x, GETY(y), bank->real_w, bank->pix_h, 0, 0, 0xff, 0xff, 0xff);
	}
}
