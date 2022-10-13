#define _CRT_SECURE_NO_WARNINGS

#include <stdafx.h>
#include "game.h"

#define MAX_STR_SIZE	80

WORD vm_usage[5],
	vm_evt_pos,
	vm_index0[500],
	vm_var0,
	vm_index1[30],
	vm_index2[512],
	vm_index3[64],
	vm_index4[64],
	vm_index5[128],
	vm_index6[500],
	vm_index7[30],
	word_42069E;

RECT vm_rects[30];

WORD word_426940;
WORD render_x,
	render_y,
	render_w,
	render_h;

int error_no;

WORD word_41D574[] =
{
	0,  79,   0,  43, 115,   8,  16,   0,   5,  40,  86,  87,  14,  88,  12,  89,
	6,  42,  91,  83,  30,  33, 256, 256,  90,  93,  96,  94,  95,  97,  20,  36,
	52,  21,  53,  37,  31,  35,  41,  66,   9,  38,  17,   3,  47, 512,  63,  44,
	512, 100,  73, 512,  32,  34, 768, 768, 102, 107,1024, 112,  62, 113,  60,  46,
	103, 109,  39,  69, 104,  67, 105, 512,1024,  50,   0,  76,  75,  80,  81,   1,
	77,  78, 118,  19,1280,1280,  10,  11,  13,  15,  24,  18, 119,  25,  27,  28,
	26,  29, 121,1536,  49,1536,  56,  64,  68,  70,1536,  57, 120,  65,1536,1536,
	59,  61, 512,   4, 117, 116,  82,  92, 108,  98
};

WORD word_41D668[] =
{
	22, 23, 45, 48, 51, 71, 114, 54, 55, 58, 72, 84, 85, 99, 101, 106,
	110, 111
};

WORD word_41D68C[] =
{
	0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 4, 4, 5,
	6, 6, 6, 9, 10, 10, 10, 10, 10, 10, 10, 12, 13, 15, 16, 16,
	16, 17, 17, 18, 19, 19, 19, 19, 20, 20, 21, 21, 22, 23, 24, 25,
	26, 27, 27, 28, 29, 29, 29, 29, 29, 29, 30, 31, 31, 32, 32, 34,
	34, 34, 35, 35, 35, 36, 36, 37, 39, 40, 40, 40, 42, 42, 42, 43,
	43, 44, 45, 45, 48, 50, 52, 52, 52, 52, 52, 53, 53, 54, 54, 54,
	55, 55, 56, 56, 56, 56, 56, 57, 57, 57, 57, 57, 58, 58, 58, 58,
	58, 58, 0, 46, 46, 47, 47, 9, 25, 25
};

void vm_func_null() {}

void vm_func_del_data()
{
	if (game_state_get(0))
		vm_index5[30] = 1;
	else
		vm_index5[30] = 0;
}

void vm_func2()
{
	static WORD word_4202F8[3][16] =
	{
		258, 4096, 519,4120, 515,4121,  0,4122, 514,4124, 258,4113,33288,   0,33067,4111,
		1112,4608, 21,4637,  23,4638,   0,4639,  22,4641,1112,4636,32808,   0,32800,4634,
		1115,4864, 27,4886,  29,4887,   0,4120,  28,4890,1115,4885,32809,   0,32801,4883
	};

	static WORD word_420358[][2] =
	{
		518,35840, 532,35850, 534,35852, 258,35854, 533,   0, 277,   0, 272
	};

	WORD v0 = render_x;
	if (vm_index5[25] == 1)
	{
		vm_index5[30] = word_4202F8[vm_index5[4]][v0 * 2];
		vm_index5[31] = word_4202F8[vm_index5[4]][v0 * 2 + 1];
	}
	else if (vm_index5[25] == 2)
	{
		vm_index5[30] = word_420358[v0][0];
		vm_index5[31] = word_420358[v0][1];
	}
	if (vm_index5[27] == 2)
	{
		vm_index5[30] ^= 0x8000u;
		vm_index5[31] ^= 0x8000u;
	}
}

void vm_func3()
{
	static short word_420384[][2] =
	{
		95, -95,
		55, -55,
		45, -45,
		45, -45,
		65, -65
	};

	bool v0; // [esp+0h] [ebp-8h]
	__int16 v1; // [esp+6h] [ebp-2h]

	v0 = vm_index5[27] == 2;
	if (vm_index5[25] == 1)
	{
		v1 = word_420384[vm_index5[4]][v0];
	}
	else if (vm_index5[25] == 2)
	{
		v1 = v1 = word_420384[vm_index5[41] + 3][v0];
	}
	vm_index5[30] = v1 + sprt_ent[0].x0;
}

void vm_func5()
{
	if (render_x <= 1)
		sprt_dat[render_x].type4 = 0;
}

void vm_func6()
{
	if (render_x < 9 && !prog.field_1D4[render_x])
	{
		vm_func7();
		game_state_get(0);
		prog.field_1D4[render_x] = 1;
		WriteData();
	}
}

void vm_func7()
{
	if (game_state_get(1))
	{
		for (int i = 0; i < 9; ++i)
			prog.field_1D4[i] = 0;
	}
}

void vm_func8()
{
	for (int i = 0; i < 9; ++i)
		vm_index3[i + 30] = prog.field_1D4[i];
}

void vm_func_clear()
{
	RenderTile(render_x / 2, render_y / 2, render_w / 2, render_h / 2, 0, 0, 0);
}

void(*vm_funcs[])() =
{
	vm_func_null,
	vm_func_null,	// 1
	vm_func2,		// 2
	vm_func3,		// 3
	vm_func_del_data,	// del_data
	vm_func5,		// 5
	vm_func6,		// 6
	vm_func7,		// 7
	vm_func8,		// 8
	vm_func_clear,	// clear
	vm_func_null,	// A (psx)
	vm_func_null,
	vm_func_null,
	vm_func_null,
	vm_func_null,
	vm_func_null
};

void Vm_all_spr_disp()
{
	RECT rcDst, rc, rcSrc;
	SPRT_ENT* sprt;

	TMapGetDstRect(&tmap, &rcSrc);
	TMapGetRect(&tmap, &rc);
	OffsetRect(&rc, -rc.left, -rc.top);
	for (sprt = prog.sprt; sprt; sprt = sprt->next)
	{
		if (sprt->field_10)
		{
			if (sprt->is_abs)
				Render_sprite(sprt, &rc);
			else
				Render_sprite(sprt, &rcSrc);
		}
	}
	if (prog.vm_func == 1)
	{
		sub_402FAF();
		if (prog.field_14C)
		{
			if (prog.field_14C == 3)
			{
				TMapGetRect(&tmap, &rcDst);
				sprt_ent[11].SetXY(prog.screen_x + prog.field_1B8 - rcDst.left - 16,
					prog.screen_y + prog.field_1BC - rcDst.top - 16, 0x64u, 1);
			}
			SetSpriteData(&sprt_ent[11], sprt_ent[11].field_3B);
			Render_sprite(&sprt_ent[11], &rcSrc);
		}
	}
}

void Vm_all_spr_clr()
{
	SprtTblDeinit();
}

void Vm_spr_clr(WORD index)
{
	sprt_ent[index].Release();
}

void Vm_spr_ent(int a1, DWORD a2, DWORD a3, DWORD a4, __int16 a5, __int16 a6, __int16 a7, DWORD a8, WORD a9)
{
	SprEnt(a1, a2, a3, a4, a5, a6, a7, a8, a9);
	sprt_ent[a1].SetList();
}

void Vm_func_call(WORD r)
{
	if (r < 16)
		vm_funcs[r]();
}

void Vm_set_proc(WORD p)
{
	prog.vm_func = p;
}

void Vm_screen_on()
{
	prog.render_bmp.screen_on = 1;
	prog.screen_on = 1;
}

void Vm_screen_off()
{
	prog.render_bmp.screen_on = 0;
	prog.screen_on = 0;
}

void Vm_slant_clr()
{
	prog.field_198 = 0;
}

void Vm_slant_set(__int16 a1, __int16 a2, __int16 a3, __int16 a4)
{
	prog.field_198 = 1;
	prog.field_19C = a1;
	prog.field_19E = a2;
	prog.field_1A0 = a3 - a1;
	prog.field_1A2 = a4 - a2;
}

void Vm_map_set_clip(int left, int top, int right, int bottom)
{
	TMapSetClipArea(&tmap, left, top, right, bottom);
}

void Vm_pal_set(int type, int index, int count, int delta, WORD id)
{
	prog.pal_obj.set_fade(type, index, count, delta, id);
}

void Vm_sce_init()
{
	prog.field_14C = 0;
	prog.field_140 = 0;
	prog.field_144 = 0;
	prog.field_148 = 0;
	prog.field_138 = 1;
	prog.field_13C = 1;
	prog.pal_obj.reset();
	int id = sub_4035DC();
	prog.pal_obj.f4091AD(id);
	Vm_slant_clr();
	vm_index5[28] = 0;
}

int ReadData(int reload)
{
	char Buffer[20]; // [esp+0h] [ebp-A8h] BYREF
	int ret; // [esp+A0h] [ebp-8h]

	ret = 0;
	FILE *fp;
	fopen_s(&fp, "DATA.BIN", "rb");
	if (fp == nullptr)
		ret = 6;
	else
	{
		fread(Buffer, 0x12, 1, fp);
		if (reload)
			memcpy(prog.field_1D4, Buffer, sizeof(prog.field_1D4));
		else
		{
			fread(prog.field_1E6, 0x24, 1, fp);
			if (fread(vm_index2, 0x400u, 1, fp) == 1024)
			{
				if (fread(vm_index5, 0x100, 1, fp) != 256)
					ret = 7;
			}
			else
				ret = 7;
		}
	}

	if (fp) fclose(fp);

	return ret;
}

int WriteData()
{
	FILE* fp;
	int ret; // [esp+8Ch] [ebp-8h]

	ret = 0;
	fopen_s(&fp, "DATA.BIN", "wb");
	if (fp == nullptr)
	{
		ret = 6;
	}
	else if (fwrite(prog.field_1D4, 18, 1, fp) == 18)
	{
		if (fwrite(prog.field_1E6, 36, 1, fp) == 36)
		{
			if (fwrite(vm_index2, 1024, 1, fp) == 1024)
			{
				if (fwrite(vm_index5, 256, 1, fp) != 256)
					ret = 8;
			}
			else
				ret = 8;
		}
		else
			ret = 8;
	}
	else
		ret = 8;

	if (fp) fclose(fp);

	return ret;
}

int __cdecl game_state_get(int a1)
{
	return ReadData(a1);
}

