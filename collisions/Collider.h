#pragma once
#include "Transform.h"

class Collider
{
public:
	Collider();
	~Collider();

	virtual bool intersect(Collider* col) = 0;
	virtual void collide(Collider* col) = 0;

	float getBounce();
protected:
	Vector3 m_speed = Vector3(0.0f, 0.0f, 0.0f);
	float m_bounciness = 0.8f;
	float m_gravity = 0.2f;

	Transform* m_transform;
};

