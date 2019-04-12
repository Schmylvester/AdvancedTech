#include "Collider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "Geometry.h"

Collider::Collider(Geometry * _game_object)
{
	m_game_object = _game_object;
	m_object_transform = _game_object->getTransform();
}

bool Collider::checkIntersection(Collider * col)
{
	BoxCollider* b = dynamic_cast<BoxCollider*>(col);
	if (b != nullptr)
	{
		checkIntersection(b);
	}
	SphereCollider* s = dynamic_cast<SphereCollider*>(col);
	if (s != nullptr)
	{
		checkIntersection(s);
	}
}

void Collider::tickCollider()
{
	for (int i = 0; i < colliding_last_frame.size(); i++)
	{
		if (searchList(&(colliding_this_frame), colliding_last_frame[i]) == -1)
		{
			m_game_object->collision(colliding_last_frame[i], CollisionClassifier::Collision_Ended);
			colliding_last_frame.erase(colliding_last_frame.begin() + i);
		}
	}
	for (Collider* col : colliding_this_frame)
	{
		if (searchList(&(colliding_last_frame), col) == -1)
		{
			m_game_object->collision(col, CollisionClassifier::Collision_This_Frame);
			colliding_last_frame.push_back(col);
		}
		else
		{
			m_game_object->collision(col, CollisionClassifier::Ongoing_Collision);
		}
	}
	colliding_this_frame.clear();
}

int Collider::searchList(std::vector<Collider*>* list, Collider * target)
{
	int i = 0;
	for (Collider* col : *list)
	{
		if (target == col)
		{
			return i;
		}
		i++;
	}
	return -1;
}
