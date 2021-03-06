#pragma once
#include "DXUtil.h"

class Collider;
class Transform;
class PhysicsBody
{
public:
	PhysicsBody() = delete;
	PhysicsBody(Collider* _collider, Transform* _transform);
	~PhysicsBody() = default;

	void addForceAtPoint(float force, Vector3 collision_point, Vector3 dir);
	void addForceAtBoxPoint(float force, Vector3 collision_point, Vector3 dir);
	void tick(float dt);
private:
	Collider* m_collider = nullptr;
	Transform* m_obj_transform = nullptr;
	Vector3 m_move_force;
	Vector3 m_rotate_dir;
	float m_rotate_force;
};

