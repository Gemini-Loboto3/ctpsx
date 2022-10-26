#include <stdafx.h>
#include "game.h"

void MoveStalker()
{
	if (vm_data.vm_index5[25] && !vm_data.vm_index5[27])
	{
		if (sprt_stalker.enabled)
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

	if (ai_stalker.type0 != ai_stalker.direction || ai_stalker.type != ai_stalker.type_next)
	{
		GetAnimData(a2, 1);
		SprAnim(1, a2[0], 0, 0);
		ai_stalker.anim = a2[1];
		ai_stalker.type0 = ai_stalker.direction;
		ai_stalker.type = ai_stalker.type_next;
	}
}

void StalkerSetDir2()
{
	if (sprt_stalker.lmx0 >= 0 && sprt_stalker.x0 <= sprt_stalker.lmx0 && ai_stalker.type0 == 1)
		ai_stalker.direction = 0;
	if (sprt_stalker.lmx1 >= 0 && sprt_stalker.x0 >= sprt_stalker.lmx1 && !ai_stalker.type0)
		ai_stalker.direction = 1;
}

void StalkerSetDir()
{
	if (vm_data.vm_index5[25] == 1 && vm_data.vm_index5[39] && sprt_stalker.is_busy && ++ai_stalker.field_24 >= 2u)
	{
		ai_stalker.field_24 = 0;
		ai_stalker.type_next = 1;
		ai_stalker.direction = sprt_stalker.x0 - sprt_player.x0 >= 0;
	}
}
