#pragma once
#include "GameObject.h"


class Camera : public GameObject
{

public:

	Camera(std::string name, bool isOrtho) : GameObject(name, ".\\Assets\\Objects\\Camera.obj", ShaderLib::getColorShaderPtr(), ColliderType::NONE)
	{
		this->m_anchor = glm::vec3(0.0f);
		this->m_view = glm::lookAt(this->position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		this->m_horizontalAngle = 3.14f;
		this->m_verticalAngle = 0.0f;
		this->m_Distance = 15.0f;
		this->m_up = glm::vec3(0.0f, 1.0f, 0.0f);

		this->immutable = false;

		this->m_near = 0.1f;
		this->m_far = 1000.0f;

		if (isOrtho)
			this->m_projection = glm::ortho(-1280.0f * 0.05f, 1280.0f * 0.05f, -720.0f * 0.05f, 720.0f * 0.05f, this->m_near, this->m_far);
		else
			this->m_projection = glm::perspective(glm::radians(80.0f), 16.0f / 9.0f, this->m_near, this->m_far);
	}

	// getter
	inline glm::vec3 getAnchor() const { return this->m_anchor; }
	inline glm::vec3* getAnchorPtr() { return &this->m_anchor; }
	inline glm::vec3 getUp() const { return this->m_up; }

	inline glm::vec3 getDirection() const { return this->m_anchor - this->position; }

	inline glm::mat4 getProjectionMatrix() const { return this->m_projection; }
	inline glm::mat4 getViewMatrix() const { return this->m_view; }
	inline glm::mat4* getViewMatrixPtr() { return &this->m_view; }

	inline float getHorizontalAngle() const { return this->m_horizontalAngle; }
	inline float* getHorizontalAnglePtr() { return &this->m_horizontalAngle; }
	inline float getVerticalAngle() const { return this->m_verticalAngle; }
	inline float* getVerticalAnglePtr() { return &this->m_verticalAngle; }
	inline float getDistance() const { return this->m_Distance; }

	inline float getNear() const { return this->m_near; }
	inline float getFar() const { return this->m_far; }

	inline bool isImmutable() const { return this->immutable; }

	// setter
	inline void setAnchor(glm::vec3 anchor) { this->m_anchor = anchor; }
	inline void setPosition(glm::vec3 position) { this->position = position; }
	inline void setProjectionMatrix(glm::mat4 matrix) { this->m_projection = matrix; }
	inline void setViewMatrix(glm::mat4 matrix) { this->m_view = matrix; }

	inline void setHorizontalAngle(float angle) { this->m_horizontalAngle = angle; }
	inline void setVerticalAngle(float angle) { this->m_verticalAngle = angle; }
	inline void setDistance(const float distance) { m_Distance = distance; }
	inline void multiplyDistance(const float multiplier) { m_Distance = m_Distance * multiplier; }

	inline void setImmutable(bool value) { this->immutable = value; }

private:

	glm::mat4 m_projection, m_view;
	glm::vec3 m_anchor, m_up;

	float m_horizontalAngle, m_verticalAngle, m_Distance;
	float m_near, m_far;

	bool immutable;
};