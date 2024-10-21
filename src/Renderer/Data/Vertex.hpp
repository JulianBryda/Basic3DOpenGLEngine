#pragma once
#include <iostream>
#include <glm/glm.hpp>

struct Vertex
{

	Vertex(glm::vec3 vertex, glm::vec3 normal, glm::vec2 uv, glm::vec3 tangent, glm::vec3 bitangent)
	{
		this->vertex = vertex;
		this->normal = normal;
		this->uv = uv;
		this->tangent = tangent;
		this->bitangent = bitangent;
	}

	inline bool operator==(const Vertex& other)
	{
		return other.vertex == this->vertex && other.normal == this->normal && other.uv == this->uv && other.tangent == this->tangent && other.bitangent == this->bitangent;
	}

	glm::vec3 vertex;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};