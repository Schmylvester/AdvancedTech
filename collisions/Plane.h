#pragma once
#include "Geometry.h"

class PlaneObject : public Geometry
{
public:
	PlaneObject() = default;
	~PlaneObject() = default;

	virtual void init(DXApp* _app, CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff) override;
};

