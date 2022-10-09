#pragma once

struct TMAP
{
	TMAP();
	~TMAP();

	WORD tile_x;
	WORD tile_y;
	int tile_no;
	int scroll;
	int w;
	int h;
	RECT clip;
	int x1;
	int y1;
	int scrlx;
	int scrly;
	int speedX;
	int speedY;
	CTim* tim;
	void* map_data;
};

extern TMAP tmap;

void TMapResetTiles(TMAP* tmap);
void TMap_SetXY1(TMAP* tmap, int x, int y);
void TMapSetScrolling(TMAP* tmap, int x, int y);
void TMap_GetXY1(TMAP* tmap, int* dst);
int TMap_is_scrolling(TMAP* tmap);
int TMapIsSpeedXSet(TMAP* tmap);
int TMapIsSpeedYSet(TMAP* tmap);
int TMapCanXScrol(TMAP* tmap);
int TMapCanYScrol(TMAP* tmap);
void TMapScrollUpdate(TMAP* tmap);
int TMapCalcSpeed(TMAP* tmap, int pos, int scrl, signed int spd);
void TMap_scrollXY(TMAP* tmap, int x, int y, int speed);
void TMap_scrollX(TMAP* a1, int scrlx, int speed);
void TMap_scrollY(TMAP* a1, int scrly, int speed);
int TMapOpenAll(TMAP* tmap, LPCSTR filename_dib, LPCSTR filename_map);

void TMapCache(LPCSTR dib_name, LPCSTR map_name);

void TMapSetClipArea(TMAP* tmap, int xLeft, int yTop, int a4, int a5);
void TMapGetRect(TMAP* tmap, RECT* lprcDst);
void TMapGetDstRect(TMAP* a1, RECT* dst);
int TMapGetSize(TMAP* tmap, RECT* dst);
void TMapRenderScroll(TMAP* s, int x0, int y0, int w, int h, int x1, int y1);
void TMapRenderScroll2(TMAP* s, int x, int y, int w, int h);
void TMapRender(TMAP* tmap);

void SetWorldPos(int x, int y);

void Vm_map_scroll(__int16 scrlx, __int16 scrly, __int16 speed);
int Vm_bg_scrolling();
void Vm_tmap_set_scroll(int cnt1, WORD* data1, int cnt2, WORD* data2);
void Vm_scroll_world(int xLeft, int yTop, int a3, int a4, int a5, int a6);

void TMap_scroller();
void sub_4036C5();
