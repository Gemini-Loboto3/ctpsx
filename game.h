#pragma once

#include "vm.h"
#include "unk.h"
#include "tim.h"
#include "dib.h"
#include "spr_ent.h"
#include "tmap.h"
#include "spr_ent.h"
#ifdef _WINDOWS
#include "render_d3d9.h"
#include "input_di8.h"
#endif
#include "bg.h"
#include "item.h"
#include "sound.h"
#include "font.h"
#include "text.h"

DWORD getTime();

void Midi_load(const char* filename);
void Midi_unload();

#include <pshpack1.h>
struct RENDER_PIC
{
	BYTE b[480][640];
};

struct BITMAP_DATA
{
	BITMAPINFOHEADER info;
	BYTE pal[256][4];
};

struct RENDER_BMP
{
	int used;
	HGDIOBJ old_obj;
	int w;
	int h;
	RECT rect;
	int screen_on;
	BITMAP_DATA* bmp_info;
	RENDER_PIC* ppvbits;
	int flipped_h;
};


struct PAL_LOG256
{
	__int16 palVersion;
	__int16 palNumEntries;
	PALETTEENTRY palPalEntry[256];
};

struct PAL_OBJ
{
	struct PAL_OBJ_VTBL
	{
		int(__cdecl* fnc0)(PAL_OBJ*);
		int(__cdecl* fnc1)(PAL_OBJ*);
	} *vtbl;
	int field_4;
	HPALETTE hPal;
	LPCVOID buf;
	BITMAP_DATA* bmp_info;
	int type;
	int nindex;
	int ncount;
	int fade_delta;
	int fade;
	PAL_LOG256 pal;
	int index;
	int count;
	int id;
	PAL_LOG256 pal_aux[3];
	__int16 field_1044;
	BYTE pal16[2][16];
	__int16 field_1066;
	int field_1068;
	int field_106C;
	int field_1070;
	__int16 field_1074;
};

#include "program.h"
#include <poppack.h>

//EXTERN_C VM* vm;

extern PAL_OBJ pal_obj;
extern RENDER_BMP render_bmp;