void __cdecl game_state_set()
{
	word_41D574[word_41D668[0]] = prog.field_1E6[0];
	word_41D574[word_41D668[1]] = prog.field_1E6[1];
	word_41D574[word_41D668[2]] = prog.field_1E6[2];
	word_41D574[word_41D668[3]] = prog.field_1E6[3];
	word_41D574[word_41D668[4]] = prog.field_1E6[4];
	word_41D574[word_41D668[5]] = prog.field_1E6[5];
	word_41D574[word_41D668[6]] = prog.field_1E6[6];
	word_41D574[word_41D668[7]] = prog.field_1E6[7];
	word_41D574[word_41D668[8]] = prog.field_1E6[8];
	word_41D574[word_41D668[9]] = prog.field_1E6[9];
	word_41D574[word_41D668[10]] = prog.field_1E6[10];
	word_41D574[word_41D668[11]] = prog.field_1E6[11];
	word_41D574[word_41D668[12]] = prog.field_1E6[12];
	word_41D574[word_41D668[13]] = prog.field_1E6[13];
	word_41D574[word_41D668[14]] = prog.field_1E6[14];
	word_41D574[word_41D668[15]] = prog.field_1E6[15];
	word_41D574[word_41D668[16]] = prog.field_1E6[16];
	word_41D574[word_41D668[17]] = prog.field_1E6[17];
}

void __cdecl rand_set(DWORD* a1, int cnt)
{
	int v2; // [esp+0h] [ebp-10h]
	int v3; // [esp+4h] [ebp-Ch]
	int j; // [esp+8h] [ebp-8h]
	int i; // [esp+Ch] [ebp-4h]

	i = 0;
	if (cnt > 0)
	{
		while (1)
		{
			v2 = 1;
			v3 = _rand() % cnt;
			j = 0;
			if (i > 0)
			{
				while (a1[j] != v3)
				{
					if (++j >= i)
						goto LABEL_6;
				}
				v2 = 0;
			}
		LABEL_6:
			if (v2)
			{
				a1[i++] = v3;
				if (i >= cnt)
					break;
			}
		}
	}
}

void game_state_rand()
{
	DWORD p[5];

	rand_set(p, 2);
	prog.field_1E6[0] = word_41D668[p[0] + 11];
	prog.field_1E6[1] = word_41D668[p[1] + 11];
	prog.field_1E6[p[0] + 11] = word_41D668[0];
	prog.field_1E6[p[1] + 11] = word_41D668[1];
	rand_set(p, 2);
	prog.field_1E6[7] = word_41D668[p[0] + 9];
	prog.field_1E6[8] = word_41D668[p[1] + 9];
	prog.field_1E6[p[0] + 9] = word_41D668[7];
	prog.field_1E6[p[1] + 9] = word_41D668[8];
	rand_set(p, 5);
	prog.field_1E6[2] = word_41D668[p[0] + 13];
	prog.field_1E6[3] = word_41D668[p[1] + 13];
	prog.field_1E6[4] = word_41D668[p[2] + 13];
	prog.field_1E6[5] = word_41D668[p[3] + 13];
	prog.field_1E6[6] = word_41D668[p[4] + 13];
	prog.field_1E6[p[0] + 13] = word_41D668[2];
	prog.field_1E6[p[1] + 13] = word_41D668[3];
	prog.field_1E6[p[2] + 13] = word_41D668[4];
	prog.field_1E6[p[3] + 13] = word_41D668[5];
	prog.field_1E6[p[4] + 13] = word_41D668[6];
	vm_index5[38] = word_41D668[p[4] + 13];
}

void __cdecl Vm_cont_init()
{
	if (game_state_get(0))
	{
		vm_index5[30] = 0;
	}
	else
	{
		game_state_set();
		vm_index5[30] = 1;
	}
}

void Game_40C6D3(VM* a1, unsigned __int16 a2)
{
	WORD posbk; // [esp+0h] [ebp-4h]
	WORD v3; // [esp+2h] [ebp-2h]

	posbk = vm_evt_pos;
	v3 = 0;
	while (vm_usage[v3])
	{
		if (++v3 >= 5u)
			goto LABEL_5;
	}

	vm_evt_pos = v3;
LABEL_5:
	vm_usage[vm_evt_pos] = 1;
	a1->ado_pos0[vm_evt_pos] = a1->adt[a1->ado_pos1[a2 + 5]].w.w[0];
	a1->ado_pos1[vm_evt_pos] = a1->adt[a1->ado_pos1[a2 + 5]].b.b[2];
	a1->updateIndex();
	vm_evt_pos = posbk;
}

void __cdecl Vm_dest(VM* vm, unsigned __int16 index)
{
	int v2; // edx
	int v3; // cf
	int v4; // edx
	int v5; // eax
	unsigned __int16 a2; // [esp+5Eh] [ebp-2h]

	if (vm_index4[index])
	{
		a2 = vm_index4[index + 1];
		if (vm->ado_pos1[a2 + 5] != 0xFF1F)
		{
			if (vm_index4[59])
				printf("Event destination: 0x%X\n", vm->adt[(unsigned __int16)vm->field_209C[a2]].dw);

			vm->ado_pos_bk[0][vm_evt_pos] = vm->ado_pos0[vm_evt_pos];
			vm->ado_pos_bk[1][vm_evt_pos] = vm->ado_pos1[vm_evt_pos];
			v2 = vm_index6[a2] & 3;
			v3 = v2 == 0;
			v4 = v2 - 1;
			if (v3)
			{
				if (vm_var0 == 1)
					vm->ado_pos_bk[2][vm_evt_pos] = vm->ado_pos2[vm_evt_pos];
				vm->field_24B6[vm_evt_pos][vm->ado_pos2[vm_evt_pos]] = (vm->ado_pos1[vm_evt_pos] << 16) & 0xFF0000;
				v5 = vm->ado_pos2[vm_evt_pos];
				vm->field_24B6[vm_evt_pos][v5] |= vm->ado_pos0[vm_evt_pos];
				vm->ado_pos0[vm_evt_pos] = vm->adt[vm->ado_pos1[a2 + 5]].w.w[0];
				vm->ado_pos1[vm_evt_pos] = HIWORD(vm->adt[vm->ado_pos1[a2 + 5]].dw);
				++vm->ado_pos2[vm_evt_pos];
				vm->updateIndex();
				vm_index4[index] = 0;
			}
			else
			{
				if (v4)
				{
					vm_index4[index + 1] = vm_index4[index + 3];
					Game_40C6D3(vm, a2);
				}
				else
				{
					vm->ado_pos_bk[2][vm_evt_pos] = vm->ado_pos2[vm_evt_pos];
					vm->ado_pos0[vm_evt_pos] = vm->adt[vm->ado_pos1[a2 + 5]].w.w[0];
					vm->ado_pos1[vm_evt_pos] = vm->adt[vm->ado_pos1[a2 + 5]].b.b[2];
					vm->updateIndex();
					vm->ado_pos2[vm_evt_pos] = 0;
				}
				vm_index4[index] = 0;
			}
		}
	}
}

void Vm_event(VM* vm)
{
	unsigned __int16 i; // [esp+0h] [ebp-2h]

	if (vm_index4[38])
	{
		if (vm_index4[38] == 1)
		{
			if (!vm_index4[60] && !vm_index4[61])
			{
				vm_index4[8] = 1;
				--vm_index4[38];
			}
		}
		else
		{
			--vm_index4[38];
		}
	}
	if (vm_index4[39])
	{
		if (vm_index4[39] == 1)
		{
			if (!vm_index4[60] && !vm_index4[61])
			{
				vm_index4[9] = 1;
				--vm_index4[39];
			}
		}
		else
		{
			--vm_index4[39];
		}
	}
	if (vm_index4[37])
	{
		vm->field_35C2 = (vm->field_35C2 + 1) & 0x1F;
		if (!vm->field_35C2)
		{
			if (vm_index4[37] == 1)
				vm_index4[7] = 1;
			--vm_index4[37];
		}
	}
	for (i = 0; i < 10u; ++i)
	{
		if (vm_index4[i] && vm->field_209C[i] != 0xFF1F)
		{
			if (vm_index4[59])
			{
				if (i == 7)
					printf("Firing clock timer event.\n");
				if (i == 8)
					printf("Starting interval timer 1 event.\n");
				if (i == 9)
					printf("Starting interval timer 2 event.\n");
			}
			if (vm_var0 == 1)
				vm->ado_pos_bk[2][vm_evt_pos] = vm->ado_pos2[vm_evt_pos];
			vm->ado_pos_bk[0][vm_evt_pos] = vm->ado_pos0[vm_evt_pos];
			vm->ado_pos_bk[1][vm_evt_pos] = vm->ado_pos1[vm_evt_pos];
			vm->ado_pos0[vm_evt_pos] = vm->adt[vm->ado_pos1[i + 5]].w.w[0];
			vm->ado_pos1[vm_evt_pos] = vm->adt[vm->ado_pos1[i + 5]].b.b[2];
			vm->updateIndex();
			vm_index4[i] = 0;
			vm_index4[60] = 1;
		}
	}

	if (vm_index4[40])
		Vm_dest(vm, 0x28u);
	if (vm_index4[50])
		Vm_dest(vm, 0x32u);
}

void Vm_game_init()
{
	game_state_rand();
	game_state_set();
	vm_index5[2] = 800;
}

void Vm_sce_end()
{
	WriteData();
}

int Vm_wait_fade()
{
	//return PalObj_404C5F(&prog.pal_obj);
	return 1;
}

int Vm_ent_wait(int id)
{
	/*if (sprt_ent[id].field_10)
		return sprt_ent[id].field_8D;
	else */return 1;
}

int Vm_wait_ck(int index)
{
	return index >= 2 || sprt_dat[index].type4 == 0;
	//return 1;
}

void Vm_map_attr(WORD attr)
{
	prog.map_attr = attr;
}

void __cdecl Vm_user_ctr(__int16 a1)
{
	prog.field_12E = a1 & 1;
	prog.field_130 = a1 & 2;
	vm_index5[45] = a1 & 1;
	Vm_spr_dir(0, -1, 0, -1, -1);
}

void Vm_work_clr()
{
	vm_var0 = 0;
	vm_index4[60] = 0;
	vm_index4[61] = 0;
	for (int i = 0; i < 64; ++i)
		vm_index3[i] = 0;
	for (int i = 0; i < 64; ++i)
	{
		if (i != 7 && i != 37)
			vm_index4[i] = 0;
	}
	for (int i = 0; i < 128; ++i)
		vm_index5[i] = 0;
	for (int i = 0; i < 512; ++i)
		vm_index2[i] = 0;
}

