#include "TerrainTool.h"
#include "DXUtil.h"

void errorBox(LPCSTR message);

TerrainTool::~TerrainTool()
{
	Memory::SafeDeleteArr(bitmap_image);
}

void TerrainTool::readFile(const char * _file_name)
{
	file_name = _file_name;
	FILE* file_ptr;
	BITMAPFILEHEADER bitmap_file_header;
	BITMAPINFOHEADER bitmap_info_header;

	int image_size;
	int index;
	unsigned char height;

	fopen_s(&file_ptr, file_name, "rb");

	if (file_ptr == nullptr)
	{
		errorBox("Bad job with that file");
		return;
	}

	fread(&bitmap_file_header, sizeof(bitmap_file_header), 1, file_ptr);
	fread(&bitmap_info_header, sizeof(bitmap_info_header), 1, file_ptr);
	image_map.img_width = bitmap_info_header.biWidth;
	image_map.img_height = bitmap_info_header.biHeight;
	image_size = image_map.img_width * image_map.img_width * 3;
	bitmap_image = new unsigned char[image_size];
	fseek(file_ptr, bitmap_file_header.bfOffBits, SEEK_SET);
	fread(bitmap_image, 1, image_size, file_ptr);
	fclose(file_ptr);
}

void TerrainTool::splitFile(int width, int height)
{
}

void TerrainTool::writeFile(const char * file_name, unsigned char* new_image)
{

}
