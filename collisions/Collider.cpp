#include "Collider.h"

Collider::Collider()
{
}

Collider::~Collider()
{
}

void Collider::tick(float dt)
{
	for (Collider* lt_col : m_last_tick_col)
	{
		if (!seachList(lt_col, &m_this_tick_col))
		{
			collisionExit(lt_col);
		}
	}
	for (Collider* tt_col : m_this_tick_col)
	{
		if (!seachList(tt_col, &m_last_tick_col))
		{
			collisionEnter(tt_col);
		}
	}

	m_speed.y -= (m_gravity * dt);
	m_transform->move((m_speed.x * dt), (m_speed.y * dt), (m_speed.z * dt));

	m_last_tick_col = m_this_tick_col;
	m_this_tick_col.clear();
}

void Collider::collide(Collider * col)
{
	m_this_tick_col.push_back(col);
}

void Collider::collisionEnter(Collider * col)
{
	m_speed *= -(m_bounciness * col->getBounce());
}

void Collider::collisionExit(Collider * col)
{
}

float Collider::getBounce()
{
	return m_bounciness;
}

void Collider::setGravity(float _gravity)
{
	m_gravity = _gravity;
}

Transform Collider::getTransform()
{
	return *m_transform;
}

bool Collider::seachList(Collider* col, std::vector<Collider*>* list)
{
	for (Collider* col_chk : *list)
	{
		if (col_chk == col)
		{
			return false;
		}
	}
	return true;
}