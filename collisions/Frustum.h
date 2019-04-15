#pragma once
#include "Geometry.h"

class Frustum : public Geometry
{
public:
	Frustum() = default;
	virtual ~Frustum() = default;

	virtual void init(DXApp* _app, CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff, GameObject* _owner) override;
};