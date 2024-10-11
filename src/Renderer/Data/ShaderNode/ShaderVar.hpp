#pragma once
#include <iostream>
#include <format>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "../../UI/ShaderNodeEditor/ShaderNodeAttribute.hpp"


class ShaderVar
{

public:

	enum ShaderVarPrefix
	{
		None,
		In,
		Out
	};

	ShaderVar(int id, void* value, GLint outputType, ShaderVarPrefix prefix = None)
	{
		this->id = id;
		this->outputType = outputType;
		this->prefix = prefix;
		this->compiled = false;
	}

	~ShaderVar()
	{

	}

	virtual std::string getShaderCode(std::vector<ShaderNodeAttribute>& inputs)
	{
		if (compiled == true) return "";

		std::string result;

		if (prefix == ShaderVarPrefix::None)
		{
			result = std::format("{} {} = {};\n", getTypeName(), getVariableName(), getFormatedValue(inputs));
		}
		else
		{
			result = std::format("{} {} {};\n", getPrefix(), getTypeName(), getVariableName());
		}

		compiled = true;

		return result;
	}

	virtual std::string getVariableName()
	{
		return std::format("var{}", this->id);
	}

	int id;
	GLint outputType;
	ShaderVarPrefix prefix;
	bool compiled;

protected:

	std::string getPrefix()
	{
		switch (prefix)
		{
		case In:
			return "in";
		case Out:
			return "out";
		default:
			return "";
		}
	}

	std::string getTypeName()
	{
		switch (outputType)
		{
		case GL_INT:
			return "int";
		case GL_FLOAT:
			return "float";
		case GL_DOUBLE:
			return "double";
		case GL_FLOAT_VEC2:
			return "vec2";
		case GL_FLOAT_VEC3:
			return "vec3";
		case GL_FLOAT_VEC4:
			return "vec4";
		default:
			throw std::runtime_error("Type not supported!");
		}
	}

	std::string getFormatedValue(std::vector<ShaderNodeAttribute>& inputs)
	{
		switch (outputType)
		{
		case GL_INT:
			assert(inputs.size() >= 1);
			return std::format("{}", getValueOrNameInt(inputs[0]));
		case GL_FLOAT:
			assert(inputs.size() >= 1);
			return std::format("{}", getValueOrNameFloat(inputs[0]));
		case GL_DOUBLE:
			assert(inputs.size() >= 1);
			return std::format("{}", getValueOrNameDouble(inputs[0]));
		case GL_FLOAT_VEC2:
			assert(inputs.size() >= 2);
			return std::format("vec2({}, {})", getValueOrNameFloat(inputs[0]), getValueOrNameFloat(inputs[1]));
		case GL_FLOAT_VEC3:
			assert(inputs.size() >= 3);
			return std::format("vec3({}, {}, {})", getValueOrNameFloat(inputs[0]), getValueOrNameFloat(inputs[1]), getValueOrNameFloat(inputs[2]));
		case GL_FLOAT_VEC4:
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