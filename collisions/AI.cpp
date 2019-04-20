#include "AI.h"
#include "NavMesh.h"
#include "NavigationCell.h"

void AI::update(float dt)
{
	if (m_mesh->initialised)
	{
		Vector3 pos = m_transform.getPos();
		if (path_idx == -1)
		{
			if (rand() % 100 == 0)
			{
				NavigationCell* closest = m_mesh->getClosestCell(pos);
				path = m_pathfinder.findpath(closest, m_mesh->getRandomCell(closest->getWorldPos().y - 0.1f, closest->getWorldPos().y + 0.1f));
				if (path.size() > 0)
				{
					path_idx = 0;
					target_object->getTransform()->setPosition(path[path.size() - 1]->getWorldPos());
				}
			}
		}
		else
		{
			Vector3 move_dir = path[path_idx]->getWorldPos() - pos;
			move_dir.Normalize();
			m_transform.translate(move_dir * move_speed * dt);
			if (Vector3::Distance(path[path_idx]->getWorldPos(), pos) < 0.3f)
			{
				if (++path_idx >= path.size())
				{
					path.clear();
					path_idx = -1;
				}
			}
		}
	}
}

void AI::addMesh(NavMesh * _mesh)
{
	m_mesh = _mesh;
}

Vector3 AI::getTarget()
{
	if(path.size() == 0)
	{
		return Vector3::One * INFINITY;
	}
	return path[path.size() - 1]->getWorldPos();
}

void AI::addTargetObject(GameObject * target)
{
	target_object = target;
}
