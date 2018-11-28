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
	Terrain(const char* _name) { file_name = _name; }
	~Terrain();

	virtual void init(DXApp* _app, CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff) override;

	void loadFile();
	void createGrid();

private:
	HeightMapInfo h_map_info;
	const char* file_name;
};

