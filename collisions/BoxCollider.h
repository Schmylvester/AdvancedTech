#pragma once
#include "Collider.h"
#include "Cube.h"

class BoxCollider : public Collider
{
public:
	BoxCollider(Transform* object_transform);

	void tick(float dt);
	void setGravity(float _gravity);
	virtual bool intersect(Collider* col) override;
	virtual void collide(Collider* col) override;

	Transform getTransform();
private:
};

