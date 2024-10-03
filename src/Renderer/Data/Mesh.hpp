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

	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
	{
		m_vertices = vertices;
		m_indices = indices;
	}

	Mesh(const Mesh& other)
	{
		m_vertices = other.m_vertices;
		m_indices = other.m_indices;
	}

	~Mesh()
	{

	}

	void calculateNormals()
	{
		for (int i = 0; i < m_indices.size(); i += 3)
		{
			glm::vec3 v0 = m_vertices[m_indices[i]].getVertex();
			glm::vec3 v1 = m_vertices[m_indices[i + 1]].getVertex();
			glm::vec3 v2 = m_vertices[m_indices[i + 2]].getVertex();

			glm::vec3 edge1 = v1 - v0;
			glm::vec3 edge2 = v2 - v0;

			glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

			m_vertices[m_indices[i]].setNormal(faceNormal);
			m_vertices[m_indices[i + 1]].setNormal(faceNormal);
			m_vertices[m_indices[i + 2]].setNormal(faceNormal);
		}
	}

	// getter
	inline std::vector<Vertex>& getVertices() { return this->m_vertices; }
	inline std::vector<Vertex>* getVerticesPtr() { return &this->m_vertices; }

	inline std::vector<GLuint>& getIndices() { return this->m_indices; }
	inline std::vector<GLuint>* getIndicesPtr() { return &this->m_indices; }

	// setter
	inline void setVertices(std::vector<Vertex>& value) { this->m_vertices = value; }
	inline void setIndices(std::vector<GLuint>& value) { this->m_indices = value; }

	// modifier
	inline void addVertex(Vertex vertex) { this->m_vertices.push_back(vertex); }
	inline void addIndex(GLuint index) { this->m_indices.push_back(index); }

private:

	std::vector<Vertex> m_vertices;

	std::vector<GLuint> m_indices;
};