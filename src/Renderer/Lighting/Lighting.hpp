#pragma once
#include <iostream>
#include <vector>

#include "../../Scene/Scene.hpp"
#include "../Graphics/Material.hpp"

namespace Lighting
{

	void render(Scene* activeScene)
	{
		int width = Config::g_settings->screenWidth, height = Config::g_settings->screenHeight;

		for (auto& light : activeScene->getLights())
		{
			Material* material = MaterialLib::get("depth");
			material->use();

			if (light->getLightType() == Directional)
			{
				glm::vec3 max = glm::vec3(0.f);
				glm::vec3 min = activeScene->getWorldBBMinMax(max);

				material->setMat4("projectionMatrix", light->getProjectionMatrix(min, max));
			}
			else
			{
				material->setMat4("projectionMatrix", light->getProjectionMatrix());
			}
			material->setMat4("viewMatrix", light->getViewMatrix());

			glViewport(0, 0, light->getShadowWidth(), light->getShadowHeight());
			glBindFramebuffer(GL_FRAMEBUFFER, light->getDepthMapFBO());
			glClear(GL_DEPTH_BUFFER_BIT);

			for (auto& object : activeScene->getObjects())
			{
				material->setMat4("modelMatrix", object->getModelMatrix());

				object->draw();
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		glClear(GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, width, height);
	}
};