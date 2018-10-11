#include "CollisionDetection.h"
#include "PhysicsCube.h"

void CollisionDetection::addPhysicsObject(PhysicsCube * _object)
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
				if (m_cubes[i]->AABBintersection(m_cubes[j]->getTransform()))
				{
					m_cubes[i]->collide(m_cubes[j]);
					m_cubes[j]->collide(m_cubes[i]);
				}
			}
		}
	}
}
