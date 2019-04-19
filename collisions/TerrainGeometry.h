#pragma once
#include "Geometry.h"
#include "ImageMapInfo.h"
class TerrainGeometry : public Geometry
{
public:
	TerrainGeometry(const char* _name, int x, int y);
	TerrainGeometry() = delete;
	~TerrainGeometry() = default;

	void init(DXApp * _app, CBPerObject * _cb, Camera * cam,
		ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff, GameObject* owner) override;


	void createVerts(ImageMapInfo h_map_info);
	void getNormals(int* earliest_instance);


private:
	std::string removeFileExt(const char * in);
	
	const char* file_name;
	int width;
	int height;
	int grid_x;
	int grid_y;
};

