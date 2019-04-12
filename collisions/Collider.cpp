#include "Collider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "GameObject.h"

Collider::Collider(GameObject * _game_object)
{
	m_game_object = _game_object;
}

bool Collider::checkIntersection(Collider * col)
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
	return false;
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

Transform * Collider::getTransform()
{
	return m_game_object->getTransform();
}

void Collider::addCol(Collider * col)
{
	if (!searchList(&(colliding_this_frame), col) != -1)
	{
		colliding_this_frame.push_back(col);
	}
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
