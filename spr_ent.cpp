#include <stdafx.h>
#include "game.h"

SPRT_ENT sprt_ent[21];
SPRT_DAT sprt_dat[2];

void SPRT_ENT::Link()
{
	if (field_4)
	{
		if (prev)
			prev->next = next;
		else
			prog.sprt = next;
		if (next)
			next->prev = prev;
		prev = 0;
		next = 0;
		field_4 = 0;
	}
}

void SPRT_ENT::SetList()
{
	SPRT_ENT* sprt;
	SPRT_ENT* i;

	if (field_4)
		Link();
	sprt = prog.sprt;
	for (i = prog.sprt; i; i = i->next)
	{
		if ((signed int)i->flag1 > (signed int)flag1)
			break;
		sprt = i;
	}
	if (i)
	{
		if (i->prev)
		{
			next = i;
			prev = i->prev;
			i->prev->next = this;
		}
		else
		{
			next = i;
			prog.sprt = this;
		}
		i->prev = this;
	}
	else
	{
		if (sprt)
			sprt->next = this;
		else
			prog.sprt = this;
		prev = sprt;
	}
	field_4 = 1;
}

void SPRT_ENT::Release()
{
	sprt_ent[field_2E].field_10 = 0;
	if (field_26 && ptr0)
	{
		delete ptr0;
		ptr0 = nullptr;
		field_26 = 0;
	}
	Link();
}

void SPRT_ENT::UpdateXY()
{
	if (flip)
	{
		x0 = (__int16)x2;
		y0 = (__int16)y2;
		x1 = (__int16)x2;
		flag0 = (__int16)flag2;
		flag1 = (__int16)flag2;
	}
	flip = 0;
}

void SPRT_ENT::Update()
{
	field_91 = 1;
	UpdateXY();
}

void SPRT_ENT::SetXY(int _x, int _y, DWORD a4, int flip)
{
	if (flip)
	{
		flip = 0;
		x0 = _x;
		y0 = _y;
		x1 = _x;
		flag0 = a4;
		flag1 = a4;
	}
	else
	{
		flip = 1;
		x2 = _x;
		y2 = _y;
		flag2 = (WORD)a4;
	}
}

void SPRT_ENT::SetX0()
{
	if (lmx >= 0 && x0 < lmx)
		x0 = lmx;
	if (lmy > 0 && x0 > lmy)
		x0 = lmy;
}

void SPRT_ENT::CalcPan()
{
	__int16 vol; // [esp+0h] [ebp-8h]
	__int16 pan; // [esp+2h] [ebp-6h]
	__int16 v3; // [esp+4h] [ebp-4h]
	unsigned __int16 v4; // [esp+6h] [ebp-2h]

	if (!field_45 && field_41 == 1)
	{
		v4 = (field_3B & 0x3FFF) >> 8;
		if (v4 <= 0x12u)
		{
			v3 = -1;//off_41F094[v4][field_95];
			if (v3 != -1)
			{
				if (vm_index5[47])
				{
					if (v3)
					{
						if (v3 == 1)
							v3 = 3;
					}
					else
					{
						v3 = 2;
					}
				}
				pan = Sound_get_pan(x0 - prog.screen_x);
				vol = 127;//vol_tbl[v3];
				if (pan == 16 || pan == -16)
					vol -= 24;
				if (vol < 0)
					vol = 0;
				//Sound_play(soundname_tbl[v3], 0, pan, vol);
			}
		}
	}
}

void Vm_spr_lmt(int a1, int lmx, int lmy)
{
	sprt_ent[a1].lmx = lmx;
	sprt_ent[a1].lmy = lmy;
}

void sub_404346(int a1, int a2)
{
	sprt_ent[a1].field_45 = a2;
}

void BgSprAnim(int id, __int16 w, __int16 h, CTim* ptr)
{
	if (sprt_ent[id].field_10)
	{
		sprt_ent[id].field_75 = w;
		sprt_ent[id].field_79 = h;
		sprt_ent[id].tim = ptr;
		//sprt_ent[id].bmp = ptr[(h - 1) * w];
		sprt_ent[id].field_37 = 0;
		sprt_ent[id].field_3B = -1;
		sub_404346(id, 1);
		sprt_ent[id].field_41 = 0;
		sprt_ent[id].field_8D = 0;
	}
}

