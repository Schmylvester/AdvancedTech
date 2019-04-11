#pragma once

class Transform;
class BoxCollider;
class SphereCollider;
class Collider
{
public:
	Collider(Transform* _transform) { m_object_transform = _transform; }
	~Collider() = default;
	virtual bool intersect(BoxCollider* col) { return false; }
	virtual bool intersect(SphereCollider* col) { return false; }
	Transform* getTransform() { return m_object_transform; }
protected:
	Transform* m_object_transform = nullptr;
};