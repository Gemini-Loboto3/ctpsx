struct PAL_LOG256
{
	__int16 palVersion;
	__int16 palNumEntries;
	PALETTEENTRY palPalEntry[256];
};

struct PAL_OBJ
{
	PAL_OBJ();

	void init();
	void make_sys_pal();
	void update_from_wnd();
	void reset();
	int is_fading();
	void set_fade(int type, int index, int count, int delta, WORD id);
	void f404CDD();
	int fade_in();
	void restart();
	int fade_out();
	void update_dib_pal();
	void pal_reset(int base, int cnt);
	void load();
	void copy_sys_pal();

	void f4091AD(int a1);

	struct PAL_OBJ_VTBL
	{
		int(__cdecl* fnc0)(PAL_OBJ*);
		int(__cdecl* fnc1)(PAL_OBJ*);
	} *vtbl;

	int field_4;
	HPALETTE hPal;
	LPCVOID buf;
	BITMAP_DATA* bmp_info;
	int type;
	int nindex;
	int ncount;
	int fade_delta;
	int fade;
	PAL_LOG256 pal;
	int index;
	int count;
	int id;
	PAL_LOG256 pal_aux[3];
	__int16 field_1044;
	BYTE pal16[2][16];
	__int16 field_1066;
	int field_1068;
	int field_106C;
	int field_1070;
	__int16 field_1074;

	enum
	{
		FADE_NONE,
		FADE_IN,
		FADE_OUT
	};
};
