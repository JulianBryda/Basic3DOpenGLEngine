#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <variant>
#include <functional>
#include <optional>
#include <format>
#include <thread>
#include "../../../../ThirdParty/ImNodes/imnodes.h"
#include "../../../Data/ShaderNode/ShaderVar.hpp"
#include "../../../../Globals/GlobalTextures.hpp"

class ShaderVarNode
{
public:

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

	ShaderVarNode(int id, std::string name, GLint outputType, ShaderNodeCategory category, ShaderVar::ShaderVarPrefix prefix = ShaderVar::None)
	{
		this->id = id;
		this->name = name;
		this->category = category;
		this->type = ShaderVarNodeType::Var;

		this->output = nullptr;
		this->shaderVar = new ShaderVar(id, outputType, prefix);

		ImNodes::SetNodeScreenSpacePos(id, ImGui::GetWindowPos());
	}

	~ShaderVarNode()
	{
		delete shaderVar;
		delete output;
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

			for (auto& input : inputs)
			{
				if (!input.hide) ImNodes::BeginInputAttribute(input.id);
				{
					ImGui::TextUnformatted(input.name);
					ImGui::SameLine();
					renderInput(input);
				}
				if (!input.hide) ImNodes::EndInputAttribute();
			}

			if (output != nullptr)
			{
				ImNodes::BeginOutputAttribute(output->id);
				{
					ImGui::TextUnformatted(output->name);
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

	void renderInput(ShaderNodeAttribute& input)
	{
		if (input.value == nullptr || input.connectedTo != nullptr) return;

		switch (input.type)
		{
		case GL_INT:
			renderInt(std::format("##{}{}", input.name, input.id).c_str(), static_cast<int*>(input.value));
			break;
		case GL_FLOAT:
			renderFloat(std::format("##{}{}", input.name, input.id).c_str(), static_cast<float*>(input.value));
			break;
		case GL_SAMPLER_2D:
			renderSampler2D(std::format("##{}{}", input.name, input.id).c_str(), input.value);
			break;
		default:
			break;
		}

	}

	void renderInt(const char* name, int* value)
	{
		ImGui::SetNextItemWidth(80.f);
		ImGui::DragInt(name, value);
	}

	void renderFloat(const char* name, float* value)
	{
		ImGui::SetNextItemWidth(80.f);
		ImGui::DragFloat(name, value, 0.1f);
	}

	char buffer[_MAX_PATH]{ 0 };

	void renderSampler2D(const char* name, void* value)
	{
		ImGui::SetNextItemWidth(200.f);
		ImGui::InputText("##25897", buffer, _MAX_PATH);
		ImGui::SameLine();
		if (ImGui::Button("Select"))
		{
			openFileDialog(buffer);

			Textures::loadTexture(buffer, static_cast<GLuint*>(value));
			this->output->value = value;
		}

		ImGui::Image((void*)(intptr_t) * static_cast<GLuint*>(value), { 100, 100 });
	}

	int getId() const
	{
		return id;
	}

	std::string getName() const
	{
		return name;
	}

	ShaderNodeCategory getCategory() const
	{
		return category;
	}

	ShaderVarNodeType getType() const
	{
		return type;
	}

	ShaderNodeAttribute* getAttributeById(int id)
	{
		if (output && output->id == id)
		{
			return output;
		}

		for (auto& input : inputs)
		{
			if (input.id == id)
			{
				return &input;
			}
		}

		return nullptr;
	}

	std::vector<std::pair<ShaderVarNode*, std::string>> getShaderCode()
	{
		std::vector<std::pair<ShaderVarNode*, std::string>> output;

		for (auto& input : inputs)
		{
			if (input.connectedTo == nullptr) continue;

			for (auto& shaderCode : input.connectedTo->node->getShaderCode())
			{
				output.push_back({ shaderCode.first, shaderCode.second });
			}
		}

		output.push_back({ this, this->shaderVar->getShaderCode(inputs) });

		return output;
	}

	std::string getTypeName()
	{
		switch (type)
		{
		case ShaderVarNodeType::Var:
			return "var";
		case ShaderVarNodeType::Uniform:
			return "uniform";
		case ShaderVarNodeType::Function:
			return "func";
		default:
			return "Unknown";
		}
	}

	void addInput(ShaderNodeAttribute attribute)
	{
		attribute.id = id * 500 + inputs.size() + 2;
		attribute.node = this;
		inputs.push_back(attribute);
	}

	void setOutput(ShaderNodeAttribute* attribute)
	{
		assert(attribute != nullptr);

		attribute->id = id * 500 + 1;
		attribute->node = this;
		output = attribute;
	}

	ShaderNodeAttribute* getOutput() const
	{
		return output;
	}

	std::vector<ShaderNodeAttribute>& getInputs()
	{
		return inputs;
	}

	ShaderVar* getShaderVar() const
	{
		return shaderVar;
	}

	int id;
	std::string name;
	ShaderNodeCategory category;

	ShaderVar* shaderVar;

protected:

	ShaderVarNode(int id, std::string name, ShaderNodeCategory category)
	{
		this->id = id;
		this->name = name;
		this->category = category;

		this->output = nullptr;
		this->shaderVar = nullptr;
		this->type = ShaderVarNode::ShaderVarNodeType::Var;

		ImNodes::SetNodeScreenSpacePos(id, ImGui::GetWindowPos());
	}

	ShaderVarNodeType type;

	std::vector<ShaderNodeAttribute> inputs;
	ShaderNodeAttribute* output;

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

	void openFileDialog(char* buffer);
};