#pragma once
#include "Transform.h"
#include <vector>

class Collider
{
public:
	Collider();
	~Collider();

	virtual bool intersect(Collider* col) = 0;
	void tick(float dt);
	virtual void collide(Collider* col);

	void setGravity(float _gravity);
	Transform getTransform();
	float getBounce();
protected:
	virtual void collisionEnter(Collider* col);
	virtual void collisionExit(Collider* col);
	Vector3 m_speed = Vector3(0.0f, 0.0f, 0.0f);
	float m_bounciness = 1;
	float m_gravity = 0.2f;

	Transform* m_transform;

	std::vector<Collider*> m_this_tick_col;
	std::vector<Collider*> m_last_tick_col;
	bool seachList(Collider * col, std::vector<Collider*>* list);
};

