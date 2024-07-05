#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Camera.hpp"

enum RendererType
{
	Environment,
	MIN = Environment,
	Object,
	UserInterface,
	MAX
};


class RendererBase
{
public:

	RendererBase(RendererType type)
	{
		this->m_type = type;
	}

	virtual void render(Camera* activeCamera) = 0;
	virtual void addObject(GameObject* object) {};

	RendererType getType() const { return m_type; }

private:

	RendererType m_type;

};
