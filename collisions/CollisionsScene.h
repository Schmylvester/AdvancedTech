#pragma once
#include "Scene.h"
#include "GameObject.h"

class CollisionsScene : public Scene
{
public:
	CollisionsScene() = default;
	~CollisionsScene();

	virtual void updateScene(float dt) override;
	virtual void drawScene(float dt) override;
	virtual void initObjects() override;
private:
	std::unique_ptr<GameObject> player = nullptr;
	std::vector<std::unique_ptr<GameObject>> scene_objects;
	
	float move_speed = 7;
	bool rot_dir = true;
};