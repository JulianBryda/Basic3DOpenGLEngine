#pragma once
#include <iostream>

#include "GameObject.hpp"
#include "PhysicalizedObject.hpp"


class PhysicalizedGameObject : public GameObject, public PhysicalizedObject
{

public:

	PhysicalizedGameObject(std::string name, std::vector<glm::vec3> vertices, std::vector<GLuint> indices) : GameObject(name, vertices, indices), PhysicalizedObject()
	{
		this->isDrawCollisionBox = false;

		genCollisionBoxBuffers();
	}

	void drawCollisionBox()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, this->getIndices().size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	bool checkBoundingBoxCollision(GameObject* object) const
	{
		return this->getPosition().x < object->getPosition().x + object->getScale().x &&
			this->getPosition().x + this->getScale().x > object->getPosition().x &&
			this->getPosition().y < object->getPosition().y + object->getScale().y &&
			this->getPosition().y + this->getScale().y > object->getPosition().y &&
			this->getPosition().z < object->getPosition().z + object->getScale().z &&
			this->getPosition().z + this->getScale().z > object->getPosition().z;
	}

	bool checkCirlceCollision(GameObject* object)
	{
		return false;
	}

	bool checkSATCollision(GameObject* object)
	{
		return false;
	}


	bool getIsDrawCollisionBox() const { return isDrawCollisionBox; }
	bool* getIsDrawCollisionBoxPtr() { return &isDrawCollisionBox; }

private:

	void genCollisionBoxBuffers()
	{
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->getVertices().size(), this->getVertices().data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * this->getIndices().size(), this->getIndices().data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	bool isDrawCollisionBox;

	GLuint vao, vbo, ebo;
};