void BgSprPos(int a1, __int16 x, __int16 y, __int16 flag)
{
	if (sprt_ent[a1].field_10)
	{
		sprt_ent[a1].x3 = x;
		sprt_ent[a1].y3 = y;
		sprt_ent[a1].x0 = x;
		sprt_ent[a1].y0 = y;
		sprt_ent[a1].flag1 = flag;
	}
}

void EntryBmpSprite(int id, __int16 x, __int16 y, __int16 flag, __int16 w, __int16 h, CTim* ptr, DWORD a8, WORD is_abs)
{
	if (id <= 20)
	{
		sprt_ent[id].field_2E = id;
		sprt_ent[id].field_10 = 1;
		sprt_ent[id].field_32 = a8;
		sprt_ent[id].is_abs = is_abs;
		BgSprPos(id, x, y, flag);
		BgSprAnim(id, w, h, ptr);
		Vm_spr_lmt(id, -1, -1);
		sprt_ent[id].SetList();
	}
}

void SprtTblDeinit()
{
	SPRT_ENT* next; // [esp+0h] [ebp-8h]
	SPRT_ENT* sprt; // [esp+4h] [ebp-4h]

	sprt = prog.sprt;
	if (prog.sprt)
	{
		do
		{
			next = sprt->next;
			sprt->Release();
			sprt = next;
		} while (next);
	}
	prog.sprt = 0;
}

void SprPos(int id, int x, int y, DWORD flags)
{
	if (sprt_ent[id].field_10)
		sprt_ent[id].SetXY(x, y, flags, 0);
}

