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

	void render(Camera* activeCamera) override
	{
		for (auto& object : m_objects)
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

			shader->setMat4("projection", activeCamera->getProjectionMatrix());
			shader->setMat4("view", activeCamera->getViewMatrix());
			shader->setMat4("model", object->getModelMatrix());

			shader->setFloat3("viewPos", activeCamera->getPosition());
			shader->setMaterial(object->getMaterialPtr());


			Scene* activeScene = RendererManager::getInstance().getActiveScene();

			// set directional lights and directional light count
			shader->setFloat("directionalLightCount", activeScene->getDirectionalLights().size());

			for (int i = 0; i < activeScene->getDirectionalLights().size(); i++)
			{
				shader->setDirectionalLight(activeScene->getDirectionalLights().at(i), i);
			}

			// set point lights and point light count
			shader->setFloat("pointLightCount", activeScene->getPointLights().size());

			for (int i = 0; i < activeScene->getPointLights().size(); i++)
			{
				shader->setPointLight(activeScene->getPointLights().at(i), i);
			}

			shader->setTexture(object->getTextureType(), object->getTexture());


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

			shader = ShaderLib::getColorShaderPtr();
			shader->use();

			shader->setMat4("projection", activeCamera->getProjectionMatrix());
			shader->setMat4("view", activeCamera->getViewMatrix());

			// draw outline?
			if (object->getIsOutline())
			{
				glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
				glStencilMask(0x00);
				glDisable(GL_DEPTH_TEST);

				float scalingFactor = glm::length(glm::abs(activeCamera->getPosition() - object->getPosition())) / 150.f;
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

	void addObject(GameObject* object) override
	{
		m_objects.push_back(object);
	}

	void removeObject(GameObject& object) override
	{
		for (size_t i = 0; i < m_objects.size(); i++)
		{
			if (m_objects[i] == &object)
			{
				m_objects.erase(m_objects.begin() + i);
				break;
			}
		}
	}

	std::vector<GameObject*>& getObjects()
	{
		return m_objects;
	}

	int getObjectCount() override
	{
		return m_objects.size();
	}

private:

	std::vector<GameObject*> m_objects;

};