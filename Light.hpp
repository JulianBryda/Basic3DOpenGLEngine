#pragma once
#include <glm/glm.hpp>

enum LightType
{
	Point,
	Directional,
	Spot
};

class Light
{
public:

	Light(std::string_view name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, LightType lightType)
	{
		this->name = name;

		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;

		this->position = glm::vec3(0.0f);

		this->lightType = lightType;
	}

	~Light()
	{

	}


	// getter
	inline std::string_view getName() const { return this->name; }

	inline glm::vec3 getAmbient() const { return this->ambient; }
	inline glm::vec3* getAmbientPtr() { return &this->ambient; }

	inline glm::vec3 getDiffuse() const { return this->diffuse; }
	inline glm::vec3* getDiffusePtr() { return &this->diffuse; }

	inline glm::vec3 getSpecular() const { return this->specular; }
	inline glm::vec3* getSpecularPtr() { return &this->specular; }

	inline LightType getLightType() const { return this->lightType; }

	// setter
	inline void setAmbient(glm::vec3 ambient) { this->ambient = ambient; }
	inline void setDiffuse(glm::vec3 diffuse) { this->diffuse = diffuse; }
	inline void setSpecular(glm::vec3 specular) { this->specular = specular; }

protected:

	glm::vec3 position;

private:

	std::string_view name;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	LightType lightType;

};