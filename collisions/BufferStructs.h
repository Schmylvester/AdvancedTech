#pragma once
#include "DXUtil.h"
#include <string>

struct VertexBuffer
{
	std::string id;
	ID3D11Buffer* buffer;
};

struct IndexBuffer
{
	std::string id;
	ID3D11Buffer* buffer;
};