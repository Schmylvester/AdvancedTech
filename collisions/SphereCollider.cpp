#include "SphereCollider.h"
#include "Transform.h"
#include "GameObject.h"

void SphereCollider::checkIntersection(BoxCollider * col)
{
}

void SphereCollider::checkIntersection(SphereCollider * col)
{
	Vector3 a_pos = getTransform()->getPos();
	Vector3 b_pos = col->getTransform()->getPos();
	float dist = Vector3::Distance(a_pos, b_pos);
	float collide_dist = m_radius + col->getRadius();
	if (dist < collide_dist)
	{
		if (!searchList(&(colliding_this_frame), col) != -1)
		{
			colliding_this_frame.push_back(col);
		}
	}
	else
	{
		int on_list_idx = searchList(&(colliding_this_frame), col);
		if (on_list_idx != -1)
		{
			colliding_this_frame.erase(colliding_last_frame.begin() + on_list_idx);
		}
	}
}

SphereCollider::SphereCollider(GameObject * _game_object, float radius) : Collider(_game_object)
{
	m_radius = radius;
}
