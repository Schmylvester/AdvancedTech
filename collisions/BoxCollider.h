#pragma once
#include "Collider.h"
#include "DXUtil.h"

class GameObject;
class SphereCollider;
class BoxCollider : public Collider
{
public:
	bool checkIntersection(Vector3 point);
	virtual bool checkIntersection(BoxCollider * col) override;
	virtual bool checkIntersection(SphereCollider * col) override;
	BoxCollider() = default;
	BoxCollider(GameObject* _game_object, Vector3 size, Vector3 offset);
	~BoxCollider() = default;
	void updateVerts();
	Vector3 getVert(int idx) { return m_current_vertices[idx]; }
	virtual void getClosestFace(Vector3 point, Vector3& position, Vector3& normal, bool& point_inside) override;

private:
	Vector3 getFaceNormal(int face);
	void setFaces(int idx, int a, int b, int c, int d);
	Vector3 quaternionByVector(Vector4 quat, Vector3 vect);

	int faces[6][4];
	Vector3 m_vertices[8];
	Vector3 m_current_vertices[8];
};

