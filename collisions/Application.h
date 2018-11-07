#pragma once
#include "DXApp.h"
#include "CollisionDetection.h"

class Cube;
class Application : public DXApp
{
public:
	Application() = default;
	~Application();

	bool init(HINSTANCE h_instance) override;
private:
	void initObjects();
	CollisionDetection m_collision_detection;
	std::vector<Cube*> m_gameobjects;
	bool play = false;
};

