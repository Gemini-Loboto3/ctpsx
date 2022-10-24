#include <stdafx.h>
#include "game.h"

AI_ENT ai_ent[2];

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

void UpdateAITriggerInteraction(int id)
{
	CRect pt;

	switch (ai_ent[id].state)
	{
	case 1:
		pt.Set(sprt_ent[id].x0, sprt_ent[id].x1, 0, 0);
		rectSwapX(&pt);
		if (ai_ent[id].dest_x >= pt.X0() && ai_ent[id].dest_x <= pt.X1())
			ai_ent[id].state = 2;
		break;
	case 2:
		sprt_ent[id].SetXY(ai_ent[id].dest_x, sprt_ent[id].y0, sprt_ent[id].flag0, 0);
		if (ai_ent[id].type == 1 || ai_ent[id].type == 2)
		{
			ai_ent[id].enabled = 1;
			ai_ent[id].type_next = 0;
		}
		ai_ent[id].state = 3;
		break;
	case 3:
		if (ai_ent[id].type3_bk == (WORD)ai_ent[id].type0)
			ai_ent[id].state = 0;
		else
		{
			ai_ent[id].direction = ai_ent[id].type3_bk;
			ai_ent[id].state = 4;
		}
		break;
	case 4:
		if (sprt_ent[id].is_busy)
			ai_ent[id].state = 0;
		break;
	}
}

int GetAnimData(WORD* dst, int a2)
{
	DWORD v3;

	if (a2)
	{
		if (a2 != 1)
			return 0;
		if (vm_data.vm_index5[25])
		{
			if (vm_data.vm_index5[25] == 1)
			{
				v3 = ai_ent[1].type_next + 6 * ai_ent[1].type + 18 * ai_ent[1].direction;
				if (ai_ent[1].type0 != ai_ent[1].direction)
					v3 += 3;
				dst[0] = word_41FF6C[vm_data.vm_index5[6]][v3][0];
				dst[1] = word_41FF6C[vm_data.vm_index5[6]][v3][1];
			}
			else if (vm_data.vm_index5[25] == 2)
			{
				if ((int)ai_ent[1].direction > 1)
					ai_ent[1].direction = 1;
				dst[0] = word_42011C[vm_data.vm_index5[43]][ai_ent[1].direction][ai_ent[1].type_next];
				dst[1] = 0;
			}
		}
	}
	else
	{
		v3 = ai_ent[0].type_next + 6 * ai_ent[0].type + 18 * ai_ent[0].direction;
		if (ai_ent[0].type0 != ai_ent[0].direction)
			v3 += 3;
		dst[0] = word_41FDA4[vm_data.vm_index5[4]][v3][0];
		dst[1] = word_41FDA4[vm_data.vm_index5[4]][v3][1];
	}
	return 1;
}

void AnimateAI(int id)
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

void ResetAI(int id)
{
	ai_ent[id].anim = 0;
	ai_ent[id].state = 0;
	ai_ent[id].enabled = 0;
	ai_ent[id].timer2 = 0;
	ai_ent[id].timer = 0;
	ai_ent[id].field_24 = 0;
}

int SprIsInReach()
{
	static WORD dist_tbl[][2] =
	{
		{ 100, 80 },
		{ 60, 50 },
		{ 50, 40 },
		{ 90, 80 }
	};

	int xdist, xdiff, dir;

	xdiff = sprt_ent[1].x0 - sprt_ent[0].x0;
	xdist = abs(xdiff);

	switch (vm_data.vm_index5[25])
	{
	case 1:
		if (xdiff <= 0)
		{
			if (ai_ent[1].type0 == 0 && dist_tbl[vm_data.vm_index5[4]][0] >= xdist ||
				ai_ent[1].type0 == 1 && dist_tbl[vm_data.vm_index5[4]][1] >= xdist)
				return 2;
		}
		else if (ai_ent[1].type0 == 1 && dist_tbl[vm_data.vm_index5[4]][0] >= xdist ||
			ai_ent[1].type0 == 0 && dist_tbl[vm_data.vm_index5[4]][1] >= xdist)
			return 1;
		break;
	case 2:
		dir = 0;
		if (sprt_ent[0].x0 - 200 < sprt_ent[0].lmx0 || sprt_ent[0].lmx1 >= 0 && sprt_ent[0].x0 + 200 > sprt_ent[0].lmx1)
			dir = 1;
		if (dist_tbl[3][dir] >= xdist)
		{
			if (xdiff <= 0)
				return 2;
			else
				return 1;
		}
		break;
	}

	return 0;
}