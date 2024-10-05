#pragma once
#include <iostream>
#include <vector>

#include "../../Scene/Scene.hpp"
#include "../Graphics/Shader.hpp"

namespace Lighting
{

	void render(Scene* activeScene)
	{
		int width = Config::g_settings->screenWidth, height = Config::g_settings->screenHeight;

		for (auto& light : activeScene->getLights())
		{
			Shader* shader = ShaderLib::get("depth.glsl");
			shader->use();

			if (light->getLightType() == Directional)
			{
				glm::vec3 max = glm::vec3(0.f);
				glm::vec3 min = activeScene->getWorldBBMinMax(max);

				shader->setMat4("projection", light->getProjectionMatrix(min, max));
			}
			else
			{
				shader->setMat4("projection", light->getProjectionMatrix());
			}
			shader->setMat4("view", light->getViewMatrix());

			glViewport(0, 0, light->getShadowWidth(), light->getShadowHeight());
			glBindFramebuffer(GL_FRAMEBUFFER, light->getDepthMapFBO());
			glClear(GL_DEPTH_BUFFER_BIT);

			for (auto& object : activeScene->getObjects())
			{
				shader->setMat4("model", object->getModelMatrix());

				object->draw();
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		glClear(GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, width, height);
	}
};