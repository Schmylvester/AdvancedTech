#include "Terrain.h"
#include "DXApp.h"
#include "Pathfinder.h"
#include <fstream>
#include "GameObject.h"
#include "TerrainGeometry.h"
#include "NavMesh.h"

void errorBox(LPCSTR message);

NavigationCell* g_cells;
Terrain::~Terrain()
{
	nav_thread.join();
	Memory::SafeDelete(nav_mesh);
	Memory::SafeDeleteArr(h_map_info.map);
}

void Terrain::init(const char* _name, int x, int y, DXApp * _app, CBPerObject * _cb, Camera * cam,
	ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff)
{
	file_name = _name;
	grid_x = x;
	grid_y = y;

	m_geometry = new TerrainGeometry(_name, x, y);
	loadFile();

	width = h_map_info.img_width;
	length = h_map_info.img_height;
	static_cast<TerrainGeometry*>(m_geometry)->createVerts(h_map_info);
	m_geometry->init(_app, _cb, cam, dev_con, c_buff, this);

	m_transform.translate(terrain_scale * grid_x * (width), 0, terrain_scale * grid_y * (length));
}

void initNavMesh();
void setPointers(NavMesh* nav_mesh, ImageMapInfo* map);
NavMesh* Terrain::addNavMesh()
{
	nav_mesh = new NavMesh();
	setPointers(nav_mesh, &h_map_info);
	nav_thread = std::thread(initNavMesh);
	return nav_mesh;
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
			cells[index] = NavigationCell(x, height / height_factor, y, h_map_info.map[index]);
			if (height / height_factor <= 10)
			{
				cells[index].setAccessible(false);
			}
		}
	}

	Memory::SafeDeleteArr(bitmap_image);
}

bool Terrain::playerInCell(int player_x, int player_z)
{
	int left = (grid_x * (terrain_scale * width)) - 64;
	int right = ((grid_x + 1) * (terrain_scale * width)) - 64;
	int bottom = (grid_y * (terrain_scale * length)) - 64;
	int top = ((grid_y + 1) * (terrain_scale * length)) - 64;
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

void Terrain::createNeighbours(std::vector<Terrain*>* geometry_list, DXApp * _app,
	CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff)
{
	if (neighbours[TOP_LEFT] == nullptr)
	{
		geometry_list->push_back(new Terrain());
		geometry_list->back()->init(file_name, grid_x - 1, grid_y + 1, _app, _cb, cam, dev_con, c_buff);
		Terrain* t = static_cast<Terrain*>(geometry_list->back());
	}
	if (neighbours[TOP] == nullptr)
	{
		geometry_list->push_back(new Terrain());
		geometry_list->back()->init(file_name, grid_x, grid_y + 1, _app, _cb, cam, dev_con, c_buff);
		Terrain* t = static_cast<Terrain*>(geometry_list->back());
	}
	if (neighbours[TOP_RIGHT] == nullptr)
	{
		geometry_list->push_back(new Terrain());
		geometry_list->back()->init(file_name, grid_x + 1, grid_y + 1, _app, _cb, cam, dev_con, c_buff);
		Terrain* t = static_cast<Terrain*>(geometry_list->back());
	}
	if (neighbours[LEFT] == nullptr)
	{
		geometry_list->push_back(new Terrain());
		geometry_list->back()->init(file_name, grid_x - 1, grid_y, _app, _cb, cam, dev_con, c_buff);
		Terrain* t = static_cast<Terrain*>(geometry_list->back());
	}
	if (neighbours[RIGHT] == nullptr)
	{
		geometry_list->push_back(new Terrain());
		geometry_list->back()->init(file_name, grid_x + 1, grid_y, _app, _cb, cam, dev_con, c_buff);
		Terrain* t = static_cast<Terrain*>(geometry_list->back());
	}
	if (neighbours[BOTTOM_LEFT] == nullptr)
	{
		geometry_list->push_back(new Terrain());
		geometry_list->back()->init(file_name, grid_x - 1, grid_y - 1, _app, _cb, cam, dev_con, c_buff);
		Terrain* t = static_cast<Terrain*>(geometry_list->back());
	}
	if (neighbours[BOTTOM] == nullptr)
	{
		geometry_list->push_back(new Terrain());
		geometry_list->back()->init(file_name, grid_x, grid_y - 1, _app, _cb, cam, dev_con, c_buff);
		Terrain* t = static_cast<Terrain*>(geometry_list->back());
	}
	if (neighbours[BOTTOM_RIGHT] == nullptr)
	{
		geometry_list->push_back(new Terrain());
		geometry_list->back()->init(file_name, grid_x + 1, grid_y - 1, _app, _cb, cam, dev_con, c_buff);
		Terrain* t = static_cast<Terrain*>(geometry_list->back());
	}
}

NavigationCell * Terrain::getCell(int index)
{
	if (index == -1)
	{
		NavigationCell* return_cell = nullptr;
		do
		{
			return_cell = &(cells[rand() % 16384]);
		} while (!return_cell->getAccessible());
		return return_cell;
	}
	else
	{
		return &(cells[index]);
	}
}