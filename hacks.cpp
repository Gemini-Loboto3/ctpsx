#include <stdafx.h>
#include "inject.h"

void MakePageWritable(unsigned long ulAddress, unsigned long ulSize);

int __cdecl CreateOffScreenBitmap(void* bmp, int w, int h)
{
	return 1;
}

void Inject_game();

void Inject_hacks()
{
	MakePageWritable(0x401000, 0x4274C0 - 0x401000);

	//Inject_game();

	//INJECT(0x40958D, CreateOffScreenBitmap);
}
