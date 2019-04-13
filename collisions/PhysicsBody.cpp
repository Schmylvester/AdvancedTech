#include "PhysicsBody.h"
#include "Transform.h"
#include "Collider.h"
#include "GameObject.h"

PhysicsBody::PhysicsBody(Collider * _collider, Transform * _transform)
{
	m_collider = _collider;
	m_obj_transform = _transform;
}

void PhysicsBody::addForceAtPoint(float force, Vector3 collision_point, Vector3 direction)
{
	direction.Normalize();
	Vector3 face_hit = Vector3::Zero;
	Vector3 face_inv_normal = Vector3::Zero;
	bool point_in_collider = false;
	m_collider->getClosestFace(collision_point, face_hit, face_inv_normal, point_in_collider);
	if (!point_in_collider)
		return;
	float dist = (collision_point - face_hit).Length();


	float dot = collision_point.Dot(face_inv_normal);
	float mag_a = collision_point.x * collision_point.x
		+ collision_point.y * collision_point.y
		+ collision_point.z * collision_point.z;
	float mag_b = face_inv_normal.x * face_inv_normal.x
		+ face_inv_normal.y * face_inv_normal.y
		+ face_inv_normal.z * face_inv_normal.z;
	float angle = acos(dot / sqrt(mag_a * mag_b));
	float torque_force = force * dist * angle;

	Vector3 rotation_direction = direction.Cross(m_obj_transform->getPos());
	m_rotate_dir += rotation_direction;
	m_rotate_dir.Normalize();
	m_rotate_force += torque_force;

	m_move_force += direction * (force - torque_force);
}

void PhysicsBody::tick(float dt)
{
	if (m_rotate_dir != Vector3::Zero)
	{
		m_obj_transform->rotate(m_rotate_dir, m_rotate_force * dt);
		if (m_rotate_force > 0)
		{
			m_rotate_force -= dt;
		}
		else
		{
			m_rotate_force = 0;
		}
	}
	if (m_obj_transform->getPos().y > the_ground)
	{
		m_gravity_force += Vector3::Down * m_gravity * dt;
	}
	else
	{
		m_obj_transform->translate(0, the_ground - m_obj_transform->getPos().y, 0);
		m_gravity_force = Vector3::Zero;
	}
	m_obj_transform->translate(m_move_force * dt);
	m_obj_transform->translate(m_gravity_force * dt);
	Vector3 force = m_move_force;
	force.Normalize();
	m_move_force -= force * dt * 8;
}
