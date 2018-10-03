#pragma once
#include "Geometry.h"
class Cube :
	public Geometry
{
public:
	Cube() = default;
	Cube(DXApp * app, Transform transform);
	~Cube();

private:
};

