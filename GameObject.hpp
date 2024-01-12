#pragma once
#include <iostream>
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "PhysicalizedObject.hpp"


class GameObject
{

public:

	GameObject(std::string name, std::vector<glm::vec3> vertices, std::vector<GLuint> indices)
	{
		this->name = name;
		this->vertices = vertices;
		this->indices = indices;
		this->position = glm::vec3(0.0f);
		this->rotation = glm::vec3(0.0f);
		this->scale = glm::vec3(5.0f);

		genBuffers();
	}

	virtual void genBuffers()
	{
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	virtual void updateBuffers()
	{
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	virtual void draw()
	{
		// uncomment this to draw wireframe
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void loadTexture(std::string textureName);
	static void loadTexture(std::string textureName, GLuint* texture);

	glm::vec3 getPosition() const { return position; }
	glm::vec3* getPositionPtr() { return &position; }
	glm::vec3 getRotation() const { return rotation; }
	glm::vec3 getScale() const { return scale; }
	glm::vec3* getScalePtr() { return &scale; }

	glm::mat4 getModelMatrix() const { return glm::translate(glm::mat4(1.0f), position); }
	glm::mat4 getScaleMatrix() const { return glm::scale(glm::mat4(1.0f), scale); }

	GLuint getTexture() const { return texture; }

	void setPosition(glm::vec3 position) { this->position = position; }
	void setRotation(glm::vec3 rotation) { this->rotation = rotation; }
	void setScale(glm::vec3 scale) { this->scale = scale; }



	std::vector<glm::vec3> getVertices() const { return vertices; }
	std::vector<GLuint> getIndices() const { return indices; }
	std::string getName() const { return name; }



private:

	std::string name;

	GLuint vao, vbo, ebo, texture;

	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;

	glm::vec3 position, rotation, scale;
};