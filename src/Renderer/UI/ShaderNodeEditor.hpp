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
			if (checkLink(startAttribute, endAttribute))
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
				deleteNodeById(id);
			}
			for (int i = numLinks - 1; i >= 0; i--)
			{
				int id = linkIds[i];
				deleteLinkById(id);
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
		for (auto& node : uniformNodes)
		{
			node->render();
		}

		for (auto& node : varNodes)
		{
			node->render();
		}

		for (auto& node : functionNodes)
		{
			node->render();
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
					ShaderVarNode* node = new ShaderVarNode(getNextNodeId(), "Int", new int(), GL_INT, ShaderVarNode::ShaderNodeCategory::Input);
					node->setOutput(new ShaderNodeAttribute(GL_INT, "Value"));

					varNodes.push_back(node);
				}
				if (ImGui::MenuItem("Float"))
				{
					ShaderVarNode* node = new ShaderVarNode(getNextNodeId(), "Float", new float(), GL_FLOAT, ShaderVarNode::ShaderNodeCategory::Input);
					node->setOutput(new ShaderNodeAttribute(GL_FLOAT, "Value"));

					varNodes.push_back(node);
				}
				if (ImGui::MenuItem("Vec2"))
				{
					ShaderVarNode* node = new ShaderVarNode(getNextNodeId(), "Vec2", new glm::vec2(), GL_FLOAT_VEC2, ShaderVarNode::ShaderNodeCategory::Input);
					node->setOutput(new ShaderNodeAttribute(GL_FLOAT_VEC2, "Value"));

					varNodes.push_back(node);
				}
				if (ImGui::MenuItem("Vec3"))
				{
					ShaderVarNode* node = new ShaderVarNode(getNextNodeId(), "Vec3", new glm::vec3(), GL_FLOAT_VEC3, ShaderVarNode::ShaderNodeCategory::Input);
					node->setOutput(new ShaderNodeAttribute(GL_FLOAT_VEC3, "Value"));

					varNodes.push_back(node);
				}
				if (ImGui::MenuItem("Vec4"))
				{
					ShaderVarNode* node = new ShaderVarNode(getNextNodeId(), "Vec4", new glm::vec4(), GL_FLOAT_VEC4, ShaderVarNode::ShaderNodeCategory::Input);
					node->setOutput(new ShaderNodeAttribute(GL_FLOAT_VEC4, "Value"));

					varNodes.push_back(node);
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Time"))
				{
					ShaderUniformNode* node = new ShaderUniformNode(getNextNodeId(), "Time", GL_FLOAT, ShaderVarNode::ShaderNodeCategory::Input, 1);
					node->setOutput(new ShaderNodeAttribute(GL_FLOAT, "Time"));

					uniformNodes.push_back(node);
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Output"))
			{
				if (ImGui::MenuItem("Output"))
				{
					ShaderVarNode* node = new ShaderVarNode(getNextNodeId(), "Output", nullptr, GL_FLOAT_VEC4, ShaderVarNode::ShaderNodeCategory::Output, ShaderVar::Out);
					node->addInput({ GL_FLOAT_VEC4, "Output" });

					varNodes.push_back(node);
				}

				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::BeginMenu("Color"))
			{
				if (ImGui::MenuItem("Mix Color"))
				{
					ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Mix Color", "mix", ShaderFunctionNode::ShaderNodeCategory::Color, ShaderFunction::ShaderFunctionOperation::FunctionCall);
					node->addInput({ GL_FLOAT_VEC4, "Color" });

					functionNodes.push_back(node);
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Converter"))
			{
				if (ImGui::MenuItem("Add"))
				{
					ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Add", "+", ShaderFunctionNode::ShaderNodeCategory::Color, ShaderFunction::ShaderFunctionOperation::Operation);
					node->addInput({ 0, "Value 1" });
					node->addInput({ 0, "Value 2" });
					node->setOutput(new ShaderNodeAttribute(0, "Result"));

					functionNodes.push_back(node);
				}

				if (ImGui::MenuItem("Subtract"))
				{
					ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Subtract", "-", ShaderFunctionNode::ShaderNodeCategory::Color, ShaderFunction::ShaderFunctionOperation::Operation);
					node->addInput({ 0, "Value 1" });
					node->addInput({ 0, "Value 2" });
					node->setOutput(new ShaderNodeAttribute(0, "Result"));

					functionNodes.push_back(node);
				}

				if (ImGui::MenuItem("Multiply"))
				{
					ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Multiply", "*", ShaderFunctionNode::ShaderNodeCategory::Color, ShaderFunction::ShaderFunctionOperation::Operation);
					node->addInput({ 0, "Value 1" });
					node->addInput({ 0, "Value 2" });
					node->setOutput(new ShaderNodeAttribute(0, "Result"));

					functionNodes.push_back(node);
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

	bool checkLink(int startAttributeId, int endAttributeId)
	{
		for (auto& link : links)
		{
			if (link.second == endAttributeId) return false;
		}

		auto startNode = getNodeByAttribId(startAttributeId);
		auto endNode = getNodeByAttribId(endAttributeId);

		auto startAttribute = startNode->getAttributeById(startAttributeId);
		auto endAttribute = endNode->getAttributeById(endAttributeId);

		bool result = startAttribute && endAttribute && (startAttribute->type == endAttribute->type || endAttribute->type == 0);

		if (result && endNode->getType() == ShaderVarNode::ShaderVarNodeType::Function)
		{
			static_cast<ShaderFunctionNode*>(endNode)->setFunctionType(startAttribute->type);
		}

		return result;
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

		for (auto& node : uniformNodes)
		{
			uniforms += node->getShaderCode();
		}

		for (auto& node : varNodes)
		{
			vars += node->getShaderCode();
		}

		for (auto& node : functionNodes)
		{
			std::vector<std::string> inputNames = getOutputVariableNames(node->getId());
			functions += node->getShaderCode(&inputNames);
		}


		for (int i = 0; i < links.size(); i++)
		{
			auto& link = links[i];
			auto nodeFirst = getNodeByAttribId(link.first);
			auto nodeSecond = getNodeByAttribId(link.second);

			if (nodeSecond->getCategory() == ShaderVarNode::ShaderNodeCategory::Output && nodeSecond->getName() == "Output")
			{
				mainBody += std::format("{}{} = {}{};", nodeSecond->getTypeName(), nodeSecond->getId(), nodeFirst->getTypeName(), nodeFirst->getId());
				i = links.size();
			}
		}

		std::string shaderCode = std::string("#version 460 core\n")
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

		std::cout << shaderCode << std::endl;

		Material* material = new Material("test-material", new Shader(".\\Vertex\\v_debug.glsl", GL_VERTEX_SHADER), new Shader(shaderCode, GL_FRAGMENT_SHADER, false));
		MaterialLib::addMaterial(material);
	}

	ShaderVarNode* getNodeByAttribId(int attribId)
	{
		for (auto& node : uniformNodes)
		{
			if (attribId / 500 == node->getId())
			{
				return node;
			}
		}

		for (auto& node : varNodes)
		{
			if (attribId / 500 == node->getId())
			{
				return node;
			}
		}

		for (auto& node : functionNodes)
		{
			if (attribId / 500 == node->getId())
			{
				return node;
			}
		}

		return nullptr;
	}

	ShaderVarNode* getNodeById(int nodeId)
	{
		for (auto& node : uniformNodes)
		{
			if (nodeId == node->getId())
			{
				return node;
			}
		}

		for (auto& node : varNodes)
		{
			if (nodeId == node->getId())
			{
				return node;
			}
		}

		for (auto& node : functionNodes)
		{
			if (nodeId == node->getId())
			{
				return node;
			}
		}

		return nullptr;
	}

	std::vector<std::string> getOutputVariableNames(int nodeId)
	{
		std::vector<std::string> varNames;

		for (auto& link : links)
		{
			if (link.second / 500 != nodeId) continue;

			ShaderVarNode* node = getNodeByAttribId(link.first);

			varNames.push_back(std::format("{}{}", node->getTypeName(), node->getId()));
		}

		return varNames;
	}

	void deleteNodeById(int nodeId)
	{
		for (int i = 0; i < uniformNodes.size(); i++)
		{
			ShaderVarNode* node = uniformNodes[i];
			if (nodeId == node->getId())
			{
				uniformNodes.erase(uniformNodes.begin() + i);
				delete node;
				return;
			}
		}

		for (int i = 0; i < varNodes.size(); i++)
		{
			ShaderVarNode* node = varNodes[i];
			if (nodeId == node->getId())
			{
				varNodes.erase(varNodes.begin() + i);
				delete node;
				return;
			}
		}

		for (int i = 0; i < functionNodes.size(); i++)
		{
			ShaderVarNode* node = functionNodes[i];
			if (nodeId == node->getId())
			{
				functionNodes.erase(functionNodes.begin() + i);
				delete node;
				return;
			}
		}
	}

	void deleteLinkById(int id)
	{
		std::pair<int, int> link = links[id];
		links.erase(links.begin() + id);

		for (auto& val : links)
		{
			if (val.second / 500 == link.second / 500) // convert attrib id to node id by / 500
			{
				return;
			}
		}

		auto node = getNodeByAttribId(link.second);
		for (auto& input : node->getInputs())
		{
			input.type = 0;
		}
	}

	int getNextNodeId()
	{
		return uniformNodes.size() + functionNodes.size() + varNodes.size();
	}

	std::vector<ShaderUniformNode*> uniformNodes;
	std::vector<ShaderFunctionNode*> functionNodes;
	std::vector<ShaderVarNode*> varNodes;
	std::vector<std::pair<int, int>> links;	// std::pair<startId, endId>

};