void __cdecl Vm_mes_print(VM* game)
{
	bool v1; // [esp+0h] [ebp-14h]
	CHAR glyph[3]; // [esp+7h] [ebp-Dh] BYREF
	int v3; // [esp+Ah] [ebp-Ah]
	unsigned __int16 i; // [esp+Ch] [ebp-8h]
	unsigned __int8 chr; // [esp+Fh] [ebp-5h]
	LONG w; // [esp+10h] [ebp-4h]

	if (game->field_2948[0] == 1)
	{
		v1 = game->field_2944 == 0;
		do
		{
			if (game->wait_timer[0])
			{
				--game->wait_timer[0];
			}
			else if (game->msg_buf[0][game->msg_pos[0]])
			{
				chr = game->msg_buf[0][game->msg_pos[0]];
				if (chr == '\n')
				{
					game->msg_x = game->msg_basex;
					game->msg_y += game->msg_basey + game->msg_v;
					if (++game->msg_pos[0] > 512)
						printf("MSG pointer exceeded buffer capacity\n");
				}
				else
				{
					glyph[0] = chr;
					glyph[1] = 0;
					if (chr > 0x7F)
					{
						glyph[1] = game->msg_buf[0][game->msg_pos[0] + 1];
						glyph[2] = 0;
						v3 = 0;
						++game->msg_pos[0];
					}
					if (++game->msg_pos[0] > 512)
						printf("MSG pointer exceeded buffer capacity\n");

					wchar_t wide[2];
					MultiByteToWideChar(932, 0, glyph, 3, wide, 2);

					w = GamePrintChar(game->msg_x, game->msg_y, wide[0]);
					v3 = w;
					game->msg_x += (WORD)w;
					// auto line carry
					if (game->msg_w + game->msg_basex - game->msg_v <= game->msg_x - TEXT_XDIFF * 2)
					{
						game->msg_x = game->msg_basex;
						game->msg_y += game->msg_basey + game->msg_v;
					}
					game->wait_timer[0] = game->msg_base_wait;
				}
			}
			else
			{
				for (i = 0; i < game->msg_pos[0] + 8; ++i)
					game->msg_buf[0][i] = 0;
				game->field_2948[0] = 0;
				v1 = 1;
				game->field_2944 = 0;
				game->msg_pos[0] = 0;
				game->field_3370[0] = 0;
			}
		} while (!v1);
	}
}

void Vm_map_disp(VM* vm)
{
	SetWorldPos(vm->scrl_x, vm->scrl_y);
	Vm_scroll_world(vm->scroll_x, vm->scroll_y, vm->map_right, vm->map_bottom, vm->scrl_x, vm->scrl_y);
}

int Vm_mark_event(WORD index, WORD a2)
{
	int ret;
	WORD id;

	ret = 1;
	if (index < 0xAu)
	{
		if ((vm_index6[index] & 0x40) == 0 && vm_var0 && (vm_index6[vm_index1[vm_var0]] & 0x20) == 0 && vm_index4[index])
		{
			printf("The event being activated now has the interrupt disabled attribute.\n");
			ret = 0;
		}
		if ((index == 7 || index == 8 || index == 9) && !a2)
		{
			ret = 0;
			vm_index4[index] = 0;
			vm_index4[index + 30] = 0;
		}
		if (ret == 1)
		{
			vm_index1[++vm_var0] = index;
			vm_index4[index] = 1;
			vm_index4[index + 20] = 0;
			vm_index4[index + 30] = a2;
			vm_index0[index] = vm_evt_pos;
			if (index == 7 || index == 8 || index == 9)
				vm_index4[index] = 0;
		}
		return ret;
	}
	id = 50;
	if (index < 0x28u)
		id = 40;
	if ((vm_index6[index] & 3) == 2)
	{
	LABEL_43:
		if (vm_index1[vm_var0] == index && vm_index4[61])
		{
			if (vm_index4[59])
				printf("The same event has already started.\n");
			ret = 0;
		}
		if (ret == 1)
		{
			if ((vm_index6[index] & 3) == 2)
			{
				vm_index4[id + 3] = vm_index4[id + 1];
			}
			else
			{
				vm_index4[61] = 1;
				vm_index1[++vm_var0] = index;
				vm_index4[id + 2] = vm_index4[id + 1];
				vm_index4[id + 3] = a2;
			}
			vm_index4[id + 1] = index;
			vm_index4[id] = 1;
			vm_index0[index] = vm_evt_pos;
			if (vm_index4[59])
				printf("Triggering event number %d.\n", index);
		}
		return ret;
	}
	if (index >= 40u)
		goto LABEL_32;
	if (vm_index6[index + 490] == 0xffff)
	{
		if (vm_index4[59])
			printf("Mark variable is not defined.\n");
		return 0;
	}
	if (vm_index2[vm_index6[index + 490]])
	{
	LABEL_32:
		if ((vm_index6[index] & 0x40) == 0 && (vm_index4[61] || vm_index4[60]) && vm_var0)
		{
			if (vm_index1[vm_var0] >= 0xAu)
			{
				if ((vm_index6[vm_index1[vm_var0]] & 0x20) == 0)
				{
					if (vm_index4[59])
						printf("The event being activated now has the interrupt disabled attribute.\n");
					ret = 0;
				}
			}
			else if ((vm_index6[index] & 3) != 1 && (vm_index6[vm_index1[vm_var0]] & 0x20) == 0)
			{
				if (vm_index4[59])
					printf("The event being activated now has the interrupt disabled attribute.\n");
				ret = 0;
			}
		}
		goto LABEL_43;
	}
	if (vm_index4[59])
		printf("Mark variable is 0.\n");
	return 0;
}

void Vm_set_63()
{
	vm_index4[63] = 1;
}

void Vm_reset(VM* vm)
{
	Vm_screen_off();

	for (int i = 0; i < _countof(vm->ado_pos0); i++)
	{
		vm->ado_pos0[i] = 0x100;
		vm->ado_pos1[i] = 0;
	}

	for (int i = 0; i < _countof(vm->ado); i++)
	{
		vm->ado[i] = new BYTE[SCE_BANK_SIZE];
		memset(vm->ado[i], 0xff, SCE_BANK_SIZE);
	}

	for (int i = 0; i < 500; ++i)
		vm->field_209C[i] = 0xFF1F;

	for (int i = 0; i < 2000; ++i)
		vm->adt[i].dw = 0;

	for (int i = 0; i < 30; ++i)
		vm_index7[i] = -1;
	for (int i = 0; i < 500; ++i)
		vm_index6[i] = 0;

	for (int i = 0; i < 21; ++i)
	{
		vm->bank_no[i] = nullptr;
		vm->bank_spr_id[i] = 0xFFFF;
	}

	for (int i = 0; i < 9; ++i)
		vm->field_33F8[i] = nullptr;

	vm->msg_basex = 32;
	vm->msg_basex2 = 32;
	vm->msg_w = 1280;
	vm->field_207E = 192;
	vm->msg_basey = 0;
	vm->texcol_r = 255;
	vm->texcol_g = 255;
	vm->texcol_b = 255;
	vm->bkcol_r = 0;
	vm->bkcol_g = 0;
	vm->bkcol_b = 0;
	vm->msg_base_wait = 1;
	vm->field_3444 = 0;
	vm->msg_y = 0;
	vm->msg_x = 0;
	vm->msg_v = 32;
	vm->_id = -1;
	vm->field_1F7A = -1;
	vm->r = 0;
	vm->g = 0;
	vm->b = 0;
	vm->field_3434 = 2;
	vm->sample_index = 0;

	for (int i= 0; i < 4; i++)
	{
		vm->sample_names[i][0] = '\0';
		vm->sample_usage[i] = 255;
	}

	vm_usage[0] = 1;
	for (int i = 1; i < 5; ++i)
	{
		vm_usage[i] = 0;
		vm->field_4[i] = 0;
	}

	for (int i = 0; i < 5; ++i)
	{
		vm->ado_pos2[i] = 0;
		vm->field_24A2[i] = 0;
		vm->ado_pos_bk[2][i] = 0;
		vm->field_270E[i] = 1;
		vm->wait_tbl2[i] = 0;
		vm->field_270E[i + 5] = 0;
		vm->wait_tbl2[i + 5] = 0;
		vm->field_2736[i] = 0;
		vm->msg_pos[i] = 0;
		vm->wait_timer[i] = 0;
		vm->field_2948[i] = 0;
		vm->field_2944 = 0;
		vm->field_3370[i] = 0;
		vm->field_28B8[i] = 0;
		for (int j = 0; j < 512; ++j)
			vm->msg_buf[i][j] = 0;
	}

	vm->field_35C0 = 0;
	vm->field_35C2 = 0;
	vm->field_28A2 = 0;
	vm->field_289E = 1;
	vm_evt_pos = 0;
	vm->field_4[vm_evt_pos] = 0;
	vm_index4[7] = 0;
	vm_index4[37] = 0;

	Vm_work_clr();
}

void Vm_door_no_set()
{
	vm_index5[30] = word_41D574[vm_index5[30]];
	vm_index5[31] = word_41D68C[vm_index5[30]];
}

BYTE op_size_tbl[] =
{
	0, 1, 1, 3, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
	5, 2, 8, 2, 0, 1, 0, 2, 2, 1, 1, 1, 1, 6, 6, 0,
	0, 0, 0, 0, 1, 1, 6, 7, 5, 5, 0, 2, 1, 7, 1, 3,
	6, 1, 6, 1, 6, 8, 1, 0, 1, 1, 3, 4, 4, 5, 0, 0,
	0, 3, 3, 5, 0, 3, 1, 1, 0, 0, 0, 0, 2, 1, 0, 0,
	0, 3, 3, 0, 6, 4, 2, 4, 0, 6, 1, 1, 1, 7, 1, 1,
	2, 9, 8, 1, 0, 2, 6, 5, 0
};

void Vm_load(VM *vm)
{
	for(int i = 0; i < _countof(op_size_tbl); i++)
		vm->size_tbl[i] = op_size_tbl[i];

	Vm_reset(vm);

	FILE* fp;

	fopen_s(&fp, "SCE\\CTPS.ADO", "rb");
	if (fp)
	{
		for (int bank = 0, cnt = 0;;)
		{
			int b = fgetc(fp);
			if (b == EOF) break;

			vm->ado[bank][cnt++] = b;
			if (cnt >= 0x8000)
			{
				cnt = 0;
				bank++;
			}
		}
		fclose(fp);
	}

	fopen_s(&fp, "SCE\\CTPS.ADT", "rb");
	if (fp)
	{
		for (int i = 0; i < 2000; i++)
			fread(&vm->adt[i], 4, 1, fp);
		fclose(fp);
	}
}

WORD* Vm_get_env_var(VM* vm, WORD var)
{
	WORD idx = var & 0x3FF;
	switch (var & 0xFF00)
	{
	case 0xC000:
		return &vm_index5[idx];
	case 0xE000:
		return &vm_index4[idx];
	case 0xF000:
		return &vm_index3[idx];
	}

	return &vm_index2[idx];
}

