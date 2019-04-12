#include "CollisionManager.h"
#include "Collider.h"
void CollisionManager::checkCollisions()
{
	//acceleration structure
	for (int i = 0; i < collisionObjects.size(); i++)
	{
		for (int j = i + 1; j < collisionObjects.size(); j++)
		{
			collisionObjects[i]->checkIntersection(collisionObjects[j]);
		}
	}
}