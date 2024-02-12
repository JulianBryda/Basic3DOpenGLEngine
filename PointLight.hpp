#pragma once
#include "glm/glm.hpp"

#include "Light.hpp"

class PointLight : public Light
{

public:


	PointLight(const char* name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : Light(name, ambient, diffuse, specular, LightType::Point)
	{
		this->constant = 1.0f;
		this->linear = 0.09f;
		this->quadratic = 0.032f;
	}

	~PointLight()
	{

	}


	// getter
	inline glm::vec3 getPosition() const { return this->position; }
	inline glm::vec3* getPositionPtr() { return &this->position; }

	inline float getConstant() const { return this->constant; }
	inline float* getConstantPtr() { return &this->constant; }

	inline float getLinear() const { return this->linear; }
	inline float* getLinearPtr() { return &this->linear; }

	inline float getQuadratic() const { return this->quadratic; }
	inline float* getQuadraticPtr() { return &this->quadratic; }

	// setter
	inline void setPosition(glm::vec3 position) { this->position = position; }

	inline void setConstant(float constant) { this->constant = constant; }
	inline void setLinear(float linear) { this->linear = linear; }
	inline void setQuadratic(float quadratic) { this->quadratic = quadratic; }




private:

	float constant;
	float linear;
	float quadratic;
};