#include "DXApp.h"

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

bool DXApp::init(HINSTANCE h_instance, int n_show_cmd, std::function<int(LoadType, std::string)> func)
{
	loader_thread = std::thread(func, LoadType::LOAD_OBJECT, "pop");
	loader_thread.detach();

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

	createDevice();

	ID3D11Texture2D* back_buffer;
	hr = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer);

	hr = m_device->CreateRenderTargetView(back_buffer, NULL, &m_rt_view);
	Memory::SafeRelease(back_buffer);

	m_device_context->OMSetRenderTargets(1, &m_rt_view, NULL);

	initScene();

	return true;
}

void DXApp::releaseObjects()
{
	Memory::SafeRelease(m_swap_chain);
	Memory::SafeRelease(m_device_context);
	Memory::SafeRelease(m_device);
	Memory::SafeRelease(m_rt_view);
	Memory::SafeRelease(m_geo_vert_buffer);
	Memory::SafeRelease(m_geo_index_buffer);
	Memory::SafeRelease(v_shader);
	Memory::SafeRelease(p_shader);
	Memory::SafeRelease(v_buffer);
	Memory::SafeRelease(p_buffer);
	Memory::SafeRelease(m_vertex_layout);
	Memory::SafeRelease(m_depth_stcl_view);
	Memory::SafeRelease(m_depth_stcl_buffer);
}

void DXApp::updateScene()
{

}

void DXApp::drawScene()
{
	float bg_colour[4]{ 0.9f, 0.6f, 1.0f, 1.0f };
	m_device_context->ClearRenderTargetView(m_rt_view, bg_colour);
	m_device_context->ClearDepthStencilView(m_depth_stcl_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0F, 0);

	m_device_context->DrawIndexed(geometry.getIndexCount(), 0, 0);

	m_swap_chain->Present(0, 0);
}

bool DXApp::createDevice()
{
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

	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION,
		&swap_chain_desc, &m_swap_chain, &m_device, NULL, &m_device_context);
	if (hr != S_OK)
	{
		errorBox("Device Initialisation - Failed");
		return false;
	}
	return true;
}

bool DXApp::initScene()
{
	if (!createVertexBuffer())
	{
		errorBox("Vertex buffer creation failed");
		return false;
	}

	if (!createPixelBuffer())
	{
		errorBox("Pixel buffer creation failed");
		return false;
	}

	if (!createInputLayout())
	{
		errorBox("Failed to create input layout");
		return false;
	}

	if (!createIndexBuffer())
	{
		errorBox("Failed to create index buffer");
		return false;
	}

	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	createViewport();

	if (!createDepthStencil())
	{
		errorBox("Failed to create depth stencil");
		return false;
	}

	return true;
}

bool DXApp::createIndexBuffer()
{
	D3D11_BUFFER_DESC index_buffer_desc;
	ZeroMemory(&index_buffer_desc, sizeof(index_buffer_desc));

	index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	index_buffer_desc.ByteWidth = sizeof(DWORD) * 3 * geometry.getTriangleCount();
	index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	index_buffer_desc.CPUAccessFlags = 0;
	index_buffer_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data;
	init_data.pSysMem = geometry.getIndices();
	if (m_device->CreateBuffer(&index_buffer_desc, &init_data, &m_geo_index_buffer) != S_OK)
	{
		return false;
	}
	m_device_context->IASetIndexBuffer(m_geo_index_buffer, DXGI_FORMAT_R32_UINT, 0);

	return true;
}

bool DXApp::createVertexBuffer()
{
	HRESULT hr;
	hr = D3DCompileFromFile(L"shaders.shader", NULL, NULL, "VShader", "vs_5_0", NULL, NULL, &v_buffer, NULL);
	if (hr != S_OK)
	{
		errorBox("The vertex shader didn't load right");
		return false;
	}

	hr = m_device->CreateVertexShader(v_buffer->GetBufferPointer(), v_buffer->GetBufferSize(), NULL, &v_shader);
	if (hr != S_OK)
	{
		errorBox("The vertex shader couldn't be created");
		return false;
	}

	m_device_context->VSSetShader(v_shader, NULL, NULL);

	D3D11_BUFFER_DESC vertex_buffer_desc;
	ZeroMemory(&vertex_buffer_desc, sizeof(vertex_buffer_desc));

	vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertex_buffer_desc.ByteWidth = sizeof(Vertex) * geometry.getVertCount();
	vertex_buffer_desc.CPUAccessFlags = 0;
	vertex_buffer_desc.MiscFlags = 0;
	vertex_buffer_desc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA vertex_buffer_data;
	ZeroMemory(&vertex_buffer_data, sizeof(vertex_buffer_data));
	vertex_buffer_data.pSysMem = geometry.getVertices();
	hr = m_device->CreateBuffer(&vertex_buffer_desc, &vertex_buffer_data, &m_geo_vert_buffer);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_device_context->IASetVertexBuffers(0, 1, &m_geo_vert_buffer, &stride, &offset);
	return true;
}

bool DXApp::createPixelBuffer()
{
	HRESULT hr;
	hr = D3DCompileFromFile(L"shaders.shader", NULL, NULL, "PShader", "ps_5_0", NULL, NULL, &p_buffer, NULL);
	if (hr != S_OK)
	{
		errorBox("The pixel shader didn't load right");
		return false;
	}
	hr = m_device->CreatePixelShader(p_buffer->GetBufferPointer(), p_buffer->GetBufferSize(), NULL, &p_shader);
	if (hr != S_OK)
	{
		errorBox("The pixel shader couldn't be created");
		return false;
	}

	m_device_context->PSSetShader(p_shader, NULL, NULL);
	return true;
}

bool DXApp::createInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT num_elements = ARRAYSIZE(layout);

	HRESULT hr = m_device->CreateInputLayout(layout, num_elements, v_buffer->GetBufferPointer(), v_buffer->GetBufferSize(), &m_vertex_layout);
	if (hr != S_OK)
	{
		return false;
	}
	m_device_context->IASetInputLayout(m_vertex_layout);

	return true;
}

bool DXApp::createViewport()
{
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(viewport));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = m_client_width;
	viewport.Height = m_client_height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	m_device_context->RSSetViewports(1, &viewport);

	return true;
}

bool DXApp::createDepthStencil()
{
	D3D11_TEXTURE2D_DESC depth_stcl_desc;
	depth_stcl_desc.Width = m_client_width;
	depth_stcl_desc.Height = m_client_height;
	depth_stcl_desc.MipLevels = 1;
	depth_stcl_desc.ArraySize = 1;
	depth_stcl_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_stcl_desc.SampleDesc.Count = 1;
	depth_stcl_desc.SampleDesc.Quality = 0;
	depth_stcl_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_stcl_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_stcl_desc.CPUAccessFlags = 0;
	depth_stcl_desc.MiscFlags = 0;

	HRESULT hr = m_device->CreateTexture2D(&depth_stcl_desc, NULL, &m_depth_stcl_buffer);
	if (hr != S_OK)
	{
		errorBox("Failed to create Texture2D");
		return false;
	}
	hr = m_device->CreateDepthStencilView(m_depth_stcl_buffer, NULL, &m_depth_stcl_view);
	if (hr != S_OK)
	{
		errorBox("Failed to create depth stencil view");
		return false;
	}
	m_device_context->OMSetRenderTargets(1, &m_rt_view, m_depth_stcl_view);


	return true;
}

float DXApp::getDeltaTime()
{
	float f = float(clock() - last_clock) / CLOCKS_PER_SEC;
	last_clock = clock();
	return f;
}