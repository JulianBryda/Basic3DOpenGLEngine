#pragma once
#include <iostream>

#include "GameObject.h"
#include "../Renderer/Renderer.hpp"

class Skybox : public GameObject
{

public:

	Skybox(std::string name) : GameObject(name, ".\\Assets\\Objects\\Cube.obj", ShaderLib::get("skybox.glsl"), ColliderType::NONE)
	{

	}


	glm::mat4 getModelMatrix() const override
	{
		return glm::scale(glm::translate(glm::mat4(1.0f), Renderer::getInstance().getActiveScene()->getActiveCamera()->getPosition() + this->position), this->getScale());
	}

};