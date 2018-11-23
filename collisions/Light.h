#pragma once
#include "DXUtil.h"

class Light
{
public:
	Light();

	void update(float dt);

	XMFLOAT3 dir;
	float pad;
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;

private:
	float x, y, z;
	int swing_dir = 0;
};