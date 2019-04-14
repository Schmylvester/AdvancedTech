#pragma once
#include "DXUtil.h"
#include <vector>

class Collider;
class Quadrant
{
public:
	Quadrant() = default;
	~Quadrant();

	void setUp(std::vector<Collider*>* collisionObjects);
	void setUp(std::vector<Collider*>* collisionObjects, Vector2 _min, Vector2 _max);
	
private:
	Quadrant* m_sub_quads[4];
	Vector2 m_min;
	Vector2 m_center;
	Vector2 m_max;
	std::vector<Collider*> m_colliders;
	bool active = false;
};

