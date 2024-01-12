#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Primitives
{
public:

	static std::vector<glm::vec3> getRectVertices()
	{
		std::vector<glm::vec3> vertices = {
			glm::vec3(-0.5f, -0.5f, 0.0f),  // Bottom-left
			glm::vec3(0.5f, -0.5f, 0.0f),  // Bottom-right
			glm::vec3(0.5f,  0.5f, 0.0f),  // Top-right
			glm::vec3(-0.5f,  0.5f, 0.0f)   // Top-left
		};

		return vertices;
	}
	static std::vector<GLuint> getRectIndices()
	{
		std::vector<GLuint> indices = {
			0, 1, 2,  // Triangle 1
			2, 3, 0   // Triangle 2
		};

		return indices;
	}


	static std::vector<glm::vec3> getSquareVertices()
	{
		std::vector<glm::vec3> vertices = {
			glm::vec3(-0.5f, -0.5f, 0.0f),  // Bottom-left
			glm::vec3(0.5f, -0.5f, 0.0f),  // Bottom-right
			glm::vec3(0.5f,  0.5f, 0.0f),  // Top-right
			glm::vec3(-0.5f,  0.5f, 0.0f)   // Top-left
		};

		return vertices;
	}
	static std::vector<GLuint> getSquareIndices()
	{
		std::vector<GLuint> indices = {
			0, 1, 2,  // Triangle 1
			2, 3, 0   // Triangle 2
		};

		return indices;
	}
};