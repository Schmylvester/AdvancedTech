#pragma once
#include "Geometry.h"
#include "ImageMapInfo.h"
#include "NavigationCell.h"
#include "GameObject.h"
#include <vector>
#include <thread>

enum NeighbourID
{
	TOP_LEFT = 0,
	TOP,
	TOP_RIGHT,
	LEFT,
	RIGHT,
	BOTTOM_LEFT,
	BOTTOM,
	BOTTOM_RIGHT
};

class NavMesh;
class Terrain : public GameObject
{
public:
	Terrain() = default;
	~Terrain();

	void init(const char* _name, int x, int y, DXApp* _app, CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff);
	NavMesh* addNavMesh();

	void loadFile();

	bool playerInCell(int player_x, int player_z);
	void addNeighbour(Terrain* t, NeighbourID neighbour_idx);
	bool isNeighbour(Terrain* t);

	void getIndex(int& x, int& y);
	void createNeighbours(std::vector<Terrain*>* geometry_list, DXApp * _app,
		CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con,
		ID3D11Buffer * c_buff);
	NavigationCell* getCell(int index = -1);

private:
	std::thread nav_thread;
	NavMesh * nav_mesh = nullptr;
	NavigationCell cells[16384];
	Terrain * neighbours[8]{ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	ImageMapInfo h_map_info;
	const char* file_name;
	int grid_x;
	int grid_y;
	int width;
	int length;
	float terrain_scale = 4;
	bool normals_set = false;
};

