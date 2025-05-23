#pragma once
#include <iostream>
#include <vector>

#include "../../Scene/Scene.hpp"
#include "../Graphics/Material.hpp"

class PostProcess
{

public:

	PostProcess()
	{
		visualizeNormals = false;

		Vertex vertex = Vertex(glm::vec3(0.f), glm::vec3(0.f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.f));

		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex), &vertex, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	~PostProcess()
	{
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
	}

	static PostProcess& getInstance()
	{
		static PostProcess instance;
		return instance;
	}

	void render(Scene* activeScene)
	{
		for (auto& object : activeScene->getObjects())
		{
			if (visualizeNormals)
			{
				Material* material = MaterialLib::get("visualizeNormals");
				material->use();

				material->setMat4("projectionMatrix", activeScene->getActiveCamera()->getProjectionMatrix());
				material->setMat4("viewMatrix", activeScene->getActiveCamera()->getViewMatrix());
				material->setMat4("modelMatrix", object->getModelMatrix());

				object->draw();
			}

			if (object->getIsDrawCollider())
			{
				Material* material = MaterialLib::get("color");
				material->use();

				material->setMat4("projectionMatrix", activeScene->getActiveCamera()->getProjectionMatrix());
				material->setMat4("viewMatrix", activeScene->getActiveCamera()->getViewMatrix());
				material->setMat4("modelMatrix", object->getCollider().getModelMatrix());

				material->setFloat4("color", glm::vec4(0.f, 1.0f, 0.f, 1.f));

				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				object->drawCollider();
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			if (object->getIsOutline())
			{
				Material* material = MaterialLib::get("color");
				material->use();

				material->setMat4("projectionMatrix", activeScene->getActiveCamera()->getProjectionMatrix());
				material->setMat4("viewMatrix", activeScene->getActiveCamera()->getViewMatrix());
				material->setMat4("modelMatrix", object->getModelMatrix());

				material->setFloat4("color", glm::vec4(1.f, 0.6f, 0.f, 1.f));

				createOutline(object);

				renderOutline(object, activeScene, material);

				renderOrigin();
			}
		}
	}

	bool visualizeNormals;

private:

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

		// enable color buffer
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}

	void renderOutline(GameObject* object, Scene* activeScene, Material* material)
	{
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

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

		material->setMat4("modelMatrix", outlineModelMatrix);

		object->draw();

		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
	}

	void renderOrigin()
	{
		glDisable(GL_DEPTH_TEST);

		glBindVertexArray(m_vao);
		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);

		glEnable(GL_DEPTH_TEST);
	}

	GLuint m_vao, m_vbo;
};