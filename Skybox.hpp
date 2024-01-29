#pragma once
#include <iostream>

#include "GameObject.h"

class Skybox : public GameObject
{

public:

	Skybox(std::string name, Mesh mesh, ColliderType colliderType) : GameObject(name, mesh, "color", colliderType)
	{

	}

	void draw() override
	{
		glDisable(GL_DEPTH_TEST);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, this->getMeshPtr()->getIndicesPtr()->size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glEnable(GL_DEPTH_TEST);
	}


};