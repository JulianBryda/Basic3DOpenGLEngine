#pragma once
#include <iostream>
#include <vector>
#include <variant>

#include "ShaderVar.hpp"

class ShaderFunction : public ShaderVar
{

public:

	enum ShaderFunctionOperation
	{
		Operation,
		FunctionCall
	};

	ShaderFunction(int id, std::string functionName, GLint outputType, ShaderFunctionOperation operation) : ShaderVar(id, nullptr, outputType)
	{
		this->functionName = functionName;
		this->operation = operation;
	}

	~ShaderFunction()
	{

	}

	std::string getShaderCode(std::vector<std::string>* inputNames) override
	{
		assert(inputNames);
		assert(inputNames->size() >= 2);

		return getFunctionCode(getTypeName(), *inputNames);
	}

private:

	std::string getFunctionCode(std::string varName, std::vector<std::string>& inputNames)
	{
		switch (operation)
		{
		case Operation:
		{
			std::string inputList = join(inputNames, std::format(" {} ", functionName));
			return std::format("{} {}{} = {};\n", varName, "func", this->id, inputList);
		}
		case FunctionCall:
		{
			std::string inputList = join(inputNames, ", ");
			return std::format("{} {}{} = {}({});\n", varName, "func", this->id, this->functionName, inputList);
		}
		default:
			break;
		}
	}

	std::string join(const std::vector<std::string>& strings, const std::string& delimiter) const
	{
		std::string result;
		for (int i = 0; i < strings.size(); ++i)
		{
			result += strings[i];
			if (i < strings.size() - 1)
			{
				result += delimiter;
			}
		}
		return result;
	}

	std::string functionName;
	ShaderFunctionOperation operation;
};