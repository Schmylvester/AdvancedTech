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
	Vector3 getDirection();

private:
	void fly(float dt);
	void constrain();
	void constrainAxis(float& pos, float& direction, float& constraint);
	void constrainSpeed(float& speed);

	bool validBoid(Boid* other);
	float alignment_force = 1.0f;
	void alignment();
	float separation_force = 14.0f;
	void separation();
	float cohesion_force = 14.0f;
	void cohesion();

	float m_acceleration = 50;
	Vector3 m_constraints;
	Vector3 m_move_force;
	Vector3 m_move_acceleration;
	float m_view_radius = 1;
	float m_max_speed = 6;
	std::vector<Boid*>* m_all_boids = nullptr;
};