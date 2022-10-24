#pragma once

struct PATTERN_DATA
{
	__int16 field_0;
	WORD field_2[120];
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
	DWORD is_bg_spr;
	int lmx0;
	int lmx1;
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


void Vm_spr_lmt(int id, int lmx0, int lmx1);
void SetSprIsBg(int id, int is_bg);
void SprCursorAnimate();
void CursorDispCk();
void BgSprAnim(int id, int w, int h, CTim* ptr);
void BgSprPos(int id, int x, int y, int flag);
void SprtTblDeinit();
void SprPos(int id, int x, int y, DWORD flags);
void SprAnim(int id, WORD anim, WORD a3, WORD a4);
int SetSpriteData(SPRT_ENT* spr, unsigned int id);
void EntryBmpSprite(int id, __int16 x, __int16 y, __int16 flag, __int16 w, __int16 h, CTim* ptr, DWORD a8, WORD is_abs);

void SprDraw(SPRT_ENT* sprt, CRect* lprcSrc);

void SprEnt(int id, int x, int y, DWORD a4, __int16 a5, __int16 a6, __int16 a7, DWORD a8, WORD is_abs);

int sub_4035DC();
void SprUpdater();
void EventWait();

int EntGetPan(int a1);

void Vm_spr_ent(int id, DWORD x, DWORD y, DWORD flags, __int16 a5, __int16 a6, __int16 a7, DWORD a8, WORD is_abs);
void Vm_spr_dir(int id, int a2, int a3, int a4, int a5);
void Vm_spr_walk_x(int id, int x0, int x1, int a4, int running);

void TriggerUpdate();
void CkAIAttack();
void SprSetDest(int id, int cur_x, int dst_y, int running);
int intersect_triggers(int x, int y);

void LBtnClick(int is_double, int cursor_x, int cursor_y);
void RBtnClick(int x, int y);