int __cdecl sub_4033A4(WORD* dst, int a2)
{
	DWORD v3; // [esp+0h] [ebp-4h]
	DWORD v4; // [esp+0h] [ebp-4h]

	static WORD word_41FDA4[][36][2] =
	{
		258,   0, 270,   0, 264, 271, 261, 258, 261, 270, 261, 271, 258,   0, 270,   0, 264, 271, 261, 258, 261, 270, 262, 271, 263, 258, 270,   0, 271,   0, 262, 258, 262, 270, 262, 271,
		33026,   0,33038,   0,33032,33039,33029,33026,33029,33038,33029,33039,33026,   0,33038,   0,33032,33039,33029,33026,33029,33038,33030,33039,33031,33026,33038,   0,33039,   0,33030,33026,33030,33038,33030,33039,
		1112,   0,1087,   0,1096,   0,1086,1112,1086,1087,1086,1096,1112,   0,1087,   0,1096,   0,1086,1112,1086,1087,1098,1096,1099,1112,1087,   0,1096,   0,1098,1112,1098,1087,1098,1096,
		33880,   0,33855,   0,33864,   0,33854,33880,33854,33855,33854,33864,33880,   0,33855,   0,33864,   0,33854,33880,33854,33855,33866,33864,33867,33880,33855,   0,33864,   0,33866,33880,33866,33855,33866,33864,
		1115,   0,1089,   0,1097,   0,1088,1115,1088,1089,1088,1097,1115,   0,1089,   0,1097,   0,1088,1115,1088,1089,1100,1097,1101,1115,1089,   0,1097,   0,1100,1115,1100,1089,1100,1097,
		33883,   0,33857,   0,33865,   0,33856,33883,33856,33857,33856,33865,33883,   0,33857,   0,33865,   0,33856,33883,33856,33857,33868,33865,33869,33883,33857,   0,33865,   0,33868,33883,33868,33857,33868,33865
	};
	static WORD word_42011C[][2][2] =
	{
		3072,3073,
		35840,35841,
		3109,3074,
		35877,35842,
		3076,3075,
		35844,35843,
		3080,3081,
		35848,35849,
		3072,3085,
		35840,35853
	};
	static WORD word_41FF6C[][36][2] =
	{
		36864,   0,36865,   0,36868,36866,36867,36864,36867,36865,36867,36866,36864,   0,36865,   0,36868,36866,36867,36864,36867,36865,36869,36866,36870,36864,36865,   0,36866,   0,36869,36864,36869,36865,36869,36866,
		4096,   0,4097,   0,4100,4098,4099,4096,4099,4097,4099,4098,4096,   0,4097,   0,4100,4098,4099,4096,4099,4097,4101,4098,4102,4096,4097,   0,4098,   0,4101,4096,4101,4097,4101,4098,
		37376,   0,37377,   0,37380,37378,37379,37376,37379,37377,37379,37378,37376,   0,37377,   0,37378,   0,37379,37376,37379,37377,37381,37378,37382,37376,37377,   0,37378,   0,37381,37376,37381,37377,37381,37378,
		4608,   0,4609,   0,4612,4610,4611,4608,4611,4609,4611,4610,4608,   0,4609,   0,4610,   0,4611,4608,4611,4609,4613,4610,4614,4608,4609,   0,4610,   0,4613,4608,4613,4609,4613,4610,
		37632,   0,37633,   0,37636,37634,37635,37632,37635,37633,37635,37634,37632,   0,37633,   0,37634,   0,37635,37632,37635,37633,37637,37634,37638,37632,37633,   0,37634,   0,37637,37632,37637,37633,37637,37634,
		4864,   0,4865,   0,4868,4866,4867,4864,4867,4865,4867,4866,4864,   0,4865,   0,4866,   0,4867,4864,4867,4865,4869,4866,4870,4864,4865,   0,4866,   0,4869,4864,4869,4865,4869,4866
	};

	if (a2)
	{
		if (a2 != 1)
			return 0;
		if (vm_index5[25])
		{
			if (vm_index5[25] == 1)
			{
				v4 = sprt_dat[1].type2 + 6 * sprt_dat[1].type + 18 * sprt_dat[1].type3;
				if (sprt_dat[1].type0 != sprt_dat[1].type3)
					v4 += 3;
				dst[0] = word_41FF6C[vm_index5[6]][v4][0];
				dst[1] = word_41FF6C[vm_index5[6]][v4][1];
			}
			else if (vm_index5[25] == 2)
			{
				if ((int)sprt_dat[1].type3 > 1)
					sprt_dat[1].type3 = 1;
				dst[0] = word_42011C[vm_index5[43]][sprt_dat[1].type3][sprt_dat[1].type2];
				dst[1] = 0;
			}
		}
	}
	else
	{
		v3 = sprt_dat[0].type2 + 6 * sprt_dat[0].type + 18 * sprt_dat[0].type3;
		if (sprt_dat[0].type0 != sprt_dat[0].type3)
			v3 += 3;
		dst[0] = word_41FDA4[vm_index5[4]][v3][0];
		dst[1] = word_41FDA4[vm_index5[4]][v3][1];
	}
	return 1;
}

void SprAnim(unsigned int id, WORD a2, WORD a3, WORD a4)
{
	WORD v5[2];

	if (sprt_ent[id].field_10)
	{
		if (a2 == 0xFFFF)
		{
			if (sub_4033A4(v5, id))
			{
				sprt_ent[id].field_3B = v5[0];
				sprt_dat[id].field_14 = v5[1];
				sprt_dat[id].field_10 = 0;
				sprt_dat[id].type0 = sprt_dat[id].type3;
				sprt_dat[id].type = sprt_dat[id].type2;
			}
		}
		else
		{
			sprt_ent[id].field_3B = a2;
		}
		sprt_ent[id].field_99 = a3;
		sprt_ent[id].field_9B = a4 & 0xff;
		sprt_ent[id].field_9D = a4 >> 8;
		sprt_ent[id].Update();
		sprt_ent[id].UpdateXY();
		sub_404346(id, 0);
		sprt_ent[id].field_41 = 0;
		sprt_ent[id].field_8D = 0;
		if (id <= 1)
			sprt_dat[id].field_14 = 0;
	}
}

