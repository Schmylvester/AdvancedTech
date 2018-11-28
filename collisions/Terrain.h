#pragma once
#include "Geometry.h"

struct HeightMapInfo
{
	int terrainWidth;
	int terrainHeight;
	XMFLOAT3 *heightMap;
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
	void addNeighbour(Terrain* t, int neighbour_idx);
	bool isNeighbour(Terrain* t);

	void setPos(int active_x, int active_y);
private:
	Terrain * neighbours[8]{ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	HeightMapInfo h_map_info;
	const char* file_name;
	int grid_x;
	int grid_y;
	int width;
	int height;
};

