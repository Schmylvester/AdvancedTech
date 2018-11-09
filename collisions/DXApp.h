#pragma once
#include "DXUtil.h"
#include <string>
#include <time.h>
#include <SimpleMath.h>

#include "Window.h"
#include "Camera.h"
#include "TriangleLoader.h"

using namespace DirectX;

class DXApp
{
public:
	DXApp() = default;
	DXApp(HINSTANCE h_instance);
	~DXApp();

	//main application loop
	int run(int n_cmd_show);

	bool init(HINSTANCE h_instance, int n_show_cmd);
	bool initDirectX3D(HINSTANCE h_instance);
	void releaseObjects();
	void updateScene();
	void drawScene();


	float getRatio() { return (float)m_client_width / m_client_height; }

	Camera* getCam() { return m_cam.get(); }
	TriangleLoader* getLoader() { return triangle_loader.get(); }

	ID3D11Device* getDevice() { return m_device; }
	ID3D11DeviceContext* getContext() { return m_device_context; }
private:
	float fps;
	std::unique_ptr<TriangleLoader> triangle_loader = nullptr;

	float getDeltaTime();

	std::unique_ptr<Camera> m_cam;
	Window window;

	HINSTANCE m_h_app_instance;
	const UINT m_client_width = 800;
	const UINT m_client_height = 600;
	
	IDXGISwapChain* m_swap_chain = nullptr;
	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_device_context = nullptr;
	ID3D11RenderTargetView* m_rt_view = nullptr;

	float red = 0.0f;
	float green = 0.0f;
	float blue = 0.0f;
	int colourmodr = 1;
	int colourmodg = 1;
	int colourmodb = 1;

	clock_t last_clock = clock();
};