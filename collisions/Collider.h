#pragma once
#include <vector>
#include "DXUtil.h"

class Collider;
class BoxCollider;
class SphereCollider;
class Transform;
class GameObject;

struct CollisionData
{
	Collider* other_object = nullptr;
	Vector3 colision_center = Vector3::Zero;
	bool did_collide = false;
};

enum class CollisionClassifier
{
	Collision_This_Frame,
	Ongoing_Collision,
	Collision_Ended,
};

class Collider
{
public:
	Collider(GameObject* _game_object);
	~Collider() = default;
	CollisionData checkIntersection(Collider* col);
	virtual CollisionData checkIntersection(BoxCollider* col) = 0;
	virtual CollisionData checkIntersection(SphereCollider* col) = 0;
	void tickCollider();
	Transform* getTransform();
	void addCol(CollisionData col);
	virtual void getClosestFace(Vector3 point, Vector3& position, Vector3& normal);
protected:
	int searchList(std::vector<CollisionData>* list, Collider* target);

	GameObject* m_game_object = nullptr;
	std::vector<CollisionData> colliding_this_frame;
	std::vector<CollisionData> colliding_last_frame;
};