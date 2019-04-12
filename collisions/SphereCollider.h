#pragma once
#include "Collider.h"

class Geometry;
class BoxCollider;
class SphereCollider : public Collider
{
public:
	virtual bool checkIntersection(BoxCollider * col) override;
	virtual bool checkIntersection(SphereCollider * col) override;
	SphereCollider(GameObject* _game_object, float radius);
	SphereCollider() = default;
	~SphereCollider() = default;

	float getRadius() { return m_radius; }
protected:
	float m_radius;
};