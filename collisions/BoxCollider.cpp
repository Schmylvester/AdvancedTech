#include "Transform.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "GameObject.h"
#include <SimpleMath.h>
#include <iostream>

bool BoxCollider::checkIntersection(Vector3 point)
{
	for (int face = 0; face < 6; face++)
	{
		Vector3 projectionAxis = getFaceNormal(face);
		float a_projected[2] = { INFINITY, -INFINITY };
		float b_projected = point.Dot(projectionAxis);
		for (int vert = 0; vert < 8; vert++)
		{
			float aVert = m_current_vertices[vert].Dot(projectionAxis);
			a_projected[0] = min(a_projected[0], aVert);
			a_projected[1] = max(a_projected[1], aVert);
		}
		if (a_projected[0] >= b_projected || a_projected[1] <= b_projected)
		{
			return false;
		}
	}
	return true;
}

CollisionData BoxCollider::checkIntersection(BoxCollider* col)
{
	updateVerts();
	col->updateVerts();
	CollisionData collision_data;
	collision_data.other_object = col;
	collision_data.penetration = INFINITY;
	Vector3 projection_axes[15];
	for (int i = 0; i < 3; i++)
	{
		projection_axes[i] = getFaceNormal(i * 2);
		projection_axes[i + 3] = col->getFaceNormal(0);
	}
	for (int i = 6; i < 15; i++)
	{
		int a_idx = i % 3;
		int b_idx = (i / 3) + 1;
		projection_axes[i] = projection_axes[a_idx] * projection_axes[b_idx];
	}
	for (int axis = 0; axis < 15; axis++)
	{
		//projected floats are the min, the max, and a number that says if it is valid
		float a_projected[3] = { INFINITY, -INFINITY, -1 };
		float b_projected[3] = { INFINITY, -INFINITY, -1 };
		if (checkProjectionIntersect(projection_axes[axis], col, a_projected, b_projected))
		{
			int on_list_idx = searchList(&(colliding_this_frame), col);
			if (on_list_idx != -1)
			{
				colliding_this_frame.erase(colliding_last_frame.begin() + on_list_idx);
			}
			CollisionData ret_false;
			ret_false.did_collide = false;
			return ret_false;
		}
		float a_diff = b_projected[1] - a_projected[0];
		float b_diff = a_projected[1] - b_projected[0];
		float penetration = min(a_diff, b_diff);
		if (penetration < collision_data.penetration)
		{
			collision_data.penetration = penetration;
			collision_data.collision_direction = projection_axes[axis];
		}
	}
	collision_data.did_collide = true;
	collision_data.collision_direction.Normalize();
	return collision_data;
}

bool BoxCollider::checkProjectionIntersect(Vector3 a_pos, Vector3 b_pos,
	Vector3 a_size, Vector3 b_size, Vector3 a_x_proj, Vector3 a_y_proj,
	Vector3 a_z_proj, Vector3 b_x_proj, Vector3 b_y_proj,
	Vector3 b_z_proj, Vector3 projection)
{
	return (abs((b_pos - a_pos).Length())) >
		abs((a_size.x / 2 * a_x_proj).Dot(projection)) +
		abs((a_size.y / 2 * a_y_proj).Dot(projection)) +
		abs((a_size.z / 2 * a_z_proj).Dot(projection)) +
		abs((b_size.x / 2 * b_x_proj).Dot(projection)) +
		abs((b_size.y / 2 * b_y_proj).Dot(projection)) +
		abs((b_size.z / 2 * b_z_proj).Dot(projection));
}

bool BoxCollider::checkProjectionIntersect(Vector3 projection, BoxCollider* other, float* a_projected, float* b_projected)
{
	for (int vert = 0; vert < 8; vert++)
	{
		float aVert = m_current_vertices[vert].Dot(projection);
		float bVert = other->getVert(vert).Dot(projection);
		if (!isnan(aVert))
		{
			a_projected[0] = min(a_projected[0], aVert);
			a_projected[1] = max(a_projected[1], aVert);
			a_projected[2] = 1;
		}
		if (!isnan(bVert))
		{
			b_projected[0] = min(b_projected[0], bVert);
			b_projected[1] = max(b_projected[1], bVert);
			b_projected[2] = 1;
		}
	}
	return (a_projected[0] >= b_projected[1]
		|| a_projected[1] <= b_projected[0]
		|| a_projected[2] < 0
		|| b_projected[2] < 0);
}

CollisionData BoxCollider::checkIntersection(SphereCollider * col)
{
	return CollisionData();
}

BoxCollider::BoxCollider(GameObject * _game_object)
	: Collider(_game_object)
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
				int i = (x * 4) + (y * 2) + z;
				m_vertices[i] = Vector3(v_x, v_y, v_z) *0.5f;
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

void BoxCollider::updateVerts()
{
	Transform* obj_transform = getTransform();
	Vector3 scale = obj_transform->getScale();
	Vector4 rot = obj_transform->getQuaternion();
	for (int i = 0; i < 8; i++)
	{
		m_current_vertices[i] = Vector3(
			m_vertices[i].x * scale.x,
			m_vertices[i].y * scale.y,
			m_vertices[i].z * scale.z);
		m_current_vertices[i] += obj_transform->getPos();

		Vector3 dir = m_current_vertices[i] - obj_transform->getPos();
		dir = rotatePoint(dir, rot);
		m_current_vertices[i] = dir + obj_transform->getPos();
	}
}

void BoxCollider::getClosestFace(Vector3 point, Vector3& position, Vector3& normal)
{
	float closest_distance = INFINITY;
	for (int face = 0; face < 6; face++)
	{
		Vector3 verts[4];
		for (int i = 0; i < 4; i++)
		{
			verts[i] = m_current_vertices[faces[face][i]];
		}
		Vector3 avg = (verts[0] + verts[1] + verts[2] + verts[3]) / 4;
		float dist = (avg - point).Length();
		if (dist < closest_distance)
		{
			position = avg;
			normal = getFaceNormal(face);
			normal.Normalize();
			closest_distance = dist;
		}
	}
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
	normal.Normalize();
	return normal;
}

Vector3 BoxCollider::rotatePoint(Vector3 point, Vector4 rotation)
{
	return Vector3((rotation.w * point.x) + (rotation.x) + (rotation.y * point.z) - (rotation.z * point.y),
		(rotation.w * point.y) - (rotation.x * point.z) + (rotation.y) + (rotation.z * point.x),
		(rotation.w * point.z) + (rotation.x * point.y) - (rotation.y * point.x) + (rotation.z));
}
