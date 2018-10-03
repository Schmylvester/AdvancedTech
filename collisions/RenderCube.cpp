#include "RenderCube.h"
#include "DXApp.h"

RenderCube::RenderCube(DXApp* app)
{
	m_app = app;

	for (int i = 0; i < 12; i++)
	{
		createTriangle();
	}
}

RenderCube::~RenderCube()
{
}

void RenderCube::updateVertices(int i, float x, float y, float z)
{
	m_vertices[i].R = 1.0f - ((float)(rand() % 10) / 20);
	m_vertices[i].G = 1.0f - ((float)(rand() % 10) / 20);
	m_vertices[i].B = 1.0f - ((float)(rand() % 10) / 20);

	m_vertices[i].X = x;
	m_vertices[i].Y = y;
	m_vertices[i].Z = z;
}

void RenderCube::createTriangle()
{
	Triangle* t = new Triangle();
	t->initPipeline(m_app);
	m_polygons.push_back(t);
}

void RenderCube::setTriangleValues(int triangle_index, int a, int b, int c)
{
	Vertex vertices[3];
	vertices[0] = m_vertices[a];
	vertices[1] = m_vertices[b];
	vertices[2] = m_vertices[c];
	m_polygons[triangle_index]->setVertices(vertices);

}