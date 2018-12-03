#include "Terrain.h"
#include "DXApp.h"

Terrain::Terrain(const char * _name, int x, int y)
{
	file_name = _name;
	grid_x = x;
	grid_y = y;
}

Terrain::~Terrain()
{
	Memory::SafeDeleteArr(h_map_info.heightMap);
}

void Terrain::init(DXApp * _app, CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff)
{
	loadFile();
	createGrid();

	m_vtx_buffer = _app->getVertexBuffer(file_name, this);
	m_idx_buffer = _app->getIndexBuffer(file_name, this);

	Geometry::init(_app, _cb, cam, dev_con, c_buff);

	m_transform.translate(terrain_scale * grid_x * (width - 1), 0, terrain_scale * grid_y * (height - 1));
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

	h_map_info.terrainWidth = bitmap_info_header.biWidth;
	h_map_info.terrainHeight = bitmap_info_header.biHeight;

	image_size = h_map_info.terrainWidth * h_map_info.terrainWidth * 3;

	unsigned char * bitmap_image = new unsigned char[image_size];

	fseek(file_ptr, bitmap_file_header.bfOffBits, SEEK_SET);

	fread(bitmap_image, 1, image_size, file_ptr);

	fclose(file_ptr);

	h_map_info.heightMap = new XMFLOAT3[h_map_info.terrainWidth * h_map_info.terrainHeight];

	int height_mod = 0;

	float height_factor = 15.0f;

	for (int x = 0; x < h_map_info.terrainWidth; x++)
	{
		for (int y = 0; y < h_map_info.terrainHeight; y++)
		{
			height = bitmap_image[height_mod];
			index = (h_map_info.terrainHeight * x) + y;
			h_map_info.heightMap[index].x = (float)y * terrain_scale;
			h_map_info.heightMap[index].y = (float)(height) / height_factor;
			h_map_info.heightMap[index].z = (float)x * terrain_scale;

			height_mod += 3;

		}
	}

	Memory::SafeDeleteArr(bitmap_image);
}

void Terrain::createGrid()
{
	width = h_map_info.terrainWidth;
	height = h_map_info.terrainHeight;
	int cols = h_map_info.terrainWidth;
	int rows = h_map_info.terrainHeight;

	vertex_count = rows * cols;
	triangle_count = (rows - 1) * (cols - 1) * 2;

	//vertices
	vertices = new Vertex[vertex_count];
	int* earliest_instance = new int[vertex_count];
	for (DWORD y = 0; y < rows; y++)
	{
		for (DWORD x = 0; x < cols; x++)
		{
			int index = (y * cols) + x;
			vertices[index].position = h_map_info.heightMap[index];
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
			earliest_instance[index] = (triangle_count * 3) + 1;
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
			earliest_instance[index] = min(earliest_instance[index], indices[idx_set + 5]);

			index = y * cols + x + 1;
			indices[idx_set + 4] = index;
			earliest_instance[index] = min(earliest_instance[index], indices[idx_set + 4]);

			index = (y + 1) * cols + x;
			indices[idx_set + 3] = index;
			earliest_instance[index] = min(earliest_instance[index], indices[idx_set + 3]);

			index = (y + 1) * cols + x;
			indices[idx_set + 2] = index;
			earliest_instance[index] = min(earliest_instance[index], indices[idx_set + 2]);

			index = y * cols + x + 1;
			indices[idx_set + 1] = index;
			earliest_instance[index] = min(earliest_instance[index], indices[idx_set + 1]);

			index = (y + 1) * cols + x + 1;
			indices[idx_set] = index;
			earliest_instance[index] = min(earliest_instance[index], indices[idx_set]);

			idx_set += 6; // next quad
		}
	}

	//normals
	float vecX, vecY, vecZ;
	XMVECTOR edge1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR edge2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	int faces = 0;
	int max_faces = 0;
	int increment = 1;
	for (int i = 0; i < vertex_count; i += increment)
	{
		for (int j = earliest_instance[i]; j < triangle_count && faces < 6; j++)
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

		for (int n = 1; n < increment; n++)
		{
			if (i < vertex_count - n)
			{
				vertices[i + n].normal = vertices[i].normal;
			}
		}

		normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		faces = 0;
	}

	Memory::SafeDeleteArr(earliest_instance);
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
		geometry_list->push_back(new Terrain("..\\Resources\\HeightMap.bmp", grid_x - 1, grid_y + 1));
		geometry_list->back()->init(_app, _cb, cam, dev_con, c_buff);
		Terrain* t = static_cast<Terrain*>(geometry_list->back());
	}
	if (neighbours[TOP] == nullptr)
	{
		geometry_list->push_back(new Terrain("..\\Resources\\HeightMap.bmp", grid_x, grid_y + 1));
		geometry_list->back()->init(_app, _cb, cam, dev_con, c_buff);
		Terrain* t = static_cast<Terrain*>(geometry_list->back());
	}
	if (neighbours[TOP_RIGHT] == nullptr)
	{
		geometry_list->push_back(new Terrain("..\\Resources\\HeightMap.bmp", grid_x + 1, grid_y + 1));
		geometry_list->back()->init(_app, _cb, cam, dev_con, c_buff);
		Terrain* t = static_cast<Terrain*>(geometry_list->back());
	}
	if (neighbours[LEFT] == nullptr)
	{
		geometry_list->push_back(new Terrain("..\\Resources\\HeightMap.bmp", grid_x - 1, grid_y));
		geometry_list->back()->init(_app, _cb, cam, dev_con, c_buff);
		Terrain* t = static_cast<Terrain*>(geometry_list->back());
	}
	if (neighbours[RIGHT] == nullptr)
	{
		geometry_list->push_back(new Terrain("..\\Resources\\HeightMap.bmp", grid_x + 1, grid_y));
		geometry_list->back()->init(_app, _cb, cam, dev_con, c_buff);
		Terrain* t = static_cast<Terrain*>(geometry_list->back());
	}
	if (neighbours[BOTTOM_LEFT] == nullptr)
	{
		geometry_list->push_back(new Terrain("..\\Resources\\HeightMap.bmp", grid_x - 1, grid_y - 1));
		geometry_list->back()->init(_app, _cb, cam, dev_con, c_buff);
		Terrain* t = static_cast<Terrain*>(geometry_list->back());
	}
	if (neighbours[BOTTOM] == nullptr)
	{
		geometry_list->push_back(new Terrain("..\\Resources\\HeightMap.bmp", grid_x, grid_y - 1));
		geometry_list->back()->init(_app, _cb, cam, dev_con, c_buff);
		Terrain* t = static_cast<Terrain*>(geometry_list->back());
	}
	if (neighbours[BOTTOM_RIGHT] == nullptr)
	{
		geometry_list->push_back(new Terrain("..\\Resources\\HeightMap.bmp", grid_x + 1, grid_y - 1));
		geometry_list->back()->init(_app, _cb, cam, dev_con, c_buff);
		Terrain* t = static_cast<Terrain*>(geometry_list->back());
	}
}