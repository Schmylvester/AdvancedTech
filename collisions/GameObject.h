#pragma once
#include "Transform.h"
#include "Collider.h"
#include "Geometry.h"

class DXApp;
class CBPerObject;
class Camera;
class ID3D11DeviceContext;
class ID3D11Buffer;

class GameObject
{
public:
	GameObject() = default;
	~GameObject();

	void init(Shape shape, DXApp* _app, CBPerObject * _cb, Camera * cam,
		ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff);
	void init(Geometry * shape, DXApp * _app, CBPerObject * _cb, Camera * cam,
		ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff);

	virtual void collision(Collider* other_col, CollisionClassifier type);
	Transform* getTransform();
	Geometry* getGeometry();
	void draw();

private:
	Collider* m_collider = nullptr;
	Geometry* m_geometry = nullptr;
	Transform m_transform;
};

