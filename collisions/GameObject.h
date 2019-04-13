#pragma once
#include "Transform.h"
#include "Collider.h"
#include "Geometry.h"
#include "PhysicsBody.h"
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
	void init(const char * file, int x, int y, DXApp * _app,
		CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con, 
		ID3D11Buffer * c_buff);
	
	void addCollider(Collider* col, CollisionManager* collision_manager, bool add_physics);
	virtual void collision(Collider* other_col, CollisionClassifier type);
	Transform* getTransform();
	Geometry* getGeometry();
	virtual void update(float dt);
	void draw();

protected:
	Camera* m_cam = nullptr;
	Collider* m_collider = nullptr;
	Geometry* m_geometry = nullptr;
	Transform m_transform;

	XMVECTOR rotDir = XMVectorSet(0, 0, 1, 1);
	float rotation = 1;
	std::unique_ptr<PhysicsBody> m_physics = nullptr;
};

