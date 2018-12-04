#include "TerrainTool.h"
#include "DXUtil.h"
#include <string>

void errorBox(LPCSTR message);

TerrainTool::~TerrainTool()
{
	Memory::SafeDeleteArr(bitmap_image);
}

void TerrainTool::readFile(const char * _file_name)
{
	file_name = _file_name;
	FILE* file_ptr;

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

	splitFile(600, 600);
}

void TerrainTool::splitFile(int width, int height)
{
	int segments_x = (image_map.img_width / width) + 1;
	int segments_y = (image_map.img_height / height) + 1;

	for (int x = 0; x < segments_x; x++)
	{
		for (int y = 0; y < segments_y; y++)
		{
			//name the file
			std::string s = file_name;
			s = removeFileExt(s);
			s += "X" + std::to_string(x) + "Y" + std::to_string(y) + ".bmp";
			const char* new_file_name = (s).c_str();

			//map the image
			unsigned char* new_image = new unsigned char[width * height];
			int init_y = y * height;
			int init_x = x * width;
			for (int portion_y = init_y; portion_y < init_y + height; portion_y++)
			{
				for (int portion_x = init_x; portion_x < init_x + width; portion_x++)
				{
					int pixel_y =  portion_y - init_y;
					int pixel_x = portion_x - init_x;
					new_image[(pixel_y * width) + pixel_x] = bitmap_image[(portion_y * image_map.img_width) + portion_x];
				}
			}

			//create the new file
			writeFile(new_file_name, new_image, height, width);
			Memory::SafeDeleteArr(new_image);
		}
	}
}

void TerrainTool::writeFile(const char * write_file_name, unsigned char* new_image,int height, int width)
{
	FILE* file_ptr;
	int image_size = height * width * 3;

	bitmap_info_header.biWidth = width;
	bitmap_info_header.biHeight = height;

	fopen_s(&file_ptr, write_file_name, "w");
	fwrite(&bitmap_file_header, sizeof(bitmap_file_header), 1, file_ptr);
	fwrite(&bitmap_info_header, sizeof(bitmap_info_header), 1, file_ptr);
	fwrite(bitmap_image, 1, image_size, file_ptr);
	fclose(file_ptr);
}

std::string TerrainTool::removeFileExt(std::string in)
{
	std::string return_string = "";

	for (int i = 0; !(in[i] == '.' && in[i + 1] == 'b') && i < 600; i++)
	{
		return_string += in[i];
	}

	return return_string;
}
