#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

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
		this->projection = glm::ortho(-1280.0f * 0.05f, 1280.0f * 0.05f, -720.0f * 0.05f, 720.0f * 0.05f, 0.1f, 100.0f);
		this->view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	virtual void render() = 0;


protected:

	glm::mat4 projection, view;

private:

	RendererType type;

};
