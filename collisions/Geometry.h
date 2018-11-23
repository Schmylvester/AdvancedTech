#pragma once
#include "DXUtil.h"
#include "Vertex.h"
#include "Transform.h"

class DXApp;
class CBPerObject;
class Camera;

class Geometry
{
public:
	Geometry() = default;
	virtual ~Geometry();
	virtual void init(DXApp* _app, CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff);
	int getVertCount()		{ return vertex_count; }
	int getIndexCount()		{ return index_count; }
	int getTriangleCount()	{ return triangle_count; }
	Vertex* getVertices()	{ return vertices; }
	DWORD* getIndices()		{ return indices; }
	Transform* getTransform() { return &m_transform; }

	void draw();
	
protected:
	Transform m_transform;

	int index_count;
	int vertex_count;
	int triangle_count;

	DWORD* indices = nullptr;
	Vertex* vertices = nullptr;
	ID3D11Buffer* m_vtx_buffer = nullptr;
	ID3D11Buffer* m_idx_buffer = nullptr;

private:
	CBPerObject* m_cb = nullptr;
	Camera* m_cam = nullptr;
	ID3D11DeviceContext* m_dev_con = nullptr;
	ID3D11Buffer* m_obj_buff = nullptr;
};

