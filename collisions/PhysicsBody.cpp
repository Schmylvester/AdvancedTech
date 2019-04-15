#include "PhysicsBody.h"
#include "Transform.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "GameObject.h"

PhysicsBody::PhysicsBody(Collider * _collider, Transform * _transform)
{
	m_collider = _collider;
	m_obj_transform = _transform;
}

void PhysicsBody::addForceAtPoint(float force, Vector3 collision_point, Vector3 direction)
{
	//this does not work yet
	return;
	direction.Normalize();
	if (dynamic_cast<BoxCollider*>(m_collider))
	{
		addForceAtBoxPoint(force, collision_point, direction);
	}
	else if (dynamic_cast<SphereCollider*>(m_collider))
	{
		//distance from point of collision to its nearest face
		float dist = (collision_point - m_obj_transform->getPos()).Length();
		//lever arm angle of the force
		Vector3 norm = collision_point - m_obj_transform->getPos();
		norm.Normalize();
		float dot = collision_point.Dot(norm);
		float mag_a = collision_point.x * collision_point.x
			+ collision_point.y * collision_point.y
			+ collision_point.z * collision_point.z;
		float mag_b = norm.x * norm.x
			+ norm.y * norm.y
			+ norm.z * norm.z;
		float angle = acos(dot / sqrt(mag_a * mag_b));
		//rotational force
		float torque_force = force * dist * angle;
		if (isnan(torque_force))
			torque_force = 0.0f;

		//get rotation direction
		Vector3 rotation_direction = direction.Cross(m_obj_transform->getPos());
		m_rotate_dir += rotation_direction;
		m_rotate_dir.Normalize();
		m_rotate_force += torque_force;

		//the rest of the force goes into moving the object
		m_move_force += direction * (force - torque_force);
	}
}

void PhysicsBody::addForceAtBoxPoint(float force, Vector3 collision_point, Vector3 dir)
{
	Vector3 face_hit = Vector3::Zero;
	Vector3 face_inv_normal = Vector3::Zero;
	m_collider->getClosestFace(collision_point, face_hit, face_inv_normal);

	//distance from point of collision to its nearest face
	float dist = (collision_point - face_hit).Length();

	//lever arm angle of the force
	float dot = collision_point.Dot(face_inv_normal);
	float mag_a = collision_point.x * collision_point.x
		+ collision_point.y * collision_point.y
		+ collision_point.z * collision_point.z;
	float mag_b = face_inv_normal.x * face_inv_normal.x
		+ face_inv_normal.y * face_inv_normal.y
		+ face_inv_normal.z * face_inv_normal.z;
	float angle = acos(dot / sqrt(mag_a * mag_b));
	//rotational force
	float torque_force = force * dist * angle;
	if (isnan(torque_force))
		torque_force = 0.0f;

	//get rotation direction
	Vector3 rotation_direction = dir.Cross(m_obj_transform->getPos());
	m_rotate_dir += rotation_direction;
	m_rotate_dir.Normalize();
	m_rotate_force += torque_force;

	//the rest of the force goes into moving the object
	m_move_force += dir * (force - torque_force);
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
	
	m_obj_transform->translate(m_move_force * dt);
	Vector3 force = m_move_force;
	force.Normalize();
	m_move_force -= force * dt * 8;
}
