#pragma once

enum SPR_ID
{
	SPID_PLAYER = 0,
	SPID_STALKER = 1,
	SPID_CURSOR = 11
};

#define SPID_GETFRAME(x)		((x) & 0xff)
#define SPID_GETENT(x)			(((x) >> 8) & 0x3f)
#define SPID_GETHFLIP(x)		((x) & 0x8000)
#define SPID_GETVFLIP(x)		((x) & 0x4000)
#define SPID_GETANIM(x)			((x) & 0x3fff)
#define SPID_GETFLIPS(x)		((x) & 0xc000)

struct SPRT_ENT
{
	void SetList();
	void Unlink();
	void Release();
	void UpdateXY();
	void Update();
	void SetXY(int x, int y, DWORD a4, int flip);
	void SetX0();
	void DoSound();

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
	TMC_PTN* seq_ptr;
	DWORD field_1E;
	DWORD field_22;
	DWORD field_26;
	DWORD field_2A;
	DWORD self_id;
	DWORD field_32;
	char bmp;
	DWORD is_reference;	// this is set to 0 and never used
	WORD frame_id;
	int flag0;
	DWORD is_animating;
	DWORD is_bg_spr;
	int lmx0;
	int lmx1;
	DWORD x1;
	short x2;
	short y2;
	WORD flag2;
	CTexture* texture;
	int x0;
	int y0;
	int priority;
	short seq_pos;
	int x3;
	int y3;
	DWORD width;
	DWORD height;
	DWORD anim_id;
	short seq_frame;
	DWORD anim_count;
	DWORD anim_flag_ex;
	DWORD is_busy;
	DWORD updated;
	DWORD sub_frame;
	WORD anim_flg;
	WORD seq_frame_new;
	WORD seq_len;
};

extern SPRT_ENT sprt_ent[21];

#define sprt_player		sprt_ent[SPID_PLAYER]
#define sprt_stalker	sprt_ent[SPID_STALKER]
#define sprt_cursor		sprt_ent[SPID_CURSOR]

void Vm_spr_lmt(int id, int lmx0, int lmx1);
void SetSprIsBg(int id, int is_bg);
void SprCursorAnimate();
void CursorDispCk();
void BgSprAnim(int id, int w, int h, CTexture* ptr);
void BgSprPos(int id, int x, int y, int pri);
void SprtTblDeinit();
void SprPos(int id, int x, int y, DWORD flags);
void SprAnim(int id, WORD anim, WORD anim_flg, WORD seq);
int SetSpriteData(SPRT_ENT* spr, unsigned int id);
void EntryBmpSprite(int id, __int16 x, __int16 y, __int16 flag, __int16 w, __int16 h, CTexture* ptr, DWORD a8, WORD is_abs);

void SprDraw(SPRT_ENT* sprt, CRect* lprcSrc);

void SprEnt(int id, int x, int y, DWORD pri, __int16 a5, __int16 a6, __int16 a7, DWORD a8, WORD is_abs);

int sub_4035DC();
void SprUpdater();
void EventWait();

int EntGetPan(int a1);

void Vm_spr_ent(int id, int x, int y, DWORD pri, __int16 a5, __int16 a6, __int16 a7, DWORD a8, WORD is_abs);
void Vm_spr_dir(int id, int a2, int a3, int a4, int a5);
void Vm_spr_walk_x(int id, int x0, int x1, int a4, int running);

void TriggerUpdate();
void CkAIAttack();
void SprSetDest(int id, int cur_x, int dst_y, int running);
int TriggerIntersectCk(int x, int y);

void LBtnClick(int is_double, int cursor_x, int cursor_y);
void RBtnClick(int x, int y);
