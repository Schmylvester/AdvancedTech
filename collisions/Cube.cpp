#include "Cube.h"

Cube::Cube(DXApp* app, Transform transform)
{
	m_transform = transform;
	float col = (float)(rand() % 100) / 100;

	float dist[3];
	dist[0] = transform.scale[0] / 2;
	dist[1] = transform.scale[1] / 2;
	dist[2] = transform.scale[2] / 2;

	Triangle* t;
	Vertex vertices[3];
	
	t = new Triangle();
	t->initPipeline(app);
	vertices[0] = { transform.position[0] - dist[0], transform.position[1] + dist[1], transform.position[2] + dist[2], col, col, col, 1.0f };
	vertices[1] = { transform.position[0] + dist[0], transform.position[1] - dist[1], transform.position[2] + dist[2], col, col, col, 1.0f };
	vertices[2] = { transform.position[0] - dist[0], transform.position[1] - dist[1], transform.position[2] + dist[2], col, col, col, 1.0f };
	t->setVertices(vertices);
	m_polygons.push_back(t);

	t = new Triangle();
	t->initPipeline(app);
	vertices[0] = { transform.position[0] + dist[0], transform.position[1] - dist[1], transform.position[2] + dist[2], col, col, col, 1.0f };
	vertices[1] = { transform.position[0] - dist[0], transform.position[1] + dist[1], transform.position[2] + dist[2], col, col, col, 1.0f };
	vertices[2] = { transform.position[0] + dist[0], transform.position[1] + dist[1], transform.position[2] + dist[2], col, col, col, 1.0f };
	t->setVertices(vertices);
	m_polygons.push_back(t);
}

Cube::~Cube()
{
}