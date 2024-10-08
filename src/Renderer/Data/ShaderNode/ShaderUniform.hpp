#pragma once
#include <iostream>
#include <GLFW/glfw3.h>

#include "ShaderVar.hpp"

template <typename T>
class ShaderUniform : public ShaderVar<T>
{

public:

	ShaderUniform(int id, std::string uniformName, int arraySize) : ShaderVar<T>(id, nullptr)
	{
		this->uniformName = uniformName;
		this->arraySize = arraySize;
	}

	~ShaderUniform()
	{

	}

	std::string getShaderCode() override
	{
		if (typeid(T) == typeid(glm::vec2))
		{
			return getUniformCode("vec2");
		}
		else if (typeid(T) == typeid(glm::vec3))
		{
			return getUniformCode("vec3");
		}
		else if (typeid(T) == typeid(glm::vec4))
		{
			return getUniformCode("vec4");
		}
		else if (typeid(T) == typeid(glm::mat4))
		{
			return getUniformCode("mat4");
		}
		else
		{
			return getUniformCode(typeid(T).name());
		}
	}


	bool operator==(const ShaderUniform& other)
	{
		return arraySize == other.arraySize;
	}


private:

	std::string getUniformCode(const char* name)
	{
		if (this->arraySize > 1)
		{
			return std::format("uniform {} {}[{}];", name, "unif" + this->id, this->arraySize);
		}
		else
		{
			return std::format("uniform {} {};", name, "unif" + this->id);
		}
	}

	std::string uniformName;
	int arraySize;
};