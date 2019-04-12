#include "CollisionManager.h"
#include "Collider.h"
void CollisionManager::checkCollisions()
{
	//acceleration structure
	for (int i = 0; i < collisionObjects.size(); i++)
	{
		for (int j = i + 1; j < collisionObjects.size(); j++)
		{
			if (collisionObjects[i]->checkIntersection(collisionObjects[j]))
			{
				collisionObjects[j]->addCol(collisionObjects[i]);
			}
		}
	}
}

void CollisionManager::tickColliders()
{
	for (Collider* col : collisionObjects)
	{
		col->tickCollider();
	}
}

void CollisionManager::addCollider(Collider * _col)
{
	for (Collider* col : collisionObjects)
	{
		if (col == _col)
			return;
	}
	collisionObjects.push_back(_col);
}