void sub_402FAF()
{
	struct tagRECT rcDst; // [esp+0h] [ebp-24h] BYREF
	RECT rcSrc1; // [esp+10h] [ebp-14h] BYREF
	int v2; // [esp+20h] [ebp-4h]

	rcSrc1.left = prog.field_1B8 + 1;
	rcSrc1.right = prog.field_1B8 + 2;
	rcSrc1.top = prog.field_1BC + 1;
	rcSrc1.bottom = prog.field_1BC + 2;
	if (prog.field_1A4)
	{
		v2 = 0;
		while (1)
		{
			if ((vm_index6[v2 + 10] & 0x10) == 0 && sub_403304(v2))
			{
				rcDst.left = prog.render_rect.left + vm_rects[v2].left - prog.screen_x;
				rcDst.right = prog.render_rect.left + vm_rects[v2].right - prog.screen_x;
				rcDst.top = prog.render_rect.top + vm_rects[v2].top - prog.screen_y;
				rcDst.bottom = prog.render_rect.top + vm_rects[v2].bottom - prog.screen_y;
				if (IntersectRect(&rcDst, &rcSrc1, &rcDst))
					break;
			}
			if (++v2 >= 30)
				goto LABEL_15;
		}
		prog.field_1A8 = vm_rects[v2].left - (32 - (vm_rects[v2].right - vm_rects[v2].left)) / 2;
		prog.field_1AC = vm_rects[v2].top - (32 - (vm_rects[v2].bottom - vm_rects[v2].top)) / 2;
		if (prog.field_14C)
		{
			if (prog.field_14C == 1)
			{
				if (sprt_ent[11].field_8D)
				{
					sprt_ent[11].SetXY(prog.field_1A8, prog.field_1AC, 0x64u, 1);
					prog.field_14C = 2;
				}
			}
			else if (prog.field_14C == 3)
			{
				prog.field_14C = 1;
				sprt_ent[11].field_99 = 0;
				sprt_ent[11].Update();
				sprt_ent[11].SetXY(prog.field_1A8, prog.field_1AC, 0x64u, 1);
			}
		}
		else
		{
			prog.cur_type1 = ShowCursor(0);
			prog.field_14C = 1;
			sprt_ent[11].field_99 = 0;
			sprt_ent[11].Update();
			sprt_ent[11].SetXY(prog.field_1A8, prog.field_1AC, 0x64u, 1);
		}
	}
	else
	{
	LABEL_15:
		if ((unsigned int)(prog.field_14C - 1) < 2)
		{
			sprt_ent[11].field_99 = 1;
			sprt_ent[11].Update();
			prog.field_14C = 3;
		}
		else if (prog.field_14C == 3)
		{
			if (sprt_ent[11].field_8D)
			{
				prog.field_14C = 0;
				if (prog.cur_type1 < 0)
					prog.cur_type1 = ShowCursor(1);
			}
		}
	}
}

void __cdecl SpriteGetRect(SPRT_ENT* s, RECT* r)
{
	r->left = s->x3;
	r->right = s->field_75 + s->x3 - 1;
	r->top = s->y3;
	r->bottom = s->field_79 + s->y3 - 1;
}

