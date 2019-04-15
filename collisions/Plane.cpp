#include "Plane.h"
#include "DXApp.h"
#include "Camera.h"

void PlaneObject::init(DXApp * _app, CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff, GameObject* _owner)
{
	triangle_count = 2;
	index_count = triangle_count * 3;
	vertex_count = 4;

	indices = new DWORD[index_count]
	{
		0, 1, 2,
		0, 2, 3,
	};

	vertices = new Vertex[vertex_count];
	vertices[0] = Vertex(-1.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f);
	vertices[1] = Vertex(-1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f);
	vertices[2] = Vertex(1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f);
	vertices[3] = Vertex(1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f);
	

	m_vtx_buffer = _app->getVertexBuffer("Plane", this);
	m_idx_buffer = _app->getIndexBuffer("Plane", this);

	Geometry::init(_app, _cb, cam, dev_con, c_buff, _owner);
}
