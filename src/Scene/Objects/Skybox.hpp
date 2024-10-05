#pragma once
#include <iostream>

#include "GameObject.hpp"
#include "../Renderer/Renderer.hpp"

class Skybox : public GameObject
{

public:

	Skybox(std::string name) : GameObject(name, ".\\Assets\\Objects\\Cube.obj", ShaderLib::get("skybox.glsl"), ColliderType::NONE)
	{

	}


	glm::mat4 getModelMatrix() override
	{
		modelMatrix = glm::scale(glm::translate(glm::mat4(1.0f), Renderer::getInstance().getActiveScene()->getActiveCamera()->getPosition() + this->position), this->getScale());

		return modelMatrix;
	}

};