#include "AIScene.h"
#include "Terrain.h"
#include "Player.h"
#include "AI.h"
#define _USE_MATH_DEFINES
#include <math.h>

AIScene::~AIScene()
{
	Memory::SafeDelete(map);
	Memory::SafeDelete(player);
}

void AIScene::updateScene(float dt)
{
	m_input.detectInput();
	player->update(dt);
	ai->update(dt);
}

void AIScene::drawScene(float dt)
{
	float bg_colour[4]{ 0.0f, 0.0f, 0.0f, 1.0f };
	m_device_context->ClearRenderTargetView(m_rt_view, bg_colour);
	m_device_context->ClearDepthStencilView(m_depth_stcl_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_frame_cb.light = m_light;
	m_device_context->UpdateSubresource(m_cb_per_frame, 0, NULL, &m_frame_cb, 0, 0);
	m_device_context->PSSetConstantBuffers(0, 1, &m_cb_per_frame);

	map->draw();
	player->draw();
	ai->draw();
	if (!isnan(ai->getTarget().x))
	{
		ai_target->draw();
	}

	m_swap_chain->Present(0, 0);
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

	ai->getTransform()->translate(50, 0, 50);
	ai->getTransform()->scale(0, 150, 0);

	ai_target = new GameObject();
	ai_target->init(Shape::Cube, this, &m_object_cb,
		&m_cam, m_device_context, m_cb_per_object);
	ai_target->getTransform()->translate(50, 0, 50);
	ai_target->getTransform()->scale(0, 150, 0);
	ai->addTargetObject(ai_target);
}
