#pragma once
#include "glm/glm.hpp"

#include "Light.hpp"

class SpotLight : public Light
{

public:


	SpotLight(const char* name, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : Light(name, ambient, diffuse, specular, LightType::Spot)
	{
		this->m_direction = direction;

		this->m_lightAngle = 20.f;
	}

	~SpotLight()
	{

	}


	// getter
	inline glm::vec3 getDirection() const { return this->m_direction; }
	inline glm::vec3* getDirectionPtr() { return &this->m_direction; }

	inline float getLightAngle() const { return this->m_lightAngle; }
	inline float* getLightAnglePtr() { return &this->m_lightAngle; }

	glm::mat4 getProjectionMatrix() override
	{
		return glm::perspective(glm::radians(m_lightAngle), 1.f, 1.f, 25.f);

	}

	// setter
	inline void setPosition(glm::vec3 position) { this->position = position; }

	inline void setLightAngle(float angle) { this->m_lightAngle = angle; }




private:

	glm::vec3 m_direction;

	float m_lightAngle;
};