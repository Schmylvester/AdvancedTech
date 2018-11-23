#include "Scene.h"
#include "Cube.h"

void Scene::updateScene(float dt)
{
	m_cam.update(dt);
	m_light.update(dt);
}

void Scene::initObjects()
{
	m_cam = Camera(getRatio());

	geometry.push_back(new Cube());
	geometry.back()->init(this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
	geometry.back()->getTransform()->translate(0, -8, 0);
	geometry.back()->getTransform()->scale(300, 0.1f, 300);

	for (int i = 0; i < 70; i++)
	{
		geometry.push_back(new Cube());
		geometry.back()->init(this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
		geometry.back()->getTransform()->translate(2 * ((rand() % 40) - 20), -6, 2 * (rand() % 40));
	}
}

void Scene::drawScene(float dt)
{
	float bg_colour[4]{ 0.0f, 0.0f, 0.0f, 1.0f };
	m_device_context->ClearRenderTargetView(m_rt_view, bg_colour);
	m_device_context->ClearDepthStencilView(m_depth_stcl_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_frame_cb.light = m_light;
	m_device_context->UpdateSubresource(m_cb_per_frame, 0, NULL, &m_frame_cb, 0, 0);
	m_device_context->PSSetConstantBuffers(0, 1, &m_cb_per_frame);

	for (Geometry* g : geometry)
	{
		g->draw();
	}

	m_swap_chain->Present(0, 0);
}