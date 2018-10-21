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
	DXApp() = delete;
	DXApp(HINSTANCE h_instance);
	virtual ~DXApp();

	//main application loop
	int run();

	virtual bool init();
	virtual void update(float dt) = 0;
	virtual void render(float dt) = 0;
	virtual LRESULT msgProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);

	ID3D11Device* getDevice() { return m_dev; }
	ID3D11DeviceContext* getContext() { return m_dev_con; }
	float getRatio() { return (float)m_client_width / m_client_height; }

	Camera* getCam() { return m_cam.get(); }
	TriangleLoader* getLoader() { return &triangle_loader; }
	void updateConstantBuffer(XMMATRIX world, XMMATRIX view);
protected:
	float fps;
	TriangleLoader triangle_loader;

	void setColour(int colour_index);
	bool initWindow();
	bool initDirect3D();
	bool initConstantBuffer();
	int quitApp();
	float getDeltaTime();

	std::unique_ptr<Camera> m_cam;
	float m_colour[4];

	HWND m_h_app_wnd;
	HINSTANCE m_h_app_instance;
	UINT m_client_width = 800;
	UINT m_client_height = 600;
	std::string m_app_title;
	DWORD m_wnd_style;

	ID3D11DepthStencilView* m_depth_stncl_view;
	ID3D11Texture2D* m_depth_txt = nullptr;
	ID3D11Device* m_dev = nullptr;
	ID3D11DeviceContext* m_dev_con = nullptr;
	IDXGISwapChain* m_swap_chain = nullptr;
	ID3D11RenderTargetView* m_render_target_view = nullptr;
	D3D_DRIVER_TYPE m_driver_type;
	D3D_FEATURE_LEVEL m_feature_level;
	D3D11_VIEWPORT m_viewport;
	ID3D11Buffer* m_constant_buffer = nullptr;
	VS_CONSTANT_BUFFER m_const_data;

	Input input;
	clock_t last_clock = clock();
};