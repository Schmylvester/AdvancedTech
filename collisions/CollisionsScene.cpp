#include "CollisionsScene.h"
#include "GeometryIncludes.h"
#include "BoxCollider.h"
#include "SphereCollider.h"

void loadTerrain(Terrain* player_loc);
void setPointers(std::vector<Geometry*>* _geometry, DXApp* _app,
	CBPerObject* _cb, ID3D11DeviceContext** _dev_con,
	ID3D11Buffer** _const_buffer, Camera* _cam);

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
	float bg_colour[4]{ 0.0f, 0.0f, 0.0f, 1.0f };
	m_device_context->ClearRenderTargetView(m_rt_view, bg_colour);
	m_device_context->ClearDepthStencilView(m_depth_stcl_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_frame_cb.light = m_light;
	m_device_context->UpdateSubresource(m_cb_per_frame, 0, NULL, &m_frame_cb, 0, 0);
	m_device_context->PSSetConstantBuffers(0, 1, &m_cb_per_frame);

	player->draw();
	for (int i = 0; i < scene_objects.size(); i++)
	{
		scene_objects[i]->draw();
	}
	m_swap_chain->Present(0, 0);
}

void CollisionsScene::initObjects()
{
	m_cam = Camera(getRatio());

	//box colliders don't work so I'm using spheres
	cubes();
	//spheres();
}

void CollisionsScene::spheres()
{
	player = std::make_unique<Player>(&m_input);
	player->init(Shape::Sphere, this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
	player->getTransform()->translate(0, 0, -13);
	player->addCollider(new SphereCollider(player.get(), 1), &m_collision_manager, true);

	for (int i = 0; i < 15; i++)
	{
		scene_objects.push_back(std::make_unique<GameObject>());
		scene_objects.back()->init(Shape::Sphere, this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
		scene_objects.back()->getTransform()->translate(3 * (i % 4), 0, 3 * (i / 4));
		scene_objects.back()->addCollider(new SphereCollider(scene_objects.back().get(), 1), &m_collision_manager, true);
	}
}

void CollisionsScene::cubes()
{
	player = std::make_unique<Player>(&m_input);
	player->init(Shape::Cube, this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
	player->getTransform()->translate(0, 0, 0);
	player->getTransform()->rotate(XMVectorSet(0, 0, 1, 1), rand());
	player->addCollider(new BoxCollider(player.get()), &m_collision_manager, true);

	for (int i = 0; i < 3; i++)
	{
		scene_objects.push_back(std::make_unique<GameObject>());
		scene_objects.back()->init(Shape::Cube, this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
		scene_objects.back()->getTransform()->translate((rand() % 10) - 5, (rand() % 10) - 5, (rand() % 10) - 5);
		scene_objects.back()->getTransform()->rotate(XMVectorSet(1, 0, 0, 1), rand());
		scene_objects.back()->addCollider(new BoxCollider(scene_objects.back().get()), &m_collision_manager, false);
	}
}
