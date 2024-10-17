#pragma once
#include "ShaderNodeAttribute.hpp"

class ShaderNodeLink
{
public:

	ShaderNodeLink(int id, ShaderNodeAttribute* first, ShaderNodeAttribute* last)
	{
		this->id = id;
		this->first = first;
		this->last = last;
	}

	~ShaderNodeLink()
	{

	}

	int getId() const
	{
		return id;
	}

	ShaderNodeAttribute* getFirst() const
	{
		return first;
	}

	ShaderNodeAttribute* getLast() const
	{
		return last;
	}

private:

	int id;

	ShaderNodeAttribute* first, * last;
};