#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Collision
{

	inline bool checkBoundingBoxCollision(glm::vec3 firstPosition, glm::vec3 firstScale, glm::vec3 secondPosition, glm::vec3 secondScale)
	{
		//NOTE: maybe the z axis collision detection is broken
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

	//NOTE: maybe the z axis collision detection is broken
	inline bool checkBoundingBoxCollisionZ(glm::vec3 firstPosition, glm::vec3 firstScale, glm::vec3 secondPosition, glm::vec3 secondScale)
	{
		return firstPosition.z + firstScale.z / 2 > secondPosition.z - secondScale.z / 2 &&
			firstPosition.z - firstScale.z / 2 < secondPosition.z + secondScale.z / 2;
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
		this->anchor = anchor;
		this->scale = scale;
		this->colliderType = colliderType;
	}

	~Collider()
	{

	}

	// getter
	glm::vec3* getAnchorPositionPtr() { return this->anchor; }
	glm::vec3 getScale() const { return this->scale; }
	glm::vec3* getScalePtr() { return &this->scale; }

	glm::mat4 getModelMatrix() const { return glm::scale(glm::translate(glm::mat4(1.0f), *anchor), scale); }

	ColliderType getColliderType() const { return this->colliderType; }


	// setter
	void setScale(glm::vec3 scale) { this->scale = scale; }


private:

	glm::vec3* anchor;
	glm::vec3 scale;

	ColliderType colliderType;
};