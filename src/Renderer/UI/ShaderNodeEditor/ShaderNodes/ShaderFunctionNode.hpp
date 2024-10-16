#pragma once
#include "ShaderVarNode.hpp"
#include "../../../Data/ShaderNode/ShaderFunction.hpp"

class ShaderFunctionNode : public ShaderVarNode
{

public:

	ShaderFunctionNode(int id, std::string name, std::string functionName, ShaderEnums::ShaderNodeCategory category, ShaderFunction::ShaderFunctionOperation operation) : ShaderVarNode(id, name, category)
	{
		this->type = ShaderEnums::ShaderVarNodeType::Function;

		this->shaderVar = new ShaderFunction(id, functionName, operation);
	}

	~ShaderFunctionNode()
	{

	}

	void setFunctionType(ShaderEnums::ShaderVarType type)
	{
		if (output && !output->immutable)
		{
			output->setType(type);
		}

		for (auto& input : inputs)
		{
			if (!input.immutable)
			{
				input.setType(type);
			}
		}
	}

private:

	void initIdsOfShaderNodeAttributes(std::vector<ShaderNodeAttribute>& inputs)
	{
		for (int i = 0; i < inputs.size(); i++)
		{
			inputs[i].id = this->id * 500 + i + 2;
		}
	}

};