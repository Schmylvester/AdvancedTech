#pragma once
#include "DXUtil.h"
#include <string>
#include <time.h>
#include <SimpleMath.h>
#include <functional>
#include <thread>

#include "Window.h"
#include "ConstantBuffers.h"
#include "Camera.h"

enum LoadType
{
	NEVER_MIND,

	LOAD_TILE,
	LOAD_OBJECT,
	SAVE_TILE_DATA,
};

using namespace DirectX;

class Geometry;
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

	ID3D11Device* getDevice() { return m_device; }
	ID3D11DeviceContext* getContext() { return m_device_context; }
private:
	void updateScene(float dt);
	void drawScene(float dt);
	float getRatio() { return (float)m_client_width / m_client_height; }

	bool initScene();
	void initObjects();
	bool createDevice();
	bool createIndexBuffer();
	bool createVertexBuffer();
	bool createPixelBuffer();
	bool createInputLayout();
	bool createViewport();
	bool createDepthStencil();
	bool createConstBuffer();

	float getDeltaTime();

	Camera m_cam;
	Geometry* geometry;
	Window window;
	HINSTANCE m_h_app_instance;
	const UINT m_client_width = 800;
	const UINT m_client_height = 600;
	
	IDXGISwapChain* m_swap_chain = nullptr;
	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_device_context = nullptr;
	ID3D11RenderTargetView* m_rt_view = nullptr;
	ID3D11VertexShader* m_v_shader = nullptr;
	ID3D11PixelShader* m_p_shader = nullptr;
	ID3D10Blob* m_v_buffer = nullptr;
	ID3D10Blob* m_p_buffer = nullptr;
	ID3D11InputLayout* m_vertex_layout = nullptr;
	ID3D11Buffer* m_geo_vert_buffer = nullptr;
	ID3D11Buffer* m_geo_index_buffer = nullptr;
	ID3D11DepthStencilView* m_depth_stcl_view = nullptr;
	ID3D11Texture2D* m_depth_stcl_buffer = nullptr;
	ID3D11Buffer* m_cb_per_object = nullptr;

	CBPerObject m_object_cb;

	XMMATRIX m_world_matrix = XMMatrixIdentity();

	std::thread loader_thread;

	clock_t last_clock = clock();
};