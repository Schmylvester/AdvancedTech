#pragma once
#include "Light.h"

struct CBPerObject
{
	XMMATRIX WVP;
	XMMATRIX World;
};

struct CBPerFrame
{
	Light light;
};