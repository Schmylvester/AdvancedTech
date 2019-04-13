#include "CollisionsScene.h"
#include "GeometryIncludes.h"
#include "BoxCollider.h"

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

	player = std::make_unique<Player>(&m_input);
	player->init(Shape::Cube, this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
	player->getTransform()->translate(0, 5.1f, 0);
	player->addCollider(new BoxCollider(player.get(), Vector3::One, Vector3::Zero), &m_collision_manager, true);
	
	scene_objects.push_back(std::make_unique<GameObject>());
	scene_objects.back()->init(Shape::Cube, this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
	scene_objects.back()->addCollider(new BoxCollider(scene_objects.back().get(), Vector3::One, Vector3::Zero), &m_collision_manager, false);
}