void Render_sprite(SPRT_ENT* sprt, RECT* lprcSrc)
{
	struct tagRECT v2; // [esp+0h] [ebp-40h] BYREF
	struct tagRECT rcDst; // [esp+10h] [ebp-30h] BYREF
	RECT rcSrc2; // [esp+20h] [ebp-20h] BYREF
	int srcy; // [esp+30h] [ebp-10h]
	int srcx; // [esp+34h] [ebp-Ch]
	int dsty; // [esp+38h] [ebp-8h]
	int dstx; // [esp+3Ch] [ebp-4h]

	SpriteGetRect(sprt, &rcDst);
	if (lprcSrc)
	{
		CopyRect(&rcSrc2, lprcSrc);
		if (!IntersectRect(&rcDst, &rcDst, &rcSrc2))
			return;
	}
	else
	{
		TMapGetDstRect(&tmap, &rcSrc2);
		if (!IntersectRect(&rcDst, &rcDst, &rcSrc2))
			return;
	}
	TMapGetRect(&tmap, &v2);
	if ((signed int)sprt->x3 < rcSrc2.left)
	{
		dstx = v2.left;
		srcx = rcSrc2.left - sprt->x3;
	}
	else
	{
		dstx = v2.left + sprt->x3 - rcSrc2.left;
		srcx = 0;
	}
	if ((signed int)sprt->y3 < rcSrc2.top)
	{
		dsty = v2.top;
		srcy = rcSrc2.top - sprt->y3;
	}
	else
	{
		dsty = v2.top + sprt->y3 - rcSrc2.top;
		srcy = 0;
	}

	RenderRect(sprt->tim, GETX(dstx), GETY(dsty), sprt->field_75, sprt->field_79, srcx, srcy, 0xff, 0xff, 0xff);

	//RenderToOffScreen(
	//	dstx,
	//	dsty,
	//	srcx,
	//	srcy,
	//	sprt->field_75,
	//	sprt->field_79,
	//	sprt->field_75,
	//	rcDst.right - rcDst.left + 1,
	//	rcDst.bottom - rcDst.top + 1,
	//	(BYTE*)sprt->field_5B,
	//	sprt->bmp,
	//	sprt->field_32,
	//	sprt->field_37);
}

void SprEnt(signed int id, int x, int y, DWORD a4, __int16 a5, __int16 a6, __int16 a7, DWORD a8, WORD is_abs)
{
	if (id <= 11)
	{
		sprt_ent[id].field_2E = id;
		sprt_ent[id].field_10 = 1;
		sprt_ent[id].field_32 = a8;
		sprt_ent[id].is_abs = is_abs;
		sprt_ent[id].SetXY(x, y, a4, 1);
		SprAnim(id, a5, a6, a7);
		Vm_spr_lmt(id, -1, -1);
	}
}

void sub_403252()
{
	struct tagRECT rcDst; // [esp+0h] [ebp-24h] BYREF
	RECT rcSrc1; // [esp+10h] [ebp-14h] BYREF

	rcSrc1.left = prog.field_1B8 + 1;
	rcSrc1.right = prog.field_1B8 + 2;
	rcSrc1.top = prog.field_1BC + 1;
	rcSrc1.bottom = prog.field_1BC + 2;
	if (prog.vm_func != 1 || !IntersectRect(&rcDst, &rcSrc1, &prog.render_rect) || prog.field_1A4 && !prog.field_14C)
	{
		if (prog.cur_type1 < 0)
			prog.cur_type1 = ShowCursor(1);
	}
	else if (prog.cur_type1 >= 0)
	{
		prog.cur_type1 = ShowCursor(0);
	}
}

__int16 sub_403619()
{
	if (vm_index5[2] <= 200)
		return 0;
	if (vm_index5[2] <= 400)
		return 1;
	if (vm_index5[2] > 600)
		return 3;
	return 2;
}

__int16 sub_4035DC()
{
	WORD v3; // al

	if (!vm_index5[28])
		return 3 - sub_403619();
	v3 = prog.field_194;
	--prog.field_194;
	return v3 & 3;
}

void __cdecl sub_401C83(SPRT_ENT* a1)
{
	unsigned __int16 v1; // ax

	if (a1->field_8D)
	{
		if ((a1->field_99 & 0x10) != 0)
			goto LABEL_17;
		v1 = a1->field_3B & 0x3FFF;
		if (v1 > 3081u)
		{
			if (v1 == 3085 || (unsigned int)v1 - 4097 < 2 || (unsigned int)v1 - 4609 < 2 || (unsigned int)v1 - 4865 < 2)
				goto LABEL_17;
		}
		else
		{
			if (v1 == 3081)
				goto LABEL_17;
			if (v1 <= 0x441u)
			{
				if (v1 != 1089 && (unsigned int)v1 - 270 >= 2 && v1 != 1087)
					return;
			LABEL_17:
				a1->Update();
				return;
			}
			if ((unsigned int)v1 - 1096 < 2 || (unsigned int)v1 - 3073 < 3)
				goto LABEL_17;
		}
	}
}

