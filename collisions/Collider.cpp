#include "Collider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "GameObject.h"
#include "CollisionManager.h"

Collider::Collider(GameObject * _game_object)
{
	m_game_object = _game_object;
}

CollisionData Collider::checkIntersection(Collider * col)
{
	BoxCollider* b = dynamic_cast<BoxCollider*>(col);
	if (b != nullptr)
	{
		return checkIntersection(b);
	}
	SphereCollider* s = dynamic_cast<SphereCollider*>(col);
	if (s != nullptr)
	{
		return checkIntersection(s);
	}
	CollisionData ret_false;
	ret_false.did_collide = false;
	return ret_false;
}

void Collider::tickCollider()
{
	for (int i = 0; i < colliding_last_frame.size(); i++)
	{
		if (searchList(&(colliding_this_frame), colliding_last_frame[i].other_object) == -1)
		{
			colliding_last_frame[i].type = CollisionClassifier::Collision_Ended;
			m_game_object->collision(colliding_last_frame[i]);
			colliding_last_frame.erase(colliding_last_frame.begin() + i);
		}
	}
	for (CollisionData col : colliding_this_frame)
	{
		if (searchList(&(colliding_last_frame), col.other_object) == -1)
		{
			col.type = CollisionClassifier::Collision_This_Frame;
			m_game_object->collision(col);
			colliding_last_frame.push_back(col);
		}
		else
		{
			col.type = CollisionClassifier::Ongoing_Collision;
			m_game_object->collision(col);
		}
	}
	colliding_this_frame.clear();
}

Transform * Collider::getTransform()
{
	return m_game_object->getTransform();
}

void Collider::addCol(CollisionData col)
{
	int list_idx = searchList(&(colliding_this_frame), col.other_object);
	if (list_idx != -1)
	{
		colliding_this_frame.erase(colliding_this_frame.begin() + list_idx);
	}
	colliding_this_frame.push_back(col);
}

void Collider::getClosestFace(Vector3 point, Vector3& position, Vector3& normal)
{
}

void Collider::objectMoved()
{
	m_collision_manager->objectMoved(this);
}

int Collider::searchList(std::vector<CollisionData>* list, Collider * target)
{
	int i = 0;
	for (CollisionData col : *list)
	{
		if (target == col.other_object)
		{
			return i;
		}
		i++;
	}
	return -1;
}
