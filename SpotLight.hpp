#pragma once
#include "glm/glm.hpp"

#include "Light.hpp"

class SpotLight : public Light
{

public:


	SpotLight(const char* name, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : Light(name, ambient, diffuse, specular, LightType::Spot)
	{
		this->m_direction = direction;

		this->m_cutOff = glm::cos(glm::radians(12.5f));
		this->m_outerCutOff = glm::cos(glm::radians(17.5f));
	}

	~SpotLight()
	{

	}


	// getter
	inline glm::vec3 getPosition() const { return this->position; }
	inline glm::vec3* getPositionPtr() { return &this->position; }

	inline glm::vec3 getDirection() const { return this->m_direction; }
	inline glm::vec3* getDirectionPtr() { return &this->m_direction; }

	inline float getCutOff() const { return this->m_cutOff; }
	inline float* getCutOffPtr() { return &this->m_cutOff; }

	inline float getOuterCutOff() const { return this->m_outerCutOff; }
	inline float* getOuterCutOffPtr() { return &this->m_outerCutOff; }

	// setter
	inline void setPosition(glm::vec3 position) { this->position = position; }

	inline void setCutOff(float cutOff) { this->m_cutOff = cutOff; }
	inline void setOuterCutOff(float outerCutOff) { this->m_outerCutOff = outerCutOff; }




private:

	glm::vec3 m_direction;

	float m_cutOff;
	float m_outerCutOff;
};