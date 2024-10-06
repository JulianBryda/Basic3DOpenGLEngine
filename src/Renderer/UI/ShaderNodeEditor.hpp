#pragma once
#include "imgui.h"
#include "../../ThirdParty/ImNodes/imnodes.h"

class ShaderNodeEditor
{

public:

	ShaderNodeEditor()
	{
		ImNodes::CreateContext();
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
			ImNodes::EndNodeEditor();
		}
		ImGui::End();
	}


private:

};