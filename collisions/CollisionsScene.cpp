#include "CollisionsScene.h"
#include "GeometryIncludes.h"

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
	float step = dt * move_speed;

	if (m_input.key(DIK_W, InputState::HELD))
	{
		player->getTransform()->translate(0, 0, step);
		m_cam.move(0, 0, step);
	}
	if (m_input.key(DIK_A, InputState::HELD))
	{
		player->getTransform()->translate(-step, 0, 0);
		m_cam.move(-step, 0, 0);
	}
	if (m_input.key(DIK_S, InputState::HELD))
	{
		player->getTransform()->translate(0, 0, -step);
		m_cam.move(0, 0, -step);
	}
	if (m_input.key(DIK_D, InputState::HELD))
	{
		player->getTransform()->translate(step, 0, 0);
		m_cam.move(step, 0, 0);
	}

	float player_x = player->getTransform()->getPos().x;
	float player_z = player->getTransform()->getPos().z;
	if (rot_dir)
	{
		player2->getTransform()->rotate(XMVectorSet(0, 1, 0, 1), dt);
	}
	else
	{
		player2->getTransform()->rotate(XMVectorSet(1, 0, 0, 1), -dt);
	}
	if (playerCol->intersect(player2Col.get()))
	{
		rot_dir = !rot_dir;
	}
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
	player2->draw();
	m_swap_chain->Present(0, 0);
}

void CollisionsScene::initObjects()
{
	m_cam = Camera(getRatio());

	player = std::make_unique<Cube>();
	player->init(this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
	playerCol = std::make_unique<BoxCollider>(player->getTransform(), Vector3::One, Vector3::Zero);
	
	player2 = std::make_unique<Cube>();
	player2->init(this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
	player2Col = std::make_unique<BoxCollider>(player2->getTransform(), Vector3::One, Vector3::Zero);
	player2->getTransform()->translate(3.3f, 0, 0);
}