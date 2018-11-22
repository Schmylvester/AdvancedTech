#include "Window.h"

namespace
{
	Window* global_wnd = nullptr;
}

LRESULT CALLBACK MainWindProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
	if (global_wnd)
	{
		return global_wnd->wndProc(hwnd, msg, w_param, l_param);
	}
	return DefWindowProc(hwnd, msg, w_param, l_param);
}

void errorBox(LPCSTR message)
{
	MessageBox(NULL, message, "Error", MB_OK | MB_ICONERROR);
}

Window::Window()
{
	global_wnd = this;
}

bool Window::init(HINSTANCE h_instance, int show_wnd, int width, int height, bool windowed)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWindProc;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = h_instance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_wnd_class_name;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		errorBox("Error registering class");
		return false;
	}

	UINT x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
	UINT y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;
	m_h_app_wnd = CreateWindowEx(NULL, m_wnd_class_name, "Thomas Sylvester deserves a first", WS_OVERLAPPEDWINDOW, x, y, width, height, NULL, NULL, h_instance, NULL);
	if (!m_h_app_wnd)
	{
		errorBox("Error creating window");
		return false;
	}
	return true;
}

LRESULT Window::wndProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
	int mouse_x = LOWORD(l_param);
	int mouse_y = HIWORD(l_param);
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		if (w_param == 27)
		{
			PostQuitMessage(0);
			return 0;
		}
		break;
	case WM_KEYUP:
		break;
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, msg, w_param, l_param);
}

void Window::runWindow(int n_cmd_show)
{
	ShowWindow(m_h_app_wnd, n_cmd_show);
	UpdateWindow(m_h_app_wnd);
}