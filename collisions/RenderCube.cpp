#include "RenderCube.h"
#include "DXApp.h"

RenderCube::RenderCube(DXApp* app, float colour[4], DirectX::XMMATRIX* _world_matrix, TriangleLoader* loader)
{
	m_app = app;

	for (int i = 0; i < 12; i++)
	{
		createTriangle(_world_matrix, loader);
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

void RenderCube::changeColour()
{
	for (int i = 0; i < 8; i++)
	{
		switch (rand() % 6)
		{
		case 0:
			m_vertices[i].R = 1.0f;
			break;
		case 1:
			m_vertices[i].G = 1.0f;
			break;
		case 2:
			m_vertices[i].R = 0.0f;
			break;
		case 3:
			m_vertices[i].B = 1.0f;
			break;
		case 4:
			m_vertices[i].G = 0.0f;
			break;
		case 5:
			m_vertices[i].B = 0.0f;
			break;
		}
	}
}

void RenderCube::updateVertices(int i, float x, float y, float z)
{
	m_vertices[i].X = x;
	m_vertices[i].Y = y;
	m_vertices[i].Z = z;
}

void RenderCube::createTriangle(DirectX::XMMATRIX* _world_matrix, TriangleLoader* loader)
{
	Triangle* t = new Triangle();
	t->initPipeline(m_app, _world_matrix, loader);
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