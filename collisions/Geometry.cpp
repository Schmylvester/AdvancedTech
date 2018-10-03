#include "Geometry.h"
#include "DXUtil.h"

Geometry::~Geometry()
{
	clearTriangles();
}

void Geometry::draw()
{
	for (Triangle* t : m_polygons)
	{
		t->draw();
	}
}

void Geometry::clearTriangles()
{
	for (Triangle* t : m_polygons)
	{
		Memory::SafeDelete(t);
	}
}
