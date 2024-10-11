#pragma once
#include <glad/glad.h>

class ShaderVarNode;

class ShaderNodeAttribute
{

public:

	ShaderNodeAttribute(GLint type, const char* name, void* value)
	{
		this->id = 0;
		this->type = type;
		this->name = name;
		this->immutable = type != 0;
		this->hide = false;

		this->value = value;

		this->node = nullptr;
		this->connectedTo = nullptr;
	}

	int id;
	GLint type;
	const char* name;
	bool immutable, hide;
	void* value;

	ShaderVarNode* node;
	ShaderNodeAttribute* connectedTo;
};