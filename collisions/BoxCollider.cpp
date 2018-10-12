#include "BoxCollider.h"
#include <SimpleMath.h>

BoxCollider::BoxCollider(Transform* object_transform)
{
	m_transform = object_transform;
	m_speed = Vector3(0, 0, 0);
}

void BoxCollider::tick(float dt)
{
	m_speed.y -= (m_gravity * dt);
	m_transform->move((m_speed.x * dt), (m_speed.y * dt), (m_speed.z * dt));
}

void BoxCollider::setGravity(float _gravity)
{
	m_gravity = _gravity;
}

bool BoxCollider::intersect(Collider* col)
{
	Transform other_tfm = static_cast<BoxCollider*>(col)->getTransform();
	Vector3 lowest = Vector3(m_transform->getPos().x - m_transform->getScl(),
		m_transform->getPos().y - m_transform->getScl(),
		m_transform->getPos().z - m_transform->getScl());
	Vector3 highest = Vector3(m_transform->getPos().x + m_transform->getScl(),
		m_transform->getPos().y + m_transform->getScl(),
		m_transform->getPos().z + m_transform->getScl());

	Vector3 _lowest = Vector3(other_tfm.getPos().x - other_tfm.getScl(),
		other_tfm.getPos().y - other_tfm.getScl(),
		other_tfm.getPos().z - other_tfm.getScl());
	Vector3 _highest = Vector3(other_tfm.getPos().x + other_tfm.getScl(),
		other_tfm.getPos().y + other_tfm.getScl(),
		other_tfm.getPos().z + other_tfm.getScl());

	bool r_val = (lowest.x <= _highest.x && highest.x >= _lowest.x) &&
		(lowest.y <= _highest.y && highest.y >= _lowest.y) &&
		(lowest.z <= _highest.z && highest.z >= _lowest.z);
	return r_val;
}

Transform BoxCollider::getTransform()
{
	return *m_transform;
}

void BoxCollider::collide(Collider * col)
{
	if (m_speed.y < 0)
		m_speed *= -(m_bounciness * col->getBounce());
}
