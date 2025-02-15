#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Collision
{

	inline bool checkBoundingBoxCollision(glm::vec3 firstPosition, glm::vec3 firstScale, glm::vec3 secondPosition, glm::vec3 secondScale)
	{
		return firstPosition.x + firstScale.x / 2 > secondPosition.x - secondScale.x / 2 &&
			firstPosition.x - firstScale.x / 2 < secondPosition.x + secondScale.x / 2 &&
			firstPosition.y + firstScale.y / 2 > secondPosition.y - secondScale.y / 2 &&
			firstPosition.y - firstScale.y / 2 < secondPosition.y + secondScale.y / 2 &&
			firstPosition.z + firstScale.z / 2 > secondPosition.z - secondScale.z / 2 &&
			firstPosition.z - firstScale.z / 2 < secondPosition.z + secondScale.z / 2;
	}

	inline bool checkBoundingBoxCollisionX(glm::vec3 firstPosition, glm::vec3 firstScale, glm::vec3 secondPosition, glm::vec3 secondScale)
	{
		return firstPosition.x + firstScale.x / 2 > secondPosition.x - secondScale.x / 2 &&
			firstPosition.x - firstScale.x / 2 < secondPosition.x + secondScale.x / 2;
	}

	inline bool checkBoundingBoxCollisionY(glm::vec3 firstPosition, glm::vec3 firstScale, glm::vec3 secondPosition, glm::vec3 secondScale)
	{
		return firstPosition.y + firstScale.y / 2 > secondPosition.y - secondScale.y / 2 &&
			firstPosition.y - firstScale.y / 2 < secondPosition.y + secondScale.y / 2;
	}

	inline bool checkBoundingBoxCollisionZ(glm::vec3 firstPosition, glm::vec3 firstScale, glm::vec3 secondPosition, glm::vec3 secondScale)
	{
		return firstPosition.z + firstScale.z / 2 > secondPosition.z - secondScale.z / 2 &&
			firstPosition.z - firstScale.z / 2 < secondPosition.z + secondScale.z / 2;
	}


	inline glm::vec3 getOverlap(glm::vec3 firstPosition, glm::vec3 firstScale, glm::vec3 secondPosition, glm::vec3 secondScale)
	{
		glm::vec3 dif = firstPosition - secondPosition;
		glm::vec3 dis = (firstScale + secondScale) / 2.f;

		return glm::vec3(glm::abs(dif - dis));;
	}
};

enum ColliderType
{
	BoundingBox,
	Circular,
	NONE
};

class Collider
{

public:

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
	glm::vec3* getAnchorPositionPtr() { return this->m_pAnchor; }
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