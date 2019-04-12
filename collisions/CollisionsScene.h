#pragma once
#include "Scene.h"
#include "Terrain.h"

class CollisionsScene : public Scene
{
public:
	CollisionsScene() = default;
	~CollisionsScene();

	virtual void updateScene(float dt) override;
	virtual void drawScene(float dt) override;
	virtual void initObjects() override;
private:
	std::unique_ptr<Geometry> player = nullptr;
	std::vector<std::unique_ptr <Geometry>> scene_objects;
	
	float move_speed = 7;
	bool rot_dir = true;
};