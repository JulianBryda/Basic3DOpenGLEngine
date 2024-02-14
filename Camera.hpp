#pragma once
#include "GameObject.h"


class Camera : public GameObject
{

public:

	Camera(std::string name, bool isOrtho) : GameObject(name, ".\\Assets\\Objects\\Camera.obj", ShaderLib::getColorShaderPtr(), ColliderType::NONE)
	{
		this->anchor = glm::vec3(0.0f);
		this->view = glm::lookAt(this->position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		this->horizontalAngle = 3.14f;
		this->verticalAngle = 0.0f;
		this->m_Distance = 15.0f;
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);

		this->near = 0.1f;
		this->far = 1000.0f;

		if (isOrtho)
			this->projection = glm::ortho(-1280.0f * 0.05f, 1280.0f * 0.05f, -720.0f * 0.05f, 720.0f * 0.05f, this->near, this->far);
		else
			this->projection = glm::perspective(glm::radians(80.0f), 16.0f / 9.0f, this->near, this->far);
	}

	// getter
	inline glm::vec3 getAnchor() const { return this->anchor; }
	inline glm::vec3* getAnchorPtr() { return &this->anchor; }
	inline glm::vec3 getUp() const { return this->up; }

	inline glm::vec3 getDirection() const { return this->anchor - this->position; }

	inline glm::mat4 getProjectionMatrix() const { return this->projection; }
	inline glm::mat4 getViewMatrix() const { return this->view; }
	inline glm::mat4* getViewMatrixPtr() { return &this->view; }

	inline float getHorizontalAngle() const { return this->horizontalAngle; }
	inline float* getHorizontalAnglePtr() { return &this->horizontalAngle; }
	inline float getVerticalAngle() const { return this->verticalAngle; }
	inline float* getVerticalAnglePtr() { return &this->verticalAngle; }
	inline float getDistance() const { return this->m_Distance; }

	inline float getNear() const { return this->near; }
	inline float getFar() const { return this->far; }

	// setter
	inline void setAnchor(glm::vec3 anchor) { this->anchor = anchor; }
	inline void setPosition(glm::vec3 position) { this->position = position; }
	inline void setViewMatrix(glm::mat4 matrix) { this->view = matrix; }

	inline void setHorizontalAngle(float angle) { this->horizontalAngle = angle; }
	inline void setVerticalAngle(float angle) { this->verticalAngle = angle; }
	inline void setDistance(const float distance) { m_Distance = distance; }
	inline void multiplyDistance(const float multiplier) { m_Distance = m_Distance * multiplier; }

private:

	glm::mat4 projection, view;
	glm::vec3 anchor, up;

	float horizontalAngle, verticalAngle, m_Distance;
	float near, far;
};