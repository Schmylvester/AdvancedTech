#include "Collider.h"

Collider::Collider(ObjectRenderer* _renderer)
{
	m_renderer = _renderer;
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
	m_last_tick_col.clear();
	for (Collider* col : m_this_tick_col)
	{
		m_last_tick_col.push_back(col);
	}
	m_this_tick_col.clear();

	//m_move_speed.y -= (m_gravity * dt);
	m_transform->rotate('x', m_rotate_speed.x * dt);
	m_transform->rotate('y', m_rotate_speed.y * dt);
	m_transform->rotate('z', m_rotate_speed.z * dt);
	m_transform->move((m_move_speed.x * dt), (m_move_speed.y * dt), (m_move_speed.z * dt));
}

void Collider::collide(Collider* col)
{
	for (Collider* already : m_this_tick_col)
	{
		if (col == already)
			return;
	}
	m_this_tick_col.push_back(col);
}

void Collider::collisionEnter(Collider* col)
{
	m_renderer->changeColour();
	m_move_speed *= -(m_bounciness * col->getBounce());
}

void Collider::collisionExit(Collider* col)
{
}

bool Collider::seachList(Collider* col, std::vector<Collider*>* list)
{
	for (Collider* col_chk : *list)
	{
		if (col_chk == col)
		{
			return true;
		}
	}
	return false;
}