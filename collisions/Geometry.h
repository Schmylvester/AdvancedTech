#pragma once
#include "DXUtil.h"
#include "Vertex.h"

class Geometry
{
public:
	Geometry() = default;
	virtual ~Geometry();
	virtual void init() = 0;
	int getVertCount()		{ return vertex_count; }
	int getIndexCount()		{ return index_count; }
	int getTriangleCount()	{ return triangle_count; }
	Vertex* getVertices()	{ return vertices; }
	DWORD* getIndices()		{ return indices; }

protected:
	int index_count;
	int vertex_count;
	int triangle_count;

	DWORD* indices;
	Vertex* vertices;
};

