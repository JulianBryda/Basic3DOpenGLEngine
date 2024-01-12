#pragma once
#include <iostream>

#include "PhysicalizedGameObject.hpp"

class SquareObject : public PhysicalizedGameObject
{

public:

	SquareObject(std::string name, std::vector<glm::vec3> vertices, std::vector<GLuint> indices) : PhysicalizedGameObject(name, vertices, indices)
	{

	}

};