#pragma once
#include "imgui.h"
#include "../../ThirdParty/ImNodes/imnodes.h"

class ShaderNodeEditor
{

public:

	ShaderNodeEditor()
	{
		ImNodes::CreateContext();

		ImNodesStyle& style = ImNodes::GetStyle();
		style.Colors[ImNodesCol_NodeBackgroundHovered] = style.Colors[ImNodesCol_NodeBackground];
		style.Colors[ImNodesCol_NodeBackgroundSelected] = style.Colors[ImNodesCol_NodeBackground];
	}

	~ShaderNodeEditor()
	{
		ImNodes::DestroyContext();
	}

	void render()
	{
		ImGui::Begin("Node Editor", nullptr);
		{
			ImNodes::BeginNodeEditor();
			{
				addNodePopup();

				renderNodes();
				renderLinks();
			}
			ImNodes::EndNodeEditor();
		}
		ImGui::End();
	}


private:

	struct Node
	{
		Node(const char* name, unsigned int color)
		{
			this->name = name;
			this->color = color;
		}

		const char* name;
		unsigned int color;

		std::vector<std::pair<GLint, const char*>> inputs;
		std::optional<std::pair<GLint, const char*>> output;

	};

	void renderLinks()
	{
		for (int i = 0; i < links.size(); i++)
		{
			auto& link = links[i];

			ImNodes::Link(i, link.first, link.second);
		}
	}

	void renderNodes()
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			auto& node = nodes[i];

			renderSingleNode(node, i);
		}
	}

	void renderSingleNode(Node& node, int index)
	{
		ImNodes::PushColorStyle(ImNodesCol_TitleBar, node.color);
		ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, node.color - IM_COL32(20, 20, 20, 0));
		ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, node.color - IM_COL32(20, 20, 20, 0));
		if (ImNodes::IsNodeSelected(index)) ImNodes::PushColorStyle(ImNodesCol_NodeOutline, IM_COL32(150, 150, 150, 255));

		ImNodes::BeginNode(index);
		{
			ImNodes::BeginNodeTitleBar();
			{
				ImGui::TextUnformatted(node.name);
			}
			ImNodes::EndNodeTitleBar();

			for (int i = 0; i < node.inputs.size(); i++)
			{
				auto& input = node.inputs[i];
				ImNodes::BeginInputAttribute(i + index * 50);
				{
					ImGui::TextUnformatted(input.second);
				}
				ImNodes::EndInputAttribute();
			}

			if (node.output.has_value())
			{
				ImNodes::BeginOutputAttribute(0);
				{
					ImGui::TextUnformatted(node.output.value().second);
				}
				ImNodes::EndOutputAttribute();
			}
		}
		ImNodes::EndNode();

		ImNodes::PopColorStyle();
		ImNodes::PopColorStyle();
		ImNodes::PopColorStyle();
		if (ImNodes::IsNodeSelected(index)) ImNodes::PopColorStyle();
	}

	void addNodePopup()
	{
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup("AddNodePopup");
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.f, 10.f));
		ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(0, 0, 0, 100));
		ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(50, 50, 50, 100));
		if (ImGui::BeginPopup("AddNodePopup"))
		{
			ColoredText("Add", IM_COL32(130, 130, 130, 255));

			ImGui::Separator();

			if (ImGui::BeginMenu("Input"))
			{

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Output"))
			{
				if (ImGui::MenuItem("Material Output"))
				{
					Node node("Material Output", IM_COL32(100, 30, 30, 255));
					node.inputs.push_back({ 0, "Output" });

					nodes.push_back(node);
				}

				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::BeginMenu("Color"))
			{

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Converter"))
			{

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Shader"))
			{

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Texture"))
			{

				ImGui::EndMenu();
			}


			ImGui::EndPopup(); // End the popup
		}
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}

	void ColoredText(const char* text, ImU32 color)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, color);
		ImGui::Text(text);
		ImGui::PopStyleColor();
	}

	std::vector<Node> nodes;
	std::vector<std::pair<int, int>> links;
};