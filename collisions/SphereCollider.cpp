#include "ColliderIncludes.h"

SphereCollider::SphereCollider(Transform* _transform, ObjectRenderer* _renderer) : Collider(_renderer)
{
	m_transform = _transform;
	radius = m_transform->getScale().x;
}

bool SphereCollider::intersect(BoxCollider * col)
{
	return col->intersect(this);
}

bool SphereCollider::intersect(SphereCollider * col)
{
	float distance = sqrt(pow(getPos().x - col->getPos().x, 2) + pow(getPos().y - col->getPos().y, 2));
	distance = sqrt(pow(distance, 2) + pow(getPos().z - col->getPos().z, 2));
	return distance < radius + col->getRadius();
}
