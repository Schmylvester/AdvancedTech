#include "Cube.h"

void Cube::init()
{
	triangle_count = 12;
	index_count = triangle_count * 3;
	vertex_count = 8;

	indices = new DWORD[index_count]{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};
	vertices = new Vertex[vertex_count];

	for (int i = 0; i < vertex_count; i++)
	{
		float x = ((i / 2) % 2) == 0 ? -1 : 1;
		float y = (((i + 1) / 2) % 2 == 0) ? -1 : 1;
		float z = (i < 4) ? -1 : 1;

		float r = float(rand() % 100) / 100;
		float g = float(rand() % 100) / 100;
		float b = float(rand() % 100) / 100;

		vertices[i] = Vertex(x, y, z, r, g, b, 1.0f);
	}
}