#include <vector>

#include "GameObject.h"
#include "GeometryIncludes.h"
#include "Scene.h"
#include "TerrainScene.h"
#include "NavigationCell.h"
#include "Pathfinder.h"

std::vector<GameObject*>* gp_objects;
DXApp* gp_app;
CBPerObject* gp_cb;
ID3D11DeviceContext** gp_dev_con;
ID3D11Buffer** gp_const_buffer;
Camera* gp_cam;

void setPointers(std::vector<GameObject*>* _geometry, DXApp* _app,
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
	for (int i = 0; i < 10000; i++)
	{
		int delete_index = rand() % gp_objects->size();
		Memory::SafeDelete((*gp_objects)[delete_index]);
		gp_objects->erase(gp_objects->begin() + delete_index);
		int shape = rand() % 2;
		gp_objects->push_back(new GameObject());
		if (shape == 0)
			gp_objects->back()->init(Shape::Frustum, gp_app, gp_cb, gp_cam, *gp_dev_con, *gp_const_buffer);
		else if (shape == 1)
			gp_objects->back()->init(Shape::Pyramid, gp_app, gp_cb, gp_cam, *gp_dev_con, *gp_const_buffer);

		float x, z;
		z = (float(rand() % 100) / 4);
		x = sqrt(100 - pow(z, 2));
		if (rand() % 2 == 0)
		{
			x *= -1;
		}
		z *= 5 + (rand() % 10);
		x *= 5 + (rand() % 10);
		gp_objects->back()->getTransform()->translate(x, -6, z);
	}
	DXApp::loader_thread_active = false;
}

void loadTerrain(Terrain* player_loc)
{
	int count = gp_objects->size();
	for (int i = 0; i < count; i++)
	{
		Terrain* t = static_cast<Terrain*>((*gp_objects)[i]);
		if (!player_loc->isNeighbour(t) && t != player_loc)
		{
			Memory::SafeDelete((*gp_objects)[i]);
		}
	}
	for (int i = count - 1; i >= 0; i--)
	{
		if ((*gp_objects)[i] == nullptr)
		{
			gp_objects->erase(gp_objects->begin() + i);
		}
	}

	std::vector<Terrain*> geometries;
	for (GameObject* go : *gp_objects)
		if (Terrain* t = dynamic_cast<Terrain*>(go))
		{
			geometries.push_back(t);
		}
	player_loc->createNeighbours(&geometries, gp_app, gp_cb, gp_cam, *gp_dev_con, *gp_const_buffer);
	static_cast<TerrainScene*>(gp_app)->setGridNeighbours();
	DXApp::loader_thread_active = false;
}