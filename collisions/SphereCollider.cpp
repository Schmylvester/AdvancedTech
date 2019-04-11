#include "SphereCollider.h"
#include "Transform.h"

bool SphereCollider::intersect(BoxCollider * col)
{
	return false;
}

bool SphereCollider::intersect(SphereCollider * col)
{
	Vector3 a_pos = m_object_transform->getPos();
	Vector3 b_pos = col->getTransform()->getPos();
	float dist = Vector3::Distance(a_pos, b_pos);
	float collide_dist = m_radius + col->getRadius();
	return dist < collide_dist;
}

SphereCollider::SphereCollider(Transform * _transform, float radius) : Collider(_transform)
{
	m_radius = radius;
}
