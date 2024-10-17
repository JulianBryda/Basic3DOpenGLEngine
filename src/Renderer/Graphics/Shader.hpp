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
			readAndCompile(shader, m_id, type);
		}
		else
		{
			processAndCompile(m_id, shader, type);
		}
	}

	~Shader()
	{
		glDeleteShader(this->m_id);
	}

	// getter
	GLuint getId() const { return this->m_id; }


private:

	void processAndCompile(GLuint& shaderId, std::string& shaderCode, GLenum shaderType)
	{
		processIncludes(shaderCode);

		compile(shaderId, shaderCode.c_str(), shaderType);
	}

	void readAndCompile(std::string& shaderPath, GLuint& shaderId, GLenum shaderType)
	{
		std::string shaderCode;

		// read shader code
		this->readFromFile(shaderPath, shaderCode);

		// create shader
		this->processAndCompile(shaderId, shaderCode, shaderType);
	}

	void compile(GLuint& shaderId, const char* shaderCode, GLenum shaderType)
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
			std::cout << shaderCode << std::endl;
			return;
		};
	}

	void readFromFile(const std::string& shaderPath, std::string& shaderCode)
	{
		// 1. retrieve the source code from filePath
		std::ifstream shaderFile;
		// ensure ifstream objects can throw exceptions:
		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			// open files
			shaderFile.open(std::format(".\\Shader{}", shaderPath).c_str());

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
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n" << e.what() << std::endl;
		}
	}

	void processIncludes(std::string& shaderCode)
	{
		std::istringstream stream(shaderCode);
		std::string line;

		while (std::getline(stream, line))
		{
			std::string trimedLine = trim(line);
			if (trimedLine.substr(0, 8) != "#include") continue;

			std::string includeName = trimedLine.substr(10, line.size() - 11);
			std::string includePath = "\\Library\\" + includeName;

			std::string libraryShaderCode;
			readFromFile(includePath, libraryShaderCode);

			shaderCode = shaderCode.replace(shaderCode.find(line), line.size(), libraryShaderCode);
		}
	}


	std::string ltrim(const std::string& s)
	{
		size_t start = s.find_first_not_of(" \t\n\r\f\v");
		return (start == std::string::npos) ? "" : s.substr(start);
	}

	std::string rtrim(const std::string& s)
	{
		size_t end = s.find_last_not_of(" \t\n\r\f\v");
		return (end == std::string::npos) ? "" : s.substr(0, end + 1);
	}

	std::string trim(const std::string& s)
	{
		return rtrim(ltrim(s));
	}


	GLuint m_id;
	GLenum type;
};