#include "PhysicsCube.h"

PhysicsCube::PhysicsCube(Vector3* pos)
{
	m_position = pos;
}

void PhysicsCube::tick(float dt)
{
	m_speed.y -= (m_gravity * dt);
	(*m_position) += (m_speed * dt);
}

void PhysicsCube::setGravity(float _gravity)
{
	m_gravity = _gravity;
}

bool PhysicsCube::intersection(Vector3 other_pos)
{
	float scale = 0.05f;

	float a_left = (*m_position).x - (scale);
	float a_right = (*m_position).x + (scale);
	float a_bottom = (*m_position).y - (scale);
	float a_top = (*m_position).y + (scale);
	float a_front = (*m_position).z - (scale);
	float a_back = (*m_position).z + (scale);

	float b_left = other_pos.x - (scale);
	float b_right = other_pos.x + (scale);
	float b_bottom = other_pos.y - (scale);
	float b_top = other_pos.y + (scale);
	float b_front = other_pos.z - (scale);
	float b_back = other_pos.z + (scale);


	return (a_left <= b_right && a_right >= b_left) &&
		(a_bottom <= b_top && a_top >= b_bottom) &&
		(a_front <= b_back && a_back >= b_front);
}

Vector3 PhysicsCube::getPos()
{
	return *m_position;
}

void PhysicsCube::collide(PhysicsCube * col)
{
	m_speed *= -m_bounciness;
}
