#pragma once
#include "Cube.h"

class PhysicsCube
{
public:
	PhysicsCube(Vector3* pos);

	void tick(float dt);
	void setGravity(float _gravity);
	bool intersection(Vector3 other_pos);

	Vector3 getPos();
	void collide(PhysicsCube* col);
private:
	Vector3 m_speed = Vector3(0.0f, 0.0f, 0.0f);
	float m_bounciness = 0.8f;
	float m_gravity = 0.2f;

	Vector3* m_position;
};

