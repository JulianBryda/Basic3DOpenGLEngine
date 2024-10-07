#pragma once
#include <iostream>
#include <vector>

#include "../../Scene/Scene.hpp"
#include "../Graphics/ShaderOLD.hpp"

namespace Environment
{

	void render(Scene* activeScene)
	{
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		for (auto& object : activeScene->getEnvObjects())
		{
			if (object->getHidden()) continue;

			ShaderOLD* shader = object->getShaderPtr();
			shader->use();

			shader->setMat4("projectionMatrix", activeScene->getActiveCamera()->getProjectionMatrix());
			shader->setMat4("viewMatrix", activeScene->getActiveCamera()->getViewMatrix());
			shader->setMat4("modelMatrix", object->getModelMatrix());

			shader->setTexture(object->getTextureType(), object->getTexture(), GL_TEXTURE0);

			object->draw();
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	}
}