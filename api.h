#pragma once
#ifndef _WINDOWS

typedef struct tagRect
{
	int left,
		right,
		top,
		bottom;
} RECT;

bool intersectRect(RECT* dst, const RECT* r1, const RECT* r2);
void setRect(RECT* r, int x0, int y0, int x1, int y1);
void offsetRect(RECT* r, int x, int y);

#define IntersectRect	intersectRect
#define SetRect			setRect
#define OffsetRect		offsetRect

#else

bool ShowCursor(bool show);

#endif
