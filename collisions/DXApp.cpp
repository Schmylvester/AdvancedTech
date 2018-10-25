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
{
	m_h_app_instance = h_instance;
	m_h_app_wnd = NULL;
	m_app_title = "UWE Bad Movies society is best society.";
	m_wnd_style = WS_OVERLAPPEDWINDOW;
	gp_app = this;

	m_dev = nullptr;
	m_dev_con = nullptr;
	m_swap_chain = nullptr;
	m_render_target_view = nullptr;;
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
	Memory::SafeRelease(m_constant_buffer);
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
	m_cam = std::make_unique<Camera>(&input, this);
	triangle_loader = std::make_unique<TriangleLoader>(this, m_cam.get());

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

	D3D11_TEXTURE2D_DESC depth_stencil_desc;
	D3D11_TEXTURE2D_DESC back_buffer_desc;
	mp_back_buffer_txt->GetDesc(&back_buffer_desc);
	depth_stencil_desc.Width = back_buffer_desc.Width;
	depth_stencil_desc.Height = back_buffer_desc.Height;
	depth_stencil_desc.MipLevels = 1;
	depth_stencil_desc.ArraySize = 1;
	depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_stencil_desc.SampleDesc.Count = 1;
	depth_stencil_desc.SampleDesc.Quality = 0;
	depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_stencil_desc.CPUAccessFlags = 0;
	depth_stencil_desc.MiscFlags = 0;

	m_dev->CreateTexture2D(&depth_stencil_desc, NULL, &m_depth_txt);

	D3D11_DEPTH_STENCIL_VIEW_DESC depth_view_desc = {};
	depth_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_view_desc.Format = depth_stencil_desc.Format;
	depth_view_desc.Texture2D.MipSlice = 0;

	m_dev->CreateDepthStencilView(m_depth_txt, &depth_view_desc, &m_depth_stncl_view);

	if (!initConstantBuffer())
	{
		return false;
	}

	return true;
}

bool DXApp::initConstantBuffer()
{
	XMStoreFloat4x4(&m_const_data.m_world_matrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_const_data.m_view_matrix, XMMatrixIdentity());

	D3D11_BUFFER_DESC cb_desc;
	cb_desc.ByteWidth = sizeof(VS_CONSTANT_BUFFER);
	cb_desc.Usage = D3D11_USAGE_DYNAMIC;
	cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb_desc.MiscFlags = 0;
	cb_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA init_data;
	init_data.pSysMem = &m_const_data;
	init_data.SysMemPitch = 0;
	init_data.SysMemSlicePitch = 0;

	auto hr = m_dev->CreateBuffer(&cb_desc, &init_data, &m_constant_buffer);
	if (FAILED(hr))
	{
		return false;
	}

	m_dev_con->VSSetConstantBuffers(0, 1, &m_constant_buffer);

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
		if (w_param == (int)KeyBind::Esc)	//probably enumerated or defined somewhere, but this is esc
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

void DXApp::setColour(int colour_index)
{
	m_colour[colour_index] = m_colour[colour_index] >= 1 ? 0 : m_colour[colour_index] + 0.1f;
}

void DXApp::updateConstantBuffer(XMMATRIX world, XMMATRIX view)
{
	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	XMStoreFloat4x4(&m_const_data.m_world_matrix, world);
	XMStoreFloat4x4(&m_const_data.m_view_matrix, view);

	D3D11_MAPPED_SUBRESOURCE mappedSubResource;
	memset(&mappedSubResource, 0, sizeof(mappedSubResource));

	HRESULT hr = m_dev_con->Map(m_constant_buffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedSubResource);
	memcpy(mappedSubResource.pData, &m_const_data, sizeof(VS_CONSTANT_BUFFER));
	m_dev_con->Unmap(m_constant_buffer, 0);

	m_dev_con->VSSetConstantBuffers(0, 1, &m_constant_buffer);
}