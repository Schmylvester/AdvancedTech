#include "SphereCollider.h"
#include "Transform.h"
#include "GameObject.h"

CollisionData SphereCollider::checkIntersection(BoxCollider * col)
{
	CollisionData ret_false;
	ret_false.type = CollisionClassifier::No_Collision;
	return ret_false;
}

CollisionData SphereCollider::checkIntersection(SphereCollider * col)
{
	Vector3 a_pos = getTransform()->getPos();
	Vector3 b_pos = col->getTransform()->getPos();
	float dist = Vector3::Distance(a_pos, b_pos);
	float collide_dist = m_radius + col->getRadius();
	if (dist < collide_dist)
	{
		CollisionData return_data;
		return_data.type = CollisionClassifier::Collision;
		return_data.collision_direction = b_pos - a_pos;
		return_data.penetration = (collide_dist - dist);
		return_data.other_object = col;
		return return_data;
	}
	else
	{
		int on_list_idx = searchList(&(colliding_this_frame), col);
		if (on_list_idx != -1)
		{
			colliding_this_frame.erase(colliding_last_frame.begin() + on_list_idx);
		}
		CollisionData ret_false;
		ret_false.type = CollisionClassifier::No_Collision;
		return ret_false;
	}
}

SphereCollider::SphereCollider(GameObject * _game_object, float radius) : Collider(_game_object)
{
	m_radius = radius;
}
