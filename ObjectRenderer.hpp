#pragma once
#include <iostream>
#include <vector>

#include "Renderer.hpp"
#include "Shader.hpp"
#include "GameObject.hpp"
#include "PhysicalizedGameObject.hpp"

class ObjectRenderer : public Renderer
{

public:

	ObjectRenderer(RendererType type) : Renderer(type)
	{
		m_shader = new Shader("v_color.glsl", "f_color.glsl");
		
		GameObject::loadTexture("default.jpg", &defaultTexture);
	}

	void render() override
	{
        m_shader->use();
		m_shader->setFloat4("color", glm::vec4(1.0f));

		for (auto& object : m_objects)
		{
			m_shader->setMat4("model", object->getModelMatrix());
			m_shader->setMat4("scale", object->getScaleMatrix());
			m_shader->setMat4("projection", projection);
			m_shader->setMat4("view", view);

			if (object->getTexture() != 0) m_shader->setTexture(GL_TEXTURE_2D, object->getTexture());
			else m_shader->setTexture(GL_TEXTURE_2D, defaultTexture);

			auto physicObject = dynamic_cast<PhysicalizedGameObject*>(object);

			if (physicObject != nullptr && physicObject->getIsDrawCollisionBox())
			{
				physicObject->drawCollisionBox();
			}

			object->draw();		
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

	Shader* m_shader;

	GLuint defaultTexture;

	std::vector<GameObject*> m_objects;

};