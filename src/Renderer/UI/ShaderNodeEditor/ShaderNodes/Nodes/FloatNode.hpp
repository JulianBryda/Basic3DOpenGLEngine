#pragma once
#include "../ShaderVarNode.hpp"

class FloatNode : public ShaderVarNode
{
public:

	FloatNode(int id) : ShaderVarNode(id, "Float", ShaderEnums::ShaderNodeCategory::Input, ShaderEnums::None)
	{
		value = 0.f;

		this->addInput({ ShaderEnums::FLOAT, "Float", &value });

		this->setOutput(new ShaderNodeAttribute(ShaderEnums::FLOAT, "Value", &value));
	}

	~FloatNode()
	{

	}

private:

	float value;

};