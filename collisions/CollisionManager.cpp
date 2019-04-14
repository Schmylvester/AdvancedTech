#include "CollisionManager.h"
#include "Collider.h"
void CollisionManager::checkCollisions()
{
	//acceleration structure
	for (int i = 0; i < collisionObjects.size(); i++)
	{
		for (int j = i + 1; j < collisionObjects.size(); j++)
		{
			CollisionData col_data = collisionObjects[i]->checkIntersection(collisionObjects[j]);
			if (col_data.did_collide)
			{
				col_data = collisionObjects[i]->checkIntersection(collisionObjects[j]);
				if (col_data.did_collide)
				{
					collisionObjects[j]->addCol(col_data);
					col_data.other_object = collisionObjects[j];
					collisionObjects[i]->addCol(col_data);
				}
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
