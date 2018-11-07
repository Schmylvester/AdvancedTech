#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include <time.h>
#include "DXUtil.h"
#include <SimpleMath.h>
#include "Camera.h"
#include "Input.h"
#include "TriangleLoader.h"

using namespace DirectX;

struct VS_CONSTANT_BUFFER
{
	XMFLOAT4X4 m_world_matrix;
	XMFLOAT4X4 m_view_matrix;
};

class DXApp
{
public:
	DXApp() = default;
	DXApp(HINSTANCE h_instance);
	virtual ~DXApp();

	//main application loop
	int run();

	virtual bool init(HINSTANCE h_instance);
	bool initDirectX3D(HINSTANCE h_instance);
	void releaseObjects();
	void updateScene();
	void drawScene();

	virtual LRESULT msgProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);

	float getRatio() { return (float)m_client_width / m_client_height; }

	Camera* getCam() { return m_cam.get(); }
	TriangleLoader* getLoader() { return triangle_loader.get(); }

	ID3D11Device* getDevice() { return m_device; }
	ID3D11DeviceContext* getContext() { return m_device_context; }
protected:
	float fps;
	std::unique_ptr<TriangleLoader> triangle_loader = nullptr;

	bool initWindow();
	int quitApp();
	float getDeltaTime();

	std::unique_ptr<Camera> m_cam;

	HWND m_h_app_wnd;
	HINSTANCE m_h_app_instance;
	UINT m_client_width = 800;
	UINT m_client_height = 600;
	std::string m_app_title;
	DWORD m_wnd_style;

	IDXGISwapChain* m_swap_chain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_device_context;
	ID3D11RenderTargetView* m_rt_view;

	float red = 0.0f;
	float green = 0.0f;
	float blue = 0.0f;
	int colourmodr = 1;
	int colourmodg = 1;
	int colourmodb = 1;

	Input input;
	clock_t last_clock = clock();
};