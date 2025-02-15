#pragma once
#include "Light.hpp"

#include <glm/gtc/matrix_transform.hpp>

class PointLight : public Light
{

public:


	PointLight(const char* name, glm::vec3 color) : Light(name, color, LightType::Point)
	{
		this->m_constant = 1.0f;
		this->m_linear = 0.09f;
		this->m_quadratic = 0.032f;
	}

	~PointLight()
	{

	}


	// getter
	inline float getConstant() const { return this->m_constant; }
	inline float* getConstantPtr() { return &this->m_constant; }

	inline float getLinear() const { return this->m_linear; }
	inline float* getLinearPtr() { return &this->m_linear; }

	inline float getQuadratic() const { return this->m_quadratic; }
	inline float* getQuadraticPtr() { return &this->m_quadratic; }

	glm::mat4 getViewMatrix() override
	{
		return glm::lookAt(this->position, glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f));
	}

	glm::mat4 getProjectionMatrix() override
	{
		return glm::perspective(glm::radians(90.0f), 1.0f, 1.f, 100.f);
	}

	// setter
	inline void setPosition(glm::vec3 position) { this->position = position; }

	inline void setConstant(float constant) { this->m_constant = constant; }
	inline void setLinear(float linear) { this->m_linear = linear; }
	inline void setQuadratic(float quadratic) { this->m_quadratic = quadratic; }




private:

	float m_constant;
	float m_linear;
	float m_quadratic;
};