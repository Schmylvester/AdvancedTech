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
	BoxCollider(Transform* _transform);
	~BoxCollider() = default;
	void updateVert(int idx);
	Vector3 getVert(int idx) { return m_current_vertices[idx]; }

private:
	Vector4 toQuaternion(Vector3 euler);
	Vector3 getFaceNormal(int face);
	void setFaces(int idx, int a, int b, int c, int d);

	int faces[6][4];
	Vector3 m_vertices[8];
	Vector3 m_current_vertices[8];
};

