#include "Terrain.h"
#include "DXApp.h"
#include <fstream>

void errorBox(LPCSTR message);

Terrain::Terrain(const char * _name, int x, int y)
{
	file_name = _name;
	grid_x = x;
	grid_y = y;
}

Terrain::~Terrain()
{
	Memory::SafeDeleteArr(h_map_info.map);
}

void Terrain::init(DXApp * _app, CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff)
{
	loadFile();
	createGrid();

	m_vtx_buffer = _app->getVertexBuffer(file_name, this);
	m_idx_buffer = _app->getIndexBuffer(file_name, this);

	Geometry::init(_app, _cb, cam, dev_con, c_buff);

	m_transform.translate(terrain_scale * grid_x * (width), 0, terrain_scale * grid_y * (height));
}

void Terrain::loadFile()
{
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
	h_map_info.img_width = bitmap_info_header.biWidth;
	h_map_info.img_height = bitmap_info_header.biHeight;
	image_size = h_map_info.img_width * h_map_info.img_width * 3;
	unsigned char * bitmap_image = new unsigned char[image_size];
	fseek(file_ptr, bitmap_file_header.bfOffBits, SEEK_SET);
	fread(bitmap_image, 1, image_size, file_ptr);
	fclose(file_ptr);

	h_map_info.map = new XMFLOAT3[h_map_info.img_width * h_map_info.img_height];

	//0 = b, 1 = g, 2 = r
	int colour_idx = 2;

	float height_factor = 20.0f;

	for (int x = 0; x < h_map_info.img_width; x++)
	{
		for (int y = 0; y < h_map_info.img_height; y++)
		{
			height = bitmap_image[colour_idx];
			index = (h_map_info.img_height * x) + y;
			h_map_info.map[index].x = (float)y * terrain_scale;
			h_map_info.map[index].y = (float)(height) / height_factor;
			h_map_info.map[index].z = (float)x * terrain_scale;

			colour_idx += 3;
		}
	}

	Memory::SafeDeleteArr(bitmap_image);
}

void Terrain::createGrid()
{
	width = h_map_info.img_width;
	height = h_map_info.img_height;
	int cols = h_map_info.img_width;
	int rows = h_map_info.img_height;

	vertex_count = rows * cols;
	triangle_count = (rows - 1) * (cols - 1) * 2;

	//vertices
	vertices = new Vertex[vertex_count];
	for (DWORD y = 0; y < rows; y++)
	{
		for (DWORD x = 0; x < cols; x++)
		{
			int index = (y * cols) + x;
			vertices[index].position = h_map_info.map[index];
			float shade = ((float)(rand() % 100)) / 500;

			if (vertices[index].position.y < 10 && vertices[index].position.y >= 5)
			{
				vertices[index].colour = XMFLOAT4(shade, 0.4f + shade, 0.4f + shade, 1.0f);
			}
			if (vertices[index].position.y >= 10)
			{
				vertices[index].colour = XMFLOAT4(shade, 0.4f + shade, shade, 1.0f);
			}
			else
			{
				vertices[index].colour = XMFLOAT4(0.15f + shade, 0.1f + shade, shade, 1.0f);
			}
			vertices[index].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		}
	}

	//indices
	index_count = triangle_count * 3;
	indices = new DWORD[index_count];
	int idx_set = 0;
	int index = 0;
	for (DWORD y = 0; y < rows - 1; y++)
	{
		for (DWORD x = 0; x < cols - 1; x++)
		{
			index = y * cols + x;
			indices[idx_set + 5] = index;

			index = y * cols + x + 1;
			indices[idx_set + 4] = index;

			index = (y + 1) * cols + x;
			indices[idx_set + 3] = index;

			index = (y + 1) * cols + x;
			indices[idx_set + 2] = index;

			index = y * cols + x + 1;
			indices[idx_set + 1] = index;

			index = (y + 1) * cols + x + 1;
			indices[idx_set] = index;

			idx_set += 6; // next quad
		}
	}
	getNormals();
}

bool Terrain::playerInCell(int player_x, int player_z)
{
	int left = (grid_x * (terrain_scale * width)) - 64;
	int right = ((grid_x + 1) * (terrain_scale * width)) - 64;
	int bottom = (grid_y * (terrain_scale * height)) - 64;
	int top = ((grid_y + 1) * (terrain_scale * height)) - 64;
	return (player_x > left && player_x <= right && player_z > bottom && player_z <= top);
}

void Terrain::addNeighbour(Terrain * t, NeighbourID neighbour_idx)
{
	neighbours[neighbour_idx] = t;
}

bool Terrain::isNeighbour(Terrain * t)
{
	for (Terrain* neighbour : neighbours)
	{
		if (neighbour == t)
		{
			return true;
		}
	}
	return false;
}

void Terrain::getIndex(int & x, int & y)
{
	x = grid_x;
	y = grid_y;
}

