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

namespace ShaderVarNodeEnums
{
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
}

template <typename T>
class ShaderVarNode
{
public:

	ShaderVarNode(int id, std::string name, T* value, ShaderVarNodeEnums::ShaderNodeCategory category, ShaderVarPrefix prefix = None)
	{
		this->id = id;
		this->name = name;
		this->category = category;
		this->type = ShaderVarNodeEnums::ShaderVarNodeType::Var;

		this->shaderVar = new ShaderVar(id, value, prefix);
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

			if (shaderVar && shaderVar->value)
			{
				renderBody(shaderVar->value);
			}

			for (int i = 0; i < inputs.size(); i++)
			{
				auto& input = inputs[i];
				ImNodes::BeginInputAttribute(id * 500 + i + 2);
				{
					ImGui::TextUnformatted(input.second.c_str());
				}
				ImNodes::EndInputAttribute();
			}

			if (output.has_value())
			{
				ImNodes::BeginOutputAttribute(id * 500 + 1);
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
		throw std::runtime_error("No render function for type!");
	}

	template<>
	void renderBody(int* value)
	{
		ImGui::SetNextItemWidth(80.f);
		ImGui::DragInt(std::format("##{}{}", name, id).c_str(), value);
	}

	template<>
	void renderBody(float* value)
	{
		ImGui::SetNextItemWidth(80.f);
		ImGui::DragFloat(std::format("##{}{}", name, id).c_str(), value, 0.1f);
	}

	template<>
	void renderBody(glm::vec2* value)
	{
		ImGui::SetNextItemWidth(80.f);
		ImGui::DragFloat(std::format("##{}{}", name, id).c_str(), &value->x, 0.1f);
		ImGui::SetNextItemWidth(80.f);
		ImGui::DragFloat(std::format("##{}{}", name, id + 1).c_str(), &value->y, 0.1f);
	}

	template<>
	void renderBody(glm::vec3* value)
	{
		ImGui::SetNextItemWidth(80.f);
		ImGui::DragFloat(std::format("##{}{}", name, id).c_str(), &value->x, 0.1f);
		ImGui::SetNextItemWidth(80.f);
		ImGui::DragFloat(std::format("##{}{}", name, id + 1).c_str(), &value->y, 0.1f);
		ImGui::SetNextItemWidth(80.f);
		ImGui::DragFloat(std::format("##{}{}", name, id + 2).c_str(), &value->z, 0.1f);
	}

	template<>
	void renderBody(glm::vec4* value)
	{
		ImGui::SetNextItemWidth(80.f);
		ImGui::DragFloat(std::format("##{}{}", name, id).c_str(), &value->x, 0.1f);
		ImGui::SetNextItemWidth(80.f);
		ImGui::DragFloat(std::format("##{}{}", name, id + 1).c_str(), &value->y, 0.1f);
		ImGui::SetNextItemWidth(80.f);
		ImGui::DragFloat(std::format("##{}{}", name, id + 2).c_str(), &value->z, 0.1f);
		ImGui::SetNextItemWidth(80.f);
		ImGui::DragFloat(std::format("##{}{}", name, id + 3).c_str(), &value->a, 0.1f);
	}

	int getId() { return id; }
	std::string getName() { return name; }
	std::vector<std::pair<GLint, std::string>>& getInputs() { return inputs; }

	std::string getShaderCode()
	{
		return this->shaderVar->getShaderCode();
	}

	ShaderVarNodeEnums::ShaderVarNodeType getType()
	{
		return type;
	}

	std::string getTypeName()
	{
		switch (type)
		{
		case ShaderVarNodeEnums::ShaderVarNodeType::Var:
			return "var";
		case ShaderVarNodeEnums::ShaderVarNodeType::Uniform:
			return "uniform";
		case ShaderVarNodeEnums::ShaderVarNodeType::Function:
			return "func";
		default:
			return "Unknown";
		}
	}

	int id;
	std::string name;
	ShaderVarNodeEnums::ShaderNodeCategory category;

	std::vector<std::pair<GLint, std::string>> inputs;
	std::optional<std::pair<GLint, std::string>> output;

	ShaderVar<T>* shaderVar;

protected:

	ShaderVarNode(int id, std::string name, std::vector<std::pair<GLint, std::string>>* inputs, ShaderVarNodeEnums::ShaderNodeCategory category)
	{
		this->id = id;
		this->name = name;
		this->category = category;

		if (inputs)
			this->inputs = *inputs;
	}

	ShaderVarNodeEnums::ShaderVarNodeType type;

private:

	unsigned int getNodeColor(ShaderVarNodeEnums::ShaderNodeCategory category) const
	{
		switch (category)
		{
		case ShaderVarNodeEnums::ShaderNodeCategory::Input:
			return IM_COL32(80, 80, 80, 255);
		case ShaderVarNodeEnums::ShaderNodeCategory::Output:
			return IM_COL32(100, 30, 30, 255);
		case ShaderVarNodeEnums::ShaderNodeCategory::Color:
			return IM_COL32(110, 110, 30, 255);
		case ShaderVarNodeEnums::ShaderNodeCategory::Converter:
			return IM_COL32(35, 100, 130, 255);
		case ShaderVarNodeEnums::ShaderNodeCategory::Shader:
			return IM_COL32(40, 100, 40, 255);
		case ShaderVarNodeEnums::ShaderNodeCategory::Texture:
			return IM_COL32(120, 70, 30, 255);
		default:
			return IM_COL32(0, 0, 0, 255);
		}
	}
};


typedef ShaderVarNode<int> IntVarNode;
typedef ShaderVarNode<float> FloatVarNode;
typedef ShaderVarNode<glm::vec2> Vec2VarNode;
typedef ShaderVarNode<glm::vec3> Vec3VarNode;
typedef ShaderVarNode<glm::vec4> Vec4VarNode;