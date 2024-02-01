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
	MAX = UserInterface
};

enum RenderMode
{
	Debug,
	Render
};


class Renderer
{
public:

	Renderer(RendererType type)
	{
		this->type = type;
	}

	virtual void render(Camera* activeCamera) = 0;

	virtual void setRenderMode(RenderMode renderMode)
	{
		std::cout << "Change render mode not implemented in " << type << "\n";
	}

private:

	RendererType type;

};
