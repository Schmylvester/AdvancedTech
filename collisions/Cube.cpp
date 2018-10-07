#include "Cube.h"
#include "RenderCube.h"
#include "PhysicsCube.h"
#include "DXApp.h"

Cube::Cube(DXApp* app, float colour[4], Vector3 position)
{
	m_position = position;
	m_render = new RenderCube(app, colour);
	m_physics = new PhysicsCube(&m_position);
	for (int i = 0; i < 8; i++)
	{
		m_corners[i].x = (i < 4) ? -1 : 1;
		m_corners[i].x *= (0.05f / app->getWidthHeightRatio());
		m_corners[i].x += position.x;

		m_corners[i].y = ((i / 2) % 2) == 0 ? -1 : 1;
		m_corners[i].y *= 0.05f;
		m_corners[i].y += position.y;

		m_corners[i].z = (i % 2 == 0) ? -1 : 1;
		m_corners[i].z *= 0.05f;
		m_corners[i].z += position.z;

		m_render->updateVertices(i, m_corners[i].x, m_corners[i].y, m_corners[i].z);
		setRenderTriangles();
	}
}

Cube::~Cube()
{
	Memory::SafeDelete(m_physics);
	Memory::SafeDelete(m_render);
}

void Cube::draw()
{
	for(int i = 0; i < 8; i++)
		m_render->updateVertices(i, m_corners[i].x, m_corners[i].y, m_corners[i].z);
	setRenderTriangles();
	m_render->draw();
}

void Cube::tick(float dt)
{
	Vector3 old_pos = m_position;
	m_physics->tick(dt);
	if (old_pos != m_position)
	{
		for (int i = 0; i < 8; i++)
		{
			m_corners[i].x += (m_position.x - old_pos.x);
			m_corners[i].y += (m_position.y - old_pos.y);
			m_corners[i].z += (m_position.z - old_pos.z);
		}
	}
}

PhysicsCube * Cube::getPhysics()
{
	return m_physics;
}

void Cube::setRenderTriangles()
{
	m_render->setTriangleValues(0, 1, 7, 5);
	m_render->setTriangleValues(1, 3, 7, 1);
	m_render->setTriangleValues(2, 5, 7, 4);
	m_render->setTriangleValues(3, 4, 7, 6);
	m_render->setTriangleValues(4, 6, 7, 3);
	m_render->setTriangleValues(5, 4, 6, 0);
	m_render->setTriangleValues(6, 0, 6, 2);
	m_render->setTriangleValues(7, 2, 6, 3);
	m_render->setTriangleValues(8, 0, 2, 3);
	m_render->setTriangleValues(9, 3, 1, 0);
	m_render->setTriangleValues(10, 5, 4, 1);
	m_render->setTriangleValues(11, 1, 4, 0);
}
