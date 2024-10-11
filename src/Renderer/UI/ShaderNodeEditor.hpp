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
			createLink(startAttribute, endAttribute);
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

			ImNodes::Link(i, link.first->id, link.second->id);
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
					int* value = new int(0);
					ShaderVarNode* node = new ShaderVarNode(getNextNodeId(), "Int", value, GL_INT, ShaderVarNode::ShaderNodeCategory::Input);
					node->addInput(ShaderNodeAttribute(GL_INT, "", value));
					node->setOutput(new ShaderNodeAttribute(GL_INT, "Value", value));

					varNodes.push_back(node);
				}
				if (ImGui::MenuItem("Float"))
				{
					float* value = new float(0);
					ShaderVarNode* node = new ShaderVarNode(getNextNodeId(), "Float", value, GL_FLOAT, ShaderVarNode::ShaderNodeCategory::Input);
					node->addInput(ShaderNodeAttribute(GL_FLOAT, "", value));
					node->setOutput(new ShaderNodeAttribute(GL_FLOAT, "Value", value));

					varNodes.push_back(node);
				}
				if (ImGui::MenuItem("Vec2"))
				{
					glm::vec2* value = new glm::vec2();
					ShaderVarNode* node = new ShaderVarNode(getNextNodeId(), "Vec2", value, GL_FLOAT_VEC2, ShaderVarNode::ShaderNodeCategory::Input);
					node->addInput(ShaderNodeAttribute(GL_FLOAT, "X", &value->x));
					node->addInput(ShaderNodeAttribute(GL_FLOAT, "Y", &value->y));
					node->setOutput(new ShaderNodeAttribute(GL_FLOAT_VEC2, "Value", value));

					varNodes.push_back(node);
				}
				if (ImGui::MenuItem("Vec3"))
				{
					glm::vec3* value = new glm::vec3();
					ShaderVarNode* node = new ShaderVarNode(getNextNodeId(), "Vec3", value, GL_FLOAT_VEC3, ShaderVarNode::ShaderNodeCategory::Input);
					node->addInput(ShaderNodeAttribute(GL_FLOAT, "X", &value->x));
					node->addInput(ShaderNodeAttribute(GL_FLOAT, "Y", &value->y));
					node->addInput(ShaderNodeAttribute(GL_FLOAT, "Z", &value->z));
					node->setOutput(new ShaderNodeAttribute(GL_FLOAT_VEC3, "Value", value));

					varNodes.push_back(node);
				}
				if (ImGui::MenuItem("Vec4"))
				{
					glm::vec4* value = new glm::vec4();
					ShaderVarNode* node = new ShaderVarNode(getNextNodeId(), "Vec4", value, GL_FLOAT_VEC4, ShaderVarNode::ShaderNodeCategory::Input);
					node->addInput(ShaderNodeAttribute(GL_FLOAT, "X", &value->x));
					node->addInput(ShaderNodeAttribute(GL_FLOAT, "Y", &value->y));
					node->addInput(ShaderNodeAttribute(GL_FLOAT, "Z", &value->z));
					node->addInput(ShaderNodeAttribute(GL_FLOAT, "A", &value->a));
					node->setOutput(new ShaderNodeAttribute(GL_FLOAT_VEC4, "Value", value));

					varNodes.push_back(node);
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Time"))
				{
					ShaderUniformNode* node = new ShaderUniformNode(getNextNodeId(), "Time", GL_FLOAT, ShaderVarNode::ShaderNodeCategory::Input, 1);
					node->setOutput(new ShaderNodeAttribute(GL_FLOAT, "Time", nullptr));

					uniformNodes.push_back(node);
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Output"))
			{
				if (ImGui::MenuItem("Output"))
				{

					ShaderVarNode* node = new ShaderVarNode(getNextNodeId(), "Output", nullptr, GL_FLOAT_VEC4, ShaderVarNode::ShaderNodeCategory::Output, ShaderVar::Out);
					node->addInput({ GL_FLOAT_VEC4, "Output", nullptr });

					varNodes.push_back(node);
				}

				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::BeginMenu("Color"))
			{
				//if (ImGui::MenuItem("Mix Color"))
				//{
				//	ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Mix Color", "mix", ShaderFunctionNode::ShaderNodeCategory::Color, ShaderFunction::ShaderFunctionOperation::FunctionCall);
				//	node->addInput({ GL_FLOAT_VEC4, "Color" });

				//	functionNodes.push_back(node);
				//}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Math"))
			{
				if (ImGui::MenuItem("Add"))
				{
					ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Add", "+", ShaderFunctionNode::ShaderNodeCategory::Color, ShaderFunction::ShaderFunctionOperation::Operation);
					node->addInput({ 0, "Value 1", nullptr });
					node->addInput({ 0, "Value 2", nullptr });
					node->setOutput(new ShaderNodeAttribute(0, "Result", nullptr));

					functionNodes.push_back(node);
				}

				if (ImGui::MenuItem("Subtract"))
				{
					ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Subtract", "-", ShaderFunctionNode::ShaderNodeCategory::Color, ShaderFunction::ShaderFunctionOperation::Operation);
					node->addInput({ 0, "Value 1", nullptr });
					node->addInput({ 0, "Value 2", nullptr });
					node->setOutput(new ShaderNodeAttribute(0, "Result", nullptr));

					functionNodes.push_back(node);
				}

				if (ImGui::MenuItem("Multiply"))
				{
					ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Multiply", "*", ShaderFunctionNode::ShaderNodeCategory::Color, ShaderFunction::ShaderFunctionOperation::Operation);
					node->addInput({ 0, "Value 1", nullptr });
					node->addInput({ 0, "Value 2", nullptr });
					node->setOutput(new ShaderNodeAttribute(0, "Result", nullptr));

					functionNodes.push_back(node);
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Sin"))
				{
					ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Sin", "sin", ShaderFunctionNode::ShaderNodeCategory::Color, ShaderFunction::ShaderFunctionOperation::FunctionCall);
					node->addInput({ 0, "Value", nullptr });
					node->setOutput(new ShaderNodeAttribute(0, "Result", nullptr));

					functionNodes.push_back(node);
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Normalize"))
				{
					ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Normalize", "normalize", ShaderFunctionNode::ShaderNodeCategory::Color, ShaderFunction::ShaderFunctionOperation::FunctionCall);
					node->addInput({ 0, "Value", nullptr });
					node->setOutput(new ShaderNodeAttribute(0, "Result", nullptr));

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


	void createLink(int startAttributeId, int endAttributeId)
	{
		if (isInputAttributeLinked(endAttributeId)) return;

		auto startNode = getNodeByAttribId(startAttributeId);
		auto endNode = getNodeByAttribId(endAttributeId);

		auto startAttribute = startNode->getAttributeById(startAttributeId);
		auto endAttribute = endNode->getAttributeById(endAttributeId);

		if (startNode->getOutput() != startAttribute)
		{
			auto temp = endAttribute;

			endAttribute = startAttribute;
			startAttribute = temp;
		}

		if (!isLinkValid(startAttribute, endAttribute)) return;

		if (endNode->getType() == ShaderVarNode::ShaderVarNodeType::Function)
		{
			static_cast<ShaderFunctionNode*>(endNode)->setFunctionType(startAttribute->type);
		}

		startAttribute->connectedTo = endAttribute;
		endAttribute->connectedTo = startAttribute;

		links.push_back({ startAttribute, endAttribute });
	}

	bool isInputAttributeLinked(int attributeId)
	{
		for (auto& link : links)
		{
			if (link.second->id == attributeId) return true;
		}

		return false;
	}

	bool isLinkValid(ShaderNodeAttribute* start, ShaderNodeAttribute* end)
	{
		return start && end && (start->type == end->type || end->type == 0);
	}


	void compileNodeTree()
	{
		ShaderVarNode* outputNode = nullptr;

		for (auto& node : varNodes)
		{
			if (node->category == ShaderVarNode::ShaderNodeCategory::Output && node->name == "Output")
			{
				outputNode = node;
				break;
			}
		}

		assert(outputNode);

		std::string topCode;
		std::string mainBody;
		auto shaderCodes = outputNode->getShaderCode();
		
		// sort out double compiled nodes (just in case(uniform!!!))
		for (int i = 0; i < shaderCodes.size(); i++)
		{
			for (int j = 0; j < shaderCodes.size(); j++)
			{
				if (i != j && shaderCodes[i] == shaderCodes[j])
				{
					shaderCodes.erase(shaderCodes.begin() + j);
				}
			}
		}

		for (auto& node : shaderCodes)
		{
			if (node.first == ShaderVarNode::ShaderVarNodeType::Uniform || node.second.substr(0, 3) == "out")
			{
				topCode += node.second;
			}
			else
			{
				mainBody += node.second;
			}
		}

		mainBody += std::format("\n{} = {};", outputNode->getShaderVar()->getVariableName(), outputNode->getInputs()[0].connectedTo->node->getShaderVar()->getVariableName());

		std::string code = std::string("#version 460 core\n\n")
			+ topCode
			+ "\n"
			+ "void main()\n"
			+ "{\n"
			+ mainBody
			+ "\n}";


		std::cout << code << std::endl;

		Material* material = new Material("test-material", new Shader(".\\Vertex\\v_debug.glsl", GL_VERTEX_SHADER), new Shader(code, GL_FRAGMENT_SHADER, false));
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

	int getNextNodeId(int correction = 0)
	{
		int id = uniformNodes.size() + functionNodes.size() + varNodes.size() + correction;

		if (checkNodeIdExists(id))
		{
			return getNextNodeId(1);
		}
		else
		{
			return id;
		}
	}

	bool checkNodeIdExists(int id)
	{
		for (auto& node : varNodes)
		{
			if (node->id == id) return true;
		}
		for (auto& node : uniformNodes)
		{
			if (node->id == id) return true;
		}
		for (auto& node : functionNodes)
		{
			if (node->id == id) return true;
		}

		return false;
	}


	void deleteNodeById(int nodeId)
	{
		for (int i = 0; i < uniformNodes.size(); i++)
		{
			ShaderVarNode* node = uniformNodes[i];
			if (nodeId == node->getId())
			{
				uniformNodes.erase(uniformNodes.begin() + i);
				deleteLinksByNode(node);
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
				deleteLinksByNode(node);
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
				deleteLinksByNode(node);
				delete node;
				return;
			}
		}
	}

	void deleteLinkById(int id)
	{
		std::pair<ShaderNodeAttribute*, ShaderNodeAttribute*> link = links[id];
		links.erase(links.begin() + id);

		link.second->connectedTo = nullptr;
		link.first->connectedTo = nullptr;

		bool secondNodeConnected = false;
		for (auto& val : links)
		{
			if (val.second->node->id == link.second->node->id)
			{
				secondNodeConnected = true;
				break;
			}
		}	

		if (!secondNodeConnected && !link.second->immutable)
		{
			auto node = link.second->node;
			for (auto& input : node->getInputs())
			{
				input.type = 0;
			}
		}
	}

	void deleteLinkByAttribId(int attribId)
	{
		for (int i = 0; i < links.size(); i++)
		{
			if (links[i].first->id == attribId || links[i].second->id == attribId)
			{
				deleteLinkById(i);
				return;
			}
		}
	}

	void deleteLinksByNode(ShaderVarNode* node)
	{
		for (int i = 0; i < links.size(); i++)
		{
			bool match = node->getOutput()->id == links[i].first->id;

			for (auto& input : node->getInputs())
			{
				if (input.id == links[i].second->id)
				{
					match = true;
					continue;
				}
			}

			if (match)
			{
				deleteLinkById(i);
				i--;
			}
		}
	}


	void ColoredText(const char* text, ImU32 color)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, color);
		ImGui::Text(text);
		ImGui::PopStyleColor();
	}

	std::vector<ShaderUniformNode*> uniformNodes;
	std::vector<ShaderFunctionNode*> functionNodes;
	std::vector<ShaderVarNode*> varNodes;
	std::vector<std::pair<ShaderNodeAttribute*, ShaderNodeAttribute*>> links;	// std::pair<startId, endId>

};