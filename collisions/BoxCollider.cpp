#include "BoxCollider.h"
#include <SimpleMath.h>

BoxCollider::BoxCollider(Transform* object_transform)
{
	m_transform = object_transform;
}

bool BoxCollider::intersect(BoxCollider* col)
{
	Transform other_tfm = col->getTransform();
	Vector3 lowest = Vector3(m_transform->getPos().x - m_transform->getScl().x,
		m_transform->getPos().y - m_transform->getScl().y,
		m_transform->getPos().z - m_transform->getScl().z);
	Vector3 highest = Vector3(m_transform->getPos().x + m_transform->getScl().x,
		m_transform->getPos().y + m_transform->getScl().y,
		m_transform->getPos().z + m_transform->getScl().z);

	Vector3 _lowest = Vector3(other_tfm.getPos().x - other_tfm.getScl().x,
		other_tfm.getPos().y - other_tfm.getScl().y,
		other_tfm.getPos().z - other_tfm.getScl().z);
	Vector3 _highest = Vector3(other_tfm.getPos().x + other_tfm.getScl().x,
		other_tfm.getPos().y + other_tfm.getScl().y,
		other_tfm.getPos().z + other_tfm.getScl().z);

	bool r_val = (lowest.x <= _highest.x && highest.x >= _lowest.x) &&
		(lowest.y <= _highest.y && highest.y >= _lowest.y) &&
		(lowest.z <= _highest.z && highest.z >= _lowest.z);
	return r_val;
}

bool BoxCollider::intersect(SphereCollider * col)
{
	return false;
}

void BoxCollider::collide(Collider* col)
{
	Collider::collide(col);
}