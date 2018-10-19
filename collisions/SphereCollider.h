#pragma once
#include "Collider.h"
class SphereCollider :
	public Collider
{
public:
	SphereCollider(Transform * _transform, RenderCube * _renderer);
	~SphereCollider() = default;
	virtual bool intersect(BoxCollider* col) override;
	virtual bool intersect(SphereCollider* col) override;

	float getRadius() { return radius; }
	Vector3 getPos() { return m_transform->getPos(); }
private:
	float radius;
};