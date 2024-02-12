#pragma once
#include "glm/glm.hpp"

#include "Light.hpp"

class SpotLight : public Light
{

public:


	SpotLight(const char* name, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : Light(name, ambient, diffuse, specular, LightType::Spot)
	{
		this->direction = direction;

		this->cutOff = glm::cos(glm::radians(12.5f));
		this->outerCutOff = glm::cos(glm::radians(17.5f));
	}

	~SpotLight()
	{

	}


	// getter
	inline glm::vec3 getPosition() const { return this->position; }
	inline glm::vec3* getPositionPtr() { return &this->position; }

	inline glm::vec3 getDirection() const { return this->direction; }
	inline glm::vec3* getDirectionPtr() { return &this->direction; }

	inline float getCutOff() const { return this->cutOff; }
	inline float* getCutOffPtr() { return &this->cutOff; }

	inline float getOuterCutOff() const { return this->outerCutOff; }
	inline float* getOuterCutOffPtr() { return &this->outerCutOff; }

	// setter
	inline void setPosition(glm::vec3 position) { this->position = position; }

	inline void setCutOff(float cutOff) { this->cutOff = cutOff; }
	inline void setOuterCutOff(float outerCutOff) { this->outerCutOff = outerCutOff; }




private:

	glm::vec3 direction;

	float cutOff;
	float outerCutOff;
};