#include <stdafx.h>
#include "api.h"

bool intersectRect(CRect* r1, CRect* r2)
{
	return !(r2->X0() > r1->X1() ||
		r2->X1() < r1->X0() ||
		r2->Y0() > r1->Y1() ||
		r2->Y1() < r1->Y0());
}

void setRect(CRect* r, int x0, int y0, int x1, int y1)
{
	r->Set(x0, x1, y0, y1);
}

void copyRect(CRect* dst, CRect* src)
{
	dst->Copy(src);
}

void offsetRect(CRect* r, int x, int y)
{
	r->Offset(x, y);
}

static int cursor_visible = 0;

bool showCursor(bool show)
{
	if (show) cursor_visible++;
	else cursor_visible = 0;

	return cursor_visible ? true : false;
}

