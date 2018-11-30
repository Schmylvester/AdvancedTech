#include <vector>

#include "Geometry.h"
#include "GeometryIncludes.h"
#include "Scene.h"
#include "TerrainScene.h"

std::vector<Geometry*>* gp_geometry;
DXApp* gp_app;
CBPerObject* gp_cb;
ID3D11DeviceContext** gp_dev_con;
ID3D11Buffer** gp_const_buffer;
Camera* gp_cam;

void setPointers(std::vector<Geometry*>* _geometry, DXApp* _app,
	CBPerObject* _cb, ID3D11DeviceContext** _dev_con,
	ID3D11Buffer** _const_buffer, Camera* _cam)
{
	gp_geometry = _geometry;
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
		int delete_index = rand() % gp_geometry->size();
		Memory::SafeDelete((*gp_geometry)[delete_index]);
		gp_geometry->erase(gp_geometry->begin() + delete_index);
		int shape = rand() % 2;
		if (shape == 0)
			gp_geometry->push_back(new Frustum());
		else if (shape == 1)
			gp_geometry->push_back(new Pyramid());

		gp_geometry->back()->init(gp_app, gp_cb, gp_cam, *gp_dev_con, *gp_const_buffer);
		float x, z;
		z = (float(rand() % 100) / 4);
		x = sqrt(100 - pow(z, 2));
		if (rand() % 2 == 0)
		{
			x *= -1;
		}
		z *= 5 + (rand() % 10);
		x *= 5 + (rand() % 10);
		gp_geometry->back()->getTransform()->translate(x, -6, z);
	}
	DXApp::loader_thread_active = false;
}

void loadTerrain(Terrain* player_loc)
{
	int count = gp_geometry->size();
	for (int i = 0; i < count; i++)
	{
		Terrain* t = static_cast<Terrain*>((*gp_geometry)[i]);
		if (!player_loc->isNeighbour(t) && t != player_loc)
		{
			Memory::SafeDelete((*gp_geometry)[i]);
		}
	}
	for (int i = count - 1; i >= 0; i--)
	{
		if ((*gp_geometry)[i] == nullptr)
		{
			gp_geometry->erase(gp_geometry->begin() + i);
		}
	}

	player_loc->createNeighbours(gp_geometry, gp_app, gp_cb, gp_cam, *gp_dev_con, *gp_const_buffer);
	static_cast<TerrainScene*>(gp_app)->setGridNeighbours();
	DXApp::loader_thread_active = false;
}