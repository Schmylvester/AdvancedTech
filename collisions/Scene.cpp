#include "Scene.h"
#include "Cube.h"

void Scene::updateScene(float dt)
{
	m_cam.update(dt);
	
	for (Geometry* geo : geometry)
	{
		int rot_spd = rand() % 10;
		geo->getTransform()->rotate(XMVectorSet(0, 0, -1, 0), rot_spd * dt);
	}
}

void Scene::initObjects()
{
	m_cam = Camera(getRatio());

	m_light.dir = XMFLOAT3(0.25f, 0.5f, -1.0f);
	m_light.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	for (int x = -5; x < 6; x++)
	{
		for (int y = -5; y < 6; y++)
		{
			for (int z = 1; z < 11; z++)
			{
				geometry.push_back(new Cube());
				geometry.back()->init(this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
				geometry.back()->getTransform()->translate(x * 3, y * 3, z * 3);
			}
		}
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