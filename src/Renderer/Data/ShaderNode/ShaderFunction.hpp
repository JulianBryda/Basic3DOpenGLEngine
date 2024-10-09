#pragma once
#include <iostream>
#include <vector>
#include <variant>

#include "ShaderVar.hpp"

namespace ShaderFunctionEnums
{
	enum ShaderFunctionOperation
	{
		Operation,
		FunctionCall
	};
}

template <typename T>
class ShaderFunction : public ShaderVar<T>
{

public:

	// inputNames not working, getting names of Node Attributes and not shader variables
	ShaderFunction(int id, std::string functionName, std::vector<std::string> inputNames, ShaderFunctionEnums::ShaderFunctionOperation operation) : ShaderVar<T>(id, nullptr)
	{
		if (operation == ShaderFunctionEnums::ShaderFunctionOperation::Operation)
		{
			assert(inputNames.size() >= 2); // when using operations inputNames must be at least 2
		}

		this->functionName = functionName;
		this->inputNames = inputNames;
		this->operation = operation;
	}

	~ShaderFunction()
	{

	}

	std::string getShaderCode() override
	{
		if (typeid(T) == typeid(glm::vec2))
		{
			return getFunctionCode("vec2");
		}
		else if (typeid(T) == typeid(glm::vec3))
		{
			return getFunctionCode("vec3");
		}
		else if (typeid(T) == typeid(glm::vec4))
		{
			return getFunctionCode("vec4");
		}
		else if (typeid(T) == typeid(glm::mat4))
		{
			return getFunctionCode("mat4");
		}
		else
		{
			return getFunctionCode(typeid(T).name());
		}
	}



private:

	std::string getFunctionCode(const char* type)
	{
		switch (operation)
		{
		case ShaderFunctionEnums::Operation:
		{
			std::string inputList = join(inputNames, std::format(" {} ", functionName));
			return std::format("{} {}{} = {};\n", type, "func", this->id, inputList);
		}
		case ShaderFunctionEnums::FunctionCall:
		{
			std::string inputList = join(inputNames, ", ");
			return std::format("{} {}{} = {}({});\n", type, "func", this->id, this->functionName, inputList);
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
	std::vector<std::string> inputNames;
	ShaderFunctionEnums::ShaderFunctionOperation operation;
};