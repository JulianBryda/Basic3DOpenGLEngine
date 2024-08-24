#pragma once
#include <iostream>
#include <vector>

#include "RendererBase.hpp"
#include "Shader.hpp"
#include "GameObject.h"
#include "Shader.hpp"
#include "ShaderLib.hpp"

class EnvironmentRenderer : public RendererBase
{

public:

	EnvironmentRenderer() : RendererBase(RendererType::Environment)
	{

	}

	void render(Scene* activeScene) override
	{
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		for (auto& object : activeScene->getEnvObjects())
		{
			if (object->getHidden()) continue;

			Shader* shader = object->getShaderPtr();
			shader->use();

			shader->setMat4("projection", activeScene->getActiveCamera()->getProjectionMatrix());
			shader->setMat4("view", activeScene->getActiveCamera()->getViewMatrix());
			shader->setMat4("model", object->getModelMatrix());

			shader->setTexture(object->getTextureType(), object->getTexture(), GL_TEXTURE0);

			object->draw();
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	}

private:

	int x = 0;

};