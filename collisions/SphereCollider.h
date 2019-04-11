#pragma once
#include "Collider.h"

class Transform;
class BoxCollider;
class SphereCollider : public Collider
{
public:
	virtual bool intersect(BoxCollider * col) override;
	virtual bool intersect(SphereCollider * col) override;
	SphereCollider(Transform* _transform, float radius);
	SphereCollider() = default;
	~SphereCollider() = default;

	float getRadius() { return m_radius; }
protected:
	float m_radius;
};