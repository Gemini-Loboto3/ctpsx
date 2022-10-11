#ifndef _WINDOWS
#include <stdafx.h>

bool intersectRect(const RECT* r1, const RECT* r2)
{
	return !(r2->left > r1->right ||
		r2->right < r1->left ||
		r2->top > r1->bottom ||
		r2->bottom < r1->top);
}

void setRect(RECT* r, int x0, int y0, int x1, int y1)
{
	r->left = x0;
	r->right = x1;
	r->top = y0;
	r->bottom = y1;
}

void offsetRect(RECT* r, int x, int y)
{
	r->left += x;
	r->right += x;
	r->top += y;
	r->bottom += y;
}

#endif
