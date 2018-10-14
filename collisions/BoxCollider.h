#pragma once
#include "Collider.h"
#include "Cube.h"

class BoxCollider : public Collider
{
public:
	BoxCollider(Transform* object_transform);

	virtual bool intersect(BoxCollider* col) override;
	virtual bool intersect(SphereCollider* col) override;

	virtual void collide(Collider* col) override;
private:
};

