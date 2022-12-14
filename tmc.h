#pragma once
#include <algorithm>

typedef struct tagTmcEntry
{
	WORD w,
		h;
	short x, y;
	DWORD pos,
		size;
} TMC_ENTRY;

typedef struct tagTmcPtn
{
	short cnt,			// frame count for animation
		ptr;			// frame pointer index
} TMC_PTN;

typedef struct tagTmcPYX
{
	short frame, len, c,
		d,
		e,
		f;				// always -1
} TMC_PYX;

typedef struct tagTmcHeader
{
	WORD magic;			// 'TM'
	WORD entry_cnt;		// number of TMC_ENTRY
	DWORD file_size,	// total file size
		pix_pos,		// absolute position to graphics
		pix_size,		// size in bytes
		ptn_pos,		// absolute pointer to coordinate data?
		ptn_size,		// size in bytes
		pyx_pos,
		pyx_size;		// size in bytes
} TMC_HEADER;

class Tmc
{
public:
	std::vector<TMC_ENTRY> entry;
	WORD clut[256];
	std::vector<BYTE> pix_data;
	std::vector<TMC_PTN> ptn_data;
	std::vector<TMC_PYX> pyx_data;

	TMC_PTN* GetSeq(int seq_pos)
	{
		seq_pos = std::clamp<int>(seq_pos, 0, ptn_data.size() - 1);
		return &ptn_data[seq_pos];
	}
	TMC_PYX* GetFrame(int frame_no)
	{
		frame_no = std::clamp<int>(frame_no, 0, pyx_data.size() - 1);
		return &pyx_data[frame_no];
	}
	TMC_ENTRY* GetEntry(int entry_no)
	{
		entry_no = std::clamp<int>(entry_no, 0, entry.size() - 1);
		return &entry[entry_no];
	}

	int open(const char* filename);
	int dec(BYTE* src, BYTE* dst, int cmp_size);

	void test(const char* filename);
};

typedef struct tagTmcAlloc
{
	WORD enabled;
	short x, y, w, h;
	Tmc tmc;
} TMC_ALLOC;

extern TMC_ALLOC tmc_alloc[64];

void TmcInit();
void TmcLoad(int id);