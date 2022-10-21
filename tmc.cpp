#define _CRT_SECURE_NO_WARNINGS

#include <stdafx.h>
#include <vector>
#include "game.h"

#ifdef _WINDOWS
#include "win/bitmap.h"
#endif

LPCSTR sprt_name0[] =
{
	"",
	"",
	"",
	"SPRT\\ESRTMC\\R1_ESR.TMC",
	"SPRT\\ESRTMC\\T1_ESR.TMC",
	"SPRT\\ESRTMC\\T2_ESR.TMC",
	"SPRT\\ESRTMC\\T3_ESR.TMC",
	"SPRT\\ESRTMC\\T4_ESR.TMC"
};

LPCSTR sprt_name1[] =
{
	"",
	"",
	"",
	"",
	"SPRT\\CYPTMC\\T1_CYP.TMC",
	"SPRT\\CYPTMC\\T2_CYP.TMC",
	"SPRT\\CYPTMC\\T3_CYP.TMC",
	""
};

LPCSTR sprt_name2[] =
{
	"",
	"",
	"",
	"",
	"SPRT\\BSRTMC\\T1_BSR.TMC",
	"SPRT\\BSRTMC\\T2_BSR.TMC",
	"SPRT\\BSRTMC\\T3_BSR.TMC",
	"SPRT\\BSRTMC\\T4_BSR.TMC"
};

LPCSTR sprt_name3[] =
{
	"",
	"",
	"SPRT\\SR1TMC\\M1_SR1.TMC",
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
	"SPRT\\CHARTMC1\\T_ACT01W.TMC",
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
	"SPRT\\LBRTMC\\T1_LBR.TMC",
	"SPRT\\LBRTMC\\T2_LBR.TMC",
	"SPRT\\LBRTMC\\T3_LBR.TMC",
	"SPRT\\LBRTMC\\T4_LBR.TMC"
};

LPCSTR sprt_name6[] =
{
	"",
	"",
	"",
	"",
	"SPRT\\BBBTMC\\T1_BBB.TMC",
	"SPRT\\BBBTMC\\T2_BBB.TMC",
	"SPRT\\BBBTMC\\T3_BBB.TMC",
	"SPRT\\BBBTMC\\T4_BBB.TMC"
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
	entries = std::vector<TMC_ENTRY>(head.entry_cnt);
	fp.Read(entries.data(), head.entry_cnt * sizeof(TMC_ENTRY));
	// palettes
	fp.Read(clut, sizeof(clut));
	// compressed pics
	fp.Seek(head.pos_pix, SEEK_SET);
	pix = std::vector<BYTE>(head.pix_size);
	fp.Read(pix.data(), head.pix_size);
	// coordinates
	fp.Seek(head.pos_coor, SEEK_SET);
	coor = std::vector<BYTE>(head.coor_size);
	fp.Read(coor.data(), head.coor_size);
	// whatever
	fp.Seek(head.unkptr_18, SEEK_SET);
	unk = std::vector<BYTE>(head.unksize_1C);
	fp.Read(unk.data(), head.unksize_1C);

	fp.Close();

	count = head.entry_cnt;

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
	for (int i = 0; i < count; i++)
	{
		if (entries[i].pos == -1)
			continue;

		bmp.Create(entries[i].w, entries[i].h);
		dec(&pix[entries[i].pos], buffer, entries[i].size);

		BYTE* src = buffer;
		for (int y = 0; y < entries[i].h; y++)
			for (int x = 0; x < entries[i].w; x++)
				bmp.setPixel(x, y, CTim::torgb888(clut[*src++]));

		sprintf_s(path, MAX_PATH, "test\\%03i.png", i);
		bmp.SavePng(path);
	}
}

void LoadTMC(int id)
{
	static signed char tmc_lut[] =
	{
		2, 0, 0, 0, 0, 0, 2,-1,-1, 2, 0,-1, 0, 0,-1, 0,
		0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1,-1, 1, 2,-1, 2, 2, 2, 2, 2, 2,
		0,-1, 0,-1, 0, 0, 0, 0, 1, 1, 1,-1
	};

	id &= 0x7fff;

	int bits = -1;
	short num = id;
	switch (vm_index5[0])
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
		switch (vm_index5[0])
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
	}
	else
	{
other_case:
		if (num < 0x501)
			fp += tmc_lut[vm_index5[0]] * 58;
		if (num >= 0xfff && num < 0x1301)
			fp += tmc_lut[vm_index5[0]] * 58;
		if (num == 0xa00)
			fp += tmc_lut[vm_index5[0]] * 58 + 10;

		sprintf_s(path, MAX_PATH, "SPRT\\CHARTMC%s", sprt_name7[fp]);
		fname = path;
	}
}
