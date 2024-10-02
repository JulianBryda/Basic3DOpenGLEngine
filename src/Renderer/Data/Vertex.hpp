#pragma once
#include <iostream>
#include <glm/glm.hpp>

class Vertex
{

public:

	Vertex(glm::vec3 vertex, glm::vec3 normal, glm::vec2 uv)
	{
		this->m_vertex = vertex;
		this->m_normal = normal;
		this->m_uv = uv;
	}

	~Vertex()
	{

	}

	inline bool operator==(const Vertex& other)
	{
		return other.m_vertex == this->m_vertex && other.m_normal == this->m_normal && other.m_uv == this->m_uv;
	}


	// getter
	glm::vec3 getVertex() const { return this->m_vertex; }
	glm::vec3* getVertexPtr() { return &this->m_vertex; }
	glm::vec3 getNormal() const { return this->m_normal; }
	glm::vec3* getNormalPtr() { return &this->m_normal; }
	glm::vec2 getUv() const { return this->m_uv; }
	glm::vec2* getUvPtr() { return &this->m_uv; }

private:

	glm::vec3 m_vertex;
	glm::vec3 m_normal;
	glm::vec2 m_uv;
};