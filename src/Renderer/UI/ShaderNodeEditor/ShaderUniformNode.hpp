#pragma once
#include <algorithm>
#include "ShaderVarNode.hpp"
#include "../../Data/ShaderNode/ShaderUniform.hpp"

class ShaderUniformNode : public ShaderVarNode
{

public:

	ShaderUniformNode(int id, std::string name, GLint outputType, ShaderNodeCategory category, int arraySize) : ShaderVarNode(id, name, category)
	{
		this->type = ShaderVarNodeType::Uniform;

		std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {return std::tolower(c); });

		this->shaderVar = new ShaderUniform(id, name, outputType, arraySize);
	}

	~ShaderUniformNode()
	{

	}

};