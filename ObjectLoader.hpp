#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "Mesh.hpp"
#include "Vertex.hpp"

namespace ObjectLoader
{

	static std::vector<std::string> splitString(std::string input, const char delimiter);

	static void LoadObjFile(std::string path, Mesh* mesh)
	{
		std::ifstream inputFile(path);

		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;

		// open file
		if (!inputFile.is_open())
		{
			std::cerr << "Failed to open file: " << path << std::endl;
			return;
		}

		// read file line by line
		std::string line;
		while (std::getline(inputFile, line))
		{
			size_t index = line.find(' ');
			std::string identifier = line.substr(0, index);
			std::string data = line.substr(index + 1, line.size() - (index + 1));

			if (identifier == "v")
			{
				// vertices
				std::vector<std::string> arr = splitString(data, ' ');
				vertices.push_back(glm::vec3(std::stof(arr[0]), std::stof(arr[1]), std::stof(arr[2])));
			}
			else if (identifier == "vt")
			{
				// uvs
				std::vector<std::string> arr = splitString(data, ' ');
				uvs.push_back(glm::vec2(std::stof(arr[0]), std::stof(arr[1])));
			}
			else if (identifier == "vn")
			{
				// normals
				std::vector<std::string> arr = splitString(data, ' ');
				normals.push_back(glm::vec3(std::stof(arr[0]), std::stof(arr[1]), std::stof(arr[2])));
			}
			else if (identifier == "f")
			{
				// faces
				std::vector<std::string> arr = splitString(data, ' ');
				for (size_t i = 0; i < arr.size(); i++)
				{
					// not working, fix this shit! :(
					std::vector<std::string> chars = splitString(arr[i], '/');
					int vertexIndex = std::stoi(chars[0]) - 1;
					int uvIndex = std::stoi(chars[1]) - 1;
					int normalIndex = std::stoi(chars[2]) - 1;

					Vertex vertex = Vertex(glm::vec3(vertices[vertexIndex]), glm::vec3(normals[normalIndex]), glm::vec2(uvs[uvIndex]));

					mesh->getVerticesPtr()->push_back(vertex);
					mesh->getIndicesPtr()->push_back(vertexIndex);
				}
			}
		}

		// close file otherwise windows machen dudu
		inputFile.close();
	}

	static std::vector<std::string> splitString(std::string input, const char delimiter)
	{
		std::vector<std::string> tokens;
		std::string token;

		for (char ch : input) {
			if (ch != delimiter) {
				token += ch;
			}
			else {
				tokens.push_back(token);
				token.clear();
			}
		}

		// Add the last token if the string doesn't end with the delimiter
		if (!token.empty()) {
			tokens.push_back(token);
		}

		return tokens;
	}

};