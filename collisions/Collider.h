#pragma once
#include <vector>

class Geometry;
class Transform;
class Collider;
class BoxCollider;
class SphereCollider;

enum class CollisionClassifier
{
	Collision_This_Frame,
	Ongoing_Collision,
	Collision_Ended,
};

class Collider
{
public:
	Collider(Geometry* _game_object);
	~Collider() = default;
	bool checkIntersection(Collider* col);
	virtual void checkIntersection(BoxCollider* col) = 0;
	virtual void checkIntersection(SphereCollider* col) = 0;
	Transform* getTransform() { return m_object_transform; }
	void tickCollider();
protected:
	int searchList(std::vector<Collider*>* list, Collider* target);

	Geometry* m_game_object = nullptr;
	Transform* m_object_transform = nullptr;
	std::vector<Collider*> colliding_this_frame;
	std::vector<Collider*> colliding_last_frame;
};