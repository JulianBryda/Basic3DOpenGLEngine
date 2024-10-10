#pragma once
#include <iostream>
#include <format>
#include <string>

#include <glm/glm.hpp>

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
		this->value = value;
		this->outputType = outputType;
		this->prefix = prefix;
	}

	~ShaderVar()
	{
		if (value)
			delete value;
	}

	virtual std::string getShaderCode(std::vector<std::string>* inputNames)
	{
		return getVarCode(getTypeName());
	}

	int id;
	void* value;
	GLint outputType;
	ShaderVarPrefix prefix;

protected:

	std::string getVarCode(std::string type)
	{
		if (value)
		{
			return std::format("{}{} {}{} = {};\n", getPrefix(), type, "var", this->id, getFormatedValue());
		}
		else
		{
			return std::format("{}{} {}{};\n", getPrefix(), type, "var", this->id);
		}
	}

	std::string getPrefix()
	{
		switch (prefix)
		{
		case In:
			return "in ";
		case Out:
			return "out ";
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

	std::string getFormatedValue()
	{
		switch (outputType)
		{
		case GL_INT:
			return std::format("{}", *static_cast<int*>(value));
		case GL_FLOAT:
			return std::format("{}", *static_cast<float*>(value));
		case GL_DOUBLE:
			return std::format("{}", *static_cast<double*>(value));
		case GL_FLOAT_VEC2:
			return std::format("vec2({}, {})", static_cast<glm::vec2*>(value)->x, static_cast<glm::vec2*>(value)->y);
		case GL_FLOAT_VEC3:
			return std::format("vec3({}, {}, {})", static_cast<glm::vec3*>(value)->x, static_cast<glm::vec3*>(value)->y, static_cast<glm::vec3*>(value)->z);
		case GL_FLOAT_VEC4:
			return std::format("vec4({}, {}, {}, {})", static_cast<glm::vec4*>(value)->x, static_cast<glm::vec4*>(value)->y, static_cast<glm::vec4*>(value)->z, static_cast<glm::vec4*>(value)->a);
		default:
			throw std::runtime_error("Type not supported!");
		}
	}

};