#pragma once
#include <glad/glad.h>
#include <imgui.h>
#include "ShaderNodes/ShaderEnums.hpp"

class ShaderVarNode;

class ShaderNodeAttribute
{

public:

	ShaderNodeAttribute(ShaderEnums::ShaderVarType type, const char* name, void* value, ShaderEnums::ShaderNodeVisibility visibility = ShaderEnums::Visible)
	{
		this->id = 0;
		this->type = type;
		this->name = name;
		this->immutable = type != ShaderEnums::NONE;
		this->visibility = visibility;

		this->value = value;

		this->node = nullptr;
		this->connectedTo = nullptr;

		this->color = getColor(type);
	}

	ShaderEnums::ShaderVarType getType() const
	{
		return type;
	}

	void setType(ShaderEnums::ShaderVarType type)
	{
		this->type = type;
		this->color = getColor(type);
	}


	int id;
	const char* name;
	bool immutable;
	ShaderEnums::ShaderNodeVisibility visibility;
	void* value;
	unsigned int color;

	ShaderVarNode* node;
	ShaderNodeAttribute* connectedTo;

private:

	ShaderEnums::ShaderVarType type;

	unsigned int getColor(ShaderEnums::ShaderVarType type)
	{
		switch (type)
		{
		case ShaderEnums::INT:
			return IM_COL32(255, 99, 71, 180);
		case ShaderEnums::FLOAT:
			return IM_COL32(30, 144, 255, 180);
		case ShaderEnums::DOUBLE:
			return IM_COL32(0, 0, 139, 180);
		case ShaderEnums::VEC:
			return IM_COL32(32, 178, 170, 180);
		case ShaderEnums::VEC_2:
			return IM_COL32(144, 238, 144, 180);
		case ShaderEnums::VEC_3:
			return IM_COL32(50, 205, 50, 180);
		case ShaderEnums::VEC_4:
			return IM_COL32(0, 100, 0, 180);
		case ShaderEnums::SAMPLER_2D:
			return IM_COL32(147, 112, 219, 180);
		default:
			return IM_COL32(128, 128, 128, 180);
		}
	}
};