#include "Quadrant.h"
#include "Collider.h"

Quadrant::~Quadrant()
{
	for (int i = 0; i < 4; i++)
	{
		Memory::SafeDelete(m_sub_quads[i]);
	}
}

void Quadrant::setUp(std::vector<Collider*>* collisionObjects)
{
}

void Quadrant::setUp(std::vector<Collider*>* collisionObjects, Vector2 _min, Vector2 _max)
{
}
