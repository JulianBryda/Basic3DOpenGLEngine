#pragma once
#include <iostream>
#include <variant>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <format>
#include <type_traits>

#include "Shader.hpp"
#include "Texture.hpp"

class Material
{

public:

	template <typename T>
	struct Parameter
	{
		const char* name;
		GLint arraySize;
		T* value;
	};

	using UniformType = std::variant<Parameter<bool>, Parameter<float>, Parameter<int>, Parameter<glm::vec2>, Parameter<glm::vec3>, Parameter<glm::mat4>>;

	template<typename... Args>
	Material(std::string name, bool nodeBased, Args*... shaders)
	{
		static_assert(sizeof...(shaders) > 0, "No Shaders provided!");
		static_assert((std::is_same_v<Args, Shader> && ...), "All arguments must be of type Shader!");

		this->m_id = glCreateProgram();
		this->m_name = name;
		this->nodeBased = nodeBased;

		std::vector<Shader*> shaderVector = makeVector(shaders...);
		compileMaterial(shaderVector);
	}

	~Material()
	{
		glDeleteProgram(this->m_id);
	}

	template<typename... Args>
	void update(Args*... shaders)
	{
		static_assert(sizeof...(shaders) > 0, "No Shaders provided!");
		static_assert((std::is_same_v<Args, Shader> && ...), "All arguments must be of type Shader!");

		glDeleteProgram(m_id);
		m_id = glCreateProgram();

		std::vector<Shader*> shaderVector = makeVector(shaders...);
		compileMaterial(shaderVector);
	}

	void use() const
	{
		glUseProgram(this->m_id);
	}

	void setUniform(const UniformType& param)
	{
		std::visit([&](auto&& arg)
			{
				setUniformParam(arg);
			}, param);
	}

	// utility uniform functions
	inline void setBool(const char* name, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_id, name), (int)value);
	}
	inline void setInt(const char* name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_id, name), value);
	}
	inline void setUInt(const char* name, unsigned int value) const
	{
		glUniform1ui(glGetUniformLocation(m_id, name), value);
	}
	inline void setFloat(const char* name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_id, name), value);
	}
	inline void setFloat3(const char* name, glm::vec3 value) const
	{
		glUniform3f(glGetUniformLocation(m_id, name), value.x, value.y, value.z);
	}
	inline void setFloat4(const char* name, glm::vec4 value) const
	{
		glUniform4f(glGetUniformLocation(m_id, name), value.x, value.y, value.z, value.w);
	}
	inline void setMat4(const char* name, glm::mat4 value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, glm::value_ptr(value));
	}

	inline void setTexture(const Texture& texture) const
	{
		setTexture(texture.getType(), texture.getId(), texture.getIndex());
	}

	inline void setTexture(GLenum type, GLuint texture, GLuint textureIndex) const
	{
		glActiveTexture(textureIndex);
		glBindTexture(type, texture);
	}

	inline void setMaterialTextures(GLuint textureIndexOffset = 0)
	{
		for (auto& texture : m_textures)
		{
			setTexture(texture.getType(), texture.getId(), texture.getIndex() + textureIndexOffset);
			setInt(texture.getName().c_str(), texture.getIndex() + textureIndexOffset - GL_TEXTURE0);
		}
	}

	inline void setImageTexture(GLuint texture, GLenum format)
	{
		glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, format);
	}

	inline void setLight(Light* light, int index, GLuint textureIndex) const
	{
		switch (light->getLightType())
		{
		case Spot:
		{
			SpotLight* spotLight = static_cast<SpotLight*>(light);

			this->setFloat3(std::format("lights[{}].direction", index).c_str(), spotLight->getDirection());
			this->setFloat(std::format("lights[{}].innerCone", index).c_str(), spotLight->getInnerCone());
			this->setFloat(std::format("lights[{}].outerCone", index).c_str(), spotLight->getOuterCone());
			break;
		}
		case Directional:
		{
			DirectionalLight* directionalLigt = static_cast<DirectionalLight*>(light);

			this->setFloat3(std::format("lights[{}].direction", index).c_str(), directionalLigt->getDirection());

			break;
		}
		default:
			break;
		}

		this->setFloat3(std::format("lights[{}].position", index).c_str(), light->getPosition());
		this->setFloat3(std::format("lights[{}].color", index).c_str(), light->getColor());

		this->setTexture(GL_TEXTURE_2D, light->getDepthMap(), textureIndex);
		this->setInt(std::format("lights[{}].shadowMap", index).c_str(), textureIndex - GL_TEXTURE0);
		this->setMat4(std::format("lights[{}].lightSpaceMatrix", index).c_str(), light->getProjectionMatrix() * light->getViewMatrix());

		this->setInt(std::format("lights[{}].lightType", index).c_str(), light->getLightType());
	}

	inline void setLightCount(size_t count) const
	{
		this->setInt("lightcount", static_cast<int>(count));
	}

	void addTexture(Texture texture)
	{
		m_textures.push_back(texture);
	}

	std::string getName() const
	{
		return m_name;
	}

	std::vector<size_t>& getParameterAtlas()
	{
		return m_parameterAtlas;
	}

	bool getNodeBased() const
	{
		return nodeBased;
	}

	std::vector<Texture>& getTextures()
	{
		return m_textures;
	}

