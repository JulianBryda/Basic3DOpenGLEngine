#pragma once
#include <iostream>

#include "ShaderVar.hpp"

class ShaderUniform : public ShaderVar
{

public:

	ShaderUniform(int id, std::string uniformName, int arraySize) : ShaderVar(id)
	{
		this->variableName = uniformName;
		this->arraySize = arraySize;
	}

	~ShaderUniform()
	{

	}

	std::string getShaderCode(std::vector<ShaderNodeAttribute>& inputs, ShaderEnums::ShaderVarType outputType) override
	{
		return getUniformCode(getTypeName(outputType));
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
			return std::format("uniform {} {}[{}];\n", varName, variableName, this->arraySize);
		}
		else
		{
			return std::format("uniform {} {};\n", varName, variableName);
		}
	}

	int arraySize;
};