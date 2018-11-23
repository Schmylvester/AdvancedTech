#include "Scene.h"
#include "Cube.h"
#include "Pyramid.h"
#include "Frustum.h"

Scene::~Scene()
{
	Memory::SafeDelete(player);
	Memory::SafeDelete(plane);
}

void Scene::updateScene(float dt)
{
	float avg_dt = (last_dt[0] + last_dt[1] + last_dt[2]);
	if (dt > avg_dt && last_dt[0] != 0)
	{
		OutputDebugString((std::to_string(frame_idx) + ": \t" + std::to_string(last_dt[2]) + " - " + std::to_string(dt)).c_str());
		OutputDebugString("\n");
	}
	last_dt[0] = last_dt[1];
	last_dt[1] = last_dt[2];
	last_dt[2] = dt;
	if (player_dir)
	{
		player_speed += player_accel * dt;
		if (player->getTransform()->getPos().x > 0)
		{
			player_dir = false;
			for (int i = 0; i < 1000; i++)
			{
				int delete_index = rand() % geometry.size();
				Memory::SafeDelete(geometry[delete_index]);
				geometry.erase(geometry.begin() + delete_index);
				int shape = rand() % 3;
				if(shape == 0)
					geometry.push_back(new Frustum());
				else if(shape == 1)
					geometry.push_back(new Pyramid());
				else
					geometry.push_back(new Cube());

				geometry.back()->init(this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
				int dist = (rand() % 3) + 3;
				geometry.back()->getTransform()->translate(dist * ((rand() % 40) - 20), -6, dist * (rand() % 40));
			}
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
	for (Geometry* geo : geometry)
	{
		geo->getTransform()->rotate(XMVectorSet(1, 0, 0, 0), dt);
		geo->getTransform()->rotate(XMVectorSet(0, 1, 0, 0), dt);
		geo->getTransform()->rotate(XMVectorSet(0, 0, 1, 0), dt);
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
		geometry.push_back(new Cube());
		geometry.back()->init(this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
		int dist = (rand() % 3) + 3;
		geometry.back()->getTransform()->translate(dist * ((rand() % 40) - 20), -6, dist * (rand() % 40));
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

	player->draw();
	plane->draw();
	for (Geometry* g : geometry)
	{
		g->draw();
	}

	m_swap_chain->Present(0, 0);
}