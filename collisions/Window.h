#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "DXUtil.h"
#include "Input.h"

class Window
{
public:
	Window();
	~Window() = default;

	bool init(HINSTANCE h_instance, int show_wnd, int width, int height, bool windowed);
	HWND* getAppWnd() { return &m_h_app_wnd; }
	LRESULT wndProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);
	void runWindow(int n_cmd_show);

private:
	HWND m_h_app_wnd;
	LPCTSTR m_wnd_class_name = "Window Class";

};