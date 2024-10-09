#pragma once
#include <iostream>
#include <format>
#include <string>

#include <glm/glm.hpp>

enum ShaderVarPrefix
{
	None,
	In,
	Out
};

template <typename T>
class ShaderVar
{

public:

	ShaderVar(int id, T* value, ShaderVarPrefix prefix = None)
	{
		this->id = id;
		this->value = value;
		this->prefix = prefix;
	}

	~ShaderVar()
	{

	}

	virtual std::string getShaderCode()
	{
		if (typeid(T) == typeid(glm::vec2))
		{
			return getVarCode("vec2");
		}
		else if (typeid(T) == typeid(glm::vec3))
		{
			return getVarCode("vec3");
		}
		else if (typeid(T) == typeid(glm::vec4))
		{
			return getVarCode("vec4");
		}
		else if (typeid(T) == typeid(glm::mat4))
		{
			return getVarCode("mat4");
		}
		else
		{
			return getVarCode(typeid(T).name());
		}
	}

	bool operator==(const ShaderVar& other)
	{
		return id == other.id && value == other.name;
	}

	int id;
	T* value;
	ShaderVarPrefix prefix;

protected:

	std::string getVarCode(const char* type)
	{
		if (value)
		{
			return std::format("{}{} {}{} = {};\n", getPrefix(), type, "var", this->id, getValue<T>(*value));
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

	template<typename V>
	std::string getValue(const V& value)
	{
		return "";
	}

	template<>
	std::string getValue(const int& value)
	{
		return std::to_string(value);
	}

	template<>
	std::string getValue(const float& value)
	{
		return std::to_string(value);
	}

	template<>
	std::string getValue(const glm::vec2& value)
	{
		return std::format("vec2({}, {})", value.x, value.y);
	}

	template<>
	std::string getValue(const glm::vec3& value)
	{
		return std::format("vec3({}, {}, {})", value.x, value.y, value.z);
	}

	template<>
	std::string getValue(const glm::vec4& value)
	{
		return std::format("vec4({}, {}, {}, {})", value.x, value.y, value.z, value.a);
	}

};