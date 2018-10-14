#pragma once
#include "Transform.h"
#include "RenderCube.h"
#include <vector>

class Collider;
class BoxCollider;
class SphereCollider;

class Collider
{
public:
	Collider() = default;
	Collider(RenderCube * _renderer);
	~Collider();

	virtual bool intersect(BoxCollider* col) = 0;
	virtual bool intersect(SphereCollider* col) = 0;
	void tick(float dt);
	virtual void collide(Collider* col);

	void setGravity(float _gravity);
	Transform getTransform();
	float getBounce();
protected:
	virtual void collisionEnter(Collider* col);
	virtual void collisionExit(Collider* col);
	Vector3 m_move_speed = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 m_rotate_speed = Vector3(0.0f, 0.0f, 0.0f);
	float m_bounciness = 0.9f;
	float m_gravity = 0.2f;

	Transform* m_transform;

	std::vector<Collider*> m_this_tick_col;
	std::vector<Collider*> m_last_tick_col;
	bool seachList(Collider * col, std::vector<Collider*>* list);

	RenderCube* m_renderer;
};

