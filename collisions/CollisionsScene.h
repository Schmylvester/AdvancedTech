#pragma once
#include "Scene.h"
#include "Player.h"
#include "GameObject.h"
#include "CollisionManager.h"

class CollisionsScene : public Scene
{
public:
	CollisionsScene() = default;
	~CollisionsScene();

	virtual void updateScene(float dt) override;
	virtual void drawScene(float dt) override;
	virtual void initObjects() override;
private:
	void spheres();
	void cubes();

	std::unique_ptr<Player> player = nullptr;
	std::vector<std::unique_ptr<GameObject>> scene_objects;
	
	CollisionManager m_collision_manager;
};