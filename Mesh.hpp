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

	Mesh(const Mesh& other)
	{
		m_vertices = other.m_vertices;
		m_indices = other.m_indices;
	}

	~Mesh()
	{

	}

	// getter
	inline std::vector<Vertex>& getVertices() { return this->m_vertices; }
	inline std::vector<Vertex>* getVerticesPtr() { return &this->m_vertices; }

	inline std::vector<GLuint>& getIndices() { return this->m_indices; }
	inline std::vector<GLuint>* getIndicesPtr() { return &this->m_indices; }

	// setter


	// modifier
	inline void addVertex(Vertex vertex) { this->m_vertices.push_back(vertex); }
	inline void addIndex(GLuint index) { this->m_indices.push_back(index); }

private:

	std::vector<Vertex> m_vertices;

	std::vector<GLuint> m_indices;
};