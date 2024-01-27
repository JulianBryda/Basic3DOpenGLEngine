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

	ObjectRenderer(RendererType type) : Renderer(type)
	{
		Camera* camera = new Camera("Camera", false);
		camera->setPosition(glm::vec3(0.0f, 0.0f, 1.0f));
		RendererPipeline::addCamera(camera);

		GameObject::loadTexture("default.jpg", &defaultTexture);
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

			if (object->getTexture() != 0) shader->setTexture(GL_TEXTURE_2D, object->getTexture());
			else shader->setTexture(GL_TEXTURE_2D, defaultTexture);

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

	void addObject(GameObject* object)
	{
		m_objects.push_back(object);
	}

	void deleteObject(GameObject* object)
	{
		m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());

		delete object;
	}

	std::vector<GameObject*>& getObjects()
	{
		return m_objects;
	}

private:

	GLuint defaultTexture;

	std::vector<GameObject*> m_objects;

};