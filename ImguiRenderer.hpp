#pragma once
#include <format>
#include <filesystem>

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"

#include "RendererBase.hpp"
#include "RendererManager.hpp"
#include "ObjectRenderer.hpp"
#include "Primitives.hpp"
#include "PhysicEngine.hpp"
#include "AssetManager.hpp"
#include "IconsFontAwesome5.h"

#include "PointLight.hpp"
#include "SpotLight.hpp"	

class ImguiRenderer : public RendererBase
{

public:

	ImguiRenderer(GLFWwindow* window) : RendererBase(RendererType::UserInterface)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		io.Fonts->AddFontDefault();
		float baseFontSize = 13.0f;
		float iconFontSize = baseFontSize * 2.0f / 3.0f;

		// merge in icons from Font Awesome
		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;
		icons_config.GlyphMinAdvanceX = iconFontSize;
		io.Fonts->AddFontFromFileTTF(".\\Assets\\Fonts\\fa-solid-900.ttf", iconFontSize, &icons_config, icons_ranges);
		// use FONT_ICON_FILE_NAME_FAR if you want regular instead of solid

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

	void render(Scene* activeScene) override
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

		// render functions here
		renderMenuBar();
		if (m_showDebugInfo) renderDebugInfo();
		if (m_showObjectManager) renderObjectManager();
		if (m_showLightManager) renderLightManager();
		if (m_showAssetManager) renderAssetManager();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void renderFolderStructure(const char* path)
	{
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			if (entry.is_directory())
			{
				if (ImGui::TreeNode(entry.path().filename().string().c_str()))
				{
					renderFolderStructure(entry.path().string().c_str());

					ImGui::TreePop();
				}
			}
			else
			{
				if (ImGui::Selectable(entry.path().filename().string().c_str(), !std::strcmp(m_selectedTexturePath, entry.path().string().c_str())))
				{
					std::strcpy(m_selectedTexturePath, entry.path().string().c_str());
				}
			}
		}
	}

	void renderAssetFolderStructure(const char* path)
	{
		renderAssetFolderStructure(std::filesystem::directory_iterator(path));
	}

	void renderAssetFolderStructure(const std::filesystem::directory_entry entry)
	{
		renderAssetFolderStructure(std::filesystem::directory_iterator(entry));
	}

	void renderAssetFolderStructure(std::filesystem::directory_iterator iterator)
	{
		for (const auto& entry : iterator)
		{
			if (entry.is_directory())
			{
				bool subDirectories = false;

				for (const auto& subEntry : std::filesystem::directory_iterator(entry))
				{
					if (subEntry.is_directory())
					{
						subDirectories = true;
						break;
					}
				}

				if (subDirectories)
				{
					if (ImGui::TreeNode(entry.path().filename().string().c_str()))
					{
						if (ImGui::IsItemClicked())
						{
							m_selectedAssetPath = entry.path().string();
						}

						renderAssetFolderStructure(entry);

						ImGui::TreePop();
					}
				}
				else
				{
					if (ImGui::Selectable(entry.path().filename().string().c_str(), false))
					{
						m_selectedAssetPath = entry.path().string();
					}
				}
			}
		}
	}

	// static

	static void setSelectedObject(GameObject* object)
	{
		if (m_selectedObject != nullptr)
		{
			m_selectedObject->setIsOutline(false);
		}

		if (object != nullptr)
		{
			object->setIsOutline(true);
		}

		m_selectedObject = object;
	}

	static GameObject* getSelectedObject()
	{
		return m_selectedObject;
	}

private:

	void setImguiStyle();

	void renderMenuBar();
	void renderDebugInfo();

	void renderObjectManager();
	void renderLightManager();
	void renderAssetManager();

	void renderLightMaterialView();

	// custom components 
	bool IconItem(int id, const char* text, GLuint imageId, const float itemSize);

	char m_selectedTexturePath[_MAX_PATH] = "";
	std::string m_selectedAssetPath = "";

	bool m_showDebugInfo = false,
		m_showObjectManager = false,
		m_highlightCloseCollidableObjects = false,
		m_showLightManager = false,
		m_showAssetManager = false;

	Light* m_selectedLight;

	// static
	static GameObject* m_selectedObject;
};