void sub_404783(SPRT_ENT* a1)
{
	a1->SetList();
}

void sub_403536()
{
	int v1;

	if (prog.field_198 && sprt_ent[0].field_41 && prog.field_19C <= sprt_ent[0].x0 && prog.field_1A0 + prog.field_19C >= sprt_ent[0].x0)
	{
		v1 = prog.field_19E + prog.field_1A2 * (sprt_ent[0].x0 - prog.field_19C) / prog.field_1A0 - sprt_ent[0].y0;
		if (v1)
		{
			sprt_ent[0].y0 += v1;
			sprt_ent[0].y3 += v1;
		}
	}
}

DWORD dword_422D5C[200];
WORD word_422AB8;

int __cdecl SetSpriteData(SPRT_ENT* a1, unsigned __int16 a2)
{
	__int16 v3[368]; // [esp+0h] [ebp-30Ch] BYREF
	char v4[14]; // [esp+2E0h] [ebp-2Ch] BYREF
	__int16 v5; // [esp+2F0h] [ebp-1Ch]
	__int16 v6; // [esp+2F2h] [ebp-1Ah]
	WORD v8; // [esp+2FAh] [ebp-12h]
	int v9; // [esp+2FCh] [ebp-10h]
	void* Src; // [esp+300h] [ebp-Ch]
	__int16 i; // [esp+304h] [ebp-8h]
	__int16 v12; // [esp+306h] [ebp-6h]
	WORD v13; // [esp+308h] [ebp-4h]
	__int16 v14; // [esp+30Ah] [ebp-2h]

	if (!a1->field_91)
	{
		if ((a1->field_99 & 2) != 0)
			a1->field_8D = 1;
		if (a1->field_8D)
			return 1;
	}
	v8 = ((int)a2 >> 8) & 0x3F;
	if (v8 != word_422AB8)
	{
		word_422AB8 = v8;
		//if (!ReadPatternData_0())
		//	return 0;
		//if (!ReadPosiData_0())
		//	return 0;
	}
	if (!a1->ptr0 || LOWORD(a1->field_89) + LOWORD(a1->field_7D) != a2)
	{
		//if (!TSprSlotManager::Read((TSprSlotManager*)sprt_slot_manager, a1, a2))
		{
			printf("SetSpriteData\n");
			return 0;
		}
		a1->field_7D = a2 & 0x3FFF;
		a1->field_81 = 0xFFFF;
		a1->field_85 = 0;
		a1->field_89 = a2 & 0xC000;
		a1->field_8D = 0;
	}
	v14 = (unsigned __int8)a2;
	Src = a1->ptr0;
	memcpy(v4, Src, sizeof(v4));
	v9 = 14;
	memcpy(v3, (char*)Src + 14, 0xEu);
	v9 = 28;
	v12 = v3[6];
	for (i = 0; i < v12; ++i)
	{
		memcpy(&v3[6 * i + 7], (char*)Src + v9, 0xCu);
		v9 += 12;
	}
	if (a1->field_91 == 1)
	{
		a1->field_81 = 0;
		if ((a1->field_99 & 1) != 0)
			a1->field_81 = (unsigned __int16)v3[6] - 1;
		if ((a1->field_99 & 4) != 0)
			a1->field_81 = a1->field_9B;
		v13 = (WORD)a1->field_81;
		a1->field_85 = (*((unsigned __int16*)dword_422D5C[v14] + (__int16)v13 + 61) + 1) / 2;
		a1->field_8D = 0;
		a1->field_91 = 0;
		a1->field_41 = 1;
		if (!v13)
			a1->field_6B = -1;
		if ((__int16)v13 == (unsigned __int16)v3[6] - 1)
			a1->field_6B = v3[6] - 1;
		if ((a1->field_99 & 4) != 0)
		{
			a1->field_6B = v13 - 1;
			while (!*((WORD*)dword_422D5C[v14] + (__int16)a1->field_6B + 61))
			{
				if ((--a1->field_6B & 0x8000u) != 0)
				{
					a1->field_6B = -1;
					break;
				}
			}
		}
	}
	while (!a1->field_85)
	{
		a1->field_41 = 1;
		if ((a1->field_99 & 1) != 0)
		{
			if ((--a1->field_81 & 0x80000000) != 0)
			{
			LABEL_38:
				a1->field_85 = 1;
				break;
			}
			v13 = (WORD)a1->field_81;
			a1->field_85 = (*((unsigned __int16*)dword_422D5C[v14] + (__int16)v13 + 61) + 1) / 2;
		}
		else
		{
			if ((unsigned __int16)v3[6] <= (int)++a1->field_81)
				goto LABEL_38;
			v13 = (WORD)a1->field_81;
			a1->field_85 = (*((unsigned __int16*)dword_422D5C[v14] + (__int16)v13 + 61) + 1) / 2;
		}
	}
	if (!--a1->field_85)
	{
		if ((a1->field_99 & 1) == 0)
		{
			if ((signed int)(a1->field_81 + 1) < (unsigned __int16)v3[6])
			{
				if ((a1->field_99 & 8) != 0 && a1->field_9D <= (int)a1->field_81)
				{
					a1->field_85 = 1;
					a1->field_8D = 1;
				}
			}
			else
			{
				a1->field_85 = 1;
				a1->field_8D = 1;
				a1->field_81 = (unsigned __int16)v3[6] - 1;
			}
		}
		if ((a1->field_99 & 1) != 0)
		{
			if (a1->field_81)
			{
				if ((a1->field_99 & 8) != 0 && a1->field_9D == a1->field_81)
				{
					a1->field_85 = 1;
					a1->field_8D = 1;
					a1->field_81 = (unsigned __int16)v3[6] - 1;
				}
			}
			else
			{
				a1->field_85 = 1;
				a1->field_8D = 1;
				a1->field_81 = 0;
			}
		}
	}
	v13 = (WORD)a1->field_81;
	//a1->flag1 = ptr_abm_tbl[v14][60].field_2[(__int16)v13] & 0x1F;
	a1->flag1 += a1->flag0;
	a1->field_75 = (unsigned __int16)v3[6 * (__int16)v13 + 7];
	a1->field_79 = (unsigned __int16)v3[6 * (__int16)v13 + 8];
	//a1->bmp_data = (BYTE*)a1->ptr0 + v9 + *(_DWORD*)&v3[6 * (__int16)v13 + 11];
	//if ((ptr_abm_tbl[v14][60].field_2[(__int16)v13] & 0x8000u) != 0)
	//	v7 = a1->field_79 * (((unsigned __int16)a1->field_75 + 3) & 0xFFFC) - 1;
	//else
	//	v7 = (a1->field_79 - 1) * (((unsigned __int16)a1->field_75 + 3) & 0xFFFC);
	//a1->bmp = a1->bmp_data[v7];
	a1->field_95 = *((unsigned __int16*)dword_422D5C[v14] + (__int16)v13 + 1);
	if (a1->field_95 == 0xFFFF)
	{
		a1->x3 = 768;
		a1->y3 = 768;
		return 1;
	}
	else
	{
		if (a1->field_6B == 0xFFFF)
		{
			//v6 = 2 * ptr_abm_tbl[v14]->field_2[0];
			//v5 = 2 * ptr_abm_tbl[v14][30].field_2[0];
			a1->field_6B = 0;
		}
		else if (v13 == a1->field_6B)
		{
			v6 = 0;
			v5 = 0;
		}
		else
		{
			//v6 = 2 * (ptr_abm_tbl[v14]->field_2[(__int16)v13] - ptr_abm_tbl[v14]->field_2[(__int16)a1->field_6B]);
			//v5 = 2 * (ptr_abm_tbl[v14][30].field_2[(__int16)v13] - ptr_abm_tbl[v14][30].field_2[(__int16)a1->field_6B]);
			a1->field_6B = v13;
		}
		if ((a1->field_89 & 0x8000) == 0)
		{
			a1->x0 += v6;
			a1->x3 = (__int16)(LOWORD(a1->x0) - v3[6 * (__int16)v13 + 9]);
		}
		else
		{
			a1->x0 -= v6;
			a1->x3 = (__int16)(LOWORD(a1->x0) + v3[6 * (__int16)v13 + 9] - v3[6 * (__int16)v13 + 7]);
		}
		if ((a1->field_89 & 0x4000) != 0)
		{
			a1->y0 -= v5;
			a1->y3 = (__int16)(LOWORD(a1->y0) - v3[6 * (__int16)v13 + 10]);
		}
		else
		{
			a1->y0 += v5;
			a1->y3 = (__int16)(v3[6 * (__int16)v13 + 10] + LOWORD(a1->y0) - LOWORD(a1->field_79));
		}
		return 1;
	}
}