void VM::reset_pos()
{
	if (vm_index4[63])
	{
		vm_var0 = 0;
		ado_pos0[vm_evt_pos] = ado_pos_bk[0][vm_evt_pos];
		ado_pos1[vm_evt_pos] = ado_pos_bk[1][vm_evt_pos];
		ado_pos2[vm_evt_pos] = ado_pos_bk[2][vm_evt_pos];
		vm_index4[42] = 0;
		vm_index4[52] = 0;
		vm_index4[60] = 0;
		vm_index4[61] = 0;
		vm_index4[62] = 0;
		vm_index3[14] = 0;
		vm_index4[63] = 0;
	}
}

void VM::updateIndex()
{
	ado_pos0[vm_evt_pos] += 2;
	if (ado_pos0[vm_evt_pos] >= 0x8000)
	{
		++ado_pos1[vm_evt_pos];
		ado_pos0[vm_evt_pos] = 0;
	}
}

void VM::rewindIndex()
{
	if (ado_pos0[vm_evt_pos])
		ado_pos0[vm_evt_pos] -= 2;
	else
	{
		--ado_pos1[vm_evt_pos];
		ado_pos0[vm_evt_pos] = 0x8000 - 2;
	}
}

WORD VM::consume()
{
	WORD ret = *(WORD*)&ado[ado_pos1[vm_evt_pos]][ado_pos0[vm_evt_pos]];

	updateIndex();
	return ret;
}

WORD VM::consumeInPlace()
{
	WORD ret = *(WORD*)&ado[ado_pos1[vm_evt_pos]][ado_pos0[vm_evt_pos]];

	rewindIndex();
	return ret;
}

WORD VM::read16()
{
	WORD ret = *(WORD*)&ado[ado_pos1[vm_evt_pos]][ado_pos0[vm_evt_pos]];

	if (ret >= 0xF000 && ret <= 0xF1FF)
		ret = vm_index3[ret & 0x1FF];
	if (ret >= 0xE000 && ret <= 0xE1FF)
		ret = vm_index4[ret & 0x1FF];
	if (ret >= 0xC000 && ret <= 0xC1FF)
		ret = vm_index5[ret & 0x1FF];
	if (ret >= 0xD000 && ret <= 0xD1FF)
		ret = vm_index2[ret & 0x1FF];

	updateIndex();
	return ret;
}

void VM::read(WORD *dst, int cnt)
{
	updateIndex();
	for (int i = 0; i < cnt; ++i)
		dst[i] = read16();
}

void VM::getstr(char* dst)
{
	int i, done;

	i = 0;
	done = 0;

	do
	{
		WORD chr = consume();
		if (chr)
		{
			*(WORD*)&dst[i] = chr;
			i += 2;
		}
		else
		{
			dst[i] = 0;
			done = 1;
		}
	} while (!done);

	if (_stricmp(dst, "NULL") == 0)
		strcpy_s(dst, MAX_STR_SIZE, "");

#if 1	// psx name remangler
	char temp[56];
	char temp2[56];

	if (strncmp(dst, "BG", 2) == 0)			// BG
	{
		int bg_no = 1;
		strcpy(temp, dst + 2);
		auto ch = temp[1];
		int ck1, ck2;
		if (temp[1] == 'D')
		{
			bg_no = 3;
		LABEL_29:
			sprintf_s(temp2, sizeof(temp2), "BG%d%s", bg_no, temp);
			strcpy(dst, temp2);
			return;
		}
		if (temp[1] >= 'E')
		{
			if (temp[1] == 'E')
			{
				bg_no = 3;
				ck2 = temp[2] < 'T';
				goto LABEL_27;
			}
			ch = temp[1];
			ck1 = temp[1] < 'U';
		}
		else
		{
			ck1 = temp[1] < 'U';
			if (temp[1] == 'C')
			{
				if (temp[2] >= 'T' && (temp[2] != 'T' || temp[3] >= 'T'))
					bg_no = 2;
				goto LABEL_29;
			}
		}
		if (!ck1)
		{
			bg_no = 6;
			goto LABEL_29;
		}
		ck2 = ch < 'D';
		if (ch >= 'O')
		{
			bg_no = 5;
			goto LABEL_29;
		}
	LABEL_27:
		if (!ck2)
			bg_no = 4;
		goto LABEL_29;
	}

	if (strncmp(dst, "VI", 2) == 0)	// VISUAL
	{
		int vi_num = 1;
		strcpy_s(temp, sizeof(temp), &dst[6]);	// copy after the "visual\\" part
		if (temp[1] == 'V')
		{
			auto vi_chr = *(strrchr(temp, '.') - 1);
			if (vi_chr >= '3')
			{
				if (vi_chr < '7')
					vi_num = 3;
				else vi_num = 4;

				goto label52;
			}
		}
		else if(temp[1] < 'T')
		{
label52:
			sprintf_s(temp2, sizeof(temp2), "VISUAL%d%s", vi_num, temp);
			strcpy(dst, temp2);
			return;
		}
		vi_num = 2;
		goto label52;
	}
#endif
}

void VM::stop()
{
	if (!field_4[vm_evt_pos])
		evt_can_stop = 1;
}

void VM::evt_calc(WORD code)
{
	updateIndex();

	WORD var = consume();
	WORD* env = Vm_get_env_var(this, var);
	WORD par;

	switch (code)
	{
	case EVT_DIV:
		par = read16();
		if (*env && par)
			*env /= par;
		break;
	case EVT_MUL:
		*env *= read16();
		break;
	case EVT_SUB:
		*env -= read16();
		break;
	case EVT_ADD:
		*env += read16();
		break;
	case EVT_DEC:
		--*env;
		break;
	case EVT_INC:
		++*env;
		break;
	case EVT_MOV:
		*env = read16();
		break;
	}
}

void VM::queue(WORD code)
{
	WORD mode;

	updateIndex();
	field_24A2[vm_evt_pos] = consume();
	WORD r0 = consume();
	int ck = condition(r0);
	if (code == EVT_IF)
		mode = EVT_ENDIF;
	else
		mode = EVT_ENDWHILE;
	if (ck)
	{
		if (vm_index4[58])
			printf("Condition has been met.\n");
	}
	else
	{
		if (vm_index4[58])
			printf("Condition was not met.\n");
		int exit_loop = 0;
		do
		{
			WORD mode_now = consume();
			if ((mode_now == mode || mode_now == EVT_ELSE && mode == EVT_ENDIF) && field_24A2[vm_evt_pos] == consume())
				exit_loop = 1;
		} while (!exit_loop);
	}
}

int VM::condition(WORD code)
{
	WORD* dst; // [esp+5Ch] [ebp-14h]
	unsigned __int16 cnt; // [esp+62h] [ebp-Eh]
	__int16 cnt_ck; // [esp+64h] [ebp-Ch]
	unsigned __int16 var_ck; // [esp+66h] [ebp-Ah]
	__int16 env; // [esp+68h] [ebp-8h]
	unsigned __int16 type; // [esp+6Ah] [ebp-6h]
	int exit_loop; // [esp+6Ch] [ebp-4h]

	exit_loop = 0;
	cnt = 0;
	cnt_ck = 0;
	do
	{
		type = consume();
		if (type < EVT_EQU || type > EVT_LEQ)
			exit_loop = 1;
		else
		{
			env = consume();
			dst = Vm_get_env_var(this, env);
			var_ck = read16();
			++cnt;
			if (vm_index4[58])
				printf("Condition %d, work value=%d, compare value=%d\n", cnt, *dst, var_ck);

			switch (type)
			{
			case EVT_EQU:
				if (*dst == var_ck)
					++cnt_ck;
				break;
			case EVT_NEQ:
				if (*dst != var_ck)
					++cnt_ck;
				break;
			case EVT_GRE:
				if (*dst > var_ck)
					++cnt_ck;
				break;
			case EVT_LWR:
				if (*dst < var_ck)
					++cnt_ck;
				break;
			case EVT_GEQ:
				if (*dst >= var_ck)
					++cnt_ck;
				break;
			case EVT_LEQ:
				if (*dst <= var_ck)
					++cnt_ck;
				break;
			default:
				break;
			}
		}
	} while (!exit_loop);

	if (code == 1)
	{
		if (cnt_ck != cnt)
			return 0;
	}
	else if (!cnt_ck)
		return 0;

	return exit_loop;
}

void VM::op_default(WORD code)
{
	if (code < EVT_DIV || code >EVT_MOV)
		updateIndex();
	else
		evt_calc(code);
}

void VM::op_nop()
{
	if (wait_tbl2[vm_evt_pos])
		--wait_tbl2[vm_evt_pos];
	else if (wait_tbl2[vm_evt_pos + 5])
	{
		DWORD tick = (DWORD)getTime();
		if (!field_2736[vm_evt_pos] && (vm_index3[0] & 1) != 0)
			tick = 1000 * wait_tbl2[vm_evt_pos + 5];
		if (tick - wait_tbl[vm_evt_pos] >= 1000u * wait_tbl2[vm_evt_pos + 5])
		{
			updateIndex();
			wait_tbl2[vm_evt_pos + 5] = 0;
		}
	}
	else
		updateIndex();
}

void VM::op_rand()
{
	updateIndex();
	int r0 = consume();
	int r1 = consume();
	int id = consume();
	auto dst = Vm_get_env_var(this, id);

	int add;
	if (r1 - r0 == -1)
		add = 0;
	else
		add = _lrand() % ((unsigned __int16)(r1 - r0) + 1);
	*dst = add + r0;
}

void VM::op_jmp()
{
	updateIndex();
	WORD r = consume();

	if (r < 0xF000)
	{
		ado_pos0[vm_evt_pos] = adt[r].w.w[0];
		ado_pos1[vm_evt_pos] = adt[r].w.w[1];
		if (vm_index4[58])
		{
			if (word_42069E != r)
			{
				word_42069E = r;
				printf("JMP=%d,0x%X\n", r, ado_pos0[vm_evt_pos]);
			}
		}
		updateIndex();
	}
	else
	{
		printf("Can't find label %d\n", r & 0xfff);
		updateIndex();
	}
}

void VM::op_call()
{
	updateIndex();
	WORD r = consume();

	if (r < 0xf000)
	{
		field_24B6[vm_evt_pos][ado_pos2[vm_evt_pos]] = (ado_pos1[vm_evt_pos] << 16) & 0xFF0000;
		WORD v1 = ado_pos2[vm_evt_pos];
		field_24B6[vm_evt_pos][v1] |= ado_pos0[vm_evt_pos];
		ado_pos0[vm_evt_pos] = adt[r].w.w[0];
		ado_pos1[vm_evt_pos] = adt[r].w.w[1];
		if (vm_index4[58] && word_42069E != r)
		{
			word_42069E = r;
			printf("Call=%d,0x%X\n", r, ado_pos0[vm_evt_pos]);
		}
		updateIndex();

		++ado_pos2[vm_evt_pos];
		if (vm_index4[61] || vm_index4[60])
			++vm_index4[62];
	}
	else
	{
		printf("Can't find label %d\n", r & 0xfff);
		updateIndex();
	}
}

