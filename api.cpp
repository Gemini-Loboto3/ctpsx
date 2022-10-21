#include <stdafx.h>
#include "api.h"

bool intersectRect(const PC_RECT* r1, const PC_RECT* r2)
{
	return !(r2->left > r1->right ||
		r2->right < r1->left ||
		r2->top > r1->bottom ||
		r2->bottom < r1->top);
}

void setRect(PC_RECT* r, int x0, int y0, int x1, int y1)
{
	r->left = x0;
	r->right = x1;
	r->top = y0;
	r->bottom = y1;
}

void copyRect(PC_RECT* dst, PC_RECT* src)
{
	dst->left = src->left;
	dst->right = src->right;
	dst->top = src->top;
	dst->bottom = src->bottom;
}

void offsetRect(PC_RECT* r, int x, int y)
{
	r->left += x;
	r->right += x;
	r->top += y;
	r->bottom += y;
}

static int cursor_visible = 0;

bool showCursor(bool show)
{
	if (show) cursor_visible++;
	else cursor_visible = 0;

	return cursor_visible ? true : false;
}

