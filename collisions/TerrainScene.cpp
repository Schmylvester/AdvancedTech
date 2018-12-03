#include "TerrainScene.h"
#include "GeometryIncludes.h"

void loadTerrain(Terrain* player_loc);
void setPointers(std::vector<Geometry*>* _geometry, DXApp* _app,
	CBPerObject* _cb, ID3D11DeviceContext** _dev_con,
	ID3D11Buffer** _const_buffer, Camera* _cam);

TerrainScene::~TerrainScene()
{
	for(Geometry* ter : terrain)
		Memory::SafeDelete(ter);
}

void TerrainScene::updateScene(float dt)
{
	OutputDebugString(std::to_string(dt).c_str());
	OutputDebugString("\n");

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
	if (!active_cell->playerInCell(player_x, player_z))
	{
		for (Geometry* g : terrain)
		{
			Terrain* t = static_cast<Terrain*>(g);
			if (t->playerInCell(player_x, player_z))
			{
				active_cell = t;
				for (Geometry* safe : terrain)
				{
					Terrain* is_safe = static_cast<Terrain*>(safe);
					if (is_safe == active_cell || active_cell->isNeighbour(is_safe))
					{
						safe_geometry.push_back(is_safe);
					}
				}
				loader_thread_active = true;
				loader_thread = std::thread(loadTerrain, active_cell);
				break;
			}
		}
	}

	m_light.update(dt);


	if (loader_thread.joinable() && !loader_thread_active)
	{
		loader_thread.join();
		safe_geometry.clear();
	}
}

void TerrainScene::drawScene(float dt)
{
	float bg_colour[4]{ 0.0f, 0.0f, 0.0f, 1.0f };
	m_device_context->ClearRenderTargetView(m_rt_view, bg_colour);
	m_device_context->ClearDepthStencilView(m_depth_stcl_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_frame_cb.light = m_light;
	m_device_context->UpdateSubresource(m_cb_per_frame, 0, NULL, &m_frame_cb, 0, 0);
	m_device_context->PSSetConstantBuffers(0, 1, &m_cb_per_frame);
	
	if (!loader_thread_active)
	{
		for (Geometry* ter : terrain)
		{
			ter->draw();
		}
	}
	else
	{
		for (Geometry* ter : safe_geometry)
		{
			ter->draw();
		}
	}
	//player->draw();

	m_swap_chain->Present(0, 0);
}

void TerrainScene::initObjects()
{
	m_cam = Camera(getRatio());
	m_cam.move(256, 55, 256);

	active_cell = new Terrain("..\\Resources\\HeightMap.bmp", 0, 0);
	terrain.push_back(active_cell);
	terrain.back()->init(this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
	terrain.back()->getTransform()->translate(0, 0, 0);

	player = new Cube();
	player->init(this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
	player->getTransform()->translate(256, 55, 256);

	setPointers(&(terrain), this, &m_object_cb, &m_device_context, &m_cb_per_object, &m_cam);

	safe_geometry.push_back(active_cell);
	loader_thread_active = true;
	loader_thread = std::thread(loadTerrain, active_cell);
}

void TerrainScene::setGridNeighbours()
{
	for (int i = 0; i < terrain.size(); i++)
	{
		for (int j = i + 1; j < terrain.size(); j++)
		{
			Terrain* i_terr = static_cast<Terrain*>(terrain[i]);
			Terrain* j_terr = static_cast<Terrain*>(terrain[j]);
			int i_x, i_y, j_x, j_y;
			i_terr->getIndex(i_x, i_y);
			j_terr->getIndex(j_x, j_y);

			if (i_x == j_x - 1)
			{
				if (i_y == j_y - 1)
				{
					i_terr->addNeighbour(j_terr, NeighbourID::TOP_RIGHT);
					j_terr->addNeighbour(i_terr, NeighbourID::BOTTOM_LEFT);
				}
				else if (i_y == j_y)
				{
					i_terr->addNeighbour(j_terr, NeighbourID::RIGHT);
					j_terr->addNeighbour(i_terr, NeighbourID::LEFT);
				}
				else if (i_y == j_y + 1)
				{
					i_terr->addNeighbour(j_terr, NeighbourID::BOTTOM_RIGHT);
					j_terr->addNeighbour(i_terr, NeighbourID::TOP_LEFT);
				}
			}
			else if (i_x == j_x)
			{
				if (i_y == j_y - 1)
				{
					i_terr->addNeighbour(j_terr, NeighbourID::TOP);
					j_terr->addNeighbour(i_terr, NeighbourID::BOTTOM);
				}
				else if (i_y == j_y + 1)
				{
					i_terr->addNeighbour(j_terr, NeighbourID::BOTTOM);
					j_terr->addNeighbour(i_terr, NeighbourID::TOP);
				}
			}
			else if (i_x == j_x + 1)
			{
				if (i_y == j_y - 1)
				{
					i_terr->addNeighbour(j_terr, NeighbourID::TOP_LEFT);
					j_terr->addNeighbour(i_terr, NeighbourID::BOTTOM_RIGHT);
				}
				else if (i_y == j_y)
				{
					i_terr->addNeighbour(j_terr, NeighbourID::LEFT);
					j_terr->addNeighbour(i_terr, NeighbourID::RIGHT);
				}
				else if (i_y == j_y + 1)
				{
					i_terr->addNeighbour(j_terr, NeighbourID::BOTTOM_LEFT);
					j_terr->addNeighbour(i_terr, NeighbourID::TOP_RIGHT);
				}
			}
		}
	}
}