private:

	template <typename... Args>
	std::vector<std::common_type_t<Args...>> makeVector(Args... args)
	{
		return { args... };
	}

	template <typename T>
	void setUniformParam(const Parameter<T>& param);

	template <>
	void setUniformParam(const Parameter<bool>& param)
	{
		GLint location = glGetUniformLocation(m_id, param.name);

		if (param.arraySize > 1)
		{
			glUniform1iv(location, param.arraySize, (GLint*)param.value);
		}
		else
		{
			glUniform1i(location, *param.value);
		}
	}

	template <>
	void setUniformParam(const Parameter<int>& param)
	{
		GLint location = glGetUniformLocation(m_id, param.name);

		if (param.arraySize > 1)
		{
			glUniform1iv(location, param.arraySize, param.value);
		}
		else
		{
			glUniform1i(location, *param.value);
		}
	}

	template <>
	void setUniformParam(const Parameter<float>& param)
	{
		GLint location = glGetUniformLocation(m_id, param.name);

		if (param.arraySize > 1)
		{
			glUniform1fv(location, param.arraySize, param.value);
		}
		else
		{
			glUniform1f(location, *param.value);
		}
	}

	template <>
	void setUniformParam(const Parameter<glm::vec2>& param)
	{
		GLint location = glGetUniformLocation(m_id, param.name);

		if (param.arraySize > 1)
		{
			glUniform2fv(location, param.arraySize, glm::value_ptr(*param.value));
		}
		else
		{
			glUniform2f(location, param.value->x, param.value->y);
		}
	}

	template <>
	void setUniformParam(const Parameter<glm::vec3>& param)
	{
		GLint location = glGetUniformLocation(m_id, param.name);

		if (param.arraySize > 1)
		{
			glUniform3fv(location, param.arraySize, glm::value_ptr(*param.value));
		}
		else
		{
			glUniform3f(location, param.value->x, param.value->y, param.value->z);
		}
	}

	template <>
	void setUniformParam(const Parameter<glm::mat4>& param)
	{
		GLint location = glGetUniformLocation(m_id, param.name);

		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(*param.value));
	}

	void compileMaterial(std::vector<Shader*>& shaderVector)
	{
		for (auto& shader : shaderVector)
		{
			glAttachShader(this->m_id, shader->getId());
		}

		glLinkProgram(this->m_id);
		// print linking errors if any
		int success;
		char infoLog[512];
		glGetProgramiv(this->m_id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(this->m_id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		// get uniforms required by shader and save it in m_parameterAtlas
		extractShaderParameters();

		for (Shader* shader : shaderVector)
		{
			delete shader;
		}
	}

	void extractShaderParameters()
	{
#ifdef _DEBUG
		std::cout << "Loaded Shader \"" << m_name << "\" with parameters:\n";
#endif

		GLint uniformCount;
		glGetProgramInterfaceiv(this->m_id, GL_UNIFORM, GL_ACTIVE_RESOURCES, &uniformCount);

		std::vector<GLchar> nameData(256);
		std::vector<GLenum> properties;
		properties.push_back(GL_NAME_LENGTH);
		properties.push_back(GL_TYPE);
		properties.push_back(GL_ARRAY_SIZE);
		std::vector<GLint> values(properties.size());
		for (GLuint i = 0; i < uniformCount; i++)
		{
			glGetProgramResourceiv(this->m_id, GL_UNIFORM, i, properties.size(), properties.data(), values.size(), NULL, values.data());

			nameData.resize(values[0]); // length of the name
			glGetProgramResourceName(this->m_id, GL_UNIFORM, i, nameData.size(), NULL, nameData.data());
			std::string name(nameData.data(), nameData.size() - 1);

			m_parameterAtlas.push_back(std::hash<std::string>()(name));

#ifdef _DEBUG
			std::cout << "\tName: " << name << "\n\tType: " << convertGLToString(values[1]) << "\n\tArray Size: " << values[2] << "\n\n";
#endif
		}
	}

	std::string convertGLToString(GLint value)
	{
		switch (value)
		{
		case GL_BOOL:
			return "GL_BOOL";
		case GL_INT:
			return "GL_INT";
		case GL_FLOAT:
			return "GL_FLOAT";
		case GL_FLOAT_VEC2:
			return "GL_FLOAT_VEC2";
		case GL_FLOAT_VEC3:
			return "GL_FLOAT_VEC3";
		case GL_FLOAT_VEC4:
			return "GL_FLOAT_VEC4";
		case GL_INT_VEC2:
			return "GL_INT_VEC2";
		case GL_INT_VEC3:
			return "GL_INT_VEC3";
		case GL_INT_VEC4:
			return "GL_INT_VEC4";
		case GL_FLOAT_MAT2:
			return "GL_FLOAT_MAT2";
		case GL_FLOAT_MAT3:
			return "GL_FLOAT_MAT3";
		case GL_FLOAT_MAT4:
			return "GL_FLOAT_MAT4";
		case GL_SAMPLER_2D:
			return "GL_SAMPLER_2D";
		case GL_SAMPLER_CUBE:
			return "GL_SAMPLER_CUBE";
		default:
			return std::to_string(value);
		}
	}

	GLuint m_id;

	std::string m_name;

	std::vector<size_t> m_parameterAtlas;
	std::vector<Texture> m_textures;

	bool nodeBased;
};