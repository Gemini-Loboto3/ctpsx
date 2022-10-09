#pragma once

#define OP_NOP		0x90

template<typename OUTSTUFF, typename INSTUFF>
OUTSTUFF ForceCast(INSTUFF in)
{
	union
	{
		INSTUFF  in;
		OUTSTUFF out;
	}
	u = { in };

	return u.out;
};

#include <pshpack1.h>
typedef struct {
	BYTE opCode;	// must be 0xE9;
	DWORD offset;	// jump offset
} JMP;

typedef struct
{
	BYTE opCode0;	// must be 0xE8
	DWORD offset0;	// call offset
	BYTE opCode1;	// must be 0xE9
	DWORD offset1;	// reroute offset
} CALLX;
#include <poppack.h>

#define INJECT(from,to) { \
	((JMP*)(from))->opCode = 0xE9; \
	((JMP*)(from))->offset = (DWORD)(to) - ((DWORD)(from) + sizeof(JMP)); \
}

#define INJECT_EXT(from,to) (*(DWORD*)(from)) = (DWORD)(to)

#define INJECT_CALL(from,to,size)	{\
	if(size > 5) \
		memset(((void*)from), 0x90, size); \
	((JMP*)(from))->opCode = 0xE8; \
	((JMP*)(from))->offset = (DWORD)(to) - ((DWORD)((void*)from) + sizeof(JMP)); \
}

#define INJECT_CALLX(from,fnc,skp)	{\
	((CALLX*)(from))->opCode0 = 0xE8; \
	((CALLX*)(from))->offset0 = (DWORD)(fnc) - ((DWORD)((void*)from) + sizeof(JMP)); \
	((CALLX*)(from))->opCode1 = 0xE9; \
	((CALLX*)(from))->offset1 = (DWORD)(skp) - ((DWORD)((void*)from) + sizeof(CALLX)); \
}\

void MessageBoxV(UINT icon, char *caption, char *message, ...);

enum InjectType
{
	IT_JUMP,
	IT_CALL,
	IT_EXTERN,
	IT_NOP
};
