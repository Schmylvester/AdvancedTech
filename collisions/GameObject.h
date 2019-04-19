#pragma once
#include "Transform.h"
#include "Geometry.h"
#include <memory>

class CollisionManager;
class DXApp;
class CBPerObject;
class Camera;
class ID3D11DeviceContext;
class ID3D11Buffer;

class GameObject
{
public:
	GameObject() = default;
	virtual ~GameObject();

	void init(Shape shape, DXApp* _app, CBPerObject * _cb, Camera * cam,
		ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff);
	
	Transform* getTransform();
	Geometry* getGeometry();
	virtual void update(float dt);
	void draw();

protected:
	Camera* m_cam = nullptr;
	Geometry* m_geometry = nullptr;
	Transform m_transform;
};

