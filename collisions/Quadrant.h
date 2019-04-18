#pragma once
#include <vector>
#include "DXUtil.h"

class Collider;
class Quadrant
{
public:
	Quadrant() = default;
	Quadrant(std::vector<Collider*>* collisionObjects);
	~Quadrant();

	void init(Quadrant* parent, std::vector<Collider*>* collisionObjects, std::vector<Collider*>* all_colliders, Vector2 _min, Vector2 _max);
	void tick(bool big_rect);
	int countColliders();
	std::vector<Collider*>* getColliders();
	void addCollider(Collider* col);
	float quadDist(Quadrant* other);
	Vector4 getRect();
	bool contains(Collider* col);
private:
	static std::vector<Quadrant*> m_quad_pool;

	void absorbSubs();
	void createSubs();
	int onList(std::vector<Collider*>* list, Collider* col);
	Quadrant* getQuadFromPool();

	int m_count = 0;
	Quadrant * m_parent_quad = nullptr;
	Quadrant* m_sub_quads[4] = { nullptr, nullptr, nullptr, nullptr };
	Vector2 m_min;
	Vector2 m_center;
	Vector2 m_max;
	std::vector<Collider*>* m_all_colliders;
	std::vector<Collider*> m_colliders;
	bool active = false;
};