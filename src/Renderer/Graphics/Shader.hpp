#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "../../Scene/Lights/PointLight.hpp"
#include "../../Scene/Lights/SpotLight.hpp"
#include "../../Scene/Lights/DirectionalLight.hpp"

class Shader
{
public:

	Shader(std::string shader, GLenum type, bool readFromFile = true)
	{
		this->type = type;

		// compile shader
		if (readFromFile)
		{
			compile_shader(shader, m_id, type);
		}
		else
		{
			create_shader(m_id, shader.c_str(), type);
		}
	}

	~Shader()
	{
		glDeleteShader(this->m_id);
	}

	// getter
	GLuint getId() const { return this->m_id; }


private:

	void create_shader(GLuint& shaderId, const char* shaderCode, GLenum shaderType)
	{
		int success;
		char infoLog[512];

		// vertex Shader
		shaderId = glCreateShader(shaderType);
		glShaderSource(shaderId, 1, &shaderCode, NULL);
		glCompileShader(shaderId);
		// print compile errors if any
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		};
	}

	void read_shader_from_file(std::string_view shaderPath, std::string& shaderCode, GLenum shaderType)
	{
		// 1. retrieve the source code from filePath
		std::ifstream shaderFile;
		// ensure ifstream objects can throw exceptions:
		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			// open files
			shaderFile.open(std::format(".\\Shader\\{}", shaderPath).c_str());

			std::stringstream shaderStream;
			// read file's buffer contents into streams
			shaderStream << shaderFile.rdbuf();
			// close file handlers
			shaderFile.close();
			// convert stream into string
			shaderCode = shaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
	}

	void compile_shader(std::string_view shaderPath, GLuint& shaderId, GLenum shaderType)
	{
		std::string shaderCode;

		// read shader code
		this->read_shader_from_file(shaderPath, shaderCode, shaderType);

		// create shader
		this->create_shader(shaderId, shaderCode.c_str(), shaderType);
	}

	GLuint m_id;
	GLenum type;
};