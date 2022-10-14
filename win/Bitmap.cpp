#include <stdafx.h>
#include "Bitmap.h"
#include <assert.h>
#include "lodepng.h"

#define FLIP_ALPHA		0

static DWORD ToRGB888_psx(WORD c)
{
	int r, g, b, a;

	if (c == 0x8000)
	{
		r = 1;
		g = 0;
		b = 0;
	}
	else
	{
		r = c & 0x1f; r <<= 3;
		g = (c >> 5) & 0x1f; g <<= 3;
		b = (c >> 10) & 0x1f; b <<= 3;
	}
#if FLIP_ALPHA
	a = c == 0 ? 0xff : 0;
#else
	a = c == 0 ? 0 : 0xff;
#endif

	return (r << 16) | (g << 8) | b | (a << 24);
}

static DWORD ToRGB888_full(WORD c)
{
	int r, g, b, a;

	if (c == 0x8000)
	{
		r = 1;
		g = 0;
		b = 0;
	}
	else
	{
		r = c & 0x1f;         r = (r << 3) | (r >> 2);
		g = (c >> 5) & 0x1f;  g = (g << 3) | (g >> 2);
		b = (c >> 10) & 0x1f; b = (b << 3) | (b >> 2);
	}
#if FLIP_ALPHA
	a = c == 0 ? 0xff : 0;
#else
	a = c == 0 ? 0 : 0xff;
#endif

	return (r << 16) | (g << 8) | b | (a << 24);
}

static DWORD(*fcolor[])(WORD color) =
{
	ToRGB888_psx,
	ToRGB888_full,
};

static DWORD(*fcolorfnc)(WORD color);

//////////////////////////////
// bitmap
void CBitmap::Release()
{
	Trim();
	w = h = 0;
}

void CBitmap::CreateFromPng(BYTE *data, size_t size)
{
	BYTE *out;
	size_t w, h;
	lodepng_decode32(&out, &w, &h, data, size);
	CreateFromPng(out, w, h);
	free(out);
}

void CBitmap::CreateFromPng(BYTE *data, size_t w, size_t h)
{
	Allocate(w, h);
	for (size_t i = 0, si = w * h; i < si; i++, data += 4)
	{
		DWORD p = (data[0] << 16) |	// red
			(data[1] << 8) |		// green
			data[2] |				// blue
			(data[3] << 24);		// alpha

		*pDataSeek++ = p;
	}
	pDataSeek = pData;
}

void CBitmap::Create(size_t w, size_t h)
{
	Allocate(w, h);
}

void CBitmap::Allocate(size_t w, size_t h)
{
	Release();

	this->w = w;
	this->h = h;
	pData = new DWORD[w*h];
	pDataSeek = pData;
}

void CBitmap::Trim()
{
	if (pData != nullptr)
		delete[] pData;
	pData = nullptr;
}

void CBitmap::Blt(CBitmap &src, int src_x, int src_y, int dst_x, int dst_y, int dst_w, int dst_h)
{
	for (int y = 0; y < dst_h; y++)
		for (int x = 0; x < dst_w; x++)
			this->setPixel(dst_x + x, dst_y + y, src.getPixel(src_x + x, src_y + y));
}

void CBitmap::SavePng(LPCSTR name)
{
	DWORD *image = new DWORD[this->w * this->h], *out = image;

	for (size_t y = 0; y < this->h; y++)
	{
		for (size_t x = 0; x < this->w; x++)
		{
			DWORD p = this->getPixel(x, y);
			*out++ = (((p & 0xff) << 16) | ((p & 0xff0000) >> 16) | (p & 0xff00ff00));
		}
	}

	lodepng::encode(name, (BYTE*)image, this->w, this->h, LCT_RGBA);

	delete[] image;
}

//////////////////////////////////////////
void Convert_clut(WORD *clut, DWORD *dst, size_t count)
{
	for (size_t i = 0; i < count; i++)
		dst[i] = fcolorfnc(clut[i]);
}

DWORD Convert_clut(WORD clut)
{
	return fcolorfnc(clut);
}

void Set_color_mode(int mode)
{
	fcolorfnc = fcolor[mode];
}
