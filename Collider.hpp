#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

class Collider
{

public:

	Collider(glm::vec3* anchor, glm::vec3 scale)
	{
		this->anchor = anchor;
		this->scale = scale;
	}

	// getter
	glm::vec3* getAnchorPositionPtr() { return anchor; }
	glm::vec3 getScale() const { return scale; }
	glm::vec3* getScalePtr() { return &scale; }

	glm::mat4 getModelMatrix() const { return glm::translate(glm::mat4(1.0f), *anchor); }
	glm::mat4 getScaleMatrix() const { return glm::scale(glm::mat4(1.0f), scale); }


	// setter
	void setScale(glm::vec3 scale) { this->scale = scale; }


private:

	glm::vec3* anchor;
	glm::vec3 scale;

};