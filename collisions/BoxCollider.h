#pragma once
#include "Collider.h"
#include "DXUtil.h"

class Transform;
class SphereCollider;
class BoxCollider : public Collider
{
public:
	virtual bool intersect(BoxCollider * col) override;
	virtual bool intersect(SphereCollider * col) override;
	BoxCollider() = default;
	BoxCollider(Transform* _transform, Vector3 size, Vector3 offset);
	~BoxCollider() = default;
	void updateVerts();
	Vector3 getVert(int idx) { return m_current_vertices[idx]; }

private:
	Vector3 getFaceNormal(int face);
	void setFaces(int idx, int a, int b, int c, int d);
	Vector3 quaternionByVector(Vector4 quat, Vector3 vect);

	int faces[6][4];
	Vector3 m_vertices[8];
	Vector3 m_current_vertices[8];
};

