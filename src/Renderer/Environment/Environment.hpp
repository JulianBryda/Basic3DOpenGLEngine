#pragma once
#include <iostream>
#include <vector>

#include "../../Scene/Scene.hpp"
#include "../Graphics/Material.hpp"
#include "../../Globals/GlobalTextures.hpp"

namespace Environment
{

	void render(Scene* activeScene)
	{
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		for (auto& object : activeScene->getEnvObjects())
		{
			if (object->getHidden()) continue;

			Material* material = object->getMaterialPtr();
			material->use();

			material->setMat4("projectionMatrix", activeScene->getActiveCamera()->getProjectionMatrix());
			material->setMat4("viewMatrix", activeScene->getActiveCamera()->getViewMatrix());
			material->setMat4("modelMatrix", object->getModelMatrix());

			material->setTexture(GL_TEXTURE_CUBE_MAP, Textures::get("skybox"), GL_TEXTURE0);

			object->draw();
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	}
}