#pragma once
#include <vector>
#include "DXUtil.h"

class Collider;
class CollisionDetection
{
public:
	void addPhysicsObject(Collider* _object);
	void checkCollisions();
private:
	void getMinMax(Vector3 & min_val, Vector3 & max_val);
	std::vector<Collider*> m_colliders;
};
