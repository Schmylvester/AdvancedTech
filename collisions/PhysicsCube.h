#pragma once
#include "Cube.h"

class PhysicsCube
{
public:
	PhysicsCube();
	~PhysicsCube();

	void tick(Vector3& pos, Vector3 rot, float dt);
private:
	float m_gravity = 0.01f;
};

