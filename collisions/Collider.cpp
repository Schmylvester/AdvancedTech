#include "Collider.h"

Collider::Collider()
{
	float numbers[5] = {-0.3f, -0.1f, 0, 0.1f, 0.3f };
	m_move_speed.x = numbers[rand() % 5];
	m_move_speed.y = numbers[rand() % 5];
	m_rotate_speed.x = numbers[rand() % 5];
	m_rotate_speed.y = numbers[rand() % 5];
	m_rotate_speed.z = numbers[rand() % 5];
}

Collider::~Collider()
{
}

void Collider::tick(float dt)
{
	for (CollisionData lt_col : m_last_tick_col)
	{
		if (!seachList(lt_col.other_collider, &m_this_tick_col))
		{
			collisionExit(lt_col);
		}
	}
	for (CollisionData tt_col : m_this_tick_col)
	{
		if (!seachList(tt_col.other_collider, &m_last_tick_col))
		{
			collisionEnter(tt_col);
		}
	}
	m_last_tick_col.clear();
	for (CollisionData col : m_this_tick_col)
	{
		m_last_tick_col.push_back(col);
	}
	m_this_tick_col.clear();

	//m_move_speed.y -= (m_gravity * dt);
	m_transform->move((m_move_speed.x * dt), (m_move_speed.y * dt), (m_move_speed.z * dt));
	m_transform->rotate('x', m_rotate_speed.x * dt);
	m_transform->rotate('y', m_rotate_speed.y * dt);
	m_transform->rotate('z', m_rotate_speed.z * dt);
}

void Collider::collide(CollisionData col)
{
	m_this_tick_col.push_back(col);
}

void Collider::collisionEnter(CollisionData col)
{
	m_move_speed *= -1;
}

void Collider::collisionExit(CollisionData col)
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

bool Collider::seachList(Collider* col, std::vector<CollisionData>* list)
{
	for (CollisionData col_chk : *list)
	{
		if (col_chk.other_collider == col)
		{
			return false;
		}
	}
	return true;
}