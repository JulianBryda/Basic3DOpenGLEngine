#pragma once
#include "../ShaderVarNode.hpp"

class Vec2Node : public ShaderVarNode
{
public:

	Vec2Node(int id) : ShaderVarNode(id, "Vec2", ShaderEnums::ShaderNodeCategory::Input, ShaderEnums::None)
	{
		value = glm::vec2(0.f);

		this->addInput({ ShaderEnums::FLOAT, "X", &value.x });
		this->addInput({ ShaderEnums::FLOAT, "Y", &value.y });

		this->setOutput(new ShaderNodeAttribute(ShaderEnums::VEC_2, "Value", &value));
	}

	~Vec2Node()
	{

	}

private:

	glm::vec2 value;

};