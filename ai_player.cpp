#include <stdafx.h>
#include "game.h"

void MoveJennifer()
{
	JenniferDir();
	AnimateAI(SPID_PLAYER);
	if (!AnimateJennifer())
	{
		if (ai_player.type == 2)
		{
			if (vm_data.vm_index5[2])
				--vm_data.vm_index5[2];
		}
		AnimateJennifer2();
		UpdateAITriggerInteraction(SPID_PLAYER);
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

	switch (ai_player.type)
	{
	case 3:
		vm_data.vm_index5[2] += 2;
		if (vm_data.vm_index5[2] > 800)
			vm_data.vm_index5[2] = 800;
		if (ai_player.type_next != 3)
		{
			ai_player.type = 5;
			SprAnim(SPID_PLAYER, word_41FF54[vm_data.vm_index5[4]][ai_player.type0][1], 0, 0);
		}
		return 1;
	case 4:
		if (sprt_player.is_busy)
		{
			ai_player.type = 3;
			ai_player.type_next = 3;
		}
		return 1;
	case 5:
		if (sprt_player.is_busy)
		{
			ai_player.type = 0;
			return 0;
		}
		return 1;
	default:
		// no timer, do nothing
		if (!ai_player.timer)
			return 0;
		// timer is active, check limits
		if (--ai_player.timer || sprt_player.x0 - 80 < sprt_player.lmx0 ||
			sprt_player.lmx1 >= 0 && sprt_player.x0 + 80 > sprt_player.lmx1)
			return 0;
		else
		{
			ai_player.type = 4;
			ai_player.type_next = 4;
			SprAnim(SPID_PLAYER, word_41FF54[vm_data.vm_index5[4]][ai_player.type0][0], 0, 0);
			ai_player.anim = 0;
			return 1;
		}
	}
}

void AnimateJennifer2()
{
	// wait until the initial animation timer runs out
	if (ai_player.timer2)
		--ai_player.timer2;
	// check if the same animation is already in place
	else if (ai_player.type0 != ai_player.direction ||
		ai_player.type != ai_player.type_next)
	{
		WORD dat[2];
		GetAnimData(dat, 0);
		if (prog.no_exec && ai_player.type == 2)
			ai_player.enabled = 1;
		if (ai_player.enabled)
		{
			if (dat[1])
			{
				dat[0] = dat[1];
				dat[1] = 0;
			}
		}
		SprAnim(SPID_PLAYER, dat[0], 0, 0);
		ai_player.anim = dat[1];
		ai_player.enabled = 0;
		ai_player.type0 = ai_player.direction;
		ai_player.type = ai_player.type_next;
	}
}

void JenniferDir()
{
	if (sprt_player.lmx0 >= 0
		&& (ai_player.type == 1 || ai_player.type == 2)
		&& sprt_player.x0 <= sprt_player.lmx0
		&& ai_player.type0 == 1)
	{
		ai_player.enabled = 1;
		ai_player.type_next = 0;
		ai_player.timer2 = 0;
		ai_player.direction = 1;
	}
	if (sprt_player.lmx1 >= 0
		&& (ai_player.type == 1 || ai_player.type == 2)
		&& sprt_player.x0 >= sprt_player.lmx1
		&& !ai_player.type0)
	{
		ai_player.enabled = 1;
		ai_player.type_next = 0;
		ai_player.timer2 = 0;
		ai_player.direction = 0;
	}
}

void MoveJenniferSlope()
{
	if (prog.slant_on)
	{
		if (sprt_player.is_animating && prog.slant_x <= sprt_player.x0
			&& prog.slant_w + prog.slant_x >= sprt_player.x0)
		{
			int y = prog.slant_y + prog.slant_h * (sprt_player.x0 - prog.slant_x) / prog.slant_w - sprt_player.y0;
			if (y)
			{
				sprt_player.y0 += y;
				sprt_player.y3 += y;
			}
		}
	}
}
