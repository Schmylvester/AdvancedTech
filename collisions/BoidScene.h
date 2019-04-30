#pragma once
#include "DXApp.h"

class Boid;
class BoidScene : public DXApp
{
public:
	BoidScene() = default;
	~BoidScene();

	virtual void updateScene(float dt) override;
	virtual void initObjects() override;
	virtual void drawObjects(float dt) override;

private:
	int boid_count = 6;
	std::vector<Boid*> boids;
};