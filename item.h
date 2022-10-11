#pragma once

extern int itemptr2_index;
extern WORD inventory[15];
extern short item_xy_tbl[][2];

void InitItem();
void ItemDeinit();
void ItemRender();
