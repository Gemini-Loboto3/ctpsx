#include <stdafx.h>
#define DIRECTINPUT_VERSION 0x800
#include <dinput.h>
#include "game.h"
#include <algorithm>

LPDIRECTINPUT8 di8;
LPDIRECTINPUTDEVICE8 di_mouse,
	di_kb;

#define CAPTURE_MOUSE	1

int mouseX = 0,
	mouseY = 0;

static int InitMouse()
{
	if (FAILED(di8->CreateDevice(GUID_SysMouse, &di_mouse, nullptr)))
		return 0;
	if (FAILED(di_mouse->SetDataFormat(&c_dfDIMouse2)))
		return 0;

#if CAPTURE_MOUSE
	di_mouse->SetCooperativeLevel(prog.hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
#else
	di_mouse->SetCooperativeLevel(prog.hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
#endif

	return 1;
}

static int InitKeyboard()
{
	if (FAILED(di8->CreateDevice(GUID_SysKeyboard, &di_kb, nullptr)))
		return 0;
	if (FAILED(di_kb->SetDataFormat(&c_dfDIKeyboard)))
		return 0;

	//di_kb->SetCooperativeLevel(prog.hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);

	return 1;
}

int InputInit()
{
	if (FAILED(DirectInput8Create(prog.hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&di8, nullptr)))
		return 0;

	if (!InitMouse())
		return 0;

	if (!InitKeyboard())
		return 0;

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
			RBtnClick(mouseX, mouseY);
		else prog.click_bits &= ~2;
	}

	char kb_state[256];
	if (SUCCEEDED(di_kb->Acquire()) && SUCCEEDED(di_kb->GetDeviceState(sizeof(kb_state), (LPVOID)kb_state)))
	{
		int event = -1;
		// new game
		if (kb_state[DIK_F8]) event = 0;
		// fast start
		else if (kb_state[DIK_F9]) event = 1;
		// continue
		else if (kb_state[DIK_F10]) event = 2;
		// ending list
		else if (kb_state[DIK_F11]) event = 3;
		// shutdown (ALT+F4)
		else if (kb_state[DIK_F4] && kb_state[DIK_LALT])
			SendMessageW(prog.hWnd, WM_CLOSE, 0, 0);

		if (event != -1)
		{
			vm_data.vm_index3[16] = event;
			Vm_set_63();
			Vm_mark_event(0x190, 1);
		}

		bool left = kb_state[DIK_A] ? true : false;
		bool right = kb_state[DIK_D] ? true : false;
		bool down = kb_state[DIK_S] ? true : false;
		bool up = kb_state[DIK_W] ? true : false;
		bool run_left = kb_state[DIK_Q] ? true : false;
		bool run_right = kb_state[DIK_E] ? true : false;
		bool rest = kb_state[DIK_LCONTROL] ? true : false;
		int fast = kb_state[DIK_LSHIFT] ? 2 : 1;
		bool conf = kb_state[DIK_SPACE] ? true : false;

		const int mspd = 8;	// cursor speed

		if (left) mouseX -= mspd * fast;
		else if (right) mouseX += mspd * fast;
		mouseX = std::clamp(mouseX, 0, 512);

		if (up) mouseY -= mspd * fast;
		else if (down) mouseY += mspd * fast;
		mouseY = std::clamp(mouseY, 0, 480);

		if (conf)
			LBtnClick(fast ? 1 : 0, mouseX, mouseY);
		if (rest)
			RBtnClick(mouseX, mouseY);
		if (run_left)
			LBtnClick(1, 0, 0);
		if (run_right)
			LBtnClick(1, 640, 0);
	}
}
