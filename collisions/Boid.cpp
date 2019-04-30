#include "Boid.h"

Boid::Boid(std::vector<Boid*>* _all_boids)
{
	m_all_boids = _all_boids;
	m_constraints = Vector3(7, 6, 3);
	float x = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 2)) - 1;
	float y = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 2)) - 1;
	float z = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 2)) - 1;
	m_move_direction = Vector3(x, y, z);
}

void Boid::update(float dt)
{
	fly(dt);
	constrain();
}

void Boid::fly(float dt)
{
	if (m_move_speed < m_max_speed)
		m_move_speed += m_acceleration * dt;
	m_move_direction.Normalize();
	Vector3 move = m_move_direction * dt * m_move_speed;
	m_transform.translate(move);
}

void Boid::constrain()
{
	Vector3 pos = m_transform.getPos();
	constrainAxis(pos.x, m_move_direction.x, m_constraints.x);
	constrainAxis(pos.y, m_move_direction.y, m_constraints.y);
	constrainAxis(pos.z, m_move_direction.z, m_constraints.z);
	m_transform.setPosition(pos);
}

void Boid::constrainAxis(float & pos, float & direction, float & constraint)
{
	if (pos < -constraint || pos > constraint)
	{
		if (pos < -constraint)
		{
			pos = -constraint;
		}
		else
		{
			pos = constraint;
		}
		direction *= -1;
		m_move_speed /= 10;
	}
}

std::vector<Boid*> Boid::getLocalFlockmates()
{
	std::vector<Boid*> ret_vector;
	return ret_vector;
}
