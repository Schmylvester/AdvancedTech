#include "Cube.h"

void Cube::init()
{
	triangle_count = 12;
	index_count = triangle_count * 3;
	vertex_count = 8;

	indices = new DWORD[index_count]{
		0, 1, 3,
		0, 3, 2,
		1, 5, 7,
		1, 7, 2,
		5, 4, 6,
		5, 6, 7,
		4, 0, 2,
		4, 2, 6,
		4, 1, 0,
		4, 5, 1,
		2, 3, 7,
		2, 7, 6,
	};
	vertices = new Vertex[vertex_count];

	for (int i = 0; i < vertex_count; i++)
	{
		float x = (i % 2 == 0) ? -0.5f : 0.5f;
		float y = ((i / 2) % 2) == 0 ? -0.5f : 0.5f;
		float z = (i < 4) ? -0.5f : 0.5f;

		float r = float(rand() % 100) / 100;
		float g = float(rand() % 100) / 100;
		float b = float(rand() % 100) / 100;

		vertices[i] = Vertex(x, y, z, r, g, b, 1.0f);
	}
}