#include "DXApp.h"

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
	: m_cam(&input)
{
	m_h_app_instance = h_instance;
	m_h_app_wnd = NULL;
	m_app_title = "3D Collisions";
	m_wnd_style = WS_OVERLAPPEDWINDOW;
	gp_app = this;

	m_dev = nullptr;
	m_dev_con = nullptr;
	m_swap_chain = nullptr;
	m_render_target_view = nullptr;
}


DXApp::~DXApp()
{
	if (m_dev_con)
	{
		m_dev_con->ClearState();
	}
	Memory::SafeRelease(m_render_target_view);
	Memory::SafeRelease(m_swap_chain);
	Memory::SafeRelease(m_dev_con);
	Memory::SafeRelease(m_dev);
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
			update(dt);
			render(dt);
		}
	}
	return static_cast<int>(msg.wParam);
}

bool DXApp::init()
{
	if (!initWindow())
	{
		return false;
	}
	if (!initDirect3D())
	{
		return false;
	}

	return true;
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

bool DXApp::initDirect3D()
{
	UINT create_device_flags = 0;
#ifdef _DEBUG
	create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif	//_DEBUG
	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	UINT num_driver_types = ARRAYSIZE(driver_types);

	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
	};

	UINT num_feature_levels = ARRAYSIZE(feature_levels);

	DXGI_SWAP_CHAIN_DESC swap_desc;
	ZeroMemory(&swap_desc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swap_desc.BufferCount = 1;
	swap_desc.BufferDesc.Width = m_client_width;
	swap_desc.BufferDesc.Height = m_client_height;
	swap_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_desc.Windowed = true;
	swap_desc.SampleDesc.Count = 4;
	swap_desc.SampleDesc.Quality = 0;
	swap_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swap_desc.OutputWindow = m_h_app_wnd;

	HRESULT result;
	for (int i = 0; i < num_driver_types; ++i)
	{
		result = D3D11CreateDeviceAndSwapChain(NULL, driver_types[i], NULL,
			create_device_flags, feature_levels, num_feature_levels,
			D3D11_SDK_VERSION, &swap_desc, &m_swap_chain, &m_dev,
			&m_feature_level, &m_dev_con);

		if (SUCCEEDED(result))
		{
			m_driver_type = driver_types[i];
			break;
		}
	}

	if (FAILED(result))
	{
		OutputDebugString("\nFailed to create device and swap chain\n");
		return false;
	}

	ID3D11Texture2D* mp_back_buffer_txt = 0;
	m_swap_chain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&mp_back_buffer_txt));
	m_dev->CreateRenderTargetView(mp_back_buffer_txt, nullptr, &m_render_target_view);

	m_dev_con->OMSetRenderTargets(1, &m_render_target_view, nullptr);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = m_client_width;
	viewport.Height = m_client_height;

	m_dev_con->RSSetViewports(1, &viewport);

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
	switch (msg)
	{
	case WM_DESTROY:
		return quitApp();
	case WM_KEYDOWN:
		if (w_param == (int)KeyBind::Esc)
		{
			return quitApp();
		}
		input.keyDown((KeyBind)w_param);
		break;
	case WM_KEYUP:
		input.keyUp((KeyBind)w_param);
		break;
	case WM_LBUTTONDOWN:	//click
	case WM_MOUSEMOVE:		//move
	default:
		break;
	}
	return DefWindowProc(hwnd, msg, w_param, l_param);
}

ID3D11Device * DXApp::getDevice()
{
	return m_dev;
}

ID3D11DeviceContext * DXApp::getContext()
{
	return m_dev_con;
}

float DXApp::getRatio()
{
	return (float)m_client_width / m_client_height;
}

void DXApp::setColour(int colour_index)
{
	m_colour[colour_index] = m_colour[colour_index] >= 1 ? 0 : m_colour[colour_index] + 0.1f;
}

Camera * DXApp::getCam()
{
	return &m_cam;
}