#include "Quadrant.h"
#include "Collider.h"
#include "Transform.h"
#include <string>

std::vector<Quadrant*> Quadrant::m_quad_pool;

Quadrant::Quadrant(std::vector<Collider*>* collisionObjects)
{
	m_all_colliders = collisionObjects;
	m_min = Vector2(INFINITY, INFINITY);
	m_max = Vector2(-INFINITY, -INFINITY);
	for (Collider* col : *collisionObjects)
	{
		m_min.x = min(m_min.x, col->getTransform()->getPos().x);
		m_max.x = max(m_max.x, col->getTransform()->getPos().x) + 1;
		m_min.y = min(m_min.y, col->getTransform()->getPos().z);
		m_max.y = max(m_max.y, col->getTransform()->getPos().z) + 1;
		if (onList(&m_colliders, col) == -1)
		{
			m_colliders.push_back(col);
		}
	}
	m_center = (m_min + m_max) / 2;
	createSubs();
}

void Quadrant::init(Quadrant* parent, std::vector<Collider*>* collisionObjects, std::vector<Collider*>* all_colliders, Vector2 _min, Vector2 _max)
{
	m_all_colliders = all_colliders;
	m_parent_quad = parent;
	m_min = _min;
	m_max = _max;
	m_center = (m_min + m_max) / 2;
	for (Collider* col : *collisionObjects)
	{
		if (contains(col) && onList(&m_colliders, col) == -1)
		{
			m_colliders.push_back(col);
		}
	}

	createSubs();
}

Quadrant::~Quadrant()
{
	for (int i = 0; i < 4; i++)
	{
		if (m_sub_quads[i] != nullptr)
		{
			delete m_sub_quads[i];
			m_sub_quads[i] = nullptr;
		}
	}
}

void Quadrant::absorbSubs()
{
	for (int i = 0; i < 4; i++)
	{
		for (Collider* col : *m_sub_quads[i]->getColliders())
		{
			if (onList(&m_colliders, col) != -1)
			{
				m_colliders.push_back(col);
			}
		}

		if (m_sub_quads[i] != nullptr)
		{
			m_quad_pool.push_back(m_sub_quads[i]);
			m_sub_quads[i] = nullptr;
		}
	}
}

void Quadrant::createSubs()
{
	if (m_colliders.size() > 1)
	{
		//if two colliders are in the same place, it will go forever, move them slightly
		for (int a = 0; a < m_colliders.size(); a++)
		{
			for (int b = a + 1; b < m_colliders.size(); b++)
			{
				Transform* a_t = m_colliders[a]->getTransform();
				Transform* b_t = m_colliders[b]->getTransform();
				if (a_t->getPos() == b_t->getPos())
				{
					b_t->translate(0.01f, 0, 0);
					a = m_colliders.size();
					b = m_colliders.size();
				}
			}
		}
		m_sub_quads[0] = getQuadFromPool();
		m_sub_quads[0]->init(this, &m_colliders, m_all_colliders, m_min, m_center);
		m_sub_quads[1] = getQuadFromPool();
		m_sub_quads[1]->init(this, &m_colliders, m_all_colliders, Vector2(m_center.x, m_min.y), Vector2(m_max.x, m_center.y));
		m_sub_quads[2] = getQuadFromPool();
		m_sub_quads[2]->init(this, &m_colliders, m_all_colliders, Vector2(m_min.x, m_center.y), Vector2(m_center.x, m_max.y));
		m_sub_quads[3] = getQuadFromPool();
		m_sub_quads[3]->init(this, &m_colliders, m_all_colliders, m_center, m_max);
	}
	else if (m_colliders.size() == 1)
	{
		m_colliders[0]->setQuad(this);
	}
}

int Quadrant::onList(std::vector<Collider*>* list, Collider * col)
{
	for (int i = 0; i < list->size(); i++)
	{
		if ((*(list))[i] == col)
		{
			return i;
		}
	}
	return -1;
}

Quadrant * Quadrant::getQuadFromPool()
{
	//if there are quads in the pool, return the first one and remove it from the pool
	if (m_quad_pool.size() > 0)
	{
		Quadrant* ret = m_quad_pool[0];
		m_quad_pool.erase(m_quad_pool.begin());
		return ret;
	}
	return new Quadrant();
}

void Quadrant::tick(bool big_rect)
{
	if (big_rect)
	{
		countColliders();
		for (Collider* col : *m_all_colliders)
		{
			m_min.x = min(m_min.x, col->getTransform()->getPos().x);
			m_max.x = max(m_max.x, col->getTransform()->getPos().x) + 1;
			m_min.y = min(m_min.y, col->getTransform()->getPos().z);
			m_max.y = max(m_max.y, col->getTransform()->getPos().z) + 1;
			if (onList(&m_colliders, col) != -1)
				m_colliders.push_back(col);
		}
		m_center = (m_min + m_max) / 2;
	}
	int quad_count = 0;
	int total_count = 0;
	for (Quadrant* quad : m_sub_quads)
	{
		if (quad != nullptr)
		{
			quad_count++;
			total_count += quad->countColliders();
		}
	}
	//if there are one or fewer colliders in all my sub-quads, they don't need to exist
	if (total_count <= 1 && quad_count == 4)
	{
		absorbSubs();
	}
	//if i have no sub-quads, i need sub-quads
	if (m_count > 1 && quad_count == 0)
	{
		createSubs();
	}
	for (Quadrant* quad : m_sub_quads)
	{
		if (quad != nullptr)
		{
			quad->tick(false);
		}
	}
}

int Quadrant::countColliders()
{
	int count = 0;
	for (Collider* col : *m_all_colliders)
	{
		int idx = onList(&m_colliders, col);
		//this collider is inside my boundaries
		if (contains(col))
		{
			count++;
			//if this isn't one of my colliders, it should be
			if (idx == -1)
			{
				m_colliders.push_back(col);
				col->setQuad(this);
			}
		}
		else if (idx >= 0)
		{
			//if this is one of my colliders, it shouldn't be
			m_colliders.erase(m_colliders.begin() + idx);
		}
	}
	m_count = count;
	return count;
}

std::vector<Collider*>* Quadrant::getColliders()
{
	return &m_colliders;
}

void Quadrant::addCollider(Collider * col)
{
	if (onList(m_all_colliders, col) == -1)
	{
		m_all_colliders->push_back(col);
	}
	tick(true);
}

float Quadrant::quadDist(Quadrant * other)
{
	if (m_min.x == m_max.x)
		return 0;
	Vector4 other_rect = other->getRect();
	float min_x = min(
		abs(m_min.x - other_rect.x),
		abs(m_min.x - other_rect.z),
		abs(m_max.x - other_rect.x),
		abs(m_max.x - other_rect.z));
	float min_y = min(
		abs(m_min.y - other_rect.y),
		abs(m_min.y - other_rect.w),
		abs(m_max.y - other_rect.y),
		abs(m_max.y - other_rect.w));
	float dist = Vector2(min_x, min_y).Length();
	return dist;
}

Vector4 Quadrant::getRect()
{
	return Vector4(m_min.x, m_min.y, m_max.x, m_max.y);
}

bool Quadrant::contains(Collider * col)
{
	if (col->getTransform()->getPos().x >= m_min.x
		&& col->getTransform()->getPos().x < m_max.x
		&& col->getTransform()->getPos().z >= m_min.y
		&& col->getTransform()->getPos().z < m_max.y)
		return true;
	return false;
}
