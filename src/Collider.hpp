#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Collision
{

	inline bool checkAABB(glm::vec3 firstPosition, glm::vec3 firstScale, glm::vec3 secondPosition, glm::vec3 secondScale)
	{
		return firstPosition.x + firstScale.x * 0.5f >= secondPosition.x - secondScale.x * 0.5f &&
			firstPosition.x - firstScale.x * 0.5f <= secondPosition.x + secondScale.x * 0.5f &&
			firstPosition.y + firstScale.y * 0.5f >= secondPosition.y - secondScale.y * 0.5f &&
			firstPosition.y - firstScale.y * 0.5f <= secondPosition.y + secondScale.y * 0.5f &&
			firstPosition.z + firstScale.z * 0.5f >= secondPosition.z - secondScale.z * 0.5f &&
			firstPosition.z - firstScale.z * 0.5f <= secondPosition.z + secondScale.z * 0.5f;
	}

	inline bool checkAABBToSphere(glm::vec3 aabbPosition, glm::vec3 aabbScale, glm::vec3 spherePosition, glm::vec3 sphereScale)
	{
		glm::vec3 dir = glm::normalize(aabbPosition - spherePosition);

		glm::vec3 point = spherePosition + dir * (sphereScale * 0.5f);

		return checkAABB(aabbPosition, aabbScale, point, glm::vec3(0.f));
	}

	inline bool checkSphere(glm::vec3 firstPosition, glm::vec3 firstScale, glm::vec3 secondPosition, glm::vec3 secondScale)
	{
		glm::vec3 diff = firstPosition - secondPosition;

		float distance = glm::length(diff);
		float radiusSum = firstScale.x + secondScale.x;

		return distance <= radiusSum * 0.5f;
	}

	inline glm::vec3 getOverlap(glm::vec3 firstPosition, glm::vec3 firstScale, glm::vec3 secondPosition, glm::vec3 secondScale)
	{
		glm::vec3 dis = firstPosition - secondPosition;
		glm::vec3 dif = (firstScale + secondScale) * 0.5f;

		float x = (dis.x < 0) ? dis.x + dif.x : dis.x - dif.x;
		float y = (dis.y < 0) ? dis.y + dif.y : dis.y - dif.y;
		float z = (dis.z < 0) ? dis.z + dif.z : dis.z - dif.z;

		return glm::vec3(x, y, z);
	}
};

enum ColliderType
{
	AABB,
	OBB,
	Sphere,
	Capsule,
	Convex,
	NONE
};

class Collider
{

public:

	Collider()
	{
		this->m_pAnchor = nullptr;
		this->m_scale = glm::vec3(0.f);
		this->m_colliderType = ColliderType::NONE;
	}

	Collider(glm::vec3* anchor, glm::vec3 scale, ColliderType colliderType)
	{
		this->m_pAnchor = anchor;
		this->m_scale = scale;
		this->m_colliderType = colliderType;
	}

	~Collider()
	{

	}

	// getter
	glm::vec3* getAnchorPtr() { return this->m_pAnchor; }
	glm::vec3 getAnchor() { return *this->m_pAnchor; }
	glm::vec3 getScale() const { return this->m_scale; }
	glm::vec3* getScalePtr() { return &this->m_scale; }

	glm::mat4 getModelMatrix() const { return glm::scale(glm::translate(glm::mat4(1.0f), *m_pAnchor), m_scale); }

	ColliderType getColliderType() const { return this->m_colliderType; }


	// setter
	void setScale(glm::vec3 scale) { this->m_scale = scale; }


private:

	glm::vec3* m_pAnchor;
	glm::vec3 m_scale;

	ColliderType m_colliderType;
};