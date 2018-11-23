#include "Scene.h"
#include "GeometryIncludes.h"

Scene::~Scene()
{
	Memory::SafeDelete(player);
	Memory::SafeDelete(plane);
}
void setPointers(std::vector<Geometry*>* _geometry, DXApp* _app,
	CBPerObject* _cb, ID3D11DeviceContext** _dev_con,
	ID3D11Buffer** _const_buffer, Camera* _cam);
void loadUnloadObjects();
void Scene::updateScene(float dt)
{
	if (dt > last_dt)
	{
		OutputDebugString((std::to_string(frame_idx) + ": \t" + std::to_string(last_dt) + " - " + std::to_string(dt)).c_str());
		OutputDebugString("\n");
	}
	last_dt = dt;
	if (player_dir)
	{
		player_speed += player_accel * dt;
		if (player->getTransform()->getPos().x > 0)
		{
			player_dir = false;
			DXApp::loader_thread_active = true;
			loader_thread = std::thread(loadUnloadObjects);
		}
	}
	else
	{
		player_speed -= player_accel * dt;
		if (player->getTransform()->getPos().x < 0)
		{
			player_dir = true;
		}
	}
	if (!loader_thread_active)
	{
		for (Geometry* geo : external_geometry)
		{
			geo->getTransform()->rotate(XMVectorSet(1, 0, 0, 0), dt);
			geo->getTransform()->rotate(XMVectorSet(0, 1, 0, 0), dt);
			geo->getTransform()->rotate(XMVectorSet(0, 0, 1, 0), dt);
		}
	}
	player->getTransform()->translate(player_speed, 0, 0);
	XMVECTOR rotate;
	switch (rand() % 3)
	{
	case 0:
		rotate = XMVectorSet(1, 0, 0, 0);
		break;
	case 1:
		rotate = XMVectorSet(0, 1, 0, 0);
		break;
	case 2:
		rotate = XMVectorSet(0, 0, 1, 0);
		break;
	default:
		break;
	}
	player->getTransform()->rotate(rotate, dt);

	m_cam.update(dt);
	m_light.update(dt);

	frame_idx++;
}

void Scene::initObjects()
{
	m_cam = Camera(getRatio());

	plane = (new Cube());
	plane->init(this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
	plane->getTransform()->translate(0, -8, 0);
	plane->getTransform()->scale(300, 0.1f, 300);

	player = new Cube();
	player->init(this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
	player->getTransform()->translate(4.8f, 0, 0);

	for (int i = 0; i < 1000; i++)
	{
		external_geometry.push_back(new Cube());
		external_geometry.back()->init(this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
		float x, z;
		x = 5 + (rand() % 5);
		z = 5 + (rand() % 5);
		if (rand() % 2)
		{
			x *= -1;
		}
		external_geometry.back()->getTransform()->translate(x, -6, z);
	}

	setPointers(&external_geometry, this, &m_object_cb, &m_device_context, &m_cb_per_object, &m_cam);
}

void Scene::drawScene(float dt)
{
	float bg_colour[4]{ 0.0f, 0.0f, 0.0f, 1.0f };
	m_device_context->ClearRenderTargetView(m_rt_view, bg_colour);
	m_device_context->ClearDepthStencilView(m_depth_stcl_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_frame_cb.light = m_light;
	m_device_context->UpdateSubresource(m_cb_per_frame, 0, NULL, &m_frame_cb, 0, 0);
	m_device_context->PSSetConstantBuffers(0, 1, &m_cb_per_frame);

	player->draw();
	plane->draw();
	if (loader_thread.joinable() && !loader_thread_active)
	{
		loader_thread.join();
	}
	if (!loader_thread_active)
	{
		for (Geometry* g : external_geometry)
		{
			g->draw();
		}
	}
	m_swap_chain->Present(0, 0);
}