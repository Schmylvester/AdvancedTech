#pragma once
#include "DXApp.h"
#include "CollisionDetection.h"

class Cube;
class Application : public DXApp
{
public:
	Application(HINSTANCE h_instance, Debug* _debug);
	~Application();

	bool init() override;
	void update(float dt) override;
	void render(float dt) override;
private:
	CollisionDetection m_collision_detection;
	Cube* m_fall_cube;
	Cube* m_static_cube;
};

