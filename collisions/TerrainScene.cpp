#include "TerrainScene.h"
#include "GeometryIncludes.h"
#include "Terrain.h"
#include "Player.h"
#include "Boid.h"

void loadTerrain(Terrain* player_loc);
void setPointers(void* _geometry, DXApp* _app,
	CBPerObject* _cb, ID3D11DeviceContext** _dev_con,
	ID3D11Buffer** _const_buffer, Camera* _cam);

TerrainScene::~TerrainScene()
{
	if (player)
	{
		delete player;
		player = nullptr;
	}
	for (GameObject* ter : terrain)
		if (ter)
		{
			delete ter;
			ter = nullptr;
		}
	for (Boid* boid : m_boids)
	{
		if (boid != nullptr)
		{
			delete boid;
			boid = nullptr;
		}
	}
	m_boids.clear();
}

void TerrainScene::updateScene(float dt)
{
	m_input.detectInput();
	player->update(dt);
	Vector3 player_pos = player->getTransform()->getPos();
	if (player_pos != m_player_last_pos)
	{
		for (Boid* boid : m_boids)
		{
			boid->setMid(Vector3(player_pos.x, boid->getMid().y, player_pos.z));
		}
		m_player_last_pos = player_pos;
	}

	float player_x = player->getTransform()->getPos().x;
	float player_z = player->getTransform()->getPos().z;
	if (!active_cell->playerInCell(player_x, player_z))
	{
		for (Terrain* g : terrain)
		{
			Terrain* t = g;
			if (t->playerInCell(player_x, player_z))
			{
				active_cell = t;
				for (GameObject* safe : terrain)
				{
					Terrain* is_safe = static_cast<Terrain*>(safe);
					if (is_safe == active_cell || active_cell->isNeighbour(is_safe))
					{
						safe_geometry.push_back(safe);
					}
				}
				loader_thread_active = true;
				loader_thread = std::thread(loadTerrain, active_cell);
				break;
			}
		}
	}

	for (Boid* boid : m_boids)
	{
		if (boid != nullptr)
		{
			boid->takeSnap();
		}
	}
	for (Boid* boid : m_boids)
	{
		if (boid != nullptr)
		{
			boid->update(dt);
		}
	}

	if (loader_thread.joinable() && !loader_thread_active)
	{
		loader_thread.join();
		safe_geometry.clear();
	}
}

void TerrainScene::drawObjects(float dt)
{
	if (!loader_thread_active)
	{
		for (GameObject* ter : terrain)
		{
			ter->draw();
		}
	}
	else
	{
		for (GameObject* ter : safe_geometry)
		{
			ter->draw();
		}
	}

	for (Boid* boid : m_boids)
	{
		if (boid != nullptr)
		{
			boid->draw();
		}
	}
	//player->draw();
}

void TerrainScene::initObjects()
{
	m_cam = Camera(getRatio());
	m_cam.move(256, 55, 206);

	Terrain* t = new Terrain();
	t->init("..\\Resources\\Perlin\\89.bmp", 0, 0, this,
		&m_object_cb, &m_cam, m_device_context, m_cb_per_frame);
	active_cell = t;
	terrain.push_back(t);

	for (int i = 0; i < boid_count; i++)
	{
		m_boids.push_back(new Boid(&m_boids, Vector3(256, 55, 206), 40.0f, false));
		m_boids.back()->init(Shape::Pyramid, this, &m_object_cb,
			&m_cam, m_device_context, m_cb_per_object);
		m_boids.back()->getTransform()->translate(
			256 + ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 50)) - 25), 55,
			256 + ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 50)) - 25));
		m_boids.back()->getTransform()->scale(0.03f, 0.03f, 0.03f);
	}

	player = new Player(&m_input);
	player->init(Shape::Cube, this, &m_object_cb, &m_cam, m_device_context, m_cb_per_object);
	player->getTransform()->translate(256, 55, 256);
	player->setFollowCam();
	m_player_last_pos = player->getTransform()->getPos();

	setPointers(&(terrain), this, &m_object_cb, &m_device_context, &m_cb_per_object, &m_cam);

	safe_geometry.push_back(t);
	loader_thread_active = true;
	loader_thread = std::thread(loadTerrain, active_cell);
}

void TerrainScene::setGridNeighbours()
{
	for (int i = 0; i < terrain.size(); i++)
	{
		for (int j = i + 1; j < terrain.size(); j++)
		{
			int i_x, i_y, j_x, j_y;
			terrain[i]->getIndex(i_x, i_y);
			terrain[j]->getIndex(j_x, j_y);

			if (i_x == j_x - 1)
			{
				if (i_y == j_y - 1)
				{
					terrain[i]->addNeighbour(terrain[j], NeighbourID::TOP_RIGHT);
					terrain[j]->addNeighbour(terrain[i], NeighbourID::BOTTOM_LEFT);
				}
				else if (i_y == j_y)
				{
					terrain[i]->addNeighbour(terrain[j], NeighbourID::RIGHT);
					terrain[j]->addNeighbour(terrain[i], NeighbourID::LEFT);
				}
				else if (i_y == j_y + 1)
				{
					terrain[i]->addNeighbour(terrain[j], NeighbourID::BOTTOM_RIGHT);
					terrain[j]->addNeighbour(terrain[i], NeighbourID::TOP_LEFT);
				}
			}
			else if (i_x == j_x)
			{
				if (i_y == j_y - 1)
				{
					terrain[i]->addNeighbour(terrain[j], NeighbourID::TOP);
					terrain[j]->addNeighbour(terrain[i], NeighbourID::BOTTOM);
				}
				else if (i_y == j_y + 1)
				{
					terrain[i]->addNeighbour(terrain[j], NeighbourID::BOTTOM);
					terrain[j]->addNeighbour(terrain[i], NeighbourID::TOP);
				}
			}
			else if (i_x == j_x + 1)
			{
				if (i_y == j_y - 1)
				{
					terrain[i]->addNeighbour(terrain[j], NeighbourID::TOP_LEFT);
					terrain[j]->addNeighbour(terrain[i], NeighbourID::BOTTOM_RIGHT);
				}
				else if (i_y == j_y)
				{
					terrain[i]->addNeighbour(terrain[j], NeighbourID::LEFT);
					terrain[j]->addNeighbour(terrain[i], NeighbourID::RIGHT);
				}
				else if (i_y == j_y + 1)
				{
					terrain[i]->addNeighbour(terrain[j], NeighbourID::BOTTOM_LEFT);
					terrain[j]->addNeighbour(terrain[i], NeighbourID::TOP_RIGHT);
				}
			}
		}
	}
}
