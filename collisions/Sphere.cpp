#include "Sphere.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "DXApp.h"

void Sphere::init(DXApp * _app, CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff, GameObject * _owner)
{
	float radius = 1;
	int stack_count = 16;
	int slice_count = 16;
	vertex_count = 2 + ((stack_count - 1) * (slice_count + 1));
	index_count = (6 * slice_count) + (6 * (stack_count - 2) * slice_count);
	triangle_count = index_count / 3;
	float phiStep = M_PI / stack_count;
	float thetaStep = 2.0f * M_PI / slice_count;

	vertices = new Vertex[vertex_count];
	Color c = Color(
		0.5f + ((rand() % 32) / 64), 
		0.5f + ((rand() % 32) / 64), 
		0.5f + ((rand() % 32) / 64));
	vertices[0] = Vertex(0, radius, 0, c[0], c[1], c[2], c[3], 0, 1, 0);
	int idx = 1;
	for (int i = 1; i < stack_count; i++) {
		float phi = i * phiStep;
		for (int j = 0; j <= slice_count; j++) {
			float theta = j * thetaStep;
			Vector3 p = Vector3(
				(radius*sin(phi)*cos(theta)),
				(radius*cos(phi)),
				(radius* sin(phi)*sin(theta)));
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

	m_vtx_buffer = _app->getVertexBuffer("Sphere", this);
	m_idx_buffer = _app->getIndexBuffer("Sphere", this);
	Geometry::init(_app, _cb, cam, dev_con, c_buff, _owner);
}

Color Sphere::randomiseColour(Color in_colour)
{
	switch (rand() % 6)
	{
	case 0:
		return Color(in_colour[0] + 0.1f, in_colour[1], in_colour[2]);
		break;
	case 1:
		return Color(in_colour[0] - 0.1f, in_colour[1], in_colour[2]);
		break;
	case 2:
		return Color(in_colour[0], in_colour[1] + 0.1f, in_colour[2]);
		break;
	case 3:
		return Color(in_colour[0], in_colour[1] - 0.1f, in_colour[2]);
		break;
	case 4:
		return Color(in_colour[0], in_colour[1], in_colour[2] + 0.1f);
		break;
	case 5:
		return Color(in_colour[0], in_colour[1], in_colour[2] - 0.1f);
		break;
	default:
		return in_colour;
		break;
	}
}
