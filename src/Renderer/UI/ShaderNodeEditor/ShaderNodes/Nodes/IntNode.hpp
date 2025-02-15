#pragma once
#include "../ShaderVarNode.hpp"

class IntNode : public ShaderVarNode
{
public:

	IntNode(int id) : ShaderVarNode(id, "Int", ShaderEnums::ShaderNodeCategory::Input, ShaderEnums::None)
	{
		value = 0;

		this->addInput({ ShaderEnums::INT, "Int", &value });

		this->setOutput(new ShaderNodeAttribute(ShaderEnums::INT, "Value", &value));
	}

	~IntNode()
	{

	}

private:

	int value;

};