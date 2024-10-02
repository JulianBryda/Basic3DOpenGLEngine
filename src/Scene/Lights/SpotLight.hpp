#pragma once
#include "glm/glm.hpp"

#include "Light.hpp"

class SpotLight : public Light
{

public:


	SpotLight(const char* name, glm::vec3 direction, glm::vec3 color) : Light(name, color, LightType::Spot)
	{
		this->m_direction = direction;

		this->m_innerCone = 0.95f;
		this->m_outerCone = 0.90f;
	}

	~SpotLight()
	{

	}


	// getter
	inline glm::vec3 getDirection() const { return this->m_direction; }
	inline glm::vec3* getDirectionPtr() { return &this->m_direction; }

	float getInnerCone() const { return this->m_innerCone; }
	float getOuterCone() const { return this->m_outerCone; }

	float* getInnerConePtr() { return &this->m_innerCone; }
	float* getOuterConePtr() { return &this->m_outerCone; }

	glm::mat4 getViewMatrix() override
	{
		return glm::lookAt(this->position, this->position + this->m_direction, glm::vec3(0.f, 0.f, 1.f));
	}

	glm::mat4 getProjectionMatrix() override
	{
		return glm::perspective(m_outerCone, 1.f, 0.1f, 100.f);

	}

	// setter
	inline void setPosition(glm::vec3 position) { this->position = position; }




private:

	glm::vec3 m_direction;

	float m_innerCone, m_outerCone;
};