#pragma once

typedef struct tagTmcEntry
{
	WORD w,
		h;
	short x, y;
	DWORD pos,
		size;
} TMC_ENTRY;

typedef struct tagTmcHeader
{
	WORD magic;			// 'TM'
	WORD entry_cnt;		// number of TMC_ENTRY
	DWORD file_size,	// total file size
		pos_pix,		// absolute position to graphics
		pix_size,		// size in bytes
		pos_coor,		// absolute pointer to coordinate data?
		coor_size,		// size in bytes
		unkptr_18,
		unksize_1C;		// size in bytes
} TMC_HEADER;

typedef struct tagTmcAlloc
{
	WORD enabled;
	short x, y, w, h;
} TMC_ALLOC;

extern TMC_ALLOC tmc_alloc[64];

class Tmc
{
public:
	std::vector<TMC_ENTRY> entries;
	WORD clut[256], count = 0;
	std::vector<BYTE> pix,
		coor,
		unk;

	int open(const char* filename);
	int dec(BYTE* src, BYTE* dst, int cmp_size);

	void test(const char* filename);
};

void LoadTMC(int id);