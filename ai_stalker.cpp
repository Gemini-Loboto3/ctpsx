#include <stdafx.h>
#include "game.h"

void MoveStalker()
{
	if (vm_data.vm_index5[25] && !vm_data.vm_index5[27])
	{
		if (sprt_ent[1].enabled)
		{
			StalkerSetDir2();
			StalkerSetDir();
			AnimateAI(1);
			AnimateStalker();
			UpdateAITriggerInteraction(1);
		}
	}
}

void AnimateStalker()
{
	WORD a2[2]; // [esp+0h] [ebp-4h] BYREF

	if (ai_ent[1].type0 != ai_ent[1].direction || ai_ent[1].type != ai_ent[1].type_next)
	{
		GetAnimData(a2, 1);
		SprAnim(1, a2[0], 0, 0);
		ai_ent[1].anim = a2[1];
		ai_ent[1].type0 = ai_ent[1].direction;
		ai_ent[1].type = ai_ent[1].type_next;
	}
}

void StalkerSetDir2()
{
	if (sprt_ent[1].lmx0 >= 0 && sprt_ent[1].x0 <= sprt_ent[1].lmx0 && ai_ent[1].type0 == 1)
		ai_ent[1].direction = 0;
	if (sprt_ent[1].lmx1 >= 0 && sprt_ent[1].x0 >= sprt_ent[1].lmx1 && !ai_ent[1].type0)
		ai_ent[1].direction = 1;
}

void StalkerSetDir()
{
	if (vm_data.vm_index5[25] == 1 && vm_data.vm_index5[39] && sprt_ent[1].is_busy && ++ai_ent[1].field_24 >= 2u)
	{
		ai_ent[1].field_24 = 0;
		ai_ent[1].type_next = 1;
		ai_ent[1].direction = sprt_ent[1].x0 - sprt_ent[0].x0 >= 0;
	}
}
