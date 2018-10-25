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
	//find most extreme points
	Vector3 min_point, max_point;
	getMinMax(min_point, max_point);
	//build boundaries
	//for each in each boundary
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

void CollisionDetection::getMinMax(Vector3& min_val, Vector3&max_val)
{
	min_val = Vector3(_HUGE_ENUF, _HUGE_ENUF, _HUGE_ENUF);
	max_val = Vector3(-_HUGE_ENUF, -_HUGE_ENUF, -_HUGE_ENUF);
	for (Collider* col : m_colliders)
	{
		Vector3 pos = col->getTransform().getPos();
		max_val.x = max(pos.x, max_val.x);
		max_val.y = max(pos.y, max_val.y);
		max_val.z = max(pos.z, max_val.z);
		min_val.x = min(pos.x, min_val.x);
		min_val.y = min(pos.y, min_val.y);
		min_val.z = min(pos.z, min_val.z);
	}
}