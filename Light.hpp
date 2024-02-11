#pragma once
#include "glm/glm.hpp"

class Light
{

public:


	Light(const char* name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
	{
		this->name = name;
		this->position = glm::vec3(0.0f);

		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
	}

	~Light()
	{
		delete name;
	}


	// getter
	inline const char* getName() const { return this->name; }

	inline glm::vec3 getPosition() const { return this->position; }
	inline glm::vec3* getPositionPtr() { return &this->position; }

	inline glm::vec3 getAmbient() const { return this->ambient; }
	inline glm::vec3* getAmbientPtr() { return &this->ambient; }

	inline glm::vec3 getDiffuse() const { return this->diffuse; }
	inline glm::vec3* getDiffusePtr() { return &this->diffuse; }

	inline glm::vec3 getSpecular() const { return this->specular; }
	inline glm::vec3* getSpecularPtr() { return &this->specular; }

	// setter
	inline void setPosition(glm::vec3 position) { this->position = position; }

	inline void setAmbient(glm::vec3 ambient) { this->ambient = ambient; }
	inline void setDiffuse(glm::vec3 diffuse) { this->diffuse = diffuse; }
	inline void setSpecular(glm::vec3 specular) { this->specular = specular; }




private:

	const char* name;

	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;



};