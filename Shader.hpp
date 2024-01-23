#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:

	Shader() = default;

	Shader(std::string shaderName)
	{
		this->name = shaderName;

		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			// open files
			vShaderFile.open(std::format("v_{}.glsl", shaderName).c_str());
			fShaderFile.open(std::format("f_{}.glsl", shaderName).c_str());
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		// 2. compile shaders
		GLuint vertex, fragment;
		int success;
		char infoLog[512];

		// vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		// print compile errors if any
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		};

		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		// print compile errors if any
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		};

		// shader Program
		Id = glCreateProgram();
		glAttachShader(Id, vertex);
		glAttachShader(Id, fragment);
		glLinkProgram(Id);
		// print linking errors if any
		glGetProgramiv(Id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(Id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	~Shader() { glDeleteProgram(Id); }

	void use()
	{
		glUseProgram(Id);
	}

	// getter
	GLuint getId() const { return this->Id; }

	std::string getName() const { return this->name; }

	// utility uniform functions
	inline void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(Id, name.c_str()), (int)value);
	}
	inline void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(Id, name.c_str()), value);
	}
	inline void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(Id, name.c_str()), value);
	}
	inline void setFloat3(const std::string& name, glm::vec3 value) const
	{
		glUniform3f(glGetUniformLocation(Id, name.c_str()), value.x, value.y, value.z);
	}
	inline void setFloat4(const std::string& name, glm::vec4 value) const
	{
		glUniform4f(glGetUniformLocation(Id, name.c_str()), value.x, value.y, value.z, value.w);
	}
	inline void setMat4(const std::string& name, glm::mat4 value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}
	inline void setTexture(GLenum type, GLuint texture) const
	{
		glBindTexture(type, texture);
	}

private:

	GLuint Id;

	std::string name;
};