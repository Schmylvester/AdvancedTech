#include "BoidScene.h"
#include "Boid.h"

BoidScene::~BoidScene()
{
	for (Boid* boid : boids)
	{
		if (boid != nullptr)
		{
			delete boid;
			boid = nullptr;
		}
	}
	boids.clear();
}

void BoidScene::updateScene(float dt)
{
	for (Boid* boid : boids)
	{
		if (boid != nullptr)
		{
			boid->update(dt);
		}
	}
}

void BoidScene::initObjects()
{
	m_cam = Camera(getRatio());
	m_cam.setPos(0, 0, -35);
	for (int i = 0; i < boid_count; i++)
	{
		boids.push_back(new Boid(&boids));
		boids.back()->init(Shape::Pyramid, this, &m_object_cb,
			&m_cam, m_device_context, m_cb_per_object);
		boids.back()->getTransform()->translate(
			(static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 50)) - 25, 
			(static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 50)) - 25, 
			(static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 50)) - 25);
	}
}

void BoidScene::drawObjects(float dt)
{
	for (Boid* boid : boids)
	{
		if (boid != nullptr)
		{
			boid->draw();
		}
	}
}
