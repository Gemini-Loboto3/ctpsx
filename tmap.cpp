#include <stdafx.h>
#include "game.h"

TMAP tmap;

TMAP::TMAP()
{
	TMapResetTiles(this);

	tim = nullptr;
	map_data = 0;
	w = 0;
	h = 0;
	x1 = 0;
	y1 = 0;
	speedX = 0;
	speedY = 0;
}

TMAP::~TMAP()
{
	if (tim)
	{
		delete tim;
		tim = nullptr;
	}

	if (map_data)
	{
		delete map_data;
		map_data = nullptr;
	}
}

void TMap_SetXY1(TMAP* tmap, int x, int y)
{
	tmap->x1 = x;
	tmap->y1 = y;
}

void TMap_GetXY1(TMAP* tmap, int* dst)
{
	dst[0] = tmap->x1;
	dst[1] = tmap->y1;
}

int TMap_is_scrolling(TMAP* tmap)
{
	if (tmap->speedX) return tmap->speedX;
	if (tmap->speedY) return tmap->speedY;

	return 0;
}

int TMapIsSpeedXSet(TMAP* tmap)
{
	return tmap->speedX != 0;
}

int TMapIsSpeedYSet(TMAP* tmap)
{
	return tmap->speedY != 0;
}

int TMapCanXScrol(TMAP* tmap)
{
	if (tmap->speedX)
		return abs(tmap->scrlx - tmap->x1) <= tmap->speedX;

	return 0;
}

int TMapCanYScrol(TMAP* tmap)
{
	if (tmap->speedY)
		return abs(tmap->scrly - tmap->y1) <= tmap->speedY;

	return 0;
}

void TMapScrollUpdate(TMAP* tmap)
{
	int spd;

	if (TMap_is_scrolling(tmap))
	{
		spd = TMapCalcSpeed(tmap, tmap->x1, tmap->scrlx, tmap->speedX);
		if (abs(tmap->scrlx - tmap->x1) <= tmap->speedX)
			tmap->speedX = 0;
		tmap->x1 += spd;
		spd = TMapCalcSpeed(tmap, tmap->y1, tmap->scrly, tmap->speedY);
		if (abs(tmap->scrly - tmap->y1) <= tmap->speedY)
			tmap->speedY = 0;
		tmap->y1 += spd;
	}
}

int TMapCalcSpeed(TMAP* tmap, int pos, int scrl, signed int spd)
{
	int difa, diff;

	if (spd)
	{
		diff = scrl - pos;
		difa = abs(diff);
		if (difa < spd)
			spd = difa;
		if (!diff)
			return 0;
		spd *= diff / difa;
	}
	return spd;
}

void TMap_scrollXY(TMAP* tmap, int x, int y, int speed)
{
	TMap_scrollX(tmap, x, speed);
	TMap_scrollY(tmap, y, speed);
}

void TMap_scrollX(TMAP* tmap, int scrlx, int speed)
{
	tmap->scrlx = scrlx;
	if (tmap->scrlx == tmap->x1)
		tmap->speedX = 0;
	else
		tmap->speedX = speed;
}

void TMap_scrollY(TMAP* tmap, int scrly, int speed)
{
	tmap->scrly = scrly;
	if (tmap->scrly == tmap->y1)
		tmap->speedY = 0;
	else
		tmap->speedY = speed;
}

void TMapGetRect(TMAP* tmap, CRect* lprcDst)
{
	copyRect(lprcDst, &tmap->clip);
}

void TMapRenderScroll(TMAP* tmap, int wx0, int wy0, int ww, int wh, int xpos, int ypos)
{
	setRect(&tmap->clip, wx0, wy0, ww + wx0 - 1, wh + wy0 - 1);
	tmap->x1 = xpos;
	tmap->y1 = ypos;
	TMapRender(tmap);
}

void TMapRenderScroll2(TMAP* tmap, int x, int y, int w, int h)
{
	setRect(&tmap->clip, x, y, w + x - 1, h + y - 1);
	TMapRender(tmap);
}

