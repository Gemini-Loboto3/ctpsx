#include <stdafx.h>

bool IntersectRect(const RECT* r1, const RECT* r2)
{
	return !(r2->left > r1->right ||
		r2->right < r1->left ||
		r2->top > r1->bottom ||
		r2->bottom < r1->top);
}
