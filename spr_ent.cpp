#include <stdafx.h>
#include "game.h"

SPRT_ENT sprt_ent[21];

void SPRT_ENT::Unlink()
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
		Unlink();
	sprt = prog.sprt;
	for (i = prog.sprt; i; i = i->next)
	{
		if (i->priority > priority)
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
	sprt_ent[self_id].enabled = 0;
	if (is_reference == 0 && texture)
	{
		texture->Release();
		texture = nullptr;
	}

	Unlink();
}

void SPRT_ENT::UpdateXY()
{
	if (flip)
	{
		x0 = x2;
		y0 = y2;
		x1 = x2;
		flag0 = (__int16)flag2;
		priority = (__int16)flag2;
	}
	flip = 0;
}

void SPRT_ENT::Update()
{
	updated = 1;
	UpdateXY();
}

void SPRT_ENT::SetXY(int _x, int _y, DWORD _pri, int _flip)
{
	if (_flip)
	{
		flip = 0;
		x0 = _x;
		y0 = _y;
		x1 = _x;
		flag0 = _pri;
		priority = _pri;
	}
	else
	{
		flip = 1;
		x2 = _x;
		y2 = _y;
		flag2 = (WORD)_pri;
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
	if (!is_bg_spr && is_animating == 1)
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

void SprPos(int id, int x, int y, DWORD pri)
{
	if (sprt_ent[id].enabled)
		sprt_ent[id].SetXY(x, y, pri, 0);
}

void SprAnim(int id, WORD anim, WORD anim_flg, WORD seq)
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
		else sprt_ent[id].frame_id = anim;

		sprt_ent[id].anim_flg = anim_flg;
		sprt_ent[id].seq_frame_new = seq & 0xff;
		sprt_ent[id].seq_len = seq >> 8;
		sprt_ent[id].Update();
		sprt_ent[id].UpdateXY();
		SetSprIsBg(id, 0);
		sprt_ent[id].is_animating = 0;
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
			if ((vm_data.vm_index6[i + 10] & 0x10) == 0 && TriggerIsEnabled(i))
			{
				rtrg.SetXYWH(prog.render_rect.X0() + vm_data.vm_rects[i].X0() - prog.screen_x,
					prog.render_rect.Y0() + vm_data.vm_rects[i].Y0() - prog.screen_y,
					vm_data.vm_rects[i].W(),
					vm_data.vm_rects[i].H());
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
			sprt_cursor.anim_flg = 0;
			sprt_cursor.Update();
			sprt_cursor.SetXY(prog.triggerX, prog.triggerY, 100, 1);
			break;
		case 1:	// animating
			if (sprt_cursor.is_busy)
			{
				sprt_cursor.SetXY(prog.triggerX, prog.triggerY, 100, 1);
				prog.curs_mode = 2;
			}
			break;
		case 2:	// idle
			break;
		case 3:	// are we done?
			prog.curs_mode = 1;
			sprt_cursor.anim_flg = 0;
			sprt_cursor.Update();
			sprt_cursor.SetXY(prog.triggerX, prog.triggerY, 100, 1);
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
			sprt_cursor.anim_flg = 1;
			sprt_cursor.Update();
			prog.curs_mode = 3;
			break;
		case 3:	// done, revert to winapi cursor
			if (sprt_cursor.is_busy)
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
	r->SetXYWH(s->x3, s->y3, s->width *2, s->height * 2);
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
	if (sprt->x3 < rcopy.X0())
	{
		dstx = rxy.X0();
		srcx = rcopy.X0() - sprt->x3;
	}
	else
	{
		dstx = rxy.X0() + sprt->x3 - rcopy.X0();
		srcx = 0;
	}
	if (sprt->y3 < rcopy.Y0())
	{
		dsty = rxy.Y0();
		srcy = rcopy.Y0() - sprt->y3;
	}
	else
	{
		dsty = rxy.Y0() + sprt->y3 - rcopy.Y0();
		srcy = 0;
	}

	RenderRect(sprt->texture, GETX(dstx), GETY(dsty), sprt->width, sprt->height, srcx, srcy, 0xff, 0xff, 0xff);
}

void SprEnt(int id, int x, int y, DWORD pri, __int16 anim, __int16 anim_flg, __int16 seq, DWORD a8, WORD is_abs)
{
	if (id <= SPID_CURSOR)
	{
		sprt_ent[id].is_reference = 0;
		sprt_ent[id].self_id = id;
		sprt_ent[id].enabled = 1;
		sprt_ent[id].field_32 = a8;
		sprt_ent[id].is_abs = is_abs;
		sprt_ent[id].SetXY(x, y, pri, 1);
		SprAnim(id, anim, anim_flg, seq);
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
	if (s->is_busy)
	{
		if ((s->anim_flg & 0x10) != 0)
			s->Update();
		else
		{
			int id = s->frame_id & 0x3FFF;

			switch (id)
			{
			default:
				if (id <= 0x441)
				{
					if (id != 0x441 && id - 0x10E >= 2 && id != 0x43F)
						return;
					s->Update();
				}
				break;
			case 0x441:
			case 0x43F:
			case 0x448:
			case 0x449:
			case 0xC01:
			case 0xC02:
			case 0xC03:
			case 0xC09:
			case 0xC0D:
			case 0x1001:
			case 0x1002:
			case 0x1301:
			case 0x1302:
				s->Update();
				break;
			}
		}
	}
}

void SprSetList(SPRT_ENT* s)
{
	s->SetList();
}

int SetSpriteData(SPRT_ENT* spr, unsigned int id)
{
	int xadd, yadd;

	if (!spr->updated)
	{
		if ((spr->anim_flg & 2) != 0)
			spr->is_busy = 1;
		if (spr->is_busy)
			return 1;
	}

	auto t = &tmc_alloc[SPID_GETENT(id)];

	// no sprite data, assign
	if (spr->seq_ptr == nullptr || (spr->anim_flag_ex + spr->anim_id) != id)
	{
		if (t->enabled == 0)
			TmcLoad(id);

		spr->seq_ptr = t->tmc.GetSeq(SPID_GETFRAME(id));
		spr->anim_id = SPID_GETANIM(id);
		spr->seq_frame = -1;
		spr->anim_count = 0;
		spr->anim_flag_ex = SPID_GETFLIPS(id);
		spr->is_busy = 0;
	}

	auto tt = &t->tmc;
	TMC_PYX* pyx = nullptr;

	if (spr->updated == 1)
	{
		spr->seq_frame = 0;
		if ((spr->anim_flg & 1) != 0)
			spr->seq_frame = spr->seq_ptr->cnt - 1;
		if ((spr->anim_flg & 4) != 0)
			spr->seq_frame = spr->seq_frame_new;

		pyx = tt->GetFrame(spr->seq_ptr->ptr + spr->seq_frame);

		spr->anim_count = pyx->len;
		spr->is_busy = 0;
		spr->updated = 0;
		spr->is_animating = 1;
		if (!spr->seq_frame)
			spr->seq_pos = -1;
		if (spr->seq_frame == pyx->len - 1)
			spr->seq_pos = pyx->len - 1;
		if ((spr->anim_flg & 4) != 0)
		{
			spr->seq_pos = spr->seq_frame - 1;
			while (!spr->seq_ptr[spr->seq_pos].cnt)
			{
				if (--spr->seq_pos < 0)
				{
					spr->seq_pos = -1;
					break;
				}
			}
		}
	}

	while (!spr->anim_count)
	{
		spr->is_animating = 1;
		if ((spr->anim_flg & 1) != 0)
		{
			if (--spr->seq_frame < 0)
			{
				spr->anim_count = 1;
				break;
			}

			pyx = tt->GetFrame(spr->seq_ptr->ptr + spr->seq_frame);
			spr->anim_count = pyx->len;
		}
		else
		{
			pyx = tt->GetFrame(spr->seq_ptr->ptr + spr->seq_frame);
			if (pyx->len <= ++spr->seq_frame)
			{
				spr->anim_count = 1;
				break;
			}

			pyx = tt->GetFrame(spr->seq_ptr->ptr + spr->seq_frame);
			spr->anim_count = pyx->len;
		}
	}
	if (!--spr->anim_count)
	{
		if ((spr->anim_flg & 1) == 0)
		{
			if ((spr->seq_frame + 1) < spr->seq_ptr->cnt)
			{
				if ((spr->anim_flg & 8) != 0 && spr->seq_len <= spr->seq_frame)
				{
					spr->anim_count = 1;
					spr->is_busy = 1;
				}
			}
			else
			{
				spr->anim_count = 1;
				spr->is_busy = 1;
				spr->seq_frame = spr->seq_ptr->cnt - 1;
			}
		}
		else
		{
			if (spr->seq_frame)
			{
				if ((spr->anim_flg & 8) != 0 && spr->seq_len == spr->seq_frame)
				{
					spr->anim_count = 1;
					spr->is_busy = 1;
					spr->seq_frame = spr->seq_ptr->cnt - 1;
				}
			}
			else
			{
				spr->anim_count = 1;
				spr->is_busy = 1;
				spr->seq_frame = 0;
			}
		}
	}

	pyx = tt->GetFrame(spr->seq_ptr->ptr + spr->seq_frame);

	spr->priority = pyx->e & 0x1F;
	spr->priority += spr->flag0;
	spr->sub_frame = pyx->f;
	auto e = tt->GetEntry(pyx->frame);
	spr->width = e->w;
	spr->height = e->h;

	if (spr->texture)
	{
		spr->texture->Release();
		spr->texture = nullptr;
	}

	BYTE *buf = new BYTE[e->w * e->h];
	tt->dec(&tt->pix_data[e->pos], buf, e->size);
	spr->texture = MakeTexture();
	spr->texture->Create(tt->clut, buf, 8, e->w, e->h);

	// movement
	if (spr->sub_frame == 0xFFFF)
	{
		spr->x3 = 768;
		spr->y3 = 768;
	}
	else
	{
		if (spr->seq_pos == -1)
		{
			xadd = pyx->e * 2;
			yadd = pyx->d * 2;
			spr->seq_pos = 0;
		}
		else if (spr->seq_frame == spr->seq_pos)
		{
			yadd = 0;
			xadd = 0;
		}
		else
		{
			xadd = pyx->e * 2;
			yadd = pyx->d * 2;
			spr->seq_pos = spr->seq_frame;
		}

		// horizontal
		if ((spr->anim_flag_ex & 0x8000) == 0)
		{
			spr->x0 += xadd;
			spr->x3 = spr->x0 - e->x * 2;
		}
		// mirrored
		else
		{
			spr->x0 -= xadd;
			spr->x3 = spr->x0 + e->x * 2 - spr->width * 2;
		}

		// vertical
		if ((spr->anim_flag_ex & 0x4000) == 0)
		{
			spr->y0 += yadd;
			spr->y3 = spr->y0 + e->y * 2 - spr->height * 2;
		}
		// flipped
		else
		{
			spr->y0 -= yadd;
			spr->y3 = spr->y0 - e->y * 2;
		}
	}

	return 1;
}

void SprUpdater()
{
	for (SPRT_ENT *s = prog.sprt; s; s = s->next)
	{
		s->is_animating = 0;
		s->x1 = s->x0;
		int priority = s->priority;
		SprUpdate(s);
		if (s->frame_id != 0xFFFF)
		{
			if ((s->frame_id & 0x3FFF) >= 0x3800);
				//sub_4013B0(s);
			else
				SetSpriteData(s, s->frame_id);
		}
		if (priority != s->priority)
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
		r0.Set(sprt_player.x0, sprt_player.x1, sprt_player.y0, sprt_player.y0 + 1);
		rectSwapX(&r0);
		v2 = 0;
		while ((vm_data.vm_index6[v2 + 10] & 0x10) == 0 || !TriggerIsEnabled(v2) || !intersectRect(&r0, &vm_data.vm_rects[v2]))
		{
			if (++v2 >= 30)
				return;
		}
		Vm_mark_event(v2 + 10, 1u);
	}
}

void CkAIAttack()
{
	int ret;

	if (vm_data.vm_index5[25] && vm_data.vm_index5[26])
	{
		if (vm_data.vm_index5[27] == 0 && vm_data.vm_index5[4] == vm_data.vm_index5[6] && vm_data.vm_index5[3] == vm_data.vm_index5[5])
		{
			ret = SprIsInReach();
			if (ret)
			{
				if ((vm_data.vm_index3[14] & 1) != 0)
				{
					if (!ai_player.state && !prog.no_exec)
						return;
					ai_player.state = 0;
					prog.no_exec = 0;
					Vm_set_63();
				}
				vm_data.vm_index5[27] = ret;
				ai_player.timer = 0;
				Vm_mark_event(0x191, 0);
				ai_player.state = 0;
				ai_stalker.state = 0;
				// check jennifer's limits
				if (sprt_player.x0 - 200 < sprt_player.lmx0 ||
					sprt_player.lmx1 >= 0 && sprt_player.x0 + 200 > sprt_player.lmx1)
					vm_data.vm_index5[41] = 1;
			}
		}
	}
}

int TriggerIntersectCk(int x, int y)
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
			if (TriggerIsEnabled(i))
			{
				rmark.SetXYWH(prog.render_rect.X0() + vm_data.vm_rects[i].X0() - prog.screen_x,
					prog.render_rect.Y0() + vm_data.vm_rects[i].Y0() - prog.screen_y,
					vm_data.vm_rects[i].W(),
					vm_data.vm_rects[i].H());
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
			else if (intersectRect(&rcur, &prog.render_rect) && prog.can_lclick && ai_player.type <= 2)
			{
				ai_player.type_next = 0;
				ai_player.timer2 = 0;
				if (!ai_player.timer && !vm_data.vm_index5[40])
					ai_player.timer = 100;
				if (ai_player.state)
				{
					ai_player.state = 0;
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
		vm_data.vm_index3[22] = (prog.screen_x + cursor_x - prog.render_rect.X0()) <= sprt_player.x0;
		if ((prog.can_lclick || prog.can_rclick) && ai_player.type != 4 && ai_player.type != 5)
		{
			ai_player.timer = 0;
			int trg = TriggerIntersectCk(cursor_x, cursor_y);
			// no trigger found
			if (trg == -1)
			{
				// free click, move around
				if (prog.can_lclick && !ai_player.state)
				{
					if (vm_data.vm_index5[40])
						is_double = 0;
					// set jennifer's destination
					SprSetDest(SPID_PLAYER, sprt_player.x0, prog.screen_x + cursor_x - prog.render_rect.X0(), is_double);
				}
			}
			// trigger found, check if it's active
			else if (Vm_mark_event(trg + 10, 0))
			{
				// proceed to the trigger
				ai_player.type_next = 0;
				ai_player.timer2 = 0;
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
