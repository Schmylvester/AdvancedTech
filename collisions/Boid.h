#pragma once
#include <vector>
#include "GameObject.h"

class Boid : public GameObject
{
public:
	Boid() = default;
	Boid(std::vector<Boid*>* _all_boids);
	~Boid() = default;

	virtual void update(float dt) override;

private:
	void fly(float dt);
	void constrain();
	void constrainAxis(float& pos, float& direction, float& constraint);
	std::vector<Boid*> getLocalFlockmates();

	int m_local_count = 3;
	float m_separation;
	float m_alignment;
	float m_cohesion;

	Vector3 m_constraints;
	Vector3 m_move_direction;
	float m_move_speed = 0;
	float m_max_speed = 12;
	float m_acceleration = 6;
	std::vector<Boid*>* m_all_boids = nullptr;
};