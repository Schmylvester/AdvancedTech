#pragma once
#include <vector>
#include "DXUtil.h"

class Collider;
class BoxCollider;
class SphereCollider;
class Transform;
class GameObject;
class Quadrant;
class CollisionManager;

enum class CollisionClassifier
{
	Collision_This_Frame,
	Ongoing_Collision,
	Collision_Ended,

	Collision,
	No_Collision,
};

struct CollisionData
{
	Collider* other_object = nullptr;
	Vector3 collision_direction = Vector3::Zero;
	float penetration = 0.0f;
	CollisionClassifier type;
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

	void objectMoved();

	void setQuad(Quadrant* quad) { m_quad = quad; }
	Quadrant* getQuad() const { return m_quad; }
	void setManager(CollisionManager* m_col_man) { m_collision_manager = m_col_man; }
	GameObject* getObject() const { return m_game_object; }

protected:
	int searchList(std::vector<CollisionData>* list, Collider* target);

	GameObject* m_game_object = nullptr;
	std::vector<CollisionData> colliding_this_frame;
	std::vector<CollisionData> colliding_last_frame;
	CollisionManager* m_collision_manager;
	Quadrant* m_quad;
};