#pragma once
#include "Collider.h"

class Geometry;
class BoxCollider;
class SphereCollider : public Collider
{
public:
	virtual CollisionData checkIntersection(BoxCollider * col) override;
	virtual CollisionData checkIntersection(SphereCollider * col) override;
	SphereCollider(GameObject* _game_object, float radius);
	SphereCollider() = default;
	~SphereCollider() = default;

	float getRadius() { return m_radius; }
protected:
	float m_radius;
};