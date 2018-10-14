#pragma once
#include "Transform.h"
#include <vector>

class Collider;
struct CollisionData
{
	Collider* other_collider;
	Vector3 direction;
};

class Collider
{
public:
	Collider();
	~Collider();

	virtual bool intersect(Collider* col) = 0;
	void tick(float dt);
	virtual void collide(CollisionData col);

	void setGravity(float _gravity);
	Transform getTransform();
	float getBounce();
protected:
	virtual void collisionEnter(CollisionData col);
	virtual void collisionExit(CollisionData col);
	Vector3 m_move_speed = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 m_rotate_speed = Vector3(0.0f, 0.0f, 0.0f);
	float m_bounciness = 1;
	float m_gravity = 0.2f;

	Transform* m_transform;

	std::vector<CollisionData> m_this_tick_col;
	std::vector<CollisionData> m_last_tick_col;
	bool seachList(Collider * col, std::vector<CollisionData>* list);
};

