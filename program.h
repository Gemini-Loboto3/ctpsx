#pragma once

struct PROGRAM
{
	PROGRAM()
	{
		memset(this, 0, sizeof(*this));

		_srand(_statcvt_i64());
		game_stopped = 0;
		vm_func = 0;
		no_exec = 0;
		field_190 = 0;
		tick_now = 0;
		tick_delta = 30;
		render_rect.Set(80, 559, 0, 303);
		menu_rect.Set(144, 607, 320, 431);
		//sprt = 0;
		screen_on = 0;
		map_attr = 0;
		trigger_debug = 1; //0;
		curs_mode = 0;
		showCursor(0);
		cur_type0 = showCursor(1);
		cur_type1 = 0;
		cur_enabled = 1;
		click_bits = 0;
		click_old = 0;
		mousePT.x = 0;
		mousePT.y = 0;
		tick = (int)getTime();
		key_is_down = 0;
		in_key = 0;
		in_repeat = 0;
		field_1CC = 0;
		field_1D0 = 0;
		for (int i = 0; i < 9; ++i)
			field_1D4[i] = 0;
		//++*game_alloc_cnt;
	}

#ifdef _WINDOWS
	LPSTR CmdLine;
	HINSTANCE hInst;
	HWND hWnd;
	LPWSTR mCaption;
	LPWSTR tCaption;
	char path[260];
	int no_reg;
#endif
	int game_stopped;
	int vm_func;
	__int16 no_exec;
	DWORD screen_on;
	__int16 can_lclick;
	__int16 can_rclick;
	WORD map_attr;
	int trigger_debug;
	int field_138;
	int click_on_item;
	int field_140;
	int field_144;
	int field_148;
	int curs_mode;
	VM* vm;
	SPRT_ENT* sprt;
	int field_158;
	int field_15C;
	int screen_x;
	int screen_y;
	CRect render_rect,
		menu_rect;
	int tick_delta;
	DWORD tick_now;
	int field_190;
	WORD fade_timer;
	WORD scroll_timer;
	int slant_on;
	short slant_x,
		slant_y,
		slant_w,
		slant_h;
	int cur_enabled;
	int triggerX;
	int triggerY;
	__int16 cur_type0;
	__int16 cur_type1;
	__int16 click_bits;
	__int16 click_old;
	POINT mousePT;
	int tick;
	int key_is_down;
	__int16 in_key;
	__int16 in_repeat;
	int field_1CC;
	int field_1D0;
	WORD field_1D4[9];
	WORD field_1E6[18];
	PAL_OBJ pal_obj;
	RENDER_BMP render_bmp;
};

extern PROGRAM prog;
