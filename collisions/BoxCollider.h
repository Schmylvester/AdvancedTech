#pragma once
#include "Collider.h"
#include "Cube.h"

class BoxCollider : public Collider
{
public:
	BoxCollider(Transform* object_transform);

	virtual bool intersect(Collider* col) override;
	virtual void collide(CollisionData col) override;
private:
};

