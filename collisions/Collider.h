#pragma once
#include <vector>

class BoxCollider;
class SphereCollider;
class Transform;
class GameObject;

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
	bool checkIntersection(Collider* col);
	virtual bool checkIntersection(BoxCollider* col) = 0;
	virtual bool checkIntersection(SphereCollider* col) = 0;
	void tickCollider();
	Transform* getTransform();
	void addCol(Collider* col);
protected:
	int searchList(std::vector<Collider*>* list, Collider* target);

	GameObject* m_game_object = nullptr;
	std::vector<Collider*> colliding_this_frame;
	std::vector<Collider*> colliding_last_frame;
};