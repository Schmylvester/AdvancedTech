#include "TerrainGeometry.h"
#include "DXApp.h"
#include <fstream>


TerrainGeometry::TerrainGeometry(const char * _name, int x, int y)
{
	file_name = _name;
	grid_x = x;
	grid_y = y;
}

void TerrainGeometry::init(DXApp * _app, CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff, GameObject * _owner)
{
	Geometry::init(_app, _cb, cam, dev_con, c_buff, _owner);
	m_vtx_buffer = _app->getVertexBuffer(file_name, this);
	m_idx_buffer = _app->getIndexBuffer(file_name, this);
}

void TerrainGeometry::createVerts(ImageMapInfo h_map_info)
{
	width = h_map_info.img_width;
	height = h_map_info.img_height;
	int cols = h_map_info.img_width;
	int rows = h_map_info.img_height;

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
			vertices[index].position = h_map_info.map[index];
			float shade = ((float)(rand() % 100)) / 500;

			if (vertices[index].position.y >= 10)
			{
				vertices[index].colour = XMFLOAT4(shade, 0.4f + shade, shade, 1.0f);
			}
			else
			{
				vertices[index].colour = XMFLOAT4(shade, shade, 0.4f + shade, 1.0f);
			}
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
	getNormals(earliest_instance);
}

void TerrainGeometry::getNormals(int* earliest_instance)
{
	int file_pos = 0;
	float vecX, vecY, vecZ;
	XMVECTOR edge1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR edge2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	int faces = 0;
	int max_faces = 0;
	for (int i = 0; i < vertex_count; i++)
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

		normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		faces = 0;
	}
}

std::string TerrainGeometry::removeFileExt(const char* in)
{
	std::string return_string = "";

	for (int i = 0; !(in[i] == '.' && in[i + 1] == 'b') && i < 600; i++)
	{
		return_string += in[i];
	}

	return return_string;
}
