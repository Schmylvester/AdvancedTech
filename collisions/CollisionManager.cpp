#include "CollisionManager.h"
#include "Collider.h"

CollisionManager::CollisionManager()
{
	m_quad_tree = Quadrant(&collisionObjects);
}

void CollisionManager::checkCollisions()
{
	m_quad_tree.countColliders();
	for (int i = 0; i < collisionObjects.size(); i++)
	{
		for (int j = i + 1; j < collisionObjects.size(); j++)
		{
			//checks the quad tree to see what they know
			if (collisionObjects[i]->getQuad()->quadDist(collisionObjects[j]->getQuad()) < 5)
			{
				CollisionData col_data = collisionObjects[i]->checkIntersection(collisionObjects[j]);
				if (col_data.did_collide)
				{
					collisionObjects[j]->addCol(col_data);
					col_data.other_object = collisionObjects[j];
					col_data.collision_direction *= -1;
					collisionObjects[i]->addCol(col_data);
					objectMoved(collisionObjects[i]);
					objectMoved(collisionObjects[j]);
				}
			}
		}
	}
}

void CollisionManager::objectMoved(Collider* object)
{
	if (!object->getQuad()->contains(object))
	{
		m_quad_tree.tick(true);
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
	_col->setManager(this);
	collisionObjects.push_back(_col);
	m_quad_tree.addCollider(_col);
}