void VM::op_else()
{
	int exit_loop = 0;
	do
	{
		if (consume() == EVT_ENDIF && field_24A2[vm_evt_pos] == consume())
			exit_loop = 1;
	} while (!exit_loop);
}

void VM::op_endif()
{
	updateIndex();
	updateIndex();
}

void VM::op_endwhile()
{
	updateIndex();
	WORD v3 = consume();
	rewindIndex();
	rewindIndex();
	rewindIndex();
	int exit_loop = 0;
	do
	{
		if (consumeInPlace() == EVT_WHILE)
		{
			updateIndex();
			updateIndex();
			updateIndex();
			updateIndex();
			WORD v1 = consume();
			if (v1 >= EVT_EQU && v1 <= EVT_LEQ)
			{
				rewindIndex();
				rewindIndex();
				rewindIndex();
				if (v3 == consume())
				{
					rewindIndex();
					rewindIndex();
					exit_loop = 1;
				}
				else
				{
					rewindIndex();
					rewindIndex();
					rewindIndex();
				}
			}
		}
	} while (!exit_loop);
}

void VM::op_evdef()
{
	WORD v1, v2;

	updateIndex();
	v2 = consume();
	v1 = read16();
	field_209C[v1] = v2;
	vm_index6[v1] = read16();
}

void Game_40C792(VM* g)
{
	int v2;

	v2 = g->field_2944;
	g->field_2944 = 1;
	Vm_mes_print(g);//GamePrintSjis(g);
	g->field_2944 = v2;
	do
	{
		Game_RedrawAll(g);
	} while (g->field_28B8[vm_evt_pos]);

	vm_usage[vm_evt_pos] = 0;
}

WORD Game_AdoMatch(VM* a1)
{
	int v2; // [esp+0h] [ebp-10h]
	DWORD v3; // [esp+4h] [ebp-Ch]
	WORD v4; // [esp+Eh] [ebp-2h]

	v2 = 0;
	v4 = 0;
	do
	{
		v3 = a1->adt[v4].dw;
		if ((a1->ado_pos0[vm_evt_pos] | (a1->ado_pos1[vm_evt_pos] << 16)) == v3)
			v2 = 1;
		else if (v3)
			++v4;
		else
		{
			v2 = 1;
			v4 = -1;
		}
	} while (!v2);

	return v4;
}

void VM::op_evret()
{
	if (ado_pos2[vm_evt_pos])
	{
		ado_pos0[vm_evt_pos] = (WORD)field_24B6[vm_evt_pos][--ado_pos2[vm_evt_pos]];
		ado_pos1[vm_evt_pos] = (field_24B6[vm_evt_pos][ado_pos2[vm_evt_pos]] & 0xFF0000) >> 16;
		if ((vm_index4[61] || vm_index4[60]) && vm_index0[vm_index1[vm_var0]] == vm_evt_pos)
		{
			if (vm_index4[62])
				--vm_index4[62];
			else
			{
				if (vm_index4[59])
					printf("Event number %d is complete. (call over)\n", vm_index1[vm_var0]);

				if (vm_index1[vm_var0] >= 0xAu)
					vm_index4[61] = 0;
				else
					vm_index4[60] = 0;
				--vm_var0;
			}
		}
	}
	else if (vm_index4[60] || vm_index4[61])
	{
		if (vm_index0[vm_index1[vm_var0]] == vm_evt_pos)
		{
			if (vm_index4[59])
				printf("Event number %d is complete.\n", vm_index1[vm_var0]);

			if (vm_index1[vm_var0] >= 0xAu)
				vm_index4[61] = 0;
			else
				vm_index4[60] = 0;
			ado_pos0[vm_evt_pos] = ado_pos_bk[0][vm_evt_pos];
			ado_pos1[vm_evt_pos] = ado_pos_bk[1][vm_evt_pos];
			--vm_var0;
		}
		else if (vm_evt_pos)
		{
			Game_40C792(this);
			evt_can_stop = 1;
			if (vm_index4[59])
				printf("Task number %d is complete.\n", vm_evt_pos);
		}
	}
	else if (vm_evt_pos)
	{
		Game_40C792(this);
		evt_can_stop = 1;
		if (vm_index4[59])
			printf("Task number %d is complete.\n", vm_evt_pos);
	}
	else
	{
		vm_index4[0] = Game_AdoMatch(this);
		updateIndex();
	}
}

void VM::op_evtstart()
{
	WORD evt_no, sub;

	updateIndex();
	evt_no = read16();
	sub = read16();
	if (field_209C[evt_no] == 0xff1f || field_209C[evt_no] == 0xffff)
		printf("Event number %d is not defined.\n", evt_no);
	else
		Vm_mark_event(evt_no, sub);
}

void VM::op_set_proc()
{
	updateIndex();
	Vm_set_proc(read16());
}

void VM::op_set_mark()
{
	WORD mark; // ax

	updateIndex();
	for (int i = 0; i < 30; ++i)
		vm_index7[i] = -1;
	int loop_end = 0;
	int i = 0;
	do
	{
		mark = read16();
		if (mark == EVT_END)
			loop_end = 1;
		else
		{
			vm_rects[i].left = (short)mark;
			vm_rects[i].top = read16s();
			vm_rects[i].right = read16s();
			vm_rects[i].bottom = read16s();
			vm_index7[i++] = consume();
		}
	} while (!loop_end);
}

void VM::op_user_ctr()
{
	updateIndex();
	Vm_user_ctr(read16());
}

void VM::op_set_bkcol(int size)
{
	read((WORD*)&r, size);
}

void VM::op_set_msgcol(int size)
{
	read((WORD*)&texcol_r, size);
	//SetBkColor
	//SetTextColor
}

void VM::op_wait()
{
	updateIndex();
	wait_tbl2[vm_evt_pos] = read16();
	field_270E[vm_evt_pos + 5] = read16();
}

void VM::op_fawait()	// fade
{
	if (Vm_wait_fade())
		updateIndex();
}

void VM::op_btwait()
{
	updateIndex();
	WORD v1 = read16();
	if ((v1 & 1) != 0 && (vm_index3[0] & 1) == 0 || (v1 & 2) != 0 && (vm_index3[0] & 2) == 0)
	{
		rewindIndex();
		rewindIndex();
	}
}

void VM::op_bg_wait()
{
	if (Vm_bg_scrolling() && !field_28A2 && !field_28B8[vm_evt_pos])
		updateIndex();
}

void VM::op_tmwait()
{
	updateIndex();
	wait_tbl2[vm_evt_pos + 5] = read16();
	field_2736[vm_evt_pos] = read16();
	wait_tbl[vm_evt_pos] = getTime();
	vm_index3[0] = 0;
}

void VM::op_screen_clr()
{
	updateIndex();
	//clear the screen here
	RenderTile(0, 0, GAME_W, GAME_H, 0, 0, 0);
}

void VM::op_box_fill()
{
	updateIndex();
	int x0 = read16s();
	int y0 = read16s();
	int x1 = read16s();
	int y1 = read16s();
	WORD r = read16();
	WORD g = read16();
	WORD b = read16();
	RenderTile(GETX(x0), GETY(y0), (x1 - x0) / 2, (y1 - y0) / 2, (BYTE)r, (BYTE)g, (BYTE)b);
}

void VM::op_bg_load()
{
	updateIndex();

	char str[MAX_STR_SIZE];
	WORD id = read16() + 1;
	getstr(str);

	if (bank_no[id])
	{
		if(bank_no[id]->is_ref == 0)
			delete bank_no[id];
		bank_no[id] = nullptr;
	}

	bank_no[id] = MakeBankFromDIB(str);

	if (bank_no[id])
	{
		rects[id].x = 0;
		rects[id].y = 0;
		rects[id].w = bank_no[id]->real_w * 2;
		rects[id].h = bank_no[id]->pix_h * 2;
		rects[id].rw1 = 0;
		rects[id].rw2 = 0;
	}
}

void VM::op_bg_disp()
{
	updateIndex();
	int x = read16s();
	int y = read16s();
	WORD id = read16() + 1;
	int w = read16();
	int h = read16();
	WORD type = read16();
	if (id >= 21)
		id = _id;
	Game_BgDispTrn(this, x, y, w, h, 0, 0, id, type != 0, 0);
}

void VM::op_bg_disp_trn()
{
	updateIndex();
	int x = read16s();
	int y = read16s();
	WORD id = read16() + 1;
	int w = read16();
	int h = read16();
	WORD type = read16();
	WORD v1 = read16();
	if (id >= 21)
		id = _id;
	Game_BgDispTrn(this, x, y, w, h, 0, 0, id, type != 0, v1 != 0);
}

void VM::op_bg_spr_ent(int is_abs)
{
	updateIndex();
	WORD id = read16();
	short x = read16s();
	short y = read16s();
	WORD flag = read16();
	WORD bank_id = read16() + 1;
	WORD v3 = read16() != 0;

	if (x < 0)
		x = 64;
	if (y < 0)
		y = 0;

	byte unk = bank_id >> 8;
	bank_id &= 0xff;
	if (bank_no[bank_id])
	{
		bank_spr_id[id] = bank_id;
	//	if (g->bank_no[bank_id]->frame)
	//		frame = g->bank_no[bank_id]->frame;
	//	else
	//		frame = 1 << g->bank_no[bank_id]->bitframe;
		EntryBmpSprite(id, x, y, flag,
			bank_no[bank_id]->real_w,
			bank_no[bank_id]->pix_h,
			bank_no[bank_id],
			v3,
			is_abs);
	}
}

void VM::op_bg_file_disp()
{
	char path[MAX_STR_SIZE];

	updateIndex();
	WORD v4 = read16();
	WORD v3 = read16();
	getstr(path);
	//Game_LoadBmp(attr, loop_end, ret, Destination);
}

void VM::op_bg_anim()
{
	field_2926[vm_evt_pos] = 0;
	updateIndex();

	field_28E0[vm_evt_pos] = read16();
	field_28E8[vm_evt_pos + 1] = read16();
	field_28CC[vm_evt_pos + 5] = read16();
	field_28FE[vm_evt_pos] = read16();
	field_28B8[vm_evt_pos + 5] = read16();
	field_28CC[vm_evt_pos] = field_28B8[vm_evt_pos + 5];
	field_28B8[vm_evt_pos] = read16();
	field_2908[vm_evt_pos] = read16();
	if (++field_28CC[vm_evt_pos + 5] >= 0x15u)
		field_28CC[vm_evt_pos + 5] = _id;
	rect_xy_index[vm_evt_pos] = field_28CC[vm_evt_pos + 5];
	if (!field_28B8[vm_evt_pos + 5])
	{
		for (int i = 0; field_28B8[vm_evt_pos] > i; ++i)
			Game_RedrawScene(this);
		field_28B8[vm_evt_pos] = 0;
	}
}

