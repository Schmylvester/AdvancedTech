#include "Geometry.h"
#include "DXUtil.h"

Geometry::~Geometry()
{
	for (Triangle* t : m_polygons)
	{
		Memory::SafeDelete(t);
	}
}

void Geometry::draw()
{
	for (Triangle* t : m_polygons)
	{
		t->draw();
	}
}
