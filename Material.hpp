#pragma once
#include <glm/glm.hpp>


class Material
{

public:

	Material(glm::vec4 color)
	{
		this->color = color;
	}

	~Material()
	{

	}

	// getter
	glm::vec4 getColor() const { return this->color; }
	glm::vec4* getColorPtr() { return &this->color; }

	// setter
	void setColor(glm::vec4 color) { this->color = color; }


private:

	glm::vec4 color;

};