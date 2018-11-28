#include "Terrain.h"
#include "DXApp.h"

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

	float height_factor = 20.0f;

	for (int x = 0; x < h_map_info.terrainWidth; x++)
	{
		for (int y = 0; y < h_map_info.terrainHeight; y++)
		{
			height = bitmap_image[height_mod];
			index = (h_map_info.terrainHeight * x) + y;
			h_map_info.heightMap[index].x = (float)y;
			h_map_info.heightMap[index].y = (float)(height) / height_factor;
			h_map_info.heightMap[index].z = (float)x;

			height_mod += 3;

		}
	}

	Memory::SafeDeleteArr(bitmap_image);
}

void Terrain::createGrid()
{
	int cols = h_map_info.terrainWidth;
	int rows = h_map_info.terrainHeight;

	vertex_count = rows * cols;
	triangle_count = (rows - 1) * (cols - 1) * 2;

	//vertices
	vertices = new Vertex[vertex_count];
	for (DWORD y = 0; y < rows; y++)
	{
		for (DWORD x = 0; x < cols; x++)
		{
			int index = (y * cols) + x;
			vertices[index].position = h_map_info.heightMap[index];
			float shade = ((float)(rand() % 100)) / 150;
			vertices[index].colour = XMFLOAT4(shade, shade, shade, 1.0f);
			vertices[index].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		}
	}

	//indices
	index_count = triangle_count * 3;
	indices = new DWORD[triangle_count * 3];
	int idx_set = 0;
	for (DWORD i = 0; i < rows - 1; i++)
	{
		for (DWORD j = 0; j < cols - 1; j++)
		{
			indices[idx_set + 5] = i * cols + j;
			indices[idx_set + 4] = i * cols + j + 1;
			indices[idx_set + 3] = (i + 1)*cols + j;
			indices[idx_set + 2] = (i + 1)*cols + j;
			indices[idx_set + 1] = i * cols + j + 1;
			indices[idx_set + 0] = (i + 1)*cols + j + 1;

			idx_set += 6; // next quad
		}
	}

	//normals
	std::vector<XMFLOAT3> tempNormal;

	XMFLOAT3 unnormalized = XMFLOAT3(0.0f, 0.0f, 0.0f);
	float vecX, vecY, vecZ;
	XMVECTOR edge1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR edge2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	for (int i = 0; i < triangle_count; ++i)
	{
		vecX = vertices[indices[(i * 3)]].position.x - vertices[indices[(i * 3) + 2]].position.x;
		vecY = vertices[indices[(i * 3)]].position.y - vertices[indices[(i * 3) + 2]].position.y;
		vecZ = vertices[indices[(i * 3)]].position.z - vertices[indices[(i * 3) + 2]].position.z;
		edge1 = XMVectorSet(vecX, vecY, vecZ, 0.0f);

		vecX = vertices[indices[(i * 3) + 2]].position.x - vertices[indices[(i * 3) + 1]].position.x;
		vecY = vertices[indices[(i * 3) + 2]].position.y - vertices[indices[(i * 3) + 1]].position.y;
		vecZ = vertices[indices[(i * 3) + 2]].position.z - vertices[indices[(i * 3) + 1]].position.z;
		edge2 = XMVectorSet(vecX, vecY, vecZ, 0.0f);

		XMStoreFloat3(&unnormalized, XMVector3Cross(edge1, edge2));
		tempNormal.push_back(unnormalized);
	}

	XMVECTOR normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	int facesUsing = 0;
	float tX;
	float tY;
	float tZ;

	for (int i = 0; i < vertex_count; ++i)
	{
		for (int j = 0; j < triangle_count; ++j)
		{
			if (indices[j * 3] == i ||
				indices[(j * 3) + 1] == i ||
				indices[(j * 3) + 2] == i)
			{
				tX = XMVectorGetX(normalSum) + tempNormal[j].x;
				tY = XMVectorGetY(normalSum) + tempNormal[j].y;
				tZ = XMVectorGetZ(normalSum) + tempNormal[j].z;

				normalSum = XMVectorSet(tX, tY, tZ, 0.0f);
				facesUsing++;
			}
		}

		normalSum = normalSum / facesUsing;

		normalSum = XMVector3Normalize(normalSum);

		vertices[i].normal.x = XMVectorGetX(normalSum);
		vertices[i].normal.y = XMVectorGetY(normalSum);
		vertices[i].normal.z = XMVectorGetZ(normalSum);

		normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		facesUsing = 0;
	}
}