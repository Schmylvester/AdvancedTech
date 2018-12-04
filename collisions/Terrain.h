#pragma once
#include "Geometry.h"
#include "ImageMapInfo.h"
#include <vector>

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

private:
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

