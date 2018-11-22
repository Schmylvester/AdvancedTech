#include "Geometry.h"
#include "ConstantBuffers.h"
#include "Camera.h"

Geometry::~Geometry()
{
	Memory::SafeDeleteArr(indices);
	Memory::SafeDeleteArr(vertices);
}

void Geometry::draw(CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff)
{
	_cb->WVP = XMMatrixTranspose(cam->getWVPMatrix(m_transform.getWorldMatrix()));
	dev_con->UpdateSubresource(c_buff, 0, NULL, _cb, 0, 0);
	dev_con->VSSetConstantBuffers(0, 1, &c_buff);

	dev_con->DrawIndexed(index_count, 0, 0);
}