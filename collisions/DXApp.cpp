#include "DXApp.h"
#include <DirectXColors.h>

namespace
{
	DXApp* gp_app = nullptr;
}

LRESULT CALLBACK MainWindProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
	if (gp_app)
	{
		return gp_app->msgProc(hwnd, msg, w_param, l_param);
	}
	return DefWindowProc(hwnd, msg, w_param, l_param);
}

DXApp::DXApp(HINSTANCE h_instance)
{
	m_h_app_instance = h_instance;
	m_h_app_wnd = NULL;
	m_app_title = "UWE Bad Movies society is best society.";
	m_wnd_style = WS_OVERLAPPEDWINDOW;
	gp_app = this;
}

DXApp::~DXApp()
{

}

int DXApp::run()
{
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			float dt = getDeltaTime();
			updateScene();
			drawScene();
		}
	}
	return static_cast<int>(msg.wParam);
}

bool DXApp::init(HINSTANCE h_instance)
{
	if (!initWindow())
	{
		return false;
	}
	if (!initDirectX3D(h_instance))
	{
		MessageBox(0, "Direct3D Initialisation - Failed", "Error", MB_OK);
		return false;
	}
	m_cam = std::make_unique<Camera>(&input, this);
	triangle_loader = std::make_unique<TriangleLoader>(this, m_cam.get());

	return true;
}

bool DXApp::initDirectX3D(HINSTANCE h_instance)
{
	HRESULT hr;

	DXGI_MODE_DESC buffer_desc;
	ZeroMemory(&buffer_desc, sizeof(DXGI_MODE_DESC));

	buffer_desc.Width = m_client_width;
	buffer_desc.Height = m_client_height;
	buffer_desc.RefreshRate.Numerator = 60;
	buffer_desc.RefreshRate.Denominator = 1;
	buffer_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	buffer_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	buffer_desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	ZeroMemory(&swap_chain_desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swap_chain_desc.BufferDesc = buffer_desc;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.BufferCount = 1;
	swap_chain_desc.OutputWindow = m_h_app_wnd;
	swap_chain_desc.Windowed = TRUE;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION,
		&swap_chain_desc, &m_swap_chain, &m_device, NULL, &m_device_context);

	ID3D11Texture2D* back_buffer;
	hr = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer);

	hr = m_device->CreateRenderTargetView(back_buffer, NULL, &m_rt_view);
	Memory::SafeRelease(back_buffer);

	m_device_context->OMSetRenderTargets(1, &m_rt_view, NULL);

	return true;
}

void DXApp::releaseObjects()
{

}

void DXApp::updateScene()
{
	red += colourmodr * 0.00005f;
	green += colourmodg * 0.00002f;
	blue += colourmodb * 0.00001f;

	if (red > 1 || red < 0)
	{
		colourmodr *= -1;
	}
	if (green > 1 || green < 0)
	{
		colourmodg *= -1;
	}
	if (blue > 1 || blue < 0)
	{
		colourmodb *= -1;
	}
}

void DXApp::drawScene()
{
	float bg_colour[4]{ red, green, blue, 1.0f };
	m_device_context->ClearRenderTargetView(m_rt_view, bg_colour);
	m_swap_chain->Present(0, 0);
}

bool DXApp::initWindow()
{
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hInstance = m_h_app_instance;
	wcex.lpfnWndProc = MainWindProc;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "DXAPPWNCLASS";
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		OutputDebugString("\nFailed to create window class\n");
		return false;
	}

	RECT r = { 0, 0, m_client_width, m_client_height };
	AdjustWindowRect(&r, m_wnd_style, FALSE);
	UINT width = r.right - r.left;
	UINT height = r.bottom - r.top;

	UINT x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
	UINT y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

	m_h_app_wnd = CreateWindow("DXAPPWNCLASS", m_app_title.c_str(),
		m_wnd_style, x, y, width, height, NULL, NULL, m_h_app_instance, NULL);

	if (!m_h_app_wnd)
	{
		OutputDebugString("\nFailed to create window instance\n");
		return false;
	}

	ShowWindow(m_h_app_wnd, SW_SHOW);

	return true;
}

int DXApp::quitApp()
{
	PostQuitMessage(0);
	return 0;
}

float DXApp::getDeltaTime()
{
	float f = float(clock() - last_clock) / CLOCKS_PER_SEC;
	fps = 1 / f;
	last_clock = clock();
	return f;
}

LRESULT DXApp::msgProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
	int mouse_x = LOWORD(l_param);
	int mouse_y = HIWORD(l_param);
	switch (msg)
	{
	case WM_DESTROY:
		return quitApp();
	case WM_KEYDOWN:		//key
		if (w_param == (int)KeyBind::Esc)
		{
			return quitApp();
		}
		input.keyboard.keyDown((KeyBind)w_param);
		break;
	case WM_KEYUP:
		input.keyboard.keyUp((KeyBind)w_param);
		break;
	case WM_LBUTTONDOWN:	//click
	case WM_MOUSEMOVE:
		input.mouse.move(mouse_x, mouse_y);
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, msg, w_param, l_param);
}