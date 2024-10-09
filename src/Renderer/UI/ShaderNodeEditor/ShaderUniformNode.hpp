#pragma once
#include <algorithm>
#include "ShaderVarNode.hpp"
#include "../../Data/ShaderNode/ShaderUniform.hpp"

template <typename T>
class ShaderUniformNode : public ShaderVarNode<T>
{

public:

	ShaderUniformNode(int id, std::string name, ShaderVarNodeEnums::ShaderNodeCategory category, int arraySize) : ShaderVarNode<T>(id, name, category)
	{
		this->type = ShaderVarNodeEnums::ShaderVarNodeType::Uniform;

		std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {return std::tolower(c); });
		this->shaderVar = new ShaderUniform<T>(id, name, arraySize);
	}

	~ShaderUniformNode()
	{

	}

};