void sub_40245E()
{
	DWORD flag1; // [esp+0h] [ebp-8h]
	SPRT_ENT* s; // [esp+4h] [ebp-4h]

	for (s = prog.sprt; s; s = s->next)
	{
		s->field_41 = 0;
		s->x1 = s->x0;
		flag1 = s->flag1;
		sub_401C83(s);
		if (s->field_3B != 0xFFFF)
		{
			if ((s->field_3B & 0x3FFF) >= 0x3800);
				//sub_4013B0(s);
			else
				SetSpriteData(s, s->field_3B);
		}
		if (flag1 != s->flag1)
			sub_404783(s);
		s->SetX0();
		s->CalcPan();
	}
	sub_403536();
}

DWORD __cdecl Vm_ent_wait(int a1)
{
	if (sprt_ent[a1].field_10)
		return sprt_ent[a1].field_8D;
	else
		return 1;
}

void sub_40243A()
{
	if (prog.field_128)
	{
		if (Vm_ent_wait(0))
			prog.field_128 = 0;
	}
}

void sub_401DB5()
{
	if (sprt_ent[0].lmx >= 0
		&& (sprt_dat[0].type == 1 || sprt_dat[0].type == 2)
		&& sprt_ent[0].x0 <= sprt_ent[0].lmx
		&& sprt_dat[0].type0 == 1)
	{
		sprt_dat[0].field_10 = 1;
		sprt_dat[0].type2 = 0;
		sprt_dat[0].field_1C = 0;
		sprt_dat[0].type3 = 1;
	}
	if (sprt_ent[0].lmy >= 0
		&& (sprt_dat[0].type == 1 || sprt_dat[0].type == 2)
		&& sprt_ent[0].x0 >= sprt_ent[0].lmy
		&& !sprt_dat[0].type0)
	{
		sprt_dat[0].field_10 = 1;
		sprt_dat[0].type2 = 0;
		sprt_dat[0].field_1C = 0;
		sprt_dat[0].type3 = 0;
	}
}

