#include "RenderCube.h"
#include "DXApp.h"

RenderCube::RenderCube(DXApp* app, float colour[4], XMMATRIX* _world_matrix)
{
	m_app = app;

	for (int i = 0; i < 12; i++)
	{
		createTriangle(_world_matrix);
	}
	for (int i = 0; i < 4; i++)
		m_colour[i] = colour[i];

	for (int i = 0; i < 8; i++)
	{
		float r, g, b;
		r = (i % 2 == 0 ? 1 : 0);
		g = (i < 4 ? 1 : 0);
		b = (((i / 2) % 2) == 0 ? 1 : 0);

		m_vertices[i].R = m_colour[0] * r;
		m_vertices[i].G = m_colour[1] * g;
		m_vertices[i].B = m_colour[2] * b;
	}
}

RenderCube::~RenderCube()
{
}

void RenderCube::updateVertices(int i, float x, float y, float z)
{
	m_vertices[i].X = x;
	m_vertices[i].Y = y;
	m_vertices[i].Z = z;
}

void RenderCube::createTriangle(XMMATRIX* _world_matrix)
{
	Triangle* t = new Triangle();
	t->initPipeline(m_app, _world_matrix);
	m_polygons.push_back(t);
}

void RenderCube::setTriangleValues(int triangle_index, int a, int b, int c, bool init)
{
	Vertex vertices[3];
	vertices[0] = m_vertices[a];
	vertices[1] = m_vertices[b];
	vertices[2] = m_vertices[c];
	if (init)
	{
		m_polygons[triangle_index]->initVertices(vertices);
	}
	else
		m_polygons[triangle_index]->setVertices(vertices);
}