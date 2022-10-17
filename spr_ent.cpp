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
	sprt_ent[id2].enabled = 0;
	if (field_26 && tim)
	{
		if (tim->is_ref == 0)
			delete tim;
		tim = nullptr;
		//delete ptr0;
		//ptr0 = nullptr;
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

void SPRT_ENT::SetXY(int _x, int _y, DWORD a4, int _flip)
{
	if (_flip)
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
		v4 = (id & 0x3FFF) >> 8;
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

void sub_404346(int id, int val)
{
	sprt_ent[id].field_45 = val;
}

void BgSprAnim(int id, __int16 w, __int16 h, CTim* ptr)
{
	if (sprt_ent[id].enabled)
	{
		sprt_ent[id].width = w;
		sprt_ent[id].height = h;
		sprt_ent[id].tim = ptr;
		//sprt_ent[id].bmp = ptr[(h - 1) * w];
		sprt_ent[id].field_37 = 0;
		sprt_ent[id].id = -1;
		sub_404346(id, 1);
		sprt_ent[id].field_41 = 0;
		sprt_ent[id].field_8D = 0;
	}
}

void BgSprPos(int a1, __int16 x, __int16 y, __int16 flag)
{
	if (sprt_ent[a1].enabled)
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
		sprt_ent[id].id2 = id;
		sprt_ent[id].enabled = 1;
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

	if (prog.sprt)
	{
		sprt = prog.sprt;
		do
		{
			next = sprt->next;
			sprt->Release();
			sprt = next;
		} while (next);
	}
	prog.sprt = nullptr;
}

void SprPos(int id, int x, int y, DWORD flags)
{
	if (sprt_ent[id].enabled)
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

	if (sprt_ent[id].enabled)
	{
		if (a2 == 0xFFFF)
		{
			if (sub_4033A4(v5, id))
			{
				sprt_ent[id].id = v5[0];
				sprt_dat[id].field_14 = v5[1];
				sprt_dat[id].enabled = 0;
				sprt_dat[id].type0 = sprt_dat[id].type3;
				sprt_dat[id].type = sprt_dat[id].type2;
			}
		}
		else
		{
			sprt_ent[id].id = a2;
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
	RECT rcDst; // [esp+0h] [ebp-24h] BYREF
	RECT rcSrc1; // [esp+10h] [ebp-14h] BYREF
	int i; // [esp+20h] [ebp-4h]

	rcSrc1.left = prog.field_1B8 + 1;
	rcSrc1.right = prog.field_1B8 + 2;
	rcSrc1.top = prog.field_1BC + 1;
	rcSrc1.bottom = prog.field_1BC + 2;
	if (prog.field_1A4)
	{
		i = 0;
		while (1)
		{
			if ((vm_index6[i + 10] & 0x10) == 0 && sub_403304(i))
			{
				rcDst.left = prog.render_rect.left + vm_rects[i].left - prog.screen_x;
				rcDst.right = prog.render_rect.left + vm_rects[i].right - prog.screen_x;
				rcDst.top = prog.render_rect.top + vm_rects[i].top - prog.screen_y;
				rcDst.bottom = prog.render_rect.top + vm_rects[i].bottom - prog.screen_y;
				if (IntersectRect(&rcDst, &rcSrc1, &rcDst))
					break;
			}
			if (++i >= 30)
				goto LABEL_15;
		}
		prog.field_1A8 = vm_rects[i].left - (32 - (vm_rects[i].right - vm_rects[i].left)) / 2;
		prog.field_1AC = vm_rects[i].top - (32 - (vm_rects[i].bottom - vm_rects[i].top)) / 2;
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
	r->right = s->width + s->x3 - 1;
	r->top = s->y3;
	r->bottom = s->height + s->y3 - 1;
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

	RenderRect(sprt->tim, GETX(dstx), GETY(dsty), sprt->width, sprt->height, srcx, srcy, 0xff, 0xff, 0xff);

	//RenderToOffScreen(
	//	dstx,
	//	dsty,
	//	srcx,
	//	srcy,
	//	sprt->width,
	//	sprt->height,
	//	sprt->width,
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
		sprt_ent[id].id2 = id;
		sprt_ent[id].enabled = 1;
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
		v1 = a1->id & 0x3FFF;
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

PATTERN_DATA *pattern_data[200];
WORD word_422AB8;

int SetSpriteData(SPRT_ENT* spr, unsigned __int16 id)
{
	__int16 v3[368]; // [esp+0h] [ebp-30Ch] BYREF
	//char v4[14]; // [esp+2E0h] [ebp-2Ch] BYREF
	__int16 v5; // [esp+2F0h] [ebp-1Ch]
	__int16 v6; // [esp+2F2h] [ebp-1Ah]
	WORD v8; // [esp+2FAh] [ebp-12h]
	int v9; // [esp+2FCh] [ebp-10h]
	//void* Src; // [esp+300h] [ebp-Ch]
	__int16 i; // [esp+304h] [ebp-8h]
	__int16 v12; // [esp+306h] [ebp-6h]
	WORD v13; // [esp+308h] [ebp-4h]
	__int16 lo_id; // [esp+30Ah] [ebp-2h]

	if (!spr->field_91)
	{
		if ((spr->field_99 & 2) != 0)
			spr->field_8D = 1;
		if (spr->field_8D)
			return 1;
	}
	v8 = ((int)id >> 8) & 0x3F;
	if (v8 != word_422AB8)
	{
		word_422AB8 = v8;
		//if (!ReadPatternData_0())
		//	return 0;
		//if (!ReadPosiData_0())
		//	return 0;
	}
	if (!spr->tim || LOWORD(spr->field_89) + LOWORD(spr->field_7D) != id)
	{
		if (!sprt_slot_manager.Read(spr, id))
		{
			printf("SetSpriteData\n");
			return 0;
		}
		spr->field_7D = id & 0x3FFF;
		spr->field_81 = 0xFFFF;
		spr->field_85 = 0;
		spr->field_89 = id & 0xC000;
		spr->field_8D = 0;
	}

	lo_id = id & 0xff;
	//Src = spr->ptr0;
	//memcpy(v4, Src, sizeof(v4));
	v9 = 14;
	//memcpy(v3, (char*)Src + 14, 0xEu);
	v9 = 28;
	v12 = v3[6];
	for (i = 0; i < v12; ++i)
	{
		//memcpy(&v3[6 * i + 7], (char*)Src + v9, 0xCu);
		v9 += 12;
	}
	if (spr->field_91 == 1)
	{
		spr->field_81 = 0;
		if ((spr->field_99 & 1) != 0)
			spr->field_81 = (unsigned __int16)v3[6] - 1;
		if ((spr->field_99 & 4) != 0)
			spr->field_81 = spr->field_9B;
		v13 = (WORD)spr->field_81;
		//spr->field_85 = (pattern_data[lo_id][30].field_2[v13] + 1) / 2;
		spr->field_8D = 0;
		spr->field_91 = 0;
		spr->field_41 = 1;
		if (!v13)
			spr->field_6B = 0xffff;
		if ((__int16)v13 == (unsigned __int16)v3[6] - 1)
			spr->field_6B = v3[6] - 1;
		if ((spr->field_99 & 4) != 0)
		{
			spr->field_6B = v13 - 1;
			//while (!pattern_data[lo_id][30].field_2[spr->field_6B])
			{
				//if ((--spr->field_6B & 0x8000u) != 0)
				{
					//spr->field_6B = 0xffff;
					//break;
				}
			}
		}
	}
	while (!spr->field_85)
	{
		spr->field_41 = 1;
		if ((spr->field_99 & 1) != 0)
		{
			if ((--spr->field_81 & 0x80000000) != 0)
			{
			LABEL_38:
				spr->field_85 = 1;
				break;
			}
			v13 = (WORD)spr->field_81;
			spr->field_85 = (pattern_data[lo_id][30].field_2[v13] + 1) / 2;
		}
		else
		{
			if ((unsigned __int16)v3[6] <= (int)++spr->field_81)
				goto LABEL_38;
			v13 = (WORD)spr->field_81;
			//spr->field_85 = (pattern_data[lo_id][30].field_2[v13] + 1) / 2;
		}
	}
	if (!--spr->field_85)
	{
		if ((spr->field_99 & 1) == 0)
		{
			if ((signed int)(spr->field_81 + 1) < (unsigned __int16)v3[6])
			{
				if ((spr->field_99 & 8) != 0 && spr->field_9D <= (int)spr->field_81)
				{
					spr->field_85 = 1;
					spr->field_8D = 1;
				}
			}
			else
			{
				spr->field_85 = 1;
				spr->field_8D = 1;
				spr->field_81 = (unsigned __int16)v3[6] - 1;
			}
		}
		if ((spr->field_99 & 1) != 0)
		{
			if (spr->field_81)
			{
				if ((spr->field_99 & 8) != 0 && spr->field_9D == spr->field_81)
				{
					spr->field_85 = 1;
					spr->field_8D = 1;
					spr->field_81 = (unsigned __int16)v3[6] - 1;
				}
			}
			else
			{
				spr->field_85 = 1;
				spr->field_8D = 1;
				spr->field_81 = 0;
			}
		}
	}
	v13 = (WORD)spr->field_81;
	//spr->flag1 = ptr_abm_tbl[lo_id][60].field_2[(__int16)v13] & 0x1F;
	spr->flag1 += spr->flag0;
	//spr->width = (unsigned __int16)v3[6 * (__int16)v13 + 7];
	//spr->height = (unsigned __int16)v3[6 * (__int16)v13 + 8];
	//spr->bmp_data = (BYTE*)spr->ptr0 + v9 + *(_DWORD*)&v3[6 * (__int16)v13 + 11];
	//if ((ptr_abm_tbl[lo_id][60].field_2[(__int16)v13] & 0x8000u) != 0)
	//	v7 = spr->height * (((unsigned __int16)spr->width + 3) & 0xFFFC) - 1;
	//else
	//	v7 = (spr->height - 1) * (((unsigned __int16)spr->width + 3) & 0xFFFC);
	//spr->bmp = spr->bmp_data[v7];
	//spr->field_95 = pattern_data[lo_id]->field_2[v13];
	if (spr->field_95 == 0xFFFF)
	{
		spr->x3 = 768;
		spr->y3 = 768;
		return 1;
	}
	else
	{
		if (spr->field_6B == 0xFFFF)
		{
			//v6 = 2 * ptr_abm_tbl[lo_id]->field_2[0];
			//v5 = 2 * ptr_abm_tbl[lo_id][30].field_2[0];
			spr->field_6B = 0;
		}
		else if (v13 == spr->field_6B)
		{
			v6 = 0;
			v5 = 0;
		}
		else
		{
			//v6 = 2 * (ptr_abm_tbl[lo_id]->field_2[(__int16)v13] - ptr_abm_tbl[lo_id]->field_2[(__int16)spr->field_6B]);
			//v5 = 2 * (ptr_abm_tbl[lo_id][30].field_2[(__int16)v13] - ptr_abm_tbl[lo_id][30].field_2[(__int16)spr->field_6B]);
			spr->field_6B = v13;
		}
		if ((spr->field_89 & 0x8000) == 0)
		{
			//spr->x0 += v6;
			//spr->x3 = (__int16)(LOWORD(spr->x0) - v3[6 * (__int16)v13 + 9]);
		}
		else
		{
			//spr->x0 -= v6;
			//spr->x3 = (__int16)(LOWORD(spr->x0) + v3[6 * (__int16)v13 + 9] - v3[6 * (__int16)v13 + 7]);
		}
		if ((spr->field_89 & 0x4000) != 0)
		{
			//spr->y0 -= v5;
			//spr->y3 = (__int16)(LOWORD(spr->y0) - v3[6 * (__int16)v13 + 10]);
		}
		else
		{
			//spr->y0 += v5;
			//spr->y3 = (__int16)(v3[6 * (__int16)v13 + 10] + LOWORD(spr->y0) - LOWORD(spr->height));
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
		if (s->id != 0xFFFF)
		{
			if ((s->id & 0x3FFF) >= 0x3800);
				//sub_4013B0(s);
			else
				SetSpriteData(s, s->id);
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
	if (sprt_ent[a1].enabled)
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
		sprt_dat[0].enabled = 1;
		sprt_dat[0].type2 = 0;
		sprt_dat[0].field_1C = 0;
		sprt_dat[0].type3 = 1;
	}
	if (sprt_ent[0].lmy >= 0
		&& (sprt_dat[0].type == 1 || sprt_dat[0].type == 2)
		&& sprt_ent[0].x0 >= sprt_ent[0].lmy
		&& !sprt_dat[0].type0)
	{
		sprt_dat[0].enabled = 1;
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
	sprt_dat[id].enabled = 0;
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

void __cdecl RectOrderLeftRight(RECT* a1)
{
	LONG left; // [esp+0h] [ebp-4h]

	if (a1->left > a1->right)
	{
		left = a1->left;
		a1->left = a1->right;
		a1->right = left;
	}
}

void __cdecl sub_4027E7(int id, int x, int y, int is_double)
{
	if (!id && vm_index5[25] == 1)
		is_double = 1;
	if (y >= x || sprt_ent[id].lmx >= x)
	{
		if (y >= x && (sprt_ent[id].lmy < 0 || sprt_ent[id].lmy > x))
		{
			sprt_dat[id].type3 = 0;
			if (is_double)
			{
				sprt_dat[id].type2 = 2;
				sprt_dat[id].field_1C = 0;
			}
			else
			{
				sprt_dat[id].type2 = 1;
				sprt_dat[id].field_1C = 8;
			}
		}
	}
	else
	{
		sprt_dat[id].type3 = 1;
		if (is_double)
		{
			sprt_dat[id].type2 = 2;
			sprt_dat[id].field_1C = 0;
		}
		else
		{
			sprt_dat[id].type2 = 1;
			sprt_dat[id].field_1C = 8;
		}
	}
}

void Vm_spr_walk_x(int id, int x0, int x1, int a4, int a5)
{
	static byte word_420144[] = { 12, 12, 10 };

	__int16 v5; // cx
	RECT a1; // [esp+0h] [ebp-1Ch] BYREF
	__int16 v7; // [esp+10h] [ebp-Ch]
	__int16 v8; // [esp+12h] [ebp-Ah]
	__int16 v9; // [esp+14h] [ebp-8h]
	__int16 v10; // [esp+16h] [ebp-6h]
	__int16 v11; // [esp+18h] [ebp-4h]
	__int16 x; // [esp+1Ah] [ebp-2h]
	__int16 a2a; // [esp+28h] [ebp+Ch]
	__int16 a3a; // [esp+2Ch] [ebp+10h]

	v10 = 8;
	v9 = 8;
	a1.left = x0;
	a1.right = x1;
	RectOrderLeftRight(&a1);
	a2a = (short)a1.left;
	a3a = (short)a1.right;
	v8 = (short)(a1.right - a1.left) / 2;
	if (v8 < v10)
	{
		if (v8)
			v5 = v8 - 1;
		else
			v5 = 0;
		v9 = v5;
	}
	x = sprt_ent[id].x0;
	if (sprt_dat[id].type == 3)
	{
		if (sprt_dat[id].type0)
			x -= word_420144[vm_index5[4]];
		else
			x += word_420144[vm_index5[4]];
	}
	if (v8 + a1.left <= x)
	{
		sprt_dat[id].field_1A = 1;
		vm_index5[29] = 1;
	}
	else
	{
		sprt_dat[id].field_1A = 0;
		vm_index5[29] = 0;
	}
	if (a4 != -1)
		sprt_dat[id].field_1A = a4 >> 3;
	if (a2a - v10 <= x)
	{
		if (v10 + a3a >= x)
		{
			if (v9 + a2a < x)
			{
				if (a3a - v9 > x)
				{
					v7 = 2;
					if (v8 + a2a < x)
						v11 = a3a;
					else
						v11 = a2a;
				}
				else
				{
					v7 = 1;
					v11 = a3a;
				}
			}
			else
			{
				v7 = 1;
				v11 = a2a;
			}
		}
		else
		{
			v7 = 0;
			v11 = a3a;
		}
	}
	else
	{
		v7 = 0;
		v11 = a2a;
	}
	sprt_dat[id].field_18 = v11;
	if (v7)
	{
		if ((unsigned int)(v7 - 1) < 2)
			sprt_dat[id].type4 = 2;
	}
	else
	{
		sub_4027E7(id, x, v11, a5);
		sprt_dat[id].type4 = 1;
		sprt_dat[id].field_1C = 0;
	}
}

int sub_401F05()
{
	static WORD word_41FF54[4][4] =
	{
		349, 350,33117,33118, 349, 350,33117,33118,  73,  74,32841,32842
	};

	switch (sprt_dat[0].type)
	{
	case 3u:
		vm_index5[2] += 2;
		if (vm_index5[2] > 0x320u)
			vm_index5[2] = 800;
		if (sprt_dat[0].type2 != 3)
		{
			sprt_dat[0].type = 5;
			SprAnim(0, word_41FF54[vm_index5[4]][2 * sprt_dat[0].type0 + 1], 0, 0);
		}
		return 1;
	case 4u:
		if (sprt_ent[0].field_8D)
		{
			sprt_dat[0].type = 3;
			sprt_dat[0].type2 = 3;
		}
		return 1;
	case 5u:
		if (sprt_ent[0].field_8D)
		{
			sprt_dat[0].type = 0;
			return 0;
		}
		else
		{
			return 1;
		}
	default:
		if (!sprt_dat[0].field_20)
			return 0;
		if (--sprt_dat[0].field_20
			|| sprt_ent[0].x0 - 80 < sprt_ent[0].lmx
			|| sprt_ent[0].lmy >= 0 && sprt_ent[0].x0 + 80 > sprt_ent[0].lmy)
		{
			return 0;
		}
		else
		{
			sprt_dat[0].type = 4;
			sprt_dat[0].type2 = 4;
			SprAnim(0, word_41FF54[vm_index5[4]][2 * sprt_dat[0].type0], 0, 0);
			sprt_dat[0].field_14 = 0;
			return 1;
		}
	}
}

void __cdecl sub_402DE5(int id)
{
	RECT a1; // [esp+0h] [ebp-10h] BYREF

	switch (sprt_dat[id].type4)
	{
	case 1u:
		a1.left = sprt_ent[id].x0;
		a1.right = sprt_ent[id].x1;
		RectOrderLeftRight(&a1);
		if (sprt_dat[id].field_18 >= LOWORD(a1.left) && sprt_dat[id].field_18 <= LOWORD(a1.right))
			sprt_dat[id].type4 = 2;
		break;
	case 2u:
		sprt_ent[id].SetXY(sprt_dat[id].field_18, sprt_ent[id].y0, sprt_ent[id].flag0, 0);
		if (sprt_dat[id].type == 1 || sprt_dat[id].type == 2)
		{
			sprt_dat[id].enabled = 1;
			sprt_dat[id].type2 = 0;
		}
		sprt_dat[id].type4 = 3;
		break;
	case 3u:
		if (sprt_dat[id].field_1A == LOWORD(sprt_dat[id].type0))
		{
			sprt_dat[id].type4 = 0;
		}
		else
		{
			sprt_dat[id].type3 = (__int16)sprt_dat[id].field_1A;
			sprt_dat[id].type4 = 4;
		}
		break;
	case 4u:
		if (sprt_ent[id].field_8D)
			sprt_dat[id].type4 = 0;
		break;
	}
}

void sub_4020BA()
{
	WORD a2[2];

	if (sprt_dat[0].field_1C)
	{
		--sprt_dat[0].field_1C;
	}
	else if (sprt_dat[0].type0 != sprt_dat[0].type3 || sprt_dat[0].type != sprt_dat[0].type2)
	{
		sub_4033A4(a2, 0);
		if (prog.field_128 && sprt_dat[0].type == 2)
			sprt_dat[0].enabled = 1;
		if (sprt_dat[0].enabled)
		{
			if (a2[1])
				a2[0] = a2[1];
		}
		SprAnim(0, a2[0], 0, 0);
		sprt_dat[0].field_14 = a2[1];
		sprt_dat[0].enabled = 0;
		sprt_dat[0].type0 = sprt_dat[0].type3;
		sprt_dat[0].type = sprt_dat[0].type2;
	}
}

void sub_401D32()
{
	sub_401DB5();
	sub_401EB7(0);
	if (!sub_401F05())
	{
		if (sprt_dat[0].type == 2)
		{
			if (vm_index5[2])
				--vm_index5[2];
		}
		sub_4020BA();
		sub_402DE5(0);
	}
}

void sub_40266A()
{
	struct tagRECT r1; // [esp+0h] [ebp-24h] BYREF
	RECT r0; // [esp+10h] [ebp-14h] BYREF
	int v2; // [esp+20h] [ebp-4h]

	if (!vm_index5[41])
	{
		r0.left = sprt_ent[0].x0;
		r0.right = sprt_ent[0].x1;
		r0.top = sprt_ent[0].y0;
		r0.bottom = sprt_ent[0].y0 + 1;
		RectOrderLeftRight(&r0);
		v2 = 0;
		while ((vm_index6[v2 + 10] & 0x10) == 0 || !sub_403304(v2) || !IntersectRect(&r1, &r0, &vm_rects[v2]))
		{
			if (++v2 >= 30)
				return;
		}
		Vm_mark_event(v2 + 10, 1u);
	}
}

void __cdecl sub_401E61()
{
	if (sprt_ent[1].lmx >= 0 && sprt_ent[1].x0 <= sprt_ent[1].lmx && sprt_dat[1].type0 == 1)
		sprt_dat[1].type3 = 0;
	if (sprt_ent[1].lmy >= 0 && sprt_ent[1].x0 >= sprt_ent[1].lmy && !sprt_dat[1].type0)
		sprt_dat[1].type3 = 1;
}

void __cdecl sub_402053()
{
	if (vm_index5[25] == 1 && vm_index5[39] && sprt_ent[1].field_8D && ++sprt_dat[1].field_24 >= 2u)
	{
		sprt_dat[1].field_24 = 0;
		sprt_dat[1].type2 = 1;
		sprt_dat[1].type3 = sprt_ent[1].x0 - sprt_ent[0].x0 >= 0;
	}
}

void __cdecl sub_402178()
{
	WORD a2[2]; // [esp+0h] [ebp-4h] BYREF

	if (sprt_dat[1].type0 != sprt_dat[1].type3 || sprt_dat[1].type != sprt_dat[1].type2)
	{
		sub_4033A4(a2, 1);
		SprAnim(1u, a2[0], 0, 0);
		sprt_dat[1].field_14 = a2[1];
		sprt_dat[1].type0 = sprt_dat[1].type3;
		sprt_dat[1].type = sprt_dat[1].type2;
	}
}

void sub_401D74()
{
	if (vm_index5[25] && !vm_index5[27])
	{
		if (sprt_ent[1].enabled)
		{
			sub_401E61();
			sub_402053();
			sub_401EB7(1u);
			sub_402178();
			sub_402DE5(1);
		}
	}
}

__int16 sub_4022ED()
{
	static WORD word_420374[][2] =
	{
		0x64, 0x50,
		0x3C, 0x32,
		0x32, 0x28
	};

	unsigned __int16 v1; // [esp+6h] [ebp-Ah]
	__int16 v2; // [esp+8h] [ebp-8h]
	__int16 v3; // [esp+Ah] [ebp-6h]
	int v4; // [esp+Ch] [ebp-4h]

	v3 = 0;
	v2 = LOWORD(sprt_ent[1].x0) - LOWORD(sprt_ent[0].x0);
	v1 = abs(LOWORD(sprt_ent[1].x0) - LOWORD(sprt_ent[0].x0));
	if (vm_index5[25] == 1)
	{
		if (v2 <= 0)
		{
			if (!sprt_dat[1].type0 && word_420374[vm_index5[4]][0] >= v1
				|| sprt_dat[1].type0 == 1 && word_420374[vm_index5[4]][1] >= v1)
			{
				return 2;
			}
		}
		else if (sprt_dat[1].type0 == 1 && word_420374[vm_index5[4]][0] >= v1
			|| !sprt_dat[1].type0 && word_420374[vm_index5[4]][1] >= v1)
		{
			return 1;
		}
	}
	else if (vm_index5[25] == 2)
	{
		static WORD word_420380[2] = { 0x5A, 0x50 };

		v4 = 0;
		if (sprt_ent[0].x0 - 200 < sprt_ent[0].lmx || sprt_ent[0].lmy >= 0 && sprt_ent[0].x0 + 200 > sprt_ent[0].lmy)
			v4 = 1;
		if (word_420380[v4] >= v1)
		{
			if (v2 <= 0)
				return 2;
			else
				return 1;
		}
	}
	return v3;
}

void sub_4021DC()
{
	__int16 v0; // [esp+0h] [ebp-2h]

	if (vm_index5[25])
	{
		if (vm_index5[26])
		{
			if (!vm_index5[27] && vm_index5[4] == vm_index5[6] && vm_index5[3] == vm_index5[5])
			{
				v0 = sub_4022ED();
				if (v0)
				{
					if ((vm_index3[14] & 1) != 0)
					{
						if (!sprt_dat[0].type4 && !prog.field_128)
							return;
						sprt_dat[0].type4 = 0;
						prog.field_128 = 0;
						Vm_set_63();
					}
					vm_index5[27] = v0;
					sprt_dat[0].field_20 = 0;
					Vm_mark_event(0x191u, 0);
					sprt_dat[0].type4 = 0;
					sprt_dat[1].type4 = 0;
					if (sprt_ent[0].x0 - 200 < sprt_ent[0].lmx || sprt_ent[0].lmy >= 0 && sprt_ent[0].x0 + 200 > sprt_ent[0].lmy)
						vm_index5[41] = 1;
				}
			}
		}
	}
}

int intersect_triggers(int x, int y)
{
	struct tagRECT rcDst; // [esp+0h] [ebp-24h] BYREF
	struct tagRECT rc; // [esp+10h] [ebp-14h] BYREF
	int i; // [esp+20h] [ebp-4h]

	if (vm_index5[41])
		return -1;
	SetRect(&rc, x + 1, y + 1, x + 2, y + 2);
	for (i = 0; i < 30; ++i)
	{
		if ((vm_index6[i + 10] & 0x10) == 0)
		{
			if (sub_403304(i))
			{
				rcDst.left = prog.render_rect.left + vm_rects[i].left - prog.screen_x;
				rcDst.right = prog.render_rect.left + vm_rects[i].right - prog.screen_x;
				rcDst.top = prog.render_rect.top + vm_rects[i].top - prog.screen_y;
				rcDst.bottom = prog.render_rect.top + vm_rects[i].bottom - prog.screen_y;
				if (IntersectRect(&rcDst, &rc, &rcDst))
					return i;
			}
		}
	}
	return -1;
}

void RBtnClick(int is_double, LONG x, LONG y)
{
	struct tagRECT rcDst; // [esp+0h] [ebp-20h] BYREF
	RECT rcSrc; // [esp+10h] [ebp-10h] BYREF

	if ((prog.click_bits & 1) == 0)
	{
		prog.click_bits |= 2u;
		SetRect(&rcSrc, x, y, x + 1, y + 1);
		if (prog.vm_func == 1)
		{
			if (vm_index5[28])
			{
				if (vm_index5[28] != 0xFFFF)
					++vm_index5[28];
				if (vm_index5[44])
				{
					if (vm_index5[44] != 0xFFFF)
						++vm_index5[44];
				}
			}
			else if (IntersectRect(&rcDst, &rcSrc, &prog.render_rect) && prog.field_12E && sprt_dat[0].type <= 2)
			{
				sprt_dat[0].type2 = 0;
				sprt_dat[0].field_1C = 0;
				if (!sprt_dat[0].field_20 && !vm_index5[40])
					sprt_dat[0].field_20 = 100;
				if (sprt_dat[0].type4)
				{
					sprt_dat[0].type4 = 0;
					Vm_set_63();
					vm_index5[45] = 1;
				}
			}
		}
	}
}

void LBtnClick(int is_double, LONG x, LONG y)
{
	struct tagRECT rcDst; // [esp+0h] [ebp-38h] BYREF
	RECT click; // [esp+10h] [ebp-28h] BYREF
	int v7; // [esp+24h] [ebp-14h]
	int v9; // [esp+2Ch] [ebp-Ch]
	UINT v10; // [esp+30h] [ebp-8h]
	int TickCount; // [esp+34h] [ebp-4h]

	if (!is_double)
	{
		TickCount = getTime();
		v10 = TickCount - prog.tick;
		prog.tick = TickCount;
		if (GetDoubleClickTime() > v10)
			is_double = 1;
	}
	prog.click_bits |= 1u;
	click.left = x;
	click.right = x + 1;
	click.top = y;
	click.bottom = y + 1;
	if (prog.vm_func == 1)
	{
		// check if it's inside the game area
		if (IntersectRect(&rcDst, &click, &prog.render_rect))
		{
			vm_index3[22] = prog.screen_x + x - prog.render_rect.left <= sprt_ent[0].x0;
			if ((prog.field_12E || prog.field_130) && sprt_dat[0].type != 4 && sprt_dat[0].type != 5)
			{
				sprt_dat[0].field_20 = 0;
				v9 = intersect_triggers(x, y);
				if (v9 == -1)
				{
					if (prog.field_12E && !sprt_dat[0].type4)
					{
						if (vm_index5[40])
							is_double = 0;
						sub_4027E7(0, sprt_ent[0].x0, prog.screen_x + x - prog.render_rect.left, is_double);
					}
				}
				else
					if (Vm_mark_event(v9 + 10, 0))
				{
					sprt_dat[0].type2 = 0;
					sprt_dat[0].field_1C = 0;
					prog.field_128 = 1;
				}
			}
		}
		// check if it's inside the inventory space
		else if (vm_index5[45] && IntersectRect(&rcDst, &click, &prog.menu_rect))
		{
			v7 = -1;
			int i;
			for (i = 0; i < 15; i++)
			{
				RECT r;
				SetRect(&r, item_xy_tbl[i][0] * 2, item_xy_tbl[i][1] * 2, item_xy_tbl[i][0] * 2 + 32, item_xy_tbl[i][1] * 2 + 32);
				if (IntersectRect(&rcDst, &click, &r))
				{
					v7 = i;
					break;
				}
			}

			if (v7 != -1 && inventory[v7] != 0xffff)
			{
				itemptr2_index = v7;
				prog.field_13C = 1;
			}
		}
	}
}
