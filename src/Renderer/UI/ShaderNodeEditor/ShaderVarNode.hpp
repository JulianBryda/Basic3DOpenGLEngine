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

template <typename T>
class ShaderVarNode
{
public:

	ShaderVarNode(int id, std::string name, T* value, ShaderNodeCategory category)
	{
		this->id = id;
		this->name = name;
		this->category = category;
		this->type = ShaderVarNodeType::Var;

		this->shaderVar = new ShaderVar(id, value);
	}

	~ShaderVarNode()
	{
		delete shaderVar;
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

			renderBody(shaderVar->value);

			for (int i = 0; i < inputs.size(); i++)
			{
				auto& input = inputs[i];
				ImNodes::BeginInputAttribute(i + 1 + id * 50);
				{
					ImGui::TextUnformatted(input.second.c_str());
				}
				ImNodes::EndInputAttribute();
			}

			if (output.has_value())
			{
				ImNodes::BeginOutputAttribute(id * 50);
				{
					ImGui::TextUnformatted(output.value().second.c_str());
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

	template <typename V>
	void renderBody(V* value)
	{
		throw std::runtime_error("Type unknown!");
	}

	// TODO EXTEN THIS FUCKING renderBody TO SUPPORT MORE TYPES!!

	template <>
	void renderBody(int* value)
	{
		ImGui::SetNextItemWidth(80.f);
		ImGui::DragInt(std::format("##{}", id * 50).c_str(), value);
	}

	std::string getShaderCode()
	{
		return this->shaderVar->getShaderCode();
	}

	ShaderVarNodeType getType()
	{
		return type;
	}

	int id;
	std::string name;
	ShaderNodeCategory category;

	std::vector<std::pair<GLint, std::string>> inputs;
	std::optional<std::pair<GLint, std::string>> output;

	ShaderVar<T>* shaderVar;

protected:

	ShaderVarNode(int id, std::string name, ShaderNodeCategory category)
	{
		this->id = id;
		this->name = name;
		this->category = category;
	}

	ShaderVarNodeType type;

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