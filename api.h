#pragma once
#ifndef _WINDOWS

typedef const char* LPCSTR, *PCSTR;
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;

#endif

class CRect
{
	int right, left,
		top, bottom;

public:
	int X0() { return left; }
	int X1() { return right; }
	int Y0() { return top; }
	int Y1() { return bottom; }
	int W() { return right - left; }
	int H() { return bottom - top; }

	void SetXYWH(int x, int y, int w, int h)
	{
		left = x;
		right = x + w;
		top = y;
		bottom = y + h;
	}

	void Set(int _left, int _right, int _top, int _bottom)
	{
		left = _left;
		right = _right;
		top = _top;
		bottom = _bottom;
	}

	void Copy(const CRect* src)
	{
		left = src->left;
		right = src->right;
		top = src->top;
		bottom = src->bottom;
	}

	void Offset(int _x, int _y)
	{
		left += _x;
		right += _x;
		top += _y;
		bottom += _y;
	}

	void SwapX() { std::swap(left, right); }
	void SwapY() { std::swap(top, bottom); }
};

bool intersectRect(CRect* r1, CRect* r2);
void setRect(CRect* r, int x0, int y0, int x1, int y1);
void copyRect(CRect* dst, CRect* src);
void offsetRect(CRect* r, int x, int y);

void rectSwapX(CRect* r);

bool showCursor(bool show);
