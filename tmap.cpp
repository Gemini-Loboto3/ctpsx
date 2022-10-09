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

void TMap_SetXY1(TMAP* a1, int x, int y)
{
	a1->x1 = x;
	a1->y1 = y;
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

void TMapGetRect(TMAP* tmap, RECT* lprcDst)
{
	CopyRect(lprcDst, &tmap->clip);
}

void TMapRenderScroll(TMAP* s, int x0, int y0, int w, int h, int x1, int y1)
{
	SetRect(&s->clip, x0, y0, w + x0 - 1, h + y0 - 1);
	s->x1 = x1;
	s->y1 = y1;
	TMapRender(s);
}

void TMapRenderScroll2(TMAP* s, int x, int y, int w, int h)
{
	SetRect(&s->clip, x, y, w + x - 1, h + y - 1);
	TMapRender(s);
}

void TMapRender(TMAP* tmap)
{
	if (tmap->tim == nullptr)
		return;

	RECT rsrc;
	RECT rdst;

	TMapGetDstRect(tmap, &rdst);
	if (TMapGetSize(tmap, &rsrc))
	{
		if (IntersectRect(&rdst, &rdst, &rsrc))
		{
			int x, y;
			
			if (tmap->scroll)
			{
				x = rdst.left;// tmap->clip.left;
				y = rdst.top; // tmap->clip.top;
				//TMapRenderScroll_0(tmap, tmap->clip.left, tmap->clip.top,
				//	rdst.right - rdst.left + 1, rdst.bottom - rdst.top + 1,
				//	rdst.left, rdst.top);
			}
			else
			{
				x = tmap->clip.left + (tmap->x1 - rdst.left);
				y = tmap->clip.top  + (tmap->y1 - rdst.top);
				//if (tmap->data->frame)
				//	frame = tmap->data->frame;
				//else
				//	frame = 1 << tmap->data->frame_bit;
				//RenderToOffScreen(tmap->clip.left - (tmap->x1 - rdst.left), tmap->clip.top - (tmap->y1 - rdst.top),
				//	rdst.left, rdst.top,
				//	tmap->w, tmap->h,
				//	(tmap->data->w + 3) & ~3u,
				//	rdst.right - rdst.left + 1,
				//	rdst.bottom - rdst.top + 1,
				//	(BYTE*)&tmap->data->ptr[frame],
				//	0, 0, 0);
			}

			//x = tmap->x1;
			//y = tmap->y1;
			int u = rdst.left / 2;
			int v = rdst.top / 2;
			int w = (rdst.right - rdst.left) / 2;
			int h = (rdst.bottom - rdst.top) / 2;

			RenderRect(tmap->tim, GETX(x), GETY(y), w, h, u, v, 0xff, 0xff, 0xff);
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
	CHAR path_map[MAX_PATH];
	CHAR path_dib[MAX_PATH];
	LPCSTR map;
	LPCSTR dib;

	if (dib_name)
	{
		sprintf_s(path_dib, MAX_PATH, "%s%s", prog.path, dib_name);
		dib = path_dib;
	}
	else dib = nullptr;

	if (map_name)
	{
		sprintf_s(path_map, MAX_PATH, "%s%s", prog.path, map_name);
		map = path_map;
	}
	else map = nullptr;

	TMapOpenAll(&tmap, map, map);
}

void Vm_map_scroll(__int16 scrlx, __int16 scrly, __int16 speed)
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

int dword_42557C, dword_425580;
__int16 word_425584[16];
__int16 word_4255A4[16];

void Vm_tmap_set_scroll(int cnt1, WORD* data1, int cnt2, WORD* data2)
{
	word_425584[0] = cnt1;
	for (int i = 1; i < 16; ++i)
		word_425584[i] = data1[i - 1];
	word_4255A4[0] = cnt2;
	for (int i = 1; i < 16; ++i)
		word_4255A4[i] = data2[i - 1];
}

void TMapScrollX_cond(int x, int speed)
{
	int scrl;

	scrl = 0;
	if (sprt_ent[0].x0 - prog.screen_x <= speed || word_425584[0] <= dword_42557C)
	{
		if (sprt_ent[0].x0 - prog.screen_x < x && dword_42557C > 1)
		{
			--dword_42557C;
			scrl = 1;
		}
	}
	else
	{
		++dword_42557C;
		scrl = 1;
	}

	if (scrl)
		TMap_scrollX(&tmap, word_425584[dword_42557C], 4);
}

void TMapScrollY_cond(int y, int speed)
{
	int scrl;

	scrl = 0;
	if ((sprt_ent[0].y0 - prog.screen_y - 56) <= speed || word_4255A4[0] <= dword_425580)
	{
		if ((sprt_ent[0].y0 - prog.screen_y - 56) < y && dword_425580 > 1)
		{
			--dword_425580;
			scrl = 1;
		}
	}
	else
	{
		++dword_425580;
		scrl = 1;
	}

	if (scrl)
		TMap_scrollY(&tmap, word_4255A4[dword_425580], 2);
}

void SetScrollBlock(int a1, int a2)
{
	int v2;
	int v3;
	int i;
	int j;

	v2 = word_425584[0];
	for (i = word_425584[0]; i; v2 = i--)
	{
		if (word_425584[i] < a1)
			break;
	}

	if (v2 == i)
		printf("SetScrollBlock");
	else
	{
		dword_42557C = i + 1;
		v3 = word_4255A4[0];
		for (j = word_4255A4[0]; j; v3 = j--)
		{
			if (word_4255A4[j] < a2)
				break;
		}
		if (v3 == j)
			printf("SetScrollBlock");
		else
			dword_425580 = v3;
	}
}

int TMapGetSize(TMAP* a1, RECT* lprc)
{
	if (!a1->w || !a1->h)
		return 0;
	SetRect(lprc, 0, 0, a1->w - 1, a1->h - 1);
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

void TMapSetClipArea(TMAP* tmap, int xLeft, int yTop, int a4, int a5)
{
	SetRect(&tmap->clip, xLeft, yTop, a4 + xLeft - 1, a5 + yTop - 1);
}

void TMapGetDstRect(TMAP* a1, RECT* dst)
{
	SetRect(dst, a1->x1, a1->y1, a1->clip.right + a1->x1 - a1->clip.left, a1->clip.bottom + a1->y1 - a1->clip.top);
}

void SetWorldPos(int x, int y)
{
	RECT v2;
	RECT rc;

	TMapSetScrolling(&tmap, x, y);
	prog.screen_x = x;
	prog.screen_y = y;
	if (TMapGetSize(&tmap, &v2))
	{
		if (prog.vm_func == 1)
		{
			TMapSetClipArea(&tmap, 40 * 2, 0, 240 * 2, 152 * 2);
			TMapGetDstRect(&tmap, &rc);
			SetScrollBlock(x, y);
		}
	}
}

void Vm_scroll_world(int xLeft, int yTop, int a3, int a4, int a5, int a6)
{
	TMapRenderScroll(&tmap, xLeft, yTop, a3, a4, a5, a6);
}

void TMap_scroller()
{
	int xy[2]; // [esp+0h] [ebp-8h] BYREF

	TMapScrollUpdate(&tmap);
	TMap_GetXY1(&tmap, xy);
	prog.screen_x = xy[0];
	prog.screen_y = xy[1];
	if ((prog.field_132 & 1) != 0)
	{
		if (TMapCanXScrol(&tmap))
			TMapScrollX_cond(180, 300);
		if (!TMapIsSpeedXSet(&tmap))
			TMapScrollX_cond(120, 360);
	}
	if ((prog.field_132 & 2) != 0 && !TMapIsSpeedYSet(&tmap))
		TMapScrollY_cond(38, 266);
}

void sub_4036C5()
{
	if (vm_index5[46])
	{
		int xspd, spd;
		++prog.field_196;
		if ((prog.field_196 & 2) != 0)
			xspd = -2;
		else
			xspd = 2;
		spd = prog.screen_y + xspd;
		if (spd < 0)
			spd = 0;
		TMap_SetXY1(&tmap, prog.screen_x, spd);
	}
}
