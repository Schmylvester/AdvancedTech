#include "ObjectRenderer.h"
#include "DXApp.h"

ObjectRenderer::ObjectRenderer(DXApp* app, float colour[4], DirectX::XMMATRIX* _world_matrix, TriangleLoader* loader)
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
		m_vertices[i].R = m_colour[0];
		m_vertices[i].G = m_colour[1];
		m_vertices[i].B = m_colour[2];
	}
}

ObjectRenderer::~ObjectRenderer()
{
}

void ObjectRenderer::changeColour()
{
}

void ObjectRenderer::updateVertices(int i, float x, float y, float z)
{
	m_vertices[i].X = x;
	m_vertices[i].Y = y;
	m_vertices[i].Z = z;
	m_vertices[i].R = m_colour[0] + ((x > 0) ? 0.4f : -0.4f);
	m_vertices[i].G = m_colour[1] + ((y > 0) ? 0.4f : -0.4f);
	m_vertices[i].B = m_colour[2] + ((z > 0) ? 0.4f : -0.4f);
}

void ObjectRenderer::createTriangle(DirectX::XMMATRIX* _world_matrix, TriangleLoader* loader)
{
	Triangle* t = new Triangle();
	t->initPipeline(m_app, _world_matrix, loader);
	m_polygons.push_back(t);
}

void ObjectRenderer::setTriangleValues(int triangle_index, int a, int b, int c, bool init)
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