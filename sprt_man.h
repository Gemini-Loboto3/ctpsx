#pragma once

struct SprtEntManEntry
{
	int Load(SPRT_ENT* dst, WORD id);

	int field_0;
	BITMAPINFOHEADER* field_4;
	SprtEntManEntry* child;
	SprtEntManEntry* next;
	__int16 id;
	__int16 field_12;
};

struct TSprSlotManager
{
	TSprSlotManager();

	int  Read(SPRT_ENT* sprt, __int16 id);
	int  ReadOneSlotData(SprtEntManEntry* ent, SPRT_ENT* spr, __int16 id);
	void LinkInto(TSprSlotManager* dst, SprtEntManEntry* src);
	void LinkNormal(SprtEntManEntry** super, SprtEntManEntry* ent);

	SprtEntManEntry* super;
	SprtEntManEntry entry1[4],
		entry2[11];
};

extern TSprSlotManager sprt_slot_manager;
