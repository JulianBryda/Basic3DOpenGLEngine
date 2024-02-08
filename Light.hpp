#pragma once
#include "glm/glm.hpp"

class Light
{

public:


	Light(const char* name)
	{
		this->name = name;
		this->intensity = 1.0f;
		this->position = glm::vec3(0.0f);
		this->rotation = glm::vec3(0.0f);

		this->lightColor = glm::vec3(1.0f);
	}

	~Light()
	{
		delete name;
	}


	// getter
	inline const char* getName() const { return this->name; };
	inline float getIntensity() const { return this->intensity; }
	inline glm::vec3 getPosition() const { return this->position; }
	inline glm::vec3 getRotation() const { return this->rotation; }
	inline glm::vec3 getLightColor() const { return this->lightColor; }

	// setter




private:

	const char* name;

	float intensity;

	glm::vec3 position, rotation;

	glm::vec3 lightColor;



};