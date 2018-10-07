#pragma once
#include <DirectXMath.h>
#include "Geometry.h"
#include "Cube.h"
class RenderCube :
	public Geometry
{
public:
	RenderCube() = default;
	RenderCube(DXApp * app, float colour[4]);
	~RenderCube();
	void updateVertices(int i, float x, float y, float z);
	void setTriangleValues(int triangle_index, int a, int b, int c);
private:
	void createTriangle();
	Vertex m_vertices[8];
	DXApp* m_app;
	float m_colour[4];
};

