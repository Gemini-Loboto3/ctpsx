#pragma once

struct SPRT_DAT
{
	DWORD type0;
	DWORD type;
	DWORD type3;
	DWORD type2;
	DWORD field_10;
	WORD field_14;
	WORD type4;
	WORD field_18;
	WORD field_1A;
	DWORD field_1C;
	DWORD field_20;
	WORD field_24;
};

struct SPRT_ENT
{
	void SetList();
	void Link();
	void Release();
	void UpdateXY();
	void Update();
	void SetXY(int x, int y, DWORD a4, int flip);
	void SetX0();

	DWORD flip;
	DWORD field_4;
	SPRT_ENT* next;
	SPRT_ENT* prev;
	DWORD field_10;
	WORD is_abs;
	char field_16;
	char field_17;
	char field_18;
	char field_19;
	BITMAPINFOHEADER* ptr0;
	DWORD field_1E;
	DWORD field_22;
	DWORD field_26;
	DWORD field_2A;
	DWORD field_2E;
	DWORD field_32;
	char bmp;
	DWORD field_37;
	WORD field_3B;
	DWORD flag0;
	DWORD field_41;
	DWORD field_45;
	int lmx;
	int lmy;
	DWORD x1;
	WORD x2;
	WORD y2;
	WORD flag2;
	CTim* tim;
	int x0;
	int y0;
	DWORD flag1;
	WORD field_6B;
	int x3;
	int y3;
	DWORD field_75;
	DWORD field_79;
	DWORD field_7D;
	DWORD field_81;
	DWORD field_85;
	DWORD field_89;
	DWORD field_8D;
	DWORD field_91;
	DWORD field_95;
	WORD field_99;
	WORD field_9B;
	WORD field_9D;
};

extern SPRT_ENT sprt_ent[21];
extern SPRT_DAT sprt_dat[2];

void Vm_spr_lmt(int a1, int lmx, int lmy);
void sub_404346(int a1, int a2);
void sub_402FAF();
void sub_403252();
void BgSprAnim(int id, __int16 w, __int16 h, BYTE* ptr);
void BgSprPos(int a1, __int16 x, __int16 y, __int16 flag);
void SprtTblDeinit();
void EntryBmpSprite(int id, __int16 x, __int16 y, __int16 flag, __int16 w, __int16 h, CTim* ptr, DWORD a8, WORD is_abs);

void Render_sprite(SPRT_ENT* sprt, RECT* lprcSrc);

__int16 sub_4035DC();
void sub_40245E();
void sub_40243A();

int EntGetPan(int a1);

void Vm_spr_dir(int id, __int16 a2, __int16 a3, __int16 a4, __int16 a5);