void VM::op_bg_scroll()
{
	updateIndex();
	rect_index = read16();
	if (++rect_index >= 0x15u)
		rect_index = _id;
	field_28AA = read16();
	rx = read16();
	field_28A4 = read16();
	field_28A6 = field_28A4;
	field_28A2 = read16();
	if (!field_28A4)
	{
		for (int i = 0; i < field_28A2; ++i)
			Game_BgDispTrn_0(this);
		field_28A2 = 0;
	}
}

void VM::op_bg_clr()
{
	updateIndex();
	WORD r1 = read16() +1;
	GameDrawFromRects(this, r1);
}

void VM::op_pal_load()
{
	char path[MAX_STR_SIZE];
	updateIndex();
	WORD r1 = read16() + 1;
	getstr(path);
	//LoadPal(r1 - 1, path);
}

void VM::op_map_load()
{
	char dib[MAX_STR_SIZE],
		map[MAX_STR_SIZE];
	char *dib_name,
		 *map_name;

	updateIndex();

	WORD unused = read16();
	getstr(dib);
	getstr(map);

	dib_name = (dib[0] == '\0') ? nullptr : dib;
	map_name = (map[0] == '\0') ? nullptr : map;

	TMapCache(dib_name, map_name);
}

void VM::op_map_attr()
{
	updateIndex();
	field_3434 = read16();
	Vm_map_attr(field_3434 & 3);
}

void VM::op_map_init(int size)
{
	read(&map_left, size);
	Vm_map_set_clip(map_left, map_top, map_right, map_bottom);
}

void VM::op_map_pos()
{
	updateIndex();
	scrl_x = read16s();
	scrl_y = read16s();
	field_3426 = read16s();
	SetWorldPos(scrl_x, scrl_y);
}

void VM::op_map_disp()
{
	updateIndex();
	for (int i = 0; i < 6; ++i)
		*(&scroll_x + i) = read16s();
	Vm_map_disp(this);
	for (int i = 0; i < 30; ++i)
	{
		if (vm_index7[i] != 0xffff
			&& vm_index2[vm_index7[i]]
			&& vm_rects[i].left >= scrl_x
			&& vm_rects[i].top >= scrl_y)
		{
			int x0 = scroll_x + vm_rects[i].left - scrl_x;
			int y0 = scroll_y + vm_rects[i].top - scrl_y;
			int x1 = vm_rects[i].right - vm_rects[i].left + x0;
			int y1 = vm_rects[i].bottom - vm_rects[i].top + y0;

			RenderTile(GETX(x0), GETY(y0), (x1 - x0) / 2, (y1 - y0) / 2, 0, 255, 0);
		}
	}
}

void VM::op_map_scroll()
{
	updateIndex();
	short x = read16s();
	short y = read16s();
	short m = read16s();
	Vm_map_scroll(x, y, m);
}

void VM::op_map_wrt()
{
	char path[MAX_STR_SIZE];

	updateIndex();
	WORD v3 = read16();
	WORD v2 = read16();
	getstr(path);
}

void VM::op_msg_attr(int cnt)
{
	read(&texcol_r, cnt);
	vm_index3[13] = (field_3434 & 2) != 0;
}

void VM::op_msg_init(int size)
{
	read(&msg_basex, size);
	msg_x = msg_basex;
	msg_y = msg_basex2;
}

void VM::op_msg_out()
{
	char buf[128]; // [esp+0h] [ebp-8Ch] BYREF
	WORD mes_len; // [esp+80h] [ebp-Ch]
	WORD v3; // [esp+82h] [ebp-Ah]
	int v4; // [esp+84h] [ebp-8h]
	int end_loop; // [esp+88h] [ebp-4h]

	vm_index3[0] = 0;
	field_2944 = 0;
	updateIndex();
	v3 = read16();
	mes_len = read16();
	if (v3 != 0xF000)
		msg_x = v3 + msg_basex;
	if (mes_len != 0xF000)
		msg_y = mes_len + msg_basex2;
	end_loop = 0;
	mes_len = 0;
	do
	{
		v3 = consume();
		switch (v3)
		{
		case EVT_RET:
			end_loop = 1;
			break;
		case EVT_ALLEND:
			end_loop = 1;
			ado_pos0[vm_evt_pos] -= 2;
			break;
		case EVT_END:
			end_loop = 1;
			break;
		case EVT_BAD_OPC:
			end_loop = 1;
			break;
		default:
			if (v3 >> 8)
			{
				buf[mes_len] = v3 & 0xff;
				buf[++mes_len] = v3 >> 8;
			}
			else
				buf[mes_len] = v3 & 0xff;
			++mes_len;
			break;
		}
	} while (!end_loop);

	buf[mes_len] = 0;
	if (vm_index3[15])
	{
		wchar_t wbuf[128];

		MultiByteToWideChar(932, MB_ERR_INVALID_CHARS, buf, -1, wbuf, 128);
		wprintf(wbuf);
	}

	strcpy_s(msg_buf[field_3370[0]], 512, buf);

	field_3370[0] += (WORD)strlen(buf);
	wait_timer[0] = msg_base_wait;
	field_2948[0] = 1;
	if (!wait_timer[0])
	{
		v4 = field_2944;
		field_2944 = 1;
		Vm_mes_print(this);
		field_2944 = v4;
	}
}

void VM::op_msg_wait()
{
	if (field_2948[0] != 1)
		updateIndex();
}

void VM::op_msg_clr()
{
	updateIndex();

	// do clearing here
	RenderTile(48, 160, 256 - 48, 240 - 160, 0, 0, 0);

	msg_x = msg_basex;
	msg_y = msg_basex2;
}

void VM::op_msg_spd(int size)
{
	read(&msg_base_wait, size);
}

void VM::op_bgm_req()
{
	char path[MAX_STR_SIZE];

	updateIndex();
	WORD r1 = read16();
	getstr(path);

	Midi_load(path);
	//Vm_bgm_req(path, r1);
}

void VM::op_bgm_attr()
{
	updateIndex();
	WORD r0 = read16();
	WORD r1 = read16();
}

void VM::op_se_req()
{
	char path[MAX_STR_SIZE];

	updateIndex();
	WORD loops = read16();
	getstr(path);
	Sound_play(path, loops, 0, 127);
}

void VM::op_se_stop()
{
	char path[MAX_STR_SIZE];

	updateIndex();
	getstr(path);
	Sound_stop(path);

	if (!_stricmp(path, "ALL"))
	{
		for (int i = 0; i < 4; ++i)
		{
			sample_names[i][0] = 0;
			sample_usage[i] = 0xff;
		}
	}
	else
	{
		int i = 0;
		while (_stricmp(sample_names[i], path))
			if (++i >= 4)
				return;

		sample_names[i][0] = 0;
		sample_usage[i] = 0xff;
	}
}

void VM::op_se_vol()
{
	char path[MAX_STR_SIZE];
	updateIndex();
	WORD vol = read16();
	getstr(path);

	Sound_set_vol(path, vol);
}

void VM::op_se_pan()
{
	char path[MAX_STR_SIZE];
	updateIndex();
	WORD pan = read16();
	getstr(path);

	Sound_set_pan(path, pan);
}

void VM::op_se_req_spr()
{
	char path[MAX_STR_SIZE];

	updateIndex();
	WORD loops = read16();
	WORD pos = read16();
	WORD vol = read16();
	getstr(path);

	Sound_play(path, loops, EntGetPan(pos), vol);
	
	// find unused sample
	int i = 0;
	while (sample_usage[i] != 0xff)
	{
		if (++i >= 4)
			return;
	}

	// set usage
	strcpy(sample_names[i], path);
	sample_usage[i] = vol;
}

void VM::op_se_req_pv()
{
	char path[MAX_STR_SIZE];

	updateIndex();
	WORD loops = read16();
	WORD pan = read16(); - 16;
	WORD vol = read16();
	getstr(path);

	Sound_play(path, loops, pan, vol);
}

void VM::op_sce_reset()
{
	WORD posbk = vm_evt_pos;
	for (int i = 0; i < 5; ++i)
	{
		vm_evt_pos = i;
		vm_index4[63] = 1;
		reset_pos();
		field_28A2 = 0;
		field_28B8[vm_evt_pos] = 0;
		auto v4 = field_2944;
		field_2944 = 1;
		Vm_mes_print(this);
		field_2944 = v4;
		Game_BgDispTrn_1(this);
		Game_RedrawAll(this);
		Vm_event(this);
	}

	for (int i = 1; i < 5; ++i)
		vm_usage[i] = 0;

	vm_evt_pos = posbk;
	ado_pos0[0] = ado_pos0[posbk];
	ado_pos1[0] = ado_pos1[vm_evt_pos];
	vm_index4[8] = 0;
	vm_index4[9] = 0;
	vm_index4[38] = 0;
	vm_index4[39] = 0;
}

void VM::op_pal_set()
{
	updateIndex();

	WORD type = read16();
	WORD index = read16();
	WORD count = read16();
	WORD delta = read16();
	WORD id = read16();

	Vm_pal_set(type, index, count, delta, id);
}

void VM::op_bg_spr_pos()
{
	updateIndex();
	WORD id = read16();
	int x = read16s();
	int y = read16s();
	WORD flags = read16();
	BgSprPos(id, x, y, flags);
}

void VM::op_bg_spr_set()
{
	updateIndex();
	WORD attr = read16();
	WORD id = read16() + 1;
	if (bank_no[id])
	{
		int v1;
		//if (bank_no[id]->frame)
		//	v1 = bank_no[id]->frame;
		//else
		//	v1 = 1 << bank_no[id]->bitframe;
		v1 = bank_no[id]->bpp;
		BgSprAnim(attr, bank_no[id]->real_w, bank_no[id]->pix_h, bank_no[id]);
	}
}

void VM::op_bg_buf_clr()
{
	updateIndex();
	int start = read16();
	int cnt = read16();
	if (start > cnt)
		std::swap(start, cnt);

	if (start != 0xffff)
	{
		for (int i = start; i < cnt + 1; ++i)
		{
			if (bank_no[i])
			{
				delete bank_no[i];
				bank_no[i] = 0;
			}
			for (int j = 0; j < 0x15u; ++j)
			{
				if (bank_spr_id[j] == i)
					Vm_spr_clr(j);
			}
		}
	}
	else if (cnt != 0xffff)
	{

	}
	else
	{

	}
}

