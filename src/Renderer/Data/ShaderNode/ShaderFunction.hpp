#pragma once
#include <iostream>
#include <vector>
#include <variant>

#include "ShaderVar.hpp"

template <typename T>
class ShaderFunction : public ShaderVar<T>
{

public:

	ShaderFunction(int id, std::string functionName, std::vector<std::string> inputNames) : ShaderVar<T>(id, nullptr)
	{
		this->functionName = functionName;
		this->inputNames = inputNames;
	}

	~ShaderFunction()
	{

	}

	std::string getShaderCode() override
	{
		std::string inputList = join(inputNames, ", ");

		if (typeid(T) == typeid(glm::vec2))
		{
			return getFunctionCode("vec2", inputList);
		}
		else if (typeid(T) == typeid(glm::vec3))
		{
			return getFunctionCode("vec3", inputList);
		}
		else if (typeid(T) == typeid(glm::vec4))
		{
			return getFunctionCode("vec4", inputList);
		}
		else if (typeid(T) == typeid(glm::mat4))
		{
			return getFunctionCode("mat4", inputList);
		}
		else
		{
			return getFunctionCode(typeid(T).name(), inputList);
		}
	}



private:

	std::string getFunctionCode(const char* type, std::string& inputList)
	{
		return std::format("{} {}{} = {}({});", type, "func", this->id, this->functionName, inputList);
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
};