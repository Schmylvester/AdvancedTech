#include "TerrainScene.h"
#include "Terrain.h"

TerrainScene::~TerrainScene()
{
	Memory::SafeDelete(terrain);
}

void TerrainScene::updateScene(float dt)
{
	m_cam.update(dt);
	m_light.update(dt);
}

void TerrainScene::drawScene(float dt)
{
	float bg_colour[4]{ 0.0f, 0.0f, 0.0f, 1.0f };
	m_device_context->ClearRenderTargetView(m_rt_view, bg_colour);
	m_device_context->ClearDepthStencilView(m_depth_stcl_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_frame_cb.light = m_light;
	m_device_context->UpdateSubresource(m_cb_per_frame, 0, NULL, &m_frame_cb, 0, 0);
	m_device_context->PSSetConstantBuffers(0, 1, &m_cb_per_frame);

	terrain->draw();

	m_swap_chain->Present(0, 0);
}

void TerrainScene::initObjects()
{
	m_cam = Camera(getRatio());

	terrain = (new Terrain("..\\Resources\\HeightMap.bmp"));
	terrain->init(this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
	terrain->getTransform()->translate(-64, -18, 0);
}
