#include "Geometry.h"
#include "ConstantBuffers.h"
#include "Camera.h"
#include "DXApp.h"

Geometry::~Geometry()
{
	Memory::SafeDeleteArr(indices);
	Memory::SafeDeleteArr(vertices);
}

void Geometry::init(DXApp * _app, CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff)
{
	m_vtx_buffer = _app->getVertexBuffer(this);
	m_idx_buffer = _app->getIndexBuffer(this);
	m_cb = _cb;
	m_cam = cam;
	m_dev_con = dev_con;
	m_c_buff = c_buff;
}

void Geometry::draw()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_dev_con->IASetVertexBuffers(0, 1, &m_vtx_buffer, &stride, &offset);

	m_dev_con->IASetIndexBuffer(m_idx_buffer, DXGI_FORMAT_R32_UINT, 0);
	m_cb->WVP = XMMatrixTranspose(m_cam->getWVPMatrix(m_transform.getWorldMatrix()));
	m_dev_con->UpdateSubresource(m_c_buff, 0, NULL, m_cb, 0, 0);
	m_dev_con->VSSetConstantBuffers(0, 1, &m_c_buff);

	m_dev_con->DrawIndexed(index_count, 0, 0);
}