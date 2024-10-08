#pragma once
#include <iostream>
#include <format>
#include <variant>
#include <glm/glm.hpp>

#include "../Renderer/Data/ShaderNode.hpp"
#include "../Renderer/Graphics/Shader.hpp"



namespace ShaderNodeCompiler
{

	std::string createVertexShader(ShaderNode node)
	{
		std::string base = std::string("#version 460 core\n")
			+ "layout(location = 0) in vec4 position;\n"
			+ "\n"
			+ "uniform mat4 projectionMatrix;\n"
			+ "uniform mat4 viewMatrix;\n"
			+ "uniform mat4 modelMatrix;\n"
			+ "\n";

		switch (node.type)
		{
		default:
			break;
		}

		base += std::string("void main()\n")
			+ "{\n"
			+ "gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;\n"
			+ "}";

		return base;
	}

	ShaderOLD createFragmentShader()
	{

	}

	ShaderOLD createGeometryShader()
	{

	}


	std::string getShaderCode(Node node)
	{
		switch (node.type)
		{
		case Node::Int:
			return std::format("{};", std::get<int>(node.value));
		case Node::Float:
			return std::format("{};", std::get<float>(node.value));
		case Node::Vec2:
			return std::format("vec2({},{});", std::get<glm::vec2>(node.value).x, std::get<glm::vec2>(node.value).y);
		case Node::Vec3:
			return std::format("vec3({},{},{});", std::get<glm::vec3>(node.value).x, std::get<glm::vec3>(node.value).y, std::get<glm::vec3>(node.value).z);
		case Node::Vec4:
			return std::format("vec4({},{},{},{});", std::get<glm::vec4>(node.value).x, std::get<glm::vec4>(node.value).y, std::get<glm::vec4>(node.value).z, std::get<glm::vec4>(node.value).a);
		case Node::MaterialOutput:
			break;
		case Node::MixColor:
			break;
		case Node::InvertColor:
			break;
		default:
			break;
		}
	}
}