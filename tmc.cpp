#define _CRT_SECURE_NO_WARNINGS

#include <stdafx.h>
#include <vector>
#include <assert.h>
#include "game.h"

TMC_ALLOC tmc_alloc[64];

#ifdef _WINDOWS
#include "win/bitmap.h"
#endif

LPCSTR sprt_name0[] =
{
	"",
	"",
	"",
	"ESRTMC\\R1_ESR",
	"ESRTMC\\T1_ESR",
	"ESRTMC\\T2_ESR",
	"ESRTMC\\T3_ESR",
	"ESRTMC\\T4_ESR"
};

LPCSTR sprt_name1[] =
{
	"",
	"",
	"",
	"",
	"CYPTMC\\T1_CYP",
	"CYPTMC\\T2_CYP",
	"CYPTMC\\T3_CYP",
	""
};

LPCSTR sprt_name2[] =
{
	"",
	"",
	"",
	"",
	"BSRTMC\\T1_BSR",
	"BSRTMC\\T2_BSR",
	"BSRTMC\\T3_BSR",
	"BSRTMC\\T4_BSR"
};

LPCSTR sprt_name3[] =
{
	"",
	"",
	"SR1TMC\\M1_SR1",
	"",
	"",
	"",
	"",
	""
};

LPCSTR sprt_name4[] =
{
	"",
	"",
	"",
	"",
	"CHARTMC1\\T_ACT01W",
	"",
	"",
	""
};

LPCSTR sprt_name5[] =
{
	"",
	"",
	"",
	"",
	"LBRTMC\\T1_LBR",
	"LBRTMC\\T2_LBR",
	"LBRTMC\\T3_LBR",
	"LBRTMC\\T4_LBR"
};

LPCSTR sprt_name6[] =
{
	"",
	"",
	"",
	"",
	"BBBTMC\\T1_BBB",
	"BBBTMC\\T2_BBB",
	"BBBTMC\\T3_BBB",
	"BBBTMC\\T4_BBB"
};

LPCSTR sprt_name7[] =
{
	"1\\J_ACT01E",
	"1\\J_ACT02E",
	"1\\J_ACT03E",
	"1\\J_ACT04E",
	"1\\J_ACT05E",
	"1\\J_ACT06E",
	"2\\A_ACT01",
	"2\\D_ACT01",
	"2\\I_ACT01",
	"2\\K_ACT01",
	"2\\K_ACT02E",
	"2\\L_ACT03",
	"2\\M_ACT01",
	"2\\O_ACT01",
	"2\\O_ACT02",
	"2\\R_ACT01",
	"1\\T_ACT01E",
	"1\\T_ACT02E",
	"1\\T_ACT03E",
	"1\\T_ACT04E",
	"2\\D_ACT02",
	"2\\O_ACT04",
	"2\\O_ACT05",
	"2\\O_ACT06",
	"2\\O_ACT07",
	"2\\O_ACT08",
	"2\\O_ACT09",
	"3\\O_ACT0A",
	"3\\O_ACT0B",
	"3\\O_ACT0C",
	"3\\O_ACT0D",
	"3\\O_ACT0E",
	"3\\O_ACT0F",
	"3\\O_ACT0G",
	"3\\O_ACT0H",
	"3\\O_ACT0I",
	"3\\O_ACT0J",
	"3\\O_ACT0K",
	"3\\O_ACT0M",
	"3\\O_ACT0N",
	"3\\O_ACT0O",
	"3\\O_ACT0P",
	"3\\O_ACT0Q",
	"3\\O_ACT0R",
	"3\\O_ACT0S",
	"3\\O_ACT0T",
	"3\\O_ACT0U",
	"3\\O_ACT0V",
	"3\\O_ACT0W",
	"3\\O_ACT0X",
	"3\\O_ACT0Y",
	"3\\O_ACT0Z",
	"2\\K_ACT03",
	"2\\C_ACT02",
	"2\\K_ACT03",
	"2\\M_ACT02",
	"2\\M_ACT02",
	"2\\D_ACT03",
	"1\\J_ACT01W",
	"1\\J_ACT02W",
	"1\\J_ACT03W",
	"1\\J_ACT04W",
	"1\\J_ACT05W",
	"1\\J_ACT06W",
	"",
	"",
	"",
	"",
	"2\\K_ACT02W",
	"",
	"",
	"",
	"",
	"",
	"1\\T_ACT01W",
	"1\\T_ACT02W",
	"1\\T_ACT03W",
	"1\\T_ACT04W",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"1\\J_ACT01U",
	"1\\J_ACT02U",
	"1\\J_ACT03U",
	"1\\J_ACT04U",
	"1\\J_ACT05U",
	"1\\J_ACT06U",
	"",
	"",
	"",
	"",
	"2\\K_ACT02U",
	"",
	"",
	"",
	"",
	"",
	"1\\T_ACT01U",
	"1\\T_ACT02U",
	"1\\T_ACT03U",
	"1\\T_ACT04U",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
};

byte buffer[128 * 1024];

int Tmc::open(const char* filename)
{
	CFile fp;
	fp.Open(filename);

	TMC_HEADER head;
	fp.Read(&head, sizeof(head));

	if (head.magic != 'TM' && head.magic != 'MT')
		return 0;

	// cache all the necessary data
	entry = std::vector<TMC_ENTRY>(head.entry_cnt);
	fp.Read(entry.data(), head.entry_cnt * sizeof(TMC_ENTRY));
	// palettes
	fp.Read(clut, sizeof(clut));
	// compressed pics
	fp.Seek(head.pix_pos, SEEK_SET);
	pix_data = std::vector<BYTE>(head.pix_size);
	fp.Read(pix_data.data(), head.pix_size);
	// coordinates
	fp.Seek(head.ptn_pos, SEEK_SET);
	ptn_data = std::vector<TMC_PTN>(head.ptn_size / sizeof(TMC_PTN));
	fp.Read(ptn_data.data(), head.ptn_size);
	// whatever
	fp.Seek(head.pyx_pos, SEEK_SET);
	pyx_data = std::vector<TMC_PYX>(head.pyx_size / sizeof(TMC_PYX));
	fp.Read(pyx_data.data(), head.pyx_size);

	fp.Close();

	return 1;
}

