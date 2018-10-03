#pragma once
#include "DXUtil.h"

struct Vertex
{
	FLOAT X, Y, Z;
	FLOAT R, G, B, A;
};

class DXApp;
class Triangle
{
public:
	Triangle() = default;
	~Triangle();

	void initPipeline(DXApp* app);
	void draw();
	void setVertices(Vertex vertices[3]);
private:
	DXApp* m_app;
	ID3D11VertexShader* m_vtx_shader;
	ID3D11PixelShader* m_pxl_shader;
	ID3D11Buffer* m_vtx_buffer;
	ID3D11InputLayout* m_input_layout;

	Vertex m_vertices[3];
};