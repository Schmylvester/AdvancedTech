#pragma once
#include "Cube.h"

class PhysicsCube
{
public:
	PhysicsCube(Transform* object_transform);

	void tick(float dt);
	void setGravity(float _gravity);
	bool AABBintersection(Transform other_tfm);

	Transform getTransform();
	
	void collide(PhysicsCube* col);
	float getBounce();
private:
	Vector3 m_speed = Vector3(0.0f, 0.0f, 0.0f);
	float m_bounciness = 0.8f;
	float m_gravity = 0.02f;

	Transform* m_transform;
};

