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
		if (output && !output->immutable && output->getType() == ShaderEnums::ShaderVarType::NONE)
		{
			output->setType(type);
		}

		for (auto& input : inputs)
		{
			if (!input.immutable)
			{
				input.setType(getInputType(type));
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

	ShaderEnums::ShaderVarType getInputType(ShaderEnums::ShaderVarType type)
	{
		switch (type)
		{
		case ShaderEnums::VEC_2:
			return static_cast<ShaderEnums::ShaderVarType>(type | ShaderEnums::FLOAT);
		case ShaderEnums::VEC_3:
			return static_cast<ShaderEnums::ShaderVarType>(type | ShaderEnums::MAT_3 | ShaderEnums::FLOAT);
		case ShaderEnums::VEC_4:
			return static_cast<ShaderEnums::ShaderVarType>(type | ShaderEnums::MAT_4 | ShaderEnums::FLOAT);
		case ShaderEnums::MAT_3:
			return static_cast<ShaderEnums::ShaderVarType>(type | ShaderEnums::VEC_3 | ShaderEnums::FLOAT);
		case ShaderEnums::MAT_4:
			return static_cast<ShaderEnums::ShaderVarType>(type | ShaderEnums::VEC_4 | ShaderEnums::FLOAT);
		default:
			return type;
		}
	}

};