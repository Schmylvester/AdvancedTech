#include <vector>

#include "Terrain.h"
#include "GeometryIncludes.h"
#include "Scene.h"
#include "TerrainScene.h"
#include "NavigationCell.h"
#include "NavMesh.h"
#include "ImageMapInfo.h"

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
		if ((*objects)[delete_index])
		{
			delete (*objects)[delete_index];
			(*objects)[delete_index] = nullptr;
		}
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
			if ((*objects)[i])
			{
				delete (*objects)[i];
				(*objects)[i] = nullptr;
			}
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

NavMesh* gp_nav_mesh = nullptr;
ImageMapInfo* gp_map = nullptr;
void setPointers(NavMesh* nav_mesh, ImageMapInfo* map)
{
	gp_nav_mesh = nav_mesh;
	gp_map = map;
}
void initNavMesh()
{
	gp_nav_mesh->cell_count = gp_map->img_width * gp_map->img_height;
	gp_nav_mesh->cells = new NavigationCell[gp_nav_mesh->cell_count];
	for (int i = 0; i < gp_nav_mesh->cell_count; i++)
	{
		int idx = i;
		gp_nav_mesh->cells[i] =
			NavigationCell(idx % gp_map->img_width,
				gp_map->map[idx].y, idx / gp_map->img_width,
				gp_map->map[idx]);
	}
	for (int i = 0; i < gp_nav_mesh->cell_count; i++)
	{
		for (int j = i + 1; j < gp_nav_mesh->cell_count; j++)
		{
			if (gp_nav_mesh->cells[j].countNeighbours() < 8)
				gp_nav_mesh->cells[i].checkNeighbour(&gp_nav_mesh->cells[j]);
			if (gp_nav_mesh->cells[i].countNeighbours() == 8)
				break;
		}
	}
	gp_nav_mesh->initialised = true;
}

void initNavMesh(NavMesh* nav_mesh, ImageMapInfo* map)
{
	nav_mesh->cell_count = map->img_width * map->img_height;
	nav_mesh->cells = new NavigationCell[nav_mesh->cell_count];
	for (int i = 0; i < nav_mesh->cell_count; i++)
	{
		int idx = i;
		nav_mesh->cells[i] =
			NavigationCell(idx % map->img_width,
				map->map[idx].y, idx / map->img_width,
				map->map[idx]);
	}
	for (int i = 0; i < nav_mesh->cell_count; i++)
	{
		for (int j = i + 1; j < nav_mesh->cell_count; j++)
		{
			if (nav_mesh->cells[j].countNeighbours() < 4)
				nav_mesh->cells[i].checkNeighbour(&nav_mesh->cells[j]);
			if (nav_mesh->cells[i].countNeighbours() == 4)
				break;
		}
	}
	nav_mesh->initialised = true;
}