#pragma once

#include "api.h"
#include "vm.h"
#include "unk.h"
#include "tim.h"
#include "tmc.h"
#include "dib.h"
#include "spr_ent.h"
#include "tmap.h"
#include "spr_ent.h"
#ifdef _WINDOWS
#include "win\render_d3d9.h"
#include "win\input_di8.h"
#include "win\FileHandle.h"
#endif
#include "bg.h"
#include "item.h"
#include "sound.h"
#include "font.h"
#include "text.h"
#include "sprt_man.h"

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
	CRect rect;
	int screen_on;
	BITMAP_DATA* bmp_info;
	int flipped_h;
};

#include "palette.h"
#include "program.h"
#include <poppack.h>

//EXTERN_C VM* vm;
