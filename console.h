#pragma once

#ifdef _DEBUG
#define USE_CONSOLE
#endif

#ifdef USE_CONSOLE
#define DEBUGPRINT(x)		printf x
#define ENABLE_CONSOLE		_AllocConsole();
#define DISABLE_CONSOLE		_FreeConsole();

void _AllocConsole();
void _FreeConsole();

#else
#define DEBUGPRINT(x)
#define ENABLE_CONSOLE
#define DISABLE_CONSOLE
#endif
