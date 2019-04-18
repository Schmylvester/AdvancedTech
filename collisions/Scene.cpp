#include <cmath>
#include "Scene.h"
#include "GeometryIncludes.h"
#include "GameObject.h"

Scene::~Scene()
{
	Memory::SafeDelete(player);
	Memory::SafeDelete(terrain);
}
void setPointers(void* _geometry, DXApp* _app,
	CBPerObject* _cb, ID3D11DeviceContext** _dev_con,
	ID3D11Buffer** _const_buffer, Camera* _cam);
void loadUnloadObjects();
void Scene::updateScene(float dt)
{
	terrain->getTransform()->scale(dt * 60, dt * 60, dt * 60);
	if (frame_count > 1000)
	{
		if (dt > max_dt)
		{
			max_dt = dt;
			OutputDebugString((std::to_string(frame_count) + ": \t" + std::to_string(last_dt) + " - " + std::to_string(dt)).c_str());
			OutputDebugString("\n");
		}
	}
	last_dt = dt;
	float new_acc = player_acc - (player_speed * 0.4f);
	if (player_dir)
	{
		player_speed += new_acc * dt;
		if (player->getTransform()->getPos().x > 0)
		{
			player_dir = false;
			DXApp::loader_thread_active = true;
			loader_thread = std::thread(loadUnloadObjects);
		}
	}
	else
	{
		player_speed -= new_acc * dt;
		if (player->getTransform()->getPos().x < 0)
		{
			player_dir = true;
		}
	}
	player->getTransform()->translate(player_speed * dt, 0, 0);

	for (GameObject* geo : visible_geometry)
	{
		geo->getTransform()->rotate(XMVectorSet(0, 0, 1, 0), dt);
	}

	if (!loader_thread_active)
	{
		for (GameObject* geo : external_geometry)
		{
			geo->getTransform()->rotate(XMVectorSet(0, 1, 0, 0), dt);
		}
	}

	frame_count++;
}

void Scene::initObjects()
{
	m_cam = Camera(getRatio());

	terrain = (new GameObject());
	terrain->init(Shape::Plane, this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
	terrain->getTransform()->translate(0, -8, 0);

	player = new GameObject();
	player->init(Shape::Cube, this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
	player->getTransform()->translate(4.8f, 0, 0);

	for (int i = 0; i < 1000; i++)
	{
		visible_geometry.push_back(new GameObject());
		visible_geometry.back()->init(Shape::Cube, this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
		float x, z;
		z = (float(rand() % 100) / 4);
		x = sqrt(100 - pow(z, 2));
		if (rand() % 2 == 0)
		{
			x *= -1;
		}
		z *= 1 + (rand() % 10);
		x *= 1 + (rand() % 10);
		visible_geometry.back()->getTransform()->translate(x, -6, z);
	}

	for (int i = 0; i < 10000; i++)
	{
		external_geometry.push_back(new GameObject());
		external_geometry.back()->init(Shape::Pyramid, this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
		float x, z;
		z = (float(rand() % 100) / 4);
		x = sqrt(100 - pow(z, 2));
		if (rand() % 2 == 0)
		{
			x *= -1;
		}
		z *= 5 + (rand() % 10);
		x *= 5 + (rand() % 10);
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
	terrain->draw();
	for (GameObject* g : visible_geometry)
	{
		g->draw();
	}
	if (loader_thread.joinable() && !loader_thread_active)
	{
		loader_thread.join();
	}
	if (!loader_thread_active)
	{
		for (GameObject* g : external_geometry)
		{
			g->draw();
		}
	}
	m_swap_chain->Present(0, 0);
}