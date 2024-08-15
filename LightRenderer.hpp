#pragma once
#include <iostream>
#include <vector>

#include "RendererBase.hpp"
#include "Shader.hpp"
#include "GameObject.h"
#include "Shader.hpp"
#include "ShaderLib.hpp"

class LightRenderer : public RendererBase
{

public:

	LightRenderer() : RendererBase(RendererType::Lighting)
	{

	}

	void render(Scene* activeScene) override
	{
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		for (auto& light : activeScene->getLights())
		{
			Shader* shader = ShaderLib::getDepthShaderPtr();
			shader->use();

			if (light->getLightType() == Directional)
			{
				shader->setMat4("projection", light->getProjectionMatrix(activeScene->getWorldBBMin(), activeScene->getWorldBBMax()));
			}
			else
			{
				shader->setMat4("projection", light->getProjectionMatrix());
			}
			shader->setMat4("view", light->getViewMatrix());

			glViewport(0, 0, light->SHADOW_WIDTH, light->SHADOW_HEIGHT);
			glBindFramebuffer(GL_FRAMEBUFFER, light->getDepthMapFBO());
			glClear(GL_DEPTH_BUFFER_BIT);
			glCullFace(GL_FRONT); // prevent shadow acne

			for (auto& object : activeScene->getObjects())
			{
				shader->setMat4("model", object->getModelMatrix());

				object->draw();
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glCullFace(GL_BACK);
		}

		glClear(GL_DEPTH_BUFFER_BIT);
		glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	}

private:

	int x = 0;

};