#pragma once
#include <vector>

class PhysicsCube;
class CollisionDetection
{
public:
	void addPhysicsObject(PhysicsCube* _object);
	void checkCollisions();
private:
	std::vector<PhysicsCube*> m_cubes;
};
