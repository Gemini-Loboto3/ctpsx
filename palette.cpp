#include <stdafx.h>
#include "game.h"

PAL_OBJ::PAL_OBJ()
{
	init();
	//vtbl
	f4091AD(0);
}

void PAL_OBJ::init()
{
	//vtbl = (PAL_OBJ_VTBL*)off_41BE48;
	//f409910();
	field_4 = 0;
	pal.palVersion = 768;
	pal.palNumEntries = 256;
	pal.palVersion = 768;
	pal_aux[0].palVersion = 768;
	pal_aux[0].palNumEntries = 256;
	pal_aux[1].palVersion = 768;
	pal_aux[1].palNumEntries = 256;
	pal_aux[2].palVersion = 768;
	pal_aux[2].palNumEntries = 256;
	count = 0;
	id = 0;
	reset();
	//if (f409983((SIZE_T*)&a1->buf, 1080u))
	{
		//hPal = make_sys_pal();
		field_4 = 1;
	}
}

void PAL_OBJ::make_sys_pal()
{
}

void PAL_OBJ::update_from_wnd()
{
}

void PAL_OBJ::reset()
{
	type = FADE_NONE;
	pal_reset(10, 245);
}

int PAL_OBJ::is_fading()
{
	return type != FADE_IN && type != FADE_OUT;
}

void PAL_OBJ::set_fade(int type, int index, int count, int delta, WORD id)
{
	if (id < 3 || id == 0xFFFF)
	{
		type = type;
		nindex = index;
		ncount = count - index + 1;
		fade_delta = delta;
		fade = 0;
		if (id != 0xFFFF)
			this->id = id;
	}
}

int PAL_OBJ::fade_in()
{
	fade += fade_delta;
	if (fade >= 255)
		fade = 255;

	return fade == 255;
}

void PAL_OBJ::restart()
{
	if (fade_delta)
	{
		if (fade_out())
			type = FADE_NONE;
	}
	else
	{
		pal_reset(nindex, ncount);
		type = FADE_NONE;
	}
	//AnimatePalette(a1->hPal, a1->nindex, a1->ncount, &a1->pal.palPalEntry[a1->nindex]);
	index = nindex;
	count = ncount;
}

int PAL_OBJ::fade_out()
{
	fade += fade_delta;
	if (fade >= 255)
		fade = 255;

	return fade == 255;
}

void PAL_OBJ::update_dib_pal()
{
	RGBQUAD pal[256];

	if (count)
	{
		for (int i = 0; i < count; ++i)
		{
			pal[i].rgbRed = this->pal.palPalEntry[i + index].peRed;
			pal[i].rgbGreen = this->pal.palPalEntry[i + index].peGreen;
			pal[i].rgbBlue = this->pal.palPalEntry[i + index].peBlue;
			pal[i].rgbReserved = 0;
		}
		//SetDIBColorTable(hdc, p->index, p->count, pal);
		count = 0;
	}
}

void PAL_OBJ::pal_reset(int base, int cnt)
{
	for (int i = base; cnt + base > i; ++i)
	{
		pal.palPalEntry[i].peRed = 0;
		pal.palPalEntry[i].peGreen = 0;
		pal.palPalEntry[i].peBlue = 0;
	}
}

void PAL_OBJ::f4091AD(int a1)
{
	field_1044 = a1;
}

void PAL_OBJ::call_fade()
{
	switch (type)
	{
	case FADE_IN:
		set_animate();
		break;
	case FADE_OUT:
		update();
		break;
	}
}

void PAL_OBJ::set_animate()
{
	if (fade_delta)
	{
		if (fade_in())
			type = FADE_DONE;
	}
	else
	{
		//memcpy(&pal.palPalEntry[nindex], &pal_aux[id].palPalEntry[nindex], 4 * ncount);
		type = FADE_DONE;
	}

	index = nindex;
	count = ncount;
}

void PAL_OBJ::update()
{
	if (fade_delta)
	{
		if (fade_out())
			type = FADE_NONE;
	}
	else
	{
		//reset_palette();
		type = FADE_NONE;
	}

	index = nindex;
	count = ncount;
}
