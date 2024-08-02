#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Camera.hpp"

enum RendererType
{
	Lighting,
	MIN = Lighting,
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
