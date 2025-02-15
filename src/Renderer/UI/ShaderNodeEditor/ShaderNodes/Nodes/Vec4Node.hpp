#pragma once
#include "../ShaderVarNode.hpp"

class Vec4Node : public ShaderVarNode
{
public:

	Vec4Node(int id) : ShaderVarNode(id, "Vec4", ShaderEnums::ShaderNodeCategory::Input, ShaderEnums::None)
	{
		value = glm::vec4(0.f);

		this->addInput({ ShaderEnums::FLOAT, "X", &value.x });
		this->addInput({ ShaderEnums::FLOAT, "Y", &value.y });
		this->addInput({ ShaderEnums::FLOAT, "Z", &value.z });
		this->addInput({ ShaderEnums::FLOAT, "W", &value.w });

		this->setOutput(new ShaderNodeAttribute(ShaderEnums::VEC_4, "Value", &value));
	}

	~Vec4Node()
	{

	}

private:

	glm::vec4 value;

};