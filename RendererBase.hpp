#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Camera.hpp"
#include "Scene.hpp"

enum RendererType
{
	Environment,
	MIN = Environment,
	Lighting,
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

	virtual void render(Scene* activeScene) = 0;

	RendererType getType() const { return m_type; }

private:

	RendererType m_type;

};
