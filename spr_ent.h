#pragma once

struct PATTERN_DATA
{
	__int16 field_0;
	WORD field_2[120];
};


struct AI_ENT
{
	DWORD type0;
	DWORD type;
	DWORD type3;
	DWORD type2;
	DWORD enabled;
	WORD anim;
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
	void CalcPan();

	DWORD flip;
	DWORD linked;
	SPRT_ENT* next;
	SPRT_ENT* prev;
	DWORD enabled;
	WORD is_abs;
	char field_16;
	char field_17;
	char field_18;
	char field_19;
	//BITMAPINFOHEADER* ptr0;
	DWORD field_1E;
	DWORD field_22;
	DWORD field_26;
	DWORD field_2A;
	DWORD id2;
	DWORD field_32;
	char bmp;
	DWORD field_37;
	WORD frame_id;
	DWORD flag0;
	DWORD field_41;
	DWORD field_45;
	int lmx;
	int lmy;
	DWORD x1;
	short x2;
	short y2;
	WORD flag2;
	CTim* tim;
	int x0;
	int y0;
	DWORD flag1;
	WORD field_6B;
	int x3;
	int y3;
	DWORD width;
	DWORD height;
	DWORD field_7D;
	DWORD field_81;
	DWORD field_85;
	DWORD field_89;
	DWORD is_busy;
	DWORD field_91;
	DWORD field_95;
	WORD field_99;
	WORD field_9B;
	WORD field_9D;
};

extern SPRT_ENT sprt_ent[21];
extern AI_ENT ai_ent[2];

void Vm_spr_lmt(int a1, int lmx, int lmy);
void sub_404346(int a1, int a2);
void SprCursorAnimate();
void CursorDispCk();
void BgSprAnim(int id, __int16 w, __int16 h, CTim* ptr);
void BgSprPos(int a1, __int16 x, __int16 y, __int16 flag);
void SprtTblDeinit();
void SprPos(int id, int x, int y, DWORD flags);
void SprAnim(unsigned int id, WORD a2, WORD a3, WORD a4);
int SetSpriteData(SPRT_ENT* spr, unsigned int id);
void EntryBmpSprite(int id, __int16 x, __int16 y, __int16 flag, __int16 w, __int16 h, CTim* ptr, DWORD a8, WORD is_abs);

void SprDraw(SPRT_ENT* sprt, CRect* lprcSrc);

void SprEnt(signed int id, int x, int y, DWORD a4, __int16 a5, __int16 a6, __int16 a7, DWORD a8, WORD is_abs);

__int16 sub_4035DC();
void sub_40245E();
void sub_40243A();

int EntGetPan(int a1);

void Vm_spr_ent(int a1, DWORD a2, DWORD a3, DWORD a4, __int16 a5, __int16 a6, __int16 a7, DWORD a8, WORD a9);
void Vm_spr_dir(int id, int a2, int a3, int a4, int a5);
void Vm_spr_walk_x(int id, int x0, int x1, int a4, int running);

void sub_401D32();
void sub_40266A();
void sub_401D74();
void sub_4021DC();
void SprSetDest(int id, int cur_x, int dst_y, int running);
int intersect_triggers(int x, int y);

void LBtnClick(int is_double, LONG x, LONG y);
void RBtnClick(LONG x, LONG y);
