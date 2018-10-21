#pragma once
#include <DirectXMath.h>
#include "Geometry.h"
#include "Cube.h"

class TriangleLoader;
class ObjectRenderer :
	public Geometry
{
public:
	ObjectRenderer() = default;
	ObjectRenderer(DXApp * app, float colour[4], DirectX::XMMATRIX* _world_matrix, TriangleLoader* loader);
	~ObjectRenderer();
	void changeColour();
	void updateVertices(int i, float x, float y, float z);
	void setTriangleValues(int triangle_index, int a, int b, int c, bool init);
private:
	void createTriangle(DirectX::XMMATRIX* _world_matrix, TriangleLoader* loader);
	Vertex m_vertices[8];
	DXApp* m_app;
	float m_colour[4];
};

