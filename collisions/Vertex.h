#pragma once
#include "DXUtil.h"

struct Vertex
{
	Vertex() {};
	Vertex(float x, float y, float z,
		float r, float g, float b, float a,
		float nx, float ny, float nz):
		position(x, y, z),
		colour(r, g, b, a),
		normal(nx, ny, nz) {};
	

	XMFLOAT3 position;
	XMFLOAT4 colour;
	XMFLOAT3 normal;
};