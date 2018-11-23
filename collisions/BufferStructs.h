#pragma once
#include "DXUtil.h"

enum class GeometryID
{
	PLANE,
	CUBE,
	PYRAMID,
	FRUSTUM
};

struct VertexBuffer
{
	GeometryID id;
	ID3D11Buffer* buffer;
};

struct IndexBuffer
{
	GeometryID id;
	ID3D11Buffer* buffer;
};