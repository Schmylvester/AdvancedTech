#pragma once
#include "GameObject.h"
#include "Pathfinder.h"

class NavMesh;
class AI : public GameObject
{
public:
	AI() = default;
	~AI() = default;
	virtual void update(float dt) override;
	void addMesh(NavMesh* _mesh);
	Vector3 getTarget();
	void addTargetObject(GameObject* target);

private:
	int path_idx = -1;
	NavMesh* m_mesh;
	Pathfinder m_pathfinder;
	std::vector<NavigationCell*> path;
	GameObject* target_object = nullptr;
	float move_speed = 75;
};