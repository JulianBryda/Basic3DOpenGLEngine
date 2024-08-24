#pragma once
#include <iostream>
#include <vector>

#include "RendererBase.hpp"
#include "Shader.hpp"
#include "GameObject.h"
#include "Shader.hpp"
#include "ShaderLib.hpp"

class PostProcessingRenderer : public RendererBase
{

public:

	PostProcessingRenderer() : RendererBase(RendererType::PostProcessing)
	{

	}

	void render(Scene* activeScene) override
	{
		for (auto& object : activeScene->getObjects())
		{
			if (!object->getIsOutline()) continue;

			Shader* shader = ShaderLib::getColorShaderPtr();
			shader->use();

			shader->setMat4("projection", activeScene->getActiveCamera()->getProjectionMatrix());
			shader->setMat4("view", activeScene->getActiveCamera()->getViewMatrix());
			shader->setMat4("model", object->getModelMatrix());

			shader->setFloat4("color", glm::vec4(1.f, 0.6f, 0.f, 1.f));

			createOutline(object);

			renderOutline(object, activeScene, shader);
		}
	}

	void createOutline(GameObject* object)
	{
		glEnable(GL_STENCIL_TEST);

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);

		glDisable(GL_DEPTH_TEST);

		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		glClear(GL_STENCIL_BUFFER_BIT);

		object->draw();

		glStencilMask(0x00);

		// enable color and depth buffer
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);

		// enable depth test
		glEnable(GL_DEPTH_TEST);
	}

	void renderOutline(GameObject* object, Scene* activeScene, Shader* shader)
	{
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glDisable(GL_DEPTH_TEST);

		float scalingFactor = glm::length(glm::abs(activeScene->getActiveCamera()->getPosition() - object->getPosition())) / 150.f;
		glm::mat4 outlineModelMatrix = glm::rotate(
			glm::rotate(
				glm::rotate(
					glm::scale(
						glm::translate(
							glm::mat4(1.0f), object->getPosition()),
						object->getScale() + scalingFactor),
					glm::radians(object->getRotation().x), glm::vec3(1.f, 0.f, 0.f)),
				glm::radians(object->getRotation().y), glm::vec3(0.f, 1.f, 0.f)),
			glm::radians(object->getRotation().z), glm::vec3(0.f, 0.f, 1.f));

		shader->setMat4("model", outlineModelMatrix);

		object->draw();

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
	}

private:

	int x = 0;

};