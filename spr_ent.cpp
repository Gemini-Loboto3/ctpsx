#include <stdafx.h>
#include "game.h"

SPRT_ENT sprt_ent[21];

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

void SPRT_ENT::SetXY(int _x, int _y, DWORD _flags, int _flip)
{
	if (_flip)
	{
		flip = 0;
		x0 = _x;
		y0 = _y;
		x1 = _x;
		flag0 = _flags;
		flag1 = _flags;
	}
	else
	{
		flip = 1;
		x2 = _x;
		y2 = _y;
		flag2 = (WORD)_flags;
	}
}

void SPRT_ENT::SetX0()
{
	if (lmx0 >= 0 && x0 < lmx0)
		x0 = lmx0;
	if (lmx1 > 0 && x0 > lmx1)
		x0 = lmx1;
}

extern signed char* off_41F094[];
BYTE vol_tbl[127] =
{
	127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,
	127,127,110,127,127,127,127,127,127,127,127,127,127,127,127,127,
	127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,
	127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,
	127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,
	127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,
	127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,
	127,127,127,127,127,127,127,127,127,127,127,127,127,127,127
};

extern const char* soundname_tbl[];

void SPRT_ENT::DoSound()
{
	if (!is_bg_spr && field_41 == 1)
	{
		int frame = (frame_id & 0x3FFF) >> 8;
		if (frame < 19)
		{
			int sample = off_41F094[frame][sub_frame];
			if (sample != -1)
			{
				if (vm_data.vm_index5[47])
				{
					switch (sample)
					{
					case 0: sample = 2; break;
					case 1: sample = 3; break;
					}
				}
				int pan = Sound_get_pan(x0 - prog.screen_x);
				int vol = vol_tbl[sample];
				if (pan == 16 || pan == -16)
					vol -= 24;
				if (vol < 0)
					vol = 0;
				Sound_play(soundname_tbl[sample], 0, pan, vol);
			}
		}
	}
}

void Vm_spr_lmt(int id, int lmx0, int lmx1)
{
	sprt_ent[id].lmx0 = lmx0;
	sprt_ent[id].lmx1 = lmx1;
}

void SetSprIsBg(int id, int is_bg)
{
	sprt_ent[id].is_bg_spr = is_bg;
}

