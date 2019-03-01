#pragma once
#include "DXUtil.h"

class Light
{
public:
	Light();

	XMFLOAT3 dir;
	float pad;
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;

private:
	float x, y, z;
	float rotation = 0;
};