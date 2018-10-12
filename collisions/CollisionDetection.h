#pragma once
#include <vector>

class Collider;
class CollisionDetection
{
public:
	void addPhysicsObject(Collider* _object);
	void checkCollisions();
private:
	std::vector<Collider*> m_cubes;
};
