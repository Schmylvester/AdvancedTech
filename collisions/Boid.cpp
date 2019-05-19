#include "Boid.h"

Boid::Boid(std::vector<Boid*>* _all_boids, Vector3 _mid_point, float _dist, bool three_dimensional)
{
	m_3d = three_dimensional;
	m_all_boids = _all_boids;
	m_mid_point = _mid_point;
	m_dist_point = _dist;
	float x = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 20)) - 10;
	float z = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 20)) - 10;
	float y = 0;
	if (m_3d)
		y = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 20)) - 10;
	m_move_acceleration = Vector3(x, y, z);
}

void Boid::update(float dt)
{
	move(dt);
	constrain();
}

void Boid::takeSnap()
{
	m_boid_snapshot = m_move_force;
}

Vector3 Boid::getSnap()
{
	return m_boid_snapshot;
}

void Boid::setMid(Vector3 _to)
{
	m_mid_point = _to;
}

Vector3 Boid::getMid() const
{
	return m_mid_point;
}

void Boid::move(float dt)
{
	if (!m_3d)
	{
		m_move_acceleration.y = 0;
	}
	m_move_acceleration.Normalize();
	m_move_force += m_move_acceleration;
	m_transform.translate(m_move_force * dt);
	m_move_acceleration = Vector3::Zero;
	alignment();
	separation();
	cohesion();
	m_move_acceleration.Normalize();
	m_move_acceleration *= 10;
}

void Boid::constrain()
{
	Vector3 pos = m_transform.getPos();
	if (Vector3::Distance(pos, m_mid_point) > m_dist_point)
	{
		Vector3 target_direction = m_mid_point - pos;
		m_move_acceleration += target_direction;
	}

	Vector3 dir = m_move_force;
	dir.Normalize();
	m_move_force = dir * m_max_speed;
}

void Boid::constrainSpeed(float & speed)
{
	if (speed > m_max_speed)
		speed = m_max_speed;
	else if (speed < -m_max_speed)
		speed = -m_max_speed;
}

bool Boid::validBoid(Boid * other)
{
	if (other != this)
	{
		Vector3 my_pos = m_transform.getPos();
		Vector3 their_pos = other->getTransform()->getPos();

		float dist = Vector3::Distance(my_pos, their_pos);
		if (dist < m_view_radius && dist > m_view_min_dist)
		{
			Vector3 direction_to_them = their_pos - my_pos;
			Vector3 facing_direction = m_move_force;
			direction_to_them.Normalize();
			facing_direction.Normalize();
			float angle_to_them = direction_to_them.Dot(facing_direction);

			if (abs(angle_to_them) < m_view_angle)
			{
				return true;
			}
		}
	}
	return false;
}

void Boid::alignment()
{
	Vector3 steering = Vector3::Zero;
	int count = 0;
	for (int i = 0; i < (*m_all_boids).size(); i++)
	{
		Boid* boid = (*m_all_boids)[i];
		if (validBoid(boid))
		{
			steering += boid->getSnap();
			count++;
		}
	}
	if (count > 0)
	{
		steering /= count;
		m_move_acceleration += (steering * alignment_force);
	}
}

void Boid::separation()
{
	Vector3 desired_direction = Vector3::Zero;
	int count = 0;
	for (int i = 0; i < (*m_all_boids).size(); i++)
	{
		Boid* boid = (*m_all_boids)[i];
		if (validBoid(boid))
		{
			Vector3 opposite_direction = m_transform.getPos() - boid->getTransform()->getPos();
			float dist = Vector3::Distance(m_transform.getPos(), boid->getTransform()->getPos());
			if (dist > 0)
				opposite_direction /= dist;
			desired_direction += opposite_direction;
			count++;
		}
	}
	if (count > 0)
	{
		desired_direction /= count;
		desired_direction -= m_move_force;
		m_move_acceleration += (desired_direction * separation_force);
	}
}

void Boid::cohesion()
{
	Vector3 direction = Vector3::Zero;
	int count = 0;
	for (int i = 0; i < (*m_all_boids).size(); i++)
	{
		Boid* boid = (*m_all_boids)[i];
		if (validBoid(boid))
		{

			direction += boid->getTransform()->getPos();
			count++;
		}
	}

	if (count > 0)
	{
		direction /= count;
		direction = -m_transform.getPos();
		direction -= m_move_force;
		m_move_acceleration += (direction * cohesion_force);
	}
}