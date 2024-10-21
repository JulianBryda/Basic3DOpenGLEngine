#pragma once
#include <iostream>
#include <format>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "../../UI/ShaderNodeEditor/ShaderNodeAttribute.hpp"


class ShaderVar
{

public:

	ShaderVar(int id, ShaderEnums::ShaderVarPrefix prefix = ShaderEnums::None)
	{
		this->id = id;
		this->prefix = prefix;
		this->variableName = std::format("var{}", id);
		this->include = "";
	}

	~ShaderVar()
	{

	}

	virtual std::string getShaderCode(std::vector<ShaderNodeAttribute>& inputs, ShaderEnums::ShaderVarType outputType)
	{
		if (prefix == ShaderEnums::ShaderVarPrefix::None)
		{
			return std::format("{} {} = {};\n", getTypeName(outputType), variableName, getFormatedValue(inputs, outputType));
		}
		else
		{
			return std::format("{} {} {};\n", getPrefix(), getTypeName(outputType), variableName);
		}
	}

	std::string getInclude() const
	{
		return include;
	}

	ShaderEnums::ShaderVarPrefix getVarPrefix() const
	{
		return prefix;
	}

	void setInclude(std::string value)
	{
		include = value;
	}

	int id;
	ShaderEnums::ShaderVarPrefix prefix;
	std::string variableName;
	std::string include;

protected:

	std::string getPrefix()
	{
		switch (prefix)
		{
		case ShaderEnums::In:
			return "in";
		case ShaderEnums::Out:
			return "out";
		default:
			return "";
		}
	}

	std::string getTypeName(ShaderEnums::ShaderVarType type)
	{
		switch (type)
		{
		case ShaderEnums::INT:
			return "int";
		case ShaderEnums::FLOAT:
			return "float";
		case ShaderEnums::DOUBLE:
			return "double";
		case ShaderEnums::VEC_2:
			return "vec2";
		case ShaderEnums::VEC_3:
			return "vec3";
		case ShaderEnums::VEC_4:
			return "vec4";
		case ShaderEnums::MAT_3:
			return "mat3";
		case ShaderEnums::MAT_4:
			return "mat4";
		case ShaderEnums::SAMPLER_2D:
			return "sampler2D";
		case ShaderEnums::LIGHT:
		case ShaderEnums::LIGHT_ARRAY:
			return "Light";
		default:
			throw std::runtime_error("Type not supported!");
		}
	}

	std::string getFormatedValue(std::vector<ShaderNodeAttribute>& inputs, ShaderEnums::ShaderVarType type)
	{
		switch (type)
		{
		case ShaderEnums::INT:
			assert(inputs.size() >= 1);
			return std::format("{}", getValueOrNameInt(inputs[0]));
		case ShaderEnums::FLOAT:
			assert(inputs.size() >= 1);
			return std::format("{}", getValueOrNameFloat(inputs[0]));
		case ShaderEnums::DOUBLE:
			assert(inputs.size() >= 1);
			return std::format("{}", getValueOrNameDouble(inputs[0]));
		case ShaderEnums::VEC_2:
			assert(inputs.size() >= 2);
			return std::format("vec2({}, {})", getValueOrNameFloat(inputs[0]), getValueOrNameFloat(inputs[1]));
		case ShaderEnums::VEC_3:
			assert(inputs.size() >= 3);
			return std::format("vec3({}, {}, {})", getValueOrNameFloat(inputs[0]), getValueOrNameFloat(inputs[1]), getValueOrNameFloat(inputs[2]));
		case ShaderEnums::VEC_4:
			assert(inputs.size() >= 4);
			return std::format("vec4({}, {}, {}, {})", getValueOrNameFloat(inputs[0]), getValueOrNameFloat(inputs[1]), getValueOrNameFloat(inputs[2]), getValueOrNameFloat(inputs[3]));
		default:
			throw std::runtime_error("Type not supported!");
		}
	}

	std::string getValueOrNameInt(ShaderNodeAttribute& attribute);
	std::string getValueOrNameFloat(ShaderNodeAttribute& attribute);
	std::string getValueOrNameDouble(ShaderNodeAttribute& attribute);

};