#pragma once
#include "DXUtil.h"
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <dinput.h>

enum InputState
{
	DOWN,
	HELD,
	UP
};

class Input
{
public:
	Input() = default;
	~Input();

	bool init(HINSTANCE h_instance, HWND hwnd);
	void detectInput();

	bool key(int key, InputState state);
	bool click(int mouse_button, InputState state);
	Vector2 mouseMove();

private:
	void getMouseInput();
	void getKeyInput();

	IDirectInputDevice8 * DIKeyboard;
	IDirectInputDevice8* DIMouse;
	
	DIMOUSESTATE mouse_current_state;
	DIMOUSESTATE mouse_last_state;

	BYTE keyboard_state[256];
	BYTE keyboard_last_state[256];

	LPDIRECTINPUT8 direct_input;
};

