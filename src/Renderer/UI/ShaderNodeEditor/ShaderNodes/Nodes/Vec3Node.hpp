#pragma once
#include "../ShaderVarNode.hpp"

class Vec3Node : public ShaderVarNode
{
public:

	Vec3Node(int id) : ShaderVarNode(id, "Vec3", ShaderEnums::ShaderNodeCategory::Input, ShaderEnums::None)
	{
		value = glm::vec3(0.f);

		this->addInput({ ShaderEnums::FLOAT, "X", &value.x });
		this->addInput({ ShaderEnums::FLOAT, "Y", &value.y });
		this->addInput({ ShaderEnums::FLOAT, "Z", &value.z });

		this->setOutput(new ShaderNodeAttribute(ShaderEnums::VEC_3, "Value", &value));
	}

	~Vec3Node()
	{

	}

private:

	glm::vec3 value;

};