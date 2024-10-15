#pragma once
#include <string>
#include <glad/glad.h>

class Texture
{
public:

	Texture(GLuint id, GLenum type, GLint index, std::string name)
	{
		this->name = std::hash<std::string>()(name);

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

private:

	size_t name;

	GLuint id;
	GLenum type;
	GLint index;
};