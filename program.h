struct PROGRAM
{
	PROGRAM()
	{
		srand(_statcvt_i64(0));
		field_120 = 0;
		vm_func = 0;
		field_128 = 0;
		field_190 = 0;
		tick_now = 0;
		tick_delta = 30;
		render_rect.left = 80;
		render_rect.top = 0;
		render_rect.right = 559;
		render_rect.bottom = 303;
		field_178 = 144;
		field_17C = 320;
		field_180 = 607;
		field_184 = 431;
		//sprt = 0;
		screen_on = 0;
		map_attr = 0;
		field_134 = 0;
		field_14C = 0;
		ShowCursor(0);
		cur_type0 = ShowCursor(1);
		//WaitCursor(p, 0);
		cur_type1 = 0;
		field_1A4 = 1;
		click_bits = 0;
		field_1B6 = 0;
		field_1B8 = 0;
		field_1BC = 0;
		tick = (int)getTime();
		key_is_down = 0;
		field_1C8 = 0;
		field_1CA = 0;
		field_1CC = 0;
		field_1D0 = 0;
		for (int i = 0; i < 9; ++i)
			field_1D4[i] = 0;
		//++*game_alloc_cnt;
	}

	int field_0;
	LPSTR CmdLine;
	HINSTANCE hInst;
	HWND hWnd;
	LPWSTR mCaption;
	LPWSTR tCaption;
	char path[260];
	int no_reg;
	int field_120;
	int vm_func;
	__int16 field_128;
	DWORD screen_on;
	__int16 field_12E;
	__int16 field_130;
	WORD map_attr;
	int field_134;
	int field_138;
	int field_13C;
	int field_140;
	int field_144;
	int field_148;
	int field_14C;
	VM* vm;
	SPRT_ENT* sprt;
	int field_158;
	int field_15C;
	int screen_x;
	int screen_y;
	RECT render_rect;
	int field_178;
	int field_17C;
	int field_180;
	int field_184;
	int tick_delta;
	DWORD tick_now;
	int field_190;
	WORD field_194;
	WORD field_196;
	int field_198;
	short field_19C;
	__int16 field_19E;
	short field_1A0;
	short field_1A2;
	int field_1A4;
	int field_1A8;
	int field_1AC;
	__int16 cur_type0;
	__int16 cur_type1;
	__int16 click_bits;
	__int16 field_1B6;
	int field_1B8;
	int field_1BC;
	int tick;
	int key_is_down;
	__int16 field_1C8;
	__int16 field_1CA;
	int field_1CC;
	int field_1D0;
	WORD field_1D4[9];
	WORD field_1E6[18];
	PAL_OBJ pal_obj;
	RENDER_BMP render_bmp;
};

extern PROGRAM prog;
