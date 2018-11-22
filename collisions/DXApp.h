#pragma once
#include "DXUtil.h"
#include <string>
#include <time.h>
#include <SimpleMath.h>
#include <functional>
#include <thread>
#include <vector>

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

struct VertexBuffer
{
	int vertices;
	ID3D11Buffer* buffer;
};
struct IndexBuffer
{
	int triangles;
	int vertices;
	ID3D11Buffer* buffer;
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

	ID3D11Buffer* getIndexBuffer(Geometry* geo);
	ID3D11Buffer* getVertexBuffer(Geometry* geo);
private:
	void updateScene(float dt);
	void drawScene(float dt);
	float getRatio() { return (float)m_client_width / m_client_height; }

	bool initScene();
	void initObjects();
	bool createDevice();
	bool createPixelBuffer();
	bool createInputLayout();
	bool createViewport();
	bool createDepthStencil();
	bool createConstBuffer();

	void setWireframe();

	float getDeltaTime();

	Camera m_cam;
	std::vector<Geometry*> geometry;
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
	ID3D11DepthStencilView* m_depth_stcl_view = nullptr;
	ID3D11Texture2D* m_depth_stcl_buffer = nullptr;
	ID3D11Buffer* m_cb_per_object = nullptr;
	ID3D11RasterizerState* m_wireframe = nullptr;

	std::vector<IndexBuffer> m_geo_index_buffers;
	std::vector<VertexBuffer> m_vertex_buffers;

	CBPerObject m_object_cb;

	std::thread loader_thread;

	clock_t last_clock = clock();
};