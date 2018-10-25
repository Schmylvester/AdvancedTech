#include "Triangle.h"
#include "DXApp.h"
#include "TriangleLoader.h"

Triangle::~Triangle()
{
}

void Triangle::initPipeline(DXApp* app, DirectX::XMMATRIX* _world_matrix, TriangleLoader* loader)
{
	m_world_matrix = _world_matrix;
	m_triangle_loader = loader;
	m_app = app;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex) * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	auto device = app->getDevice();
	device->CreateBuffer(&bd, NULL, &m_vtx_buffer);

	auto context = app->getContext();
	D3D11_MAPPED_SUBRESOURCE ms;
	context->Map(m_vtx_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, m_vertices, sizeof(m_vertices));
	context->Unmap(m_vtx_buffer, NULL);

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	device->CreateInputLayout(ied, 2, loader->VS->GetBufferPointer(), loader->VS->GetBufferSize(), &m_input_layout);
	context->IASetInputLayout(m_input_layout);
}

void Triangle::draw()
{
	auto context = m_app->getContext();
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &m_vtx_buffer, &stride, &offset);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->Draw(3, 0);
}

void Triangle::initVertices(Vertex _vertices[3])
{
	m_vertices[0] = _vertices[0];
	m_vertices[1] = _vertices[1];
	m_vertices[2] = _vertices[2];

	D3D11_MAPPED_SUBRESOURCE ms;
	m_app->getContext()->Map(m_vtx_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, m_vertices, sizeof(m_vertices));
	m_app->getContext()->Unmap(m_vtx_buffer, NULL);
}

void Triangle::setVertices(Vertex _vertices[3])
{
	Vertex render_vert[3];
	for (int i = 0; i < 3; i++)
	{
		render_vert[i] = {
			m_vertices[i].X,
			m_vertices[i].Y,
			m_vertices[i].Z,
			_vertices[i].R,
			_vertices[i].G,
			_vertices[i].B,
			_vertices[i].A
		};
	}

	D3D11_MAPPED_SUBRESOURCE ms;
	m_app->getContext()->Map(m_vtx_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, render_vert, sizeof(render_vert));
	m_app->getContext()->Unmap(m_vtx_buffer, NULL);
}