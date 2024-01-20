#pragma once
#include <format>

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"

#include "Renderer.hpp"
#include "RendererPipeline.hpp"
#include "ObjectRenderer.hpp"
#include "Primitives.hpp"

class ImguiRenderer : public Renderer
{

public:

	ImguiRenderer(RendererType type, GLFWwindow* window) : Renderer(type)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

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

	void render() override
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// render functions here
		renderMenuBar();
		if (m_showDebugInfo) renderDebugInfo();
		if (m_showObjectManager) renderObjectManager();


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

private:

	void setImguiStyle();

	void renderMenuBar();
	void renderDebugInfo();
	void renderObjectManager();


	int selectedObjectIndex = 0;

	bool m_showDebugInfo = false, m_showObjectManager = false;

};