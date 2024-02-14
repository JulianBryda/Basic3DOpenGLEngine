#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <format>
#include <iostream>

#include "Material.hpp"
#include "PointLight.hpp"
#include "DirectionalLight.hpp"


struct LibraryShader
{
	const char* shaderPath;
	GLenum shaderType;
	GLuint shaderId;
};

class Shader
{
public:

	template<typename... Args>
	Shader(std::string_view vShaderPath, std::string_view fShaderPath, Args&&... rest)
	{
		this->name = vShaderPath.substr(vShaderPath.find_last_of("\\") + 3);

		GLuint vertex, fragment;
		int success;
		char infoLog[512];

		// compile shader
		this->compile_shader(vShaderPath, vertex, GL_VERTEX_SHADER);
		this->compile_shader(fShaderPath, fragment, GL_FRAGMENT_SHADER);

		// compile libraries if available
		this->compile_shader_pack(std::forward<Args>(rest)...);

		// shader Program
		this->Id = glCreateProgram();
		glAttachShader(Id, vertex);
		glAttachShader(Id, fragment);

		// attach library shaders
		this->attach_shader_pack(this->Id, std::forward<Args>(rest)...);

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
	inline void setBool(const char* name, bool value) const
	{
		glUniform1i(glGetUniformLocation(Id, name), (int)value);
	}
	inline void setInt(const char* name, int value) const
	{
		glUniform1i(glGetUniformLocation(Id, name), value);
	}
	inline void setFloat(const char* name, float value) const
	{
		glUniform1f(glGetUniformLocation(Id, name), value);
	}
	inline void setFloat3(const char* name, glm::vec3 value) const
	{
		glUniform3f(glGetUniformLocation(Id, name), value.x, value.y, value.z);
	}
	inline void setFloat4(const char* name, glm::vec4 value) const
	{
		glUniform4f(glGetUniformLocation(Id, name), value.x, value.y, value.z, value.w);
	}
	inline void setMat4(const char* name, glm::mat4 value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(Id, name), 1, GL_FALSE, glm::value_ptr(value));
	}
	inline void setTexture(GLenum type, GLuint texture) const
	{
		glBindTexture(type, texture);
	}
	inline void setMaterial(Material* material) const
	{
		this->setFloat3("material.ambient", material->getAmbient());
		this->setFloat3("material.diffuse", material->getDiffuse());
		this->setFloat3("material.specular", material->getSpecular());

		this->setFloat("material.shininess", material->getShininess());
	}
	inline void setPointLight(PointLight* light) const
	{
		this->setFloat3("pointLight.position", light->getPosition());

		this->setFloat3("pointLight.ambient", light->getAmbient());
		this->setFloat3("pointLight.diffuse", light->getDiffuse());
		this->setFloat3("pointLight.specular", light->getSpecular());

		this->setFloat("pointLight.constant", light->getConstant());
		this->setFloat("pointLight.linear", light->getLinear());
		this->setFloat("pointLight.quadratic", light->getQuadratic());
	}
	inline void setPointLight(PointLight* light, int index) const
	{
		this->setFloat3(std::format("pointLights[{}].position", index).c_str(), light->getPosition());

		this->setFloat3(std::format("pointLights[{}].ambient", index).c_str(), light->getAmbient());
		this->setFloat3(std::format("pointLights[{}].diffuse", index).c_str(), light->getDiffuse());
		this->setFloat3(std::format("pointLights[{}].specular", index).c_str(), light->getSpecular());

		this->setFloat(std::format("pointLights[{}].constant", index).c_str(), light->getConstant());
		this->setFloat(std::format("pointLights[{}].linear", index).c_str(), light->getLinear());
		this->setFloat(std::format("pointLights[{}].quadratic", index).c_str(), light->getQuadratic());
	}
	inline void setDirectionalLight(DirectionalLight* light) const
	{
		this->setFloat3("directionalLight.direction", light->getDirection());

		this->setFloat3("directionalLight.ambient", light->getAmbient());
		this->setFloat3("directionalLight.diffuse", light->getDiffuse());
		this->setFloat3("directionalLight.specular", light->getSpecular());
	}
	inline void setDirectionalLight(DirectionalLight* light, int index) const
	{
		this->setFloat3(std::format("directionalLights[{}].direction", index).c_str(), light->getDirection());

		this->setFloat3(std::format("directionalLights[{}].ambient", index).c_str(), light->getAmbient());
		this->setFloat3(std::format("directionalLights[{}].diffuse", index).c_str(), light->getDiffuse());
		this->setFloat3(std::format("directionalLights[{}].specular", index).c_str(), light->getSpecular());
	}

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
			switch (shaderType)
			{
			case GL_VERTEX_SHADER:
				shaderFile.open(std::format(".\\Shader\\{}", shaderPath).c_str());
				break;
			case GL_FRAGMENT_SHADER:
				shaderFile.open(std::format(".\\Shader\\{}", shaderPath).c_str());
				break;
			default:
				return;
			}

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

	void attach_shader_pack(GLuint programId) {}

	template<typename ...Args>
	void attach_shader_pack(GLuint programId, LibraryShader& first, Args&&... rest)
	{
		glAttachShader(programId, first.shaderId);

		this->attach_shader_pack(programId, std::forward<Args>(rest)...);
	}

#pragma region Shader compilation
	void compile_shader(std::string_view shaderPath, GLuint& shaderId, GLenum shaderType)
	{
		std::string shaderCode;

		// read shader code
		this->read_shader_from_file(shaderPath, shaderCode, shaderType);

		// create shader
		this->create_shader(shaderId, shaderCode.c_str(), shaderType);
	}

	void compile_shader_pack() {}

	template<typename... Args>
	void compile_shader_pack(LibraryShader& first, Args&&... rest)
	{
		this->compile_shader(first.shaderPath, first.shaderId, first.shaderType);

		this->compile_shader_pack(std::forward<Args>(rest)...);
	}
#pragma endregion

	GLuint Id;

	std::string name;
};