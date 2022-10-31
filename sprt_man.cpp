#include <stdafx.h>
#include "game.h"

TSprSlotManager sprt_slot_manager;

TSprSlotManager::TSprSlotManager()
{
	super = nullptr;
}

int TSprSlotManager::Read(SPRT_ENT* sprt, WORD id)
{
	auto self_id = sprt->self_id;
	auto sid = self_id - 1;
	if (self_id == 0)
	{
		if (super == nullptr)
			return 0;
		auto supr = super;
		SprtEntManEntry* src;
		for (src = super; src; src = src->next)
		{
			if (src->id == id)
				break;
			supr = src;
		}
		if (src)
		{
			sprt->seq_ptr = src->seq_ptr;
		}
		else
		{
			src = supr;
			if (supr->Load(sprt, id) == 0)
				return 0;
		}
		TSprSlotManager::LinkInto(this, src);
		TSprSlotManager::LinkNormal(&super, src);
	}
	else
	{
		if (sid < 11)
			return ReadOneSlotData(&entry1[sprt->self_id + 3], sprt, id);
		if (sid - 11 >= 9)
		{
			TmcLoad(id);
			sprt->seq_ptr = tmc_alloc[SPID_GETENT(id)].tmc.GetSeq(SPID_GETFRAME(id));
		}
	}

	return 1;
}

int TSprSlotManager::ReadOneSlotData(SprtEntManEntry* ent, SPRT_ENT* spr, __int16 id)
{
	if (ent->seq_ptr == nullptr)
		return 0;

	if (ent->id == id)
		spr->seq_ptr = ent->seq_ptr;
	else if (!ent->Load(spr, id))
		return 0;

	return 1;
}

void TSprSlotManager::LinkInto(TSprSlotManager* dst, SprtEntManEntry* src)
{
	if (src->child)
		src->child->next = src->next;
	else
		dst->super = src->next;
	if (src->next)
		src->next->child = src->child;
	src->child = nullptr;
	src->next = nullptr;
}

void TSprSlotManager::LinkNormal(SprtEntManEntry **super, SprtEntManEntry * ent)
{
	if (*super)
	{
		ent->next = *super;
		(*super)->child = ent;
	}
	*super = ent;
}

///////////////////////////////////////////
SprtEntManEntry::SprtEntManEntry()
{
	field_0 = 0;
	seq_ptr = nullptr;

	next = nullptr;
	child = nullptr;
	id = -1;
}

int SprtEntManEntry::Load(SPRT_ENT* dst, WORD id)
{
	dst->field_2A = field_0;

	TmcLoad(id);
	seq_ptr = tmc_alloc[SPID_GETENT(id)].tmc.GetSeq(SPID_GETFRAME(id));
	dst->seq_ptr = seq_ptr;

	this->id = id;
	return 1;
}
