#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "Vertex.hpp"

class Mesh
{

public:

	Mesh()
	{

	}

	~Mesh()
	{

	}

	// getter
	std::vector<Vertex> getVertices() const { return this->vertices; }
	std::vector<Vertex>* getVerticesPtr() { return &this->vertices; }

	std::vector<GLuint> getIndices() const { return this->indices; }
	std::vector<GLuint>* getIndicesPtr() { return &this->indices; }

	// setter

private:

	std::vector<Vertex> vertices;

	std::vector<GLuint> indices;
};