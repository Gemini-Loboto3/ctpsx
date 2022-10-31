#pragma once

struct SprtEntManEntry
{
	SprtEntManEntry();

	int Load(SPRT_ENT* dst, WORD id);

	int field_0;
	TMC_PTN* seq_ptr;
	SprtEntManEntry* child;
	SprtEntManEntry* next;
	WORD id;
};

struct TSprSlotManager
{
	TSprSlotManager();

	int  Read(SPRT_ENT* sprt, WORD id);
	int  ReadOneSlotData(SprtEntManEntry* ent, SPRT_ENT* spr, __int16 id);
	void LinkInto(TSprSlotManager* dst, SprtEntManEntry* src);
	void LinkNormal(SprtEntManEntry** super, SprtEntManEntry* ent);

	SprtEntManEntry* super;
	SprtEntManEntry entry1[4],
		entry2[11];
};

extern TSprSlotManager sprt_slot_manager;
