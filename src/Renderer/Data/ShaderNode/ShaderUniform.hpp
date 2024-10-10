#pragma once
#include <iostream>
#include <GLFW/glfw3.h>

#include "ShaderVar.hpp"

class ShaderUniform : public ShaderVar
{

public:

	ShaderUniform(int id, std::string uniformName, GLint outputType, int arraySize) : ShaderVar(id, nullptr, outputType)
	{
		this->uniformName = uniformName;
		this->arraySize = arraySize;
	}

	~ShaderUniform()
	{

	}

	std::string getShaderCode(std::vector<std::string>* inputNames) override
	{
		return getUniformCode(getTypeName());
	}


	bool operator==(const ShaderUniform& other)
	{
		return arraySize == other.arraySize;
	}


private:

	std::string getUniformCode(std::string varName)
	{
		if (this->arraySize > 1)
		{
			return std::format("uniform {} {}[{}];\n", varName, uniformName, this->arraySize);
		}
		else
		{
			return std::format("uniform {} {};\n", varName, uniformName);
		}
	}

	std::string uniformName;
	int arraySize;
};