#pragma once
#include "glm/glm.hpp"

#include "Light.hpp"

class PointLight : public Light
{

public:


	PointLight(const char* name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : Light(name, ambient, diffuse, specular, LightType::Point)
	{
		this->m_constant = 1.0f;
		this->m_linear = 0.09f;
		this->m_quadratic = 0.032f;
	}

	~PointLight()
	{

	}


	// getter
	inline glm::vec3 getPosition() const { return this->position; }
	inline glm::vec3* getPositionPtr() { return &this->position; }

	inline float getConstant() const { return this->m_constant; }
	inline float* getConstantPtr() { return &this->m_constant; }

	inline float getLinear() const { return this->m_linear; }
	inline float* getLinearPtr() { return &this->m_linear; }

	inline float getQuadratic() const { return this->m_quadratic; }
	inline float* getQuadraticPtr() { return &this->m_quadratic; }

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