void VM::op_bg_spr_anim()
{
	field_2926[vm_evt_pos] = 1;
	updateIndex();
	field_28E0[vm_evt_pos] = read16();
	field_28E8[vm_evt_pos + 1] = read16();
	*((WORD*)&field_28F4 + vm_evt_pos) = read16();
	field_28CC[vm_evt_pos + 5] = read16();
	field_28FE[vm_evt_pos] = read16();
	field_28B8[vm_evt_pos + 5] = read16();
	field_28CC[vm_evt_pos] = field_28B8[vm_evt_pos + 5];
	field_28B8[vm_evt_pos] = read16();
	field_2908[vm_evt_pos] = 0;
	if (++field_28CC[vm_evt_pos + 5] >= 0x15u)
		field_28CC[vm_evt_pos + 5] = _id;
	rect_xy_index[vm_evt_pos] = field_28CC[vm_evt_pos + 5];
	ent_index[vm_evt_pos] = read16();
	if (read16())
		field_2912[vm_evt_pos] = 1;
	else
		field_2912[vm_evt_pos] = 0;
	if (!field_28B8[vm_evt_pos + 5])
	{
		for (int i = 0; field_28B8[vm_evt_pos] > i; ++i)
			Game_RedrawScene(this);
		field_28B8[vm_evt_pos] = 0;
	}
}

void VM::op_spr_clr()
{
	updateIndex();
	Vm_spr_clr(read16());
}

void VM::op_spr_ent(int is_abs)
{
	updateIndex();
	WORD id = read16();
	WORD v5 = read16();
	WORD v4 = read16();
	WORD v3 = read16();
	WORD v8 = read16();
	WORD v7 = read16();
	WORD v6 = read16();
	if (read16())
		Vm_spr_ent(id, v5, v4, v3, v8, v7, v6, 1, is_abs);
	else
		Vm_spr_ent(id, v5, v4, v3, v8, v7, v6, 0, is_abs);
}

void VM::op_spr_pos()
{
	updateIndex();
	WORD id = read16();
	int x = read16s();
	int y = read16s();
	WORD flags = read16();
	SprPos(id, x, y, flags);
}

void VM::op_spr_anim()
{
	updateIndex();
	WORD v5 = read16();
	WORD v4 = read16();
	WORD v3 = read16();
	WORD v2 = read16();
	SprAnim(v5, v4, v3, v2);
}

void VM::op_spr_dir()
{
	updateIndex();
	WORD v5 = read16();
	int v4 = read16s();
	int v3 = read16s();
	int v2 = read16s();
	int v1 = read16s();
	Vm_spr_dir(v5, v4, v3, v2, v1);
}

void VM::op_spr_lmt()
{
	updateIndex();
	WORD id = read16();
	WORD x = read16();
	WORD y = read16();

	int lmx, lmy;
	if (x < 0x8000) lmx = x & 0x7FFF;
	else lmx = (unsigned __int16)(x + 0x8000) - 0x8000;
	if (y < 0x8000) lmy = y & 0x7FFF;
	else lmy = (unsigned __int16)(y + 0x8000) - 0x8000;

	Vm_spr_lmt(id, lmx, lmy);
}

void VM::op_spr_walkx()
{
	updateIndex();
	WORD id = read16();
	int x0 = read16s();
	int x1 = read16s();
	int v3 = read16s();
	int v2 = read16s();
	Vm_spr_walk_x(id, x0, x1, v3, v2);
}

void VM::op_spr_wait()
{
	updateIndex();
	WORD v2 = read16();
	int v3 = Vm_ent_wait(v2);
	int v1 = Vm_wait_ck(v2);
	if (!v3 || !v1)
	{
		rewindIndex();
		rewindIndex();
	}
}

void VM::op_spr_mark()
{
	updateIndex();
	WORD r0 = read16();
	WORD r1 = read16();
	WORD r2 = read16();
	WORD r3 = read16();
	WORD r4 = read16();
}

void VM::op_abs_obj_anim()
{
	updateIndex();
	WORD id = read16();
	int x = read16s();
	int y = read16s();
	WORD flags = read16();
	read16();
	WORD a2 = read16();
	WORD a3 = read16();
	WORD a4 = read16();
	SprAnim(id, a2, a3, a4);
	SprPos(id, x, y, flags);
}

void VM::op_obj_anim()
{
	updateIndex();
	WORD id = read16();
	int x = read16s();
	int y = read16s();
	WORD flags = read16();
	read16();
	WORD a2 = read16();
	WORD a3 = read16();
	WORD a4 = read16();
	SprAnim(id, a2, a3, a4);
	SprPos(id, x, y, flags);
}

void VM::op_slant_set()
{
	updateIndex();

	WORD a = read16();
	WORD b = read16();
	WORD c = read16();
	WORD d = read16();

	Vm_slant_set(a, b, c, d);
	stop();
}

void VM::op_slant_clr()
{
	updateIndex();
	Vm_slant_clr();
	stop();
}

void VM::op_scl_block()
{
	updateIndex();
	int v3 = read16();
	for (int i = 0; i < v3; ++i)
		field_3446[i] = read16();
	int v2 = read16();
	for (int i = 0; i < v2; ++i)
		field_3482[i] = read16();
	Vm_tmap_set_scroll(v3, field_3446, v2, field_3482);
}

void VM::op_spc_func()
{
	updateIndex();
	WORD fnc = read16();
	word_426940 = read16();
	for (int i = 0; i < word_426940; ++i)
		*(&render_x + i) = read16();
	Vm_func_call(fnc);
}

void VM::op_sce_init()
{
	updateIndex();
	Vm_sce_init();
}

void VM::op_sce_end()
{
	updateIndex();
	Vm_sce_end();
}

void VM::op_game_init()
{
	updateIndex();
	Vm_game_init();
}

void VM::op_cont_init()
{
	updateIndex();
	Vm_cont_init();
}

void VM::op_avi_play()
{
	char path[MAX_STR_SIZE];

	updateIndex();
	WORD flag = read16();
	int x = read16s();
	int y = read16s();
	int w = read16();
	int h = read16();
	getstr(path);
}

LPCSTR op_names[] =
{
	"ALLEND",	// 20
	"JMP",		// 21
	"CALL",		// 22
	"EVDEF",	// 23
	"STRANGE1",	// 24
	"STRANGE2",	// 25
	"STRANGE3",	// 26
	"STRANGE4",	// 27
	"END",		// 28
	"IF",		// 29
	"WHILE",	// 2A
	"NOP",		// 2B
	"BREAK",	// 2C
	"ENDIF",	// 2D
	"ENDWHILE",	// 2E
	"ELSE",		// 2F
	"MSGINIT",	// 30
	"MSGTYPE",	// 31
	"MSGATTR",	// 32
	"MSGOUT",	// 33
	"SETMARK",	// 34
	"SETWAIT",	// 35
	"MSG_WAIT",	// 36
	"EVSTART",	// 37
	"BGFILEDISP",// 38
	"BGLOAD",	// 39
	"PALLOAD",	// 3A
	"BGMREQ",	// 3B
	"SPRCLR",	// 3C
	"ABSOBJANIM",// 3D
	"OBJANIM",	// 3E
	"ALLSPRCLR",// 3F
	"MSGCLR",	// 40
	"SCREENCLR",// 41
	"SCREENON",	// 42
	"SCREENOFF",// 43
	"SCREENIN",	// 44
	"SCREENOUT",// 45
	"BGDISP",	// 46
	"BGANIM",	// 47
	"BGSCROLL",	// 48
	"PALSET",	// 49
	"BGWAIT",	// 4A
	"WAIT",		// 4B
	"BWAIT",	// 4C
	"BOXFILL",	// 4D
	"BGCLR",	// 4E
	"SETBKCOL",	// 4F
	"MSGCOL",	// 50
	"MSGSPD",	// 51
	"MAPINIT",	// 52
	"MAPLOAD",	// 53
	"MAPDISP",	// 54
	"SPRENT",	// 55
	"SETPROC",	// 56
	"SCEINIT",	// 57
	"USERCTRL",	// 58
	"MAPATTR",	// 59
	"MAPPOS",	// 5A
	"SPRPOS",	// 5B
	"SPRANIM",	// 5C
	"SPRDIR",	// 5D
	"GAMEINIT",	// 5E
	"CONTINIT",	// 5F
	"SCEEND",	// 60
	"MAPSCROLL",// 61
	"SPRLMT",	// 62
	"SPRWALKX",// 63
	"ALLSPRDISP",// 64
	"MAPWRT",	// 65
	"SPRWAIT",	// 66
	"SEREQ",	// 67
	"SNDSTOP",	// 68
	"SESTOP",	// 69
	"BGMSTOP",	// 6A
	"DOORNOSET",// 6B
	"RAND",		// 6C
	"BTWAIT",	// 6D
	"FAWAIT",	// 6E
	"SCLBLOCK",	// 6F
	"EVSTOP",	// 70
	"SEREQPV",	// 71
	"SEREQSPR",	// 72
	"SCERESET",	// 73
	"BGSPRENT",	// 74
	"BGSPRPOS",	// 75
	"BGSPRSET",	// 76
	"SLANTSET",	// 77
	"SLANTCLR",	// 78
	"DUMMY",	// 79
	"SPCFUNC",	// 7A
	"SEPAN",	// 7B
	"SEVOL",	// 7C
	"BGDISPTRN",// 7D
	"DEBUG",	// 7E
	"TRACE",	// 7F
	"TMWAIT",	// 80
	"BGSPRANIM",// 81
	"ABSSPRENT",// 82
	"NEXTCOM",	// 83
	"WORKCLR",	// 84
	"BGBUFCLR",	// 85
	"ABSBGSPRENT",// 86
	"AVIPLAY",	// 87
	"AVISTOP",	// 88
	"SPRMARK",	// 89
	"BGMATTR",	// 8A
	"BAD_OPC"
};

