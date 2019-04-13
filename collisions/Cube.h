#pragma once
#include "Geometry.h"

class Cube : public Geometry
{
public:
	Cube() = default;
	virtual ~Cube() = default;

	virtual void init(DXApp* _app, CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff, GameObject* _owner) override;
	void setColour(int to) { colour = to; }

private:
	int colour = 0;
};