int Tmc::dec(BYTE* src, BYTE* dst, int cmp_size)
{
	int dec_size = 0;

	while (cmp_size > 0)
	{
		// compressed
		if (*src == 0)
		{
			memset(dst, 0, src[1]);
			dst += src[1];
			dec_size += src[1];
			src += 2;
			cmp_size -= 2;
		}
		else
		{
			*dst++ = *src++;
			cmp_size--;
			dec_size++;
		}
	}

	return dec_size;
}

void Tmc::test(const char* filename)
{
	char path[MAX_PATH];
	CreateDirectoryA("test", nullptr);

	CBitmap bmp;
	for (size_t i = 0; i < entry.size(); i++)
	{
		if (entry[i].pos == -1)
			continue;

		bmp.Create(entry[i].w, entry[i].h);
		dec(&pix_data[entry[i].pos], buffer, entry[i].size);

		BYTE* src = buffer;
		for (int y = 0; y < entry[i].h; y++)
			for (int x = 0; x < entry[i].w; x++)
				bmp.setPixel(x, y, CTim::torgb888(clut[*src++]));

		sprintf_s(path, MAX_PATH, "test\\%03i.png", i);
		bmp.SavePng(path);
	}
}

static signed char tmc_lut[] =
{
	2, 0, 0, 0, 0, 0, 2,-1,-1, 2, 0,-1, 0, 0,-1, 0,
	0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1,-1, 1, 2,-1, 2, 2, 2, 2, 2, 2,
	0,-1, 0,-1, 0, 0, 0, 0, 1, 1, 1,-1
};

void TmcLoad(int id)
{
	id &= 0x7fff;

	int bits = id >> 8;
	if (57 < bits || tmc_alloc[bits].enabled == 1)
		return;

	auto t = &tmc_alloc[bits];
	t->enabled = 1;

	bits = -1;
	short num = id;
	switch (vm_data.vm_index5[0])
	{
	case 0x04:
	case 0x05:
	case 0x0d:
	case 0x13:
	case 0x1a:
	case 0x30:
	case 0x3a:
		switch (num)
		{
		case 0x0600: bits = 0; break;
		case 0x0b00: bits = 1; break;
		case 0x0c00: bits = 2; break;
		case 0x0f00: bits = 3; break;
		case 0x1000: bits = 4; break;
		case 0x1100: bits = 5; break;
		case 0x1200: bits = 6; break;
		case 0x1300: bits = 7; break;
		}
	}

	const char* fname;
	char path[MAX_PATH];
	int fp = num >> 8;

	if ((~bits & 0xffff) >> 0xf != 0)
	{
		switch (vm_data.vm_index5[0])
		{
		case 0x04:
			fname = sprt_name2[bits];
			break;
		case 0x05:
			fname = sprt_name0[bits];
			break;
		case 0x0d:
			fname = sprt_name6[bits];
			break;
		case 0x13:
			fname = sprt_name1[bits];
			break;
		case 0x1a:
			fname = sprt_name5[bits];
			break;
		case 0x30:
			fname = sprt_name3[bits];
			break;
		case 0x3a:
			fname = sprt_name4[bits];
			break;
		default:
			goto other_case;
		}

		sprintf_s(path, MAX_PATH, "SPRT\\%s.TMC", fname);
		fname = path;
	}
	else
	{
other_case:
		if (num < 0x501)
			fp += tmc_lut[vm_data.vm_index5[0]] * 58;
		if (num >= 0xfff && num < 0x1301)
			fp += tmc_lut[vm_data.vm_index5[0]] * 58;
		if (num == 0xa00)
			fp += tmc_lut[vm_data.vm_index5[0]] * 58 + 10;

		sprintf_s(path, MAX_PATH, "SPRT\\CHARTMC%s.TMC", sprt_name7[fp]);
		fname = path;
	}

	t->enabled = 1;
	t->tmc.open(fname);

	printf("Loading TMC %s\n", fname);
}

int tmc_cur_id = - 1;

void TmcInit()
{
	if (tmc_cur_id != tmc_lut[vm_data.vm_index5[0]])
	{
		tmc_cur_id = tmc_lut[vm_data.vm_index5[0]];

		// jennifer's base sprites
		TmcLoad(0x0000);
		TmcLoad(0x0100);
		TmcLoad(0x0200);
		TmcLoad(0x0300);
		TmcLoad(0x0400);
		// giant dan, no idea why he's cached
		TmcLoad(0x0a00);
		// cursor & icons
		TmcLoad(0x3500);
	}
}

void TmcLoadSprt(int id)
{
	if (tmc_alloc[id >> 8].enabled == 0 /*&& < 13*/)
		TmcLoad(id);
}

void TmcUnload()
{
	for (int i = 0; i < _countof(tmc_alloc); i++)
	{
		if (tmc_alloc[i].enabled)
		{
			tmc_alloc[i].enabled = 0;
		}
	}
}

void TmcConvert(SPRT_ENT *s, int id, int frame)
{
	auto gp = (id >> 8) & 0x3f;

	if (tmc_alloc[gp].enabled == 0) return;

	auto t = &tmc_alloc[gp];
	MakeTexture();

	auto f = &t->tmc.entry[frame];
	t->tmc.dec(&t->tmc.pix_data[f->pos], buffer, f->size);
}
