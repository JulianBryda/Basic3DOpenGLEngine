#pragma once
#include <iostream>
#include <GLFW/glfw3.h>

class ShaderUniform
{

public:

	ShaderUniform(std::string name, GLint type, int arraySize)
	{
		this->name = name;
		this->type = type;
		this->arraySize = arraySize;
	}

	~ShaderUniform()
	{

	}

	bool operator==(const ShaderUniform& other)
	{
		return name == other.name && type == other.type && arraySize == other.arraySize;
	}

private:

	std::string name;
	GLint type;
	int arraySize;

};