#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <mutex>
#include <thread>

#include "Mesh.hpp"
#include "Vertex.hpp"

namespace ObjectLoader
{
	inline std::vector<std::string> splitString(const std::string input, const char delimiter)
	{
		std::vector<std::string> tokens;
		std::string token;

		for (char ch : input)
		{
			if (ch != delimiter)
			{
				token += ch;
			}
			else
			{
				tokens.push_back(token.c_str());
				token.clear();
			}
		}

		// Add the last token if the string doesn't end with the delimiter
		if (!token.empty())
		{
			tokens.push_back(token.c_str());
		}

		return tokens;
	}

	static void load_obj_file(const char* path, Mesh* mesh)
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
			const size_t index = line.find(' ');
			const std::string identifier = line.substr(0, index);
			const std::string data = line.substr(index + 1, line.size() - (index + 1));

			if (identifier == "v")
			{
				// vertices
				const std::vector<std::string> arr = splitString(data, ' ');
				vertices.push_back(glm::vec3(std::stof(arr[0]), std::stof(arr[1]), std::stof(arr[2])));
			}
			else if (identifier == "vt")
			{
				// uvs
				const std::vector<std::string> arr = splitString(data, ' ');
				uvs.push_back(glm::vec2(std::stof(arr[0]), std::stof(arr[1])));
			}
			else if (identifier == "vn")
			{
				// normals
				const std::vector<std::string> arr = splitString(data, ' ');
				normals.push_back(glm::vec3(std::stof(arr[0]), std::stof(arr[1]), std::stof(arr[2])));
			}
			else if (identifier == "f")
			{
				// faces
				const std::vector<std::string> arr = splitString(data, ' ');
				for (size_t i = 0; i < arr.size(); i++)
				{
					const std::vector<std::string> chars = splitString(arr[i], '/');
					const int vertexIndex = std::stoi(chars[0]) - 1;
					const int uvIndex = std::stoi(chars[1]) - 1;
					const int normalIndex = std::stoi(chars[2]) - 1;

					const Vertex vertex = Vertex(glm::vec3(vertices[vertexIndex]), glm::vec3(normals[normalIndex]), glm::vec2(uvs[uvIndex]));
					auto iterator = std::find(mesh->getVerticesPtr()->begin(), mesh->getVerticesPtr()->end(), vertex);

					if (iterator != mesh->getVerticesPtr()->end())
					{
						// contains
						mesh->getIndicesPtr()->push_back(static_cast<GLuint>(iterator - mesh->getVerticesPtr()->begin()));
					}
					else
					{
						// does not contain
						mesh->getVerticesPtr()->push_back(vertex);
						mesh->getIndicesPtr()->push_back(glm::max((int)mesh->getVerticesPtr()->size() - 1, 0));
					}
				}
			}
		}

		// close file otherwise windows machen dudu
		inputFile.close();
	}

	static void load_obj_file_async(const char* path, Mesh* mesh)
	{
		std::thread thread(load_obj_file, path, mesh);

		thread.join();
	}

};