#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Light.hpp"

class DirectionalLight : public Light
{

public:


	DirectionalLight(const char* name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : Light(name, ambient, diffuse, specular, LightType::Directional)
	{
		projection = glm::mat4(1.f);
	}

	~DirectionalLight()
	{

	}


	// getter
	inline glm::vec3 getDirection() const { return this->position; }
	inline glm::vec3* getDirectionPtr() { return &this->position; }

	glm::mat4 getProjectionMatrix() override
	{
		return projection;
	}

	glm::mat4 getProjectionMatrix(glm::vec3 min, glm::vec3 max) override
	{
		glm::mat4 view = getViewMatrix();

		glm::vec4 lightSpaceMin = view * glm::vec4(min, 1.f);
		glm::vec4 lightSpaceMax = view * glm::vec4(max, 1.f);

		float left = lightSpaceMax.x;
		float right = lightSpaceMin.x;
		float bottom = lightSpaceMin.y;
		float top = lightSpaceMax.y;
		float zNear = -lightSpaceMax.z;
		float zFar = -lightSpaceMin.z;

		projection = glm::ortho(left, right, bottom, top, zNear, zFar);
		return projection;
	}

	// setter
	inline void setDirection(glm::vec3 direction) { this->position = direction; }

private:

	glm::mat4 projection;
};