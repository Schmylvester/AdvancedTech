#pragma once
#include <vector>
#include <memory>
#include "Quadrant.h"

class Collider;
class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager() = default;
	void checkCollisions();
	void objectMoved(Collider* col);
	void tickColliders();
	void addCollider(Collider* col);
private:
	std::vector<Collider*> collisionObjects;
	Quadrant m_quad_tree;
};