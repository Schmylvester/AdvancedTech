#include "DXApp.h"
#include <DirectXColors.h>

DXApp::DXApp(HINSTANCE h_instance)
{
}

DXApp::~DXApp()
{

}

int DXApp::run(int n_cmd_show)
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			window.runWindow(n_cmd_show);
			float dt = getDeltaTime();
			updateScene();
			drawScene();
		}
	}
	return msg.wParam;
}

bool DXApp::init(HINSTANCE h_instance, int n_show_cmd, int threaded_func())
{
	threaded_func();
	if (!window.init(h_instance, n_show_cmd, m_client_width, m_client_height, true))
	{
		errorBox("Window Initialization - Failed");
		return false;
	}
	if (!initDirectX3D(h_instance))
	{
		errorBox("Direct3D Initialisation - Failed");
		return false;
	}

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
	swap_chain_desc.OutputWindow = *window.getAppWnd();
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
	Memory::SafeDelete(m_swap_chain);
	Memory::SafeDelete(m_device_context);
	Memory::SafeDelete(m_device);
	Memory::SafeDelete(m_rt_view);
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

void DXApp::loadShtuff()
{
}

float DXApp::getDeltaTime()
{
	float f = float(clock() - last_clock) / CLOCKS_PER_SEC;
	fps = 1 / f;
	last_clock = clock();
	return f;
}