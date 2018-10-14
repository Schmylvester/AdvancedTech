#include "CollisionDetection.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"

void CollisionDetection::addPhysicsObject(Collider * _object)
{
	m_colliders.push_back(_object);
}

void CollisionDetection::checkCollisions()
{
	for (int i = 0; i < m_colliders.size(); i++)
	{
		for (int j = i + 1; j < m_colliders.size(); j++)
		{
			if (m_colliders[i] && m_colliders[j])
			{
				bool collide = false;
				BoxCollider* box;
				if (box = dynamic_cast<BoxCollider*>(m_colliders[j]))
				{
					collide = m_colliders[i]->intersect(box);
				}
				SphereCollider* sphere;
				if (sphere = dynamic_cast<SphereCollider*>(m_colliders[j]))
				{
					collide = m_colliders[i]->intersect(sphere);
				}

				if (collide)
				{
					m_colliders[j]->collide(m_colliders[i]);
					m_colliders[i]->collide(m_colliders[j]);
				}
			}
		}
	}
}
