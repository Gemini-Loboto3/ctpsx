#include <stdafx.h>
#define DIRECTINPUT_VERSION 0x800
#include <dinput.h>
#include "game.h"

LPDIRECTINPUT8 di8;
LPDIRECTINPUTDEVICE8 di_mouse;

int InputInit()
{
	if (FAILED(DirectInput8Create(prog.hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&di8, nullptr)))
		return 0;

	if (FAILED(di8->CreateDevice(GUID_SysMouse, &di_mouse, nullptr)))
		return 0;
	/*if (FAILED(*/di_mouse->SetCooperativeLevel(prog.hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)/*))*/
		/*return 0*/;

	DIDATAFORMAT DIDataFormat;
	memcpy(&DIDataFormat, &c_dfDIMouse2, sizeof(DIDataFormat));
	DIDataFormat.dwFlags = DIDF_ABSAXIS;
	if (FAILED(di_mouse->SetDataFormat(&DIDataFormat)))
		return 0;

	return 1;
}

void InputRead()
{
	DIMOUSESTATE2 state;
	if (SUCCEEDED(di_mouse->Acquire()) && SUCCEEDED(di_mouse->GetDeviceState(sizeof(DIMOUSESTATE2), &state)))
	{
		if (state.rgbButtons[0])
			LBtnClick(0, state.lX, state.lY);
		else prog.click_bits &= ~1;

		if (state.rgbButtons[1])
			RBtnClick(0, state.lX, state.lY);
		else prog.click_bits &= ~2;
	}
}
