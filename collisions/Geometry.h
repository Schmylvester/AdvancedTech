#pragma once
#include "DXUtil.h"
#include "Vertex.h"

class Geometry
{
public:
	Geometry();
	~Geometry();
	int getVertCount()		{ return vertex_count; }
	int getIndexCount()		{ return index_count; }
	int getTriangleCount()	{ return triangle_count; }
	Vertex* getVertices()	{ return vertices; }
	DWORD* getIndices()		{ return indices; }

private:
	int index_count;
	int vertex_count;
	int triangle_count;

	DWORD* indices;
	Vertex* vertices;
};

