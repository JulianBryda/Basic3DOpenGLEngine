#pragma once
#include <format>
#include <filesystem>

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"

#include "Renderer.hpp"
#include "RendererPipeline.hpp"
#include "ObjectRenderer.hpp"
#include "EnvironmentRenderer.hpp"
#include "Primitives.hpp"
#include "PhysicEngine.hpp"

class ImguiRenderer : public Renderer
{

public:

	ImguiRenderer(GLFWwindow* window) : Renderer(RendererType::UserInterface)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		setImguiStyle();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();
	}

	~ImguiRenderer()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void render(Camera* activeCamera) override
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		// ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

		// render functions here
		renderMenuBar();
		if (m_showDebugInfo) renderDebugInfo();
		if (m_showObjectManager) renderObjectManager();
		if (showLightManager) renderLightManager();


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void renderFolderStructure(const char* path)
	{
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			if (std::filesystem::is_directory(entry.status()))
			{
				if (ImGui::TreeNode(entry.path().filename().string().c_str()))
				{
					renderFolderStructure(entry.path().string().c_str());

					ImGui::TreePop();
				}
			}
			else
			{
				ImGui::Selectable(entry.path().filename().string().c_str(), !std::strcmp(selectedTexturePath, entry.path().string().c_str()));
				if (ImGui::IsItemClicked())
				{
					std::strcpy(selectedTexturePath, entry.path().string().c_str());
				}
			}
		}
	}

	// static

	static void setSelectedObject(GameObject* object)
	{
		selectedObject = object;
	}

private:

	void setImguiStyle();

	void renderMenuBar();
	void renderDebugInfo();
	void renderObjectManager();
	void renderLightManager();
	void renderLightMaterialView();

	char selectedTexturePath[_MAX_PATH] = "";

	bool m_showDebugInfo = false,
		m_showObjectManager = false,
		highlightCloseCollidableObjects = false,
		showLightManager = false;

	Light* selectedLight;

	// static
	static GameObject* selectedObject;
};