#pragma once

struct AI_ENT
{
	DWORD type0;
	DWORD type;
	int direction;		// 0 = right, 1 = left
	DWORD type_next;
	DWORD enabled;
	WORD anim;
	WORD state;
	WORD dest_x;
	WORD type3_bk;
	DWORD timer2;			// ticks until the entity moves
	DWORD timer;
	WORD field_24;
};

extern AI_ENT ai_ent[2];

#define ai_player	ai_ent[SPID_PLAYER]
#define ai_stalker	ai_ent[SPID_STALKER]

void AnimateAI(int id);
void UpdateAITriggerInteraction(int id);
int GetAnimData(WORD* dst, int a2);
void ResetAI(int id);

int SprIsInReach();

void MoveJennifer();
int AnimateJennifer();
void AnimateJennifer2();
void JenniferDir();
void MoveJenniferSlope();

void MoveStalker();
void AnimateStalker();
void StalkerSetDir2();
void StalkerSetDir();
