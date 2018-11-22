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