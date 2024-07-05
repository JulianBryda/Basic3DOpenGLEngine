#pragma once
#include <glm/glm.hpp>


class Material
{

public:

	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
	{
		this->m_ambient = ambient;
		this->m_diffuse = diffuse;
		this->m_specular = specular;

		this->m_shininess = shininess;
	}

	~Material()
	{

	}

	// getter
	inline glm::vec3 getAmbient() const { return this->m_ambient; }
	inline glm::vec3* getAmbientPtr() { return &this->m_ambient; }

	inline glm::vec3 getDiffuse() const { return this->m_diffuse; }
	inline glm::vec3* getDiffusePtr() { return &this->m_diffuse; }

	inline glm::vec3 getSpecular() const { return this->m_specular; }
	inline glm::vec3* getSpecularPtr() { return &this->m_specular; }

	inline float getShininess() const { return this->m_shininess; }
	inline float* getShininessPtr() { return &this->m_shininess; }

	// setter
	inline void setAmbient(glm::vec3 ambient) { this->m_ambient = ambient; }
	inline void setDiffuse(glm::vec3 diffuse) { this->m_diffuse = diffuse; }
	inline void setSpecular(glm::vec3 specular) { this->m_specular = specular; }

	inline void setShininess(float shininess) { this->m_shininess = shininess; }


private:

	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;

	float m_shininess;

};