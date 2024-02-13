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
	inline std::vector<Vertex> getVertices() const { return this->vertices; }
	inline std::vector<Vertex>* getVerticesPtr() { return &this->vertices; }

	inline std::vector<GLuint> getIndices() const { return this->indices; }
	inline std::vector<GLuint>* getIndicesPtr() { return &this->indices; }

	// setter


	// modifier
	inline void addVertex(Vertex vertex) { this->vertices.push_back(vertex); }
	inline void addIndex(GLuint index) { this->indices.push_back(index); }

private:

	std::vector<Vertex> vertices;

	std::vector<GLuint> indices;
};