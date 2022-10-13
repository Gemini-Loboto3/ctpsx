#include <stdafx.h>
#define DIRECTINPUT_VERSION 0x800
#include <dinput.h>
#include "game.h"
#include <algorithm>

LPDIRECTINPUT8 di8;
LPDIRECTINPUTDEVICE8 di_mouse;

#define CAPTURE_MOUSE	1

int mouseX = 0,
	mouseY = 0;

int InputInit()
{
	if (FAILED(DirectInput8Create(prog.hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&di8, nullptr)))
		return 0;

	if (FAILED(di8->CreateDevice(GUID_SysMouse, &di_mouse, nullptr)))
		return 0;
#if CAPTURE_MOUSE
	di_mouse->SetCooperativeLevel(prog.hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
#else
	di_mouse->SetCooperativeLevel(prog.hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
#endif

#if 0
	DIDATAFORMAT DIDataFormat;
	memcpy(&DIDataFormat, &c_dfDIMouse2, sizeof(DIDataFormat));
	DIDataFormat.dwFlags = DIDF_ABSAXIS;
	if (FAILED(di_mouse->SetDataFormat(&DIDataFormat)))
		return 0;
#else
	if (FAILED(di_mouse->SetDataFormat(&c_dfDIMouse2)))
		return 0;
#endif

	return 1;
}

void InputRead()
{
	DIMOUSESTATE2 state;
	if (SUCCEEDED(di_mouse->Acquire()) && SUCCEEDED(di_mouse->GetDeviceState(sizeof(DIMOUSESTATE2), &state)))
	{
		mouseX += state.lX;
		mouseY += state.lY;

		mouseX = std::clamp(mouseX, 0, 512);
		mouseY = std::clamp(mouseY, 0, 480);

		if (state.rgbButtons[0])
			LBtnClick(0, mouseX, mouseY);
		else prog.click_bits &= ~1;

		if (state.rgbButtons[1])
			RBtnClick(0, mouseX, mouseY);
		else prog.click_bits &= ~2;

		char mes[32];
		sprintf_s(mes, 32, "X %d Y %d\n", mouseX, mouseY);
		OutputDebugStringA(mes);
	}
}
