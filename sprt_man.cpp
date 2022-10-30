#include <stdafx.h>
#include "game.h"

TSprSlotManager sprt_slot_manager;

TSprSlotManager::TSprSlotManager()
{
	super = nullptr;
}

int TSprSlotManager::Read(SPRT_ENT* sprt, __int16 id)
{
	auto v3 = sprt->id2;
	auto v4 = v3 == 0;
	auto v5 = v3 - 1;
	if (v4)
	{
		if (!super)
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
			//sprt->ptr0 = src->info;
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
		v4 = v5 < 11;
		auto v6 = v5 - 11;
		if (v4)
			return ReadOneSlotData(&entry1[sprt->id2 + 3], sprt, id);
		if (v6 >= 9)
		{
			TmcLoad(id);
			//sprt->ptr0 = (BITMAPINFOHEADER*)LoadAbmFile((BYTE*)sprt->ptr0, id, sprt);
			//if (!sprt->ptr0)
			//	return 0;
		}
	}

	return 1;
}

int TSprSlotManager::ReadOneSlotData(SprtEntManEntry* ent, SPRT_ENT* spr, __int16 id)
{
	//if (!ent->info)
		//return 0;

	if (ent->id == id)
	{
		//spr->ptr0 = (BITMAPINFOHEADER*)ent->linked;
	}
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
	info = nullptr;

	next = nullptr;
	child = nullptr;
	id = -1;
}

int SprtEntManEntry::Load(SPRT_ENT* dst, WORD id)
{
	dst->field_2A = field_0;

	TmcLoad(id);

	this->id = id;
	return 1;
}