void __cdecl sub_401EB7(unsigned int a1)
{
	if (sprt_ent[a1].field_8D)
	{
		if (sprt_dat[a1].field_14)
		{
			SprAnim(a1, sprt_dat[a1].field_14, 0, 0);
			sprt_dat[a1].field_14 = 0;
		}
	}
}

int EntGetPan(int a1)
{
	return Sound_get_pan(sprt_ent[a1].x0 - prog.screen_x);
}

void sub_4042C0(int id)
{
	sprt_dat[id].field_14 = 0;
	sprt_dat[id].type4 = 0;
	sprt_dat[id].field_10 = 0;
	sprt_dat[id].field_1C = 0;
	sprt_dat[id].field_20 = 0;
	sprt_dat[id].field_24 = 0;
}

void Vm_spr_dir(int id, __int16 a2, __int16 a3, __int16 a4, __int16 a5)
{
	DWORD v5; // ecx

	if (a2 != -1)
		sprt_dat[id].type0 = a2 >> 3;
	if (a4 == -1)
		v5 = sprt_dat[id].type0;
	else
		v5 = a4 >> 3;
	sprt_dat[id].type3 = v5;
	if (a3 != -1)
		sprt_dat[id].type = a3;
	if (a5 == -1)
		sprt_dat[id].type2 = sprt_dat[id].type;
	else
		sprt_dat[id].type2 = a5;
	sub_4042C0(id);
}

