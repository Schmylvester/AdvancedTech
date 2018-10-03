#include "PhysicsCube.h"



PhysicsCube::PhysicsCube()
{
}


PhysicsCube::~PhysicsCube()
{
}

void PhysicsCube::tick(Vector3 & pos, Vector3 rot, float dt)
{
	pos.y -= m_gravity * dt;
}