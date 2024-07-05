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
				shader = ShaderLib::getRenderShaderPtr();
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

			for (size_t i = 0; i < activeScene->getDirectionalLights().size(); i++)
			{
				shader->setDirectionalLight(activeScene->getDirectionalLights().at(i), i);
			}

			// set point lights and point light count
			shader->setFloat("pointLightCount", activeScene->getPointLights().size());

			for (size_t i = 0; i < activeScene->getPointLights().size(); i++)
			{
				shader->setPointLight(activeScene->getPointLights().at(i), i);
			}

			shader->setTexture(object->getTextureType(), object->getTexture());

			object->draw();


			// set shader for debug operations
			shader = ShaderLib::getColorShaderPtr();
			shader->use();

			shader->setMat4("projection", activeCamera->getProjectionMatrix());
			shader->setMat4("view", activeCamera->getViewMatrix());

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

	void removeObject(GameObject* object)
	{
		for (size_t i = 0; i < m_objects.size(); i++)
		{
			if (m_objects[i] == object)
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

private:

	std::vector<GameObject*> m_objects;

};