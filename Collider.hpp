#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

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