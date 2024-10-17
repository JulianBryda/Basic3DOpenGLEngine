#pragma once
#include <algorithm>

#include "../../ThirdParty/ImNodes/imnodes.h"
#include "ShaderNodeEditor/ShaderNodes/ShaderVarNode.hpp"
#include "ShaderNodeEditor/ShaderNodes/ShaderUniformNode.hpp"
#include "ShaderNodeEditor/ShaderNodes/ShaderFunctionNode.hpp"
#include "ShaderNodeEditor/ShaderNodeLink.hpp"

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

				ImGui::SetNextItemWidth(300.f);
				if (ImGui::BeginCombo("Material", selectedMaterial ? selectedMaterial->getName().c_str() : "--Selected Material--"))
				{
					for (auto& material : MaterialLib::g_materials)
					{
						if (!material.second->getNodeBased()) continue;

						bool isSelected = selectedMaterial == material.second;
						if (ImGui::Selectable(material.second->getName().c_str(), isSelected))
							selectedMaterial = material.second;
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}
			}
			ImGui::EndMenuBar();

			ImGui::BeginChild("Node Editor", { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y - 150 });
			{
				ImNodes::BeginNodeEditor();
				{
					renderNodePopup();

					renderNodes();
					renderLinks();
				}
				ImNodes::EndNodeEditor();
			}
			ImGui::EndChild();

			ImGui::BeginChild("Node Editor Output");
			{
				ImGui::TextUnformatted(outputText.c_str());
			}
			ImGui::EndChild();
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

			ImNodes::PushColorStyle(ImNodesCol_Link, link->getFirst()->color);
			ImNodes::PushColorStyle(ImNodesCol_LinkHovered, link->getFirst()->color + IM_COL32(0, 0, 0, 75));
			ImNodes::PushColorStyle(ImNodesCol_LinkSelected, link->getFirst()->color + IM_COL32(0, 0, 0, 75));
			ImNodes::Link(i, link->getFirst()->id, link->getLast()->id);
			ImNodes::PopColorStyle();
			ImNodes::PopColorStyle();
			ImNodes::PopColorStyle();
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
					ShaderVarNode* node = new ShaderVarNode(getNextNodeId(), "Int", ShaderEnums::ShaderNodeCategory::Input, ShaderEnums::None);
					node->addInput(ShaderNodeAttribute(ShaderEnums::INT, "", value, ShaderEnums::AttributeHidden));
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::INT, "Value", value));

					varNodes.push_back(node);
				}
				if (ImGui::MenuItem("Float"))
				{
					float* value = new float(0);
					ShaderVarNode* node = new ShaderVarNode(getNextNodeId(), "Float", ShaderEnums::ShaderNodeCategory::Input, ShaderEnums::None);
					node->addInput(ShaderNodeAttribute(ShaderEnums::FLOAT, "", value, ShaderEnums::AttributeHidden));
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::FLOAT, "Value", value));

					varNodes.push_back(node);
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Vec2"))
				{
					glm::vec2* value = new glm::vec2();
					ShaderVarNode* node = new ShaderVarNode(getNextNodeId(), "Vec2", ShaderEnums::ShaderNodeCategory::Input, ShaderEnums::None);
					node->addInput(ShaderNodeAttribute(ShaderEnums::FLOAT, "X", &value->x));
					node->addInput(ShaderNodeAttribute(ShaderEnums::FLOAT, "Y", &value->y));
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::VEC_2, "Value", value));

					varNodes.push_back(node);
				}
				if (ImGui::MenuItem("Vec3"))
				{
					glm::vec3* value = new glm::vec3();
					ShaderVarNode* node = new ShaderVarNode(getNextNodeId(), "Vec3", ShaderEnums::ShaderNodeCategory::Input, ShaderEnums::None);
					node->addInput(ShaderNodeAttribute(ShaderEnums::FLOAT, "X", &value->x));
					node->addInput(ShaderNodeAttribute(ShaderEnums::FLOAT, "Y", &value->y));
					node->addInput(ShaderNodeAttribute(ShaderEnums::FLOAT, "Z", &value->z));
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::VEC_3, "Value", value));

					varNodes.push_back(node);
				}
				if (ImGui::MenuItem("Vec4"))
				{
					glm::vec4* value = new glm::vec4();
					ShaderVarNode* node = new ShaderVarNode(getNextNodeId(), "Vec4", ShaderEnums::ShaderNodeCategory::Input, ShaderEnums::None);
					node->addInput(ShaderNodeAttribute(ShaderEnums::FLOAT, "X", &value->x));
					node->addInput(ShaderNodeAttribute(ShaderEnums::FLOAT, "Y", &value->y));
					node->addInput(ShaderNodeAttribute(ShaderEnums::FLOAT, "Z", &value->z));
					node->addInput(ShaderNodeAttribute(ShaderEnums::FLOAT, "A", &value->a));
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::VEC_4, "Value", value));

					varNodes.push_back(node);
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Position"))
				{
					ShaderVarNode* node = new ShaderVarNode(getNextNodeId(), "Position", ShaderEnums::ShaderNodeCategory::Input, ShaderEnums::In);
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::VEC_3, "Value", nullptr));
					node->getShaderVar()->variableName = "fragPos";

					varNodes.push_back(node);
				}
				if (ImGui::MenuItem("Normal"))
				{
					ShaderVarNode* node = new ShaderVarNode(getNextNodeId(), "Normal", ShaderEnums::ShaderNodeCategory::Input, ShaderEnums::In);
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::VEC_3, "Value", nullptr));
					node->getShaderVar()->variableName = "fragNorm";

					varNodes.push_back(node);
				}
				if (ImGui::MenuItem("Uv"))
				{
					ShaderVarNode* node = new ShaderVarNode(getNextNodeId(), "Uv", ShaderEnums::ShaderNodeCategory::Input, ShaderEnums::In);
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::VEC_2, "Value", nullptr));
					node->getShaderVar()->variableName = "fragUv";

					varNodes.push_back(node);
				}

				ImGui::Separator();

				if (ImGui::MenuItem("ViewPos"))
				{
					ShaderUniformNode* node = new ShaderUniformNode(getNextNodeId(), "ViewPos", ShaderEnums::ShaderNodeCategory::Input, 1);
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::VEC_3, "ViewPos", nullptr));

					uniformNodes.push_back(node);
				}

				if (ImGui::MenuItem("Time"))
				{
					ShaderUniformNode* node = new ShaderUniformNode(getNextNodeId(), "Time", ShaderEnums::ShaderNodeCategory::Input, 1);
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::FLOAT, "Time", nullptr));

					uniformNodes.push_back(node);
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Lights"))
				{
					ShaderUniformNode* node = new ShaderUniformNode(getNextNodeId(), "Lights", ShaderEnums::ShaderNodeCategory::Input, 10);
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::LIGHT_ARRAY, "Lights", nullptr));

					uniformNodes.push_back(node);
				}

				if (ImGui::MenuItem("LightCount"))
				{
					ShaderUniformNode* node = new ShaderUniformNode(getNextNodeId(), "LightCount", ShaderEnums::ShaderNodeCategory::Input, 1);
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::INT, "LightCount", nullptr));

					uniformNodes.push_back(node);
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Output"))
			{
				if (ImGui::MenuItem("Output"))
				{
					ShaderVarNode* node = new ShaderVarNode(getNextNodeId(), "Output", ShaderEnums::ShaderNodeCategory::Output, ShaderEnums::Out);
					node->addInput({ ShaderEnums::VEC_4, "Output", nullptr });
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::VEC_4, "Output", nullptr, ShaderEnums::Hidden));

					varNodes.push_back(node);
				}

				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::BeginMenu("Converter"))
			{
				if (ImGui::MenuItem("Vec2To3"))
				{
					ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Vec2To3", "vec3", ShaderEnums::ShaderNodeCategory::Converter, ShaderFunction::ShaderFunctionOperation::FunctionCall);
					node->addInput({ ShaderEnums::VEC_2, "RG", nullptr });
					node->addInput({ ShaderEnums::FLOAT, "B", nullptr });
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::VEC_3, "Value", nullptr));

					functionNodes.push_back(node);
				}

				if (ImGui::MenuItem("Vec3To4"))
				{
					ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Vec3To4", "vec4", ShaderEnums::ShaderNodeCategory::Converter, ShaderFunction::ShaderFunctionOperation::FunctionCall);
					node->addInput({ ShaderEnums::VEC_3, "RGB", nullptr });
					node->addInput({ ShaderEnums::FLOAT, "A", nullptr });
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::VEC_4, "Value", nullptr));

					functionNodes.push_back(node);
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Vec4To3"))
				{
					ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Vec4To3", "vec3", ShaderEnums::ShaderNodeCategory::Converter, ShaderFunction::ShaderFunctionOperation::FunctionCall);
					node->addInput({ ShaderEnums::VEC_4, "Value", nullptr });
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::VEC_3, "Value", nullptr));

					functionNodes.push_back(node);
				}

				if (ImGui::MenuItem("Vec3To2"))
				{
					ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Vec3To2", "vec2", ShaderEnums::ShaderNodeCategory::Converter, ShaderFunction::ShaderFunctionOperation::FunctionCall);
					node->addInput({ ShaderEnums::VEC_3, "Value", nullptr });
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::VEC_2, "Value", nullptr));

					functionNodes.push_back(node);
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Color"))
			{
				if (ImGui::MenuItem("Mix Color"))
				{
					ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Mix", "mix", ShaderEnums::ShaderNodeCategory::Color, ShaderFunction::ShaderFunctionOperation::FunctionCall);
					node->addInput({ ShaderEnums::NONE, "Value 1", nullptr });
					node->addInput({ ShaderEnums::NONE, "Value 2", nullptr });
					node->addInput({ ShaderEnums::FLOAT, "Blend", nullptr });
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::NONE, "Result", nullptr));

					functionNodes.push_back(node);
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Math"))
			{
				if (ImGui::MenuItem("Add"))
				{
					ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Add", "+", ShaderEnums::ShaderNodeCategory::Color, ShaderFunction::ShaderFunctionOperation::Operation);
					node->addInput({ ShaderEnums::NONE, "Value 1", nullptr });
					node->addInput({ ShaderEnums::NONE, "Value 2", nullptr });
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::NONE, "Result", nullptr));

					functionNodes.push_back(node);
				}

				if (ImGui::MenuItem("Subtract"))
				{
					ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Subtract", "-", ShaderEnums::ShaderNodeCategory::Color, ShaderFunction::ShaderFunctionOperation::Operation);
					node->addInput({ ShaderEnums::NONE, "Value 1", nullptr });
					node->addInput({ ShaderEnums::NONE, "Value 2", nullptr });
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::NONE, "Result", nullptr));

					functionNodes.push_back(node);
				}

				if (ImGui::MenuItem("Multiply"))
				{
					ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Multiply", "*", ShaderEnums::ShaderNodeCategory::Color, ShaderFunction::ShaderFunctionOperation::Operation);
					node->addInput({ ShaderEnums::NONE, "Value 1", nullptr });
					node->addInput({ ShaderEnums::NONE, "Value 2", nullptr });
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::NONE, "Result", nullptr));

					functionNodes.push_back(node);
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Sin"))
				{
					ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Sin", "sin", ShaderEnums::ShaderNodeCategory::Color, ShaderFunction::ShaderFunctionOperation::FunctionCall);
					node->addInput({ ShaderEnums::NONE, "Value", nullptr });
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::NONE, "Result", nullptr));

					functionNodes.push_back(node);
				}
				if (ImGui::MenuItem("Cos"))
				{
					ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Cos", "cos", ShaderEnums::ShaderNodeCategory::Color, ShaderFunction::ShaderFunctionOperation::FunctionCall);
					node->addInput({ ShaderEnums::NONE, "Value", nullptr });
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::NONE, "Result", nullptr));

					functionNodes.push_back(node);
				}
				if (ImGui::MenuItem("Tan"))
				{
					ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Tan", "tan", ShaderEnums::ShaderNodeCategory::Color, ShaderFunction::ShaderFunctionOperation::FunctionCall);
					node->addInput({ ShaderEnums::NONE, "Value", nullptr });
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::NONE, "Result", nullptr));

					functionNodes.push_back(node);
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Min"))
				{
					ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Min", "min", ShaderEnums::ShaderNodeCategory::Color, ShaderFunction::ShaderFunctionOperation::FunctionCall);
					node->addInput({ ShaderEnums::NONE, "Value 1", nullptr });
					node->addInput({ ShaderEnums::NONE, "Value 2", nullptr });
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::NONE, "Result", nullptr));

					functionNodes.push_back(node);
				}
				if (ImGui::MenuItem("Max"))
				{
					ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Max", "max", ShaderEnums::ShaderNodeCategory::Color, ShaderFunction::ShaderFunctionOperation::FunctionCall);
					node->addInput({ ShaderEnums::NONE, "Value 1", nullptr });
					node->addInput({ ShaderEnums::NONE, "Value 2", nullptr });
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::NONE, "Result", nullptr));

					functionNodes.push_back(node);
				}
				if (ImGui::MenuItem("Dot"))
				{
					ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Dot", "dot", ShaderEnums::ShaderNodeCategory::Color, ShaderFunction::ShaderFunctionOperation::FunctionCall);
					node->addInput({ ShaderEnums::NONE, "Value 1", nullptr });
					node->addInput({ ShaderEnums::NONE, "Value 2", nullptr });
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::FLOAT, "Result", nullptr));

					functionNodes.push_back(node);
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Normalize"))
				{
					ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Normalize", "normalize", ShaderEnums::ShaderNodeCategory::Color, ShaderFunction::ShaderFunctionOperation::FunctionCall);
					node->addInput({ ShaderEnums::NONE, "Value", nullptr });
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::NONE, "Result", nullptr));

					functionNodes.push_back(node);
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Texture"))
			{
				if (ImGui::MenuItem("Sampler2D"))
				{
					GLuint* value = new GLuint();
					int nodeId = getNextNodeId();
					ShaderUniformNode* node = new ShaderUniformNode(nodeId, std::format("Sampler2D_{}", nodeId), ShaderEnums::ShaderNodeCategory::Texture, 1);
					node->addInput(ShaderNodeAttribute(ShaderEnums::SAMPLER_2D, "", value, ShaderEnums::AttributeHidden));
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::SAMPLER_2D, "Value", nullptr));

					uniformNodes.push_back(node);
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Texture"))
				{
					ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Texture", "texture", ShaderEnums::ShaderNodeCategory::Texture, ShaderFunction::ShaderFunctionOperation::FunctionCall);
					node->addInput({ ShaderEnums::SAMPLER_2D, "Sampler2D", nullptr });
					node->addInput({ ShaderEnums::VEC_2, "Uv", nullptr });
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::VEC_4, "Value", nullptr));

					functionNodes.push_back(node);
				}

				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::BeginMenu("Shader"))
			{
				if (ImGui::MenuItem("Lighting"))
				{
					ShaderFunctionNode* node = new ShaderFunctionNode(getNextNodeId(), "Lighting", "CalculateLighting", ShaderEnums::ShaderNodeCategory::Shader, ShaderFunction::ShaderFunctionOperation::FunctionCall);
					node->addInput({ ShaderEnums::LIGHT_ARRAY, "Lights", nullptr });
					node->addInput({ ShaderEnums::INT, "LightCount", nullptr });
					node->addInput({ ShaderEnums::VEC_3, "Position", nullptr });
					node->addInput({ ShaderEnums::VEC_3, "Normal", nullptr });
					node->addInput({ ShaderEnums::VEC_3, "ViewPos", nullptr });
					node->addInput({ ShaderEnums::VEC_3, "Ambient", nullptr });
					node->addInput({ ShaderEnums::VEC_3, "Diffuse", nullptr });
					node->addInput({ ShaderEnums::VEC_3, "Specular", nullptr });
					node->addInput({ ShaderEnums::FLOAT, "Shininess", nullptr });
					node->setOutput(new ShaderNodeAttribute(ShaderEnums::VEC_4, "Value", nullptr));
					node->setInclude("light.glsl");

					functionNodes.push_back(node);
				}

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

		if (endNode->getType() == ShaderEnums::ShaderVarNodeType::Function && !endAttribute->immutable)
		{
			static_cast<ShaderFunctionNode*>(endNode)->setFunctionType(startAttribute->getType());
		}

		startAttribute->connectedTo = endAttribute;
		endAttribute->connectedTo = startAttribute;

		links.push_back(new ShaderNodeLink(getNextLinkId(), startAttribute, endAttribute));
	}

	bool isInputAttributeLinked(int attributeId)
	{
		for (auto& link : links)
		{
			if (link->getLast()->id == attributeId) return true;
		}

		return false;
	}

	bool isLinkValid(ShaderNodeAttribute* start, ShaderNodeAttribute* end)
	{
		return start && end && (start->getType() & end->getType() || end->getType() & ShaderEnums::NONE);
	}


	void compileNodeTree()
	{
		ShaderVarNode* outputNode = nullptr;

		for (auto& node : varNodes)
		{
			if (node->category == ShaderEnums::ShaderNodeCategory::Output && node->name == "Output")
			{
				outputNode = node;
				break;
			}
		}

		if (!outputNode)
		{
			outputText += "Error: No Output Node found!\n";
			return;
		}
		if (!outputNode->getInputs()[0].connectedTo)
		{
			outputText += "Error: No Node connected to Output Node!\n";
			return;
		}

		std::string topCode;
		std::string mainBody;
		std::string includes;
		auto shaderCodes = outputNode->getShaderCode();

		// sort out double compiled nodes
		for (int i = 0; i < shaderCodes.size(); i++)
		{
			for (int j = 0; j < shaderCodes.size(); j++)
			{
				if (i != j && shaderCodes[i].second == shaderCodes[j].second)
				{
					shaderCodes.erase(shaderCodes.begin() + j);
				}
			}
		}

		for (auto& node : shaderCodes)
		{
			if (node.first->getType() == ShaderEnums::ShaderVarNodeType::Uniform || node.first->getShaderVar()->getVarPrefix() == ShaderEnums::Out || node.first->getShaderVar()->getVarPrefix() == ShaderEnums::In)
			{
				topCode += node.second;
			}
			else
			{
				mainBody += node.second;
			}

			std::string toInclude = std::format("#include \"{}\"\n", node.first->getShaderVar()->getInclude());
			if (node.first->getShaderVar()->getInclude() != "" && includes.find(toInclude) == std::string::npos)
			{
				includes += toInclude;
			}
		}

		mainBody += std::format("\n{} = {};", outputNode->getShaderVar()->variableName, outputNode->getInputs()[0].connectedTo->node->getShaderVar()->variableName);

		std::string code = std::string("#version 460 core\n")
			+ includes
			+ "\n"
			+ topCode
			+ "\n"
			+ "void main()\n"
			+ "{\n"
			+ mainBody
			+ "\n}";

		Material* material;
		if (selectedMaterial)
		{
			material = selectedMaterial;
			material->update(new Shader("\\Vertex\\v_material.glsl", GL_VERTEX_SHADER), new Shader(code, GL_FRAGMENT_SHADER, false));
		}
		else
		{
			material = new Material(std::format("Material-{}", MaterialLib::g_materials.size()), true, new Shader("\\Vertex\\v_material.glsl", GL_VERTEX_SHADER), new Shader(code, GL_FRAGMENT_SHADER, false));
			MaterialLib::addMaterial(material);
		}

		int index = 0;
		for (auto& node : shaderCodes)
		{
			if (node.first->getType() == ShaderEnums::ShaderVarNodeType::Uniform && node.first->getOutput()->getType() == ShaderEnums::SAMPLER_2D)
			{
				if (node.first->getOutput()->value == nullptr)
				{
					outputText += "Warning: No Texture selected for Sampler2D!\n";
					continue;
				}

				material->addTexture(Texture(*static_cast<GLuint*>(node.first->getOutput()->value), GL_TEXTURE_2D, GL_TEXTURE0 + index, node.first->getName()));
				index++;
			}

		}

		outputText += std::format("{} compiled successfully!\n", material->getName());
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

	int getNextLinkId(int correction = 0)
	{
		int id = links.size() + correction;

		if (checkLinkIdExists(id))
		{
			return getNextLinkId(1);
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

	bool checkLinkIdExists(int id)
	{
		for (auto& link : links)
		{
			if (link->getId() == id) return true;
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
		ShaderNodeLink* link = nullptr;
		for (int i = 0; i < links.size(); i++)
		{
			if (links[i]->getId() == id)
			{
				link = links[i];
				links.erase(links.begin() + i);
				break;
			}
		}

		if (!link) return;

		link->getLast()->connectedTo = nullptr;
		link->getFirst()->connectedTo = nullptr;

		bool secondNodeConnected = false;
		for (auto& val : links)
		{
			if (val->getLast()->node->id == link->getLast()->node->id)
			{
				secondNodeConnected = true;
				break;
			}
		}

		if (!secondNodeConnected && !link->getLast()->immutable)
		{
			auto node = link->getLast()->node;
			for (auto& input : node->getInputs())
			{
				input.setType(ShaderEnums::NONE);
			}
		}

		delete link;
	}

	void deleteLinksByNode(ShaderVarNode* node)
	{
		for (int i = 0; i < links.size(); i++)
		{
			bool match = node->getOutput()->id == links[i]->getFirst()->id;

			for (auto& input : node->getInputs())
			{
				if (input.id == links[i]->getLast()->id)
				{
					match = true;
					continue;
				}
			}

			if (match)
			{
				deleteLinkById(links[i]->getId());
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

	std::string outputText;
	Material* selectedMaterial;

	std::vector<ShaderUniformNode*> uniformNodes;
	std::vector<ShaderFunctionNode*> functionNodes;
	std::vector<ShaderVarNode*> varNodes;
	std::vector<ShaderNodeLink*> links;	// std::pair<startId, endId>

};