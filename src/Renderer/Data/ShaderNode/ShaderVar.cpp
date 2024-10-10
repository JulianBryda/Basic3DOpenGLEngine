#include "ShaderVar.hpp"

#include "../../UI/ShaderNodeEditor/ShaderVarNode.hpp"

std::string ShaderVar::getShaderCode(std::vector<ShaderNodeAttribute>& inputs)
{
	return getVarCode(getTypeName());
}