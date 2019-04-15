#include "Sphere.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "DXApp.h"

void Sphere::init(DXApp * _app, CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff, GameObject * _owner)
{
	float radius = 1;
	int stack_count = 12;
	int slice_count = 12;
	vertex_count = 2 + ((stack_count - 1) * (slice_count + 1));
	index_count = (6 * slice_count) + (6 * (stack_count - 2) * slice_count);
	triangle_count = index_count / 3;
	float phi_step = M_PI / stack_count;
	float theta_step = 2.0f * M_PI / slice_count;

	vertices = new Vertex[vertex_count];
	float c_r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float c_g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 1.0f - c_r);
	float c_b = 1 - (c_r + c_g);
	Color c = Color(c_r, c_g, c_b);
	vertices[0] = Vertex(0, radius, 0, c[0], c[1], c[2], c[3], 0, 1, 0);
	int idx = 1;
	for (int i = 1; i < stack_count; i++) {
		float phi = i * phi_step;
		for (int j = 0; j <= slice_count; j++) {
			float theta = j * theta_step;
			Vector3 p = Vector3(
				(radius *sin(phi) * cos(theta)),
				(radius *cos(phi)),
				(radius * sin(phi) * sin(theta)));
			Vector3 n = p;
			n.Normalize();
			c = randomiseColour(c);
			vertices[idx] = Vertex(p.x, p.y, p.z, c[0], c[1], c[2], c[3], n.x, n.y, n.z);
			idx++;
		}
	}
	vertices[idx] = Vertex(0, -radius, 0, 0, 1, 0, 1, 0, -1, 0);

	indices = new DWORD[index_count];
	idx = 0;
	for (int i = 1; i <= slice_count; i++)
	{
		indices[idx++] = 0;
		indices[idx++] = i + 1;
		indices[idx++] = i;
	}
	int baseIndex = 1;
	int ringVertexCount = slice_count + 1;
	for (int i = 0; i < stack_count - 2; i++) {
		for (int j = 0; j < slice_count; j++) {
			indices[idx++] = (baseIndex + i * ringVertexCount + j);
			indices[idx++] = (baseIndex + i * ringVertexCount + j + 1);
			indices[idx++] = (baseIndex + (i + 1)*ringVertexCount + j);

			indices[idx++] = (baseIndex + (i + 1)*ringVertexCount + j);
			indices[idx++] = (baseIndex + i * ringVertexCount + j + 1);
			indices[idx++] = (baseIndex + (i + 1)*ringVertexCount + j + 1);
		}
	}
	int southPoleIndex = vertex_count - 1;
	baseIndex = southPoleIndex - ringVertexCount;
	for (int i = 0; i < slice_count; i++) {
		indices[idx++] = (southPoleIndex);
		indices[idx++] = (baseIndex + i);
		indices[idx++] = (baseIndex + i + 1);
	}
	std::string id = std::to_string(rand());
	m_vtx_buffer = _app->getVertexBuffer(id, this);
	m_idx_buffer = _app->getIndexBuffer("Sphere", this);
	Geometry::init(_app, _cb, cam, dev_con, c_buff, _owner);
}

Color Sphere::randomiseColour(Color in_colour)
{
	if (rand() % 60 > 0)
		return in_colour;
	float r;
	float g;
	float b;

	r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 1.0f - r);
	b = 1 - (r + g);
	return Color(r, g, b);
}
