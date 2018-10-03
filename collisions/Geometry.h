#pragma once
#include "Triangle.h"
#include <vector>

class Transform
{
public:
	Transform() = default;
	~Transform() = default;

	float position[3];
	float rotation[3];
	float scale[3];
};

class Geometry
{
public:
	Geometry() = default;
	virtual ~Geometry();

	void draw();
protected:
	Transform m_transform;
	std::vector<Triangle*> m_polygons;
};