void Terrain::createNeighbours(std::vector<Geometry*>* geometry_list, DXApp * _app,
	CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff)
{
	if (neighbours[TOP_LEFT] == nullptr)
	{
		geometry_list->push_back(new Terrain(file_name, grid_x - 1, grid_y + 1));
		geometry_list->back()->init(_app, _cb, cam, dev_con, c_buff);
		Terrain* t = static_cast<Terrain*>(geometry_list->back());
	}
	if (neighbours[TOP] == nullptr)
	{
		geometry_list->push_back(new Terrain(file_name, grid_x, grid_y + 1));
		geometry_list->back()->init(_app, _cb, cam, dev_con, c_buff);
		Terrain* t = static_cast<Terrain*>(geometry_list->back());
	}
	if (neighbours[TOP_RIGHT] == nullptr)
	{
		geometry_list->push_back(new Terrain(file_name, grid_x + 1, grid_y + 1));
		geometry_list->back()->init(_app, _cb, cam, dev_con, c_buff);
		Terrain* t = static_cast<Terrain*>(geometry_list->back());
	}
	if (neighbours[LEFT] == nullptr)
	{
		geometry_list->push_back(new Terrain(file_name, grid_x - 1, grid_y));
		geometry_list->back()->init(_app, _cb, cam, dev_con, c_buff);
		Terrain* t = static_cast<Terrain*>(geometry_list->back());
	}
	if (neighbours[RIGHT] == nullptr)
	{
		geometry_list->push_back(new Terrain(file_name, grid_x + 1, grid_y));
		geometry_list->back()->init(_app, _cb, cam, dev_con, c_buff);
		Terrain* t = static_cast<Terrain*>(geometry_list->back());
	}
	if (neighbours[BOTTOM_LEFT] == nullptr)
	{
		geometry_list->push_back(new Terrain(file_name, grid_x - 1, grid_y - 1));
		geometry_list->back()->init(_app, _cb, cam, dev_con, c_buff);
		Terrain* t = static_cast<Terrain*>(geometry_list->back());
	}
	if (neighbours[BOTTOM] == nullptr)
	{
		geometry_list->push_back(new Terrain(file_name, grid_x, grid_y - 1));
		geometry_list->back()->init(_app, _cb, cam, dev_con, c_buff);
		Terrain* t = static_cast<Terrain*>(geometry_list->back());
	}
	if (neighbours[BOTTOM_RIGHT] == nullptr)
	{
		geometry_list->push_back(new Terrain(file_name, grid_x + 1, grid_y - 1));
		geometry_list->back()->init(_app, _cb, cam, dev_con, c_buff);
		Terrain* t = static_cast<Terrain*>(geometry_list->back());
	}
}

void Terrain::getNormals()
{
	std::string f = removeFileExt(file_name) + "Normal";
	const char* file = (f).c_str();
	std::ifstream read_from_file;
	read_from_file.open(file);
	if (read_from_file.is_open())
	{
		char data;
		for (int i = 0; i < vertex_count; i++)
		{
			read_from_file >> data;
			vertices[i].normal.x = ((float)((int)data)) / 255;
			read_from_file >> data;
			vertices[i].normal.y = ((float)((int)data)) / 255;
			read_from_file >> data;
			vertices[i].normal.z = ((float)((int)data)) / 255;
		}

		read_from_file.close();
	}
	else
	{
		int file_pos = 0;
		float vecX, vecY, vecZ;
		XMVECTOR edge1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR edge2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		int faces = 0;
		int max_faces = 0;
		std::ofstream write_to_file;
		write_to_file.open(file, std::ofstream::out);
		for (int i = 0; i < vertex_count; i++)
		{
			for (int j = 0; j < triangle_count; j++)
			{
				if (indices[j * 3] == i ||
					indices[(j * 3) + 1] == i ||
					indices[(j * 3) + 2] == i)
				{
					vecX = vertices[indices[(j * 3)] + 2].position.x - vertices[indices[(j * 3)]].position.x;
					vecY = vertices[indices[(j * 3)] + 2].position.y - vertices[indices[(j * 3)]].position.y;
					vecZ = vertices[indices[(j * 3)] + 2].position.z - vertices[indices[(j * 3)]].position.z;
					edge1 = XMVectorSet(vecX, vecY, vecZ, 0.0f);

					vecX = vertices[indices[(j * 3) + 1]].position.x - vertices[indices[(j * 3) + 2]].position.x;
					vecY = vertices[indices[(j * 3) + 1]].position.y - vertices[indices[(j * 3) + 2]].position.y;
					vecZ = vertices[indices[(j * 3) + 1]].position.z - vertices[indices[(j * 3) + 2]].position.z;
					edge2 = XMVectorSet(vecX, vecY, vecZ, 0.0f);

					normalSum = XMVectorAdd(normalSum, XMVector3Cross(edge1, edge2));
					faces++;
				}
			}
			normalSum = normalSum / faces;

			normalSum = XMVector3Normalize(normalSum);

			vertices[i].normal.x = XMVectorGetX(normalSum);
			vertices[i].normal.y = XMVectorGetY(normalSum);
			vertices[i].normal.z = XMVectorGetZ(normalSum);

			normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			faces = 0;

			char file_char;
			file_char = (char)((int)(vertices[i].normal.x * 255));
			write_to_file << file_char;
			file_pos++;
			file_char = (char)((int)(vertices[i].normal.y * 255));
			write_to_file << file_char;
			file_pos++;
			file_char = (char)((int)(vertices[i].normal.z * 255));
			write_to_file << file_char;
			file_pos++;
		}
		write_to_file.close();
	}
}

std::string Terrain::removeFileExt(std::string in)
{
	std::string return_string = "";

	for (int i = 0; !(in[i] == '.' && in[i + 1] == 'b') && i < 600; i++)
	{
		return_string += in[i];
	}

	return return_string;
}