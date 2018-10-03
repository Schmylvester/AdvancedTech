#pragma once
#include "Triangle.h"
#include <vector>

class Geometry
{
public:
	Geometry() = default;
	virtual ~Geometry();

	void draw();
protected:
	void clearTriangles();
	std::vector<Triangle*> m_polygons;
};

