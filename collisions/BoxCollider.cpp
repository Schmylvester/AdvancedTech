#include "Transform.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include <SimpleMath.h>
#include <iostream>

bool BoxCollider::intersect(BoxCollider* col)
{
	for (int vert = 0; vert < 8; vert++)
	{
		updateVert(vert);
		col->updateVert(vert);
	}
	for (int face = 0; face < 6; face++)
	{
		Vector3 projectionAxis = getFaceNormal(face);
		float a_projected[2] = { INFINITY, -INFINITY };
		float b_projected[2] = { INFINITY, -INFINITY };
		for (int vert = 0; vert < 8; vert++)
		{
			float aVert = m_current_vertices[vert].Dot(projectionAxis);
			float bVert = col->getVert(vert).Dot(projectionAxis);
			a_projected[0] = min(a_projected[0], aVert);
			a_projected[1] = max(a_projected[1], aVert);
			b_projected[0] = min(b_projected[0], bVert);
			b_projected[1] = max(b_projected[1], bVert);
		}
		if (a_projected[0] >= b_projected[1] || a_projected[1] <= b_projected[0])
		{
			return false;
		}
	}
	return true;
}

bool BoxCollider::intersect(SphereCollider * col)
{
	return false;
}

BoxCollider::BoxCollider(Transform * _transform)
	: Collider(_transform)
{
	setFaces(0, 0, 2, 3, 1);
	setFaces(1, 4, 5, 7, 6);
	setFaces(2, 0, 1, 5, 4);
	setFaces(3, 2, 6, 7, 3);
	setFaces(4, 0, 4, 6, 2);
	setFaces(5, 1, 3, 7, 5);

	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			for (int z = 0; z < 2; z++)
			{
				float v_x = (1 - (x * 2));
				float v_y = (1 - (y * 2));
				float v_z = (1 - (z * 2));
				m_vertices[(x * 4) + (y * 2) + z] = Vector3(v_x, v_y, v_z);
			}
		}
	}
}

void BoxCollider::setFaces(int idx, int a, int b, int c, int d)
{
	faces[idx][0] = a;
	faces[idx][1] = b;
	faces[idx][2] = c;
	faces[idx][3] = d;
}

void BoxCollider::updateVert(int idx)
{
	m_current_vertices[idx] = 0.5f * Vector3(
		m_vertices[idx].x * m_object_transform->getScale().x,
		m_vertices[idx].y * m_object_transform->getScale().y,
		m_vertices[idx].z * m_object_transform->getScale().z);
	m_current_vertices[idx] += m_object_transform->getPos();

	Vector3 dir = m_current_vertices[idx] - m_object_transform->getPos();
	dir = toQuaternion(m_object_transform->getEulerAngles()) * dir;
	m_current_vertices[idx] = dir + m_object_transform->getPos();
}

Vector4 BoxCollider::toQuaternion(Vector3 euler)
{
	float cy = cos(euler.z * 0.5);
	float sy = sin(euler.z * 0.5);
	float cp = cos(euler.y * 0.5);
	float sp = sin(euler.y * 0.5);
	float cr = cos(euler.x * 0.5);
	float sr = sin(euler.x * 0.5);

	float w = cy * cp * cr + sy * sp * sr;
	float x = cy * cp * sr - sy * sp * cr;
	float y = sy * cp * sr + cy * sp * cr;
	float z = sy * cp * cr - cy * sp * sr;
	return Vector4(w, x, y, z);
}

Vector3 BoxCollider::getFaceNormal(int face)
{
	Vector3 verts[4];
	for (int i = 0; i < 4; i++)
	{
		verts[i] = m_current_vertices[faces[face][i]];
	}
	Vector3 a_normal = verts[0].Cross(verts[2]);
	Vector3 b_normal = verts[1].Cross(verts[3]);
	Vector3 normal = a_normal.Cross(b_normal);
	return normal;
}
