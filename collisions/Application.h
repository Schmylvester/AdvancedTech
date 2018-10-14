#pragma once
#include "DXApp.h"
#include "CollisionDetection.h"

class Cube;
class Application : public DXApp
{
public:
	Application(HINSTANCE h_instance);
	~Application();

	bool init() override;
	void update(float dt) override;
	void render(float dt) override;
private:
	void initObjects();
	CollisionDetection m_collision_detection;
	std::vector<Cube*> m_gameobjects;
	bool play = false;
};

