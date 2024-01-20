#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Camera.hpp"

enum RendererType
{
	Object,
	MIN = Object,
	Terrain,
	Light,
	UserInterface,
	MAX = UserInterface
};

class Renderer
{
public:

	Renderer(RendererType type)
	{
		this->type = type;
	}

	virtual void render(Camera* activeCamera) = 0;

private:

	RendererType type;

};
