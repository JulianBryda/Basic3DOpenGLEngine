#pragma once
#include <iostream>

#include "GameObject.hpp"

class Skybox : public GameObject
{

public:

	Skybox(std::string name, std::vector<glm::vec3> vertices, std::vector<GLuint> indices) : GameObject(name, vertices, indices)
	{

	}

	void draw() override
	{
		glDisable(GL_DEPTH_TEST);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glEnable(GL_DEPTH_TEST);
	}


};