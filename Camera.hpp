#pragma once
#include "GameObject.h"


class Camera : public GameObject
{

public:

	Camera(std::string name, bool isOrtho) : GameObject(name, "C:\\Users\\JulianBrydaVeloce\\source\\repos\\FuckWindows\\Assets\\Objects\\Camera.obj", "color", ColliderType::NONE)
	{
		this->anchor = glm::vec3(0.0f);
		this->view = glm::lookAt(this->position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		this->horizontalAngle = 3.14f;
		this->verticalAngle = 0.0f;
		this->m_Distance = 15.0f;

		if (isOrtho)
			this->projection = glm::ortho(-1280.0f * 0.05f, 1280.0f * 0.05f, -720.0f * 0.05f, 720.0f * 0.05f, 0.1f, 1000.0f);
		else
			this->projection = glm::perspective(glm::radians(80.0f), 16.0f / 9.0f, 0.1f, 1000.0f);
	}

	// getter
	glm::vec3 getAnchor() const { return this->anchor; }
	glm::vec3* getAnchorPtr() { return &this->anchor; }

	glm::mat4 getProjectionMatrix() const { return this->projection; }
	glm::mat4 getViewMatrix() const { return this->view; }
	glm::mat4* getViewMatrixPtr() { return &this->view; }

	float* getHorizontalAnglePtr() { return &this->horizontalAngle; }
	float* getVerticalAnglePtr() { return &this->verticalAngle; }
	float getDistance() const { return this->m_Distance; }
	inline void setDistance(const float distance) { m_Distance = distance; };
	inline void multiplyDistance(const float multiplier) { m_Distance = m_Distance * multiplier; };
private:

	glm::mat4 projection, view;
	glm::vec3 anchor;

	float horizontalAngle, verticalAngle, m_Distance;
};