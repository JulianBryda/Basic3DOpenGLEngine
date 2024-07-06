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
		this->m_name = name;

		this->m_ambient = ambient;
		this->m_diffuse = diffuse;
		this->m_specular = specular;

		this->position = glm::vec3(0.0f);

		this->m_lightType = lightType;
	}

	~Light()
	{

	}


	// getter
	inline std::string_view getName() const { return this->m_name; }

	inline glm::vec3 getAmbient() const { return this->m_ambient; }
	inline glm::vec3* getAmbientPtr() { return &this->m_ambient; }

	inline glm::vec3 getDiffuse() const { return this->m_diffuse; }
	inline glm::vec3* getDiffusePtr() { return &this->m_diffuse; }

	inline glm::vec3 getSpecular() const { return this->m_specular; }
	inline glm::vec3* getSpecularPtr() { return &this->m_specular; }

	inline LightType getLightType() const { return this->m_lightType; }

	// setter
	inline void setAmbient(glm::vec3 ambient) { this->m_ambient = ambient; }
	inline void setDiffuse(glm::vec3 diffuse) { this->m_diffuse = diffuse; }
	inline void setSpecular(glm::vec3 specular) { this->m_specular = specular; }

protected:

	glm::vec3 position;

private:

	std::string_view m_name;

	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;

	LightType m_lightType;

};