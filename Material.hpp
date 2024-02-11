#pragma once
#include <glm/glm.hpp>


class Material
{

public:

	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;

		this->shininess = shininess;
	}

	~Material()
	{

	}

	// getter
	inline glm::vec3 getAmbient() const { return this->ambient; }
	inline glm::vec3* getAmbientPtr() { return &this->ambient; }

	inline glm::vec3 getDiffuse() const { return this->diffuse; }
	inline glm::vec3* getDiffusePtr() { return &this->diffuse; }

	inline glm::vec3 getSpecular() const { return this->specular; }
	inline glm::vec3* getSpecularPtr() { return &this->specular; }

	inline float getShininess() const { return this->shininess; }
	inline float* getShininessPtr() { return &this->shininess; }

	// setter
	inline void setAmbient(glm::vec3 ambient) { this->ambient = ambient; }
	inline void setDiffuse(glm::vec3 diffuse) { this->diffuse = diffuse; }
	inline void setSpecular(glm::vec3 specular) { this->specular = specular; }

	inline void setShininess(float shininess) { this->shininess = shininess; }


private:

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float shininess;

};