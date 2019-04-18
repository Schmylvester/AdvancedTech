#include <vector>

#include "Terrain.h"
#include "GeometryIncludes.h"
#include "Scene.h"
#include "TerrainScene.h"
#include "NavigationCell.h"
#include "Pathfinder.h"

void* gp_objects;
DXApp* gp_app;
CBPerObject* gp_cb;
ID3D11DeviceContext** gp_dev_con;
ID3D11Buffer** gp_const_buffer;
Camera* gp_cam;

void setPointers(void* _geometry, DXApp* _app,
	CBPerObject* _cb, ID3D11DeviceContext** _dev_con,
	ID3D11Buffer** _const_buffer, Camera* _cam)
{
	gp_objects = _geometry;
	gp_app = _app;
	gp_cb = _cb;
	gp_dev_con = _dev_con;
	gp_const_buffer = _const_buffer;
	gp_cam = _cam;
}

void loadUnloadObjects()
{
	std::vector<GameObject*>* objects = static_cast<std::vector<GameObject*>*>(gp_objects);
	for (int i = 0; i < 10000; i++)
	{
		int delete_index = rand() % objects->size();
		Memory::SafeDelete((*objects)[delete_index]);
		objects->erase(objects->begin() + delete_index);
		int shape = rand() % 2;
		objects->push_back(new GameObject());
		if (shape == 0)
			objects->back()->init(Shape::Frustum, gp_app, gp_cb, gp_cam, *gp_dev_con, *gp_const_buffer);
		else if (shape == 1)
			objects->back()->init(Shape::Pyramid, gp_app, gp_cb, gp_cam, *gp_dev_con, *gp_const_buffer);

		float x, z;
		z = (float(rand() % 100) / 4);
		x = sqrt(100 - pow(z, 2));
		if (rand() % 2 == 0)
		{
			x *= -1;
		}
		z *= 5 + (rand() % 10);
		x *= 5 + (rand() % 10);
		objects->back()->getTransform()->translate(x, -6, z);
	}
	DXApp::loader_thread_active = false;
}

void loadTerrain(Terrain* player_loc)
{
	std::vector<Terrain*>* objects = static_cast<std::vector<Terrain*>*>(gp_objects);
	int count = objects->size();
	for (int i = 0; i < count; i++)
	{
		Terrain* t = static_cast<Terrain*>((*objects)[i]);
		if (!player_loc->isNeighbour(t) && t != player_loc)
		{
			Memory::SafeDelete((*objects)[i]);
		}
	}
	for (int i = count - 1; i >= 0; i--)
	{
		if ((*objects)[i] == nullptr)
		{
			objects->erase(objects->begin() + i);
		}
	}

	player_loc->createNeighbours(objects, gp_app, gp_cb, gp_cam, *gp_dev_con, *gp_const_buffer);
	static_cast<TerrainScene*>(gp_app)->setGridNeighbours();
	DXApp::loader_thread_active = false;
}