#pragma once
#include <algorithm>

#include "../../ThirdParty/ImNodes/imnodes.h"
#include "../UI/ShaderNodeEditor/ShaderVarNode.hpp"
#include "../UI/ShaderNodeEditor/ShaderUniformNode.hpp"
#include "../UI/ShaderNodeEditor/ShaderFunctionNode.hpp"

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
		ImGui::Begin("Node Editor", nullptr, ImGuiWindowFlags_MenuBar);
		{
			ImGui::BeginMenuBar();
			{
				if (ImGui::Button("Compile"))
				{
					compileNodeTree();
				}
			}
			ImGui::EndMenuBar();

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
			std::visit([](auto& shaderNode) {
				shaderNode->render();
				}, node);
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
					static int test = 235;
					ShaderVarNode<int>* node = new ShaderVarNode<int>(nodes.size(), "Int", &test, ShaderNodeCategory::Input);
					node->output = { GL_INT, "Value" };

					nodes.push_back(node);
				}
				if (ImGui::MenuItem("Float"))
				{
					ShaderVarNode<float>* node = new ShaderVarNode<float>(nodes.size(), "Float", nullptr, ShaderNodeCategory::Input);
					node->output = { GL_FLOAT, "Value" };

					nodes.push_back(node);
				}
				if (ImGui::MenuItem("Vec2"))
				{
					ShaderVarNode<glm::vec2>* node = new ShaderVarNode<glm::vec2>(nodes.size(), "Vec2", nullptr, ShaderNodeCategory::Input);
					node->output = { GL_FLOAT_VEC2, "Value" };

					nodes.push_back(node);
				}
				if (ImGui::MenuItem("Vec3"))
				{
					ShaderVarNode<glm::vec3>* node = new ShaderVarNode<glm::vec3>(nodes.size(), "Vec3", nullptr, ShaderNodeCategory::Input);
					node->output = { GL_FLOAT_VEC3, "Value" };

					nodes.push_back(node);
				}
				if (ImGui::MenuItem("Vec4"))
				{
					ShaderVarNode<glm::vec4>* node = new ShaderVarNode<glm::vec4>(nodes.size(), "Vec4", nullptr, ShaderNodeCategory::Input);
					node->output = { GL_FLOAT_VEC4, "Value" };

					nodes.push_back(node);
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Time"))
				{
					ShaderUniformNode<float>* node = new ShaderUniformNode<float>(nodes.size(), "Time", ShaderNodeCategory::Input, 1);
					node->inputs.push_back({ GL_FLOAT, "Value" });

					nodes.push_back(node);
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Output"))
			{
				if (ImGui::MenuItem("Output"))
				{
					ShaderVarNode<int>* node = new ShaderVarNode<int>(nodes.size(), "Output", nullptr, ShaderNodeCategory::Output);
					node->inputs.push_back({ 0, "Output" });

					nodes.push_back(node);
				}

				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::BeginMenu("Color"))
			{
				if (ImGui::MenuItem("Mix Color"))
				{
					ShaderFunctionNode<glm::vec3>* node = new ShaderFunctionNode<glm::vec3>(nodes.size(), "Mix Color", "mix", ShaderNodeCategory::Color);
					node->inputs.push_back({ GL_FLOAT_VEC3 | GL_FLOAT_VEC4, "Color" });

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

	void compileNodeTree()
	{
		std::string uniforms;
		std::string functions;
		std::string vars;
		std::string mainBody;

		for (auto& node : nodes)
		{
			std::visit([&](auto& shaderNode) {
				if (shaderNode->getType() == ShaderVarNodeType::Var)
				{
					vars += shaderNode->getShaderCode();
				}
				else if (shaderNode->getType() == ShaderVarNodeType::Uniform)
				{
					uniforms += shaderNode->getShaderCode();
				}
				else if (shaderNode->getType() == ShaderVarNodeType::Function)
				{
					functions += shaderNode->getShaderCode();
				}
				}, node);
		}

		auto test = 0;
	}

	using ShaderNode = std::variant<ShaderVarNode<int>*, ShaderVarNode<float>*, ShaderVarNode<glm::vec2>*, ShaderVarNode<glm::vec3>*, ShaderVarNode<glm::vec4>*, ShaderVarNode<glm::mat4>*>;

	std::vector<ShaderNode> nodes;
	std::vector<std::pair<int, int>> links;

};