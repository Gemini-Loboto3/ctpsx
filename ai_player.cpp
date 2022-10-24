#include <stdafx.h>
#include "game.h"

void MoveJennifer()
{
	JenniferDir();
	AnimateAI(0);
	if (!AnimateJennifer())
	{
		if (ai_ent[0].type == 2)
		{
			if (vm_data.vm_index5[2])
				--vm_data.vm_index5[2];
		}
		AnimateJennifer2();
		UpdateAITriggerInteraction(0);
	}
}

int AnimateJennifer()
{
	static WORD word_41FF54[4][2][2] =
	{
		{ { 349u, 350u }, { 33117u, 33118u } },
		{ { 349u, 350u }, { 33117u, 33118u } },
		{ { 73u, 74u },   { 32841u, 32842u } },
		{ { 36864u, 0u }, { 36865u, 0u } }
	};

	switch (ai_ent[0].type)
	{
	case 3:
		vm_data.vm_index5[2] += 2;
		if (vm_data.vm_index5[2] > 800)
			vm_data.vm_index5[2] = 800;
		if (ai_ent[0].type_next != 3)
		{
			ai_ent[0].type = 5;
			SprAnim(0, word_41FF54[vm_data.vm_index5[4]][ai_ent[0].type0][1], 0, 0);
		}
		return 1;
	case 4:
		if (sprt_ent[0].is_busy)
		{
			ai_ent[0].type = 3;
			ai_ent[0].type_next = 3;
		}
		return 1;
	case 5:
		if (sprt_ent[0].is_busy)
		{
			ai_ent[0].type = 0;
			return 0;
		}
		return 1;
	default:
		// no timer, do nothing
		if (!ai_ent[0].timer)
			return 0;
		// timer is active, check limits
		if (--ai_ent[0].timer || sprt_ent[0].x0 - 80 < sprt_ent[0].lmx0 || sprt_ent[0].lmx1 >= 0 && sprt_ent[0].x0 + 80 > sprt_ent[0].lmx1)
			return 0;
		else
		{
			ai_ent[0].type = 4;
			ai_ent[0].type_next = 4;
			SprAnim(0, word_41FF54[vm_data.vm_index5[4]][ai_ent[0].type0][0], 0, 0);
			ai_ent[0].anim = 0;
			return 1;
		}
	}
}

void AnimateJennifer2()
{
	// wait until the initial animation timer runs out
	if (ai_ent[0].timer2)
		--ai_ent[0].timer2;
	// check if the same animation is already in place
	else if (ai_ent[0].type0 != ai_ent[0].direction || ai_ent[0].type != ai_ent[0].type_next)
	{
		WORD dat[2];
		GetAnimData(dat, 0);
		if (prog.no_exec && ai_ent[0].type == 2)
			ai_ent[0].enabled = 1;
		if (ai_ent[0].enabled)
		{
			if (dat[1])
			{
				dat[0] = dat[1];
				dat[1] = 0;
			}
		}
		SprAnim(0, dat[0], 0, 0);
		ai_ent[0].anim = dat[1];
		ai_ent[0].enabled = 0;
		ai_ent[0].type0 = ai_ent[0].direction;
		ai_ent[0].type = ai_ent[0].type_next;
	}
}

void JenniferDir()
{
	if (sprt_ent[0].lmx0 >= 0
		&& (ai_ent[0].type == 1 || ai_ent[0].type == 2)
		&& sprt_ent[0].x0 <= sprt_ent[0].lmx0
		&& ai_ent[0].type0 == 1)
	{
		ai_ent[0].enabled = 1;
		ai_ent[0].type_next = 0;
		ai_ent[0].timer2 = 0;
		ai_ent[0].direction = 1;
	}
	if (sprt_ent[0].lmx1 >= 0
		&& (ai_ent[0].type == 1 || ai_ent[0].type == 2)
		&& sprt_ent[0].x0 >= sprt_ent[0].lmx1
		&& !ai_ent[0].type0)
	{
		ai_ent[0].enabled = 1;
		ai_ent[0].type_next = 0;
		ai_ent[0].timer2 = 0;
		ai_ent[0].direction = 0;
	}
}

void MoveJenniferY()
{
	if (prog.field_198 && sprt_ent[0].field_41 && prog.field_19C <= sprt_ent[0].x0 && prog.field_1A0 + prog.field_19C >= sprt_ent[0].x0)
	{
		int y = prog.field_19E + prog.field_1A2 * (sprt_ent[0].x0 - prog.field_19C) / prog.field_1A0 - sprt_ent[0].y0;
		if (y)
		{
			sprt_ent[0].y0 += y;
			sprt_ent[0].y3 += y;
		}
	}
}
