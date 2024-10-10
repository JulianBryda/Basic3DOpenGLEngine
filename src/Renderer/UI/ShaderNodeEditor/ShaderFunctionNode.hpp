#pragma once
#include "ShaderVarNode.hpp"
#include "../../Data/ShaderNode/ShaderFunction.hpp"

class ShaderFunctionNode : public ShaderVarNode
{

public:

	ShaderFunctionNode(int id, std::string name, std::string functionName, ShaderNodeCategory category, ShaderFunction::ShaderFunctionOperation operation) : ShaderVarNode(id, name, category)
	{
		this->type = ShaderVarNodeType::Function;

		this->shaderVar = new ShaderFunction(id, functionName, 0, operation);
	}

	~ShaderFunctionNode()
	{

	}

	void setFunctionType(GLint type)
	{
		if (!output->immutable)
		{
			output->type = type;
			shaderVar->outputType = type;
		}

		for (auto& input : inputs)
		{
			if (!input.immutable)
			{
				input.type = type;
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