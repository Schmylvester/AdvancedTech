#include "GameObject.h"
#include "GeometryIncludes.h"
#include "DXUtil.h"
#include "DXApp.h"
#include "CollisionManager.h"

GameObject::~GameObject()
{
	Memory::SafeDelete(m_geometry);
	Memory::SafeDelete(m_collider);
}

void GameObject::init(Shape shape, DXApp * _app, CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff)
{
	m_cam = cam;
	switch (shape)
	{
	case Shape::Cube:
		m_geometry = new Cube();
		break;
	case Shape::Plane:
		m_geometry = new PlaneObject();
		break;
	case Shape::Sphere:
		m_geometry = new Sphere();
		break;
	default:
		break;
	}
	m_geometry->init(_app, _cb, cam, dev_con, c_buff, this);
}

void GameObject::addCollider(Collider * col, CollisionManager * collision_manager, float mass)
{
	collision_manager->addCollider(col);
	m_collider = col;
	if (mass > 0)
	{
		m_physics = std::make_unique<PhysicsBody>(m_collider, &m_transform, mass);
	}
}

void GameObject::collision(CollisionData col)
{
	if (col.type == CollisionClassifier::Ongoing_Collision)
	{
		//get the masses of the objects to determine which moves more
		float my_mass = m_physics->getMass();
		float their_mass = col.other_object->getObject()->getPhysics()->getMass();
		float relative_mass = 1 - (my_mass / (my_mass + their_mass));
		//move out of the object
		m_transform.translate(col.collision_direction * col.penetration * relative_mass);
		m_collider->objectMoved();
	}
	if (col.type == CollisionClassifier::Collision_This_Frame)
	{
		if (m_physics != nullptr)
		{
			m_physics->addForceAtPoint(10.0f, m_transform.getPos() - col.other_object->getTransform()->getPos(), col.collision_direction);
		}
	}
}

Transform * GameObject::getTransform()
{
	return &m_transform;
}

Geometry * GameObject::getGeometry()
{
	return m_geometry;
}

void GameObject::update(float dt)
{
	if (m_physics != nullptr)
	{
		m_physics->tick(dt);
	}
}

void GameObject::draw()
{
	m_geometry->draw(m_transform.getWorldMatrix());
}
