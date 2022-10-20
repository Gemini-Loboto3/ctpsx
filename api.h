#pragma once
#ifndef _WINDOWS

typedef const char* LPCSTR, *PCSTR;
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;

typedef struct tagRect
{
	int left,
		right,
		top,
		bottom;
} RECT;

#endif

bool intersectRect(const RECT* r1, const RECT* r2);
void setRect(RECT* r, int x0, int y0, int x1, int y1);
void copyRect(RECT* dst, RECT* src);
void offsetRect(RECT* r, int x, int y);

#define SetRect			setRect
#define OffsetRect		offsetRect

bool showCursor(bool show);
