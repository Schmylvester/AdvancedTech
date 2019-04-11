#pragma once
#include <vector>
class Collider;
class CollisionManager
{
public:
	CollisionManager() = default;
	~CollisionManager() = default;
	bool checkCollision(Collider* a, Collider* b);
private:
	std::vector<Collider*> collisionObjects;
};

