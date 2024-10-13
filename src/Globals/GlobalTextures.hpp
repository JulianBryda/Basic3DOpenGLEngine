#pragma once
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <unordered_map>

namespace Textures
{
	void loadTexture(const char* path, GLuint* texture);
	void loadCubeMap(std::vector<const char*> faces, GLuint& texture);

	void addTexture(std::string name, GLuint texture);
	GLuint& get(std::string name);

	void updateImageTexture(GLuint& texture, int newScreenWidth, int newScreenHeight);

	inline std::unordered_map<size_t, GLuint> g_textures;
}