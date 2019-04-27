#include "Input.h"

Input::~Input()
{
	DIMouse->Unacquire();
	DIKeyboard->Unacquire();
	if (direct_input)
	{
		direct_input->Release();
		direct_input = nullptr;
	}
}

bool Input::init(HINSTANCE h_instance, HWND hwnd)
{
	HRESULT hr = DirectInput8Create(h_instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&direct_input, NULL);
	if (hr != S_OK)
		return false;
	hr = direct_input->CreateDevice(GUID_SysKeyboard, &DIKeyboard, NULL);
	if (hr != S_OK)
		return false;
	hr = direct_input->CreateDevice(GUID_SysMouse, &DIMouse, NULL);
	if (hr != S_OK)
		return false;
	hr = DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (hr != S_OK)
		return false;
	hr = DIKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (hr != S_OK)
		return false;
	hr = DIMouse->SetDataFormat(&c_dfDIMouse);
	if (hr != S_OK)
		return false;
	hr = DIMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
	if (hr != S_OK)
		return false;
	return true;
}

void Input::detectInput()
{
	getMouseInput();
	getKeyInput();
}

bool Input::key(int key, InputState state)
{
	switch (state)
	{
	case DOWN:
		return ((keyboard_state[key] & 0x80) && !(keyboard_last_state[key] & 0x80));
	case HELD:
		return (keyboard_state[key] & 0x80);
	case UP:
		return (!(keyboard_state[key] & 0x80) && (keyboard_last_state[key] & 0x80));
	default:
		return false;
	}
}

bool Input::click(int mouse_button, InputState state)
{
	switch (state)
	{
	case DOWN:
		return(mouse_current_state.rgbButtons[mouse_button] & 0x80 && !(mouse_last_state.rgbButtons[mouse_button] & 0x80));
	case HELD:
		return(mouse_current_state.rgbButtons[mouse_button] & 0x80);
	case UP:
		return(!(mouse_current_state.rgbButtons[mouse_button] & 0x80) && (mouse_last_state.rgbButtons[mouse_button] & 0x80));
	default:
		return false;
	}
}

Vector2 Input::mouseMove()
{
	int x = mouse_current_state.lX - mouse_last_state.lX;
	int y = mouse_current_state.lY - mouse_last_state.lY;
	
	return Vector2(x, y);
}

void Input::getMouseInput()
{
	mouse_last_state = mouse_current_state;
	DIMouse->Acquire();
	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouse_current_state);
}

void Input::getKeyInput()
{
	for (int i = 0; i < 256; i++)
	{
		keyboard_last_state[i] = keyboard_state[i];
	}
	DIKeyboard->Acquire();
	DIKeyboard->GetDeviceState(sizeof(keyboard_state), (LPVOID)&keyboard_state);
}
