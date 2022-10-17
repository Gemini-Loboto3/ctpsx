#include <stdafx.h>
#include "game.h"
#include "vm.h"
#include "unk.h"
#include "resource.h"
#include "console.h"
#include "timer.h"

ULONG_PTR EnableVisualStyles()
{
	TCHAR dir[MAX_PATH];
	ULONG_PTR ulpActivationCookie = FALSE;
	ACTCTX actCtx =
	{
		sizeof(actCtx),
		ACTCTX_FLAG_RESOURCE_NAME_VALID
		| ACTCTX_FLAG_SET_PROCESS_DEFAULT
		| ACTCTX_FLAG_ASSEMBLY_DIRECTORY_VALID,
		TEXT("shell32.dll"), 0, 0, dir, (LPCTSTR)124
	};
	UINT cch = GetSystemDirectory(dir, sizeof(dir) / sizeof(*dir));
	if (cch >= sizeof(dir) / sizeof(*dir))
		return FALSE;
	dir[cch] = TEXT('\0');
	ActivateActCtx(CreateActCtx(&actCtx), &ulpActivationCookie);
	return ulpActivationCookie;
}

PROGRAM prog;
FrameLimiter fps;

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_DESTROY:
		DestroyWindow(hWnd);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_F1:
			fps.PrevSpeed();
			return 1;
		case VK_F2:
			fps.NextSpeed();
			return 1;
		}
	}

	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

void RegisterWndClass()
{
	WNDCLASSW cls = { 0 };

	cls.lpszClassName = prog.mCaption;
	cls.hInstance = prog.hInst;
	cls.style = 0xB;
	//cls.hCursor = LoadCursorA(prog.hInst, (LPCSTR)0xC8);
	cls.hIcon = LoadIconW(prog.hInst, MAKEINTRESOURCEW(IDI_ICON1));
	cls.lpszMenuName = L"Menu";
	cls.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	cls.lpfnWndProc = WndProc;
	RegisterClassW(&cls);
}

void Init()
{
	//Vm_set_snd_mode(0);
	Vm_set_proc(0);
	//BmpInitRect(&prog.render_bmp, 80, 0, 480, 304);
	vm_index5[0] = 0;
	//sprt_dat[0].type4 = 0;
	//sprt_dat[1].type4 = 0;

#ifdef NDEBUG
	char path[MAX_PATH];
	GetModuleFileNameA(GetModuleHandleA(nullptr), path, sizeof(path));
	memcpy(prog.path, path, strrchr(path, '\\') - path + 1);
#endif

}

void Midi_init();

void InitApp()
{
	error_no = 0;
	Init();

	//InitAbm();
	Vm_screen_off();

	prog.vm = new VM();
	Vm_load(prog.vm);
	//sub_405DDF();

	InitRender();
	InputInit();

	InitItem();
	InitFont();
	LoadFaces();

	Midi_init();
}

void InitInstance()
{
	RegisterWndClass();

	prog.hWnd = CreateWindowExW(0, prog.mCaption, prog.tCaption, 0xA0000, 0, 0, 640, 480, nullptr, nullptr, prog.hInst, nullptr);
	ShowWindow(prog.hWnd, SW_SHOW);
	UpdateWindow(prog.hWnd);

	InitApp();
}

// human logo
void draw_func0()
{
	SwapBuffer();
	sub_403252();
}

void DispCursor();

// game?
void draw_func1()
{
	sub_40266A();
	TMap_scroller();
	TMap_scroller2();
	sub_401D32();
	sub_401D74();
	sub_4021DC();
	sub_40243A();
	sub_40245E();
	int pal = sub_4035DC();
	prog.pal_obj.f4091AD(pal);
	DrawQSprites();
	DispCursor();
	SwapBuffer();
	sub_403252();
}

// opening / title screen
void draw_func2()
{
	sub_40245E();
	DrawQSprites();
	SwapBuffer();
	sub_403252();
}

void (*draw_func[])()
{
	draw_func0,
	draw_func1,
	draw_func2
};

DWORD start;

void startTime()
{
	start = (DWORD)GetTickCount64();
}

DWORD getTime()
{
	return ((DWORD)GetTickCount64() - start);
}

int Game()
{
	if (prog.field_120 == 0)
	{
		auto tick = (DWORD)getTime();

		if (!fps.Sync(2)) return 0;

		//if (tick >= prog.tick_now)
		{
			prog.tick_now = tick + prog.tick_delta;
			if (prog.vm_func != 1)
				prog.field_128 = 0;
			InputRead();
			Vm_update(prog.vm, prog.field_128);
			sub_4017BB();
			draw_func[prog.vm_func]();
			prog.pal_obj.call_fade();
			sub_4017D6();
		}
	}

	if (error_no)
	{
		SendMessageW(prog.hWnd, WM_DESTROY, 0, 0);
		return 1;
	}

	return 0;
}

void WinLoop()
{
	MSG Msg;
	fps.Init();

	while (1)
	{
		while (!PeekMessageW(&Msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (Game())
				WaitMessage();
		}

		if (Msg.message == WM_QUIT)
			break;
		TranslateMessage(&Msg);
		DispatchMessageW(&Msg);
	}
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	EnableVisualStyles();

	ENABLE_CONSOLE;

	prog.hInst = hInstance;
	prog.CmdLine = lpCmdLine;
	prog.mCaption = (LPWSTR)L"Clock Tower";
	prog.tCaption = (LPWSTR)L"Clock Tower: The First Fear";

	InitInstance();

	Tmc tm;
	tm.open("SPRT\\CHARTMC2\\A_ACT01.TMC");

	vm_index4[58] = 1;

	startTime();
	WinLoop();

	delete prog.vm;

	DISABLE_CONSOLE;

	return 1;
}
