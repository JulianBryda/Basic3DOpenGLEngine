#pragma once
#include <algorithm>

#include "../../ThirdParty/ImNodes/imnodes.h"
#include "ShaderNodeEditor/ShaderVarNode.hpp"
#include "ShaderNodeEditor/ShaderUniformNode.hpp"
#include "ShaderNodeEditor/ShaderFunctionNode.hpp"

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
					static int test = 0;
					IntVarNode* node = new IntVarNode(nodes.size(), "Int", &test, ShaderVarNodeEnums::ShaderNodeCategory::Input);
					node->output = { GL_INT, "Value" };

					nodes.push_back(node);
				}
				if (ImGui::MenuItem("Float"))
				{
					static float test = 1;
					FloatVarNode* node = new FloatVarNode(nodes.size(), "Float", &test, ShaderVarNodeEnums::ShaderNodeCategory::Input);
					node->output = { GL_FLOAT, "Value" };

					nodes.push_back(node);
				}
				if (ImGui::MenuItem("Vec2"))
				{
					static glm::vec2 test = glm::vec2();
					ShaderVarNode<glm::vec2>* node = new Vec2VarNode(nodes.size(), "Vec2", &test, ShaderVarNodeEnums::ShaderNodeCategory::Input);
					node->output = { GL_FLOAT_VEC2, "Value" };

					nodes.push_back(node);
				}
				if (ImGui::MenuItem("Vec3"))
				{
					static glm::vec3 test = glm::vec3();
					Vec3VarNode* node = new Vec3VarNode(nodes.size(), "Vec3", &test, ShaderVarNodeEnums::ShaderNodeCategory::Input);
					node->output = { GL_FLOAT_VEC3, "Value" };

					nodes.push_back(node);
				}
				if (ImGui::MenuItem("Vec4"))
				{
					static glm::vec4 test = glm::vec4();
					Vec4VarNode* node = new Vec4VarNode(nodes.size(), "Vec4", &test, ShaderVarNodeEnums::ShaderNodeCategory::Input);
					node->output = { GL_FLOAT_VEC4, "Value" };

					nodes.push_back(node);
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Time"))
				{
					ShaderUniformNode<float>* node = new ShaderUniformNode<float>(nodes.size(), "Time", nullptr, ShaderVarNodeEnums::ShaderNodeCategory::Input, 1);
					node->output = { GL_FLOAT, "Time" };

					nodes.push_back(node);
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Output"))
			{
				if (ImGui::MenuItem("Output"))
				{
					ShaderVarNode<glm::vec4>* node = new Vec4VarNode(nodes.size(), "Output", nullptr, ShaderVarNodeEnums::ShaderNodeCategory::Output, ShaderVarPrefix::Out);
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
					std::vector<std::pair<GLint, std::string>> inputs;
					inputs.push_back({ GL_FLOAT_VEC3 | GL_FLOAT_VEC4, "Color" });
					ShaderFunctionNode<glm::vec3>* node = new ShaderFunctionNode<glm::vec3>(nodes.size(), "Mix Color", "mix", &inputs, ShaderVarNodeEnums::ShaderNodeCategory::Color, ShaderFunctionEnums::ShaderFunctionOperation::FunctionCall);

					nodes.push_back(node);
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Converter"))
			{
				if (ImGui::MenuItem("Add"))
				{
					std::vector<std::pair<GLint, std::string>> inputs;
					inputs.push_back({ GL_FLOAT_VEC4, "Value 1" });
					inputs.push_back({ GL_FLOAT_VEC4, "Value 2" });
					ShaderFunctionNode<glm::vec4>* node = new ShaderFunctionNode<glm::vec4>(nodes.size(), "Add", "+", &inputs, ShaderVarNodeEnums::ShaderNodeCategory::Color, ShaderFunctionEnums::ShaderFunctionOperation::Operation);
					node->output = { GL_FLOAT_VEC4, "Result" };

					nodes.push_back(node);
				}

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
				if (shaderNode->getType() == ShaderVarNodeEnums::ShaderVarNodeType::Var)
				{
					vars += shaderNode->getShaderCode();
				}
				else if (shaderNode->getType() == ShaderVarNodeEnums::ShaderVarNodeType::Uniform)
				{
					uniforms += shaderNode->getShaderCode();
				}
				else if (shaderNode->getType() == ShaderVarNodeEnums::ShaderVarNodeType::Function)
				{
					functions += shaderNode->getShaderCode();
				}
				}, node);
		}

		for (int i = 0; i < links.size(); i++)
		{
			auto& link = links[i];
			auto nodeFirst = findNodeByAttribId(link.first);
			auto nodeSecond = findNodeByAttribId(link.second);

			std::visit([&](auto& valueFirst, auto& valueSecond) {

				mainBody += std::format("{}{} = {}{};", valueSecond->getTypeName(), valueSecond->getId(), valueFirst->getTypeName(), valueFirst->getId());

				}, nodeFirst, nodeSecond);
		}

		std::string shaderCode = std::string("out vec4 FragColor;\n\n")
			+ uniforms
			+ "\n"
			+ vars
			+ "\n"
			+ functions
			+ "\n"
			+ "void main()\n"
			+ "{\n"
			+ mainBody
			+ "\n"
			+ "}";

		Material* material = new Material("test-material", new Shader(".\\Vertex\\v_debug.glsl", GL_VERTEX_SHADER), new Shader(shaderCode, GL_FRAGMENT_SHADER, false));
		MaterialLib::addMaterial(material);
	}

	using ShaderNode = std::variant<ShaderVarNode<int>*, ShaderVarNode<float>*, ShaderVarNode<glm::vec2>*, ShaderVarNode<glm::vec3>*, ShaderVarNode<glm::vec4>*, ShaderVarNode<glm::mat4>*>;

	ShaderNode findNodeByAttribId(int attribId)
	{
		ShaderNode shaderNode;

		for (auto& node : nodes)
		{
			std::visit([&](auto& value)
				{
					int nodeId = attribId / 500;
					if (nodeId == value->getId())
					{
						shaderNode = value;
					}

				}, node);
		}

		return shaderNode;
	}

	std::vector<ShaderNode> nodes;
	std::vector<std::pair<int, int>> links;	// std::pair<startId, endId>

};