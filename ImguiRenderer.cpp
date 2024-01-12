#include "ImguiRenderer.hpp"


void ImguiRenderer::renderMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Objects"))
		{
            if (ImGui::MenuItem("Square"))
            {
                auto value = RendererPipeline::getRendererMap().find(RendererType::Object)->second;
                if (value != nullptr)
                {
                    auto renderer = static_cast<ObjectRenderer*>(value);
                    SquareObject* obj = new SquareObject(std::format("Square{}", renderer->getObjects().size()), Primitives::getSquareVertices(), Primitives::getSquareIndices());

                    renderer->addObject(obj);
                    PhysicEngine::addObject(obj);
                }
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::Checkbox("Object Manager", &m_showObjectManager);


			ImGui::EndMenu();
		}
        else if (ImGui::BeginMenu("View"))
        {
            ImGui::Checkbox("Show Debug Info", &m_showDebugInfo);


            ImGui::EndMenu();
        }

		ImGui::EndMainMenuBar();
	}
}

void ImguiRenderer::renderDebugInfo()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec2 windowPadding = style.WindowPadding;

    style.WindowRounding = 0.0f;
    style.WindowPadding = ImVec2(3.0f, 3.0f);

    ImGui::SetNextWindowPos(ImVec2(0.0f, 18.0f));
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 20.0f));

    ImGui::Begin("FPS Bar", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs);
    {
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    }
    ImGui::End();

    style.WindowRounding = 5.0f;
    style.WindowPadding = windowPadding;
}

void ImguiRenderer::renderObjectManager()
{
    ImGui::Begin("Object Manager");
    {
        ImGui::BeginChild("Objects", ImVec2(200, 0), true);
        {
            if (ImGui::TreeNode("Object Renderer"))
            {
                auto value = RendererPipeline::getRendererMap().find(RendererType::Object)->second;
                if (value != nullptr)
                {
                    auto renderer = static_cast<ObjectRenderer*>(value);

                    for (size_t i = 0; i < renderer->getObjects().size(); i++)
                    {
                        auto& obj = renderer->getObjects()[i];

                        ImGui::Selectable(obj->getName().c_str(), selectedObjectIndex == i);
                        if (ImGui::IsItemClicked())
                        {
                            selectedObjectIndex = i;
                        }
                    }
                }

                ImGui::TreePop();
            }
        }
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("Detail View", ImVec2(0, 0), true);
        {
            auto value = RendererPipeline::getRendererMap().find(RendererType::Object)->second;
            if (value != nullptr)
            {
                auto renderer = static_cast<ObjectRenderer*>(value);
                auto& objects = renderer->getObjects();

                if (selectedObjectIndex >= 0 && selectedObjectIndex < objects.size())
                {
                    GameObject* selObj = objects[selectedObjectIndex];
                    PhysicalizedGameObject* phObj = PhysicEngine::getObject(selObj);

                    float *pos[3] = { &selObj->getPositionPtr()->x, &selObj->getPositionPtr()->y, &selObj->getPositionPtr()->z};
                    float *scale[3] = { &selObj->getScalePtr()->x, &selObj->getScalePtr()->y, &selObj->getScalePtr()->z};

                    ImGui::Text("Selected Object: %s", selObj->getName().c_str());
                    ImGui::Separator();

                    ImGui::Text("Position");
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX());
                    ImGui::InputFloat3("##0", *pos);

                    ImGui::Text("Scale");
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX());
                    ImGui::InputFloat3("##1", *scale);

                    if (phObj != nullptr)
                    {
                        float *vel[3] = { &phObj->getVelocityPtr()->x, &phObj->getVelocityPtr()->y, &phObj->getVelocityPtr()->z };

                        ImGui::Text("Velocity");
                        ImGui::SameLine();
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX());
                        ImGui::InputFloat3("##2", *vel);

                        ImGui::Text("Linear Drag");
                        ImGui::SameLine();
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX());
                        ImGui::InputFloat("##3", phObj->getLinearDragPtr());

                        ImGui::Checkbox("Gravity", phObj->getIsGravityEnabledPtr());

                        ImGui::Checkbox("Draw Collision Box", phObj->getIsDrawCollisionBoxPtr());
                    }

                }
            }
        }
        ImGui::EndChild();

    }
    ImGui::End();
}



void ImguiRenderer::setImguiStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();

    // Colors
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.24f, 0.29f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.44f, 0.75f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.20f, 0.44f, 0.75f, 0.31f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 0.60f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.70f, 0.70f, 0.70f, 0.80f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.50f, 0.50f, 0.50f, 0.60f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.70f, 0.70f, 0.70f, 0.80f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.20f, 0.44f, 0.75f, 0.86f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.20f, 0.44f, 0.75f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.24f, 0.52f, 0.88f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

    // Adjust other settings
    style.WindowMinSize = ImVec2(1.0f, 1.0f);
    style.WindowRounding = 5.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 3.0f;
    style.ScrollbarRounding = 2.0f;
}