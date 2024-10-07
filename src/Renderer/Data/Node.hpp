#pragma once
#include <functional>
#include "../../ThirdParty/ImNodes/imnodes.h"


class Node
{
public:

	enum NodeCategory
	{
		Input,
		Output,
		Color,
		Converter,
		Shader,
		Texture
	};

	enum NodeType
	{
		Int,
		Float,
		Vec2,
		Vec3,
		Vec4,
		MaterialOutput,
		MixColor,
		InvertColor,
		DiffuseBSDF
	};

	using NodeValue = std::variant<int, float, glm::vec2, glm::vec3, glm::vec4>;

	Node(int id, std::string name, NodeCategory category, NodeType type)
	{
		this->id = id;
		this->name = name;
		this->category = category;
		this->type = type;

		this->body = [](Node* node) {};
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

			body(this);

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

	int id;
	std::string name;
	NodeCategory category;
	NodeType type;
	NodeValue value;

	std::function<void(Node* node)> body;

	std::vector<std::pair<GLint, std::string>> inputs;
	std::optional<std::pair<GLint, std::string>> output;

private:

	unsigned int getNodeColor(NodeCategory category) const
	{
		switch (category)
		{
		case NodeCategory::Input:
			return IM_COL32(80, 80, 80, 255);
		case NodeCategory::Output:
			return IM_COL32(100, 30, 30, 255);
		case NodeCategory::Color:
			return IM_COL32(110, 110, 30, 255);
		case NodeCategory::Converter:
			return IM_COL32(35, 100, 130, 255);
		case NodeCategory::Shader:
			return IM_COL32(40, 100, 40, 255);
		case NodeCategory::Texture:
			return IM_COL32(120, 70, 30, 255);
		default:
			return IM_COL32(0, 0, 0, 255);
		}
	}
};