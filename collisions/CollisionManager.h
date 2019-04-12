#pragma once
#include <vector>
class Collider;
class CollisionManager
{
public:
	CollisionManager() = default;
	~CollisionManager() = default;
	void checkCollisions();
	void tickColliders();
	void addCollider(Collider* col);
private:
	std::vector<Collider*> collisionObjects;
};

