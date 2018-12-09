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
	std::string vertex_tag = "Cube";
	switch (colour)
	{
	case 0:
		r = 0.8f;
		b = 0.3f;
		g = 0.3f;
		vertex_tag = "Red Cube";
		break;
	case 1:
		r = 0.3f;
		b = 0.3f;
		g = 0.8f;
		vertex_tag = "Green Cube";
		break;
	case 2:
		r = 0.3f;
		b = 0.8f;
		g = 0.3f;
		vertex_tag = "Blue Cube";
		break;
	case 3:
		r = 0.8f;
		b = 0.8f;
		g = 0.3f;
		vertex_tag = "Magenta Cube";
		break;
	case 4:
		r = 0.8f;
		b = 0.3f;
		g = 0.8f;
		vertex_tag = "Yellow Cube";
		break;
	case 5:
		r = 0.3f;
		b = 0.8f;
		g = 0.8f;
		vertex_tag = "Cyan Cube";
		break;
	default:
		break;
	}

	// Front Face
	vertices[0] = Vertex(-1.0f, -1.0f, -1.0f, r * 1.2f, g * 1.2f, b * 1.2f, 1.0f, -1.0f, -1.0f, -1.0f);
	vertices[1] = Vertex(-1.0f, 1.0f, -1.0f, r * 1.2f, g * 1.2f, b * 1.2f, 1.0f, -1.0f, 1.0f, -1.0f);
	vertices[2] = Vertex(1.0f, 1.0f, -1.0f, r * 1.2f, g * 1.2f, b * 1.2f, 1.0f, 1.0f, 1.0f, -1.0f);
	vertices[3] = Vertex(1.0f, -1.0f, -1.0f, r * 1.2f, g * 1.2f, b * 1.2f, 1.0f, 1.0f, -1.0f, -1.0f);

	// Back Face
	vertices[4] = Vertex(-1.0f, -1.0f, 1.0f, r * 1.2f, g, b, 1.0f, -1.0f, -1.0f, 1.0f);
	vertices[5] = Vertex(1.0f, -1.0f, 1.0f, r * 1.2f, g, b, 1.0f, 1.0f, -1.0f, 1.0f);
	vertices[6] = Vertex(1.0f, 1.0f, 1.0f, r * 1.2f, g, b, 1.0f, 1.0f, 1.0f, 1.0f);
	vertices[7] = Vertex(-1.0f, 1.0f, 1.0f, r * 1.2f, g, b, 1.0f, -1.0f, 1.0f, 1.0f);

	// Top Face
	vertices[8] = Vertex(-1.0f, 1.0f, -1.0f, r, g * 1.2f, b, 1.0f, -1.0f, 1.0f, -1.0f);
	vertices[9] = Vertex(-1.0f, 1.0f, 1.0f, r, g * 1.2f, b, 1.0f, -1.0f, 1.0f, 1.0f);
	vertices[10] = Vertex(1.0f, 1.0f, 1.0f, r, g * 1.2f, b, 1.0f, 1.0f, 1.0f, 1.0f);
	vertices[11] = Vertex(1.0f, 1.0f, -1.0f, r, g * 1.2f, b, 1.0f, 1.0f, 1.0f, -1.0f);

	// Bottom Face
	vertices[12] = Vertex(-1.0f, -1.0f, -1.0f, r, g, b * 1.2f, 1.0f, -1.0f, -1.0f, -1.0f);
	vertices[13] = Vertex(1.0f, -1.0f, -1.0f, r, g, b * 1.2f, 1.0f, 1.0f, -1.0f, -1.0f);
	vertices[14] = Vertex(1.0f, -1.0f, 1.0f, r, g, b * 1.2f, 1.0f, 1.0f, -1.0f, 1.0f);
	vertices[15] = Vertex(-1.0f, -1.0f, 1.0f, r, g, b * 1.2f, 1.0f, -1.0f, -1.0f, 1.0f);

	// Left Face
	vertices[16] = Vertex(-1.0f, -1.0f, 1.0f, r * 0.8f, g* 0.8f, b* 0.8f, 1.0f, -1.0f, -1.0f, 1.0f);
	vertices[17] = Vertex(-1.0f, 1.0f, 1.0f, r* 0.8f, g* 0.8f, b* 0.8f, 1.0f, -1.0f, 1.0f, 1.0f);
	vertices[18] = Vertex(-1.0f, 1.0f, -1.0f, r* 0.8f, g* 0.8f, b* 0.8f, 1.0f, -1.0f, 1.0f, -1.0f);
	vertices[19] = Vertex(-1.0f, -1.0f, -1.0f, r* 0.8f, g* 0.8f, b* 0.8f, 1.0f, -1.0f, -1.0f, -1.0f);

	// Right Face
	vertices[20] = Vertex(1.0f, -1.0f, -1.0f, r, g, b, 1.0f, 1.0f, -1.0f, -1.0f);
	vertices[21] = Vertex(1.0f, 1.0f, -1.0f, r, g, b, 1.0f, 1.0f, 1.0f, -1.0f);
	vertices[22] = Vertex(1.0f, 1.0f, 1.0f, r, g, b, 1.0f, 1.0f, 1.0f, 1.0f);
	vertices[23] = Vertex(1.0f, -1.0f, 1.0f, r, g, b, 1.0f, 1.0f, -1.0f, 1.0f);

	m_vtx_buffer = _app->getVertexBuffer(vertex_tag, this);
	m_idx_buffer = _app->getIndexBuffer("Cube", this);

	Geometry::init(_app, _cb, cam, dev_con, c_buff);
}