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
#include "BufferStructs.h"

using namespace DirectX;

class Geometry;
class GameObject;
class DXApp
{
public:
	DXApp() = default;
	virtual ~DXApp();

	//main application loop
	int run(int n_cmd_show);

	bool init(HINSTANCE h_instance, int n_show_cmd);
	bool initDirectX3D(HINSTANCE h_instance);
	void releaseObjects();

	ID3D11Device* getDevice() { return m_device; }
	ID3D11DeviceContext* getContext() { return m_device_context; }

	ID3D11Buffer* getIndexBuffer(std::string id, Geometry* geo);
	ID3D11Buffer* getVertexBuffer(std::string id, Geometry* geo);

protected:
	bool initScene();
	virtual void updateScene(float dt) = 0;
	virtual void initObjects() = 0;
	virtual void drawScene(float dt) = 0;

	void toggleWireframe();

	ID3D11DeviceContext* m_device_context = nullptr;
	CBPerObject m_object_cb;
	ID3D11Buffer* m_cb_per_object = nullptr;
	CBPerFrame m_frame_cb;
	Input m_input;
	ID3D11RenderTargetView* m_rt_view = nullptr;
	ID3D11Texture2D* m_depth_stcl_buffer = nullptr;
	ID3D11DepthStencilView* m_depth_stcl_view = nullptr;
	Light m_light;
	ID3D11Buffer* m_cb_per_frame = nullptr;
	IDXGISwapChain* m_swap_chain = nullptr;
	Camera m_cam;
	float getRatio() { return (float)m_client_width / m_client_height; }

private:
	void draw(float dt);
	float getDeltaTime();
	Window window;
	HINSTANCE m_h_app_instance;
	const UINT m_client_width = 800;
	const UINT m_client_height = 600;

	ID3D11Device* m_device = nullptr;
	ID3D11VertexShader* m_v_shader = nullptr;
	ID3D11PixelShader* m_p_shader = nullptr;
	ID3D10Blob* m_v_buffer = nullptr;
	ID3D10Blob* m_p_buffer = nullptr;
	ID3D11InputLayout* m_vertex_layout = nullptr;
	ID3D11ShaderResourceView* m_cube_texture = nullptr;
	ID3D11SamplerState* m_cubes_text_sampler_state = nullptr;
	ID3D11Resource* m_texture = nullptr;

	bool wireframe_active = false;
	ID3D11RasterizerState* m_solid = nullptr;
	ID3D11RasterizerState* m_wireframe = nullptr;

	std::vector<IndexBuffer> m_geo_index_buffers;
	std::vector<VertexBuffer> m_vertex_buffers;
	
	clock_t last_clock = clock();

	bool createDevice();
	bool createPixelBuffer();
	bool createInputLayout();
	bool createViewport();
	bool createDepthStencil();
	bool createConstBuffer();
	bool initRasteriserStates();
};