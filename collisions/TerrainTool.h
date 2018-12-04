#pragma once
#include "ImageMapInfo.h"

class TerrainTool
{
public:
	TerrainTool() = default;
	~TerrainTool();

	void readFile(const char* _file_name);
	void splitFile(int width, int height);
	void writeFile(const char* file_name, unsigned char* new_image);
private:
	const char* file_name;
	ImageMapInfo image_map;
	unsigned char * bitmap_image;
};