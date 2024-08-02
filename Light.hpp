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

		this->m_depthMap = 0;
		this->m_depthMapFBO = 0;
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

	inline GLuint getDepthMapFBO() const { return m_depthMapFBO; }

	glm::mat4 getViewMatrix() const
	{
		return glm::lookAt(this->position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	}

	virtual glm::mat4 getProjectionMatrix() = 0;

	// setter
	inline void setAmbient(glm::vec3 ambient) { this->m_ambient = ambient; }
	inline void setDiffuse(glm::vec3 diffuse) { this->m_diffuse = diffuse; }
	inline void setSpecular(glm::vec3 specular) { this->m_specular = specular; }



	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

protected:

	glm::vec3 position;

private:

	void genBuffers()
	{
		glGenFramebuffers(1, &m_depthMapFBO);

		glGenTextures(1, &m_depthMap);
		glBindTexture(GL_TEXTURE_2D, m_depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		// Attach the depth texture to the framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	std::string_view m_name;

	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;

	LightType m_lightType;

	GLuint m_depthMapFBO, m_depthMap;
};