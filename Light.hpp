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

		genBuffers();
	}

	~Light()
	{
		glDeleteTextures(1, &m_depthMap),
			glDeleteFramebuffers(1, &m_depthMapFBO);
	}


	// getter
	inline glm::vec3 getPosition() const { return this->position; }
	inline glm::vec3* getPositionPtr() { return &this->position; }

	inline std::string_view getName() const { return this->m_name; }

	inline glm::vec3 getAmbient() const { return this->m_ambient; }
	inline glm::vec3* getAmbientPtr() { return &this->m_ambient; }

	inline glm::vec3 getDiffuse() const { return this->m_diffuse; }
	inline glm::vec3* getDiffusePtr() { return &this->m_diffuse; }

	inline glm::vec3 getSpecular() const { return this->m_specular; }
	inline glm::vec3* getSpecularPtr() { return &this->m_specular; }

	inline LightType getLightType() const { return this->m_lightType; }

	inline GLuint getDepthMapFBO() const { return m_depthMapFBO; }
	inline GLuint getDepthMap()const { return m_depthMap; }

	glm::mat4 getViewMatrix() const
	{
		return glm::lookAt(this->position, glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f));
	}

	virtual glm::mat4 getProjectionMatrix() = 0;
	virtual glm::mat4 getProjectionMatrix(glm::vec3 min, glm::vec3 max)
	{
		throw std::runtime_error("This method can only be used with Directional Lights!");
	};

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
		glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);

		glGenTextures(1, &m_depthMap);
		glBindTexture(GL_TEXTURE_2D, m_depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthMap, 0);

		glDrawBuffer(GL_NONE);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Failed to create Framebuffer for " << m_name << "!" << std::endl;
		}
	}

	std::string_view m_name;

	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;

	LightType m_lightType;

	GLuint m_depthMapFBO, m_depthMap;
};