#include "Geometry.h"

Geometry::~Geometry()
{
	Memory::SafeDeleteArr(indices);
	Memory::SafeDeleteArr(vertices);
}
