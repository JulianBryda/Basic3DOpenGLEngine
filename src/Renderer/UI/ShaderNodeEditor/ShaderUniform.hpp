#pragma once
#include <iostream>
#include <GLFW/glfw3.h>

template <typename T>
class ShaderUniform
{

public:

	ShaderUniform(std::string name, T* value, int arraySize)
	{
		this->name = name;
		this->arraySize = arraySize;

		this->value = value;
	}

	~ShaderUniform()
	{

	}

	template <>
	std::string ShaderUniform<int>::getShaderCode()
	{
		if (arraySize > 1)
			return getUniformArrayCode("int");
		else
			return getUniformCode("int");
	}

	template <>
	std::string ShaderUniform<float>::getShaderCode()
	{
		if (arraySize > 1)
			return getUniformArrayCode("float");
		else
			return getUniformCode("float");
	}

	template <>
	std::string ShaderUniform<glm::vec2>::getShaderCode()
	{
		if (arraySize > 1)
			return getUniformArrayCode("vec2");
		else
			return getUniformCode("vec2");
	}

	template <>
	std::string ShaderUniform<glm::vec3>::getShaderCode()
	{
		if (arraySize > 1)
			return getUniformArrayCode("vec3");
		else
			return getUniformCode("vec3");
	}

	template <>
	std::string ShaderUniform<glm::vec4>::getShaderCode()
	{
		if (arraySize > 1)
			return getUniformArrayCode("vec4");
		else
			return getUniformCode("vec4");
	}

	std::string getUniformCode(std::string typeName)
	{
		return std::format("uniform {} {};", typeName, name);
	}

	std::string getUniformArrayCode(std::string typeName)
	{
		return std::format("uniform {} {}[{}];", typeName, name, arraySize);
	}


	bool operator==(const ShaderUniform& other)
	{
		return name == other.name && type == other.type && arraySize == other.arraySize;
	}


private:

	std::string name;
	int arraySize;
	T* value;
};