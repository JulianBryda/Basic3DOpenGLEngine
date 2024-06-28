#pragma once
#include <iostream>
#include <vector>

#include "Renderer.hpp"
#include "Shader.hpp"
#include "GameObject.h"
#include "Shader.hpp"
#include "ShaderLib.hpp"

class ObjectRenderer : public Renderer
{

public:

	ObjectRenderer() : Renderer(RendererType::Object)
	{

	}

	void render(Camera* activeCamera) override
	{
		for (auto& object : m_objects)
		{
			if (object->getIsHidden()) continue;

			Shader* shader = nullptr;
			if (RendererPipeline::getRenderMode() == RenderMode::Debug)
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


			Scene* activeScene = RendererPipeline::getActiveScenePtr();

			// set directional light
			shader->setDirectionalLight(activeScene->getDirectionalLightPtr());

			// set point lights and point light count
			shader->setFloat("pointLightCount", activeScene->getPointLightsPtr()->size());

			for (size_t i = 0; i < activeScene->getPointLightsPtr()->size(); i++)
			{
				shader->setPointLight(activeScene->getPointLightsPtr()->at(i), i);
			}

			shader->setTexture(object->getTextureType(), object->getTexture());

			object->draw();


			// set shader for debug operations
			shader = ShaderLib::getColorShaderPtr();
			shader->use();

			shader->setMat4("projection", activeCamera->getProjectionMatrix());
			shader->setMat4("view", activeCamera->getViewMatrix());

			// draw wireframe?

			if (object->getIsDrawWireframe())
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

	void addObject(GameObject* object)
	{
		m_objects.push_back(object);
	}

	void removeObject(GameObject* object)
	{
		m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
	}

	std::vector<GameObject*>& getObjects()
	{
		return m_objects;
	}

private:

	std::vector<GameObject*> m_objects;

};