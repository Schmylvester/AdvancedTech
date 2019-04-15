#pragma once
#include "Collider.h"
#include "DXUtil.h"

class GameObject;
class SphereCollider;

class BoxCollider : public Collider
{
public:
	bool checkIntersection(Vector3 point);
	virtual CollisionData checkIntersection(BoxCollider * col) override;
	virtual CollisionData checkIntersection(SphereCollider * col) override;
	BoxCollider() = default;
	BoxCollider(GameObject* _game_object);
	~BoxCollider() = default;
	void updateVerts();
	Vector3 getVert(int idx) { return m_current_vertices[idx]; }
	virtual void getClosestFace(Vector3 point, Vector3& position, Vector3& normal) override;

private:
	bool checkProjectionIntersect(Vector3 a_pos, Vector3 b_pos, Vector3 a_size, Vector3 b_size, Vector3 a_x_proj, Vector3 a_y_proj, Vector3 a_z_proj, Vector3 b_x_proj, Vector3 b_y_proj, Vector3 b_z_proj, Vector3 projection);
	bool checkProjectionIntersect(Vector3 projection, BoxCollider * other, float* a_projected, float* b_projected);
	Vector3 getFaceNormal(int face);
	void setFaces(int idx, int a, int b, int c, int d);
	Vector3 rotatePoint(Vector3 a, Vector4 b);

	int faces[6][4];
	Vector3 m_vertices[8];
	Vector3 m_current_vertices[8];
};