int Vm_execute(VM* vm)
{
	int ret = 0;
	WORD size = 0;

	if ((vm_index3[0] & 1) != 0)
		vm->field_2944 = 1;

	do
	{
		vm->evt_can_stop = 0;
		ret = 0;

		WORD op = vm->ado[vm->ado_pos1[vm_evt_pos]][vm->ado_pos0[vm_evt_pos]];
		op |= vm->ado[vm->ado_pos1[vm_evt_pos]][vm->ado_pos0[vm_evt_pos] + 1] << 8;

		EVT_CODES code = (EVT_CODES)op;

		if (op >= EVT_ALLEND && op != EVT_BAD_OPC)
		{
			size = vm->size_tbl[op - EVT_ALLEND];
			if (vm_index4[58])
			{
				if (op != EVT_JMP && op != EVT_CALL && op != EVT_WAIT && op != EVT_NOP && op != EVT_SPRWAIT && op != EVT_BGWAIT && op != EVT_MSG_WAIT)
					printf("0x%X %s\n", (vm->ado_pos1[vm_evt_pos] << 15) + vm->ado_pos0[vm_evt_pos], op_names[op - EVT_ALLEND]);
			}
		}

		switch (code)
		{
		case EVT_RET:
			vm->op_evret();
			break;
		case EVT_ALLEND:
			ret = 1;
			break;
		case EVT_JMP:
			vm->op_jmp();
			break;
		case EVT_CALL:
			vm->op_call();
			break;
		case EVT_EVDEF:
			vm->op_evdef();
			break;
		case EVT_IF:
			vm->queue(EVT_IF);
			break;
		case EVT_WHILE:
			vm->queue(EVT_WHILE);
			break;
		case EVT_NOP:
			vm->op_nop();
			vm->evt_can_stop = 1;
			break;
		case EVT_ENDIF:
			vm->op_endif();
			break;
		case EVT_ENDWHILE:
			vm->op_endwhile();
			break;
		case EVT_ELSE:
			vm->op_else();
			break;
		case EVT_MSGINIT:
			vm->op_msg_init(size);
			break;
		case EVT_MSGTYPE:		// unused
			break;
		case EVT_MSGATTR:
			vm->op_msg_attr(size);
			break;
		case EVT_MSGOUT:
			vm->op_msg_out();
			vm->stop();
			break;
		case EVT_SETMARK:
			vm->op_set_mark();
			break;
		case EVT_SETWAIT:		// unused
			break;
		case EVT_MSG_WAIT:
			vm->op_msg_wait();
			vm->evt_can_stop = 1;
			break;
		case EVT_EVSTART:
			vm->op_evtstart();
			break;
		case EVT_BGFILEDISP:
			vm->op_bg_file_disp();
			vm->stop();
			break;
		case EVT_BGLOAD:
			vm->op_bg_load();
			vm->stop();
			break;
		case EVT_PALLOAD:
			vm->op_pal_load();
			break;
		case EVT_BGMREQ:
			vm->op_bgm_req();
			break;
		case EVT_SPRCLR:
			vm->op_spr_clr();
			vm->stop();
			break;
		case EVT_ABSOBJANIM:
			vm->op_abs_obj_anim();
			vm->stop();
			break;
		case EVT_OBJANIM:
			vm->op_obj_anim();
			vm->stop();
			break;
		case EVT_ALLSPRCLR:
			vm->updateIndex();
			Vm_all_spr_clr();
			vm->evt_can_stop = 1;
			break;
		case EVT_MSGCLR:
			vm->op_msg_clr();
			vm->stop();
			break;
		case EVT_SCREENCLR:
			TMapCache(nullptr, nullptr);
			render_x = 0;
			render_y = 0;
			render_w = 640;
			render_h = 480;
			Vm_func_call(9);
			vm->op_screen_clr();
			vm->stop();
			break;
		case EVT_SCREENON:
			vm->updateIndex();
			Vm_screen_on();
			break;
		case EVT_SCREENOFF:
			vm->updateIndex();
			Vm_screen_off();
			vm->stop();
			break;
		case EVT_SCREENIN:		// unused
		case EVT_SCREENOUT:		// unused
			break;
		case EVT_BGDISP:
			vm->op_bg_disp();
			vm->stop();
			break;
		case EVT_BGANIM:
			vm->op_bg_anim();
			vm->stop();
			break;
		case EVT_BGSCROLL:
			vm->op_bg_scroll();
			vm->stop();
			break;
		case EVT_PALSET:
			vm->op_pal_set();
			vm->stop();
			break;
		case EVT_BGWAIT:
			vm->op_bg_wait();
			vm->evt_can_stop = 1;
			break;
		case EVT_WAIT:
			vm->op_wait();
			vm->evt_can_stop = 1;
			break;
		case EVT_BWAIT:			// unused
			break;
		case EVT_BOXFILL:
			vm->op_box_fill();
			vm->stop();
			break;
		case EVT_BGCLR:
			vm->op_bg_clr();
			vm->stop();
			break;
		case EVT_SETBKCOL:
			vm->op_set_bkcol(size);
			break;
		case EVT_MSGCOL:
			vm->op_set_msgcol(size);
			break;
		case EVT_MSGSPD:
			vm->op_msg_spd(size);
			break;
		case EVT_MAPINIT:
			vm->op_map_init(size);
			break;
		case EVT_MAPLOAD:
			vm->op_map_load();
			break;
		case EVT_MAPDISP:
			vm->op_map_disp();
			vm->stop();
			break;
		case EVT_SPRENT:
			vm->op_spr_ent(0);
			vm->stop();
			break;
		case EVT_SETPROC:
			vm->op_set_proc();
			break;
		case EVT_SCEINIT:
			vm->op_sce_init();
			break;
		case EVT_USERCTRL:
			vm->op_user_ctr();
			break;
		case EVT_MAPATTR:
			vm->op_map_attr();
			break;
		case EVT_MAPPOS:
			vm->op_map_pos();
			break;
		case EVT_SPRPOS:
			vm->op_spr_pos();
			break;
		case EVT_SPRANIM:
			vm->op_spr_anim();
			vm->stop();
			break;
		case EVT_SPRDIR:
			vm->op_spr_dir();
			break;
		case EVT_GAMEINIT:
			vm->op_game_init();
			break;
		case EVT_CONTINIT:
			vm->op_cont_init();
			break;
		case EVT_SCEEND:
			vm->op_sce_end();
			break;
		case EVT_MAPSCROLL:
			vm->op_map_scroll();
			vm->stop();
			break;
		case EVT_SPRLMT:
			vm->op_spr_lmt();
			break;
		case EVT_SPRWALKX:
			vm->op_spr_walkx();
			break;
		case EVT_ALLSPRDISP:
			vm->updateIndex();
			Vm_all_spr_disp();
			vm->evt_can_stop = 1;
			break;
		case EVT_MAPWRT:	// unused
			vm->op_map_wrt();
			vm->stop();
			break;
		case EVT_SPRWAIT:
			vm->op_spr_wait();
			vm->evt_can_stop = 1;
			break;
		case EVT_SEREQ:
			vm->op_se_req();
			break;
		case EVT_SNDSTOP:
			vm->updateIndex();
			Midi_unload();
			Sound_stop("ALL");
			break;
		case EVT_SESTOP:
			vm->op_se_stop();
			break;
		case EVT_BGMSTOP:
			vm->updateIndex();
			Midi_unload();
			break;
		case EVT_DOORNOSET:
			vm->updateIndex();
			Vm_door_no_set();
			break;
		case EVT_RAND:
			vm->op_rand();
			break;
		case EVT_BTWAIT:
			vm->op_btwait();
			vm->evt_can_stop = 1;
			break;
		case EVT_FAWAIT:
			vm->op_fawait();
			vm->evt_can_stop = 1;
			break;
		case EVT_SCLBLOCK:
			vm->op_scl_block();
			break;
		case EVT_EVSTOP:
			vm->updateIndex();
			vm->ado_pos_bk[0][vm_evt_pos] = vm->ado_pos0[vm_evt_pos];
			vm->ado_pos_bk[1][vm_evt_pos] = vm->ado_pos1[vm_evt_pos];
			vm->op_sce_reset();
			break;
		case EVT_SEREQPV:
			vm->op_se_req_pv();
			break;
		case EVT_SEREQSPR:
			vm->op_se_req_spr();
			break;
		case EVT_SCERESET:
			vm->updateIndex();
			vm->ado_pos_bk[2][vm_evt_pos] = 0;
			vm->ado_pos_bk[0][vm_evt_pos] = vm->ado_pos0[vm_evt_pos];
			vm->ado_pos_bk[1][vm_evt_pos] = vm->ado_pos1[vm_evt_pos];
			vm->field_4[vm_evt_pos] = 0;
			vm->op_sce_reset();
			break;
		case EVT_BGSPRENT:
			vm->op_bg_spr_ent(0);
			vm->stop();
			break;
		case EVT_BGSPRPOS:
			vm->op_bg_spr_pos();
			vm->stop();
			break;
		case EVT_BGSPRSET:
			vm->op_bg_spr_set();
			vm->stop();
			break;
		case EVT_SLANTSET:
			vm->op_slant_set();
			break;
		case EVT_SLANTCLR:
			vm->op_slant_clr();
			break;
		case EVT_DUMMY:			// unused
			break;
		case EVT_SPCFUNC:
			vm->op_spc_func();
			vm->stop();
			break;
		case EVT_SEPAN:
			vm->op_se_pan();
			break;
		case EVT_SEVOL:
			vm->op_se_vol();
			break;
		case EVT_BGDISPTRN:
			vm->op_bg_disp_trn();
			vm->stop();
			break;
		case EVT_DEBUG:
			vm->updateIndex();
			vm_index4[59] = vm->read16();
			break;
		case EVT_TRACE:
			vm->updateIndex();
			vm_index4[58] = vm->read16();
			break;
		case EVT_TMWAIT:
			vm->op_tmwait();
			vm->evt_can_stop = 1;
			break;
		case EVT_BGSPRANIM:
			vm->op_bg_spr_anim();
			vm->stop();
			break;
		case EVT_ABSSPRENT:
			vm->op_spr_ent(1);
			vm->stop();
			break;
		case EVT_NEXTCOM:
			vm->updateIndex();
			vm->field_4[vm_evt_pos] = vm->read16();
			break;
		case EVT_WORKCLR:
			vm->updateIndex();
			vm->field_4[vm_evt_pos] = 0;
			Vm_work_clr();
			break;
		case EVT_BGBUFCLR:
			vm->op_bg_buf_clr();
			break;
		case EVT_ABSBGSPRENT:
			vm->op_bg_spr_ent(1);
			vm->stop();
			break;
		case EVT_AVIPLAY:
			vm->op_avi_play();
			break;
		case EVT_AVISTOP:
			vm->updateIndex();
			//avi_stop();
			break;
		case EVT_SPRMARK:		// unused on pc
			vm->op_spr_mark();
			break;
		case EVT_BGMATTR:		// unused on pc
			vm->op_bgm_attr();
			break;
		case EVT_BAD_OPC:
			ret = 1;
			break;
		default:	
			vm->op_default(op);
		}
	} while (!vm->evt_can_stop);

	return ret;
}

void Vm_update_ent_pan(VM* vm)
{
	if (vm->sample_usage[vm->sample_index] != 0xFF)
		Sound_set_pan(vm->sample_names[vm->sample_index], EntGetPan(vm->sample_usage[vm->sample_index]));

	if (++vm->sample_index >= 4)
		vm->sample_index = 0;
}

void Vm_update(VM* vm, int mode)
{
	Vm_update_ent_pan(vm);
	for (int i = 0; i < 5; ++i)
	{
		if (vm_usage[i] == 1)
		{
			vm_evt_pos = i;
			vm->reset_pos();
			Vm_mes_print(vm);
			Game_BgDispTrn_1(vm);
			Game_RedrawAll(vm);
			Vm_event(vm);
			if (!mode)
				Vm_execute(vm);
		}
	}
	vm_evt_pos = 0;
	sub_41259E();
}
