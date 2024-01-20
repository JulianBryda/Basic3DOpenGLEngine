#pragma once
#include <iostream>
#include <vector>

#include "Renderer.hpp"
#include "Shader.hpp"
#include "GameObject.h"
#include "Shader.hpp"

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
			Shader* shader = object->getShaderPtr();

			shader->use();
			shader->setFloat4("color", glm::vec4(1.0f));

			shader->setMat4("model", object->getModelMatrix());
			shader->setMat4("scale", object->getScaleMatrix());
			shader->setMat4("projection", activeCamera->getProjectionMatrix());
			shader->setMat4("view", activeCamera->getViewMatrix());

			if (object->getTexture() != 0) shader->setTexture(GL_TEXTURE_2D, object->getTexture());
			else shader->setTexture(GL_TEXTURE_2D, defaultTexture);

			object->draw();


			// draw hitbox?

			if (object->getIsDrawCollider())
			{
				shader->setFloat4("color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

				shader->setMat4("model", object->getColliderPtr()->getModelMatrix());
				shader->setMat4("scale", object->getColliderPtr()->getScaleMatrix());

				object->drawCollider();
			}
		}

	}

	void addObject(GameObject* object)
	{
		m_objects.push_back(object);
	}

	std::vector<GameObject*>& getObjects()
	{
		return m_objects;
	}

private:

	GLuint defaultTexture;

	std::vector<GameObject*> m_objects;

};