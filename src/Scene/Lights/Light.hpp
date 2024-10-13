#pragma once
#include <glad/glad.h>
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

	Light(std::string_view name, glm::vec3 color, LightType lightType)
	{
		this->m_name = name;

		this->m_color = color;

		this->position = glm::vec3(0.0f);

		this->m_lightType = lightType;

		this->m_depthMap = 0;
		this->m_depthMapFBO = 0;

		this->m_shadowWidth = 1024;
		this->m_shadowHeight = 1024;

		this->m_shadowQuality = 1.f;

		genBuffers();
	}

	~Light()
	{
		glDeleteTextures(1, &m_depthMap);
		glDeleteFramebuffers(1, &m_depthMapFBO);
	}


	void updateShadows()
	{
		m_shadowWidth = static_cast<int>(1024.f * m_shadowQuality);
		m_shadowHeight = static_cast<int>(1024.f * m_shadowQuality);

		if (m_depthMap)
		{
			glDeleteTextures(1, &m_depthMap);
			m_depthMap = 0;
		}
		if (m_depthMapFBO)
		{
			glDeleteFramebuffers(1, &m_depthMapFBO);
			m_depthMapFBO = 0;
		}

		genBuffers();
	}


	// getter
	inline glm::vec3 getPosition() const { return this->position; }
	inline glm::vec3* getPositionPtr() { return &this->position; }

	inline std::string_view getName() const { return this->m_name; }

	inline glm::vec3 getColor() const { return this->m_color; }
	inline glm::vec3* getColorPtr() { return &this->m_color; }

	inline LightType getLightType() const { return this->m_lightType; }

	inline GLuint getDepthMapFBO() const { return m_depthMapFBO; }
	inline GLuint getDepthMap() const { return m_depthMap; }

	int getShadowHeight() const { return m_shadowHeight; }
	int getShadowWidth() const { return m_shadowWidth; }

	float* getShadowQualityPtr() { return &m_shadowQuality; }

	virtual glm::mat4 getViewMatrix() = 0;

	virtual glm::mat4 getProjectionMatrix() = 0;
	virtual glm::mat4 getProjectionMatrix(glm::vec3 min, glm::vec3 max)
	{
		throw std::runtime_error("This method can only be used with Directional Lights!");
	};

	inline void setColor(glm::vec3 color) { this->m_color = color; }

protected:

	glm::vec3 position;
	int m_shadowHeight, m_shadowWidth;

private:

	void genBuffers()
	{
		glGenFramebuffers(1, &m_depthMapFBO);

		glGenTextures(1, &m_depthMap);
		glBindTexture(GL_TEXTURE_2D, m_depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_shadowWidth, m_shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float clampColor[] = { 1.f, 1.f, 1.f, 1.f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

		glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Failed to create Framebuffer for " << m_name << "!" << std::endl;
		}
	}

	std::string_view m_name;

	glm::vec3 m_color;

	LightType m_lightType;

	GLuint m_depthMapFBO, m_depthMap;

	float m_shadowQuality;
};