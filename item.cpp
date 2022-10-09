#include <stdafx.h>
#include "game.h"

int itemptr2_index;
WORD inventory[15];
CTim* item_tim;

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
	"BLACK", 12,	// black robe
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
	item_tim = new CTim();
	item_tim->Open("ITEM\\ITEM.TIM");

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
		if (item_lut[i] >= 0 && vm_index5[i + 8])
			inventory[item_lut[i]] = i;
	}
	if (inventory[itemptr2_index] == -1)
		itemptr2_index = 0;
	vm_index5[24] = inventory[itemptr2_index];
}

void ItemListDisp()
{
	static short xy[][2] =
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

	ItemUpdate();
	for (int i = 0; i < 15; ++i)
	{
		if (inventory[i] == 0xffff)
			continue;
		int x = xy[i][0];
		int y = xy[i][1];

		int u = item_tbl[item_lut2[inventory[i]]].id % 8 * 16;
		int v = item_tbl[item_lut2[inventory[i]]].id / 8 * 16;
		RenderRect(item_tim, x, y, 16, 16, u, v, 0xff, 0xff, 0xff);

		// selected
		if (i == itemptr2_index)
		{
			u = 7 % 8 * 16;
			v = 7 / 8 * 16;
			RenderRect(item_tim, x, y, 16, 16, u, v, 0xff, 0xff, 0xff);
		}
	}
}

void ItemRender()
{
	prog.field_148 = 1;
	if (prog.field_13C)
	{
		ItemListDisp();
		prog.field_13C = 0;
		prog.field_148 = 1;
	}
}