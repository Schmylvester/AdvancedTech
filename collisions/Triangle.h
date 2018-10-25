#pragma once
#include "DXUtil.h"

struct Vertex
{
	FLOAT X, Y, Z;
	FLOAT R, G, B, A;
};

class DXApp;
class TriangleLoader;
class Triangle
{
public:
	Triangle() = default;
	~Triangle();

	void initPipeline(DXApp* app, DirectX::XMMATRIX* _world_matrix, TriangleLoader* loader);
	void draw();
	void initVertices(Vertex _vertices[3]);
	void setVertices(Vertex _vertices[3]);
private:
	DXApp * m_app;
	ID3D11Buffer* m_vtx_buffer;
	ID3D11InputLayout* m_input_layout;

	DirectX::XMMATRIX* m_world_matrix;

	TriangleLoader* m_triangle_loader;

	Vertex m_vertices[3];
};