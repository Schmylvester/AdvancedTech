#pragma once
#include <vector>
class Collider;
class CollisionManager
{
public:
	CollisionManager() = default;
	~CollisionManager() = default;
	void checkCollisions();
private:
	std::vector<Collider*> collisionObjects;
};

