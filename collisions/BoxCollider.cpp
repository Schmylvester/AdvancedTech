#include "BoxCollider.h"
#include <SimpleMath.h>

BoxCollider::BoxCollider(Transform* object_transform)
{
	m_transform = object_transform;
}

bool BoxCollider::intersect(BoxCollider* col)
{
	//so like I project each object onto the axis perpendicular to this object
	//then I like project them both onto the other object's axis
	//after that I see if there's an overlap on all those?
	//if there is, they're colliding I guess
	//but if I'm projecting, I'll get a 2d shape, so I need to do it twice?

	//like literally how the heck do I find the axis I need to project onto?
	//XMPlaneDotNormal?

	Transform other_tfm = col->getTransform();
	Vector3 lowest = Vector3(m_transform->getPos().x - m_transform->getScale().x,
		m_transform->getPos().y - m_transform->getScale().y,
		m_transform->getPos().z - m_transform->getScale().z);
	Vector3 highest = Vector3(m_transform->getPos().x + m_transform->getScale().x,
		m_transform->getPos().y + m_transform->getScale().y,
		m_transform->getPos().z + m_transform->getScale().z);

	Vector3 _lowest = Vector3(other_tfm.getPos().x - other_tfm.getScale().x,
		other_tfm.getPos().y - other_tfm.getScale().y,
		other_tfm.getPos().z - other_tfm.getScale().z);
	Vector3 _highest = Vector3(other_tfm.getPos().x + other_tfm.getScale().x,
		other_tfm.getPos().y + other_tfm.getScale().y,
		other_tfm.getPos().z + other_tfm.getScale().z);

	bool r_val = (lowest.x <= _highest.x && highest.x >= _lowest.x) &&
		(lowest.y <= _highest.y && highest.y >= _lowest.y) &&
		(lowest.z <= _highest.z && highest.z >= _lowest.z);
	return r_val;
}

bool BoxCollider::intersect(SphereCollider * col)
{
	return false;
}

void BoxCollider::collide(Collider* col)
{
	Collider::collide(col);
}