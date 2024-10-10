#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <variant>
#include <functional>
#include <optional>
#include <GLFW/glfw3.h>
#include <format>
#include "../../../ThirdParty/ImNodes/imnodes.h"
#include "../../Data/ShaderNode/ShaderVar.hpp"

struct ShaderNodeAttribute
{
	ShaderNodeAttribute(GLint type, const char* name)
	{
		this->id = 0;
		this->type = type;
		this->name = name;
		this->immutable = type != 0;
	}

	int id;
	GLint type;
	const char* name;
	bool immutable;
};

class ShaderVarNode
{
public:

	enum ShaderNodeCategory
	{
		Input,
		Output,
		Color,
		Converter,
		Shader,
		Texture
	};

	enum ShaderVarNodeType
	{
		Var,
		Function,
		Uniform
	};

	ShaderVarNode(int id, std::string name, void* value, GLint outputType, ShaderNodeCategory category, ShaderVar::ShaderVarPrefix prefix = ShaderVar::None)
	{
		this->id = id;
		this->name = name;
		this->category = category;
		this->type = ShaderVarNodeType::Var;

		this->shaderVar = new ShaderVar(id, value, outputType, prefix);
	}

	~ShaderVarNode()
	{
		delete shaderVar;
		delete output;
	}

	void render()
	{
		ImNodes::PushColorStyle(ImNodesCol_TitleBar, getNodeColor(category));
		ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, getNodeColor(category) - IM_COL32(20, 20, 20, 0));
		ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, getNodeColor(category) - IM_COL32(20, 20, 20, 0));
		if (ImNodes::IsNodeSelected(id)) ImNodes::PushColorStyle(ImNodesCol_NodeOutline, IM_COL32(150, 150, 150, 255));

		ImNodes::BeginNode(id);
		{
			ImNodes::BeginNodeTitleBar();
			{
				ImGui::TextUnformatted(name.c_str());
			}
			ImNodes::EndNodeTitleBar();

			if (shaderVar && shaderVar->value)
			{
				renderBody();
			}

			for (auto& input : inputs)
			{
				ImNodes::BeginInputAttribute(input.id);
				{
					ImGui::TextUnformatted(input.name);
				}
				ImNodes::EndInputAttribute();
			}

			if (output != nullptr)
			{
				ImNodes::BeginOutputAttribute(output->id);
				{
					ImGui::TextUnformatted(output->name);
				}
				ImNodes::EndOutputAttribute();
			}
		}
		ImNodes::EndNode();

		ImNodes::PopColorStyle();
		ImNodes::PopColorStyle();
		ImNodes::PopColorStyle();
		if (ImNodes::IsNodeSelected(id)) ImNodes::PopColorStyle();
	}

	void renderBody()
	{
		switch (shaderVar->outputType)
		{
		case GL_INT:
			renderIntBody();
			break;
		case GL_FLOAT:
			renderFloatBody();
			break;
		case GL_FLOAT_VEC2:
			renderVec2Body();
			break;
		case GL_FLOAT_VEC3:
			renderVec3Body();
			break;
		case GL_FLOAT_VEC4:
			renderVec4Body();
			break;
		default:
			throw std::runtime_error("No body to render!");
		}
	}

	void renderIntBody()
	{
		ImGui::SetNextItemWidth(80.f);
		ImGui::DragInt(std::format("##{}{}", name, id).c_str(), static_cast<int*>(shaderVar->value));
	}

	void renderFloatBody()
	{
		ImGui::SetNextItemWidth(80.f);
		ImGui::DragFloat(std::format("##{}{}", name, id).c_str(), static_cast<float*>(shaderVar->value), 0.1f);
	}

	void renderVec2Body()
	{
		ImGui::SetNextItemWidth(80.f);
		ImGui::DragFloat(std::format("##{}{}", name, id).c_str(), &static_cast<glm::vec2*>(shaderVar->value)->x, 0.1f);
		ImGui::SetNextItemWidth(80.f);
		ImGui::DragFloat(std::format("##{}{}", name, id + 1).c_str(), &static_cast<glm::vec2*>(shaderVar->value)->y, 0.1f);
	}

	void renderVec3Body()
	{
		ImGui::SetNextItemWidth(80.f);
		ImGui::DragFloat(std::format("##{}{}", name, id).c_str(), &static_cast<glm::vec3*>(shaderVar->value)->x, 0.1f);
		ImGui::SetNextItemWidth(80.f);
		ImGui::DragFloat(std::format("##{}{}", name, id + 1).c_str(), &static_cast<glm::vec3*>(shaderVar->value)->y, 0.1f);
		ImGui::SetNextItemWidth(80.f);
		ImGui::DragFloat(std::format("##{}{}", name, id + 2).c_str(), &static_cast<glm::vec3*>(shaderVar->value)->z, 0.1f);
	}

	void renderVec4Body()
	{
		ImGui::SetNextItemWidth(80.f);
		ImGui::DragFloat(std::format("##{}{}", name, id).c_str(), &static_cast<glm::vec4*>(shaderVar->value)->x, 0.1f);
		ImGui::SetNextItemWidth(80.f);
		ImGui::DragFloat(std::format("##{}{}", name, id + 1).c_str(), &static_cast<glm::vec4*>(shaderVar->value)->y, 0.1f);
		ImGui::SetNextItemWidth(80.f);
		ImGui::DragFloat(std::format("##{}{}", name, id + 2).c_str(), &static_cast<glm::vec4*>(shaderVar->value)->z, 0.1f);
		ImGui::SetNextItemWidth(80.f);
		ImGui::DragFloat(std::format("##{}{}", name, id + 3).c_str(), &static_cast<glm::vec4*>(shaderVar->value)->a, 0.1f);
	}

	int getId() const
	{
		return id;
	}

	std::string getName() const
	{
		return name;
	}

	ShaderNodeCategory getCategory() const
	{
		return category;
	}

	ShaderVarNodeType getType() const
	{
		return type;
	}

	ShaderNodeAttribute* getAttributeById(int id)
	{
		if (output && output->id == id)
		{
			return output;
		}

		for (auto& input : inputs)
		{
			if (input.id == id)
			{
				return &input;
			}
		}

		return nullptr;
	}

	std::string getShaderCode(std::vector<std::string>* inputNames = nullptr)
	{
		return this->shaderVar->getShaderCode(inputNames);
	}

	std::string getTypeName()
	{
		switch (type)
		{
		case ShaderVarNodeType::Var:
			return "var";
		case ShaderVarNodeType::Uniform:
			return "uniform";
		case ShaderVarNodeType::Function:
			return "func";
		default:
			return "Unknown";
		}
	}

	void addInput(ShaderNodeAttribute attribute)
	{
		attribute.id = id * 500 + inputs.size() + 2;
		inputs.push_back(attribute);
	}

	void setOutput(ShaderNodeAttribute* attribute)
	{
		assert(attribute != nullptr);

		output = attribute;
		output->id = id * 500 + 1;
	}

	ShaderNodeAttribute* getOutput() const
	{
		return output;
	}

	std::vector<ShaderNodeAttribute>& getInputs()
	{
		return inputs;
	}

	int id;
	std::string name;
	ShaderNodeCategory category;

	ShaderVar* shaderVar;

protected:

	ShaderVarNode(int id, std::string name, ShaderNodeCategory category)
	{
		this->id = id;
		this->name = name;
		this->category = category;
	}

	ShaderVarNodeType type;

	std::vector<ShaderNodeAttribute> inputs;
	ShaderNodeAttribute* output;

private:

	unsigned int getNodeColor(ShaderNodeCategory category) const
	{
		switch (category)
		{
		case ShaderNodeCategory::Input:
			return IM_COL32(80, 80, 80, 255);
		case ShaderNodeCategory::Output:
			return IM_COL32(100, 30, 30, 255);
		case ShaderNodeCategory::Color:
			return IM_COL32(110, 110, 30, 255);
		case ShaderNodeCategory::Converter:
			return IM_COL32(35, 100, 130, 255);
		case ShaderNodeCategory::Shader:
			return IM_COL32(40, 100, 40, 255);
		case ShaderNodeCategory::Texture:
			return IM_COL32(120, 70, 30, 255);
		default:
			return IM_COL32(0, 0, 0, 255);
		}
	}
};