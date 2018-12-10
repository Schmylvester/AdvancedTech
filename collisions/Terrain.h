#pragma once
#include "Geometry.h"
#include "ImageMapInfo.h"
#include "NavigationCell.h"
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

class Terrain : public Geometry
{
public:
	Terrain() = delete;
	Terrain(const char* _name, int x, int y);
	~Terrain();

	virtual void init(DXApp* _app, CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff) override;

	void loadFile();
	void createGrid();

	bool playerInCell(int player_x, int player_z);
	void addNeighbour(Terrain* t, NeighbourID neighbour_idx);
	bool isNeighbour(Terrain* t);

	void getIndex(int& x, int& y);
	void createNeighbours(std::vector<Geometry*>* geometry_list, DXApp * _app,
		CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con,
		ID3D11Buffer * c_buff);
	NavigationCell* getCell(int index = -1);
	void linkCellMap() { if (cell_map_ready && create_cell_map.joinable()) { create_cell_map.join(); } }


	static bool cell_map_ready;
private:
	void getNormals();
	std::string removeFileExt(std::string in);
	std::thread create_cell_map;
	NavigationCell cells[16384];
	Terrain * neighbours[8]{ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	ImageMapInfo h_map_info;
	const char* file_name;
	int grid_x;
	int grid_y;
	int width;
	int height;
	float terrain_scale = 4;
	bool normals_set = false;
};

