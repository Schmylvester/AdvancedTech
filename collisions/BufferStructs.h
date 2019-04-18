#pragma once
#include "DXUtil.h"
#include "Geometry.h"
#include <string>

struct VertexBuffer
{
	std::string id;
	ID3D11Buffer* buffer;
};

struct IndexBuffer
{
	Shape id;
	ID3D11Buffer* buffer;
};