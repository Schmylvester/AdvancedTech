#pragma once
#include "ImageMapInfo.h"

class TerrainTool
{
public:
	TerrainTool() = default;
	~TerrainTool();

	void readFile(const char* _file_name);
	void splitFile(int width, int height);
	void writeFile(const char* file_name, unsigned char* new_image, int height, int width);
private:
	std::string removeFileExt(std::string in);

	const char* file_name;
	ImageMapInfo image_map;
	unsigned char * bitmap_image;
	BITMAPFILEHEADER bitmap_file_header;
	BITMAPINFOHEADER bitmap_info_header;
};