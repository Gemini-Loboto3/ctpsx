#pragma once
#ifndef _WINDOWS

typedef const char* LPCSTR, *PCSTR;
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;

typedef struct tagPcRect
{
	int left,
		right,
		top,
		bottom;
} PC_RECT;

#else

typedef RECT PC_RECT;

#endif

class CRect : public PC_RECT
{
public:
	int W() { return right - left; }
	int H() { return bottom - top; }
};

bool intersectRect(const PC_RECT* r1, const PC_RECT* r2);
void setRect(PC_RECT* r, int x0, int y0, int x1, int y1);
void copyRect(PC_RECT* dst, PC_RECT* src);
void offsetRect(PC_RECT* r, int x, int y);

#define SetRect			setRect
#define OffsetRect		offsetRect

bool showCursor(bool show);
