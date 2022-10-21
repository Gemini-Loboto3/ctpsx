#include <stdafx.h>
#include "game.h"

FONT_DATA fnt_obj;
std::vector<BYTE> fnt_data;

void InitFont()
{
	CFile fp;
	if (fp.Open("font.bin"))
	{
		size_t size = fp.GetSize();

		fnt_data = std::vector<BYTE>(size);
		fp.Read(fnt_data.data(), size);
		fp.Close();

		FontOpen(fnt_data.data(), &fnt_obj);
	}
}

#define align(x,y)		((x) + ((y) - 1)) & ~(y - 1)

int GamePrintChar(int x, int y, wchar_t glyph)
{
	auto g = FontGetGlyph(&fnt_obj, glyph);

	DWORD gfx[12 * 2];
	FontExpandGlyph(g, gfx);

	D3DCOLOR pal[4] =
	{
		D3DCOLOR_ARGB(0, 0, 0, 0),
		D3DCOLOR_ARGB(255, 248, 248, 248),
		D3DCOLOR_ARGB(255, 160, 160, 160),
		D3DCOLOR_ARGB(255, 96, 96, 96)
	};

	x /= 2;
	y /= 2;
	x -= TEXT_XDIFF;

	D3DLOCKED_RECT r;
	d3d9text->LockRect(0, &r, nullptr, 0);

	DWORD* dst = (DWORD*)r.pBits;
	size_t pitch = r.Pitch / 4;
	BYTE* src = (BYTE*)gfx;
	int w = align(g->w, 8);

	memset(dst, 0, r.Pitch * 12);

	for (int yi = 0; yi < 12; yi++)
	{
		DWORD* d = dst;
		for (int xi = 0; xi < w; xi += 2, src++)
		{
			BYTE b = *src & 0xf;
			if (b) d[0] = pal[b];
			b = *src >> 4;
			if (b) d[1] = pal[b];
			d += 2;
		}
		dst += pitch;
	}
	d3d9text->UnlockRect(0);

	// border
	RenderRect(d3d9text, x - 1, y + g->y, 12, 12, g->w, g->h, 0, 0, 0x40, 0x40, 0x40);
	RenderRect(d3d9text, x,     y + g->y - 1, 12, 12, g->w, g->h, 0, 0, 0x40, 0x40, 0x40);
	RenderRect(d3d9text, x + 1, y + g->y, 12, 12, g->w, g->h, 0, 0, 0x40, 0x40, 0x40);
	RenderRect(d3d9text, x,     y + g->y + 1, 12, 12, g->w, g->h, 0, 0, 0x40, 0x40, 0x40);
	// glyph
	RenderRect(d3d9text, x, y + g->y, 12, 12, g->w, g->h, 0, 0, (BYTE)prog.vm->texcol_r, (BYTE)prog.vm->texcol_g, (BYTE)prog.vm->texcol_b);

	return (g->w + 1) * 2;
}