void TMapRender(TMAP* tmap)
{
	if (tmap->tim == nullptr)
		return;

	CRect rsrc, rdst;

	TMapGetDstRect(tmap, &rdst);
	if (TMapGetSize(tmap, &rsrc))
	{
		if (intersectRect(&rdst, &rsrc))
		{
			int x, y;
			
			if (tmap->scroll)
			{
				x = tmap->clip.X0();
				y = tmap->clip.Y0();
			}
			else
			{
				x = (tmap->clip.X0() + tmap->x1 - rdst.X0());
				y = (tmap->clip.Y0()  + tmap->y1 - rdst.Y0());
			}

			x /= 2;
			y /= 2;
			int u = rdst.X0() / 2;
			int v = rdst.Y0() / 2;
			int w = rdst.W() / 2 + 1;
			int h = rdst.H() / 2 + 1;

			RenderRect(tmap->tim, x, y, w, h, u, v, 0xff, 0xff, 0xff);
		}
	}
	//else
	{
		//hbr = (HBRUSH)GetStockObject(BLACK_BRUSH);
		//BmpRenderFill(
		//	&prog.render_bmp,
		//	tmap->clip.left,
		//	tmap->clip.top,
		//	tmap->clip.right - tmap->clip.left + 1,
		//	tmap->clip.bottom - tmap->clip.top + 1,
		//	hbr);
		//DeleteObject(hbr);
	}
}

void TMapResetTiles(TMAP* tmap)
{
	tmap->tile_x = 0;
	tmap->tile_y = 0;
	tmap->tile_no = 0;
}

int LoadDIB(CTim** t, const char* filename)
{
	char path[MAX_PATH];
	strncpy_s(path, MAX_PATH, filename, strrchr(filename, '.') - filename);
	strcat_s(path, MAX_PATH, ".TIM");

	CTim* tim = new CTim();
	if(tim->Open(path))
	{
		*t = tim;

		return 1;
	}

	return 0;
}

int TMapOpenDIB(TMAP* tmap, LPCSTR lpFileName)
{
	if (LoadDIB(&tmap->tim, lpFileName))
	{
		tmap->w = tmap->tim->real_w * 2;
		tmap->h = tmap->tim->pix_h * 2;
		tmap->scroll = 0;

		return 1;
	}

	return 0;
}

int TMapOpenAll(TMAP* tmap, LPCSTR filename_dib, LPCSTR filename_map)
{
	if (filename_dib)
	{
		if (!TMapOpenDIB(tmap, filename_dib))
			return 0;
		//if (filename_map && !TMapOpen(tmap, filename_map))	// there's no .map on psx
		//	return 0;
	}
	else
	{
		tmap->w = 0;
		tmap->h = 0;
	}

	return 1;
}

void TMapCache(LPCSTR dib_name, LPCSTR map_name)
{
	LPCSTR map;
	LPCSTR dib;

	dib = dib_name ? dib_name : nullptr;
	map = map_name ? map_name : nullptr;

	TMapOpenAll(&tmap, map, map);
}

void Vm_map_scroll(int scrlx, int scrly, int speed)
{
	if (scrlx != -1)
		TMap_scrollX(&tmap, scrlx, speed);
	if (scrly != -1)
		TMap_scrollY(&tmap, scrly, speed);
}

int Vm_bg_scrolling()
{
	return TMap_is_scrolling(&tmap) ? 0 : 1;
}

int tmap_xscroll_index, tmap_yscroll_index;
__int16 tmap_xscroll_tbl[16];
__int16 tmap_yscroll_tbl[16];

void Vm_tmap_set_scroll(int xscroll_cnt, WORD* xscroll_data, int yscroll_cnt, WORD* yscroll_data)
{
	tmap_xscroll_tbl[0] = xscroll_cnt;
	for (int i = 1; i < 16; ++i)
		tmap_xscroll_tbl[i] = xscroll_data[i - 1];
	tmap_yscroll_tbl[0] = yscroll_cnt;
	for (int i = 1; i < 16; ++i)
		tmap_yscroll_tbl[i] = yscroll_data[i - 1];
}

void TMapScrollX_cond(int x, int speed)
{
	int scrl;

	scrl = 0;
	if (sprt_player.x0 - prog.screen_x <= speed || tmap_xscroll_tbl[0] <= tmap_xscroll_index)
	{
		if (sprt_player.x0 - prog.screen_x < x && tmap_xscroll_index > 1)
		{
			--tmap_xscroll_index;
			scrl = 1;
		}
	}
	else
	{
		++tmap_xscroll_index;
		scrl = 1;
	}

	if (scrl)
		TMap_scrollX(&tmap, tmap_xscroll_tbl[tmap_xscroll_index], 4);
}

