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
	BOTTOM_RIGHT,
	None
};

class NavMesh;
class Terrain : public GameObject
{
public:
	Terrain() = default;
	~Terrain();

	void init(std::string _name, int x, int y, DXApp* _app, CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff);
	NavMesh* addNavMesh();

	void loadFile();

	bool playerInCell(int player_x, int player_z);
	void addNeighbour(Terrain* t, NeighbourID neighbour_idx);
	bool isNeighbour(Terrain* t);
	void getIndex(int & x, int & y);
	void createNeighbours(std::vector<Terrain*>* geometry_list, DXApp * _app,
		CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con,
		ID3D11Buffer * c_buff);
private:
	std::string getNextFile(std::string in, int dir_x, int dir_y);

	std::thread nav_thread;

	char perl_limit_x = 'F';
	char perl_limit_y = 'F';
	NavMesh * nav_mesh = nullptr;
	Terrain * neighbours[8]{ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	ImageMapInfo h_map_info;
	std::string file_name;
	int grid_x;
	int grid_y;
	int width;
	int length;
	float terrain_scale = 4;
	float overlap = 0.01f;
	bool normals_set = false;
};

