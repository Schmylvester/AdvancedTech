#pragma once
#include "DXUtil.h"

struct Vertex
{
	Vertex(float x, float y, float z,
		float r, float g, float b, float a):
		position(x, y, z),
		colour(r, g, b, a) {};
	

	XMFLOAT3 position;
	XMFLOAT4 colour;
};