#pragma once
#include <algorithm>

#include "../Data/Node.hpp"

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
				renderNodePopup();

				renderNodes();
				renderLinks();
			}
			ImNodes::EndNodeEditor();
		}
		ImGui::End();

		static int startAttribute, endAttribute;
		if (ImNodes::IsLinkCreated(&startAttribute, &endAttribute))
		{
			if (checkLink(endAttribute))
			{
				links.push_back({ startAttribute, endAttribute });
			}
		}

		if (ImGui::IsKeyPressed(ImGuiKey_Delete))
		{
			int numNodes = ImNodes::NumSelectedNodes();
			int numLinks = ImNodes::NumSelectedLinks();
			int* nodeIds = new int[numNodes];
			int* linkIds = new int[numLinks];

			ImNodes::GetSelectedNodes(nodeIds);
			ImNodes::GetSelectedLinks(linkIds);

			for (int i = numNodes - 1; i >= 0; i--)
			{
				int id = nodeIds[i];
				nodes.erase(nodes.begin() + id);
			}
			for (int i = numLinks - 1; i >= 0; i--)
			{
				int id = linkIds[i];
				links.erase(links.begin() + id);
			}

			ImNodes::ClearNodeSelection();
			ImNodes::ClearLinkSelection();

			delete[] nodeIds;
			delete[] linkIds;
		}
	}


private:

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
		for (auto& node : nodes)
		{
			node.render();
		}
	}

	void renderNodePopup()
	{
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
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
				if (ImGui::MenuItem("Int"))
				{
					Node node(nodes.size(), "Int", Node::NodeCategory::Input, Node::NodeType::Int);
					node.output = { GL_INT, "Value" };
					node.value = 0;
					node.body = [](Node* n) {
						if (int* val = std::get_if<int>(&n->value))
						{
							ImGui::SetNextItemWidth(80.f);
							ImGui::DragInt(std::format("##0{}{}", n->name, n->id).c_str(), val);
						}
						};

					nodes.push_back(node);
				}
				if (ImGui::MenuItem("Float"))
				{
					Node node(nodes.size(), "Float", Node::NodeCategory::Input, Node::NodeType::Float);
					node.output = { GL_FLOAT, "Value" };
					node.value = 0.f;
					node.body = [](Node* n) {
						if (float* val = std::get_if<float>(&n->value))
						{
							ImGui::SetNextItemWidth(80.f);
							ImGui::DragFloat(std::format("##0{}{}", n->name, n->id).c_str(), val, 0.1f);
						}
						};

					nodes.push_back(node);
				}
				if (ImGui::MenuItem("Vec2"))
				{
					Node node(nodes.size(), "Vec2", Node::NodeCategory::Input, Node::NodeType::Vec2);
					node.output = { GL_FLOAT_VEC2, "Value" };
					node.value = glm::vec2();
					node.body = [](Node* n) {
						if (glm::vec2* val = std::get_if<glm::vec2>(&n->value))
						{
							ImGui::SetNextItemWidth(80.f);
							ImGui::DragFloat(std::format("##0{}{}", n->name, n->id).c_str(), &val->x, 0.1f);
							ImGui::SetNextItemWidth(80.f);
							ImGui::DragFloat(std::format("##1{}{}", n->name, n->id).c_str(), &val->y, 0.1f);
						}
						};

					nodes.push_back(node);
				}
				if (ImGui::MenuItem("Vec3"))
				{
					Node node(nodes.size(), "Vec3", Node::NodeCategory::Input, Node::NodeType::Vec3);
					node.output = { GL_FLOAT_VEC3, "Value" };
					node.value = glm::vec3();
					node.body = [](Node* n) {
						if (glm::vec3* val = std::get_if<glm::vec3>(&n->value))
						{
							ImGui::SetNextItemWidth(80.f);
							ImGui::DragFloat(std::format("##0{}{}", n->name, n->id).c_str(), &val->x, 0.1f);
							ImGui::SetNextItemWidth(80.f);
							ImGui::DragFloat(std::format("##1{}{}", n->name, n->id).c_str(), &val->y, 0.1f);
							ImGui::SetNextItemWidth(80.f);
							ImGui::DragFloat(std::format("##2{}{}", n->name, n->id).c_str(), &val->z, 0.1f);
						}
						};

					nodes.push_back(node);
				}
				if (ImGui::MenuItem("Vec4"))
				{
					Node node(nodes.size(), "Vec4", Node::NodeCategory::Input, Node::NodeType::Vec4);
					node.output = { GL_FLOAT_VEC4, "Value" };
					node.value = glm::vec4();
					node.body = [](Node* n) {
						if (glm::vec4* val = std::get_if<glm::vec4>(&n->value))
						{
							ImGui::SetNextItemWidth(80.f);
							ImGui::DragFloat(std::format("##0{}{}", n->name, n->id).c_str(), &val->x, 0.1f);
							ImGui::SetNextItemWidth(80.f);
							ImGui::DragFloat(std::format("##1{}{}", n->name, n->id).c_str(), &val->y, 0.1f);
							ImGui::SetNextItemWidth(80.f);
							ImGui::DragFloat(std::format("##2{}{}", n->name, n->id).c_str(), &val->z, 0.1f);
							ImGui::SetNextItemWidth(80.f);
							ImGui::DragFloat(std::format("##3{}{}", n->name, n->id).c_str(), &val->a, 0.1f);
						}
						};

					nodes.push_back(node);
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Output"))
			{
				if (ImGui::MenuItem("Material Output"))
				{
					Node node(nodes.size(), "Material Output", Node::NodeCategory::Output, Node::NodeType::MaterialOutput);
					node.inputs.push_back({ 0, "Output" });

					nodes.push_back(node);
				}

				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::BeginMenu("Color"))
			{
				if (ImGui::MenuItem("Mix Color"))
				{
					Node node(nodes.size(), "Mix Color", Node::NodeCategory::Color, Node::NodeType::MixColor);
					node.inputs.push_back({ GL_FLOAT_VEC3 | GL_FLOAT_VEC4, "Color" });

					nodes.push_back(node);
				}

				if (ImGui::MenuItem("Invert Color"))
				{
					Node node(nodes.size(), "Invert Color", Node::NodeCategory::Color, Node::NodeType::InvertColor);
					node.inputs.push_back({ GL_FLOAT_VEC3 | GL_FLOAT_VEC4, "Color" });

					nodes.push_back(node);
				}

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

	bool checkLink(int endAttribute)
	{
		for (auto& link : links)
		{
			if (link.second == endAttribute) return false;
		}

		return true;
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