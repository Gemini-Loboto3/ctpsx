#include <stdafx.h>
#include "game.h"

int itemptr2_index;
WORD inventory[15];
short item_xy_tbl[][2] =
{
	64, 200,
	80, 200,
	96, 200,
	112, 200,
	128, 200,
	144, 200,
	160, 200,
	176, 200,
	192, 200,
	208, 200,
	64, 216,
	80, 216,
	96, 216,
	112, 216,
	128, 216
};
CTexture* item_tim;

short item_lut[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, -1, -1, -1, 11, -1, 11 };
short item_lut2[] =
{
	1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  -1,  -1,  -1,  11,  -1,  12,
	13,  88, 100,  76,  28, 100,  23,   4,   6,   4,  20,  23,   6,  38,  11,  13,
	5,  51,  51,  40,  32, 100,   1, 100, 100, 100, 100, 100, 100, 100, 100, 100,
	100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
};

typedef struct ITEM_DATA
{
	LPCSTR name;
	int id;
} ITEM_DATA;

ITEM_DATA item_tbl[] =
{
	"CUR_09", 0,	// empty slot
	"KOUSUI", 8,	// cologne
	"NIKU", 9,		// beef
	"ENGINE", 10,	// car key
	"GASS", 11,		// insecticide
	"BLACK", 12,	// black robes
	"ROPE", 13,		// rope
	"ISI", 15,		// clothing
	"KAGOKEY", 15,	// some key
	"CAPPKEY", 16,	// cupboard key
	"LANTAN", 17,	// no idea, it's a key
	"TUE", 21,		// staff
	"MAZOU", 23,	// evil statue
	"CUR_08", 18,	// dummy
	"CUR_08", 18,	// dummy
	"CUR_08", 18,	// dummy
	"CUR_08", 18	// dummy
};

void InitItem()
{
	CTim tim;
	tim.Open("ITEM\\ITEM.TIM");

	item_tim = MakeTexture();
	item_tim->Create(tim.clut, tim.pixel, tim.bpp, tim.real_w, tim.pix_h);

	itemptr2_index = 0;
	inventory[0] = 16;
	for (int i = 1; i < 15; i++)
		inventory[i] = -1;
}

void ItemUpdate()
{
	for (int i = 1; i < 15; ++i)
		inventory[i] = -1;
	for (int i = 0; i < 16; ++i)
	{
		if (item_lut[i] >= 0 && vm_data.vm_index5[i + 8])
			inventory[item_lut[i]] = i;
	}
	if (inventory[itemptr2_index] == -1)
		itemptr2_index = 0;
	vm_data.vm_index5[24] = inventory[itemptr2_index];
}

void DispCursor()
{
	RenderRect(item_tim, (mouseX - 80) / 2, mouseY / 2, 16, 16, 0, 0, 0xff, 0xff, 0xff);
}

void ItemListDisp()
{
	ItemUpdate();
	for (int i = 0; i < 15; ++i)
	{
		if (inventory[i] == 0xffff)
			continue;
		int x = item_xy_tbl[i][0];
		int y = item_xy_tbl[i][1];

		int u = item_tbl[item_lut2[inventory[i]]].id % 8 * 16;
		int v = item_tbl[item_lut2[inventory[i]]].id / 8 * 16;

		// selected
		if (i == itemptr2_index)
		{
			u = 7 % 8 * 16;
			v = 7 / 8 * 16;
			RenderRect(item_tim, x, y, 16, 16, u, v, 0xff, 0xff, 0xff);
			RenderRect(item_tim, x, y, 16, 16, u, v, 0xff, 0x80, 0x80);
		}
		RenderRect(item_tim, x, y, 16, 16, u, v, 0xff, 0xff, 0xff);
	}
}

void ItemRender()
{
	prog.field_148 = 1;
	if (prog.click_on_item)
	{
		ItemListDisp();
		prog.click_on_item = 0;
		prog.field_148 = 1;
	}
}