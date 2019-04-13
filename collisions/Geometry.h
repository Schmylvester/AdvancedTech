#pragma once
#include "DXUtil.h"
#include "Vertex.h"
#include "Transform.h"
#include "Collider.h"

class GameObject;
class DXApp;
class CBPerObject;
class Camera;

enum class Shape
{
	Cube,
	Pyramid,
	Plane,
	Frustum,
};

class Geometry
{
public:
	Geometry() = default;
	virtual ~Geometry();
	virtual void init(DXApp * _app, CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff, GameObject * _owner);
	int getVertCount() { return vertex_count; }
	int getIndexCount() { return index_count; }
	int getTriangleCount() { return triangle_count; }
	Vertex* getVertices() { return vertices; }
	DWORD* getIndices() { return indices; }
	void draw(XMMATRIX _world_matrix);
protected:
	int index_count;
	int vertex_count;
	int triangle_count;

	DWORD* indices = nullptr;
	Vertex* vertices = nullptr;
	ID3D11Buffer* m_vtx_buffer = nullptr;
	ID3D11Buffer* m_idx_buffer = nullptr;
	GameObject* m_owner;
private:
	CBPerObject* m_cb = nullptr;
	Camera* m_cam = nullptr;
	ID3D11DeviceContext* m_dev_con = nullptr;
	ID3D11Buffer* m_obj_buff = nullptr;
};

