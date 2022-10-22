#include <stdafx.h>
#include "game.h"

SPRT_ENT sprt_ent[21];
AI_ENT ai_ent[2];

void SPRT_ENT::Link()
{
	if (linked)
	{
		if (prev)
			prev->next = next;
		else
			prog.sprt = next;
		if (next)
			next->prev = prev;
		prev = nullptr;
		next = nullptr;
		linked = 0;
	}
}

void SPRT_ENT::SetList()
{
	SPRT_ENT* sprt;
	SPRT_ENT* i;

	if (linked)
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
	linked = 1;
}

void SPRT_ENT::Release()
{
	sprt_ent[id2].enabled = 0;
	if (/*field_26 &&*/ tim)
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
		x0 = x2;
		y0 = y2;
		x1 = x2;
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
	int vol; // [esp+0h] [ebp-8h]
	int pan; // [esp+2h] [ebp-6h]
	int v3; // [esp+4h] [ebp-4h]
	unsigned __int16 v4; // [esp+6h] [ebp-2h]

	if (!field_45 && field_41 == 1)
	{
		v4 = (frame_id & 0x3FFF) >> 8;
		if (v4 <= 0x12u)
		{
			v3 = -1;//off_41F094[v4][field_95];
			if (v3 != -1)
			{
				if (vm_data.vm_index5[47])
				{
					switch (v3)
					{
					case 0: v3 = 2; break;
					case 1: v3 = 3; break;
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

void Vm_spr_lmt(int id, int lmx, int lmy)
{
	sprt_ent[id].lmx = lmx;
	sprt_ent[id].lmy = lmy;
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
		sprt_ent[id].frame_id = -1;
		sub_404346(id, 1);
		sprt_ent[id].field_41 = 0;
		sprt_ent[id].is_busy = 0;
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
	SPRT_ENT* next;
	SPRT_ENT* sprt;

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

WORD word_41FDA4[][36][2] =
{
  {
	{ 258u, 0u },
	{ 270u, 0u },
	{ 264u, 271u },
	{ 261u, 258u },
	{ 261u, 270u },
	{ 261u, 271u },
	{ 258u, 0u },
	{ 270u, 0u },
	{ 264u, 271u },
	{ 261u, 258u },
	{ 261u, 270u },
	{ 262u, 271u },
	{ 263u, 258u },
	{ 270u, 0u },
	{ 271u, 0u },
	{ 262u, 258u },
	{ 262u, 270u },
	{ 262u, 271u },
	{ 33026u, 0u },
	{ 33038u, 0u },
	{ 33032u, 33039u },
	{ 33029u, 33026u },
	{ 33029u, 33038u },
	{ 33029u, 33039u },
	{ 33026u, 0u },
	{ 33038u, 0u },
	{ 33032u, 33039u },
	{ 33029u, 33026u },
	{ 33029u, 33038u },
	{ 33030u, 33039u },
	{ 33031u, 33026u },
	{ 33038u, 0u },
	{ 33039u, 0u },
	{ 33030u, 33026u },
	{ 33030u, 33038u },
	{ 33030u, 33039u }
  },
  {
	{ 1112u, 0u },
	{ 1087u, 0u },
	{ 1096u, 0u },
	{ 1086u, 1112u },
	{ 1086u, 1087u },
	{ 1086u, 1096u },
	{ 1112u, 0u },
	{ 1087u, 0u },
	{ 1096u, 0u },
	{ 1086u, 1112u },
	{ 1086u, 1087u },
	{ 1098u, 1096u },
	{ 1099u, 1112u },
	{ 1087u, 0u },
	{ 1096u, 0u },
	{ 1098u, 1112u },
	{ 1098u, 1087u },
	{ 1098u, 1096u },
	{ 33880u, 0u },
	{ 33855u, 0u },
	{ 33864u, 0u },
	{ 33854u, 33880u },
	{ 33854u, 33855u },
	{ 33854u, 33864u },
	{ 33880u, 0u },
	{ 33855u, 0u },
	{ 33864u, 0u },
	{ 33854u, 33880u },
	{ 33854u, 33855u },
	{ 33866u, 33864u },
	{ 33867u, 33880u },
	{ 33855u, 0u },
	{ 33864u, 0u },
	{ 33866u, 33880u },
	{ 33866u, 33855u },
	{ 33866u, 33864u }
  },
  {
	{ 1115u, 0u },
	{ 1089u, 0u },
	{ 1097u, 0u },
	{ 1088u, 1115u },
	{ 1088u, 1089u },
	{ 1088u, 1097u },
	{ 1115u, 0u },
	{ 1089u, 0u },
	{ 1097u, 0u },
	{ 1088u, 1115u },
	{ 1088u, 1089u },
	{ 1100u, 1097u },
	{ 1101u, 1115u },
	{ 1089u, 0u },
	{ 1097u, 0u },
	{ 1100u, 1115u },
	{ 1100u, 1089u },
	{ 1100u, 1097u },
	{ 33883u, 0u },
	{ 33857u, 0u },
	{ 33865u, 0u },
	{ 33856u, 33883u },
	{ 33856u, 33857u },
	{ 33856u, 33865u },
	{ 33883u, 0u },
	{ 33857u, 0u },
	{ 33865u, 0u },
	{ 33856u, 33883u },
	{ 33856u, 33857u },
	{ 33868u, 33865u },
	{ 33869u, 33883u },
	{ 33857u, 0u },
	{ 33865u, 0u },
	{ 33868u, 33883u },
	{ 33868u, 33857u },
	{ 33868u, 33865u }
  }
};

WORD word_41FF6C[][36][2] =
{
  {
	{ 36864u, 0u },
	{ 36865u, 0u },
	{ 36868u, 36866u },
	{ 36867u, 36864u },
	{ 36867u, 36865u },
	{ 36867u, 36866u },
	{ 36864u, 0u },
	{ 36865u, 0u },
	{ 36868u, 36866u },
	{ 36867u, 36864u },
	{ 36867u, 36865u },
	{ 36869u, 36866u },
	{ 36870u, 36864u },
	{ 36865u, 0u },
	{ 36866u, 0u },
	{ 36869u, 36864u },
	{ 36869u, 36865u },
	{ 36869u, 36866u },
	{ 4096u, 0u },
	{ 4097u, 0u },
	{ 4100u, 4098u },
	{ 4099u, 4096u },
	{ 4099u, 4097u },
	{ 4099u, 4098u },
	{ 4096u, 0u },
	{ 4097u, 0u },
	{ 4100u, 4098u },
	{ 4099u, 4096u },
	{ 4099u, 4097u },
	{ 4101u, 4098u },
	{ 4102u, 4096u },
	{ 4097u, 0u },
	{ 4098u, 0u },
	{ 4101u, 4096u },
	{ 4101u, 4097u },
	{ 4101u, 4098u }
  },
  {
	{ 37376u, 0u },
	{ 37377u, 0u },
	{ 37380u, 37378u },
	{ 37379u, 37376u },
	{ 37379u, 37377u },
	{ 37379u, 37378u },
	{ 37376u, 0u },
	{ 37377u, 0u },
	{ 37378u, 0u },
	{ 37379u, 37376u },
	{ 37379u, 37377u },
	{ 37381u, 37378u },
	{ 37382u, 37376u },
	{ 37377u, 0u },
	{ 37378u, 0u },
	{ 37381u, 37376u },
	{ 37381u, 37377u },
	{ 37381u, 37378u },
	{ 4608u, 0u },
	{ 4609u, 0u },
	{ 4612u, 4610u },
	{ 4611u, 4608u },
	{ 4611u, 4609u },
	{ 4611u, 4610u },
	{ 4608u, 0u },
	{ 4609u, 0u },
	{ 4610u, 0u },
	{ 4611u, 4608u },
	{ 4611u, 4609u },
	{ 4613u, 4610u },
	{ 4614u, 4608u },
	{ 4609u, 0u },
	{ 4610u, 0u },
	{ 4613u, 4608u },
	{ 4613u, 4609u },
	{ 4613u, 4610u }
  },
  {
	{ 37632u, 0u },
	{ 37633u, 0u },
	{ 37636u, 37634u },
	{ 37635u, 37632u },
	{ 37635u, 37633u },
	{ 37635u, 37634u },
	{ 37632u, 0u },
	{ 37633u, 0u },
	{ 37634u, 0u },
	{ 37635u, 37632u },
	{ 37635u, 37633u },
	{ 37637u, 37634u },
	{ 37638u, 37632u },
	{ 37633u, 0u },
	{ 37634u, 0u },
	{ 37637u, 37632u },
	{ 37637u, 37633u },
	{ 37637u, 37634u },
	{ 4864u, 0u },
	{ 4865u, 0u },
	{ 4868u, 4866u },
	{ 4867u, 4864u },
	{ 4867u, 4865u },
	{ 4867u, 4866u },
	{ 4864u, 0u },
	{ 4865u, 0u },
	{ 4866u, 0u },
	{ 4867u, 4864u },
	{ 4867u, 4865u },
	{ 4869u, 4866u },
	{ 4870u, 4864u },
	{ 4865u, 0u },
	{ 4866u, 0u },
	{ 4869u, 4864u },
	{ 4869u, 4865u },
	{ 4869u, 4866u }
  }
};

WORD word_42011C[][2][2] =
{
  { { 3072u, 3073u }, { 35840u, 35841u } },
  { { 3109u, 3074u }, { 35877u, 35842u } },
  { { 3076u, 3075u }, { 35844u, 35843u } },
  { { 3080u, 3081u }, { 35848u, 35849u } },
  { { 3072u, 3085u }, { 35840u, 35853u } }
};

int __cdecl sub_4033A4(WORD* dst, int a2)
{
	DWORD v3;
	DWORD v4;

	if (a2)
	{
		if (a2 != 1)
			return 0;
		if (vm_data.vm_index5[25])
		{
			if (vm_data.vm_index5[25] == 1)
			{
				v4 = ai_ent[1].type2 + 6 * ai_ent[1].type + 18 * ai_ent[1].type3;
				if (ai_ent[1].type0 != ai_ent[1].type3)
					v4 += 3;
				dst[0] = word_41FF6C[vm_data.vm_index5[6]][v4][0];
				dst[1] = word_41FF6C[vm_data.vm_index5[6]][v4][1];
			}
			else if (vm_data.vm_index5[25] == 2)
			{
				if ((int)ai_ent[1].type3 > 1)
					ai_ent[1].type3 = 1;
				dst[0] = word_42011C[vm_data.vm_index5[43]][ai_ent[1].type3][ai_ent[1].type2];
				dst[1] = 0;
			}
		}
	}
	else
	{
		v3 = ai_ent[0].type2 + 6 * ai_ent[0].type + 18 * ai_ent[0].type3;
		if (ai_ent[0].type0 != ai_ent[0].type3)
			v3 += 3;
		dst[0] = word_41FDA4[vm_data.vm_index5[4]][v3][0];
		dst[1] = word_41FDA4[vm_data.vm_index5[4]][v3][1];
	}
	return 1;
}

void SprAnim(unsigned int id, WORD anim, WORD a3, WORD a4)
{
	WORD v5[2];

	if (sprt_ent[id].enabled)
	{
		if (anim == 0xFFFF)
		{
			if (sub_4033A4(v5, id))
			{
				sprt_ent[id].frame_id = v5[0];
				ai_ent[id].anim = v5[1];
				ai_ent[id].enabled = 0;
				ai_ent[id].type0 = ai_ent[id].type3;
				ai_ent[id].type = ai_ent[id].type2;
			}
		}
		else
		{
			sprt_ent[id].frame_id = anim;
		}
		sprt_ent[id].field_99 = a3;
		sprt_ent[id].field_9B = a4 & 0xff;
		sprt_ent[id].field_9D = a4 >> 8;
		sprt_ent[id].Update();
		sprt_ent[id].UpdateXY();
		sub_404346(id, 0);
		sprt_ent[id].field_41 = 0;
		sprt_ent[id].is_busy = 0;
		if (id <= 1)
			ai_ent[id].anim = 0;
	}
}

void SprCursorAnimate()
{
	CRect rtrg, rcur;
	int i;

	rcur.SetXYWH(prog.mousePT.x + 1, prog.mousePT.y + 1, 1, 1);
	if (prog.cur_enabled)
	{
		i = 0;
		while (1)
		{
			if ((vm_data.vm_index6[i + 10] & 0x10) == 0 && sub_403304(i))
			{
				rtrg.Set(prog.render_rect.X0() + vm_data.vm_rects[i].X0() - prog.screen_x,
					prog.render_rect.X0() + vm_data.vm_rects[i].X1() - prog.screen_x,
					prog.render_rect.Y0() + vm_data.vm_rects[i].Y0() - prog.screen_y,
					prog.render_rect.Y0() + vm_data.vm_rects[i].Y1() - prog.screen_y);
				if (intersectRect(&rcur, &rtrg))
					break;
			}
			if (++i >= 30)
				goto LABEL_15;
		}
		prog.triggerX = vm_data.vm_rects[i].X0() - (32 - vm_data.vm_rects[i].W()) / 2;
		prog.triggerY = vm_data.vm_rects[i].Y0() - (32 - vm_data.vm_rects[i].H()) / 2;
		if (prog.field_14C)
		{
			if (prog.field_14C == 1)
			{
				if (sprt_ent[11].is_busy)
				{
					sprt_ent[11].SetXY(prog.triggerX, prog.triggerY, 0x64u, 1);
					prog.field_14C = 2;
				}
			}
			else if (prog.field_14C == 3)
			{
				prog.field_14C = 1;
				sprt_ent[11].field_99 = 0;
				sprt_ent[11].Update();
				sprt_ent[11].SetXY(prog.triggerX, prog.triggerY, 0x64u, 1);
			}
		}
		else
		{
			prog.cur_type1 = showCursor(0);
			prog.field_14C = 1;
			sprt_ent[11].field_99 = 0;
			sprt_ent[11].Update();
			sprt_ent[11].SetXY(prog.triggerX, prog.triggerY, 0x64u, 1);
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
			if (sprt_ent[11].is_busy)
			{
				prog.field_14C = 0;
				if (prog.cur_type1 < 0)
					prog.cur_type1 = showCursor(1);
			}
		}
	}
}

void SpriteGetRect(SPRT_ENT* s, CRect* r)
{
	r->SetXYWH(s->x3, s->width, s->y3, s->height);
}

void SprDraw(SPRT_ENT* sprt, CRect* lprcSrc)
{
	CRect rxy, rspr, rcopy;
	int srcx, srcy, dstx, dsty;

	SpriteGetRect(sprt, &rspr);
	if (lprcSrc)
	{
		copyRect(&rcopy, lprcSrc);
		if (!intersectRect(&rspr, &rcopy))
			return;
	}
	else
	{
		TMapGetDstRect(&tmap, &rcopy);
		if (!intersectRect(&rspr, &rcopy))
			return;
	}

	TMapGetRect(&tmap, &rxy);
	if ((signed int)sprt->x3 < rcopy.X0())
	{
		dstx = rxy.X0();
		srcx = rcopy.X0() - sprt->x3;
	}
	else
	{
		dstx = rxy.X0() + sprt->x3 - rcopy.X0();
		srcx = 0;
	}
	if ((signed int)sprt->y3 < rcopy.Y0())
	{
		dsty = rxy.Y0();
		srcy = rcopy.Y0() - sprt->y3;
	}
	else
	{
		dsty = rxy.Y0() + sprt->y3 - rcopy.Y0();
		srcy = 0;
	}

	RenderRect(sprt->tim, GETX(dstx), GETY(dsty), sprt->width, sprt->height, srcx, srcy, 0xff, 0xff, 0xff);
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

void CursorDispCk()
{
	CRect rcSrc1;

	rcSrc1.SetXYWH(prog.mousePT.x + 1, 1, prog.mousePT.y + 1, 1);
	if (prog.vm_func != 1 || !intersectRect(&rcSrc1, &prog.render_rect) || prog.cur_enabled && !prog.field_14C)
	{
		if (prog.cur_type1 < 0)
			prog.cur_type1 = showCursor(1);
	}
	else if (prog.cur_type1 >= 0)
	{
		prog.cur_type1 = showCursor(0);
	}
}

__int16 sub_403619()
{
	if (vm_data.vm_index5[2] <= 200)
		return 0;
	if (vm_data.vm_index5[2] <= 400)
		return 1;
	if (vm_data.vm_index5[2] > 600)
		return 3;
	return 2;
}

__int16 sub_4035DC()
{
	WORD v3; // al

	if (!vm_data.vm_index5[28])
		return 3 - sub_403619();
	v3 = prog.field_194;
	--prog.field_194;
	return v3 & 3;
}

void SprUpdate(SPRT_ENT* s)
{
	unsigned int id;

	if (s->is_busy)
	{
		if ((s->field_99 & 0x10) != 0)
			goto update;
		id = s->frame_id & 0x3FFF;
		if (id > 3081)
		{
			if (id == 3085 || (unsigned int)id - 4097 < 2 || (unsigned int)id - 4609 < 2 || (unsigned int)id - 4865 < 2)
				goto update;
		}
		else
		{
			if (id == 3081)
				goto update;
			if (id <= 1089)
			{
				if (id != 1089 && (unsigned int)id - 270 >= 2 && id != 1087)
					return;
update:
				s->Update();
				return;
			}
			if ((unsigned int)id - 1096 < 2 || (unsigned int)id - 3073 < 3)
				goto update;
		}
	}
}

void SprSetList(SPRT_ENT* s)
{
	s->SetList();
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

int SetSpriteData(SPRT_ENT* spr, unsigned int id)
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
			spr->is_busy = 1;
		if (spr->is_busy)
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

	// no sprite data, assign
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
		spr->is_busy = 0;
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
		//memcpy(&v3[6 * i + 7], (char*)Src + trg, 0xCu);
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
		spr->is_busy = 0;
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
			//spr->field_85 = (pattern_data[lo_id][30].field_2[v13] + 1) / 2;
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
					spr->is_busy = 1;
				}
			}
			else
			{
				spr->field_85 = 1;
				spr->is_busy = 1;
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
					spr->is_busy = 1;
					spr->field_81 = (unsigned __int16)v3[6] - 1;
				}
			}
			else
			{
				spr->field_85 = 1;
				spr->is_busy = 1;
				spr->field_81 = 0;
			}
		}
	}
	v13 = (WORD)spr->field_81;
	//spr->flag1 = ptr_abm_tbl[lo_id][60].field_2[(__int16)v13] & 0x1F;
	spr->flag1 += spr->flag0;
	//spr->width = (unsigned __int16)v3[6 * (__int16)v13 + 7];
	//spr->height = (unsigned __int16)v3[6 * (__int16)v13 + 8];
	//spr->bmp_data = (BYTE*)spr->ptr0 + trg + *(_DWORD*)&v3[6 * (__int16)v13 + 11];
	//if ((ptr_abm_tbl[lo_id][60].field_2[(__int16)v13] & 0x8000u) != 0)
	//	mmode = spr->height * (((unsigned __int16)spr->width + 3) & 0xFFFC) - 1;
	//else
	//	mmode = (spr->height - 1) * (((unsigned __int16)spr->width + 3) & 0xFFFC);
	//spr->bmp = spr->bmp_data[mmode];
	//spr->field_95 = pattern_data[lo_id]->field_2[v13];

	// movement
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
			//t3 = 2 * ptr_abm_tbl[lo_id][30].field_2[0];
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
			//t3 = 2 * (ptr_abm_tbl[lo_id][30].field_2[(__int16)v13] - ptr_abm_tbl[lo_id][30].field_2[(__int16)spr->field_6B]);
			spr->field_6B = v13;
		}

		// horizontal
		if ((spr->field_89 & 0x8000) == 0)
		{
			spr->x0 += 4;
			spr->x3 = spr->x0 - 4;
			//spr->x0 += v6;
			//spr->x3 = (__int16)(LOWORD(spr->x0) - v3[6 * (__int16)v13 + 9]);
		}
		else
		{
			spr->x0 -= 4;
			spr->x3 = spr->x0 + 4;
			//spr->x0 -= v6;
			//spr->x3 = (__int16)(LOWORD(spr->x0) + v3[6 * (__int16)v13 + 9] - v3[6 * (__int16)v13 + 7]);
		}

		// vertical
		if ((spr->field_89 & 0x4000) != 0)
		{
			spr->y0 -= 4;
			spr->y3 = spr->y0 - 4;
			//spr->y0 -= t3;
			//spr->y3 = (__int16)(LOWORD(spr->y0) - v3[6 * (__int16)v13 + 10]);
		}
		else
		{
			spr->y0 += 4;
			spr->y3 = spr->y0 - spr->height + 4;
			//spr->y0 += t3;
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
		SprUpdate(s);
		if (s->frame_id != 0xFFFF)
		{
			if ((s->frame_id & 0x3FFF) >= 0x3800);
				//sub_4013B0(s);
			else
				SetSpriteData(s, s->frame_id);
		}
		if (flag1 != s->flag1)
			SprSetList(s);
		s->SetX0();
		s->CalcPan();
	}
	sub_403536();
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
		&& (ai_ent[0].type == 1 || ai_ent[0].type == 2)
		&& sprt_ent[0].x0 <= sprt_ent[0].lmx
		&& ai_ent[0].type0 == 1)
	{
		ai_ent[0].enabled = 1;
		ai_ent[0].type2 = 0;
		ai_ent[0].field_1C = 0;
		ai_ent[0].type3 = 1;
	}
	if (sprt_ent[0].lmy >= 0
		&& (ai_ent[0].type == 1 || ai_ent[0].type == 2)
		&& sprt_ent[0].x0 >= sprt_ent[0].lmy
		&& !ai_ent[0].type0)
	{
		ai_ent[0].enabled = 1;
		ai_ent[0].type2 = 0;
		ai_ent[0].field_1C = 0;
		ai_ent[0].type3 = 0;
	}
}

void __cdecl sub_401EB7(unsigned int id)
{
	if (sprt_ent[id].is_busy)
	{
		if (ai_ent[id].anim)
		{
			SprAnim(id, ai_ent[id].anim, 0, 0);
			ai_ent[id].anim = 0;
		}
	}
}

int EntGetPan(int a1)
{
	return Sound_get_pan(sprt_ent[a1].x0 - prog.screen_x);
}

void sub_4042C0(int id)
{
	ai_ent[id].anim = 0;
	ai_ent[id].type4 = 0;
	ai_ent[id].enabled = 0;
	ai_ent[id].field_1C = 0;
	ai_ent[id].field_20 = 0;
	ai_ent[id].field_24 = 0;
}

void Vm_spr_dir(int id, int a2, int a3, int a4, int a5)
{
	int t3;

	if (a2 != -1)
		ai_ent[id].type0 = a2 >> 3;
	if (a4 == -1)
		t3 = ai_ent[id].type0;
	else
		t3 = a4 >> 3;
	ai_ent[id].type3 = t3;
	if (a3 != -1)
		ai_ent[id].type = a3;
	if (a5 == -1)
		ai_ent[id].type2 = ai_ent[id].type;
	else
		ai_ent[id].type2 = a5;
	sub_4042C0(id);
}

void rectSwapX(CRect* r)
{
	if (r->X0() > r->X1())
		r->SwapX();
}

void SprSetDest(int id, int cur_x, int dst_x, int running)
{
	if (!id && vm_data.vm_index5[25] == 1)
		running = 1;
	if (dst_x >= cur_x || sprt_ent[id].lmx >= cur_x)
	{
		if (dst_x >= cur_x && (sprt_ent[id].lmy < 0 || sprt_ent[id].lmy > cur_x))
		{
			ai_ent[id].type3 = 0;
			if (running)
			{
				ai_ent[id].type2 = 2;
				ai_ent[id].field_1C = 0;
			}
			else
			{
				ai_ent[id].type2 = 1;
				ai_ent[id].field_1C = 8;
			}
		}
	}
	else
	{
		ai_ent[id].type3 = 1;
		if (running)
		{
			ai_ent[id].type2 = 2;
			ai_ent[id].field_1C = 0;
		}
		else
		{
			ai_ent[id].type2 = 1;
			ai_ent[id].field_1C = 8;
		}
	}
}

void Vm_spr_walk_x(int id, int x0, int x1, int a4, int running)
{
	static byte spd_tbl[] = { 12, 12, 10 };

	CRect r;
	int mmode;
	int mleft, mright;
	int dst_x, cur_x;
	int left, right, center;

	mleft = 8;
	mright = 8;
	r.Set(x0, x1, 0, 0);
	rectSwapX(&r);

	left = r.X0();
	right = r.X1();
	center = r.W() / 2;

	if (center < mleft)
	{
		if (center)
			mright = center - 1;
		else
			mright = 0;
	}
	cur_x = sprt_ent[id].x0;
	if (ai_ent[id].type == 3)
	{
		if (ai_ent[id].type0)
			cur_x -= spd_tbl[vm_data.vm_index5[4]];
		else
			cur_x += spd_tbl[vm_data.vm_index5[4]];
	}

	if (center + r.X0() <= cur_x)
	{
		ai_ent[id].field_1A = 1;
		vm_data.vm_index5[29] = 1;
	}
	else
	{
		ai_ent[id].field_1A = 0;
		vm_data.vm_index5[29] = 0;
	}

	if (a4 != -1)
		ai_ent[id].field_1A = a4 >> 3;

	if (left - mleft <= cur_x)
	{
		if (mleft + right >= cur_x)
		{
			if (mright + left < cur_x)
			{
				if (right - mright > cur_x)
				{
					mmode = 2;
					if (center + left < cur_x)
						dst_x = right;
					else
						dst_x = left;
				}
				else
				{
					mmode = 1;
					dst_x = right;
				}
			}
			else
			{
				mmode = 1;
				dst_x = left;
			}
		}
		else
		{
			mmode = 0;
			dst_x = right;
		}
	}
	else
	{
		mmode = 0;
		dst_x = left;
	}

	ai_ent[id].field_18 = dst_x;

	switch (mmode)
	{
	case 0:
		SprSetDest(id, cur_x, dst_x, running);
		ai_ent[id].type4 = 1;
		ai_ent[id].field_1C = 0;
		break;
	case 1:
	case 2:
		ai_ent[id].type4 = 2;
		break;
	}
}

int sub_401F05()
{
	static WORD word_41FF54[4][4] =
	{
		{ 349u, 350u, 33117u, 33118u },
		{ 349u, 350u, 33117u, 33118u },
		{ 73u, 74u, 32841u, 32842u },
		{ 36864u, 0u, 36865u, 0u }
	};

	switch (ai_ent[0].type)
	{
	case 3u:
		vm_data.vm_index5[2] += 2;
		if (vm_data.vm_index5[2] > 800)
			vm_data.vm_index5[2] = 800;
		if (ai_ent[0].type2 != 3)
		{
			ai_ent[0].type = 5;
			SprAnim(0, word_41FF54[vm_data.vm_index5[4]][2 * ai_ent[0].type0 + 1], 0, 0);
		}
		return 1;
	case 4u:
		if (sprt_ent[0].is_busy)
		{
			ai_ent[0].type = 3;
			ai_ent[0].type2 = 3;
		}
		return 1;
	case 5u:
		if (sprt_ent[0].is_busy)
		{
			ai_ent[0].type = 0;
			return 0;
		}
		return 1;
	default:
		if (!ai_ent[0].field_20)
			return 0;

		if (--ai_ent[0].field_20
			|| sprt_ent[0].x0 - 80 < sprt_ent[0].lmx
			|| sprt_ent[0].lmy >= 0 && sprt_ent[0].x0 + 80 > sprt_ent[0].lmy)
			return 0;
		else
		{
			ai_ent[0].type = 4;
			ai_ent[0].type2 = 4;
			SprAnim(0, word_41FF54[vm_data.vm_index5[4]][2 * ai_ent[0].type0], 0, 0);
			ai_ent[0].anim = 0;
			return 1;
		}
	}
}

void __cdecl sub_402DE5(int id)
{
	CRect pt; // [esp+0h] [ebp-10h] BYREF

	switch (ai_ent[id].type4)
	{
	case 1u:
		pt.Set(sprt_ent[id].x0, sprt_ent[id].x1, 0, 0);
		rectSwapX(&pt);
		if (ai_ent[id].field_18 >= pt.X0() && ai_ent[id].field_18 <= pt.X1())
			ai_ent[id].type4 = 2;
		break;
	case 2u:
		sprt_ent[id].SetXY(ai_ent[id].field_18, sprt_ent[id].y0, sprt_ent[id].flag0, 0);
		if (ai_ent[id].type == 1 || ai_ent[id].type == 2)
		{
			ai_ent[id].enabled = 1;
			ai_ent[id].type2 = 0;
		}
		ai_ent[id].type4 = 3;
		break;
	case 3u:
		if (ai_ent[id].field_1A == LOWORD(ai_ent[id].type0))
		{
			ai_ent[id].type4 = 0;
		}
		else
		{
			ai_ent[id].type3 = (__int16)ai_ent[id].field_1A;
			ai_ent[id].type4 = 4;
		}
		break;
	case 4u:
		if (sprt_ent[id].is_busy)
			ai_ent[id].type4 = 0;
		break;
	}
}

void sub_4020BA()
{
	WORD a2[2];

	if (ai_ent[0].field_1C)
	{
		--ai_ent[0].field_1C;
	}
	else if (ai_ent[0].type0 != ai_ent[0].type3 || ai_ent[0].type != ai_ent[0].type2)
	{
		sub_4033A4(a2, 0);
		if (prog.field_128 && ai_ent[0].type == 2)
			ai_ent[0].enabled = 1;
		if (ai_ent[0].enabled)
		{
			if (a2[1])
				a2[0] = a2[1];
		}
		SprAnim(0, a2[0], 0, 0);
		ai_ent[0].anim = a2[1];
		ai_ent[0].enabled = 0;
		ai_ent[0].type0 = ai_ent[0].type3;
		ai_ent[0].type = ai_ent[0].type2;
	}
}

void sub_401D32()
{
	sub_401DB5();
	sub_401EB7(0);
	if (!sub_401F05())
	{
		if (ai_ent[0].type == 2)
		{
			if (vm_data.vm_index5[2])
				--vm_data.vm_index5[2];
		}
		sub_4020BA();
		sub_402DE5(0);
	}
}

void sub_40266A()
{
	CRect r0; // [esp+10h] [ebp-14h] BYREF
	int v2; // [esp+20h] [ebp-4h]

	if (!vm_data.vm_index5[41])
	{
		r0.Set(sprt_ent[0].x0, sprt_ent[0].x1, sprt_ent[0].y0, sprt_ent[0].y0 + 1);
		rectSwapX(&r0);
		v2 = 0;
		while ((vm_data.vm_index6[v2 + 10] & 0x10) == 0 || !sub_403304(v2) || !intersectRect(&r0, &vm_data.vm_rects[v2]))
		{
			if (++v2 >= 30)
				return;
		}
		Vm_mark_event(v2 + 10, 1u);
	}
}

void __cdecl sub_401E61()
{
	if (sprt_ent[1].lmx >= 0 && sprt_ent[1].x0 <= sprt_ent[1].lmx && ai_ent[1].type0 == 1)
		ai_ent[1].type3 = 0;
	if (sprt_ent[1].lmy >= 0 && sprt_ent[1].x0 >= sprt_ent[1].lmy && !ai_ent[1].type0)
		ai_ent[1].type3 = 1;
}

void __cdecl sub_402053()
{
	if (vm_data.vm_index5[25] == 1 && vm_data.vm_index5[39] && sprt_ent[1].is_busy && ++ai_ent[1].field_24 >= 2u)
	{
		ai_ent[1].field_24 = 0;
		ai_ent[1].type2 = 1;
		ai_ent[1].type3 = sprt_ent[1].x0 - sprt_ent[0].x0 >= 0;
	}
}

void __cdecl sub_402178()
{
	WORD a2[2]; // [esp+0h] [ebp-4h] BYREF

	if (ai_ent[1].type0 != ai_ent[1].type3 || ai_ent[1].type != ai_ent[1].type2)
	{
		sub_4033A4(a2, 1);
		SprAnim(1u, a2[0], 0, 0);
		ai_ent[1].anim = a2[1];
		ai_ent[1].type0 = ai_ent[1].type3;
		ai_ent[1].type = ai_ent[1].type2;
	}
}

void sub_401D74()
{
	if (vm_data.vm_index5[25] && !vm_data.vm_index5[27])
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
	if (vm_data.vm_index5[25] == 1)
	{
		if (v2 <= 0)
		{
			if (!ai_ent[1].type0 && word_420374[vm_data.vm_index5[4]][0] >= v1
				|| ai_ent[1].type0 == 1 && word_420374[vm_data.vm_index5[4]][1] >= v1)
			{
				return 2;
			}
		}
		else if (ai_ent[1].type0 == 1 && word_420374[vm_data.vm_index5[4]][0] >= v1
			|| !ai_ent[1].type0 && word_420374[vm_data.vm_index5[4]][1] >= v1)
		{
			return 1;
		}
	}
	else if (vm_data.vm_index5[25] == 2)
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

	if (vm_data.vm_index5[25])
	{
		if (vm_data.vm_index5[26])
		{
			if (!vm_data.vm_index5[27] && vm_data.vm_index5[4] == vm_data.vm_index5[6] && vm_data.vm_index5[3] == vm_data.vm_index5[5])
			{
				v0 = sub_4022ED();
				if (v0)
				{
					if ((vm_data.vm_index3[14] & 1) != 0)
					{
						if (!ai_ent[0].type4 && !prog.field_128)
							return;
						ai_ent[0].type4 = 0;
						prog.field_128 = 0;
						Vm_set_63();
					}
					vm_data.vm_index5[27] = v0;
					ai_ent[0].field_20 = 0;
					Vm_mark_event(0x191u, 0);
					ai_ent[0].type4 = 0;
					ai_ent[1].type4 = 0;
					if (sprt_ent[0].x0 - 200 < sprt_ent[0].lmx || sprt_ent[0].lmy >= 0 && sprt_ent[0].x0 + 200 > sprt_ent[0].lmy)
						vm_data.vm_index5[41] = 1;
				}
			}
		}
	}
}

int intersect_triggers(int x, int y)
{
	CRect rmark, rcurs;
	int i;

	if (vm_data.vm_index5[41])
		return -1;
	setRect(&rcurs, x + 1, y + 1, x + 2, y + 2);
	for (i = 0; i < 30; ++i)
	{
		if ((vm_data.vm_index6[i + 10] & 0x10) == 0)
		{
			if (sub_403304(i))
			{
				rmark.Set(prog.render_rect.X0() + vm_data.vm_rects[i].X0() - prog.screen_x,
					prog.render_rect.X0() + vm_data.vm_rects[i].X1() - prog.screen_x,
					prog.render_rect.Y0() + vm_data.vm_rects[i].Y0() - prog.screen_y,
					prog.render_rect.Y0() + vm_data.vm_rects[i].Y1() - prog.screen_y);
				if (intersectRect(&rcurs, &rmark))
					return i;
			}
		}
	}
	return -1;
}

void RBtnClick(LONG x, LONG y)
{
	CRect rcur;

	if ((prog.click_bits & 1) == 0)
	{
		prog.click_bits |= 2;
		setRect(&rcur, x, y, x + 1, y + 1);
		if (prog.vm_func == 1)
		{
			if (vm_data.vm_index5[28])
			{
				if (vm_data.vm_index5[28] != 0xFFFF)
					++vm_data.vm_index5[28];
				if (vm_data.vm_index5[44])
				{
					if (vm_data.vm_index5[44] != 0xFFFF)
						++vm_data.vm_index5[44];
				}
			}
			else if (intersectRect(&rcur, &prog.render_rect) && prog.field_12E && ai_ent[0].type <= 2)
			{
				ai_ent[0].type2 = 0;
				ai_ent[0].field_1C = 0;
				if (!ai_ent[0].field_20 && !vm_data.vm_index5[40])
					ai_ent[0].field_20 = 100;
				if (ai_ent[0].type4)
				{
					ai_ent[0].type4 = 0;
					Vm_set_63();
					vm_data.vm_index5[45] = 1;
				}
			}
		}
	}
}

void LBtnClick(int is_double, LONG x, LONG y)
{
	CRect rcur;
	int item_hit; // [esp+24h] [ebp-14h]
	int trg; // [esp+2Ch] [ebp-Ch]

	prog.click_bits |= 1;
	rcur.SetXYWH(x, y, 1, 1);
	if (prog.vm_func == 1)
	{
		// check if it's inside the game area
		if (intersectRect(&rcur, &prog.render_rect))
		{
			vm_data.vm_index3[22] = prog.screen_x + x - prog.render_rect.X0() <= sprt_ent[0].x0;
			if ((prog.field_12E || prog.field_130) && ai_ent[0].type != 4 && ai_ent[0].type != 5)
			{
				ai_ent[0].field_20 = 0;
				trg = intersect_triggers(x, y);
				if (trg == -1)
				{
					// free click, move around
					if (prog.field_12E && !ai_ent[0].type4)
					{
						if (vm_data.vm_index5[40])
							is_double = 0;
						SprSetDest(0, sprt_ent[0].x0, prog.screen_x + x - prog.render_rect.X0(), is_double);
					}
				}
				else if (Vm_mark_event(trg + 10, 0))
				{
					ai_ent[0].type2 = 0;
					ai_ent[0].field_1C = 0;
					prog.field_128 = 1;
				}
			}
		}
		// check if it's inside the inventory space
		else if (vm_data.vm_index5[45] && intersectRect(&rcur, &prog.menu_rect))
		{
			item_hit = -1;
			int i;
			for (i = 0; i < 15; i++)
			{
				CRect r;
				setRect(&r, item_xy_tbl[i][0] * 2, item_xy_tbl[i][1] * 2, item_xy_tbl[i][0] * 2 + 32, item_xy_tbl[i][1] * 2 + 32);
				if (intersectRect(&rcur, &r))
				{
					item_hit = i;
					break;
				}
			}

			if (item_hit != -1 && inventory[item_hit] != 0xffff)
			{
				itemptr2_index = item_hit;
				prog.click_on_item = 1;
			}
		}
	}
}
