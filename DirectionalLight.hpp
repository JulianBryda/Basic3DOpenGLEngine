#pragma once
#include "glm/glm.hpp"

#include "Light.hpp"

class DirectionalLight : public Light
{

public:


	DirectionalLight(const char* name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : Light(name, ambient, diffuse, specular, LightType::Directional)
	{

	}

	~DirectionalLight()
	{

	}


	// getter
	inline glm::vec3 getDirection() const { return this->position; }
	inline glm::vec3* getDirectionPtr() { return &this->position; }

	// setter
	inline void setDirection(glm::vec3 direction) { this->position = direction; }
};