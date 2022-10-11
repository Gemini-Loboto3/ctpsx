#define _CRT_SECURE_NO_WARNINGS

#include <stdafx.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include "console.h"

void console_print(const char* fmt, ...)
{
#if 0
	va_list ap;
	char buffer[1024];

	va_start(ap, fmt);
	vsprintf(buffer, fmt, ap);
	printf(buffer);
	va_end(ap);
#else
	va_list args;
	va_start(args, fmt);
	vfprintf(stdout, fmt, args);
	va_end(args);
#endif
}

#ifdef USE_CONSOLE
void _AllocConsole()
{
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("CON", "w", stdout);
}

void _FreeConsole()
{
	FreeConsole();
}
#endif
