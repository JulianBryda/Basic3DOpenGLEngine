#pragma once
#include <iostream>
#include <vector>

#include "RendererBase.hpp"
#include "Shader.hpp"
#include "GameObject.h"
#include "Shader.hpp"
#include "ShaderLib.hpp"

class EnvironmentRenderer : public RendererBase
{

public:

	EnvironmentRenderer() : RendererBase(RendererType::Environment)
	{

	}

	void render(Camera* activeCamera) override
	{
		for (auto& object : m_objects)
		{
			if (object->getHidden()) continue;

			Shader* shader = object->getShaderPtr();
			shader->use();

			shader->setMat4("projection", activeCamera->getProjectionMatrix());
			shader->setMat4("view", activeCamera->getViewMatrix());
			shader->setMat4("model", object->getModelMatrix());

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

	void deleteObject(GameObject* object)
	{
		for (size_t i = 0; i < m_objects.size(); i++)
		{
			if (m_objects[i] == object)
			{
				m_objects.erase(m_objects.begin() + i);
				break;
			}
		}

		delete object;
	}

	std::vector<GameObject*>& getObjects()
	{
		return m_objects;
	}

private:

	std::vector<GameObject*> m_objects;

};