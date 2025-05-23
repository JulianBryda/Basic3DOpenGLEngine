#include "ShaderVar.hpp"

#include "../../UI/ShaderNodeEditor/ShaderNodes/ShaderVarNode.hpp"

std::string ShaderVar::getValueOrNameInt(ShaderNodeAttribute& attribute)
{
	if (attribute.connectedTo == nullptr)
	{
		assert(attribute.value);
		return std::to_string(*static_cast<int*>(attribute.value));
	}
	else
	{
		return attribute.connectedTo->node->getShaderVar()->variableName;
	}
}

std::string ShaderVar::getValueOrNameFloat(ShaderNodeAttribute& attribute)
{
	if (attribute.connectedTo == nullptr)
	{
		assert(attribute.value);
		return std::to_string(*static_cast<float*>(attribute.value));
	}
	else
	{
		return attribute.connectedTo->node->getShaderVar()->variableName;
	}
}

std::string ShaderVar::getValueOrNameDouble(ShaderNodeAttribute& attribute)
{
	if (attribute.connectedTo == nullptr)
	{
		assert(attribute.value);
		return std::to_string(*static_cast<double*>(attribute.value));
	}
	else
	{
		return attribute.connectedTo->node->getShaderVar()->variableName;
	}
}