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
		Camera* camera = new Camera("Camera", false);
		camera->setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
		RendererPipeline::addCamera(camera);
	}

	void render(Camera* activeCamera) override
	{
		for (auto& object : m_objects)
		{
			if (object->getIsHidden()) continue;

			Shader* shader = object->getShaderPtr();
			shader->use();

			shader->setMat4("projection", activeCamera->getProjectionMatrix());
			shader->setMat4("view", activeCamera->getViewMatrix());
			shader->setMat4("model", object->getModelMatrix());

			shader->setFloat3("camPos", activeCamera->getPosition());
			shader->setFloat4("color", object->getMaterialPtr()->getColor());

			shader->setTexture(object->getTextureType(), object->getTexture());

			object->draw();


			// set shader for debug operations
			shader = ShaderLib::getColorShader();
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

	void setRenderMode(RenderMode renderMode) override
	{
		switch (renderMode)
		{
		case Debug:
			for (auto const& object : m_objects)
			{
				object->setShader(ShaderLib::getDebugShader());
			}
			break;
		case Render:
			for (auto const& object : m_objects)
			{
				object->setShader(ShaderLib::getRenderShader());
			}
			break;
		default:
			break;
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