#include "CollisionsScene.h"
#include "GeometryIncludes.h"
#include "BoxCollider.h"
#include "SphereCollider.h"

CollisionsScene::~CollisionsScene()
{
}

void CollisionsScene::updateScene(float dt)
{
	m_input.detectInput();
	player->update(dt);
	for (int i = 0; i < scene_objects.size(); i++)
		scene_objects[i]->update(dt);

	m_collision_manager.checkCollisions();
	m_collision_manager.tickColliders();
}

void CollisionsScene::drawScene(float dt)
{
	player->draw();
	for (int i = 0; i < scene_objects.size(); i++)
	{
		scene_objects[i]->draw();
	}
}

void CollisionsScene::initObjects()
{
	m_cam = Camera(getRatio());
	//box colliders almost work, but don't work so I'm using spheres
	cubes(5);
	//spheres(15);
}

void CollisionsScene::spheres(int num)
{
	player = std::make_unique<Player>(&m_input);
	player->init(Shape::Sphere, this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
	player->getTransform()->translate(0, 0, -13);
	player->addCollider(new SphereCollider(player.get(), 1), &m_collision_manager, 1);

	for (int i = 0; i < num; i++)
	{
		scene_objects.push_back(std::make_unique<GameObject>());
		scene_objects.back()->init(Shape::Sphere, this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
		scene_objects.back()->getTransform()->translate(3 * (i % 4), 0, 3 * (i / 4));
		scene_objects.back()->addCollider(new SphereCollider(scene_objects.back().get(), 1), &m_collision_manager, 1);
	}
}

void CollisionsScene::cubes(int num)
{
	player = std::make_unique<Player>(&m_input);
	player->init(Shape::Cube, this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
	player->getTransform()->translate(0, 0, 0);
	player->getTransform()->rotate(XMVectorSet(0, 0, 1, 1), rand());
	player->addCollider(new BoxCollider(player.get()), &m_collision_manager, 1);

	for (int i = 0; i < num; i++)
	{
		scene_objects.push_back(std::make_unique<GameObject>());
		scene_objects.back()->init(Shape::Cube, this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
		scene_objects.back()->getTransform()->translate((rand() % 10) - 5, (rand() % 10) - 5, (rand() % 10) - 5);
		scene_objects.back()->getTransform()->rotate(XMVectorSet(1, 0, 0, 1), rand());
		scene_objects.back()->addCollider(new BoxCollider(scene_objects.back().get()), &m_collision_manager, 1);
	}
}
