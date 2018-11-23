#include "Pyramid.h"
#include "DXApp.h"
#include "Camera.h"

void Pyramid::init(DXApp * _app, CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff)
{
	triangle_count = 6;
	index_count = triangle_count * 3;
	vertex_count = 16;

	indices = new DWORD[index_count]
	{
		// Front Face
		0,  1,  2,

		// Right Face
		3, 4, 5,

		// Back Face
		6, 7, 8,

		// Left Face
		9, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,
	};

	vertices = new Vertex[vertex_count];
	vertices[0] = Vertex(0.0f, 1.3f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	vertices[1] = Vertex(1.0f, -0.5f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	vertices[2] = Vertex(-1.0f, -0.5f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);

	vertices[3] = Vertex(0.0f, 1.3f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	vertices[4] = Vertex(1.0f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	vertices[5] = Vertex(1.0f, -0.5f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);

	vertices[6] = Vertex(0.0f, 1.3f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	vertices[7] = Vertex(-1.0f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	vertices[8] = Vertex(1.0f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);

	vertices[9] = Vertex(0.0f, 1.3f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	vertices[10] = Vertex(-1.0f, -0.5f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	vertices[11] = Vertex(-1.0f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);

	vertices[12] = Vertex(-1.0f, -0.5f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	vertices[13] = Vertex(1.0f, -0.5f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	vertices[14] = Vertex(1.0f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	vertices[15] = Vertex(-1.0f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);

	m_vtx_buffer = _app->getVertexBuffer(GeometryID::PYRAMID, this);
	m_idx_buffer = _app->getIndexBuffer(GeometryID::PYRAMID, this);

	Geometry::init(_app, _cb, cam, dev_con, c_buff);
}
