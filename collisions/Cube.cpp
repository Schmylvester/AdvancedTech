#include "Cube.h"
#include "DXApp.h"
#include "Camera.h"

void Cube::init(DXApp* _app, CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff)
{
	triangle_count = 12;
	index_count = triangle_count * 3;
	vertex_count = 24;

	indices = new DWORD[index_count]
	{
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};

	vertices = new Vertex[vertex_count];
	float r, g, b;
	r = float(rand() % 100) / 100;
	g = float(rand() % 100) / 100;
	b = float(rand() % 100) / 100;
	// Front Face
	vertices[0] = Vertex(-1.0f, -1.0f, -1.0f, r, g, b, 1.0f, -1.0f, -1.0f, -1.0f);
	vertices[1] = Vertex(-1.0f, 1.0f, -1.0f, r, g, b, 1.0f, -1.0f, 1.0f, -1.0f);
	vertices[2] = Vertex(1.0f, 1.0f, -1.0f, r, g, b, 1.0f, 1.0f, 1.0f, -1.0f);
	vertices[3] = Vertex(1.0f, -1.0f, -1.0f, r, g, b, 1.0f, 1.0f, -1.0f, -1.0f);

	r = 1.0f - r;
	// Back Face
	vertices[4] = Vertex(-1.0f, -1.0f, 1.0f, r, g, b, 1.0f, -1.0f, -1.0f, 1.0f);
	vertices[5] = Vertex(1.0f, -1.0f, 1.0f, r, g, b, 1.0f, 1.0f, -1.0f, 1.0f);
	vertices[6] = Vertex(1.0f, 1.0f, 1.0f, r, g, b, 1.0f, 1.0f, 1.0f, 1.0f);
	vertices[7] = Vertex(-1.0f, 1.0f, 1.0f, r, g, b, 1.0f, -1.0f, 1.0f, 1.0f);

	g = 1.0f - g;
	// Top Face
	vertices[8] = Vertex(-1.0f, 1.0f, -1.0f, r, g, b, 1.0f, -1.0f, 1.0f, -1.0f);
	vertices[9] = Vertex(-1.0f, 1.0f, 1.0f, r, g, b, 1.0f, -1.0f, 1.0f, 1.0f);
	vertices[10] = Vertex(1.0f, 1.0f, 1.0f, r, g, b, 1.0f, 1.0f, 1.0f, 1.0f);
	vertices[11] = Vertex(1.0f, 1.0f, -1.0f, r, g, b, 1.0f, 1.0f, 1.0f, -1.0f);

	b = 1.0f - b;
	// Bottom Face
	vertices[12] = Vertex(-1.0f, -1.0f, -1.0f, r, g, b, 1.0f, -1.0f, -1.0f, -1.0f);
	vertices[13] = Vertex(1.0f, -1.0f, -1.0f, r, g, b, 1.0f, 1.0f, -1.0f, -1.0f);
	vertices[14] = Vertex(1.0f, -1.0f, 1.0f, r, g, b, 1.0f, 1.0f, -1.0f, 1.0f);
	vertices[15] = Vertex(-1.0f, -1.0f, 1.0f, r, g, b, 1.0f, -1.0f, -1.0f, 1.0f);

	r = 1.0f - r;
	// Left Face
	vertices[16] = Vertex(-1.0f, -1.0f, 1.0f, r, g, b, 1.0f, -1.0f, -1.0f, 1.0f);
	vertices[17] = Vertex(-1.0f, 1.0f, 1.0f, r, g, b, 1.0f, -1.0f, 1.0f, 1.0f);
	vertices[18] = Vertex(-1.0f, 1.0f, -1.0f, r, g, b, 1.0f, -1.0f, 1.0f, -1.0f);
	vertices[19] = Vertex(-1.0f, -1.0f, -1.0f, r, g, b, 1.0f, -1.0f, -1.0f, -1.0f);

	g = 1.0f - g;
	// Right Face
	vertices[20] = Vertex(1.0f, -1.0f, -1.0f, r, g, b, 1.0f, 1.0f, -1.0f, -1.0f);
	vertices[21] = Vertex(1.0f, 1.0f, -1.0f, r, g, b, 1.0f, 1.0f, 1.0f, -1.0f);
	vertices[22] = Vertex(1.0f, 1.0f, 1.0f, r, g, b, 1.0f, 1.0f, 1.0f, 1.0f);
	vertices[23] = Vertex(1.0f, -1.0f, 1.0f, r, g, b, 1.0f, 1.0f, -1.0f, 1.0f);

	m_vtx_buffer = _app->getVertexBuffer(GeometryID::CUBE, this);
	m_idx_buffer = _app->getIndexBuffer(GeometryID::CUBE, this);

	Geometry::init(_app, _cb, cam, dev_con, c_buff);
}