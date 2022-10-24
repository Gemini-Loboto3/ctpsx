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

int GamePrintChar(int x, int y, wchar_t glyph)
{
	auto g = FontGetGlyph(&fnt_obj, glyph);

	DWORD gfx[12 * 2];
	FontExpandGlyph(g, gfx);

	x /= 2;
	y /= 2;
	x -= TEXT_XDIFF;

	RenderGlyph(x, y, g, gfx);

	return (g->w + 1) * 2;
}
