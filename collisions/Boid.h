#pragma once
#include <vector>
#include "GameObject.h"
#include "Transform.h"

class Boid : public GameObject
{
public:
	Boid() = default;
	Boid(std::vector<Boid*>* _all_boids, Vector3 _mid_point, float _dist, bool three_dimensional);
	~Boid() = default;

	virtual void update(float dt) override;

	void takeSnap();
	Vector3 getSnap();

	void setMid(Vector3 _to);
	Vector3 getMid() const;
private:
	void move(float dt);
	void constrain();
	void constrainSpeed(float& speed);

	bool validBoid(Boid* other);
	float alignment_force = 1.8f;
	void alignment();
	float separation_force = 3.8f;
	void separation();
	float cohesion_force = 1.0f;
	void cohesion();

	bool m_3d = false;

	Vector3 m_mid_point;
	float m_dist_point;
	Vector3 m_move_force;
	Vector3 m_move_acceleration;
	float m_view_radius = 10;
	float m_view_min_dist = 6;
	float m_view_angle = 0.3f;
	float m_max_speed = 6;
	std::vector<Boid*>* m_all_boids = nullptr;
	Vector3 m_boid_snapshot;
};