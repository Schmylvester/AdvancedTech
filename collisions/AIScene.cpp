#include "AIScene.h"
#include "Terrain.h"
#include "Player.h"
#include "AI.h"
#define _USE_MATH_DEFINES
#include <math.h>

AIScene::~AIScene()
{
	if (map)
	{
		delete map;
		map = nullptr;
	}
	if (player)
	{
		delete player;
		player = nullptr;
	}
}

void AIScene::updateScene(float dt)
{
	m_input.detectInput();
	player->update(dt);
	ai->update(dt);
}

void AIScene::drawObjects(float dt)
{
	map->draw();
	player->draw();
	ai->draw();
	if (!isnan(ai->getTarget().x))
	{
		ai_target->draw();
	}
}

void AIScene::initObjects()
{
	m_cam = Camera(getRatio(), true);

	map = new Terrain();
	map->init("..\\Resources\\Maze.bmp", 0, 0, this,
		&m_object_cb, &m_cam, m_device_context, m_cb_per_frame);
	NavMesh* mesh = map->addNavMesh();

	player = new Player(&m_input);
	player->init(Shape::Cube, this, &m_object_cb,
		&m_cam, m_device_context, m_cb_per_object);
	player->getTransform()->translate(240, 90, -360);

	ai = new AI();
	ai->init(Shape::Cube, this, &m_object_cb,
		&m_cam, m_device_context, m_cb_per_object);
	ai->addMesh(mesh);

	ai->getTransform()->translate(50, 0, 40);
	ai->getTransform()->scale(0, 150, 0);

	ai_target = new GameObject();
	ai_target->init(Shape::Cube, this, &m_object_cb,
		&m_cam, m_device_context, m_cb_per_object);
	ai_target->getTransform()->translate(50, 0, 40);
	ai_target->getTransform()->scale(0, 150, 0);
	ai->addTargetObject(ai_target);
}
