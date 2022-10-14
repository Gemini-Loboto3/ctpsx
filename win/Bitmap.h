#pragma once
#include <vector>

class CBitmap
{
public:
	CBitmap() { pData = nullptr; w = h = 0; }
	~CBitmap() { Trim(); }

	__inline void setPixel(size_t x, size_t y, DWORD p)
	{
		// check boundaries
		if (x >= w || y >= h)
			return;

		pData[w * y + x] = p;
	}

	__inline DWORD getPixel(size_t x, size_t y)
	{
		// check boundaries
		if (x >= w || y >= h)
			return 0xff000000;

		return pData[w * y + x];
	}

	__inline DWORD getPixelO(size_t x, size_t y)
	{
		// check boundaries
		if (x < 0) x = 0;
		else if (x >= (int)w) x = w - 1;
		if (y < 0) y = 0;
		else if (y >= (int)h) y = h - 1;

		return pData[w * y + x] & 0xffffff;
	}

	__inline WORD getPixel16(size_t x, size_t y)
	{
		// check boundaries
		if (x >= w || y >= h)
			return 0x0000;

		BYTE *p = (BYTE*)&pData[w * y + x];

		return (p[0] >> 3) | ((p[1] >> 2) << 5) | ((p[2] >> 3) << 11);
	}

	__inline void fastPixel(DWORD p)
	{
		*pDataSeek = p;
		pDataSeek++;
	}

	void TransBlt(CBitmap& src, int src_x, int src_y, int dst_x, int dst_y, int dst_w, int dst_h)
	{
		for (int y = 0; y < dst_h; y++)
		{
			for (int x = 0; x < dst_w; x++)
			{
				auto p = src.getPixel(src_x + x, src_y + y);
				if ((p & 0xff000000) == 0xff000000)
					this->setPixel(dst_x + x, dst_y + y, p);
			}
		}
	}

	void Allocate(size_t w, size_t h);
	void Release();
	void Trim();

	void   CreateFromPng(BYTE *data, size_t size);
	void   CreateFromPng(BYTE *data, size_t w, size_t h);
	void   Create(size_t w, size_t h);

	void   Blt(CBitmap &src, int src_x, int src_y, int dst_x, int dst_y, int dst_w, int dst_h);
	void   SavePng(LPCSTR name);

	bool   IsOpen() { return pData != nullptr ? true : false; }

	size_t w, h;
	DWORD *pData,
		*pDataSeek;

	typedef struct tagTimChunk
	{
		DWORD size;	// size of the whole chunk
		WORD x, y;	// vram coordinates
		WORD w, h;	// size in vram
	} TIM_CHUNK;
};

void Convert_clut(WORD *clut, DWORD *dst, size_t count);
DWORD Convert_clut(WORD clut);
void Set_color_mode(int mode);
void RenderDeposterize(CBitmap &Src, CBitmap &Dst);