void TMapScrollY_cond(int y, int speed)
{
	int scrl;

	scrl = 0;
	if ((sprt_player.y0 - prog.screen_y - 56) <= speed || tmap_yscroll_tbl[0] <= tmap_yscroll_index)
	{
		if ((sprt_player.y0 - prog.screen_y - 56) < y && tmap_yscroll_index > 1)
		{
			--tmap_yscroll_index;
			scrl = 1;
		}
	}
	else
	{
		++tmap_yscroll_index;
		scrl = 1;
	}

	if (scrl)
		TMap_scrollY(&tmap, tmap_yscroll_tbl[tmap_yscroll_index], 2);
}

void SetScrollBlock(int x, int y)
{
	int v2;
	int v3;
	int i;
	int j;

	v2 = tmap_xscroll_tbl[0];
	for (i = tmap_xscroll_tbl[0]; i; v2 = i--)
	{
		if (tmap_xscroll_tbl[i] < x)
			break;
	}

	if (v2 == i)
		printf("SetScrollBlock");
	else
	{
		tmap_xscroll_index = i + 1;
		v3 = tmap_yscroll_tbl[0];
		for (j = tmap_yscroll_tbl[0]; j; v3 = j--)
		{
			if (tmap_yscroll_tbl[j] < y)
				break;
		}
		if (v3 == j)
			printf("SetScrollBlock");
		else
			tmap_yscroll_index = v3;
	}
}

int TMapGetSize(TMAP* tmap, CRect* lprc)
{
	if (!tmap->w || !tmap->h)
		return 0;
	setRect(lprc, 0, 0, tmap->w - 1, tmap->h - 1);
	return 1;
}

void TMapSetScrolling(TMAP* tmap, int x, int y)
{
	tmap->scrlx = x;
	tmap->x1 = x;
	tmap->scrly = y;
	tmap->y1 = y;
	tmap->speedX = 0;
	tmap->speedY = 0;
}

void TMapSetClipArea(TMAP* tmap, int x, int y, int w, int h)
{
	setRect(&tmap->clip, x, y, w + x - 1, y + h - 1);
}

void TMapGetDstRect(TMAP* tmap, CRect* dst)
{
	setRect(dst, tmap->x1, tmap->y1, tmap->clip.X1() + tmap->x1 - tmap->clip.X0(), tmap->clip.Y1() + tmap->y1 - tmap->clip.Y0());
}

void SetWorldPos(int x, int y)
{
	CRect size, dest;

	TMapSetScrolling(&tmap, x, y);
	prog.screen_x = x;
	prog.screen_y = y;
	if (TMapGetSize(&tmap, &size))
	{
		if (prog.vm_func == 1)
		{
			TMapSetClipArea(&tmap, WINDOW_X * 2, WINDOW_Y * 2, WINDOW_W * 2, WINDOW_H * 2);
			TMapGetDstRect(&tmap, &dest);
			SetScrollBlock(x, y);
		}
	}
}

void Vm_scroll_world(int wx, int wy, int ww, int wh, int xpos, int ypos)
{
	TMapRenderScroll(&tmap, wx, wy, ww, wh, xpos, ypos);
}

void TMap_scroller()
{
	int xy[2];

	TMapScrollUpdate(&tmap);
	TMap_GetXY1(&tmap, xy);
	prog.screen_x = xy[0];
	prog.screen_y = xy[1];
	if ((prog.map_attr & 1) != 0)
	{
		if (TMapCanXScrol(&tmap))
			TMapScrollX_cond(180, 300);
		if (!TMapIsSpeedXSet(&tmap))
			TMapScrollX_cond(120, 360);
	}
	if ((prog.map_attr & 2) != 0 && !TMapIsSpeedYSet(&tmap))
		TMapScrollY_cond(38, 266);
}

void TMap_scroller2()
{
	if (vm_data.vm_index5[46])
	{
		int xspd, spd;
		++prog.scroll_timer;
		if ((prog.scroll_timer & 2) != 0)
			xspd = -2;
		else
			xspd = 2;
		spd = prog.screen_y + xspd;
		if (spd < 0)
			spd = 0;
		TMap_SetXY1(&tmap, prog.screen_x, spd);
	}
}
