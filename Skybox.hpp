#pragma once
#include <iostream>

#include "GameObject.h"
#include "RendererPipeline.hpp"

class Skybox : public GameObject
{

public:

	Skybox(std::string name) : GameObject(name, "C:\\Users\\JulianBrydaVeloce\\source\\repos\\FuckWindows\\Assets\\Objects\\Cube.obj", "skybox", ColliderType::NONE)
	{

	}

	void draw() override
	{
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, this->getMeshPtr()->getIndicesPtr()->size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	}


	glm::mat4 getModelMatrix() const override { return glm::scale(glm::translate(glm::mat4(1.0f), RendererPipeline::getActiveCameraPtr()->getPosition() + this->position), this->getScale()); }

};