#pragma once

class CTim;

#ifdef _WINDOWS
#include <d3d9.h>
#include "win\render_d3d9.h"
#endif

class CTim
{
public:
	CTim() { pixel = nullptr, clut = nullptr; }
	~CTim();

	size_t Open(BYTE* buffer);
	size_t Open(const char* filename);
	void Create(int w, int h, int bpp, int pals);
	void Release();

	void Copy(CTim* src, int u, int v, int w, int h);

	typedef struct tagTimHeader
	{
		DWORD magic;
		DWORD attr;
	} TIM_HEADER;

	typedef struct tagTimChunk
	{
		DWORD nsize;	// size of the chunk + data
		WORD x, y,
			w, h;
	} TIM_CHUNK;

	WORD* clut;
	BYTE* pixel;
	int pal_w = 0, pal_h = 0;
	int pix_w = 0, pix_h = 0;
	int bpp = 0;

	int is_ref = 0;
	int real_w = 0;
#ifdef _WINDOWS
	LPDIRECT3DTEXTURE9 tex = nullptr;

	void SetTexture();
	static DWORD torgb888(WORD c)
	{
		if (c == 0)
			return D3DCOLOR_ARGB(0, 0, 0, 0);

		BYTE r = c & 0x1f;
		BYTE g = (c >> 5) & 0x1f;
		BYTE b = (c >> 10) & 0x1f;
		BYTE a = c & 0x8000 ? 0 : 255;

		r = (r << 3) | (r >> 2);
		g = (g << 3) | (g >> 2);
		b = (b << 3) | (b >> 2);

		return D3DCOLOR_ARGB(a, r, g, b);
	}
#endif
};
