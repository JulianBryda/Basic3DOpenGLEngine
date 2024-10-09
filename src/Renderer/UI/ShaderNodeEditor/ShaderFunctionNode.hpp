#pragma once
#include "ShaderVarNode.hpp"
#include "../../Data/ShaderNode/ShaderFunction.hpp"

template <typename T>
class ShaderFunctionNode : public ShaderVarNode<T>
{

public:

	ShaderFunctionNode(int id, std::string name, std::string functionName, ShaderVarNodeEnums::ShaderNodeCategory category) : ShaderVarNode<T>(id, name, category)
	{
		this->type = ShaderVarNodeEnums::ShaderVarNodeType::Function;

		this->shaderVar = new ShaderFunction<T>(id, functionName, getNameList(this->inputs));
	}

	~ShaderFunctionNode()
	{

	}

private:

	std::vector<std::string> getNameList(std::vector<std::pair<GLint, std::string>>& inputs)
	{
		std::vector<std::string> nameList;

		for (auto& input : inputs)
		{
			nameList.push_back(input.second);
		}

		return nameList;
	}

};