void SprtTblDeinit()
{
	if (prog.sprt)
	{
		SPRT_ENT *sprt = prog.sprt, *next;
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

void SprAnim(int id, WORD anim, WORD a3, WORD a4)
{
	if (sprt_ent[id].enabled)
	{
		if (anim == 0xFFFF)
		{
			WORD dat[2];
			if (GetAnimData(dat, id))
			{
				sprt_ent[id].frame_id = dat[0];
				ai_ent[id].anim = dat[1];
				ai_ent[id].enabled = 0;
				ai_ent[id].type0 = ai_ent[id].direction;
				ai_ent[id].type = ai_ent[id].type_next;
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
		//sprt_ent[id].UpdateXY();	// useless, Update() does this step already
		SetSprIsBg(id, 0);
		sprt_ent[id].field_41 = 0;
		sprt_ent[id].is_busy = 0;
		if (id <= 1)
			ai_ent[id].anim = 0;
	}
}

void SprCursorAnimate()
{
	CRect rtrg, rcur;
	int i = 0;

	rcur.SetXYWH(prog.mousePT.x + 1, prog.mousePT.y + 1, 1, 1);
	if (prog.cur_enabled)
	{
		// find any trigger intersecting with the cursor
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
				goto not_found;
		}

		prog.triggerX = vm_data.vm_rects[i].X0() - (32 - vm_data.vm_rects[i].W()) / 2;
		prog.triggerY = vm_data.vm_rects[i].Y0() - (32 - vm_data.vm_rects[i].H()) / 2;
		switch (prog.curs_mode)
		{
		case 0:	// hide winapi cursor and play the animation
			prog.cur_type1 = showCursor(0);
			prog.curs_mode = 1;
			sprt_ent[11].field_99 = 0;
			sprt_ent[11].Update();
			sprt_ent[11].SetXY(prog.triggerX, prog.triggerY, 0x64u, 1);
			break;
		case 1:	// animating
			if (sprt_ent[11].is_busy)
			{
				sprt_ent[11].SetXY(prog.triggerX, prog.triggerY, 0x64u, 1);
				prog.curs_mode = 2;
			}
			break;
		case 2:	// idle
			break;
		case 3:	// are we done?
			prog.curs_mode = 1;
			sprt_ent[11].field_99 = 0;
			sprt_ent[11].Update();
			sprt_ent[11].SetXY(prog.triggerX, prog.triggerY, 0x64u, 1);
			break;
		}
	}
	else
	{
not_found:
		switch (prog.curs_mode)
		{
		case 0:	// play selection animation backwards
		case 1:
			sprt_ent[11].field_99 = 1;
			sprt_ent[11].Update();
			prog.curs_mode = 3;
			break;
		case 3:	// done, revert to winapi cursor
			if (sprt_ent[11].is_busy)
			{
				prog.curs_mode = 0;
				if (prog.cur_type1 < 0)
					prog.cur_type1 = showCursor(1);
			}
			break;
		}
	}
}

void SpriteGetRect(SPRT_ENT* s, CRect* r)
{
	r->SetXYWH(s->x3, s->y3, s->width, s->height);
}

void SprDraw(SPRT_ENT* sprt, CRect* lprcSrc)
{
	CRect rxy, rspr, rcopy;
	int srcx, srcy, dstx, dsty;

	SpriteGetRect(sprt, &rspr);
	if (lprcSrc)
	{
		copyRect(&rcopy, lprcSrc);
		//if (!intersectRect(&rspr, &rcopy))
		//	return;
	}
	else
	{
		TMapGetDstRect(&tmap, &rcopy);
		//if (!intersectRect(&rspr, &rcopy))
		//	return;
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

void SprEnt(int id, int x, int y, DWORD a4, __int16 a5, __int16 a6, __int16 a7, DWORD a8, WORD is_abs)
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
	CRect rcur;

	rcur.SetXYWH(prog.mousePT.x + 1, 1, prog.mousePT.y + 1, 1);
	if (prog.vm_func != 1 || !intersectRect(&rcur, &prog.render_rect) || prog.cur_enabled && prog.curs_mode == 0)
	{
		if (prog.cur_type1 < 0)
			prog.cur_type1 = showCursor(1);
	}
	else if (prog.cur_type1 >= 0)
	{
		prog.cur_type1 = showCursor(0);
	}
}

int sub_403619()
{
	if (vm_data.vm_index5[2] <= 200)
		return 0;
	if (vm_data.vm_index5[2] <= 400)
		return 1;
	if (vm_data.vm_index5[2] > 600)
		return 3;
	return 2;
}

int sub_4035DC()
{
	WORD v3; // al

	if (!vm_data.vm_index5[28])
		return 3 - sub_403619();
	v3 = prog.fade_timer;
	--prog.fade_timer;
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
	if (spr->sub_frame == 0xFFFF)
	{
		spr->x3 = 768;
		spr->y3 = 768;
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
			spr->x0 += 8;
			spr->x3 = spr->x0 - 4;
			//spr->x0 += v6;
			//spr->x3 = (__int16)(LOWORD(spr->x0) - v3[6 * (__int16)v13 + 9]);
		}
		else
		{
			spr->x0 -= 8;
			spr->x3 = spr->x0 + 4;
			//spr->x0 -= v6;
			//spr->x3 = (__int16)(LOWORD(spr->x0) + v3[6 * (__int16)v13 + 9] - v3[6 * (__int16)v13 + 7]);
		}

		// vertical
		if ((spr->field_89 & 0x4000) != 0)
		{
			spr->y0 -= 8;
			spr->y3 = spr->y0 - 4;
			//spr->y0 -= t3;
			//spr->y3 = (__int16)(LOWORD(spr->y0) - v3[6 * (__int16)v13 + 10]);
		}
		else
		{
			spr->y0 += 8;
			spr->y3 = spr->y0 - spr->height + 4;
			//spr->y0 += t3;
			//spr->y3 = (__int16)(v3[6 * (__int16)v13 + 10] + LOWORD(spr->y0) - LOWORD(spr->height));
		}
	}

	return 1;
}

void SprUpdater()
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
		s->DoSound();
	}
	MoveJenniferSlope();
}

void EventWait()
{
	if (prog.no_exec)
	{
		if (Vm_ent_wait(0))
			prog.no_exec = 0;
	}
}

int EntGetPan(int a1)
{
	return Sound_get_pan(sprt_ent[a1].x0 - prog.screen_x);
}

void Vm_spr_dir(int id, int a2, int a3, int a4, int a5)
{
	int dir;

	if (a2 != -1)
		ai_ent[id].type0 = a2 >> 3;
	if (a4 == -1)
		dir = ai_ent[id].type0;
	else
		dir = a4 >> 3;
	ai_ent[id].direction = dir;
	if (a3 != -1)
		ai_ent[id].type = a3;
	if (a5 == -1)
		ai_ent[id].type_next = ai_ent[id].type;
	else
		ai_ent[id].type_next = a5;
	ResetAI(id);
}

void SprSetDest(int id, int cur_x, int dst_x, int running)
{
	if (id == 0 && vm_data.vm_index5[25] == 1)
		running = 1;
	// moving to the right
	if (dst_x >= cur_x || sprt_ent[id].lmx0 >= cur_x)
	{
		if (dst_x >= cur_x && (sprt_ent[id].lmx1 < 0 || sprt_ent[id].lmx1 > cur_x))
		{
			ai_ent[id].direction = 0;
			if (running)
			{
				ai_ent[id].type_next = 2;
				ai_ent[id].timer2 = 0;
			}
			else
			{
				ai_ent[id].type_next = 1;
				ai_ent[id].timer2 = 8;
			}
		}
	}
	// moving to the left
	else
	{
		ai_ent[id].direction = 1;
		if (running)
		{
			ai_ent[id].type_next = 2;
			ai_ent[id].timer2 = 0;
		}
		else
		{
			ai_ent[id].type_next = 1;
			ai_ent[id].timer2 = 8;
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
		ai_ent[id].type3_bk = 1;
		vm_data.vm_index5[29] = 1;
	}
	else
	{
		ai_ent[id].type3_bk = 0;
		vm_data.vm_index5[29] = 0;
	}

	if (a4 != -1)
		ai_ent[id].type3_bk = a4 >> 3;

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

	ai_ent[id].dest_x = dst_x;

	switch (mmode)
	{
	case 0:
		SprSetDest(id, cur_x, dst_x, running);
		ai_ent[id].state = 1;
		ai_ent[id].timer2 = 0;
		break;
	case 1:
	case 2:
		ai_ent[id].state = 2;
		break;
	}
}

void TriggerUpdate()
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

void CkAIAttack()
{
	int reach;

	if (vm_data.vm_index5[25] && vm_data.vm_index5[26])
	{
		if (vm_data.vm_index5[27] == 0 && vm_data.vm_index5[4] == vm_data.vm_index5[6] && vm_data.vm_index5[3] == vm_data.vm_index5[5])
		{
			reach = SprIsInReach();
			if (reach)
			{
				if ((vm_data.vm_index3[14] & 1) != 0)
				{
					if (!ai_ent[0].state && !prog.no_exec)
						return;
					ai_ent[0].state = 0;
					prog.no_exec = 0;
					Vm_set_63();
				}
				vm_data.vm_index5[27] = reach;
				ai_ent[0].timer = 0;
				Vm_mark_event(0x191, 0);
				ai_ent[0].state = 0;
				ai_ent[1].state = 0;
				// check jennifer's limits
				if (sprt_ent[0].x0 - 200 < sprt_ent[0].lmx0 || sprt_ent[0].lmx1 >= 0 && sprt_ent[0].x0 + 200 > sprt_ent[0].lmx1)
					vm_data.vm_index5[41] = 1;
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

void RBtnClick(int x, int y)
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
			else if (intersectRect(&rcur, &prog.render_rect) && prog.can_lclick && ai_ent[0].type <= 2)
			{
				ai_ent[0].type_next = 0;
				ai_ent[0].timer2 = 0;
				if (!ai_ent[0].timer && !vm_data.vm_index5[40])
					ai_ent[0].timer = 100;
				if (ai_ent[0].state)
				{
					ai_ent[0].state = 0;
					Vm_set_63();
					vm_data.vm_index5[45] = 1;
				}
			}
		}
	}
}

void LBtnClick(int is_double, int cursor_x, int cursor_y)
{
	prog.click_bits |= 1;

	if (prog.vm_func != 1)	// ensure it's game mode
		return;

	CRect rcur;
	rcur.SetXYWH(cursor_x, cursor_y, 1, 1);

	// check if click is inside the map window
	if (intersectRect(&rcur, &prog.render_rect))
	{
		vm_data.vm_index3[22] = (prog.screen_x + cursor_x - prog.render_rect.X0()) <= sprt_ent[0].x0;
		if ((prog.can_lclick || prog.can_rclick) && ai_ent[0].type != 4 && ai_ent[0].type != 5)
		{
			ai_ent[0].timer = 0;
			int trg = intersect_triggers(cursor_x, cursor_y);
			// no trigger found
			if (trg == -1)
			{
				// free click, move around
				if (prog.can_lclick && !ai_ent[0].state)
				{
					if (vm_data.vm_index5[40])
						is_double = 0;
					// set jennifer's destination
					SprSetDest(0, sprt_ent[0].x0, prog.screen_x + cursor_x - prog.render_rect.X0(), is_double);
				}
			}
			// trigger found, check if it's active
			else if (Vm_mark_event(trg + 10, 0))
			{
				// proceed to the trigger
				ai_ent[0].type_next = 0;
				ai_ent[0].timer2 = 0;
				prog.no_exec = 1;
			}
		}
	}
	// check if it's inside the inventory space
	else if (vm_data.vm_index5[45] && intersectRect(&rcur, &prog.menu_rect))
	{
		int item_hit = -1;

		// search for an intersecting item in the menu
		for (int i = 0; i < 15; i++)
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
