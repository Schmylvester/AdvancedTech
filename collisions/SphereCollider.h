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

	float getRadius();
	Vector3 getPos();
private:
	float radius;
};

