#pragma once
#include <iostream>
#include <vector>

#include "RendererManager.hpp"
#include "GameObject.h"
#include "Shader.hpp"
#include "ShaderLib.hpp"

class ObjectRenderer : public RendererBase
{

public:

	ObjectRenderer() : RendererBase(RendererType::Object)
	{

	}

	void render(Scene* activeScene) override
	{
		for (auto& object : activeScene->getObjects())
		{
			if (object->getHidden()) continue;

			Shader* shader = nullptr;
			if (RendererManager::getInstance().getRenderMode() == RenderMode::Debug)
			{
				shader = ShaderLib::getDebugShaderPtr();
			}
			else
			{
				shader = object->getShaderPtr();
			}
			shader->use();

			shader->setMat4("projection", activeScene->getActiveCamera()->getProjectionMatrix());
			shader->setMat4("view", activeScene->getActiveCamera()->getViewMatrix());
			shader->setMat4("model", object->getModelMatrix());

			shader->setFloat3("viewPos", activeScene->getActiveCamera()->getPosition());
			shader->setMaterial(object->getMaterialPtr());

			shader->setTexture(object->getTextureType(), object->getTexture(), GL_TEXTURE0);

			shader->setLightCount(activeScene->getLights().size());
			for (int i = 0; i < activeScene->getLights().size(); i++)
			{
				shader->setLight(activeScene->getLights()[i], i, GL_TEXTURE1 + i);
			}


			if (object->getIsOutline())
			{
				// render object to stencil buffer for outline 
				glEnable(GL_STENCIL_TEST);

				// disable color and depth buffer
				glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
				glDepthMask(GL_FALSE);

				// disable depth test
				glDisable(GL_DEPTH_TEST);

				glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
				glStencilFunc(GL_ALWAYS, 1, 0xFF);
				glStencilMask(0xFF);

				object->draw();

				// enable color and depth buffer
				glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
				glDepthMask(GL_TRUE);

				// enable depth test
				glEnable(GL_DEPTH_TEST);
			}

			object->draw();

			// cleanup texture to prevent wrong usage on other objects
			shader->setTexture(object->getTextureType(), 0, GL_TEXTURE0);

			shader = ShaderLib::getColorShaderPtr();
			shader->use();

			shader->setMat4("projection", activeScene->getActiveCamera()->getProjectionMatrix());
			shader->setMat4("view", activeScene->getActiveCamera()->getViewMatrix());

			// draw outline?
			if (object->getIsOutline())
			{
				glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
				glStencilMask(0x00);
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
				shader->setFloat4("color", glm::vec4(1.f, 0.6f, 0.f, 1.f));

				object->draw();

				glStencilMask(0xFF);
				glEnable(GL_DEPTH_TEST);
				glDisable(GL_STENCIL_TEST);
			}


			// draw wireframe?
			if (object->getDrawWireframe())
			{
				shader->setMat4("model", object->getModelMatrix());

				shader->setFloat4("color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

				object->drawWireframe();
			}

			// draw hitbox?

			if (object->getIsDrawCollider())
			{
				shader->setMat4("model", object->getColliderPtr()->getModelMatrix());

				shader->setFloat4("color", glm::vec4(1.0f, 0.0f, 0.0f, 0.2f));

				object->drawCollider();
			}
		}

	}

private:

	int x = 0;

};