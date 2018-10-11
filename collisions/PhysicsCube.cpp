#include "PhysicsCube.h"

PhysicsCube::PhysicsCube(Transform* object_transform)
{
	m_transform = object_transform;
	m_speed = Vector3(0, 0, 0);
}

void PhysicsCube::tick(float dt)
{
	m_speed.y -= (m_gravity * dt);
	m_transform->setPos(m_transform->getPos() + (m_speed * dt));
}

void PhysicsCube::setGravity(float _gravity)
{
	m_gravity = _gravity;
}

bool PhysicsCube::AABBintersection(Transform other_tfm)
{
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

Transform PhysicsCube::getTransform()
{
	return *m_transform;
}

void PhysicsCube::collide(PhysicsCube * col)
{
	if(m_speed.y < 0)
		m_speed *= -(m_bounciness * col->getBounce());
}

float PhysicsCube::getBounce()
{
	return m_bounciness;
}
