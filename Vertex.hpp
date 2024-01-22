#pragma once
#include <iostream>
#include <glm/glm.hpp>

class Vertex
{

public:

	Vertex(glm::vec3 vertex, glm::vec3 normal, glm::vec2 uv)
	{
		this->vertex = vertex;
		this->normal = normal;
		this->uv = uv;
	}

	~Vertex()
	{

	}

	inline bool operator==(const Vertex& other)
	{
		return other.vertex == this->vertex && other.normal == this->normal && other.uv == this->uv;
	}


	// getter
	glm::vec3 getVertex() const { return this->vertex; }
	glm::vec3* getVertexPtr() { return &this->vertex; }
	glm::vec3 getNormal() const { return this->normal; }
	glm::vec3* getNormalPtr() { return &this->normal; }
	glm::vec2 getUv() const { return this->uv; }
	glm::vec2* getUvPtr() { return &this->uv; }

private:

	glm::vec3 vertex;
	glm::vec3 normal;
	glm::vec2 uv;
};