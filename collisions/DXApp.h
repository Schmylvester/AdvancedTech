#pragma once
#include "DXUtil.h"
#include <string>
#include <time.h>
#include <SimpleMath.h>
#include <functional>
#include <thread>

#include "Window.h"

enum LoadType
{
	NEVER_MIND,

	LOAD_TILE,
	LOAD_OBJECT,
	SAVE_TILE_DATA,
};

using namespace DirectX;

class DXApp
{
public:
	DXApp() = default;
	DXApp(HINSTANCE h_instance);
	~DXApp();

	//main application loop
	int run(int n_cmd_show);

	bool init(HINSTANCE h_instance, int n_show_cmd, std::function<int(LoadType, std::string)> func);
	bool initDirectX3D(HINSTANCE h_instance);
	void releaseObjects();
	void updateScene();
	void drawScene();


	float getRatio() { return (float)m_client_width / m_client_height; }

	ID3D11Device* getDevice() { return m_device; }
	ID3D11DeviceContext* getContext() { return m_device_context; }
private:
	bool createDevice();
	bool initScene();
	bool createBuffers();
	bool createInputLayout();
	void createViewport();

	float getDeltaTime();

	Window window;
	HINSTANCE m_h_app_instance;
	const UINT m_client_width = 800;
	const UINT m_client_height = 600;
	
	IDXGISwapChain* m_swap_chain			= nullptr;
	ID3D11Device* m_device					= nullptr;
	ID3D11DeviceContext* m_device_context	= nullptr;
	ID3D11RenderTargetView* m_rt_view		= nullptr;
	ID3D11Buffer* triangleVertBuffer		= nullptr;
	ID3D11VertexShader* VS					= nullptr;
	ID3D11PixelShader* PS					= nullptr;
	ID3D10Blob* VS_Buffer					= nullptr;
	ID3D10Blob* PS_Buffer					= nullptr;
	ID3D11InputLayout* vertLayout			= nullptr;

	std::thread loader_thread;

	clock_t last_clock = clock();
};