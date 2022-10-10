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

#include "palette.h"
#include "program.h"
#include <poppack.h>

//EXTERN_C VM* vm;

extern RENDER_BMP render_bmp;
