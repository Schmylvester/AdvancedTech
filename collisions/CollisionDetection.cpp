#include "CollisionDetection.h"
#include "Collider.h"

void CollisionDetection::addPhysicsObject(Collider * _object)
{
	m_cubes.push_back(_object);
}

void CollisionDetection::checkCollisions()
{
	for (int i = 0; i < m_cubes.size(); i++)
	{
		for (int j = i + 1; j < m_cubes.size(); j++)
		{
			if (m_cubes[i] && m_cubes[j])
			{
				if (m_cubes[i]->intersect(m_cubes[j]))
				{
					m_cubes[i]->collide(m_cubes[j]);
					m_cubes[j]->collide(m_cubes[i]);
				}
			}
		}
	}
}
