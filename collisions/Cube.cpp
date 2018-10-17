#include "Cube.h"
#include "RenderCube.h"
#include "DXApp.h"
#include "ColliderIncludes.h"

Cube::Cube(DXApp * app, float colour[4])
{
	m_transform = new Transform();
	m_render = new RenderCube(app, colour, m_transform->getMatrix(), app->getLoader());
	//if (rand() % 2 == 0)
	//	m_physics = new BoxCollider(m_transform);
	//else
		m_physics = new SphereCollider(m_transform, m_render);
	for (int i = 0; i < 8; i++)
	{
		m_corners[i].x = (i < 4) ? -1 : 1;
		m_corners[i].x *= m_transform->getScale().x;
		m_corners[i].y = ((i / 2) % 2) == 0 ? -1 : 1;
		m_corners[i].y *= m_transform->getScale().y;
		m_corners[i].z = (i % 2 == 0) ? -1 : 1;
		m_corners[i].z *= m_transform->getScale().z;

		m_render->updateVertices(i, m_corners[i].x, m_corners[i].y, m_corners[i].z);
		setRenderTriangles(true);
	}
}

Cube::~Cube()
{
	Memory::SafeDelete(m_physics);
	Memory::SafeDelete(m_render);
	Memory::SafeDelete(m_transform);
}

void Cube::draw()
{
	m_transform->update();
	for(int i = 0; i < 8; i++)
		m_render->updateVertices(i, m_corners[i].x, m_corners[i].y, m_corners[i].z);
	setRenderTriangles(false);
	m_render->draw();
}

void Cube::tick(float dt)
{
	m_physics->tick(dt);
}

void Cube::move(float x, float y, float z)
{
	m_transform->move(x, y, z);
}

void Cube::rotate(char axis, float rot)
{
	m_transform->rotate(axis, rot);
}

void Cube::setScl(float x, float y, float z)
{

	m_transform->setScl(x, y, z);
}

Collider * Cube::getPhysics()
{
	return m_physics;
}

void Cube::setRenderTriangles(bool init)
{
	m_render->setTriangleValues(0, 1, 7, 5, init);
	m_render->setTriangleValues(1, 3, 7, 1, init);
	m_render->setTriangleValues(2, 5, 7, 4, init);
	m_render->setTriangleValues(3, 4, 7, 6, init);
	m_render->setTriangleValues(4, 6, 7, 3, init);
	m_render->setTriangleValues(5, 4, 6, 0, init);
	m_render->setTriangleValues(6, 0, 6, 2, init);
	m_render->setTriangleValues(7, 2, 6, 3, init);
	m_render->setTriangleValues(8, 0, 2, 3, init);
	m_render->setTriangleValues(9, 3, 1, 0, init);
	m_render->setTriangleValues(10, 5, 4, 1, init);
	m_render->setTriangleValues(11, 1, 4, 0, init);
}
