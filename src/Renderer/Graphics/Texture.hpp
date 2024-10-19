#pragma once
#include <string>
#include <glad/glad.h>

class Texture
{
public:

	Texture(GLuint id, GLenum type, GLint index, std::string name)
	{
		this->name = name;

		this->id = id;
		this->type = type;
		this->index = index;
	}

	~Texture()
	{

	}

	GLuint getId() const { return id; }
	GLenum getType() const { return type; }
	GLint getIndex() const { return index; }

	std::string getName() const { return name; }

private:

	std::string name;

	GLuint id;
	GLenum type;
	GLint index;
};