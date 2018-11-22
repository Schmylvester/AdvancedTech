#pragma once
#include "DXUtil.h"
#include "Vertex.h"
#include "Transform.h"

class CBPerObject;
class Camera;
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
	Transform* getTransform() { return &m_transform; }

	void draw(CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff);
	
protected:
	Transform m_transform;

	int index_count;
	int vertex_count;
	int triangle_count;

	DWORD* indices;
	Vertex* vertices;
};

